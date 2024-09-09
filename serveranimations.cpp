#include "includes.h"

float smoothstep_bounds(float edge0, float edge1, float x)
{
	x = std::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3 - 2 * x);
}

void animstate_pose_param_cache_t::SetValue(Player* player, float flValue) {
}

int ServerAnimations::LookupSequence(Player* e, const char* label)
{
	if (!e || !label)
		return -1;

	typedef int(__thiscall* fnLookupSequence)(void*, const char*);
	static auto lookup_sequnece_adr = pattern::find(g_csgo.m_client_dll, XOR("E8 ? ? ? ? 5E 83 F8 FF")).rel32(1).as<fnLookupSequence>();

	return lookup_sequnece_adr(e, label);
}

Activity ServerAnimations::GetLayerActivity(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer) {
	if (!m_pAnimstate || !pLayer)
		return ACT_INVALID;

	return (Activity)m_pAnimstate->m_player->GetSequenceActivity(pLayer->m_sequence);
}


void ServerAnimations::IncrementLayerCycle(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer, bool bAllowLoop) {
	if (!pLayer || !m_pAnimstate)
		return;

	if (abs(pLayer->m_playback_rate) <= 0)
		return;

	float flCurrentCycle = pLayer->m_cycle;
	flCurrentCycle += m_pAnimstate->m_last_update_increment * pLayer->m_playback_rate;

	if (!bAllowLoop && flCurrentCycle >= 1)
	{
		flCurrentCycle = 0.999f;
	}

	pLayer->m_cycle = math::ClampCycle(flCurrentCycle);
}

void ServerAnimations::IncrementLayerWeight(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer) {
	if (!m_pAnimstate || !pLayer)
		return;

	if (abs(pLayer->m_weight_delta_rate) <= 0.f)
		return;

	float flCurrentWeight = pLayer->m_weight;
	flCurrentWeight += m_pAnimstate->m_last_update_increment * pLayer->m_weight_delta_rate;
	flCurrentWeight = std::clamp(flCurrentWeight, 0.f, 1.f);

	pLayer->m_weight = flCurrentWeight;
}


void ServerAnimations::UpdateAnimLayer(C_AnimationLayer* pLayer, int nSequence, float flPlaybackRate, float flWeight, float flCycle)
{
	if (nSequence > 1)
	{
		pLayer->m_sequence = nSequence;
		pLayer->m_playback_rate = flPlaybackRate;
		pLayer->m_cycle = std::clamp(flCycle, 0.f, 1.f);

		pLayer->m_weight = std::clamp(flWeight, 0.f, 1.f);
	}
}

mstudioseqdesc_t* SeqDesc(CStudioHdr* mdl, int seq) {
	if (!mdl || !seq)
		return nullptr;

	// fix this shit
	static auto target = pattern::find(g_csgo.m_client_dll, "E8 ? ? ? ? 03 40 04").rel32(1).as<mstudioseqdesc_t * (__thiscall*)(void*, int)>();
	return target(mdl, seq);
}

float ServerAnimations::GetLayerIdealWeightFromSeqCycle(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer) {
	if (!m_pAnimstate || !pLayer)
		return 0.f;

	auto model = m_pAnimstate->m_player->GetModelPtr();
	if (!model)
		return 0.f;

	auto seqdesc = SeqDesc(model, pLayer->m_sequence);
	if (!seqdesc)
		return 0.f;

	float flCycle = pLayer->m_cycle;
	if (flCycle >= 0.999f)
		flCycle = 1;

	float flEaseIn = seqdesc->fadeintime; // seqdesc.fadeintime;
	float flEaseOut = seqdesc->fadeouttime; // seqdesc.fadeouttime;
	float flIdealWeight = 1;

	if (flEaseIn > 0 && flCycle < flEaseIn)
	{
		flIdealWeight = smoothstep_bounds(0, flEaseIn, flCycle);
	}
	else if (flEaseOut < 1 && flCycle > flEaseOut)
	{
		flIdealWeight = smoothstep_bounds(1.0f, flEaseOut, flCycle);
	}

	if (flIdealWeight < 0.0015f)
		return 0.f;

	return std::clamp(flIdealWeight, 0.f, 1.f);
}

bool ServerAnimations::IsLayerSequenceCompleted(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer) {
	if (!m_pAnimstate || !pLayer)
		return false;

	return (pLayer->m_cycle + (m_pAnimstate->m_last_update_increment * pLayer->m_playback_rate)) >= 1;
}

void ServerAnimations::IncrementLayerCycleWeightRateGeneric(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer) {
	if (!m_pAnimstate || !pLayer)
		return;

	float flWeightPrevious = pLayer->m_weight;
	IncrementLayerCycle(m_pAnimstate, pLayer, false);
	pLayer->m_weight = GetLayerIdealWeightFromSeqCycle(m_pAnimstate, pLayer);
	pLayer->m_weight_delta_rate = (pLayer->m_weight - flWeightPrevious) / m_pAnimstate->m_last_update_increment;
}

void ServerAnimations::SetLayerSequence(CCSGOPlayerAnimState* state, C_AnimationLayer* layer, int act, int overrideseq)
{
	if (!state || !layer)
		return;

	int32_t sequence = state->select_sequence_from_acitivty_modifier(act);

	if (overrideseq != -1)
		sequence = overrideseq;

	if (sequence < 0)
		return;

	layer->m_cycle = 0.0f;
	layer->m_weight = 0.0f;
	layer->m_sequence = sequence;
	layer->m_playback_rate = state->m_player->get_layer_seq_cycle_rate(layer, sequence);
}

void ServerAnimations::RebuildLayers(Player* player, serveranimation_data_t* data) {
	if (!player)
		return;

	auto state = player->m_PlayerAnimState();
	if (!state)
		return;

	player->GetPoseParameters(data->m_poses);

	// setup layers that we want to use/fix
	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_MOVE = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_MOVE];
	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_STRAFECHANGE = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_STRAFECHANGE];
	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB];
	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL];
	C_AnimationLayer* ANIMATION_LAYER_ADJUST = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_ADJUST];
	C_AnimationLayer* ANIMATION_LAYER_WHOLE_BODY = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_WHOLE_BODY];
	C_AnimationLayer* ANIMATION_LAYER_ALIVELOOP = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_ALIVELOOP];
	C_AnimationLayer* ANIMATION_LAYER_LEAN = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_LEAN];

	ANIMATION_LAYER_MOVEMENT_MOVE->m_owner =
		ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_owner =
		ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_owner =
		ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_owner =
		ANIMATION_LAYER_ADJUST->m_owner =
		ANIMATION_LAYER_WHOLE_BODY->m_owner =
		ANIMATION_LAYER_ALIVELOOP->m_owner =
		ANIMATION_LAYER_LEAN->m_owner = player;

	data->m_old_vel = data->m_vel;
	data->m_vel = math::Approach(player->m_vecAbsVelocity(), player->m_vecVelocity(), state->m_last_update_increment * 2000);

	data->m_old_ground = data->m_ground;
	data->m_ground = player->m_fFlags() & FL_ONGROUND;

	data->m_old_moving = data->m_moving;
	data->m_moving = data->m_vel.length_2d() >= 0.1f;

	data->m_old_ladder = data->m_ladder;
	data->m_ladder = !data->m_ground && player->m_MoveType() == MOVETYPE_LADDER;

	data->m_old_pos = data->m_pos;
	data->m_pos = player->m_vecOrigin();

	data->m_last_weapon = data->m_weapon;
	data->m_weapon = player->GetActiveWeapon();

	/*if (g_csgo.m_gamerules->m_bFreezePeriod()) {
		data->m_ladder = data->m_moving = false;
		data->m_ground = true;
		data->m_vel = { 0,0,0 };
	}*/

	// setup ground and flag stuff
	if (data->m_spawn_time != player->m_flSpawnTime()) {
		ANIMATION_LAYER_MOVEMENT_MOVE->m_weight =
			ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight =
			ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight =
			ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight =
			ANIMATION_LAYER_WHOLE_BODY->m_weight =
			ANIMATION_LAYER_ALIVELOOP->m_weight =
			ANIMATION_LAYER_LEAN->m_weight = 0.f;

		data->m_old_ground = false;
		data->m_ground = true;

		data->m_old_moving = false;
		data->m_moving = true;

		data->m_old_ladder = false;
		data->m_ladder = true;
	}
	float curtime = game::TICKS_TO_TIME(player->m_nTickBase());

	float velocity_xy = std::min(data->m_vel.length_2d(), 260.f);

	data->m_increment = curtime - data->m_last_update;

	data->m_spawn_time = player->m_flSpawnTime();

	data->m_vel_normalized = data->m_vel.normalized();

	if (velocity_xy > 0)
		data->m_vel_normalized_non_zero = data->m_vel_normalized;

	bool bStartedMovingThisFrame = data->m_moving && !data->m_old_moving;
	bool bStoppedMovingThisFrame = !data->m_moving && data->m_old_moving;

	bool bLeftTheGroundThisFrame = data->m_old_ground && !data->m_ground;
	bool bLandedOnGroundThisFrame = !data->m_old_ground && data->m_ground;

	bool bStartedLadderingThisFrame = (!data->m_old_ladder && data->m_ladder);
	bool bStoppedLadderingThisFrame = (data->m_old_ladder && !data->m_ladder);

	if (player->m_iMoveState() != data->m_old_movestate)
	{
		data->m_stutter_step += 10;
	}

	data->m_old_movestate = player->m_iMoveState();

	data->m_stutter_step = std::clamp(math::Approach(0, data->m_stutter_step, state->m_last_update_increment * 40), 0.f, 100.f);

	if (!data->m_adjust_started && bStoppedMovingThisFrame && data->m_ground && !data->m_ladder && !data->m_landing && data->m_stutter_step < 50)
	{
		SetLayerSequence(state, ANIMATION_LAYER_ADJUST, ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING, -1);
		data->m_adjust_started = true;
	}

	if (GetLayerActivity(state, ANIMATION_LAYER_ADJUST) == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING ||
		GetLayerActivity(state, ANIMATION_LAYER_ADJUST) == ACT_CSGO_IDLE_TURN_BALANCEADJUST)
	{
		if (data->m_adjust_started && state->m_speed_as_portion_of_crouch_top_speed <= 0.25f)
		{
			IncrementLayerCycleWeightRateGeneric(state, ANIMATION_LAYER_ADJUST);
			data->m_adjust_started = !(IsLayerSequenceCompleted(state, ANIMATION_LAYER_ADJUST));
		}
		else
		{
			data->m_adjust_started = false;
			float flWeight = ANIMATION_LAYER_ADJUST->m_weight;

			ANIMATION_LAYER_ADJUST->m_weight = math::Approach(0, flWeight, state->m_last_update_increment * 5);
			ANIMATION_LAYER_ADJUST->m_weight_delta_rate = (ANIMATION_LAYER_ADJUST->m_weight - flWeight) / state->m_last_update_increment;
		}
	}

	if (velocity_xy > 0 && data->m_ground)
	{
		// convert horizontal velocity vec to angular yaw
		float flRawYawIdeal = (atan2(-data->m_vel[1], -data->m_vel[0]) * 180 / math::pi);
		if (flRawYawIdeal < 0)
			flRawYawIdeal += 360;

		data->m_move_yaw_ideal = math::AngleDiff(flRawYawIdeal, state->m_abs_yaw);
	}

	float current_to_ideal = math::AngleDiff(data->m_move_yaw_ideal, data->m_move_yaw);

	if (bStartedMovingThisFrame && data->m_move_weight <= 0)
		data->m_move_yaw = data->m_move_yaw_ideal;
	else if (ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight >= 1)
		data->m_move_yaw = data->m_move_yaw_ideal;
	else
	{
		float flMoveWeight = math::Lerp(state->m_anim_duck_amount, std::clamp(state->m_speed_as_portion_of_walk_top_speed, 0.f, 1.f), std::clamp(state->m_speed_as_portion_of_crouch_top_speed, 0.f, 1.f));
		float flRatio = math::Bias(flMoveWeight, 0.18f) + 0.1f;

		data->m_move_yaw = math::NormalizedAngle(data->m_move_yaw + (current_to_ideal * flRatio));
	}


	if (data->m_ground)
	{
		if (velocity_xy > 0.1f)
		{
			data->m_body_update = curtime + (1.1f * 0.2f);
			data->m_body = state->m_eye_yaw;
		}
		else
		{
			if (curtime > data->m_body_update && abs(math::AngleDiff(state->m_abs_yaw, state->m_eye_yaw)) > 35.0f)
			{
				data->m_body_update = curtime + 1.1f;
				data->m_body = state->m_eye_yaw;
			}
		}
	}

	if (velocity_xy <= 1.0f && data->m_ground && !data->m_ladder && !data->m_landing && state->m_last_update_increment > 0 && abs(math::AngleDiff(data->m_last_foot_yaw, state->m_abs_yaw) / state->m_last_update_increment > 120.0f))
	{
		SetLayerSequence(state, ANIMATION_LAYER_ADJUST, ACT_CSGO_IDLE_TURN_BALANCEADJUST, -1);
		data->m_adjust_started = true;
	}

	static auto GetWeaponPrefix = pattern::find(g_csgo.m_client_dll, XOR("E8 ? ? ? ? 50 8D 44 24 54")).resolve_rip().as<const char* (__thiscall*)(CCSGOPlayerAnimState*)>();

	char szWeaponMoveSeq[64];
	std::sprintf(szWeaponMoveSeq, "move_%s", GetWeaponPrefix(state));

	int nWeaponMoveSeq = LookupSequence(player, szWeaponMoveSeq);
	if (nWeaponMoveSeq == -1)
	{
		nWeaponMoveSeq = LookupSequence(player, "move");
	}

	// see: CSGOPlayerAnimState::SetUpMovement
	float flTargetMoveWeight = math::Lerp(state->m_anim_duck_amount, std::clamp(state->m_speed_as_portion_of_walk_top_speed, 0.f, 1.f), std::clamp(state->m_speed_as_portion_of_crouch_top_speed, 0.f, 1.f));

	if (data->m_move_weight <= flTargetMoveWeight)
	{
		data->m_move_weight = flTargetMoveWeight;
	}
	else
	{
		data->m_move_weight = math::Approach(flTargetMoveWeight, data->m_move_weight, state->m_last_update_increment * math::RemapValClamped(data->m_stutter_step, 0.0f, 100.0f, 2, 20));
	}

	vec3_t vecMoveYawDir;
	math::AngleVectors(ang_t(0, math::NormalizedAngle(state->m_abs_yaw + data->m_move_yaw + 180), 0), &vecMoveYawDir);
	float flYawDeltaAbsDot = abs(data->m_vel_normalized_non_zero.dot(vecMoveYawDir));
	data->m_move_weight *= math::Bias(flYawDeltaAbsDot, 0.2);

	float flMoveWeightWithAirSmooth = data->m_move_weight * data->m_air_smooth;

	// dampen move weight for landings
	flMoveWeightWithAirSmooth *= std::max((1.0f - ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight), 0.55f);

	float flMoveCycleRate = 0;
	if (velocity_xy > 0)
	{
		flMoveCycleRate = player->get_layer_seq_cycle_rate(ANIMATION_LAYER_MOVEMENT_MOVE, nWeaponMoveSeq);
		float flSequenceGroundSpeed = std::max(player->get_seq_move_dist(player->GetModelPtr(), nWeaponMoveSeq, data->m_poses) / (1.0f / flMoveCycleRate), 0.001f);
		flMoveCycleRate *= velocity_xy / flSequenceGroundSpeed;

		flMoveCycleRate *= math::Lerp(state->m_walk_run_transition, 1.0f, 0.85f);
	}

	float flLocalCycleIncrement = (flMoveCycleRate * state->m_last_update_increment);
	data->m_primary_cycle = math::ClampCycle(data->m_primary_cycle + flLocalCycleIncrement);

	flMoveWeightWithAirSmooth = std::clamp(flMoveWeightWithAirSmooth, 0.f, 1.f);
	UpdateAnimLayer(ANIMATION_LAYER_MOVEMENT_MOVE, nWeaponMoveSeq, flLocalCycleIncrement, flMoveWeightWithAirSmooth, data->m_primary_cycle);

	vec3_t vecForward;
	vec3_t vecRight;
	math::AngleVectors(ang_t(0, state->m_abs_yaw, 0), &vecForward, &vecRight, NULL);
	vecRight.normalized();

	float flVelToRightDot = data->m_vel_normalized_non_zero.dot(vecRight);
	float flVelToForwardDot = data->m_vel_normalized_non_zero.dot(vecForward);

	// We're interested in if the player's desired direction (indicated by their held buttons) is opposite their current velocity.
	// This indicates a strafing direction change in progress.

	bool moveRight = (g_cl.m_cmd->m_buttons & (IN_MOVERIGHT)) != 0;
	bool moveLeft = (g_cl.m_cmd->m_buttons & (IN_MOVELEFT)) != 0;
	bool moveForward = (g_cl.m_cmd->m_buttons & (IN_FORWARD)) != 0;
	bool moveBackward = (g_cl.m_cmd->m_buttons & (IN_BACK)) != 0;

	bool bStrafeRight = (state->m_speed_as_portion_of_walk_top_speed >= 0.73f && moveRight && !moveLeft && flVelToRightDot < -0.63f);
	bool bStrafeLeft = (state->m_speed_as_portion_of_walk_top_speed >= 0.73f && moveLeft && !moveRight && flVelToRightDot > 0.63f);
	bool bStrafeForward = (state->m_speed_as_portion_of_walk_top_speed >= 0.65f && moveForward && !moveBackward && flVelToForwardDot < -0.55f);
	bool bStrafeBackward = (state->m_speed_as_portion_of_walk_top_speed >= 0.65f && moveBackward && !moveForward && flVelToForwardDot > 0.55f);

	bool bStrafing = (bStrafeRight || bStrafeLeft || bStrafeForward || bStrafeBackward);

	if (bStrafing)
	{
		data->m_strafe_changed = true;

		ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight = math::Approach(1, ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight, state->m_last_update_increment * 20);
		ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_cycle = math::Approach(0, ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_cycle, state->m_last_update_increment * 10);
	}
	else if (ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight > 0)
	{
		data->m_strafe_duration += state->m_last_update_increment;

		if (data->m_strafe_duration > 0.08f)
			ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight = math::Approach(0, ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight, state->m_last_update_increment * 5);

		ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_sequence = LookupSequence(player, "strafe");

		float flRate = player->get_layer_seq_cycle_rate(ANIMATION_LAYER_MOVEMENT_STRAFECHANGE, ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_sequence);
		ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_cycle = std::clamp(ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_cycle + state->m_last_update_increment * flRate, 0.f, 1.f);
	}

	if (ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight <= 0)
	{
		data->m_strafe_changed = false;
	}

	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight *= (1.0f - state->m_anim_duck_amount);
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight *= data->m_air_smooth;
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight = std::clamp(ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight, 0.f, 1.f);

	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_playback_rate = 0.f;

	if (bLandedOnGroundThisFrame)
	{
		float flDistanceFell = abs(state->m_left_ground_height - data->m_pos.z);
		float flDistanceFallNormalizedBiasRange = math::RemapValClamped(flDistanceFell, 12.0f, 72.0f, 0.0f, 1.0f);

		data->m_land_multiplier = std::clamp(data->m_duration_in_air, 0.1f, 1.0f);
		data->m_duck_additive = std::max(state->m_land_anim_multiplier, flDistanceFallNormalizedBiasRange);
	}
	else
	{
		data->m_duck_additive = math::Approach(0, data->m_duck_additive, state->m_last_update_increment * 2);
	}

	data->m_air_smooth = math::Approach(data->m_ground ? 1 : 0, data->m_air_smooth, math::Lerp(state->m_anim_duck_amount, 8.f, 16.f) * state->m_last_update_increment);
	data->m_air_smooth = std::clamp(data->m_air_smooth, 0.f, 1.f);

	// fade out jump if we're climbing
	if (data->m_ladder_weight || data->m_ladder) {
		if (bStartedLadderingThisFrame) {
			SetLayerSequence(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB, ACT_CSGO_CLIMB_LADDER, -1);
		}

		if (abs(data->m_vel.z) > 100 && data->m_ladder) {
			data->m_ladder_speed = math::Approach(1, data->m_ladder_speed, state->m_last_update_increment * 10.0f);
		}
		else {
			data->m_ladder_speed = math::Approach(0, data->m_ladder_speed, state->m_last_update_increment * 10.0f);
		}
		data->m_ladder_speed = std::clamp(data->m_ladder_speed, 0.f, 1.f);

		if (data->m_ladder) {
			data->m_ladder_weight = math::Approach(1, data->m_ladder_weight, state->m_last_update_increment * 5.0f);
		}
		else {
			data->m_ladder_weight = math::Approach(0, data->m_ladder_weight, state->m_last_update_increment * 10.0f);
		}

		data->m_ladder_weight = std::clamp(data->m_ladder_weight, 0.f, 1.f);

		float flLadderClimbCycle = ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_cycle;
		flLadderClimbCycle += (data->m_pos.z - data->m_old_pos.z) * math::Lerp(data->m_ladder_speed, 0.010f, 0.004f);

		data->m_poses[POSE_LADDER_SPEED] = data->m_ladder_speed;

		if (GetLayerActivity(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB) == ACT_CSGO_CLIMB_LADDER) {
			ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight = data->m_ladder_weight;
		}

		ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_cycle = flLadderClimbCycle;

		if (data->m_ladder) {
			float flIdealJumpWeight = 1.0f - data->m_ladder_weight;
			if (ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight > flIdealJumpWeight)
			{
				ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight = flIdealJumpWeight;
			}
		}
	}

	// fix ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL
	if (data->m_ground) {
		if (bLandedOnGroundThisFrame || bStoppedLadderingThisFrame)
		{
			SetLayerSequence(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB, data->m_duration_in_air > 1 ? ACT_CSGO_LAND_HEAVY : ACT_CSGO_LAND_LIGHT, -1);
			ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_cycle = 0.f;
			data->m_landing = true;
		}

		data->m_duration_in_air = 0.f;

		if (data->m_landing && !data->m_ladder) {
			IncrementLayerCycle(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB, false);
			IncrementLayerCycle(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL, false);

			data->m_poses[POSE_JUMP_FALL] = 0.f;

			if (IsLayerSequenceCompleted(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB)) {
				data->m_landing = false;
				ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight = 0.f;
				ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight = 0.f;
				data->m_land_multiplier = 1.0f;
			}
			else {
				ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight = GetLayerIdealWeightFromSeqCycle(state, ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB) * data->m_land_multiplier * std::clamp((1.0f - state->m_anim_duck_amount), 0.2f, 1.0f);

				ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight = math::Approach(0, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight, state->m_last_update_increment * 10.0f);
			}
		}

		if (!data->m_landing && data->m_ladder_weight <= 0)
		{
			ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight = 0.f;
		}
	}
	else if (!data->m_ladder) {
		data->m_duration_in_air += state->m_last_update_increment;

		IncrementLayerCycle(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL, false);

		if (bLeftTheGroundThisFrame) {
			// setup the sequence responsible for jumping
			if (state->m_speed_as_portion_of_walk_top_speed > .25f)
				SetLayerSequence(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL, -1, state->m_anim_duck_amount > 0.55f ? 18 : 16);
			else
				SetLayerSequence(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL, -1, state->m_anim_duck_amount > 0.55f ? 17 : 15);
		}

		// increase jump weight
		float flNextJumpWeight = GetLayerIdealWeightFromSeqCycle(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL);
		if (flNextJumpWeight > ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight)
		{
			ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL->m_weight = flNextJumpWeight;
		}

		// bash any lingering land weight to zero
		if (ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight > 0)
		{
			ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB->m_weight *= smoothstep_bounds(0.2f, 0.0f, state->m_duration_in_air);
		}

		data->m_poses[POSE_JUMP_FALL] = std::clamp(smoothstep_bounds(0.72f, 1.52f, data->m_duration_in_air), 0.f, 1.f);
	}

	state->m_landing = data->m_landing;
	state->m_duck_additional = data->m_duck_additive;
	state->m_land_anim_multiplier = data->m_land_multiplier;
	state->m_duration_in_air = data->m_duration_in_air;

	if (player->m_iTeamNum() == TEAM_CT && player->m_bIsDefusing()) // should be defusing
	{
		if (!data->m_defuse_started)
		{
			// we are now defusing and need to start the anim
			SetLayerSequence(state, ANIMATION_LAYER_WHOLE_BODY, player->m_bHasDefuser() ? ACT_CSGO_DEFUSE_WITH_KIT : ACT_CSGO_DEFUSE, -1);
			data->m_defuse_started = true;
		}
		else
		{
			IncrementLayerCycleWeightRateGeneric(state, ANIMATION_LAYER_WHOLE_BODY);
		}
	}
	else if (GetLayerActivity(state, ANIMATION_LAYER_WHOLE_BODY) == ACT_CSGO_DEFUSE || GetLayerActivity(state, ANIMATION_LAYER_WHOLE_BODY) == ACT_CSGO_DEFUSE_WITH_KIT)
	{
		// should NOT be defusing but IS
		if (ANIMATION_LAYER_WHOLE_BODY->m_weight > 0)
		{
			float flCurrentWeight = ANIMATION_LAYER_WHOLE_BODY->m_weight;
			ANIMATION_LAYER_WHOLE_BODY->m_weight = math::Approach(0, flCurrentWeight, state->m_last_update_increment * 8.0f);
			ANIMATION_LAYER_WHOLE_BODY->m_weight_delta_rate = (ANIMATION_LAYER_WHOLE_BODY->m_weight - flCurrentWeight) / state->m_last_update_increment;
		}
		data->m_defuse_started = false;
	}
	else if (GetLayerActivity(state, ANIMATION_LAYER_WHOLE_BODY) == ACT_CSGO_PLANT_BOMB) // is planting
	{
		if (data->m_weapon && data->m_weapon->m_iItemDefinitionIndex() != C4)
			data->m_plant_anim_started = false; // cancel planting if we're not holding c4

		if (data->m_plant_anim_started) // plant in progress
		{
			// Inlined IncrementLayerCycleWeightRateGeneric() so we can tune the layering weight when crouch-planting the bomb.
			//
			// Instead of setting the weight to GetLayerIdealWeightFromSeqCycle, we approach that value which smoothly blends to
			// the bomb plant animation.  This means that the 'standing' part of the animation doesn't get overly blended to when
			// planting the animation from a crouched position.  In addition, if you are in thirdperson camera, the crouch is
			// predicted but the plant animation is server-side.  So we do this blend regardless of the crouch state because it
			// could differ between the client and the server.
			//
			// This does mean that fine detail in the beginning of the plant animation is lost.  Fortunately there isn't much of
			// that at the moment.
			float flWeightPrevious = ANIMATION_LAYER_WHOLE_BODY->m_weight;
			IncrementLayerCycle(state, ANIMATION_LAYER_WHOLE_BODY, false);
			ANIMATION_LAYER_WHOLE_BODY->m_weight = math::Approach(GetLayerIdealWeightFromSeqCycle(state, ANIMATION_LAYER_WHOLE_BODY), flWeightPrevious, state->m_last_update_increment * 1.2f);
			ANIMATION_LAYER_WHOLE_BODY->m_weight_delta_rate = (ANIMATION_LAYER_WHOLE_BODY->m_weight - flWeightPrevious) / state->m_last_update_increment;

			data->m_plant_anim_started = !(IsLayerSequenceCompleted(state, ANIMATION_LAYER_WHOLE_BODY));
		}
		else
		{
			if (ANIMATION_LAYER_WHOLE_BODY->m_weight > 0)
			{
				//bomb plant aborted, pull out the weight
				float flCurrentWeight = ANIMATION_LAYER_WHOLE_BODY->m_weight;
				ANIMATION_LAYER_WHOLE_BODY->m_weight = math::Approach(0, flCurrentWeight, state->m_last_update_increment * 12.0f);
				ANIMATION_LAYER_WHOLE_BODY->m_weight_delta_rate = (ANIMATION_LAYER_WHOLE_BODY->m_weight - flCurrentWeight) / state->m_last_update_increment;
			}
			data->m_plant_anim_started = false;
		}
	}
	else
	{
		// fallback
		ANIMATION_LAYER_WHOLE_BODY->m_cycle = 0.999f;
		ANIMATION_LAYER_WHOLE_BODY->m_weight = 0;
		ANIMATION_LAYER_WHOLE_BODY->m_weight_delta_rate = 0;
	}

	if (GetLayerActivity(state, ANIMATION_LAYER_ALIVELOOP) != ACT_CSGO_ALIVE_LOOP)
	{
		// first time init
		SetLayerSequence(state, ANIMATION_LAYER_ALIVELOOP, ACT_CSGO_ALIVE_LOOP, -1);
		ANIMATION_LAYER_ALIVELOOP->m_cycle = g_csgo.RandomFloat(0, 1);

		ANIMATION_LAYER_ALIVELOOP->m_playback_rate = player->get_layer_seq_cycle_rate(ANIMATION_LAYER_ALIVELOOP, ANIMATION_LAYER_ALIVELOOP->m_sequence);
		ANIMATION_LAYER_ALIVELOOP->m_playback_rate *= g_csgo.RandomFloat(0.8f, 1.1f);
	}
	else
	{
		if (data->m_weapon && data->m_weapon != data->m_last_weapon)
		{
			//re-roll act on weapon change
			float flRetainCycle = ANIMATION_LAYER_ALIVELOOP->m_cycle;
			SetLayerSequence(state, ANIMATION_LAYER_ALIVELOOP, ACT_CSGO_ALIVE_LOOP, -1);
			ANIMATION_LAYER_ALIVELOOP->m_cycle = flRetainCycle;
		}
		else if (IsLayerSequenceCompleted(state, ANIMATION_LAYER_ALIVELOOP))
		{
			ANIMATION_LAYER_ALIVELOOP->m_playback_rate = player->get_layer_seq_cycle_rate(ANIMATION_LAYER_ALIVELOOP, ANIMATION_LAYER_ALIVELOOP->m_sequence);
			ANIMATION_LAYER_ALIVELOOP->m_playback_rate *= g_csgo.RandomFloat(0.8f, 1.1f);
		}
		else
		{
			ANIMATION_LAYER_ALIVELOOP->m_weight = math::RemapValClamped(state->m_speed_as_portion_of_run_top_speed, 0.55f, 0.9f, 1.0f, 0.0f);
		}
	}

	IncrementLayerCycle(state, ANIMATION_LAYER_ALIVELOOP, true);


	float flInterval = fabs(curtime - data->m_vel_test_time);
	if (flInterval > 0.025f)
	{
		flInterval = std::min(flInterval, 0.1f);
		data->m_vel_test_time = curtime;

		data->m_acceleration = (data->m_vel - data->m_vel_test) / flInterval;
		data->m_acceleration.z = 0;

		data->m_vel_test = data->m_vel;
	}

	data->m_lerped_acceleration = math::Approach(data->m_acceleration, data->m_lerped_acceleration, state->m_last_update_increment * 800.0f);

	ang_t temp;
	math::VectorAngles(data->m_lerped_acceleration, temp);

	ANIMATION_LAYER_LEAN->m_weight = std::clamp((data->m_lerped_acceleration.length() / 260.f) * state->m_speed_as_portion_of_run_top_speed, 0.f, 1.f);
	ANIMATION_LAYER_LEAN->m_weight *= (1.0f - data->m_old_ladder);

	data->m_poses[animstate_pose_param_idx_t::POSE_LEAN_YAW] = (math::NormalizedAngle(state->m_abs_yaw - temp.y) / 360.f) + 0.5f;

	if (ANIMATION_LAYER_LEAN->m_sequence <= 0)
		SetLayerSequence(state, ANIMATION_LAYER_LEAN, -1, LookupSequence(player, "lean"));

	data->m_last_foot_yaw = state->m_abs_yaw;

	float flAimYaw = math::AngleDiff(state->m_eye_yaw, state->m_abs_yaw);

	data->m_poses[animstate_pose_param_idx_t::POSE_STRAFE_YAW] = (math::NormalizedAngle(data->m_move_yaw) / 360.f) + 0.5f;
	data->m_poses[animstate_pose_param_idx_t::POSE_BODY_PITCH] = (state->m_eye_pitch / 180.f) + 0.5;
	data->m_poses[animstate_pose_param_idx_t::POSE_BODY_YAW] = (flAimYaw / 116.f) + 0.5;
	data->m_poses[animstate_pose_param_idx_t::POSE_SPEED] = std::clamp(state->m_speed_as_portion_of_run_top_speed, 0.0f, 1.0f);
	data->m_poses[animstate_pose_param_idx_t::POSE_STAND] = std::clamp(1.0f - (ANIMATION_LAYER_MOVEMENT_STRAFECHANGE->m_weight * state->m_anim_duck_amount), 0.0f, 1.0f);

	data->m_poses[animstate_pose_param_idx_t::POSE_MOVE_BLEND_WALK] = (1.0f - state->m_walk_run_transition) * (1.0f - state->m_anim_duck_amount);
	data->m_poses[animstate_pose_param_idx_t::POSE_MOVE_BLEND_RUN] = state->m_walk_run_transition * (1.0f - state->m_anim_duck_amount);
	data->m_poses[animstate_pose_param_idx_t::POSE_MOVE_BLEND_CROUCH] = state->m_anim_duck_amount;

	data->m_last_update = curtime;

	data->m_updated = true;
}

void ServerAnimations::DoAnimationEvent(Player* player, serveranimation_data_t* data) {
	if (!player)
		return;

	auto state = player->m_PlayerAnimState();
	if (!state)
		return;

	Weapon* weapon = player->GetActiveWeapon();

	data->m_buttons = g_cl.m_cmd->m_buttons;

	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB];
	C_AnimationLayer* ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL];
	C_AnimationLayer* ANIMATION_LAYER_WHOLE_BODY = &data->m_layers[animstate_layer_t::ANIMATION_LAYER_WHOLE_BODY];

	if ((data->m_buttons & IN_JUMP) && !(data->m_old_buttons & IN_JUMP)) {
		data->m_jumping = true;
		SetLayerSequence(state, ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL, ACT_CSGO_JUMP, -1);
	}

	if (weapon && ((data->m_buttons & IN_ATTACK) && !(data->m_old_buttons & IN_ATTACK) || (data->m_buttons & IN_USE) && !(data->m_old_buttons & IN_USE)) && weapon->m_iItemDefinitionIndex() == C4)
	{
		SetLayerSequence(state, ANIMATION_LAYER_WHOLE_BODY, ACT_CSGO_PLANT_BOMB, -1);
		data->m_plant_anim_started = true;
	}

	data->m_old_buttons = data->m_buttons;
}
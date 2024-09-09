#include "includes.h"

Aimbot g_aimbot{ };;


static WeaponCfg m_settings;


void Aimbot::update_shoot_pos()
{
	if (!g_cl.m_local || !g_cl.m_processing)
		return;

	const auto anim_state = g_cl.m_local->m_PlayerAnimState();
	if (!anim_state)
		return;

	const auto backup = g_cl.m_local->m_flPoseParameter()[12];
	const auto originbackup = g_cl.m_local->GetAbsOrigin();

	// set pitch, rotation etc
	g_cl.m_local->SetAbsOrigin(g_cl.m_local->m_vecOrigin());
	g_cl.m_local->m_flPoseParameter()[12] = 0.5f;

	// update shoot pos
	g_cl.m_shoot_pos = g_cl.m_local->GetShootPosition();

	// set to backup
	g_cl.m_local->SetAbsOrigin(originbackup);
	g_cl.m_local->m_flPoseParameter()[12] = backup;
}

void AimPlayer::HandleLagcompensation() {
	if (m_records.empty())
		return;

	LagRecord* newest = &m_records[0];
	LagRecord* previous = m_records.size() >= 2 ? &m_records[1] : nullptr;

	if (!newest || !previous)
		return;

	newest->m_predicted = false;

	newest->m_breaking_lc[0] = newest->m_breaking_lc[1] = false;

	if (newest->m_sim_time < previous->m_sim_time)
		m_target_time = previous->m_sim_time;

	const bool breaking_lc = (newest->m_origin - previous->m_origin).length_sqr() > 4096.f;

	newest->m_breaking_lc[0] = newest->m_sim_time <= m_target_time;

	if (breaking_lc)
		for (auto& record : m_records)
			record.m_breaking_lc[1] = true;
}

float RemapValClampedInverse(float val, float A, float B, float C, float D) {
	if (C == D)
		return (val == C) ? A : B;
	float cVal = (val - C) / (D - C);
	cVal = std::clamp(cVal, 0.0f, 1.0f);

	return A + (B - A) * cVal;
}

void AimPlayer::CorrectOldStateData(Player* player, LagRecord* record, LagRecord* previous) {
	CCSGOPlayerAnimState* state = m_player->m_PlayerAnimState();
	if (!state)
		return;

	//state->m_last_update_time = previous->m_anim_time;
	state->m_move_weight_smoothed = previous->m_layers[6].m_weight;
	state->m_acceleration_weight = previous->m_layers[12].m_weight;

	if (previous->m_anim_flags & FL_ONGROUND)
		state->m_duration_in_air = 0.f;
}

void AimPlayer::SyncPlayerVelocity(Player* player, LagRecord* record, LagRecord* previous) {
	CCSGOPlayerAnimState* state = m_player->m_PlayerAnimState();
	if (!state)
		return;

	if (!(record->m_anim_flags & FL_ONGROUND))
		return;

	record->m_velocity[1].z = record->m_abs_velocity[1].z = 0.f;

	if (record->m_layers[6].m_weight == 0.0f) {
		record->m_velocity[1] = record->m_abs_velocity[1] = { 0, 0, 0 };
		return;
	}

	auto weapon = player->GetActiveWeapon();
	if (!weapon)
		return;

	auto weapondata = weapon->GetWpnData();
	if (!weapondata)
		return;

	if (record->m_layers[11].m_playback_rate != previous->m_layers[11].m_playback_rate || weapon != state->m_weapon)
		return;

	float max_speed = std::max((player->m_bIsScoped() ? weapondata->m_max_player_speed_alt : weapondata->m_max_player_speed), 0.001f);
	const vec3_t animstate_velocity = math::Approach(record->m_velocity[1], record->m_abs_velocity[1], (1.f / game::TICKS_TO_TIME(record->m_lag)) * 1000.f);

	const float server_weight = record->m_layers[11].m_weight;
	const float client_weight = RemapValClampedInverse(animstate_velocity.length_2d() / max_speed, 0.55f, 0.9f, 1.0f, 0.0f);

	//const float multiplier = std::clamp(server_weight - client_weight + 1.f, 0.f, 1.f);
	const float multiplier = server_weight - client_weight + 1.f;

	record->m_velocity[1] *= multiplier;
	record->m_abs_velocity[1] *= multiplier;
}

void AimPlayer::UpdateAnimations(LagRecord* record) {
	CCSGOPlayerAnimState* state = m_player->m_PlayerAnimState();
	if (!state)
		return;

	LagRecord* previous = m_records.size() >= 2 ? &m_records[1] : nullptr;

	// player respawned.
	if (m_player->m_flSpawnTime() != m_spawn) {
		// reset animation state.
		game::ResetAnimationState(state);

		// note new spawn time.
		m_spawn = m_player->m_flSpawnTime();
	}

	record->store(m_player);

	// backup curtime.
	float curtime = g_csgo.m_globals->m_curtime;
	float frametime = g_csgo.m_globals->m_frametime;

	// set curtime to animtime.
	// set frametime to ipt just like on the server during simulation.
	g_csgo.m_globals->m_curtime = record->m_anim_time;
	g_csgo.m_globals->m_frametime = g_csgo.m_globals->m_interval;

	// backup stuff that we do not want to fuck with.
	AnimationBackup_t backup;

	backup.m_origin = m_player->m_vecOrigin();
	backup.m_abs_origin = m_player->GetAbsOrigin();
	backup.m_velocity = m_player->m_vecVelocity();
	backup.m_abs_velocity = m_player->m_vecAbsVelocity();
	backup.m_flags = m_player->m_fFlags();
	backup.m_eflags = m_player->m_iEFlags();
	backup.m_duck = m_player->m_flDuckAmount();
	backup.m_body = m_player->m_flLowerBodyYawTarget();

	m_player->GetAnimLayers(backup.m_layers);

	// is player a bot?
	bool bot = game::IsFakePlayer(m_player->index());

	// reset fakewalk state.
	record->m_mode = Resolver::Modes::RESOLVE_DISABLED;


	if (previous) {
		record->m_anim_time = previous->m_sim_time + g_csgo.m_globals->m_interval;

		record->m_lag = std::clamp(game::TIME_TO_TICKS(record->m_sim_time - previous->m_sim_time), 1, (int)(1 / g_csgo.m_globals->m_interval));
	}

	// recalculate velocity and fix choke.
	// https://github.com/VSES/SourceEngine2007/blob/master/se2007/game/client/c_baseplayer.cpp#L659

	record->m_abs_velocity[0] = record->m_velocity[0] = { 0, 0, 0 }; // reset them we will recalcualte.

	if (previous) {
		record->m_abs_velocity[0] = record->m_velocity[0] = (record->m_origin - previous->m_origin) * (1.f / game::TICKS_TO_TIME(record->m_lag));

		//record->m_abs_origin += record->m_velocity[0];

		//record->m_abs_velocity[0] = (record->m_abs_origin - previous->m_abs_origin) * (1.f / game::TICKS_TO_TIME(record->m_lag));
	}

	record->m_velocity[1] = record->m_velocity[0];
	record->m_abs_velocity[1] = record->m_abs_velocity[0];

	// set this fucker, it will get overriden.
	record->m_anim_flags = record->m_flags;
	record->m_anim_duck = record->m_duck;

	if (previous) {
		// the game animates the first choked packet.
		if (record->m_lag > 1) {
			// set previous flags.
			record->m_anim_flags = previous->m_flags;

			// strip the on ground flag.
			record->m_anim_flags &= ~FL_ONGROUND;

			// been onground for 2 consecutive ticks? fuck yeah.
			if ((record->m_flags & FL_ONGROUND) && (previous->m_flags & FL_ONGROUND))
				record->m_anim_flags |= FL_ONGROUND;

			// fix jump_fall.
			if (record->m_layers[ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB].m_weight > previous->m_layers[ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB].m_weight)
				record->m_anim_flags |= FL_ONGROUND;

			if (record->m_layers[ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL].m_weight > previous->m_layers[ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL].m_weight)
				record->m_anim_flags &= ~FL_ONGROUND;

			record->m_anim_duck = math::Lerp(g_csgo.m_globals->m_interval, previous->m_duck, record->m_duck);
			record->m_velocity[1] = math::Lerp(g_csgo.m_globals->m_interval, previous->m_velocity[0], record->m_velocity[0]);
			record->m_abs_velocity[1] = math::Lerp(g_csgo.m_globals->m_interval, previous->m_abs_velocity[0], record->m_abs_velocity[0]);

			SyncPlayerVelocity(m_player, record, previous);

			g_resolver.ResolveAngles(m_player, record, previous);
			record->m_eye_angles.y = m_resolved_angle;
		}

		m_player->SetAnimLayers(previous->m_layers);
		CorrectOldStateData(m_player, record, previous);
	}

	// set stuff before animating.
	m_player->m_vecOrigin() = record->m_origin;
	m_player->SetAbsOrigin(record->m_origin);

	m_player->m_vecVelocity() = record->m_velocity[1];
	m_player->m_vecAbsVelocity() = record->m_abs_velocity[1];

	m_player->m_flDuckAmount() = record->m_anim_duck;
	m_player->m_flLowerBodyYawTarget() = record->m_body;

	m_player->m_fFlags() = record->m_anim_flags;

	// EFL_DIRTY_ABSVELOCITY
	// skip call to C_BaseEntity::CalcAbsoluteVelocity
	m_player->m_iEFlags() &= ~0x1000;

	// write potentially resolved angles.
	m_player->m_angEyeAngles() = record->m_eye_angles;

	// 'm_animating' returns true if being called from SetupVelocity, passes raw velocity to animstate.
	UpdateClientAnimations::DoUpdate(m_player);

	// restore server layers.
	m_player->SetAnimLayers(record->m_layers);

	record->m_abs_ang = ang_t(0.f, state->m_abs_yaw, 0.f);

	m_player->SetAbsAngles(record->m_abs_ang);
	m_player->GetPoseParameters(record->m_poses);

	g_csgo.m_globals->m_curtime = record->m_sim_time;

	record->m_setup = g_bones.BuildBones(m_player, BONE_USED_BY_ANYTHING, record->m_bones, true);

	// restore backup data.
	m_player->m_vecOrigin() = backup.m_origin;
	m_player->m_vecVelocity() = backup.m_velocity;
	m_player->m_vecAbsVelocity() = backup.m_abs_velocity;
	m_player->m_fFlags() = backup.m_flags;
	m_player->m_iEFlags() = backup.m_eflags;
	m_player->m_flDuckAmount() = backup.m_duck;
	m_player->m_flLowerBodyYawTarget() = backup.m_body;
	m_player->SetAbsOrigin(backup.m_abs_origin);
	m_player->SetAnimLayers(backup.m_layers);

	// IMPORTANT: do not restore poses here, since we want to preserve them for rendering.
	// also dont restore the render angles which indicate the model rotation.

	// restore globals.
	g_csgo.m_globals->m_curtime = curtime;
	g_csgo.m_globals->m_frametime = frametime;
}

void AimPlayer::OnNetUpdate(Player* player) {
	if (!player->alive() || player->dormant()) {
		m_records.clear();
		g_resolver.m_override_data[player->index() - 1].m_fov = FLT_MAX;
		m_target_time = 0.f;
		return;
	}

	if (!g_cl.m_processing || (g_cl.m_local && !player->enemy(g_cl.m_local))) {
		if (player->m_flSimulationTime() != player->m_flOldSimulationTime())
			UpdateClientAnimations::DoUpdate(player);

		m_records.clear();
		g_resolver.m_override_data[player->index() - 1].m_fov = FLT_MAX;
		m_target_time = 0.f;
		return;
	}

	if (m_player != player)
		m_records.clear();

	m_player = player;

	bool update = (m_records.empty() || m_player->m_flSimulationTime() != m_records[0].m_sim_time);

	if (update && !m_records.empty()) {
		LagRecord* previous = &m_records[0];

		if (m_player->m_AnimOverlay()[11].m_cycle == previous->m_layers[11].m_cycle) {
			m_player->m_flSimulationTime() = previous->m_sim_time;
			update = false;
		}
	}

	if (update) {
		m_records.emplace_front(LagRecord());

		UpdateAnimations(&m_records[0]);
	}

	HandleLagcompensation();

	// no need to store insane amt of data.
	while (m_records.size() > 256)
		m_records.pop_back();
}

void AimPlayer::OnRoundStart(Player* player) {
	m_player = player;
	m_walk_body = 0.f;
	m_moved = false;
	m_last_move_time = 0.f;

	m_shots = 0;
	m_missed_shots = 0;

	// reset stand and body index.
	m_ff_index = 0;
	m_stand_index = 0;
	m_stand2_index = 0;
	m_body_index = 0;
	m_air_index = 0;

	m_records.clear();
	m_hitboxes.clear();

	// IMPORTANT: DO NOT CLEAR LAST HIT SHIT.
}

void AimPlayer::SetupHitboxes(LagRecord* record, bool history) {
	// reset hitboxes.
	m_hitboxes.clear();

	if (g_cl.m_weapon_id == ZEUS) {
		// hitboxes for the zeus.
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	bool resolved = record->m_mode == Resolver::Modes::RESOLVE_DISABLED || record->m_mode == Resolver::Modes::RESOLVE_WALK || record->m_mode == Resolver::Modes::RESOLVE_FLICK || record->m_mode == Resolver::Modes::RESOLVE_PREFLICK;

	// only, always.
	if (m_settings.baim2.get(0)) {
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	// only, health.
	if (m_settings.baim2.get(1) && m_player->m_iHealth() <= (int)m_settings.baim_hp.get()) {
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	// only, fake.
	if (m_settings.baim2.get(2) && !resolved) {
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	// only, in air.
	if (m_settings.baim2.get(3) && !(record->m_flags & FL_ONGROUND)) {
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	// only, on key.
	if (g_aimbot.m_baim) {
		m_hitboxes.push_back({ HITBOX_BODY, HitscanMode::PREFER });
		return;
	}

	if (g_aimbot.m_headglitch) {
		m_hitboxes.push_back({ HITBOX_HEAD, HitscanMode::PREFER });
		return;
	}

	int baim_mode = HitscanMode::NORMAL;

	// prefer, always.
	if (m_settings.baim1.get(0))
		baim_mode = HitscanMode::PREFER;
	// prefer, fake.
	else if (m_settings.baim1.get(3) && !resolved)
		baim_mode = HitscanMode::PREFER;
	// prefer, in air.
	else if (m_settings.baim1.get(4) && !(record->m_flags & FL_ONGROUND))
		baim_mode = HitscanMode::PREFER;
	// prefer, lethal x2.
	else if (m_settings.baim1.get(2))
		baim_mode = HitscanMode::LETHAL2;
	// prefer, lethal.
	else if (m_settings.baim1.get(1))
		baim_mode = HitscanMode::LETHAL;

	std::vector< size_t > hitbox{ history ? m_settings.hitbox_history.GetActiveIndices() : m_settings.hitbox.GetActiveIndices() };

	if (hitbox.empty())
		return;

	for (const auto& h : hitbox) {
		// head.
		if (h == 0)
			m_hitboxes.push_back({ HITBOX_HEAD, HitscanMode::NORMAL });

		// stomach.
		if (h == 2) {
			m_hitboxes.push_back({ HITBOX_PELVIS, baim_mode });
			m_hitboxes.push_back({ HITBOX_BODY, baim_mode });
		}

		// chest.
		if (h == 1) {
			m_hitboxes.push_back({ HITBOX_THORAX, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_CHEST, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_UPPER_CHEST, HitscanMode::NORMAL });
		}

		// arms.
		if (h == 3) {
			m_hitboxes.push_back({ HITBOX_L_UPPER_ARM, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_R_UPPER_ARM, HitscanMode::NORMAL });
		}

		// legs.
		if (h == 4) {
			m_hitboxes.push_back({ HITBOX_L_THIGH, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_R_THIGH, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_L_CALF, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_R_CALF, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_L_FOOT, HitscanMode::NORMAL });
			m_hitboxes.push_back({ HITBOX_R_FOOT, HitscanMode::NORMAL });
		}
	}
}

void Aimbot::init() {
	// clear old targets.
	m_targets.clear();

	m_target = nullptr;
	m_aim = vec3_t{ };
	m_angle = ang_t{ };
	m_damage = -1.f;
	m_record = nullptr;
	m_stop = false;

	m_best_dist = std::numeric_limits< float >::max();
	m_best_fov = 180.f + 1.f;
	m_best_damage = 0.f;
	m_best_hp = 100 + 1;
	m_best_lag = std::numeric_limits< float >::max();
	m_best_height = std::numeric_limits< float >::max();
	m_best_resolve = -1;

	//UpdateShootpos(0.f);
}

void Aimbot::StripAttack() {
	if (g_cl.m_weapon_id == REVOLVER)
		g_cl.m_cmd->m_buttons &= ~IN_ATTACK2;

	else
		g_cl.m_cmd->m_buttons &= ~IN_ATTACK;
}

void Aimbot::UpdateSettings() {
	m_settings = g_menu.main.aimbot.general;

	switch (g_cl.m_weapon_type) {
	case WEAPONTYPE_SNIPER_RIFLE: {
		if (g_cl.m_weapon_id == G3SG1 || g_cl.m_weapon_id == SCAR20)
			m_settings = g_menu.main.aimbot.auto_sniper;
		else if (g_cl.m_weapon_id == AWP)
			m_settings = g_menu.main.aimbot.awp;
		else if (g_cl.m_weapon_id == SSG08)
			m_settings = g_menu.main.aimbot.scout;
	} break;
	case WEAPONTYPE_PISTOL:
	{
		if (g_cl.m_weapon_id == DEAGLE || g_cl.m_weapon_id == REVOLVER)
			m_settings = g_menu.main.aimbot.heavy_pistol;
		else
			m_settings = g_menu.main.aimbot.pistol;
	} break;
	}
}

void Aimbot::think() {
	for (int i = 0; i <= 255;) {
		InitializeSeed(i);
		++i;
	}

	init();

	if (!g_cl.m_weapon || !g_cl.m_weapon_info)
		return;

	if (g_cl.m_weapon_type == WEAPONTYPE_GRENADE || g_cl.m_weapon_type == WEAPONTYPE_C4)
		return;

	UpdateSettings();

	// we have no aimbot enabled.
	if (!g_menu.main.aimbot.enable.get())
		return;

	// no point in aimbotting if we cannot fire this tick.
	if (!g_cl.m_weapon_fire)
		return;

	// setup bones for all valid targets.
	for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

		if (!IsValidTarget(player))
			continue;

		AimPlayer* data = &m_players[i - 1];
		if (!data)
			continue;

		bool skip_player = false;

		for (auto id : g_cl.m_whitelist_ids) {
			if (i == id)
				skip_player = true;
		}

		if (skip_player)
			continue;

		// store player as potential target this tick.
		m_targets.emplace_back(data);
	}

	// run knifebot.	
	if (g_cl.m_weapon_type == WEAPONTYPE_KNIFE && g_cl.m_weapon_id != ZEUS) {
		return;
	}

	// scan available targets... if we even have any.
	find();

	// finally set data when shooting.
	apply();
}

float Aimbot::MinDamageSystemKurwa(float hp) {
	float dmg;
	const float BASE_HP = 100.f;


	if (g_cl.m_weapon_id == ZEUS) {
		dmg = hp + 1;
	}
	else {
			dmg = g_aimbot.m_override ? g_menu.main.aimbot.dmg_override_amount.get() : m_settings.minimal_damage.get();
		}

	//	goal_damage = g_aimbot.m_override ? g_menu.main.aimbot.dmg_override_amount.get() : m_settings.minimal_damage.get();

	//	if (goal_damage >= 100 || m_settings.minimal_damage.get())
	//		goal_damage = hp + (goal_damage - 100);
	//}

		// scale to player hp.
		dmg *= hp / 100.f;

	// make sure min damage is at least 1.
	return std::max(dmg, 1.f);
}



void Aimbot::find() {
	struct BestTarget_t { Player* player; vec3_t pos; float damage; int hitbox; LagRecord* record; };

	vec3_t       tmp_pos;
	float        tmp_damage;
	int          tmp_hitbox;
	BestTarget_t best;
	best.player = nullptr;
	best.damage = 0.f;
	best.hitbox = -1;
	best.pos = { };
	best.record = nullptr;

	if (m_targets.empty())
		return;

	if (g_cl.m_weapon_id == ZEUS && !g_menu.main.aimbot.aimbobs.get(1))
		return;
	// iterate all targets.
	for (const auto& t : m_targets) {
		if (t->m_records.empty())
			continue;

		LagRecord* ideal = g_resolver.FindIdealRecord(t);
		LagRecord* last = g_resolver.FindLastRecord(t);


		if (!ideal)
			continue;

		t->SetupHitboxes(ideal, false);
		if (t->m_hitboxes.empty())
			continue;

		// try to select best record as target.
		if (t->GetBestAimPosition(tmp_pos, tmp_damage, tmp_hitbox, ideal) && SelectTarget(ideal, tmp_pos, tmp_damage)) {
			// if we made it so far, set shit.
			best.player = t->m_player;
			best.pos = tmp_pos;
			best.damage = tmp_damage;
			best.hitbox = tmp_hitbox;
			best.record = ideal;
		}


		if (!last || last == ideal)
			continue;

		t->SetupHitboxes(last, true);
		if (t->m_hitboxes.empty())
			continue;

		// rip something went wrong..
		if (t->GetBestAimPosition(tmp_pos, tmp_damage, tmp_hitbox, last) && SelectTarget(last, tmp_pos, tmp_damage)) {
			// if we made it so far, set shit.
			best.player = t->m_player;
			best.pos = tmp_pos;
			best.damage = tmp_damage;
			best.hitbox = tmp_hitbox;
			best.record = last;
		}
	};

	// verify our target and set needed data.
	if (best.player && best.record && best.damage >= 1) {
		// calculate aim angle.


		update_shoot_pos();

		math::VectorAngles(best.pos - g_cl.m_shoot_pos, m_angle);


		// set member vars.
		m_target = best.player;
		m_aim = best.pos;
		m_damage = best.damage;
		m_hitbox = best.hitbox;
		m_record = best.record;

		// write data, needed for traces / etc.
		m_record->cache();

		bool on = m_settings.hitchance.get() && g_menu.main.config.mode.get() == 0;
		bool hit = on && CheckHitchance2(m_target, m_angle);


		// if we can scope.
		bool can_scope = !g_cl.m_local->m_bIsScoped() && (g_cl.m_weapon_id == AUG || g_cl.m_weapon_id == SG553 || g_cl.m_weapon_type == WEAPONTYPE_SNIPER_RIFLE);

		if (can_scope) {
			// always.
			if (g_menu.main.aimbot.zoom.get() == 1)
				g_cl.m_cmd->m_buttons |= IN_ATTACK2;
			// hitchance fail.
			else if (g_menu.main.aimbot.zoom.get() == 2 && on && !hit)
				g_cl.m_cmd->m_buttons |= IN_ATTACK2;
		}

		// set autostop shit.
		if (g_menu.main.aimbot.aimbobs.get(4) && on && !hit) {
			m_stop = true;

			if (m_stop) {
				g_movement.AutoStop();
			}
		}

		if (on && !hit && !(g_cl.m_cmd->m_buttons & IN_ATTACK) && !g_menu.main.misc.nospread.get())
			return;

		if (!g_menu.main.antiaim.lag_shot.get() && !g_cl.m_lag && g_cl.m_weapon_id != REVOLVER)
			return;

		g_cl.m_cmd->m_buttons |= IN_ATTACK;
	}
}

void Aimbot::InitializeSeed(int seed) {
	//if (m_spread_seeds[seed].initialized)
	//	return;

	g_csgo.RandomSeed((seed & 255) + 1);

	m_spread_seeds[seed].r1 = g_csgo.RandomFloat(0.f, 1.f);
	m_spread_seeds[seed].r2 = g_csgo.RandomFloat(0.f, math::pi_2);
	m_spread_seeds[seed].r3 = g_csgo.RandomFloat(0.f, 1.f);
	m_spread_seeds[seed].r4 = g_csgo.RandomFloat(0.f, math::pi_2);

	m_spread_seeds[seed].initialized = true;
}

bool Aimbot::CanHitHitbox(const vec3_t& start, const vec3_t& end, Player* player, int hitbox, matrix3x4_t* matrix)
{
	const model_t* model = player->GetModel();
	if (!model)
		return false;

	studiohdr_t* hdr = g_csgo.m_model_info->GetStudioModel(model);
	if (!hdr)
		return false;

	mstudiohitboxset_t* set = hdr->GetHitboxSet(player->m_nHitboxSet());
	if (!set)
		return false;

	mstudiobbox_t* bbox = set->GetHitbox(hitbox);
	if (!bbox)
		return false;

	vec3_t min{ }, max{ };

	math::VectorTransform(bbox->m_mins, matrix[bbox->m_bone], min);
	math::VectorTransform(bbox->m_maxs, matrix[bbox->m_bone], max);

	return math::SegmentToSegment(start, end, min, max) <= (bbox->m_radius == -1.f ? (min - max).length() : bbox->m_radius);
}

bool Aimbot::CheckHitchance2(Player* player, const ang_t& angle) {
	constexpr float HITCHANCE_MAX = 100.f;
	int   SEED_MAX = 256;

	vec3_t     start{ g_cl.m_shoot_pos }, end, fwd, right, up, dir, wep_spread;
	float      inaccuracy, spread;
	CGameTrace tr;
	size_t     total_hits{ }, needed_hits{ (size_t)std::ceil((m_settings.hitchance_amount.get() * SEED_MAX) / HITCHANCE_MAX) };

	math::AngleVectors(angle, &fwd, &right, &up);

	inaccuracy = g_cl.m_weapon->GetInaccuracy();
	spread = g_cl.m_weapon->GetSpread();
	float recoil_index = g_cl.m_weapon->m_flRecoilIndex();

	const vec3_t backup_origin = player->m_vecOrigin();
	const vec3_t backup_abs_origin = player->GetAbsOrigin();
	const ang_t backup_abs_angles = player->GetAbsAngles();
	const vec3_t backup_obb_mins = player->m_vecMins();
	const vec3_t backup_obb_maxs = player->m_vecMaxs();
	const auto backup_cache = player->m_BoneCache2();

	auto restore = [&]() -> void {
		player->m_vecOrigin() = backup_origin;
		player->SetAbsOrigin(backup_abs_origin);
		player->SetAbsAngles(backup_abs_angles);
		player->m_vecMins() = backup_obb_mins;
		player->m_vecMaxs() = backup_obb_maxs;
		player->m_BoneCache2() = backup_cache;
		};

	for (int i{ 0 }; i <= SEED_MAX; ++i) {
		wep_spread = g_cl.m_weapon->CalculateSpread(i, inaccuracy, spread);

		dir = (fwd + (right * wep_spread.x) + (up * wep_spread.y)).normalized();

		end = start + (dir * g_cl.m_weapon_info->m_range);

		player->m_vecOrigin() = m_record->m_pred_origin;
		player->SetAbsOrigin(m_record->m_pred_origin);
		player->SetAbsAngles(m_record->m_abs_ang);
		player->m_vecMins() = m_record->m_mins;
		player->m_vecMaxs() = m_record->m_maxs;
		player->m_BoneCache2() = reinterpret_cast<matrix3x4_t**>(m_record->m_bones);

		g_csgo.m_engine_trace->ClipRayToEntity(Ray(start, end), MASK_SHOT, player, &tr);

		restore();

		if (tr.m_entity == player && game::IsValidHitgroup(tr.m_hitgroup))
			++total_hits;

		if (total_hits >= needed_hits)
			return true;

		if ((SEED_MAX - i + total_hits) < needed_hits)
			return false;
	}

	for (int i{ 0 }; i <= SEED_MAX; ++i) {
		float a = g_csgo.RandomFloat(0.f, 1.f);
		float b = g_csgo.RandomFloat(0.f, math::pi * 2.f);
		float c = g_csgo.RandomFloat(0.f, 1.f);
		float d = g_csgo.RandomFloat(0.f, math::pi * 2.f);

		if (g_cl.m_weapon_id == REVOLVER) {
			a = 1.f - a * a;
			a = 1.f - c * c;
		}
		else if (g_cl.m_weapon_id == NEGEV && recoil_index < 3.0f) {
			for (int i = 3; i > recoil_index; i--) {
				a *= a;
				c *= c;
			}

			a = 1.0f - a;
			c = 1.0f - c;
		}

		float inac = a * inaccuracy;
		float sir = c * spread;

		vec3_t sirVec((cos(b) * inac) + (cos(d) * sir), (sin(b) * inac) + (sin(d) * sir), 0), direction;

		direction.x = fwd.x + (sirVec.x * right.x) + (sirVec.y * up.x);
		direction.y = fwd.y + (sirVec.x * right.y) + (sirVec.y * up.y);
		direction.z = fwd.z + (sirVec.x * right.z) + (sirVec.y * up.z);
		direction.normalize();

		ang_t viewAnglesSpread;
		math::VectorAngles(direction, viewAnglesSpread, &up);
		viewAnglesSpread.normalize();

		vec3_t viewForward;
		math::AngleVectors(viewAnglesSpread, &viewForward);
		viewForward.normalized();

		end = start + (viewForward * g_cl.m_weapon_info->m_range);

		player->m_vecOrigin() = m_record->m_pred_origin;
		player->SetAbsOrigin(m_record->m_pred_origin);
		player->SetAbsAngles(m_record->m_abs_ang);
		player->m_vecMins() = m_record->m_mins;
		player->m_vecMaxs() = m_record->m_maxs;
		player->m_BoneCache2() = reinterpret_cast<matrix3x4_t**>(m_record->m_bones);

		g_csgo.m_engine_trace->ClipRayToEntity(Ray(start, end), MASK_SHOT, player, &tr);

		restore();

		if (tr.m_entity == player && game::IsValidHitgroup(tr.m_hitgroup))
			++total_hits;

		if (total_hits >= needed_hits)
			return true;

		if ((SEED_MAX - i + total_hits) < needed_hits)
			return false;
	}
}


//bool Aimbot::CheckHitchance(Player* player, const ang_t& angle)
//{
//	constexpr float HITCHANCE_MAX = 100.f;
//	constexpr int   SEED_MAX = 255;
//
//	float hc = m_settings.hitchance_amount.get();
//
//	vec3_t     start{ g_cl.m_shoot_pos }, end, fwd, right, up, dir, wep_spread;
//	float      inaccuracy, spread;
//	CGameTrace tr;
//	float     total_hits{ }, needed_hits{ (hc / HITCHANCE_MAX) * SEED_MAX };
//
//	// get needed directional vectors.
//	math::AngleVectors(angle, &fwd, &right, &up);
//
//	// store off inaccuracy / spread ( these functions are quite intensive and we only need them once ).
//	inaccuracy = g_cl.m_weapon->GetInaccuracy();
//	spread = g_cl.m_weapon->GetSpread();
//
//	// get player hp.
//	float goal_damage = 1.f;
//	int hp = std::min(100, player->m_iHealth());
//
//	if (g_cl.m_weapon_id == ZEUS)
//	{
//		goal_damage = hp + 1;
//	}
//	else
//	{
//		goal_damage = g_aimbot.m_override ? g_menu.main.aimbot.dmg_override_amount.get() : m_settings.minimal_damage.get();
//
//		if (goal_damage >= 100 || m_settings.minimal_damage.get())
//			goal_damage = hp + (goal_damage - 100);
//	}
//
//	//if (g_aimbot.m_override) {
//	//	pendmg = dmg = g_menu.main.aimbot.dmg_override_amount.get();
//	//}
//	//else {
//	//	dmg = m_settings.minimal_damage.get();
//	//	pendmg = m_settings.penetrate_minimal_damage.get();
//	//}
//
//
//	// iterate all possible seeds.
//	for (int i{ }; i <= SEED_MAX; ++i)
//	{
//		// get spread.
//		wep_spread = g_cl.m_weapon->CalculateSpread(i, inaccuracy, spread);
//
//		// get spread direction.
//		dir = (fwd + (right * wep_spread.x) + (up * wep_spread.y)).normalized();
//
//		// get end of trace.
//		end = start + (dir * g_cl.m_weapon_info->m_range);
//
//		// setup ray and trace.
//		g_csgo.m_engine_trace->ClipRayToEntity(Ray(start, end), MASK_SHOT_HULL | CONTENTS_HITBOX, player, &tr);
//
//		// check if we hit a valid player / hitgroup on the player and increment total hits.
//		if (tr.m_entity == player && game::IsValidHitgroup(tr.m_hitgroup))
//		{
//			penetration::PenetrationInput_t in;
//			in.m_damage = 1.f;
//			in.m_damage_pen = 1.f;
//			in.m_can_pen = g_cl.m_weapon_id == ZEUS ? false : true;
//			in.m_target = player;
//			in.m_from = g_cl.m_local;
//			in.m_pos = end;
//
//			penetration::PenetrationOutput_t out;
//
//			bool hit = penetration::run(&in, &out);
//
//			if (hit) {
//				++total_hits;
//			}
//		}
//
//		// we cant make it anymore.
//		if ((SEED_MAX - i + total_hits) < needed_hits)
//			return false;
//	}
//
//	return total_hits >= needed_hits;
//}

bool AimPlayer::SetupHitboxPoints(float pitch, BoneArray* bones, int index, std::vector< PointData_t >& points) {
	// reset points.
	points.clear();

	const model_t* model = m_player->GetModel();
	if (!model)
		return false;

	studiohdr_t* hdr = g_csgo.m_model_info->GetStudioModel(model);
	if (!hdr)
		return false;

	mstudiohitboxset_t* set = hdr->GetHitboxSet(m_player->m_nHitboxSet());
	if (!set)
		return false;

	mstudiobbox_t* bbox = set->GetHitbox(index);
	if (!bbox)
		return false;

	if (!bones)
		return false;

	// get hitbox scales.
	float scale = index == HITBOX_HEAD ? (m_settings.scale.get() / 100.f) : (m_settings.body_scale.get() / 100.f);

	vec3_t center{};

	// these indexes represent boxes.
	if (bbox->m_radius <= 0.f) {
		// references: 
		//      https://developer.valvesoftware.com/wiki/Rotation_Tutorial
		//      CBaseAnimating::GetHitboxBonePosition
		//      CBaseAnimating::DrawServerHitboxes

		// convert rotation angle to a matrix.
		matrix3x4_t rot_matrix;
		g_csgo.AngleMatrix(bbox->m_angle, rot_matrix);

		// apply the rotation to the entity input space (local).
		matrix3x4_t matrix;
		math::ConcatTransforms(bones[bbox->m_bone], rot_matrix, matrix);

		// extract origin from matrix.
		vec3_t origin = matrix.GetOrigin();

		// compute raw center point.
		center = (bbox->m_mins + bbox->m_maxs) / 2.f;

		vec3_t dist = bbox->m_maxs - center;

		points.emplace_back(center, PointMode::CENTER);

		// the feet hiboxes have a side, heel and the toe.
		if (index == HITBOX_R_FOOT || index == HITBOX_L_FOOT) {
			float d1 = dist.z * 0.875f;
			float d2 = dist.x * 0.875f;

			// invert.
			if (index == HITBOX_L_FOOT)
				d1 *= -1.f;

			if (m_settings.multipoint.get(3)) {
				points.emplace_back(vec3_t(center.x, center.y, center.z + d1), PointMode::TOP);

				points.emplace_back(vec3_t(center.x + d2, center.y, center.z), PointMode::SIDE);

				points.emplace_back(vec3_t(center.x - d2, center.y, center.z), PointMode::SIDE);
			}
		}

		// nothing to do here we are done.
		if (points.empty())
			return false;

		// rotate our bbox points by their correct angle
		// and convert our points to world space.
		for (auto& p : points) {
			// VectorRotate.
			// rotate point by angle stored in matrix.
			p.m_pos = { p.m_pos.dot(matrix[0]), p.m_pos.dot(matrix[1]), p.m_pos.dot(matrix[2]) };

			// transform point to world space.
			p.m_pos += origin;
		}
	}

	// these hitboxes are capsules.
	else {
		center = (bbox->m_mins + bbox->m_maxs) / 2.f;

		points.emplace_back(center, PointMode::CENTER);

		const float radius = bbox->m_radius * scale;

		if (index == HITBOX_HEAD) {
			if (pitch >= 85.f)
				points.emplace_back(vec3_t(center.x + radius, center.y - radius, center.z), PointMode::TOP);

			points.emplace_back(vec3_t(center.x, center.y - radius, center.z), PointMode::TOP);
		}

		points.emplace_back(vec3_t(center.x, center.y, center.z + radius), PointMode::SIDE);
		points.emplace_back(vec3_t(center.x, center.y, center.z - radius), PointMode::SIDE);

		// nothing left to do here.
		if (points.empty())
			return false;

		// transform capsule points.
		for (auto& p : points)
			math::VectorTransform(p.m_pos, bones[bbox->m_bone], p.m_pos);
	}

	return true;
}

bool AimPlayer::GetBestAimPosition(vec3_t& aim, float& damage, int& hitbox, LagRecord* record) {
	bool                  pen;
	float                 dmg, pendmg;

	// get player hp.
	int hp = std::min(100, m_player->m_iHealth());

	bool can_hit = false;

	if (g_cl.m_weapon_id == ZEUS) {
		dmg = pendmg = hp + 5;
		pen = true;
	}

	else {
		if (g_aimbot.m_override) {
			pendmg = dmg = g_menu.main.aimbot.dmg_override_amount.get();
		}
		else {
			dmg = m_settings.minimal_damage.get();
			pendmg = m_settings.penetrate_minimal_damage.get();
		}

		if (dmg >= 100)
			dmg += hp - 100;
		else if (m_settings.minimal_damage_hp.get() && dmg > hp)
			dmg = hp;

		if (pendmg >= 100)
			pendmg += hp - 100;
		else if (m_settings.minimal_damage_hp.get() && pendmg > hp)
			pendmg = hp;

		pen = m_settings.penetrate.get();
	}


	const auto sort_points = [](const PointData_t a, const PointData_t b) -> bool {
		return a.m_mode < b.m_mode;
		};

	if (!record)
		return false;

	// write all data of this record.
	record->cache();

	bool done = false;
	int best_damage = 0;

	// iterate hitboxes.
	for (auto& it : m_hitboxes) {
		std::vector< PointData_t > points;
		// setup points on hitbox.
		if (!SetupHitboxPoints(record->m_eye_angles.x, record->m_bones, it.m_index, points))
			continue;

		// iterate points on hitbox.
		for (auto& point : points) {
			penetration::PenetrationInput_t in;

			in.m_damage = dmg;
			in.m_damage_pen = pendmg;
			in.m_can_pen = pen;
			in.m_target = m_player;
			in.m_from = g_cl.m_local;
			in.m_pos = point.m_pos;

			penetration::PenetrationOutput_t out;

			if (penetration::run(&in, &out) && (it.m_index == out.m_hitbox || g_cl.m_weapon_id == ZEUS)) {
				point.m_hitbox = out.m_hitbox;
				point.m_damage = out.m_damage;
				point.m_pen = out.m_pen;
			}
		}

		// sort by point mode
		std::sort(points.begin(), points.end(), sort_points);

		// scan points.
		for (auto& point : points) {
			if (point.m_damage < (point.m_pen ? pendmg : dmg))
				continue;

			if (point.m_damage > hp && point.m_mode == PointMode::CENTER && it.m_mode == HitscanMode::LETHAL)
				done = true;

			if (point.m_damage * 2 > hp && point.m_mode == PointMode::CENTER && it.m_mode == HitscanMode::LETHAL2)
				done = true;

			if (point.m_mode == PointMode::CENTER && it.m_mode == HitscanMode::PREFER)
				done = true;

			if (point.m_damage > best_damage)
				best_damage = point.m_damage;

			else if (!done && point.m_damage <= best_damage)
				continue;

			can_hit = true;

			aim = point.m_pos;
			damage = point.m_damage;
			hitbox = point.m_hitbox;

			if (point.m_damage > hp || done)
				break;
		}

		if (done)
			break;
	}

	return can_hit;
}

bool Aimbot::SelectTarget(LagRecord* record, const vec3_t& aim, float damage) {
	float dist, fov, height;
	int   hp;


	switch (g_menu.main.aimbot.selection.get()) {

		// distance.
	case 0:
		dist = (record->m_origin - g_cl.m_shoot_pos).length();

		if (dist < m_best_dist) {
			m_best_dist = dist;
			return true;
		}

		break;

		// crosshair.
	case 1:
		fov = math::GetFOV(g_cl.m_view_angles, g_cl.m_shoot_pos, aim);

		if (fov < m_best_fov) {
			m_best_fov = fov;
			return true;
		}

		break;

		// damage.
	case 2:
		if (damage > m_best_damage) {
			m_best_damage = damage;
			return true;
		}

		break;

		// lowest hp.
	case 3:
		// fix for retarded servers?
		hp = std::min(100, record->m_player->m_iHealth());

		if (hp < m_best_hp) {
			m_best_hp = hp;
			return true;
		}

		break;

		// least lag.
	case 4:
		if (record->m_lag < m_best_lag) {
			m_best_lag = record->m_lag;
			return true;
		}

		break;

		// height.
	case 5:
		height = record->m_origin.z - g_cl.m_local->m_vecOrigin().z;

		if (height < m_best_height) {
			m_best_height = height;
			return true;
		}

		break;
		// resolver.
	case 6:
		if (m_best_resolve == -1) {
			m_best_resolve = record->m_mode;
			return true;
		}
		else if (record->m_mode != m_best_resolve) {
			switch (record->m_mode) {
			case Resolver::Modes::RESOLVE_DISABLED:
				m_best_resolve = record->m_mode;
				return true;
			case Resolver::Modes::RESOLVE_WALK:
				m_best_resolve = record->m_mode;
				return true;
			case Resolver::Modes::RESOLVE_PREFLICK:
				m_best_resolve = record->m_mode;
				return true;
			case Resolver::Modes::RESOLVE_FLICK:
				m_best_resolve = record->m_mode;
				return true;
			}
		}
		break;

	default:
		return false;
	}

	return false;
}

void Aimbot::apply() {
	if (!m_target || !m_record || m_damage < 1)
		return;


	g_cl.m_shot = true;

	// make sure to aim at un-interpolated data.
	// do this so BacktrackEntity selects the exact record.
	g_cl.m_cmd->m_tick = game::TIME_TO_TICKS(m_record->m_sim_time + g_cl.m_lerp);

	// set angles to target.
	g_cl.m_cmd->m_view_angles = m_angle;

	// if not silent aim, apply the viewangles.
	if (!g_menu.main.aimbot.aimbobs.get(0))
		return
		g_csgo.m_engine->SetViewAngles(m_angle);

	// norecoil.
	if (g_menu.main.aimbot.aimbobs.get(3))
		g_cl.m_cmd->m_view_angles -= g_cl.m_local->m_aimPunchAngle() * g_csgo.weapon_recoil_scale->GetFloat();

	switch (g_menu.main.aimbot.shot_matrix.get()) {
	case 1:
		g_visuals.DrawHitboxMatrix(m_record, g_menu.main.aimbot.shot_matrix_color.get(), 4.f);
		break;
	}

	g_chams.m_shots.emplace_back(ShotMatrix(m_target->index(), g_csgo.m_globals->m_curtime, m_record->m_bones));

	if (g_menu.main.misc.notifications.get(2)) {
		player_info_t info;
		g_csgo.m_engine->GetPlayerInfo(m_target->index(), &info);

		std::string shot_data = "fired shot at: ";

		shot_data += info.m_name;
		shot_data += ". ";

		/*shot_data += "hb: " + g_shots.m_groups[m_hitbox];
		shot_data += ". ";*/

		shot_data += "dmg: " + std::to_string((int)m_damage);
		shot_data += ". ";

		shot_data += "bt: " + std::to_string(g_csgo.m_globals->m_tick_count - game::TIME_TO_TICKS(m_record->m_sim_time));
		shot_data += ". ";

		shot_data += "mode: " + std::to_string(m_record->m_mode);
		shot_data += ". ";

		shot_data += "vel: " + std::to_string((int)m_record->m_abs_velocity[1].length());
		shot_data += ". ";

		shot_data += "\n";

		g_notify.add(shot_data);
	}
}

void Aimbot::NoSpread() {
	//bool    attack2;
	//vec3_t  spread, forward, right, up, dir;

	//// revolver state.
	//attack2 = (g_cl.m_weapon_id == REVOLVER && (g_cl.m_cmd->m_buttons & IN_ATTACK2));

	//// get spread.
	//spread = g_cl.m_weapon->CalculateSpread(g_cl.m_cmd->m_random_seed, attack2);

	//// compensate.
	//g_cl.m_cmd->m_view_angles -= { -math::rad_to_deg(std::atan(spread.length_2d())), 0.f, math::rad_to_deg(std::atan2(spread.x, spread.y)) };
}
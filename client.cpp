#include "includes.h"


Client g_cl{ };
ServerAnimations g_server_animations{ };

// loader will set this fucker.
char username[33] = "\x90\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x90";



bool run_cheat = false;

// init routine.
ulong_t __stdcall Client::init(void* arg) {
#ifdef RELEASE
	MainThread();

	if (!run_cheat)
		return 0;
#endif // RELEASE

	// if not in interwebz mode, the driver will not set the username.
	g_cl.m_user = XOR("i was up late night ballin");

	// stop here if we failed to acquire all the data needed from csgo.
	if (!g_csgo.init())
		return 0;

	// welcome the user.
	g_notify.add(XOR("welcome\n"));

	return 1;
}

void Client::DrawHUD() {
	if (!g_csgo.m_engine->IsInGame())
		return;

	if (!g_menu.main.misc.watermark.get())
		return;

	// get time.
	time_t t = std::time(nullptr);
	std::ostringstream time;
	time << std::put_time(std::localtime(&t), ("%H:%M:%S"));

	// get round trip time in milliseconds.
	int ms = std::max(0, (int)std::round(g_cl.m_latency * 1000.f));

	// get tickrate.
	int rate = (int)std::round(1.f / g_csgo.m_globals->m_interval);

	std::string text = tfm::format(XOR("i was up late night ballin | rtt: %ims"), ms);
	render::FontSize_t size = render::hud.size(text);

	// background.
	render::rect_filled(m_width - size.m_width - 20, 10, size.m_width + 10, size.m_height + 2, { 240, 110, 140, 130 });

	// text.
	render::hud.string(m_width - 15, 10, { 240, 160, 180, 250 }, text, render::ALIGN_RIGHT);
}


void Client::UnlockHiddenConvars()
{
	if (!g_csgo.m_cvar)
		return;

	auto p = **reinterpret_cast<ConVar***>(g_csgo.m_cvar + 0x34);
	for (auto c = p->m_next; c != nullptr; c = c->m_next) {
		c->m_flags &= ~FCVAR_DEVELOPMENTONLY;
		c->m_flags &= ~FCVAR_HIDDEN;
	}
}


void Client::Skybox()
{
	static auto sv_skyname = g_csgo.m_cvar->FindVar(HASH("sv_skyname"));
	if (g_menu.main.misc.skyboxchange.get()) {
		switch (g_menu.main.misc.skybox.get()) {
		case 0: //Tibet
			//sv_skyname->SetValue("cs_tibet");
			sv_skyname->SetValue(XOR("cs_tibet"));
			break;
		case 1: //Embassy
			//sv_skyname->SetValue("embassy");
			sv_skyname->SetValue(XOR("embassy"));
			break;
		case 2: //Italy
			//sv_skyname->SetValue("italy");
			sv_skyname->SetValue(XOR("italy"));
			break;
		case 3: //Daylight 1
			//sv_skyname->SetValue("sky_cs15_daylight01_hdr");
			sv_skyname->SetValue(XOR("sky_cs15_daylight01_hdr"));
			break;
		case 4: //Cloudy
			//sv_skyname->SetValue("sky_csgo_cloudy01");
			sv_skyname->SetValue(XOR("sky_csgo_cloudy01"));
			break;
		case 5: //Night 1
			sv_skyname->SetValue(XOR("sky_csgo_night02"));
			break;
		case 6: //Night 2
			//sv_skyname->SetValue("sky_csgo_night02b");
			sv_skyname->SetValue(XOR("sky_csgo_night02b"));
			break;
		case 7: //Night Flat
			//sv_skyname->SetValue("sky_csgo_night_flat");
			sv_skyname->SetValue(XOR("sky_csgo_night_flat"));
			break;
		case 8: //Day HD
			//sv_skyname->SetValue("sky_day02_05_hdr");
			sv_skyname->SetValue(XOR("sky_day02_05_hdr"));
			break;
		case 9: //Day
			//sv_skyname->SetValue("sky_day02_05");
			sv_skyname->SetValue(XOR("sky_day02_05"));
			break;
		case 10: //Rural
			//sv_skyname->SetValue("sky_l4d_rural02_ldr");
			sv_skyname->SetValue(XOR("sky_l4d_rural02_ldr"));
			break;
		case 11: //Vertigo HD
			//sv_skyname->SetValue("vertigo_hdr");
			sv_skyname->SetValue(XOR("vertigo_hdr"));
			break;
		case 12: //Vertigo Blue HD
			//sv_skyname->SetValue("vertigoblue_hdr");
			sv_skyname->SetValue(XOR("vertigoblue_hdr"));
			break;
		case 13: //Vertigo
			//sv_skyname->SetValue("vertigo");
			sv_skyname->SetValue(XOR("vertigo"));
			break;
		case 14: //Vietnam
			//sv_skyname->SetValue("vietnam");
			sv_skyname->SetValue(XOR("vietnam"));
			break;
		case 15: //Dusty Sky
			//sv_skyname->SetValue("sky_dust");
			sv_skyname->SetValue(XOR("sky_dust"));
			break;
		case 16: //Jungle
			sv_skyname->SetValue(XOR("jungle"));
			break;
		case 17: //Nuke
			sv_skyname->SetValue(XOR("nukeblank"));
			break;
		case 18: //Office
			sv_skyname->SetValue(XOR("office"));
			//game::SetSkybox(XOR("office"));
			break;
		default:
			break;
		}
	}
}
void Client::KillFeed() {
	if (!g_menu.main.misc.killfeed.get())
		return;

	if (!g_csgo.m_engine->IsInGame())
		return;

	// get the addr of the killfeed.
	KillFeed_t* feed = (KillFeed_t*)g_csgo.m_hud->FindElement(HASH("SFHudDeathNoticeAndBotStatus"));
	if (!feed)
		return;

	int size = feed->notices.Count();
	if (!size)
		return;

	for (int i{ }; i < size; ++i) {
		NoticeText_t* notice = &feed->notices[i];

		// this is a local player kill, delay it.
		if (notice->fade == 1.5f)
			notice->fade = FLT_MAX;
	}
}

void Client::OnPaint() {
	// update screen size.
	g_csgo.m_engine->GetScreenSize(m_width, m_height);

	// render stuff.
	g_visuals.think();
	g_grenades.paint();
	g_notify.think();

	DrawHUD();
	KillFeed();

	// menu goes last.
	g_gui.think();
}

void Client::OnMapload() {
	m_server_data.reset();

	// store class ids.
	g_netvars.SetupClassData();

	// createmove will not have been invoked yet.
	// but at this stage entites have been created.
	// so now we can retrive the pointer to the local player.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	// world materials.
	Visuals::ModulateWorld();

	// init knife shit.
	g_skins.load();

	m_sequences.clear();
	m_sent_cmds.clear();

	m_taps = 0;

	// if the INetChannelInfo pointer has changed, store it for later.
	g_csgo.m_net = g_csgo.m_engine->GetNetChannelInfo();
}

void Client::StartMove(CUserCmd* cmd) {
	// save some usercmd stuff.
	m_cmd = cmd;
	m_tick = cmd->m_tick;
	m_view_angles = cmd->m_view_angles;
	m_buttons = cmd->m_buttons;

	// reset shit.
	m_weapon = nullptr;
	m_weapon_info = nullptr;
	m_weapon_id = -1;
	m_weapon_type = WEAPONTYPE_UNKNOWN;
	m_player_fire = m_weapon_fire = false;

	// store weapon stuff.
	m_weapon = m_local->GetActiveWeapon();

	// store max choke
	// TODO; 11 -> m_bIsValveDS
	m_max_lag = (g_csgo.sv_maxusrcmdprocessticks ? g_csgo.sv_maxusrcmdprocessticks->GetInt() : 16) - 1;
	m_fake_walk = g_input.GetKeyState(g_menu.main.movement.fakewalk.get());
	m_fake_walk_ticks = std::min((int)g_menu.main.movement.fakewalk_speed.get(), m_max_lag);
	m_lag = g_csgo.m_cl->m_choked_commands;
	m_lerp = game::GetClientInterpAmount();
	m_latency = g_csgo.m_net->GetLatency(INetChannel::FLOW_OUTGOING);
	m_latency_ticks = game::TIME_TO_TICKS(m_latency);
	m_server_tick = g_csgo.m_cl->m_server_tick;
	m_arrival_tick = m_server_tick + m_latency_ticks;
	g_movement.m_autopeek = g_input.GetKeyState(g_menu.main.movement.fakewalk.get());

	static bool old_override_resolver;
	m_override_resolver = g_input.GetKeyState(g_menu.main.aimbot.correct_override.get());

	if (old_override_resolver != m_override_resolver && m_override_resolver) {
		callbacks::UpdateOverrideResolver();
	}

	old_override_resolver = m_override_resolver;

	// make sure prediction has ran on all usercommands.
	// because prediction runs on frames, when we have low fps it might not predict all usercommands.
	// also fix the tick being inaccurate.
	g_inputpred.update();

	// ...
	m_shot = false;
}

void Client::BackupPlayers(bool restore) {
	if (restore) {
		// restore stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].restore(player);
		}
	}

	else {
		// backup stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].store(player);
		}
	}
}

void Client::StoreAnimData(C_AnimationLayer* layers, float* poses, ang_t& angle, float& yaw) {
	std::memcpy(layers, m_layers, sizeof(C_AnimationLayer) * 13);
	std::memcpy(poses, m_poses, sizeof(float) * 24);

	angle = m_angle;
	yaw = m_abs_yaw;
}

void Client::SetAnimData(C_AnimationLayer* layers, float* poses, ang_t& angle, float& yaw) {
	std::memcpy(m_layers, layers, sizeof(C_AnimationLayer) * 13);
	std::memcpy(m_poses, poses, sizeof(float) * 24);

	m_angle = angle;
	m_abs_yaw = yaw;
}

void Client::UpdateShootPos() {
	if (!m_local || !m_processing)
		return;

	if (!m_server_data.m_updated) {
		m_local->GetEyePos(&m_shoot_pos);
		return;
	}

	C_AnimationLayer backup_layers[13];
	float backup_poses[24];
	BoneArray backup_bones[128];
	CCSGOPlayerAnimState backup_state;
	CCSGOPlayerAnimState* state = m_local->m_PlayerAnimState();
	if (!state)
		return;

	serveranimation_data_t shot_server_data = m_server_data;

	const float backup_curtime = g_csgo.m_globals->m_curtime;
	const float backup_frametime = g_csgo.m_globals->m_frametime;

	const int eflags = m_local->m_iEFlags();
	const vec3_t abs_velocity = m_local->m_vecAbsVelocity();

	const vec3_t backup_abs_origin = m_local->GetAbsOrigin();
	const ang_t backup_angle = m_local->GetAbsAngles();

	g_csgo.m_globals->m_curtime = game::TICKS_TO_TIME(m_local->m_nTickBase());
	g_csgo.m_globals->m_frametime = g_csgo.m_globals->m_interval;

	m_local->GetAnimState(&backup_state);
	m_local->GetAnimLayers(backup_layers);
	m_local->GetPoseParameters(backup_poses);
	m_local->GetBones(backup_bones);

	m_local->SetAnimLayers(m_layers);
	m_local->SetPoseParameters(m_poses);

	m_local->SetAbsOrigin(m_local->m_vecOrigin());

	if (!m_lag) {
		UpdateClientAnimations::DoUpdate(m_local);
		g_server_animations.RebuildLayers(m_local, &shot_server_data);

		m_local->SetPoseParameters(shot_server_data.m_poses);

		m_local->m_flPoseParameter()[12] = 0.5f;

		g_hvh.m_desync_delta = m_local->max_abs_yaw();
	}

	g_server_animations.DoAnimationEvent(m_local, &shot_server_data);
	m_local->SetAnimLayers(shot_server_data.m_layers);

	m_local->SetAbsAngles({ 0, state->m_abs_yaw, 0 });

	g_bones.BuildBones(m_local, BONE_USED_BY_ANYTHING, m_local->m_BoneCache().m_pCachedBones, true);

	m_shoot_pos = m_local->GetShootPosition();

	m_local->SetAnimState(&backup_state);
	m_local->SetAnimLayers(backup_layers);
	m_local->SetPoseParameters(backup_poses);
	m_local->SetBones(backup_bones);

	m_local->SetAbsOrigin(backup_abs_origin);
	m_local->SetAbsAngles(backup_angle);

	g_csgo.m_globals->m_curtime = backup_curtime;
	g_csgo.m_globals->m_frametime = backup_frametime;

	m_local->m_iEFlags() = eflags;
	m_local->m_vecAbsVelocity() = abs_velocity;
}

void Client::UpdateShot() {
	m_shot = m_weapon_fire && ((m_cmd->m_buttons & IN_ATTACK) || ((m_cmd->m_buttons & IN_ATTACK) && (m_weapon_id == REVOLVER || m_weapon_type == WEAPONTYPE_KNIFE)));
}

void Client::DoMove() {
	penetration::PenetrationOutput_t tmp_pen_data{ };

	// backup strafe angles (we need them for input prediction)
	m_strafe_angles = m_cmd->m_view_angles;

	g_movement.DoMovement();

	// predict input.
	g_inputpred.run(false);

	// restore original angles after input prediction
	m_cmd->m_view_angles = m_view_angles;

	// convert viewangles to directional forward vector.
	math::AngleVectors(m_view_angles, &m_forward_dir);

	// store stuff after input pred.

	m_speed = m_local->m_vecVelocity().length_2d();
	m_ground = m_local->m_fFlags() & FL_ONGROUND;
	//
	UpdateShootPos();

//	g_cl.m_shoot_pos = g_cl.m_local->GetShootPosition(); // get corrected shootpos

	if (m_weapon) {
		m_weapon_info = m_weapon->GetWpnData();
		m_weapon_id = m_weapon->m_iItemDefinitionIndex();

		// run autowall once for penetration crosshair if we have an appropriate weapon.
		if (m_weapon_info) {
			m_weapon_type = m_weapon_info->m_weapon_type;

			if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon_type != WEAPONTYPE_C4 && m_weapon_type != WEAPONTYPE_GRENADE) {
				penetration::PenetrationInput_t in;
				in.m_from = m_local;
				in.m_target = nullptr;
				in.m_pos = m_shoot_pos + (m_forward_dir * m_weapon_info->m_range);
				in.m_damage = 1.f;
				in.m_damage_pen = 1.f;
				in.m_can_pen = true;

				// run autowall.
				penetration::run(&in, &tmp_pen_data);
			}
		}

		if (g_cl.m_old_shot
			&& g_input.GetKeyState(g_menu.main.movement.autopeek.get())
			&& (g_menu.main.aimbot.aimbobs.get(2))) {
			auto weapons = g_cl.m_local->m_hMyWeapons();
			for (auto i = 0; weapons[i].IsValid(); i++)
			{
				Weapon* weapon = (Weapon*)g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(weapons[i]);
				if (!weapon)
					continue;

				if (weapon->IsKnife())
					m_cmd->m_weapon_select = weapon->index();
			}
		}


		// set pen data for penetration crosshair.
		m_pen_data = tmp_pen_data;

		// can the player fire.
		m_player_fire = game::TICKS_TO_TIME(m_local->m_nTickBase()) >= m_local->m_flNextAttack() && !g_csgo.m_gamerules->m_bFreezePeriod() && !(g_cl.m_unpred_netvars.flags & FL_FROZEN);

		UpdateRevolverCock();
		m_weapon_fire = CanFireWeapon();
	}

	if (!m_weapon_fire && m_weapon_id != REVOLVER && m_weapon_type != WEAPONTYPE_GRENADE)
		m_cmd->m_buttons &= ~(IN_ATTACK | IN_ATTACK2);

	UpdateShot();

	// last tick defuse.
	// todo - dex;  figure out the range for CPlantedC4::Use?
	//              add indicator if valid (on ground, still have time, not being defused already, etc).
	//              move this? not sure where we should put it.
	if (g_input.GetKeyState(g_menu.main.misc.last_tick_defuse.get()) && g_visuals.m_c4_planted) {
		float defuse = (m_local->m_bHasDefuser()) ? 5.f : 10.f;
		float remaining = g_visuals.m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;
		float dt = remaining - defuse - (m_latency / 2.f);

		m_cmd->m_buttons &= ~IN_USE;
		if (dt <= game::TICKS_TO_TIME(2))
			m_cmd->m_buttons |= IN_USE;
	}

	// grenade prediction.
	g_grenades.think();

	// run aimbot.
	g_aimbot.think();

	UpdateShot();

	if (g_cl.m_weapon && g_cl.m_weapon_info && g_cl.m_shot) {
		if (g_aimbot.m_target && g_aimbot.m_record)
			g_shots.OnShotFire(g_aimbot.m_target, g_aimbot.m_damage, g_aimbot.m_record, g_aimbot.m_aim);
		else {
			vec3_t dir;
			math::AngleVectors(g_cl.m_view_angles, &dir);

			dir *= g_cl.m_weapon_info->m_range;


			g_shots.OnShotFire(nullptr, -1, nullptr, g_cl.m_shoot_pos + dir);
		}
	}

	// run fakelag.
	g_hvh.SendPacket();

	// run antiaims.
	g_hvh.AntiAim();
}

void Client::EndMove(CUserCmd* cmd) {
	WriteUserCmdDeltaToBuffer::LastShift = WriteUserCmdDeltaToBuffer::Shift;

	// if matchmaking mode, anti untrust clamp.
	m_cmd->m_view_angles.SanitizeAngle();
	// fix our movement.
	g_movement.FixMove(cmd, m_strafe_angles);

	// this packet will be sent.
	if (*m_packet) {
		g_hvh.m_step_switch = (bool)g_csgo.RandomInt(0, 1);

		// we are sending a packet, so this will be reset soon.
		// store the old value.
		m_old_lag = m_lag;

		// get current origin.
		vec3_t cur = m_local->m_vecOrigin();

		// get prevoius origin.
		vec3_t prev = m_net_pos.empty() ? cur : m_net_pos.front().m_pos;

		// check if we broke lagcomp.
		m_lagcomp = (cur - prev).length_sqr() > 4096.f;

		// save sent origin and time.
		m_net_pos.emplace_front(g_csgo.m_globals->m_curtime, cur);
	}

	// store some values for next tick.
	m_old_packet = *m_packet;
	m_old_shot = m_shot;

	// update client-side animations.
	UpdateInformation();

	if (!m_override_resolver_toggle) {
		g_resolver.m_override_pos = g_cl.m_shoot_pos;
		g_resolver.m_override_viewangles = m_view_angles;
	}

	// restore curtime/frametime
	// and prediction seed/player.
	g_inputpred.restore();
}

void Client::OnTick(CUserCmd* cmd) {
	// TODO; add this to the menu.
	if (g_menu.main.misc.ranks.get() && cmd->m_buttons & IN_SCORE) {
		static CCSUsrMsg_ServerRankRevealAll msg{ };
		g_csgo.ServerRankRevealAll(&msg);
	}

	// save the original state of players.
	BackupPlayers(false);

	// store some data and update prediction.
	StartMove(cmd);

	// run all movement related code.
	DoMove();

	// store stome additonal stuff for next tick
	// sanetize our usercommand if needed and fix our movement.
	EndMove(cmd);

	// restore the players.
	BackupPlayers(true);
}

void Client::UpdateAnimations() {
	if (!m_local || !m_processing)
		return;

	CCSGOPlayerAnimState* state = m_local->m_PlayerAnimState();
	if (!state)
		return;

	static C_AnimationLayer layers[13];
	static float            poses[24];
	static float            angle;

	std::memcpy(layers, m_layers, sizeof(C_AnimationLayer) * 13);

	if ((!m_body_update && !m_pre_body_update) || !g_menu.main.misc.dont_render_body_update.get()) {
		// apply the rotation.
		if (g_menu.main.misc.interpolate_local.get()) {
			for (int i = 0; i < 24; i++) {
				switch (i) {
				case POSE_SPEED:
				case POSE_JUMP_FALL:
				case POSE_BODY_YAW:
				case POSE_BODY_PITCH:
					poses[i] = math::Approach(m_poses[i], poses[i], 4.f * g_csgo.m_globals->m_frametime * 1.0f);
					break;
				default:
					poses[i] = m_poses[i];
					break;
				}
			}

			angle = math::ApproachAngle(m_abs_yaw, angle, 720.f * g_csgo.m_globals->m_frametime * 1.6f);
		}
		else {
			for (int i = 0; i < 24; i++)
				poses[i] = m_poses[i];

			angle = m_abs_yaw;
		}
	}

	m_local->SetAbsAngles({ 0, angle, 0 });
	m_local->SetPoseParameters(poses);
	m_local->SetAnimLayers(layers);
}


void Client::StoreLayers(C_AnimationLayer* layers) {
	if (!m_local || !m_processing)
		return;

	//g_notify.add(std::to_string(g_cl.m_local->GetSequenceActivity(m_local->m_AnimOverlay()[ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL].m_sequence)));

	for (int i{ ANIMATION_LAYER_AIMMATRIX }; i < ANIMATION_LAYER_COUNT; i++) {
		if (i == ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ||
			i == ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ||
			i == ANIMATION_LAYER_MOVEMENT_STRAFECHANGE ||
			i == ANIMATION_LAYER_ADJUST ||
			i == ANIMATION_LAYER_WHOLE_BODY ||
			i == ANIMATION_LAYER_ALIVELOOP ||
			i == ANIMATION_LAYER_MOVEMENT_MOVE ||
			i == ANIMATION_LAYER_LEAN)
			continue;

		layers[i] = m_local->m_AnimOverlay()[i];
	}
}

void Client::UpdateLayers(C_AnimationLayer* layers) {
	if (!m_local || !m_processing)
		return;

	for (int i{ ANIMATION_LAYER_AIMMATRIX }; i < ANIMATION_LAYER_COUNT; i++) {
		if (i != ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL &&
			i != ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB &&
			i != ANIMATION_LAYER_MOVEMENT_STRAFECHANGE &&
			i != ANIMATION_LAYER_ADJUST &&
			i != ANIMATION_LAYER_WHOLE_BODY &&
			i != ANIMATION_LAYER_ALIVELOOP &&
			i != ANIMATION_LAYER_MOVEMENT_MOVE &&
			i != ANIMATION_LAYER_LEAN)
			continue;

		layers[i] = m_server_data.m_layers[i];
	}
}

void Client::UpdateInformation() {
	if (!m_local || !m_processing)
		return;

	CCSGOPlayerAnimState* state = m_local->m_PlayerAnimState();
	if (!state)
		return;

	if (!m_lag) {
		const int eflags = m_local->m_iEFlags();
		const vec3_t abs_velocity = m_local->m_vecAbsVelocity();

		// update time.
		m_anim_frame = game::TICKS_TO_TIME(m_local->m_nTickBase()) - m_anim_time;
		m_anim_time = game::TICKS_TO_TIME(m_local->m_nTickBase());

		// current angle will be animated.
		m_angle = m_cmd->m_view_angles;

		math::clamp(m_angle.x, -90.f, 90.f);
		m_angle.normalize();

		// set lby to predicted value.
		m_local->m_flLowerBodyYawTarget() = m_server_data.m_body;

		// CCSGOPlayerAnimState::Update, bypass already animated checks.
		if (state->m_last_update_frame == g_csgo.m_globals->m_frame)
			--state->m_last_update_frame;

		m_local->m_iEFlags() &= ~0x1000;
		//m_local->m_vecAbsVelocity() = m_local->m_vecVelocity();

		// call original, bypass hook.
		UpdateClientAnimations::DoUpdate(m_local);

		m_shot_update = m_shot;

		g_server_animations.RebuildLayers(m_local, &m_server_data);

		m_local->InvalidatePhysicsRecursive(POSITION_CHANGED | ANGLES_CHANGED | VELOCITY_CHANGED | ANIMATION_CHANGED);

		// store updated abs yaw.
		m_abs_yaw = state->m_abs_yaw;

		m_body_update = m_server_data.m_body_update != m_body_pred && m_server_data.m_vel.length_2d() < 0.1f;
		m_pre_body_update = m_server_data.m_body_update <= (m_anim_time + m_anim_frame) &&
			m_server_data.m_body_update > m_anim_time &&
			m_server_data.m_vel.length_2d() < 0.1f;

		if (m_body_update) {
			++m_body_updates;

			g_hvh.m_distortion_add = rand() % 2 ? 18.f : 5.f;
		}

		if (state->m_velocity_length_xy <= 0.0f)
			m_last_standing_yaw = m_angle.y;

		m_body_pred = m_server_data.m_body_update;
		m_body = m_server_data.m_body;

		m_local->m_iEFlags() = eflags;
		m_local->m_vecAbsVelocity() = abs_velocity;
	}

	g_server_animations.DoAnimationEvent(m_local, &m_server_data);

	std::memcpy(m_poses, m_server_data.m_poses, sizeof(float) * 24);
	UpdateLayers(m_layers);
}

void Client::print(const std::string text, ...) {
	va_list     list;
	int         size;
	std::string buf;

	if (text.empty())
		return;

	va_start(list, text);

	// count needed size.
	size = std::vsnprintf(0, 0, text.c_str(), list);

	// allocate.
	buf.resize(size);

	// print to buffer.
	std::vsnprintf(buf.data(), size + 1, text.c_str(), list);

	va_end(list);

	// print to console.
	g_csgo.m_cvar->ConsoleColorPrintf(g_gui.m_color, g_menu.main.m_mode == 0 ? XOR("[source] ") : XOR("[source] "));
	g_csgo.m_cvar->ConsoleColorPrintf(colors::white, buf.c_str());
}

bool Client::CanFireWeapon() {
	// the player cant fire.
	if (!m_player_fire || !m_weapon || !m_weapon_info)
		return false;

	if (m_weapon_type == WEAPONTYPE_GRENADE)
		return false;

	// if we have no bullets, we cant shoot.
	if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon->m_iClip1() < 1)
		return false;

	float curtime = game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase());

	// do we have any burst shots to handle?
	if ((m_weapon_id == GLOCK || m_weapon_id == FAMAS) && m_weapon->m_iBurstShotsRemaining() > 0) {
		// new burst shot is coming out.
		if (curtime >= m_weapon->m_fNextBurstShot())
			return true;
	}

	// r8 revolver.
	if (m_weapon_id == REVOLVER) {
		int act = m_weapon->m_Activity();

		// mouse1.
		if (!m_revolver_fire) {
			if ((act == 185 || act == 193) && m_revolver_cock == 0)
				return curtime >= m_weapon->m_flNextPrimaryAttack();

			return false;
		}
	}

	// yeez we have a normal gun.
	if (curtime >= m_weapon->m_flNextPrimaryAttack())
		return true;

	return false;
}

void Client::UpdateRevolverCock() {
	if (!m_weapon || !m_weapon_info)
		return;
	// default to false.
	m_revolver_fire = false;

	// reset properly.
	if (m_revolver_cock == -1)
		m_revolver_cock = 0;

	// we dont have a revolver.
	// we have no ammo.
	// player cant fire
	// we are waiting for we can shoot again.
	if (m_weapon_id != REVOLVER || m_weapon->m_iClip1() < 1 || !m_player_fire || game::TICKS_TO_TIME(m_local->m_nTickBase()) < m_weapon->m_flNextPrimaryAttack()) {
		// reset.
		m_revolver_cock = 0;
		m_revolver_query = 0;
		return;
	}

	// calculate max number of cocked ticks.
	// round to 6th decimal place for custom tickrates..
	int shoot = (int)(0.25f / (std::round(g_csgo.m_globals->m_interval * 1000000.f) / 1000000.f));

	// amount of ticks that we have to query.
	m_revolver_query = shoot - 1;

	// we held all the ticks we needed to hold.
	if (m_revolver_query == m_revolver_cock) {
		// reset cocked ticks.
		m_revolver_cock = -1;

		// we are allowed to fire, yay.
		m_revolver_fire = true;
	}

	else {
		// we still have ticks to query.
		// apply inattack.
		if (g_menu.main.config.mode.get() == 0 && m_revolver_query > m_revolver_cock)
			m_cmd->m_buttons |= IN_ATTACK;

		// count cock ticks.
		// do this so we can also count 'legit' ticks
		// that didnt originate from the hack.
		if (m_cmd->m_buttons & IN_ATTACK)
			m_revolver_cock++;

		// inattack was not held, reset.
		else m_revolver_cock = 0;
	}

	// remove inattack2 if cocking.
	if (m_revolver_cock > 0)
		m_cmd->m_buttons &= ~IN_ATTACK2;
}

void Client::UpdateIncomingSequences() {
	if (!g_csgo.m_net)
		return;

	if (m_sequences.empty() || g_csgo.m_net->m_in_seq > m_sequences.front().m_seq) {
		// store new stuff.
		m_sequences.emplace_front(g_csgo.m_globals->m_realtime, g_csgo.m_net->m_in_rel_state, g_csgo.m_net->m_in_seq);
	}

	// do not save too many of these.
	while (m_sequences.size() > 2048)
		m_sequences.pop_back();
}
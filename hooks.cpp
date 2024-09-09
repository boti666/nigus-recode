#include "includes.h"
#include "minhook/minhook.h"
#include "voice.h"

Hooks                g_hooks{ };;
CustomEntityListener g_custom_entity_listener{ };;

void Hooks::hkVoiceData(void* msg) {
	static auto m_oVoiceData = g_hooks.m_client_state.GetOldMethod< VoiceData_t >(CClientState::VOICEDATA);

	if (!msg)
		return m_oVoiceData(this, msg);

	auto local = g_cl.m_local;


	CSVCMsg_VoiceData_Legacy* msg_ = (CSVCMsg_VoiceData_Legacy*)msg;
	int idx = msg_->client + 1;
	AimPlayer* data = &g_aimbot.m_players[idx - 1];
	VoiceDataCustom msg_data = msg_->get_data();

	if (!local || !local->alive() || !g_csgo.m_engine->IsInGame() || !g_cl.m_processing || !data)
		return m_oVoiceData(this, msg);

	if (local->index() == idx)
		return m_oVoiceData(this, msg);

	if (msg_->format != 0)
		return m_oVoiceData(this, msg);

	// check if its empty
	if (msg_data.section_number == 0 && msg_data.sequence_bytes == 0 && msg_data.uncompressed_sample_offset == 0)
		return m_oVoiceData(this, msg);

	Voice_Vader* packet = (Voice_Vader*)msg_data.get_raw_data();

	if (!strcmp(packet->cheat_name, XOR("rax1337")))
		data->m_cheat_id = rax;

	if (!strcmp(packet->cheat_name, XOR("MintyHack")))
		data->m_cheat_id = minty;

	if (!strcmp(packet->cheat_name, XOR("familykkk24")))
		data->m_cheat_id = family;

	//// cheese crack
	//else if (msg_->sequence_bytes == 321420420) {
	//	data->m_cheat_id = cheese;
	//	//data->m_networked_angle = (*(float*)&msg_->section_number) / 69.f;
	//}
	//// kaaba crack
	//else if (msg_->sequence_bytes == -858993664) {
	//	data->m_cheat_id = kaaba;
	//	//data->m_networked_angle = (*(float*)&msg_->section_number) * 0.0007479516421f;
	//}
	//// dopium
	//else if (msg_->xuid_low == 1338)
	//	data->m_cheat_id = dopium;
	//// robertpaste
	//else if (msg_->xuid_low == 4539731)
	//	data->m_cheat_id = robert;
	//// pandora
	//else if (msg_->xuid_low == 6969)
	//	data->m_cheat_id = pandora;
	//// godhook
	//else if (!strcmp(packet->cheat_name, XOR("g0db00k.indeed")) || !strcmp(packet->cheat_name, "h0dg00k.ind33d") || msg_->xuid_low == 666613333)
	//	data->m_cheat_id = godhook;
	//// fade
	//else if (msg_->xuid_low == 1701077350 || msg_->sequence_bytes == 24930)
	//	data->m_cheat_id = fade;

	return m_oVoiceData(this, msg);
}

int lastsent = 0;

bool __fastcall SendNetMessage::Hook(INetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice) {
	if (pNetChan != g_csgo.m_engine->GetNetChannelInfo())
		return original(pNetChan, msg, bForceReliable, bVoice);

	if (msg.GetType() == 14) // Return and don't send messsage if its FileCRCCheck
		return false;
	else if (msg.GetGroup() == 9) { // group 9 is VoiceData
		// Fixing fakelag with voice
		bVoice = true;
	}

	return original(pNetChan, msg, bForceReliable, bVoice);
}

void Force_proxy(CRecvProxyData* data, Address ptr, Address out) {
	// convert to ragdoll.
	Ragdoll* ragdoll = ptr.as< Ragdoll* >();

	// get ragdoll owner.
	Player* player = ragdoll->GetPlayer();

	// we only want this happening to noobs we kill.
	if (g_menu.main.misc.ragdoll_force.get() && g_cl.m_local && player && player->enemy(g_cl.m_local)) {
		// get m_vecForce.
		vec3_t vel = { data->m_Value.m_Vector[0], data->m_Value.m_Vector[1], data->m_Value.m_Vector[2] };

		// give some speed to all directions.
		vel *= 1000.f;

		// boost z up a bit.
		if (vel.z <= 1.f)
			vel.z = 2.f;

		vel.z *= 2.f;

		// don't want crazy values for this... probably unlikely though?
		math::clamp(vel.x, std::numeric_limits< float >::lowest(), std::numeric_limits< float >::max());
		math::clamp(vel.y, std::numeric_limits< float >::lowest(), std::numeric_limits< float >::max());
		math::clamp(vel.z, std::numeric_limits< float >::lowest(), std::numeric_limits< float >::max());

		// set new velocity.
		data->m_Value.m_Vector[0] = vel.x;
		data->m_Value.m_Vector[1] = vel.y;
		data->m_Value.m_Vector[2] = vel.z;
	}

	if (g_hooks.m_Force_original)
		g_hooks.m_Force_original(data, ptr, out);
}

bool __fastcall SkipAnimationFrame::Hook(void* ecx, void* edx) {
	return false;
}

void __fastcall ModifyEyePosition::Hook(void* ecx, uintptr_t, vec3_t& position) {
	return;
}

void __fastcall DoProceduralFootPlant::Hook(void* _this, void* edx, void* boneToWorld, void* pLeftFootChain, void* pRightFootChain, void* pos) {
	return;
}


bool __fastcall SetupBones::Hook(void* ecx, void* edx, BoneArray* bone_to_world_out, int max_bones, int bone_mask, float curtime) {
	Player* pl = reinterpret_cast<Player*>(reinterpret_cast<uintptr_t>(ecx) - 0x4);
	if (!pl)
		return false;

	if (!pl->IsPlayer() || !pl->alive() || pl->dormant())
		return original(ecx, edx, bone_to_world_out, max_bones, bone_mask, curtime);

	if (bone_to_world_out && max_bones > 0 && max_bones <= 128)
		pl->GetBones(bone_to_world_out);

	return true;
}

void __fastcall UpdateClientAnimations::Hook(void* ecx)
{
	Player* pl = (Player*)ecx;
	if (!pl || !pl->IsPlayer() || !pl->alive() || pl->dormant())
		return original(ecx);
}

void __fastcall ExtraBonesProcessing::Hook(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7) {
	return;
}

void __fastcall InterpolateServerEntities::Hook() {
	return original();
}

ang_t* __fastcall GetEyeAngles::Hook(void* ecx, void* edx) {
	Player* pl = (Player*)ecx;

	static auto returnaddr1 = pattern::find(g_csgo.m_client_dll, XOR("8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55 ?"));
	static auto returnaddr2 = pattern::find(g_csgo.m_client_dll, XOR("F3 0F 10 55 ? 51 8B 8E ? ? ? ?"));
	static auto returnaddr3 = pattern::find(g_csgo.m_client_dll, XOR("8B 55 0C 8B C8 E8 ? ? ? ? 83 C4 08 5E 8B E5"));

	if ((_ReturnAddress() != returnaddr1 && _ReturnAddress() != returnaddr2 && _ReturnAddress() != returnaddr3))
		return original(ecx, edx);

	else if (!pl || pl != g_cl.m_local)
		return original(ecx, edx);

	return &g_cl.m_angle;
}

int __fastcall InterpolatePart::Hook(Entity* ent, void* edx, float& curtime, vec3_t& old_origin, vec3_t& old_angles, int& no_more_changes) {
	return original(ent, edx, curtime, old_origin, old_angles, no_more_changes);
}

void UpdateClientAnimations::DoUpdate(Player* pl) {
	if (!pl || !pl->IsPlayer() || !pl->alive() || pl->dormant())
		return;

	CCSGOPlayerAnimState* state = pl->m_PlayerAnimState();
	if (!state)
		return;

	state->m_last_update_frame = g_csgo.m_globals->m_frame - 1;

	C_AnimationLayer layers[13];
	pl->GetAnimLayers(layers);
	original(pl);
	pl->SetAnimLayers(layers);
}

void WriteUserCmd(void* buf, CUserCmd* incmd, CUserCmd* outcmd)
{
	using write_user_cmd_t = void(__fastcall*)(void*, CUserCmd*, CUserCmd*);
	static auto Fn = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D")).as<write_user_cmd_t>();

	__asm
	{
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    Fn
		add     esp, 4
	}
}

bool __fastcall WriteUserCmdDeltaToBuffer::Hook(void* ecx, void* edx, int slot, void* buf, int from, int to, bool is_new_command)
{
	if (!Shift)
		return original(ecx, slot, buf, from, to, is_new_command);

	if (from != -1)
		return true;

	auto final_from = -1;

	uintptr_t frame_ptr;
	__asm mov frame_ptr, ebp;

	auto backup_commands = reinterpret_cast <int*> (frame_ptr + 0xFD8);
	auto new_commands = reinterpret_cast <int*> (frame_ptr + 0xFDC);

	int newcmds = *new_commands;
	int shift = Shift;

	Shift = 0;
	*backup_commands = 0;

	auto choked_modifier = newcmds + shift;

	*new_commands = choked_modifier;

	auto next_cmdnr = g_csgo.m_cl->m_choked_commands + g_csgo.m_cl->m_last_outgoing_command + 1;
	auto final_to = next_cmdnr - newcmds + 1;

	if (final_to <= next_cmdnr)
	{
		while (original(ecx, slot, buf, final_from, final_to, true))
		{
			final_from = final_to++;

			if (final_to > next_cmdnr)
				goto next_cmd;
		}

		return false;
	}
next_cmd:

	auto user_cmd = g_csgo.m_input->GetUserCmd(final_from);

	if (!user_cmd)
		return true;

	CUserCmd to_cmd;
	CUserCmd from_cmd;

	from_cmd = *user_cmd;
	to_cmd = from_cmd;

	to_cmd.m_command_number++;
	to_cmd.m_tick += 200;
	to_cmd.m_view_angles.y += 180;

	if (newcmds > choked_modifier)
		return true;

	for (auto i = choked_modifier - newcmds + 1; i > 0; --i)
	{
		WriteUserCmd(buf, &to_cmd, &from_cmd);

		from_cmd = to_cmd;
		to_cmd.m_command_number++;
		to_cmd.m_tick++;
	}

	return true;
}

void __cdecl CLMove::Hook(float fSamples, bool bFinalTick) {
	return original(fSamples, bFinalTick);
}

//void __fastcall SetText::Hook(void* ecx, void* edx, const char* tokenName) {
//	if (!g_cl.m_local || !g_cl.m_local->alive() || !g::pCmd)
//		return original(ecx, edx, tokenName);
//
//	Weapon* pWeapon = g_cl.m_local->GetActiveWeapon();
//
//	if (!pWeapon)
//		return original(ecx, edx, tokenName);
//
//	std::string text_buffer = "FAMILYHOOK";
//
//	static float timerLmao = 0.f;
//	if (g::pLocal->GetWeapon()->GetItemDefinitionIndex() == C4 && g::pCmd->iButtons & IN_ATTACK) {
//
//		if (timerLmao == 0.f)
//			timerLmao = g_csgo.m_globals->m_curtime + 0.5f;
//
//		float flTransition = g_csgo.m_globals->m_curtime - timerLmao;
//
//		// transition time can be made to scale with text length but idc
//		std::string thisText;
//		for (size_t i = 0; i < sizeof(text_buffer); i++)
//			if (flTransition >= (i * 0.18f))
//				thisText += text_buffer[i];
//			else
//				break;
//
//		return original(ecx, edx, thisText.c_str());
//	}
//	else
//		timerLmao = 0.f;
//
//	return original(ecx, edx, tokenName);
//}

void Hooks::init() {
	// hook wndproc.
	m_old_wndproc = (WNDPROC)g_winapi.SetWindowLongA(g_csgo.m_game->m_hWindow, GWL_WNDPROC, util::force_cast<LONG>(Hooks::WndProc));

	// setup normal VMT hooks.
	m_panel.init(g_csgo.m_panel);
	m_panel.add(IPanel::PAINTTRAVERSE, util::force_cast(&Hooks::PaintTraverse));

	m_client.init(g_csgo.m_client);
	m_client.add(CHLClient::LEVELINITPREENTITY, util::force_cast(&Hooks::LevelInitPreEntity));
	m_client.add(CHLClient::LEVELINITPOSTENTITY, util::force_cast(&Hooks::LevelInitPostEntity));
	m_client.add(CHLClient::LEVELSHUTDOWN, util::force_cast(&Hooks::LevelShutdown));
	//m_client.add( CHLClient::INKEYEVENT, util::force_cast( &Hooks::IN_KeyEvent ) );
	m_client.add(CHLClient::FRAMESTAGENOTIFY, util::force_cast(&Hooks::FrameStageNotify));

	m_engine.init(g_csgo.m_engine);
	m_engine.add(IVEngineClient::ISCONNECTED, util::force_cast(&Hooks::IsConnected));

	m_engine_sound.init(g_csgo.m_sound);
	m_engine_sound.add(IEngineSound::EMITSOUND, util::force_cast(&Hooks::EmitSound));

	m_prediction.init(g_csgo.m_prediction);
	m_prediction.add(CPrediction::INPREDICTION, util::force_cast(&Hooks::InPrediction));

	m_client_mode.init(g_csgo.m_client_mode);
	m_client_mode.add(IClientMode::SHOULDDRAWPARTICLES, util::force_cast(&Hooks::ShouldDrawParticles));
	m_client_mode.add(IClientMode::SHOULDDRAWFOG, util::force_cast(&Hooks::ShouldDrawFog));
	m_client_mode.add(IClientMode::OVERRIDEVIEW, util::force_cast(&Hooks::OverrideView));
	m_client_mode.add(IClientMode::DOPOSTSPACESCREENEFFECTS, util::force_cast(&Hooks::DoPostScreenSpaceEffects));

	m_surface.init(g_csgo.m_surface);
	//m_surface.add( ISurface::GETSCREENSIZE, util::force_cast( &Hooks::GetScreenSize ) );
	m_surface.add(ISurface::LOCKCURSOR, util::force_cast(&Hooks::LockCursor));
	m_surface.add(ISurface::PLAYSOUND, util::force_cast(&Hooks::PlaySound));
	m_surface.add(ISurface::ONSCREENSIZECHANGED, util::force_cast(&Hooks::OnScreenSizeChanged));

	m_model_render.init(g_csgo.m_model_render);
	m_model_render.add(IVModelRender::DRAWMODELEXECUTE, util::force_cast(&Hooks::DrawModelExecute));

	m_render_view.init(g_csgo.m_render_view);
	m_render_view.add(IVRenderView::SCENEEND, util::force_cast(&Hooks::SceneEnd));

	m_shadow_mgr.init(g_csgo.m_shadow_mgr);
	m_shadow_mgr.add(IClientShadowMgr::COMPUTESHADOWDEPTHTEXTURES, util::force_cast(&Hooks::ComputeShadowDepthTextures));

	m_view_render.init(g_csgo.m_view_render);
	m_view_render.add(CViewRender::ONRENDERSTART, util::force_cast(&Hooks::OnRenderStart));
	m_view_render.add(CViewRender::RENDERVIEW, util::force_cast(&Hooks::RenderView));
	m_view_render.add(CViewRender::RENDER2DEFFECTSPOSTHUD, util::force_cast(&Hooks::Render2DEffectsPostHUD));
	m_view_render.add(CViewRender::RENDERSMOKEOVERLAY, util::force_cast(&Hooks::RenderSmokeOverlay));

	m_match_framework.init(g_csgo.m_match_framework);
	m_match_framework.add(CMatchFramework::GETMATCHSESSION, util::force_cast(&Hooks::GetMatchSession));

	m_material_system.init(g_csgo.m_material_system);
	m_material_system.add(IMaterialSystem::OVERRIDECONFIG, util::force_cast(&Hooks::OverrideConfig));

	m_fire_bullets.init(g_csgo.TEFireBullets);
	m_fire_bullets.add(7, util::force_cast(&Hooks::PostDataUpdate));

	static auto clientstate_instance = reinterpret_cast<void*>((CClientState*)(uint32_t(g_csgo.m_cl) + 0x8));

	m_client_state.init(clientstate_instance);
	m_client_state.add(CClientState::PACKETSTART, util::force_cast(&Hooks::PacketStart));
	//m_client_state.add(CClientState::VOICEDATA, util::force_cast(&Hooks::hkVoiceData));

	// register our custom entity listener.
	// todo - dex; should we push our listeners first? should be fine like this.
	g_custom_entity_listener.init();

	// cvar hooks.
	m_debug_spread.init(g_csgo.net_showfragments);
	m_debug_spread.add(ConVar::GETINT, util::force_cast(&Hooks::DebugSpreadGetInt));

	m_net_show_fragments.init(g_csgo.net_showfragments);
	m_net_show_fragments.add(ConVar::GETBOOL, util::force_cast(&Hooks::NetShowFragmentsGetBool));

	m_jiggle_bones.init(g_csgo.r_jiggle_bones);
	m_jiggle_bones.add(ConVar::GETINT, util::force_cast(&Hooks::JiggleBonesGetInt));

	m_extrapolate.init(g_csgo.cl_extrapolate);
	m_extrapolate.add(ConVar::GETINT, util::force_cast(&Hooks::ExtrapolateGetInt));

	static auto runcommand_target = util::get_method(g_csgo.m_prediction, CPrediction::RUNCOMMAND);
	static auto createmove_target = util::get_method(g_csgo.m_client, CHLClient::CREATEMOVE);
	static auto wdtb_target = util::get_method(g_csgo.m_client, CHLClient::WDTB);
	static auto hltv_target = util::get_method(g_csgo.m_engine, IVEngineClient::ISHLTV);
	static auto skipanimationframe_traget = pattern::find(g_csgo.m_client_dll, XOR("57 8B F9 8B 07 8B 80 60")).as<void*>();
	static auto modifyeyeposition_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F8 83 EC 58 56 57 8B F9 83 7F 60")).as<void*>();
	static auto setup_bones_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9")).as<void*>();
	static auto do_extra_bones_processing_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F8 81 EC FC 00 00 00 53 56 8B F1 57")).as<void*>();
	static auto update_clientside_animation_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36")).as<void*>();
	static auto calculate_view_bob_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9")).as<void*>();
	static auto get_eye_ang_target = pattern::find(g_csgo.m_client_dll, XOR("56 8B F1 85 F6 74 32")).as<void*>();
	static auto interpolate_part_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 51 8B 45 14 56")).as<void*>();
	static auto foot_plant_target = pattern::find(g_csgo.m_client_dll, XOR("55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B")).as<void*>();
	static auto clmove_target = pattern::find(g_csgo.m_engine_dll, XOR("55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A")).as<void*>();
	static auto send_datagram_target = pattern::find(g_csgo.m_engine_dll, XOR("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 89 7C 24 18")).as<void*>();
	static auto process_packet_target = pattern::find(g_csgo.m_engine_dll, XOR("55 8B EC 83 E4 C0 81 EC B4 00 00 00 53 56 57 8B 7D 08 8B D9")).as<void*>();
	static auto sendnetmsg_target = pattern::find(g_csgo.m_engine_dll, XOR("55 8B EC 56 8B F1 8B 86 ? ? ? ? 85 C0 74 24 48 83 F8 02 77 2C 83 BE ? ? ? ? ? 8D 8E ? ? ? ? 74 06 32 C0 84 C0 EB 10 E8 ? ? ? ? 84 C0 EB 07 83 BE ? ? ? ? ? 0F 94 C0 84 C0 74 07 B0 01 5E 5D C2 0C 00")).as<void*>();

	MH_Initialize();

	//MH_CreateHook(sendnetmsg_target, SendNetMessage::Hook, (void**)&SendNetMessage::original);
	MH_CreateHook(hltv_target, HLTV::Hook, (void**)&HLTV::original);
	MH_CreateHook(send_datagram_target, SendDataGram::Hook, (void**)&SendDataGram::original);
	MH_CreateHook(process_packet_target, ProcessPacket::Hook, (void**)&ProcessPacket::original);
	MH_CreateHook(clmove_target, CLMove::Hook, (void**)&CLMove::original);
	MH_CreateHook(wdtb_target, WriteUserCmdDeltaToBuffer::Hook, (void**)&WriteUserCmdDeltaToBuffer::original);
	MH_CreateHook(runcommand_target, RunCommand::Hook, (void**)&RunCommand::original);
	MH_CreateHook(foot_plant_target, DoProceduralFootPlant::Hook, (void**)&DoProceduralFootPlant::original);
	MH_CreateHook(interpolate_part_target, InterpolatePart::Hook, (void**)&InterpolatePart::original);
	MH_CreateHook(get_eye_ang_target, GetEyeAngles::Hook, (void**)&GetEyeAngles::original);
	//MH_CreateHook(interpolateserverentitys_target, InterpolateServerEntities::Hook, (void**)&InterpolateServerEntities::original);
	MH_CreateHook(update_clientside_animation_target, UpdateClientAnimations::Hook, (void**)&UpdateClientAnimations::original);
	MH_CreateHook(do_extra_bones_processing_target, ExtraBonesProcessing::Hook, (void**)&ExtraBonesProcessing::original);
	MH_CreateHook(skipanimationframe_traget, SkipAnimationFrame::Hook, (void**)&SkipAnimationFrame::original);
	MH_CreateHook(modifyeyeposition_target, ModifyEyePosition::Hook, (void**)&ModifyEyePosition::original);
	MH_CreateHook(setup_bones_target, SetupBones::Hook, (void**)&SetupBones::original);
	MH_CreateHook(createmove_target, CreateMove::Hook, (void**)&CreateMove::original);

	MH_EnableHook(nullptr);

	// set netvar proxies.
	g_netvars.SetProxy(HASH("DT_CSRagdoll"), HASH("m_vecForce"), Force_proxy, m_Force_original);
}
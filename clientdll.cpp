#include "includes.h"

void Hooks::LevelInitPreEntity(const char* map) {
	float rate{ 1.f / g_csgo.m_globals->m_interval };

	// set rates when joining a server.
	g_csgo.cl_updaterate->SetValue(rate);
	g_csgo.cl_cmdrate->SetValue(rate);

	g_aimbot.reset();

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPreEntity_t >(CHLClient::LEVELINITPREENTITY)(this, map);
}

void Hooks::LevelInitPostEntity() {
	g_cl.OnMapload();

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPostEntity_t >(CHLClient::LEVELINITPOSTENTITY)(this);
}

void Hooks::LevelShutdown() {
	g_aimbot.reset();

	g_cl.m_local = nullptr;
	g_cl.m_weapon = nullptr;
	g_cl.m_processing = false;
	g_cl.m_weapon_info = nullptr;
	g_cl.m_round_end = false;

	g_cl.m_sequences.clear();

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelShutdown_t >(CHLClient::LEVELSHUTDOWN)(this);
}

/*int Hooks::IN_KeyEvent( int evt, int key, const char* bind ) {
	// see if this key event was fired for the drop bind.
	/*if( bind && FNV1a::get( bind ) == HASH( "drop" ) ) {
		// down.
		if( evt ) {
			g_cl.m_drop = true;
			g_cl.m_drop_query = 2;
			g_cl.print( "drop\n" );
		}

		// up.
		else
			g_cl.m_drop = false;

		// ignore the event.
		return 0;
	}

	return g_hooks.m_client.GetOldMethod< IN_KeyEvent_t >( CHLClient::INKEYEVENT )( this, evt, key, bind );
}*/

struct var_map_entry_t {
public:
	unsigned short type;
	unsigned short need_interp;
	void* data;
	void* watcher;
};

struct varmapping_t {
	varmapping_t() {
		interp_entries = 0;
	}

	var_map_entry_t* entries;
	int interp_entries;
	float last_interp_time;
};

varmapping_t* get_var_map(void* base_ent) {
	return reinterpret_cast<varmapping_t*>((DWORD)base_ent + 0x24); //0x4C );
}

void interpolation(Player* ent, bool enable) {

	varmapping_t* map = get_var_map(ent);

	if (!map)
		return;

	for (int i = 0; i < map->interp_entries; i++) {
		var_map_entry_t* e = &map->entries[i];
		if (!e)
			continue;

		e->need_interp = enable;
	}
}


void draw_server_hitboxes(int index)
{
	auto get_player_by_index = [](int index) -> Player*
		{ //i dont need this shit func for anything else so it can be lambda
			typedef Player* (__fastcall* player_by_index)(int);
			static auto player_index = pattern::find(g_csgo.m_server_dll, XOR("85 C9 7E 2A A1")).as<player_by_index>();

			if (!player_index)
				return false;

			return player_index(index);
		};

	static auto fn = pattern::find(g_csgo.m_server_dll, XOR("55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE"));

	auto duration = -1.f;
	PVOID entity = nullptr;

	entity = get_player_by_index(index);

	if (!entity)
		return;

	__asm {
		pushad
		movss xmm1, duration
		push 0 // 0 - colored, 1 - blue
		mov ecx, entity
		call fn
		popad
	}
}



void Hooks::FrameStageNotify(Stage_t stage) {
	// save stage.
	if (stage != FRAME_START)
		g_cl.m_stage = stage;

	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	if (stage == FRAME_START) {
		static ConVar* cl_extrapolate = g_csgo.m_cvar->FindVar(HASH("cl_extrapolate"));
		if (cl_extrapolate)
			cl_extrapolate->SetValue(0);

		static ConVar* r_aspectratio = g_csgo.m_cvar->FindVar(HASH("r_aspectratio"));
		if (r_aspectratio)
			r_aspectratio->SetValue(g_menu.main.visuals.aspect_ratio.get() / 50.f);

		static ConVar* r_jiggle_bones = g_csgo.m_cvar->FindVar(HASH("r_jiggle_bones"));
		if (r_jiggle_bones)
			r_jiggle_bones->SetValue(0);
	}

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);
			if (!player || player->m_bIsLocalPlayer())
				continue;

			interpolation(player, true); // force interp for absorigin and absvelocity.
		}

		g_inputpred.CorrectViewmodelData();
	}
	static ang_t aim_punch{};
	static ang_t view_punch{};

	if (stage == FRAME_RENDER_START) {
		// restore non-compressed netvars.
		g_netdata.apply();
		if (g_csgo.m_engine->IsInGame() && g_cl.m_local) {
			using ClanTagfn = int(__fastcall*)(const char*, const char*);
			static auto fnClantagChanged = g_csgo.SetClanTag.as<ClanTagfn>();

			if (g_menu.main.misc.clantag.get())
				fnClantagChanged(g_menu.main.misc.clantag_text.get().c_str(), g_menu.main.misc.clantag_text.get().c_str());
			else
				fnClantagChanged("", "");
		}

		if (g_cl.m_local && g_cl.m_processing) {
			aim_punch = g_cl.m_local->m_aimPunchAngle();
			view_punch = g_cl.m_local->m_viewPunchAngle();

			if (g_menu.main.visuals.novisrecoil.get()) {
				g_cl.m_local->m_viewPunchAngle() = g_cl.m_local->m_aimPunchAngle() = { 0,0,0 };
			}

			if (g_menu.main.visuals.remove_shadows.get()) {

				if (g_csgo.cl_csm_shadows->GetInt() != 0)
					g_csgo.cl_csm_shadows->SetValue(0);
			}
			else {

				if (g_csgo.cl_csm_shadows->GetInt() != 1)
					g_csgo.cl_csm_shadows->SetValue(1);
			}


		}


		// draw our custom beams.
		g_visuals.DrawBeams();

		*g_csgo.PostProcessing.as<bool*>() = g_menu.main.visuals.nopostprocessing.get();

		static auto cross = g_csgo.m_cvar->FindVar(HASH("weapon_debug_spread_show"));

		cross->SetValue(g_menu.main.visuals.force_xhair.get() && !g_cl.m_local->m_bIsScoped() ? 3 : 0); 
	}

	// call og.
	g_hooks.m_client.GetOldMethod< FrameStageNotify_t >(CHLClient::FRAMESTAGENOTIFY)(this, stage);

	if (stage == FRAME_NET_UPDATE_END) {
		g_cl.StoreLayers(g_cl.m_layers);

		//if (g_cl.m_local && g_cl.m_processing) {
		//	if (g_cl.m_local->m_AnimOverlay()[ANIMATION_LAYER_WHOLE_BODY].m_sequence)
		//		g_notify.add(std::to_string(g_cl.m_local->m_AnimOverlay()[ANIMATION_LAYER_WHOLE_BODY].m_sequence));
		//}

		// update all players.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);
			if (!player || player->m_bIsLocalPlayer()) {
				g_resolver.m_override_data[i - 1].m_fov = FLT_MAX;
				continue;
			}

			AimPlayer* data = &g_aimbot.m_players[i - 1];
			if (!data) {
				g_resolver.m_override_data[i - 1].m_fov = FLT_MAX;
				continue;
			}

			data->OnNetUpdate(player);
		}
	}

	if (stage == FRAME_RENDER_START) {
		if (g_cl.m_local && g_cl.m_processing) {
			g_cl.m_local->m_aimPunchAngle() = aim_punch;
			g_cl.m_local->m_viewPunchAngle() = view_punch;

		}
		// restore non-compressed netvars.
		g_netdata.apply();

		g_cl.UpdateAnimations();

		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);
			if (!player)
				continue;

			AimPlayer* data = &g_aimbot.m_players[i - 1];
			if (!data)
				continue;

			for (auto& impacts : g_shots.m_impacts) {
				if (impacts.m_hurt || impacts.m_scanned || impacts.m_shot->m_i != i)
					continue;

				impacts.m_scanned = true;

				if (!player->alive()) {
					g_notify.add(XOR("missed shot due to target death\n"));
					continue;
				}

				if (!impacts.m_shot->m_record)
					continue;

				if (impacts.m_spread) {
					g_notify.add(XOR("missed target point due to spread\n"));
					continue;
				}

				if (impacts.m_shot->m_record->m_predicted) {
					g_notify.add(XOR("missed target point due invalid record\n"));
				}
				else {
					int mode = impacts.m_shot->m_record->m_mode;
					switch (mode) {
					case Resolver::Modes::RESOLVE_DISABLED:
						break;
					case Resolver::Modes::RESOLVE_WALK:
						++data->m_ff_index;
						break;
					case Resolver::Modes::RESOLVE_PREFLICK:
						break;
					case Resolver::Modes::RESOLVE_STAND:
						++data->m_stand_index;
						break;
					case Resolver::Modes::RESOLVE_STAND_LOGIC:
						++data->m_stand2_index;
						break;
					case Resolver::Modes::RESOLVE_AIR:
						++data->m_air_index;
						break;
					case Resolver::Modes::RESOLVE_FLICK:
						++data->m_body_index;
						break;
					}
					++data->m_missed_shots;

					g_notify.add((mode != Resolver::Modes::RESOLVE_WALK && mode != Resolver::Modes::RESOLVE_DISABLED) ?
						XOR("missed target point due to fake angles\n") :
						XOR("missed target point due to animations\n"));
				}
			}

			if (!player->alive() || player->dormant())
				continue;

			auto cache = &player->m_BoneCache();
			if (!cache || !cache->m_pCachedBones)
				continue;

			g_bones.BuildBones(player, BONE_USED_BY_ANYTHING, cache->m_pCachedBones, player->m_bIsLocalPlayer());
		}
	}

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		g_skins.think();
	}

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
		g_visuals.NoSmoke();
	}
}
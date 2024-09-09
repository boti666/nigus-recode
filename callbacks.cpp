#include "includes.h"

// execution callbacks..

bool callbacks::HasShotMatrix() {
	return g_menu.main.aimbot.shot_matrix.get() > 0;
}

bool callbacks::IsGeneral() {
	return g_menu.main.aimbot.weapon.get() == 0 && IsTabWeaponAimbot();
}
bool callbacks::IsAuto() {
	return g_menu.main.aimbot.weapon.get() == 1 && IsTabWeaponAimbot();
}
bool callbacks::IsAwp() {
	return g_menu.main.aimbot.weapon.get() == 2 && IsTabWeaponAimbot();
}
bool callbacks::IsScout() {
	return g_menu.main.aimbot.weapon.get() == 3 && IsTabWeaponAimbot();
}
bool callbacks::IsHPistol() {
	return g_menu.main.aimbot.weapon.get() == 4 && IsTabWeaponAimbot();
}
bool callbacks::IsPistol() {
	return g_menu.main.aimbot.weapon.get() == 5 && IsTabWeaponAimbot();
}

void callbacks::UpdateAutoPeek() {
	g_movement.m_autopeek = !g_movement.m_autopeek;
}

bool callbacks::IsTabGeneralAimbot() {
	return g_menu.main.aimbot.tab.get() == 0;
}
bool callbacks::IsTabWeaponAimbot() {
	return g_menu.main.aimbot.tab.get() == 1;
}

void callbacks::SwitchManualMode() {
	++g_hvh.m_manual_mode;

	if (g_hvh.m_manual_mode > 3)
		g_hvh.m_manual_mode = 0;
}

void callbacks::UpdateOverrideResolver() {
	g_cl.m_override_resolver_toggle = !g_cl.m_override_resolver_toggle;
}

void callbacks::UpdateInverter() {
	g_hvh.m_inverter = !g_hvh.m_inverter;
}

bool callbacks::IsTabGeneralVisuals() {
	return g_menu.main.visuals.tab.get() == 0;
}
bool callbacks::IsTabRemovalsVisuals() {
	return g_menu.main.visuals.tab.get() == 1;
}
bool callbacks::IsTabViewVisuals() {
	return g_menu.main.visuals.tab.get() == 2;
}

bool callbacks::IsTabEspPlayers() {
	return g_menu.main.players.tab.get() == 0;
}

bool callbacks::IsTabModelPlayers() {
	return g_menu.main.players.tab.get() == 1;
}

bool callbacks::HealthBar() {
	return g_menu.main.players.health.get(0) || g_menu.main.players.health.get(1);
}
bool callbacks::CustomHealthEnemy() {
	return g_menu.main.players.custom_health.get(0);
}
bool callbacks::CustomHealthFriendly() {
	return g_menu.main.players.custom_health.get(1);
}

bool callbacks::IsTransparentProps() {
	return g_menu.main.visuals.transparent_props.get();
}


bool callbacks::IsSkyBoxChange() {
	return g_menu.main.misc.skyboxchange.get();
}


bool callbacks::IsNightMode() {
	return g_menu.main.visuals.world_modulation.get();
}

bool callbacks::ForceFakeLatency() {
	return g_menu.main.misc.force_fake_latency.get();
}


void callbacks::UpdateOverride() {
	g_aimbot.m_override = !g_aimbot.m_override;
}

void callbacks::UpdateFront() {
	g_hvh.m_front = !g_hvh.m_front;
	g_hvh.m_back = false;
	g_hvh.m_right = false;
	g_hvh.m_left = false;
}
void callbacks::UpdateBack() {
	g_hvh.m_front = false;
	g_hvh.m_back = !g_hvh.m_back;
	g_hvh.m_right = false;
	g_hvh.m_left = false;
}
void callbacks::UpdateRight() {
	g_hvh.m_front = false;
	g_hvh.m_back = false;
	g_hvh.m_right = !g_hvh.m_right;
	g_hvh.m_left = false;
}
void callbacks::UpdateLeft() {
	g_hvh.m_front = false;
	g_hvh.m_back = false;
	g_hvh.m_right = false;
	g_hvh.m_left = !g_hvh.m_left;
}

void callbacks::UpdateFakeFlick() {
	g_hvh.m_fake_flick = !g_hvh.m_fake_flick;
}

void callbacks::RefreshSeeds() {
	for (int i = 0; i <= 255;) {
		g_aimbot.m_spread_seeds[i].initialized = false;
		++i;
	}
}

void callbacks::SkinUpdate() {
	g_skins.m_update = true;
}

void callbacks::ForceFullUpdate() {
	//static DWORD tick{};
	//
	//if( tick != g_winapi.GetTickCount( ) ) {
	//	g_csgo.cl_fullupdate->m_callback( );
	//	tick = g_winapi.GetTickCount( );
	//

	g_csgo.m_cl->m_delta_tick = -1;
}

void callbacks::ToggleThirdPerson() {
	g_visuals.m_thirdperson = !g_visuals.m_thirdperson;
}

void callbacks::ToggleFakeLatency() {
	g_aimbot.m_fake_latency = !g_aimbot.m_fake_latency;
	g_aimbot.m_fake_latency2 = false;
}

void callbacks::ToggleKillfeed() {
	KillFeed_t* feed = (KillFeed_t*)g_csgo.m_hud->FindElement(HASH("SFHudDeathNoticeAndBotStatus"));
	if (feed)
		g_csgo.ClearNotices(feed);
}

void callbacks::SaveHotkeys() {
	g_config.SaveHotkeys();
}

void callbacks::ConfigLoad1() {
	g_config.load(&g_menu.main, XOR("1.cfg"));
	g_menu.main.config.config.select(1 - 1);

	g_notify.add(tfm::format(XOR("loaded config 1\n")));
}

void callbacks::ConfigLoad2() {
	g_config.load(&g_menu.main, XOR("2.cfg"));
	g_menu.main.config.config.select(2 - 1);
	g_notify.add(tfm::format(XOR("loaded config 2\n")));
}

void callbacks::ConfigLoad3() {
	g_config.load(&g_menu.main, XOR("3.cfg"));
	g_menu.main.config.config.select(3 - 1);
	g_notify.add(tfm::format(XOR("loaded config 3\n")));
}

void callbacks::ConfigLoad4() {
	g_config.load(&g_menu.main, XOR("4.cfg"));
	g_menu.main.config.config.select(4 - 1);
	g_notify.add(tfm::format(XOR("loaded config 4\n")));
}

void callbacks::ConfigLoad5() {
	g_config.load(&g_menu.main, XOR("5.cfg"));
	g_menu.main.config.config.select(5 - 1);
	g_notify.add(tfm::format(XOR("loaded config 5\n")));
}

void callbacks::ConfigLoad6() {
	g_config.load(&g_menu.main, XOR("6.cfg"));
	g_menu.main.config.config.select(6 - 1);
	g_notify.add(tfm::format(XOR("loaded config 6\n")));
}

void callbacks::ConfigLoad() {
	std::string config = g_menu.main.config.config.GetActiveItem();
	std::string file = tfm::format(XOR("%s.cfg"), config.data());

	g_config.load(&g_menu.main, file);
	g_notify.add(tfm::format(XOR("loaded config %s\n"), config.data()));
}

void callbacks::ConfigSave() {
	std::string config = g_menu.main.config.config.GetActiveItem();
	std::string file = tfm::format(XOR("%s.cfg"), config.data());

	g_config.save(&g_menu.main, file);
	g_notify.add(tfm::format(XOR("saved config %s\n"), config.data()));
}

void callbacks::WhitelistAdd() {
	int id = std::stoi(g_menu.main.whitelist.id.get());

	if (id < 1 || id > 64)
		return;

	for (auto& added : g_cl.m_whitelist_ids) {
		if (added == id)
			return;
	}

	g_cl.m_whitelist_ids.push_back(std::stoi(g_menu.main.whitelist.id.get()));
}
void callbacks::WhitelistRemove() {
	int id = std::stoi(g_menu.main.whitelist.id.get());

	if (id < 1 || id > 64)
		return;

	for (int i = 0; i < g_cl.m_whitelist_ids.size();) {
		if (g_cl.m_whitelist_ids.at(i) == id)
			g_cl.m_whitelist_ids.erase(g_cl.m_whitelist_ids.begin() + i);

		i++;
	}
}
void callbacks::WhitelistClear() {
	g_cl.m_whitelist_ids.clear();
}

bool callbacks::IsTabGeneralAntiAim() {
	return g_menu.main.antiaim.tab.get() == 0;
}
bool callbacks::IsTabFakeAntiAim() {
	return g_menu.main.antiaim.tab.get() == 1;
}

void callbacks::UpdateBaim() {
	g_aimbot.m_baim = !g_aimbot.m_baim;
}

void callbacks::HeadGlitchAimbot() {
	g_aimbot.m_headglitch = !g_aimbot.m_headglitch;
}

bool callbacks::HasLbyStand() {
	return g_menu.main.antiaim.body_fake_stand.get();
}

bool callbacks::IsBaimHealth() {
	return true;
}

bool callbacks::IsHitchanceOn() {
	return true;
}

bool callbacks::IsPenetrationOn() {
	return true;
}

bool callbacks::IsMultipointOn() {
	return true;
}

bool callbacks::IsMultipointBodyOn() {
	return true;
}

bool callbacks::HasLogicResolver() {
	return g_menu.main.aimbot.correct.get() == 2;
}


bool callbacks::ShotmatrixOn1() {
	return g_menu.main.aimbot.shot_matrix.get() == 1;
}

bool callbacks::IsAntiAimModeStand() {
	return g_menu.main.antiaim.mode.get() == 0;
}

bool callbacks::HasStandYaw() {
	return g_menu.main.antiaim.yaw_stand.get() > 0;
}

bool callbacks::IsStandYawJitter() {
	return g_menu.main.antiaim.yaw_stand.get() == 2 || g_menu.main.antiaim.yaw_stand.get() == 5;
}

bool callbacks::IsStandYawRotate() {
	return g_menu.main.antiaim.yaw_stand.get() == 3 || g_menu.main.antiaim.yaw_stand.get() == 4;
}

bool callbacks::IsStandYawRnadom() {
	return g_menu.main.antiaim.yaw_stand.get() == 6;
}

bool callbacks::IsStandDirAuto() {
	return g_menu.main.antiaim.dir_stand.get() == 0;
}

bool callbacks::IsStandDirCustom() {
	return g_menu.main.antiaim.dir_stand.get() == 4;
}

bool callbacks::IsAntiAimModeWalk() {
	return g_menu.main.antiaim.mode.get() == 1;
}

bool callbacks::WalkHasYaw() {
	return g_menu.main.antiaim.yaw_walk.get() > 0;
}

bool callbacks::IsWalkYawJitter() {
	return g_menu.main.antiaim.yaw_walk.get() == 2 || g_menu.main.antiaim.yaw_walk.get() == 5;
}

bool callbacks::IsWalkYawRotate() {
	return g_menu.main.antiaim.yaw_walk.get() == 3 || g_menu.main.antiaim.yaw_walk.get() == 4;
}

bool callbacks::IsWalkYawRandom() {
	return g_menu.main.antiaim.yaw_walk.get() == 6;
}

bool callbacks::IsWalkDirAuto() {
	return g_menu.main.antiaim.dir_walk.get() == 0;
}

bool callbacks::IsWalkDirCustom() {
	return g_menu.main.antiaim.dir_walk.get() == 4;
}

bool callbacks::IsAntiAimModeAir() {
	return g_menu.main.antiaim.mode.get() == 2;
}

bool callbacks::SingleFlick() {
	return g_menu.main.antiaim.fake_flick_mode.get() == 1;
}

bool callbacks::AirHasYaw() {
	return g_menu.main.antiaim.yaw_air.get() > 0;
}

bool callbacks::IsAirYawJitter() {
	return g_menu.main.antiaim.yaw_air.get() == 2 || g_menu.main.antiaim.yaw_air.get() == 5;
}

bool callbacks::IsAirYawRotate() {
	return g_menu.main.antiaim.yaw_air.get() == 3 || g_menu.main.antiaim.yaw_air.get() == 4;
}

bool callbacks::IsAirYawRandom() {
	return g_menu.main.antiaim.yaw_air.get() == 6;
}

bool callbacks::IsAirDirAuto() {
	return g_menu.main.antiaim.dir_air.get() == 0;
}

bool callbacks::IsAirDirCustom() {
	return g_menu.main.antiaim.dir_air.get() == 4;
}

bool callbacks::IsFakeAntiAimRelative() {
	return g_menu.main.antiaim.fake_yaw.get() == 2;
}

bool callbacks::IsFakeAntiAimJitter() {
	return g_menu.main.antiaim.fake_yaw.get() == 3;
}


bool callbacks::IsConfigMM() {
	return g_menu.main.config.mode.get() == 0;
}

bool callbacks::IsConfigNS() {
	return g_menu.main.config.mode.get() == 1;
}

bool callbacks::IsConfig1() {
	return g_menu.main.config.config.get() == 0;
}

bool callbacks::IsConfig2() {
	return g_menu.main.config.config.get() == 1;
}

bool callbacks::IsConfig3() {
	return g_menu.main.config.config.get() == 2;
}

bool callbacks::IsConfig4() {
	return g_menu.main.config.config.get() == 3;
}

bool callbacks::IsConfig5() {
	return g_menu.main.config.config.get() == 4;
}

bool callbacks::IsConfig6() {
	return g_menu.main.config.config.get() == 5;
}

// weaponcfgs callbacks.
bool callbacks::DEAGLE() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::DEAGLE;
}

bool callbacks::ELITE() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::ELITE;
}

bool callbacks::FIVESEVEN() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::FIVESEVEN;
}

bool callbacks::GLOCK() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::GLOCK;
}

bool callbacks::AK47() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::AK47;
}

bool callbacks::AUG() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::AUG;
}

bool callbacks::AWP() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::AWP;
}

bool callbacks::FAMAS() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::FAMAS;
}

bool callbacks::G3SG1() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::G3SG1;
}

bool callbacks::GALIL() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::GALIL;
}

bool callbacks::M249() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::M249;
}

bool callbacks::M4A4() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::M4A4;
}

bool callbacks::MAC10() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::MAC10;
}

bool callbacks::P90() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::P90;
}

bool callbacks::UMP45() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::UMP45;
}

bool callbacks::XM1014() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::XM1014;
}

bool callbacks::BIZON() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::BIZON;
}

bool callbacks::MAG7() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::MAG7;
}

bool callbacks::NEGEV() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::NEGEV;
}

bool callbacks::SAWEDOFF() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::SAWEDOFF;
}

bool callbacks::TEC9() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::TEC9;
}

bool callbacks::P2000() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::P2000;
}

bool callbacks::MP7() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::MP7;
}

bool callbacks::MP9() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::MP9;
}

bool callbacks::NOVA() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::NOVA;
}

bool callbacks::P250() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::P250;
}

bool callbacks::SCAR20() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::SCAR20;
}

bool callbacks::SG553() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::SG553;
}

bool callbacks::SSG08() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::SSG08;
}

bool callbacks::M4A1S() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::M4A1S;
}

bool callbacks::USPS() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::USPS;
}

bool callbacks::CZ75A() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::CZ75A;
}

bool callbacks::REVOLVER() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::REVOLVER;
}

bool callbacks::KNIFE_BAYONET() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BAYONET;
}

bool callbacks::KNIFE_FLIP() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_FLIP;
}

bool callbacks::KNIFE_GUT() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_GUT;
}

bool callbacks::KNIFE_KARAMBIT() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_KARAMBIT;
}

bool callbacks::KNIFE_M9_BAYONET() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_M9_BAYONET;
}

bool callbacks::KNIFE_HUNTSMAN() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_HUNTSMAN;
}

bool callbacks::KNIFE_FALCHION() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_FALCHION;
}

bool callbacks::KNIFE_BOWIE() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BOWIE;
}

bool callbacks::KNIFE_BUTTERFLY() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BUTTERFLY;
}

bool callbacks::KNIFE_SHADOW_DAGGERS() {
	if (!g_csgo.m_engine->IsInGame() || !g_cl.m_processing)
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_SHADOW_DAGGERS;
}

bool callbacks::AUTO_STOP() {
	return false;
}
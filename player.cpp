#include "includes.h"

Weapon* Hooks::GetActiveWeapon() {
	Stack stack;

	static Address ret_1 = pattern::find(g_csgo.m_client_dll, XOR("85 C0 74 1D 8B 88 ? ? ? ? 85 C9"));

	// note - dex; stop call to CIronSightController::RenderScopeEffect inside CViewRender::RenderView.
	if (g_menu.main.visuals.noscope.get()) {
		if (stack.ReturnAddress() == ret_1)
			return nullptr;
	}

	return g_hooks.m_GetActiveWeapon(this);
}

void CustomEntityListener::OnEntityCreated(Entity* ent) {
	if (ent) {
		// player created.
		if (ent->IsPlayer()) {
			Player* player = ent->as< Player* >();

			// access out player data stucture and reset player data.
			AimPlayer* data = &g_aimbot.m_players[player->index() - 1];
			if (data)
				data->reset();

			// get ptr to vmt instance and reset tables.
			VMT* vmt = &g_hooks.m_player[player->index() - 1];
			if (vmt) {
				// init vtable with new ptr.
				vmt->reset();
				vmt->init(player);

				// local gets special treatment.
				if (player->index() == g_csgo.m_engine->GetLocalPlayer()) {
					g_hooks.m_GetActiveWeapon = vmt->add< Hooks::GetActiveWeapon_t >(Player::GETACTIVEWEAPON, util::force_cast(&Hooks::GetActiveWeapon));
				}
			}
		}

		// ragdoll created.
		// note - dex; sadly, it seems like m_hPlayer (GetRagdollPlayer) is null here... probably has to be done somewhere else.
		// if( ent->is( HASH( "CCSRagdoll" ) ) ) {
		//     Player *ragdoll_player{ ent->GetRagdollPlayer( ) };
		// 
		//     // note - dex;  ragdoll ents (DT_CSRagdoll) seem to contain some new netvars now, m_flDeathYaw and m_flAbsYaw.
		//     //              didnt test much but making a bot with bot_mimic look at yaws:
		//     //
		//     //              -107.98 gave me m_flDeathYaw=-16.919 and m_flAbsYaw=268.962
		//     //              46.05 gave me m_flDeathYaw=-21.685 and m_flAbsYaw=67.742
		//     //             
		//     //              these angles don't seem consistent... but i didn't test much.
		// 
		//     g_cl.print( "ragdoll 0x%p created on client at time %f, from player 0x%p\n", ent, g_csgo.m_globals->m_curtime, ragdoll_player );
		// }
	}
}

void CustomEntityListener::OnEntityDeleted(Entity* ent) {
	// note; IsPlayer doesn't work here because the ent class is CBaseEntity.
	if (ent && ent->index() >= 1 && ent->index() <= 64) {
		Player* player = ent->as< Player* >();

		// access out player data stucture and reset player data.
		AimPlayer* data = &g_aimbot.m_players[player->index() - 1];
		if (data)
			data->reset();

		// get ptr to vmt instance and reset tables.
		VMT* vmt = &g_hooks.m_player[player->index() - 1];
		if (vmt)
			vmt->reset();
	}
}
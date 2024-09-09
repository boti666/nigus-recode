#include "includes.h"

Chams g_chams{ };;



Chams::model_type_t Chams::GetModelType(const ModelRenderInfo_t& info) {
	// model name.
	//const char* mdl = info.m_model->m_name;

	std::string mdl{ info.m_model->m_name };

	//static auto int_from_chars = [ mdl ]( size_t index ) {
	//	return *( int* )( mdl + index );
	//};

	// little endian.
	//if( int_from_chars( 7 ) == 'paew' ) { // weap
	//	if( int_from_chars( 15 ) == 'om_v' && int_from_chars( 19 ) == 'sled' )
	//		return model_type_t::arms;
	//
	//	if( mdl[ 15 ] == 'v' )
	//		return model_type_t::view_weapon;
	//}

	//else if( int_from_chars( 7 ) == 'yalp' ) // play
	//	return model_type_t::player;

	if (mdl.find(XOR("player")) != std::string::npos)
		return model_type_t::player;

	return model_type_t::invalid;
}

bool Chams::IsInViewPlane(const vec3_t& world) {
	float w;

	const VMatrix& matrix = g_csgo.m_engine->WorldToScreenMatrix();

	w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

	return w > 0.001f;
}

void Chams::SetColor(Color col, IMaterial* mat) {
	if (mat)
		mat->ColorModulate(col);

	else
		g_csgo.m_render_view->SetColorModulation(col);
}

void Chams::SetAlpha(float alpha, IMaterial* mat) {
	if (mat)
		mat->AlphaModulate(alpha);

	else
		g_csgo.m_render_view->SetBlend(alpha);
}

void Chams::SetupMaterial(IMaterial* mat, Color col, bool z_flag) {
	SetColor(col);

	// mat->SetFlag( MATERIAL_VAR_HALFLAMBERT, flags );
	mat->SetFlag(MATERIAL_VAR_ZNEARER, z_flag);
	mat->SetFlag(MATERIAL_VAR_NOFOG, z_flag);
	mat->SetFlag(MATERIAL_VAR_IGNOREZ, z_flag);

	g_csgo.m_studio_render->ForcedMaterialOverride(mat);
}

void Chams::init() {
	std::ofstream("csgo\\materials\\pdr_shine.vmt") << R"#("VertexLitGeneric" 
	{
					"$basetexture"				"vgui/white_additive"
					"$ignorez"					"0"
					"$phong"					"1"
					"$BasemapAlphaPhongMask"    "1"
					"$phongexponent"			"15"
					"$normalmapalphaenvmask"	"1"
					"$envmap"					"env_cubemap"
					"$envmaptint"				"[0.0 0.0 0.0]"
					"$phongboost"				"[0.6 0.6 0.6]"
					"phongfresnelranges"		"[0.5 0.5 1.0]"
					"$nofog"					"1"
					"$model"					"1"
					"$nocull"					"0"
					"$selfillum"				"1"
					"$halflambert"				"1"
					"$znearer"					"0"
					"$flat"						"0"	
					"$rimlight"					"1"
					"$rimlightexponent"			"2"
					"$rimlightboost"			"0"
		})#";

	metallic = g_csgo.m_material_system->FindMaterial(XOR("pdr_shine"), XOR("Model textures"));
	metallic->IncrementReferenceCount();

	debugambientcube = g_csgo.m_material_system->FindMaterial(XOR("debug/debugambientcube"), XOR("Model textures"));
	debugambientcube->IncrementReferenceCount();

	debugdrawflat = g_csgo.m_material_system->FindMaterial(XOR("debug/debugdrawflat"), XOR("Model textures"));
	debugdrawflat->IncrementReferenceCount();
}

bool Chams::OverridePlayer(int index) {
	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(index);
	if (!player)
		return false;

	// always skip the local player in DrawModelExecute.
	// this is because if we want to make the local player have less alpha
	// the static props are drawn after the players and it looks like aids.
	// therefore always process the local player in scene end.
	if (player == g_cl.m_local)
		return true;

	if (!player->alive() && g_menu.main.aimbot.shot_matrix.get() == 3)
		return true;

	// see if this player is an enemy to us.
	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);

	// we have chams on enemies.
	if (enemy && g_menu.main.players.chams_enemy.get(0))
		return true;

	// we have chams on friendly.
	else if (!enemy && g_menu.main.players.chams_friendly.get(0))
		return true;

	return false;
}

void Chams::RenderHistoryChams(int index) {
	AimPlayer* data;
	LagRecord* record = nullptr;

	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(index);
	if (!player)
		return;

	if (!g_aimbot.IsValidTarget(player))
		return;

	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);
	if (enemy) {
		data = &g_aimbot.m_players[index - 1];
		if (!data || data->m_records.empty())
			return;

		for (auto& it : data->m_records) {
			if (!it.valid())
				continue;

			// get current record.
			record = &it;
		}

		if (!record || !record->m_setup)
			return;

		// override blend.
		SetAlpha(g_menu.main.players.chams_enemy_history_col.get().a() / 255.f);

		// set material and color.
		SetupMaterial(debugdrawflat, g_menu.main.players.chams_enemy_history_col.get(), true);

		// backup the bone cache before we fuck with it.
		auto backup_bones = player->m_BoneCache().m_pCachedBones;

		// replace their bone cache with our custom one.
		player->m_BoneCache().m_pCachedBones = record->m_bones;

		// manually draw the model.
		player->DrawModel();

		// reset their bone cache to the previous one.
		player->m_BoneCache().m_pCachedBones = backup_bones;
	}
}

bool Chams::DrawModel(uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone) {
	// store and validate model type.
	model_type_t type = GetModelType(info);
	if (type == model_type_t::invalid)
		return true;

	// is a valid player.
	if (type == model_type_t::player) {
		// do not cancel out our own calls from SceneEnd
		// also do not cancel out calls from the glow.
		if (!m_running && !g_csgo.m_studio_render->m_pForcedMaterial && OverridePlayer(info.m_index))
			return false;
	}

	return true;
}

void Chams::SceneEnd() {
	// store and sort ents by distance.
	if (SortPlayers()) {
		// iterate each player and render them.
		for (const auto& p : m_players)
			RenderPlayer(p);
	}

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride(nullptr);
	g_csgo.m_render_view->SetColorModulation(colors::white);
	g_csgo.m_render_view->SetBlend(1.f);
}

IMaterial* Chams::GetMaterial(int i) {
	switch (i) {
	case 0:
		return debugambientcube;
	case 1:
		return debugdrawflat;
	case 2:
		return metallic;
	}

	return nullptr;
}

void Chams::RenderPlayer(Player* player) {
	// prevent recruisive model cancelation.
	m_running = true;

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride(nullptr);
	g_csgo.m_render_view->SetColorModulation(colors::white);
	g_csgo.m_render_view->SetBlend(1.f);

	if (metallic) {
		auto $envmaptint = metallic->FindVar(XOR("$envmaptint"), nullptr);
		if ($envmaptint)
			$envmaptint->SetVecValue(1.0, 1.0, 1.0);

		auto $phongboost = metallic->FindVar(XOR("$phongboost"), nullptr);
		if ($phongboost)
			$phongboost->SetVecValue(0.2, 0.2, 0.2);

		auto $rimlightboost = metallic->FindVar(XOR("$rimlightboost"), nullptr);
		if ($rimlightboost)
			$rimlightboost->SetFloatValue(1.0);
	}

	if (g_menu.main.aimbot.shot_matrix.get() == 3) {
		for (int i = 0; i < m_shots.size(); i++)
		{
			ShotMatrix* it = &m_shots.at(i);

			float time_delta_clamped = std::clamp(g_csgo.m_globals->m_curtime - it->m_time - 4.f, 0.f, 1.f);

			if (time_delta_clamped >= 1.0f) {
				m_shots.erase(m_shots.begin() + i);
				continue;
			}

			if (it->m_i != player->index())
				continue;

			// override blend.
			SetAlpha((g_menu.main.aimbot.shot_matrix_color.get().a() / 255.f) * (1.f - time_delta_clamped));

			// set material and color.
			SetupMaterial(GetMaterial(g_menu.main.aimbot.shot_matrix_material.get()), g_menu.main.aimbot.shot_matrix_color.get(), true);

			// backup the bone cache before we fuck with it.
			auto backup_bones = player->m_BoneCache().m_pCachedBones;

			// replace their bone cache with our custom one.
			player->m_BoneCache().m_pCachedBones = it->m_bones;

			// manually draw the model.
			player->DrawModel();

			// reset their bone cache to the previous one.
			player->m_BoneCache().m_pCachedBones = backup_bones;
		}
	}

	// check if is an enemy.
	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);

	if (player->alive()) {
		// this is the local player.
		// we always draw the local player manually in drawmodel.
		if (player == g_cl.m_local) {
			if (g_menu.main.players.chams_local_scope.get() && player->m_bIsScoped())
				SetAlpha(0.5f);

			else if (g_menu.main.players.chams_local.get()) {
				// override blend.
				SetAlpha(g_menu.main.players.chams_local_col.get().a() / 255.f);

				// set material and color.
				SetupMaterial(GetMaterial(g_menu.main.players.chams_local_mat.get()), g_menu.main.players.chams_local_col.get(), false);
			}

			// manually draw the model.
			player->DrawModel();
		}

		if (enemy && g_menu.main.players.chams_enemy_history.get()) {
			RenderHistoryChams(player->index());
		}

		if (enemy && g_menu.main.players.chams_enemy.get(0)) {
			if (g_menu.main.players.chams_enemy.get(1)) {

				SetAlpha(g_menu.main.players.chams_enemy_invis.get().a() / 255.f);
				SetupMaterial(GetMaterial(g_menu.main.players.chams_enemy_mat.get()), g_menu.main.players.chams_enemy_invis.get(), true);

				player->DrawModel();
			}

			SetAlpha(g_menu.main.players.chams_enemy_vis.get().a() / 255.f);
			SetupMaterial(GetMaterial(g_menu.main.players.chams_enemy_mat.get()), g_menu.main.players.chams_enemy_vis.get(), false);

			player->DrawModel();
		}

		else if (!enemy && g_menu.main.players.chams_friendly.get(0)) {
			if (g_menu.main.players.chams_friendly.get(1)) {

				SetAlpha(g_menu.main.players.chams_friendly_invis.get().a() / 255.f);
				SetupMaterial(GetMaterial(g_menu.main.players.chams_friendly_mat.get()), g_menu.main.players.chams_friendly_invis.get(), true);

				player->DrawModel();
			}

			SetAlpha(g_menu.main.players.chams_friendly_vis.get().a() / 255.f);
			SetupMaterial(GetMaterial(g_menu.main.players.chams_friendly_mat.get()), g_menu.main.players.chams_friendly_vis.get(), false);

			player->DrawModel();
		}
	}

	m_running = false;
}

bool Chams::SortPlayers() {
	// lambda-callback for std::sort.
	// to sort the players based on distance to the local-player.
	static auto distance_predicate = [](Entity* a, Entity* b) {
		vec3_t local = g_cl.m_local->GetAbsOrigin();

		// note - dex; using squared length to save out on sqrt calls, we don't care about it anyway.
		float len1 = (a->GetAbsOrigin() - local).length_sqr();
		float len2 = (b->GetAbsOrigin() - local).length_sqr();

		return len1 < len2;
		};

	// reset player container.
	m_players.clear();

	// find all players that should be rendered.
	for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
		// get player ptr by idx.
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

		// validate.
		if (!player || !player->IsPlayer() || player->dormant())
			continue;

		// do not draw players occluded by view plane.
		if (!IsInViewPlane(player->WorldSpaceCenter()))
			continue;

		// this player was not skipped to draw later.
		// so do not add it to our render list.
		if (!OverridePlayer(i))
			continue;

		m_players.push_back(player);
	}

	// any players?
	if (m_players.empty())
		return false;

	// sorting fixes the weird weapon on back flickers.
	// and all the other problems regarding Z-layering in this shit game.
	std::sort(m_players.begin(), m_players.end(), distance_predicate);

	return true;
}
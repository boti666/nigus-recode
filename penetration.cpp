#include "includes.h"

bool IsArmored(Player* player, const int hitgroup)
{
	const bool bHasHelmet = player->m_bHasHelmet();
	const bool bHasHeavyArmor = player->m_bHasHeavyArmor();
	const float flArmorValue = player->m_ArmorValue();

	if (flArmorValue > 0) {
		switch (hitgroup) {
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
		case HITGROUP_GEAR:
			return true;
			break;
		case HITGROUP_HEAD:
			return bHasHelmet || bHasHeavyArmor;
			break;
		default:
			return bHasHeavyArmor;
			break;
		}
	}

	return false;
}

float penetration::scale(Player* player, float damage, float armor_ratio, int hitgroup) {
	static ConVar* mp_damage_scale_ct_head = g_csgo.m_cvar->FindVar(HASH("mp_damage_scale_ct_head"));
	static ConVar* mp_damage_scale_ct_body = g_csgo.m_cvar->FindVar(HASH("mp_damage_scale_ct_body"));
	static ConVar* mp_damage_scale_t_head = g_csgo.m_cvar->FindVar(HASH("mp_damage_scale_t_head"));
	static ConVar* mp_damage_scale_t_body = g_csgo.m_cvar->FindVar(HASH("mp_damage_scale_t_body"));

	const int nTeamNum = player->m_iTeamNum();
	float flHeadScale = nTeamNum == TEAM_CT ? mp_damage_scale_ct_head->GetFloat() : mp_damage_scale_t_head->GetFloat();
	const float flBodyScale = nTeamNum == TEAM_CT ? mp_damage_scale_ct_body->GetFloat() : mp_damage_scale_t_body->GetFloat();

	const bool bIsArmored = IsArmored(player, hitgroup);
	const float flArmorValue = static_cast<float>(player->m_ArmorValue());

	switch (hitgroup) {
	case HITGROUP_HEAD:
		damage *= 4.f;
		damage *= flHeadScale;
		break;
	case HITGROUP_STOMACH:
		damage *= 1.25f;
		damage *= flBodyScale;
		break;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	case HITGROUP_GEAR:
		damage *= flBodyScale;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		damage *= 0.75f;
		damage *= flBodyScale;
		break;
	default:
		break;
	}

	// enemy have armor
	if (bIsArmored) {
		float flArmorScale = 1.f;
		float flArmorBonusRatio = 0.5f;
		float flArmorRatioCalculated = armor_ratio * 0.5f;
		float fDamageToHealth = damage * flArmorRatioCalculated;
		float fDamageToArmor = (damage - fDamageToHealth) * (flArmorScale * flArmorBonusRatio);

		// Does this use more armor than we have?
		if (fDamageToArmor > flArmorValue)
			fDamageToHealth = damage - (flArmorValue / flArmorBonusRatio);

		damage = fDamageToHealth;
	}

	return std::floor(damage);
}

bool penetration::TraceToExit(const vec3_t& start, const vec3_t& dir, vec3_t& out, CGameTrace* enter_trace, CGameTrace* exit_trace) {
	static CTraceFilterSimple_game filter{};

	float  dist{};
	vec3_t new_end;
	int    contents, first_contents{};

	// max pen distance is 90 units.
	while (dist <= 90.f) {
		// step forward a bit.
		dist += 4.f;

		// set out pos.
		out = start + (dir * dist);

		if (!first_contents)
			first_contents = g_csgo.m_engine_trace->GetPointContents(out, MASK_SHOT, nullptr);

		contents = g_csgo.m_engine_trace->GetPointContents(out, MASK_SHOT, nullptr);

		if ((contents & MASK_SHOT_HULL) && (!(contents & CONTENTS_HITBOX) || (contents == first_contents)))
			continue;

		// move end pos a bit for tracing.
		new_end = out - (dir * 4.f);

		// do first trace aHR0cHM6Ly9zdGVhbWNvbW11bml0eS5jb20vaWQvc2ltcGxlcmVhbGlzdGlj.
		g_csgo.m_engine_trace->TraceRay(Ray(out, new_end), MASK_SHOT, nullptr, exit_trace);

		// note - dex; this is some new stuff added sometime around late 2017 ( 10.31.2017 update? ).
		if (g_csgo.sv_clip_penetration_traces_to_players->GetInt())
			game::UTIL_ClipTraceToPlayers(out, new_end, MASK_SHOT, nullptr, exit_trace, -60.f);

		// we hit an ent's hitbox, do another trace.
		if (exit_trace->m_startsolid && (exit_trace->m_surface.m_flags & SURF_HITBOX)) {
			filter.SetPassEntity(exit_trace->m_entity);

			g_csgo.m_engine_trace->TraceRay(Ray(out, start), MASK_SHOT, (ITraceFilter*)&filter, exit_trace);

			if (exit_trace->hit() && !exit_trace->m_startsolid) {
				out = exit_trace->m_endpos;
				return true;
			}

			continue;
		}

		if (!exit_trace->hit() || exit_trace->m_startsolid) {
			if (game::IsBreakable(enter_trace->m_entity)) {
				*exit_trace = *enter_trace;
				exit_trace->m_endpos = start + dir;
				return true;
			}

			continue;
		}

		if ((exit_trace->m_surface.m_flags & SURF_NODRAW)) {
			// note - dex; ok, when this happens the game seems to not ignore world?
			if (game::IsBreakable(exit_trace->m_entity) && game::IsBreakable(enter_trace->m_entity)) {
				out = exit_trace->m_endpos;
				return true;
			}

			if (!(enter_trace->m_surface.m_flags & SURF_NODRAW))
				continue;
		}

		if (exit_trace->m_plane.m_normal.dot(dir) <= 1.f) {
			out -= (dir * (exit_trace->m_fraction * 4.f));
			return true;
		}
	}

	return false;
}

void penetration::ClipTraceToPlayer(const vec3_t& start, const vec3_t& end, uint32_t mask, CGameTrace* tr, Player* player, float min) {
	vec3_t     pos, to, dir, on_ray;
	float      len, range_along, range;
	Ray        ray;
	CGameTrace new_trace;

	// reference: https://github.com/alliedmodders/hl2sdk/blob/3957adff10fe20d38a62fa8c018340bf2618742b/game/shared/util_shared.h#L381

	// set some local vars.
	pos = player->m_vecOrigin() + ((player->m_vecMins() + player->m_vecMaxs()) * 0.5f);
	to = pos - start;
	dir = start - end;
	len = dir.normalize();
	range_along = dir.dot(to);

	// off start point.
	if (range_along < 0.f)
		range = -(to).length();

	// off end point.
	else if (range_along > len)
		range = -(pos - end).length();

	// within ray bounds.
	else {
		on_ray = start + (dir * range_along);
		range = (pos - on_ray).length();
	}

	if ( /*min <= range &&*/ range <= 60.f) {
		// clip to player.
		g_csgo.m_engine_trace->ClipRayToEntity(Ray(start, end), mask, player, &new_trace);

		if (tr->m_fraction > new_trace.m_fraction)
			*tr = new_trace;
	}
}

bool penetration::run(PenetrationInput_t* in, PenetrationOutput_t* out) {
	static CTraceFilterSkipTwoEntities_game filter{};

	int			  pen{ 4 }, enter_material, exit_material;
	float		  damage, penetration, remaining, trace_len{};
	surfacedata_t* enter_surface, * exit_surface;
	bool		  nodraw, grate, solid_surf, light_surf;
	vec3_t		  start, dir, end, pen_end;
	CGameTrace	  trace, exit_trace;
	Weapon* weapon;
	WeaponInfo* weapon_info;

	static ConVar* ff_damage_reduction_bullets = g_csgo.m_cvar->FindVar(HASH("ff_damage_reduction_bullets"));

	// if we are tracing from our local player perspective.
	if (in->m_from == g_cl.m_local) {
		weapon = g_cl.m_weapon;
		weapon_info = g_cl.m_weapon_info;
		start = g_cl.m_shoot_pos;
	}

	// not local player.
	else {
		weapon = in->m_from->GetActiveWeapon();
		if (!weapon)
			return false;

		// get weapon info.
		weapon_info = weapon->GetWpnData();
		if (!weapon_info)
			return false;

		// set trace start.
		start = in->m_from->GetShootPosition();
	}

	// get some weapon data.
	damage = (float)weapon_info->m_damage;
	penetration = weapon_info->m_penetration;

	// get direction to end point.
	dir = (in->m_pos - start).normalized();

	// setup trace filter for later.
	filter.SetPassEntity(in->m_from);

	remaining = weapon_info->m_range;

	while (damage > 0.f) {
		// calculating remaining len.
		remaining -= trace_len;

		// set trace end.
		end = start + (dir * remaining);

		// setup ray and trace.
		// TODO; use UTIL_TraceLineIgnoreTwoEntities?
		g_csgo.m_engine_trace->TraceRay(Ray(start, end), MASK_SHOT, (ITraceFilter*)&filter, &trace);

		// check for player hitboxes extending outside their collision bounds.
		// if no target is passed we clip the trace to a specific player, otherwise we clip the trace to any player.
		if (in->m_target)
			ClipTraceToPlayer(start, end + (dir * 40.f), MASK_SHOT, &trace, in->m_target, -60.f);
		else
			game::UTIL_ClipTraceToPlayers(start, end + (dir * 40.f), MASK_SHOT, (ITraceFilter*)&filter, &trace, -60.f);

		// we didn't hit anything.
		if (trace.m_fraction == 1.f)
			return false;

		// calculate damage based on the distance the bullet traveled.
		trace_len += trace.m_fraction * remaining;
		damage *= pow(weapon_info->m_range_modifier, trace_len / 500.f);

		// if a target was passed.
		if (in->m_target) {

			// validate that we hit the target we aimed for.
			if (trace.m_entity && trace.m_entity == in->m_target && game::IsValidHitgroup(trace.m_hitgroup)) {
				int group = (weapon->m_iItemDefinitionIndex() == ZEUS) ? HITGROUP_GENERIC : trace.m_hitgroup;

				// set result data for when we hit a player.
				out->m_pen = pen != 4;
				out->m_hitgroup = group;
				out->m_hitbox = trace.m_hitbox;
				out->m_damage = scale(in->m_target, damage, weapon_info->m_armor_ratio, group);
				out->m_target = in->m_target;

				// non-penetrate damage.
				if (pen == 4)
					return out->m_damage >= in->m_damage;

				// penetration damage.
				return out->m_damage >= in->m_damage_pen;
			}
		}

		// no target was passed, check for any player hit or just get final damage done.
		else {
			out->m_pen = pen != 4;

			// todo - dex; team checks / other checks / etc.
			if (trace.m_entity && trace.m_entity->IsPlayer() && game::IsValidHitgroup(trace.m_hitgroup)) {
				int group = (weapon->m_iItemDefinitionIndex() == ZEUS) ? HITGROUP_GENERIC : trace.m_hitgroup;

				// set result data for when we hit a player.
				out->m_hitgroup = group;
				out->m_hitbox = trace.m_hitbox;
				out->m_damage = scale(trace.m_entity->as< Player* >(), damage, weapon_info->m_armor_ratio, group);;
				out->m_target = trace.m_entity->as< Player* >();

				// non-penetrate damage.
				if (pen == 4)
					return out->m_damage >= in->m_damage;

				// penetration damage.
				return out->m_damage >= in->m_damage_pen;
			}

			// if we've reached here then we didn't hit a player yet, set damage and hitgroup.
			out->m_damage = damage;
		}

		// don't run pen code if it's not wanted.
		if (!in->m_can_pen || trace.m_startsolid)
			return false;

		// get surface at entry point.
		enter_surface = g_csgo.m_phys_props->GetSurfaceData(trace.m_surface.m_surface_props);

		// this happens when we're too far away from a surface and can penetrate walls or the surface's pen modifier is too low.
		if ((trace_len > 3000.f && penetration) || enter_surface->m_game.m_penetration_modifier < 0.1f)
			return false;

		// store data about surface flags / contents.
		nodraw = (trace.m_surface.m_flags & SURF_NODRAW);
		grate = (trace.m_contents & CONTENTS_GRATE);
		solid_surf = trace.m_contents >> 3 & CONTENTS_SOLID;
		light_surf = trace.m_surface.m_flags >> 7 & SURF_LIGHT;

		// get material at entry point.
		enter_material = enter_surface->m_game.m_material;

		// note - dex; some extra stuff the game does.
		if (!pen && !nodraw && !grate && enter_material != CHAR_TEX_GRATE && enter_material != CHAR_TEX_GLASS)
			return false;

		// no more pen.
		if (penetration <= 0.f || pen <= 0)
			return false;

		// try to penetrate object.
		if (!TraceToExit(trace.m_endpos, dir, pen_end, &trace, &exit_trace)) {
			if (!(g_csgo.m_engine_trace->GetPointContents(pen_end, MASK_SHOT_HULL) & MASK_SHOT_HULL))
				return false;
		}

		// get surface / material at exit point.
		exit_surface = g_csgo.m_phys_props->GetSurfaceData(exit_trace.m_surface.m_surface_props);
		exit_material = exit_surface->m_game.m_material;

		float combined_penetration_modifier = 0.f;
		float final_damage_modifier = 0.16f;

		if (grate || nodraw || enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE)
		{
			// If we're a concrete grate (TOOLS/TOOLSINVISIBLE texture) allow more penetrating power.
			if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE)
			{
				combined_penetration_modifier = 3.0f;
				final_damage_modifier = 0.05f;
			}
			else
				combined_penetration_modifier = 1.0f;
		}
		else if (solid_surf || light_surf)
		{
			combined_penetration_modifier = 1.f;
		}
		else if (enter_material == CHAR_TEX_FLESH)
		{

			combined_penetration_modifier = ff_damage_reduction_bullets->GetInt();
			if (combined_penetration_modifier == 0.f)
				return false;
		}
		else
		{
			combined_penetration_modifier = (enter_surface->m_game.m_penetration_modifier + exit_surface->m_game.m_penetration_modifier) / 2.f;
		}

		if (enter_material == exit_material)
		{
			if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
				combined_penetration_modifier = 3.f;
			else if (exit_material == CHAR_TEX_PLASTIC)
				combined_penetration_modifier = 2.f;
		}

		float thickness = (exit_trace.m_endpos - trace.m_endpos).length();
		thickness *= thickness;
		thickness *= fmaxf(0.f, 1.0f / combined_penetration_modifier);
		thickness /= 24.0f;

		const float v35 = (damage * final_damage_modifier) + fmaxf(0.f, 1.0f / combined_penetration_modifier) * 3.0f * fmaxf(0.0f, (3.0f / penetration) * 1.25f);

		const float lost_damage = fmaxf(0.0f, v35 + thickness);

		if (lost_damage > damage)
			return false;

		if (lost_damage > 0.f)
			damage -= lost_damage;

		if (damage < 1.f)
			return false;

		start = exit_trace.m_endpos;

		--pen;
	}

	return false;
}

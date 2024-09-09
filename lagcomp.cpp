#include "includes.h"

LagCompensation g_lagcomp{};;

LagRecord* LagCompensation::StartPrediction(AimPlayer* data, LagRecord* pred_record) {
	// we have no data to work with.
	// this should never happen if we call this
	if (data->m_records.empty())
		return nullptr;

	Player* player = data->m_player;

	CCSGOPlayerAnimState* state = player->m_PlayerAnimState();

	if (!state)
		return nullptr;

	// get first record.
	LagRecord* current = &data->m_records[0];
	LagRecord* previous = data->m_records.size() > 1 ? &data->m_records[1] : nullptr;

	if (!current || !previous)
		return nullptr;

	if (current->immune())
		return nullptr;

	if (previous->immune())
		return nullptr;

	if (current->m_breaking_lc[0] || !current->m_breaking_lc[1])
		return nullptr;

	vec3_t predicted_origin{ current->m_origin };
	vec3_t predicted_vel{ current->m_velocity[0] };
	int predicted_flags{ current->m_anim_flags };

	for (int tick = 1; tick <= current->m_lag;) {
		if (!(predicted_flags & FL_ONGROUND)) {
			if (previous->m_anim_flags & FL_ONGROUND)
				predicted_vel.z = g_csgo.sv_jump_impulse->GetFloat();

			predicted_vel.z -= g_csgo.m_globals->m_interval * g_csgo.sv_gravity->GetFloat();
		}

		PlayerMove(player, predicted_origin, predicted_vel, predicted_flags);
		++tick;
	}

	const int latency = g_cl.m_latency_ticks * 2;

	float target_time = 0.f;

	int next_update = game::TIME_TO_TICKS(current->m_sim_time) + current->m_lag - 1;

	if (next_update >= g_cl.m_arrival_tick || latency <= 0)
		return current;

	if (!g_menu.main.aimbot.lagfix.get())
		return nullptr;

	const float curtime{ g_csgo.m_globals->m_curtime }, frametime{ g_csgo.m_globals->m_frametime };

	//TODO: free the memory!!!
	LagRecord* new_record = new LagRecord();

	new_record->store(player);

	new_record->m_predicted = true;

	new_record->m_lag = current->m_lag;
	new_record->m_eye_angles = current->m_eye_angles;

	new_record->m_anim_time = new_record->m_sim_time + g_csgo.m_globals->m_interval;
	new_record->m_sim_time = game::TICKS_TO_TIME(next_update);

	new_record->m_origin = predicted_origin;
	new_record->m_velocity[0] = predicted_vel;
	new_record->m_flags = predicted_flags;

	g_csgo.m_globals->m_curtime = new_record->m_anim_time;
	g_csgo.m_globals->m_frametime = g_csgo.m_globals->m_interval;

	const ang_t angles{ player->m_angEyeAngles() }, absangles{ player->GetAbsAngles() };
	const vec3_t origin{ player->m_vecOrigin() }, abs_origin{ player->GetAbsOrigin() },
		velocity{ player->m_vecVelocity() }, abs_velocity{ player->m_vecAbsVelocity() };

	const int flags{ player->m_fFlags() }, eflags{ player->m_iEFlags() };

	player->m_iEFlags() &= ~0x1000;

	player->m_vecOrigin() = predicted_origin;
	player->SetAbsOrigin(predicted_origin);
	player->m_vecAbsVelocity() = player->m_vecVelocity() = predicted_vel;
	player->m_fFlags() = predicted_flags;
	player->m_angEyeAngles() = new_record->m_eye_angles;

	g_resolver.ResolveAngles(player, new_record, previous);

	UpdateClientAnimations::DoUpdate(player);

	g_csgo.m_globals->m_curtime = new_record->m_sim_time;

	new_record->m_setup = g_bones.BuildBones(player, BONE_USED_BY_ANYTHING, new_record->m_bones, true);

	player->m_vecOrigin() = origin;
	player->SetAbsOrigin(abs_origin);
	player->m_vecAbsVelocity() = abs_velocity;
	player->m_vecVelocity() = velocity;
	player->m_fFlags() = flags;
	player->m_iEFlags() = eflags;
	player->m_angEyeAngles() = angles;
	player->SetAbsAngles(absangles);

	g_csgo.m_globals->m_curtime = curtime;
	g_csgo.m_globals->m_frametime = frametime;

	if ((predicted_origin - current->m_origin).length_sqr() <= 4096.f) {// player will not break lc.
		// the next update they will be valid so we need to check for valid_tick.
		return new_record->valid() ? new_record : nullptr;
	}

	return new_record;
}

void LagCompensation::PlayerMove(Player* target, vec3_t& pred_origin, vec3_t& pred_vel, int& pred_flags) {
	vec3_t                end, normal;
	CGameTrace            trace;
	CTraceFilterWorldOnly filter;

	vec3_t start{ pred_origin }, mins{ target->m_vecMins() }, maxs{ target->m_vecMins() };

	// move trace end one tick into the future using predicted velocity.
	end = start + (pred_vel * g_csgo.m_globals->m_interval);

	// trace.
	g_csgo.m_engine_trace->TraceRay(Ray(start, end, mins, maxs), CONTENTS_SOLID, &filter, &trace);

	// we hit shit
	// we need to fix hit.
	if (trace.m_fraction != 1.f) {

		// fix sliding on planes.
		for (int i{}; i < 2; ++i) {
			pred_vel -= trace.m_plane.m_normal * pred_vel.dot(trace.m_plane.m_normal);

			float adjust = pred_vel.dot(trace.m_plane.m_normal);
			if (adjust < 0.f)
				pred_vel -= (trace.m_plane.m_normal * adjust);

			start = trace.m_endpos;
			end = start + (pred_vel * (g_csgo.m_globals->m_interval * (1.f - trace.m_fraction)));

			g_csgo.m_engine_trace->TraceRay(Ray(start, end, mins, maxs), CONTENTS_SOLID, &filter, &trace);
			if (trace.m_fraction == 1.f)
				break;
		}
	}

	// set new final origin.
	start = end = pred_origin = trace.m_endpos;

	// move endpos 2 units down.
	// this way we can check if we are in/on the ground.
	end.z -= 2.f;

	// trace.
	g_csgo.m_engine_trace->TraceRay(Ray(start, end, mins, maxs), CONTENTS_SOLID, &filter, &trace);

	// strip onground flag.
	pred_flags &= ~FL_ONGROUND;

	// add back onground flag if we are onground.
	if (trace.m_fraction != 1.f && trace.m_plane.m_normal.z > 0.7f)
		pred_flags |= FL_ONGROUND;
}
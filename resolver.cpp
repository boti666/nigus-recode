#include "includes.h"
Resolver g_resolver{};;

LagRecord* Resolver::FindIdealRecord(AimPlayer* data) {
	LagRecord* best = nullptr;

	if (data->m_records.empty())
		return nullptr;

	if (data->m_records.size() >= 2) {
		LagRecord* current = &data->m_records[0];
		if (current->m_breaking_lc[1]) {
			int next_update_tick = game::TIME_TO_TICKS(current->m_sim_time) + current->m_lag;

			if (next_update_tick > g_cl.m_arrival_tick)
				return current;
		}
	}

	// iterate records.
	for (auto& it : data->m_records) {
		if (!it.valid() || it.m_breaking_lc[0] || it.m_breaking_lc[1])
			continue;

		// get current record.
		best = &it;
		break;
	}

	// none found above, return the first valid record if possible.
	return best;
}

LagRecord* Resolver::FindLastRecord(AimPlayer* data) {
	LagRecord* current = nullptr;

	if (data->m_records.empty())
		return nullptr;

	// iterate records in reverse.
	for (auto& it : data->m_records) {
		if (!it.valid() || it.m_breaking_lc[0] || it.m_breaking_lc[1])
			continue;

		// get current record.
		current = &it;
	}

	return current;
}

float Resolver::GetAwayAngle(LagRecord* record, float add) {
	ang_t  away;
	math::VectorAngles(g_cl.m_local->m_vecOrigin() - record->m_origin, away);
	return math::NormalizedAngle(away.y + add);
}

float Resolver::GetVelocityAngle(LagRecord* record) {
	ang_t  velocity;
	math::VectorAngles(record->m_velocity[1], velocity);
	return velocity.y;
}



void Resolver::MatchShot(AimPlayer* data, LagRecord* record) {
	LagRecord* previous = data->m_records.size() >= 2 ? &data->m_records[1] : nullptr;

	if (!previous)
		return;

	float shoot_time = -1.f;

	Weapon* weapon = data->m_player->GetActiveWeapon();
	if (weapon) {
		// with logging this time was always one tick behind.
		// so add one tick to the last shoot time.
		shoot_time = weapon->m_fLastShotTime() + g_csgo.m_globals->m_interval;
	}

	// this record has a shot on it.
	if (shoot_time > previous->m_sim_time && shoot_time <= record->m_sim_time) {
		record->m_shot = true;

		if (game::TIME_TO_TICKS(shoot_time) != game::TIME_TO_TICKS(record->m_anim_time))
			record->m_eye_angles.x = previous->m_eye_angles.x;
	}
}

class AntiFsAngle {
public:
	float m_base, m_add;
	float m_dist;
public:
	// ctor.
	__forceinline AntiFsAngle(float yaw, float add) {
		// set yaw.
		m_base = yaw;
		m_add = add;
		m_dist = 0.f;
	}
};

void Resolver::AntiFreestand(LagRecord* record, float& current_yaw, float multiplier) {
	// constants
	constexpr float STEP{ 4.f };
	constexpr float RANGE{ 32.f };

	// best target.
	vec3_t enemypos = record->m_player->GetShootPosition();
	float away = GetAwayAngle(record, 0.f);

	// construct vector of angles to test.
	std::vector< AdaptiveAngle > angles{ };
	angles.emplace_back(away, 180.f);
	angles.emplace_back(away, 90.f);
	angles.emplace_back(away, -90.f);

	// start the trace at the enemy shoot pos.
	vec3_t start = g_cl.m_local->GetShootPosition();

	// see if we got any valid result.
	// if this is false the path was not obstructed with anything.
	bool valid{ false };

	// iterate vector of angles.
	for (auto it = angles.begin(); it != angles.end(); ++it) {
		float yaw = math::NormalizedAngle(it->m_base + it->m_add);

		// compute the 'rough' estimation of where our head will be.
		vec3_t end{ enemypos.x + std::cos(math::deg_to_rad(yaw)) * RANGE,
			enemypos.y + std::sin(math::deg_to_rad(yaw)) * RANGE,
			enemypos.z };

		// compute the direction.
		vec3_t dir = start - end;
		float len = dir.normalize();

		// should never happen.
		if (len <= 0.f)
			continue;

		// step thru the total distance, 4 units per step.
		for (float i{ 0.f }; i < len; i += STEP) {
			// get the current step position.
			vec3_t point = end + (dir * i);

			// get the contents at this point.
			int contents = g_csgo.m_engine_trace->GetPointContents(point, MASK_SHOT_HULL);

			// contains nothing that can stop a bullet.
			if (!(contents & MASK_SHOT_HULL))
				continue;

			float mult = 1.f;

			// over 50% of the total length, prioritize this shit.
			if (i > (len * 0.5f))
				mult = 1.25f;

			// over 90% of the total length, prioritize this shit.
			if (i > (len * 0.75f))
				mult = 1.25f;

			// over 90% of the total length, prioritize this shit.
			if (i > (len * 0.9f))
				mult = 2.f;

			// append 'penetrated distance'.
			it->m_dist += (STEP * mult);

			// mark that we found anything.
			valid = true;
		}
	}

	if (!valid) {
		current_yaw = math::NormalizedAngle(away - 180.f);
		return;
	}

	// put the most distance at the front of the container.
	std::sort(angles.begin(), angles.end(),
		[](const AdaptiveAngle& a, const AdaptiveAngle& b) {
			return a.m_dist > b.m_dist;
		});

	// the best angle should be at the front now.
	AdaptiveAngle* best = &angles.front();

	current_yaw = math::NormalizedAngle(best->m_base + (best->m_add * multiplier));
}

struct OverrideAngle {
	ang_t m_angle;
	vec3_t m_pos;
};

ang_t calculateAngle(const vec2_t& point1, const vec2_t& point2) {
	ang_t angle;
	vec2_t delta = point2 - point1;

	angle.x = atan2(delta.y, delta.x) * (180.f / math::pi);
	angle.y = atan2(delta.x, delta.y) * (180.f / math::pi);
	angle.z = 0.f;

	return angle;
}

void Resolver::Override(LagRecord* record) {
	if (!record || !record->m_player)
		return;

	if (g_cl.m_override_resolver_toggle && !g_cl.m_override_resolver)
		return;

	OverrideData* data = &m_override_data[record->m_player->index() - 1];

	ang_t viewangles;
	g_csgo.m_engine->GetViewAngles(viewangles);

	vec3_t enemy_eye_pos{};

	record->m_player->GetSmoothEyePos(&enemy_eye_pos);
	g_cl.m_local->GetSmoothEyePos(&data->m_from);

	data->m_dist = (enemy_eye_pos - data->m_from).length();

	vec3_t dir[2];
	math::AngleVectors(m_override_viewangles, &dir[0]);
	dir[0] *= data->m_dist;

	math::AngleVectors(viewangles, &dir[1]);
	dir[1] *= data->m_dist;

	data->m_pos = data->m_from + dir[0];
	data->m_end = data->m_from + dir[1];

	vec2_t screen_pos[2];

	render::WorldToScreen(data->m_pos, screen_pos[0]);
	render::WorldToScreen(data->m_end, screen_pos[1]);

	data->m_record = record;
	data->m_angle = calculateAngle(screen_pos[0], screen_pos[1]).y + GetAwayAngle(record, 0.f);
	data->m_fov = math::GetFOV(m_override_viewangles, data->m_from, enemy_eye_pos);
}

char* Resolver::ResolverModeToString(int mode) {
	switch (mode) {
	case RESOLVE_WALK:
		return "WALK";
	case RESOLVE_PREFLICK:
		return "WALK-STAND";
	case RESOLVE_STAND:
		return "STAND";
	case RESOLVE_STAND_LOGIC:
		return "STAND2";
	case RESOLVE_AIR:
		return "AIR";
	case RESOLVE_FLICK:
		return "FLICK";
	case RESOLVE_FAKEFLICK:
		return "FAKEFLICK";
	}

	return "NONE";
}

void Resolver::SetMode(LagRecord* record) {
	if (record->m_anim_flags & FL_ONGROUND) {
		if (record->m_abs_velocity[1].length_2d() >= 0.1f)
			record->m_mode = RESOLVE_WALK;
		else
			record->m_mode = RESOLVE_STAND;
	}
	else
		record->m_mode = RESOLVE_AIR;
}

void Resolver::ResolveAngles(Player* player, LagRecord* record, LagRecord* previous) {
	AimPlayer* data = &g_aimbot.m_players[player->index() - 1];

	SetMode(record);
	MatchShot(data, record);

	CCSGOPlayerAnimState* state = player->m_PlayerAnimState();

	if (!g_menu.main.aimbot.correct.get()) {
		data->m_resolved_angle = record->m_eye_angles.y;
		return;
	}

	if (record->m_mode == RESOLVE_STAND && previous) {
		bool has_updated = false;

		if (record->m_body != previous->m_body) {
			data->m_body_update = record->m_anim_time + 1.1f;
			data->m_can_predict = true;

			has_updated = true;
		}
		else if (data->m_body_update <= record->m_anim_time && data->m_can_predict) {
			data->m_body_update = record->m_anim_time + 1.1f;

			has_updated = true;
		}

		if (has_updated) {
			if (!data->m_update_count && data->m_moved)
				data->m_lby_delta = math::NormalizedAngle(record->m_body - previous->m_body);

			++data->m_update_count;

			if (data->m_body_index < 2)
				record->m_mode = RESOLVE_FLICK;
		}
	}

	if (previous && record->m_body != previous->m_body)
		data->m_last_body = previous->m_body;

	if (g_menu.main.config.mode.get() == 1)
		record->m_eye_angles.x = 90.f;

	switch (record->m_mode) {
	case RESOLVE_WALK: {
		ResolveWalk(data, record, state);
		data->m_standing = false;
	}break;
	case RESOLVE_STAND: {
		ResolveStand(data, record, previous, state);
		data->m_standing = true;
	}break;
	case RESOLVE_AIR: {
		ResolveAir(data, record, state);
		data->m_standing = false;
	}break;
	case RESOLVE_FLICK: {
		ResolveLby(data, record, state);
		data->m_standing = true;
	}break;
	}

	if (g_cl.m_override_resolver_toggle) {
		if (record->m_mode == RESOLVE_WALK) {
			state->m_abs_yaw = m_override_data[record->m_player->index() - 1].m_angle;
		}
		else if (record->m_mode != RESOLVE_FLICK) {
			data->m_resolved_angle = m_override_data[record->m_player->index() - 1].m_angle;
		}
	}

	if (record->m_lag <= 1) // not fakelagging
		data->m_resolved_angle = record->m_eye_angles.y;

	if (record->m_mode != RESOLVE_WALK && record->m_mode != RESOLVE_FLICK)
		data->m_last_air_stand_mode = record->m_mode;

	if (g_menu.main.aimbot.correct.get() == 3) {
		data->m_resolved_angle = *(float*)((uintptr_t)record->m_player + g_entoffsets.m_flGuardianTooFarDistFrac);

		record->m_mode = RESOLVE_SERVER;
	}

	math::NormalizeAngle(data->m_resolved_angle);
}

void Resolver::ResolveWalk(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state) {
	data->m_body_update = record->m_anim_time + 0.22f;
	data->m_can_predict = true;

	data->m_resolved_angle = record->m_body;
	record->m_mode = RESOLVE_WALK;

	data->m_moved = true;

	data->m_stand_index = 0;
	data->m_stand2_index = 0;
	data->m_body_index = 0;
	data->m_air_index = 0;

	data->m_update_count = 0;
	data->m_lby_delta = 0.f;

	data->m_last_move_time = record->m_anim_time;

	data->m_walk_body = record->m_body;

	data->m_move_away = GetAwayAngle(record, 180.f);
}

void Resolver::ResolveNS(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state) {
	const float away = GetAwayAngle(record, 180.f);
	const float velocity = GetVelocityAngle(record);

	switch (data->m_shots % 6) {
	case 0:
		data->m_resolved_angle = away;
		break;
	case 1:
		data->m_resolved_angle = velocity + 180;
		break;
	case 2:
		data->m_resolved_angle = velocity;
		break;
	case 3:
		data->m_resolved_angle = away + 180.f;
		break;
	case 4:
		data->m_resolved_angle = away + 90.f;
		break;
	case 5:
		data->m_resolved_angle = away - 90.f;
		break;
	}

	record->m_mode = RESOLVE_NOSPREAD;
}

void Resolver::ResolveLogic(AimPlayer* data, LagRecord* record, LagRecord* previous, CCSGOPlayerAnimState* state) {
	if (g_menu.main.aimbot.correct.get() != 2)
		return;

	if (data->m_stand2_index >= 3)
		return;

	float freestand;
	AntiFreestand(record, freestand, 1.f);

	const float away = GetAwayAngle(record, 180.f);

	const float resolve_threshold = g_menu.main.aimbot.correct_threshold.get();

	const float yaw1 = math::NormalizedAngle(record->m_body - data->m_lby_delta);
	const float yaw2 = math::NormalizedAngle(record->m_body + data->m_lby_delta);
	const float lby_change = math::AngleDiff(record->m_body, data->m_last_body);

	const float backwards_delta1 = fabs(math::AngleDiff(away, record->m_body));
	const float backwards_delta2 = fabs(math::AngleDiff(away, yaw1));
	const float backwards_delta3 = fabs(math::AngleDiff(away, yaw2));

	const float backwards_delta_avg = (backwards_delta2 + backwards_delta3) / 2.f;

	const float lby_to_last_move = math::AngleDiff(record->m_body, data->m_walk_body);

	const int act = record->m_player->GetSequenceActivity(record->m_layers[3].m_sequence);

	// preflick resolver.
	if (data->m_update_count && fabs(lby_change) < 100.f && act == 979) {
		if ((record->m_anim_time + game::TICKS_TO_TIME(record->m_lag)) >= data->m_body_update && fabs(lby_change) <= 35.f) {
			const float foot_delta = math::AngleDiff(record->m_body, state->m_abs_yaw);

			if (fabs(foot_delta) < 100.f) {
				if (fabs(foot_delta) > 0.0f) {
					record->m_eye_angles.y = record->m_body + (foot_delta < 0.0f ? 100.f : -100.f);
				}
				else if (fabs(lby_change) > 0.0f) {
					record->m_eye_angles.y = record->m_body + (lby_change < 0.0f ? 100.f : -100.f);
				}

				record->m_mode = RESOLVE_STAND_LOGIC;
				return;
			}
		}
	}

	// backwards resolver.
	if (backwards_delta_avg <= resolve_threshold) {
		if (backwards_delta2 != backwards_delta3 && backwards_delta1 > (backwards_delta2 < backwards_delta3 ? backwards_delta2 : backwards_delta3)) {
			if (backwards_delta2 < backwards_delta3)
				data->m_resolved_angle = yaw1;
			else
				data->m_resolved_angle = yaw2;
		}
		else
			data->m_resolved_angle = away; // FORCE BACKWARDS!!!

		record->m_mode = RESOLVE_STAND_LOGIC;
		return;
	}

	// set base angle.
	data->m_resolved_angle = yaw1;

	const int scan_count = 8;

	// choose lby side with logic.
	for (int i = 0; i <= scan_count;) {
		const float angle = away + ((360.f / scan_count) * i);

		const float delta1 = math::AngleDiff(yaw1, angle);
		const float delta2 = math::AngleDiff(yaw2, angle);
		const float delta3 = math::AngleDiff(record->m_body, angle);

		if (fabs(delta3) < 35.f && act != 979)
			data->m_resolved_angle = record->m_body;

		if (fabs(delta1) < 35.f || fabs(delta2) < 35.f)
			data->m_resolved_angle = delta1 < delta2 ? yaw1 : yaw2;

		++i;
	}

	const float fs_delta1 = math::AngleDiff(freestand, yaw1);
	const float fs_delta2 = math::AngleDiff(freestand, yaw2);

	if (fabs(fs_delta1) < 35.f || fabs(fs_delta2) < 35.f)
		data->m_resolved_angle = freestand;

	record->m_mode = RESOLVE_STAND_LOGIC;
}

void Resolver::ResolveStand(AimPlayer* data, LagRecord* record, LagRecord* previous, CCSGOPlayerAnimState* state) {
	if (g_menu.main.misc.nospread.get()) {
		ResolveNS(data, record, state);
		return;
	}

	const float away = GetAwayAngle(record, 180.f);
	const float time_since_moved = record->m_anim_time - data->m_last_move_time;

	if (time_since_moved < 0.22f) {
		data->m_resolved_angle = record->m_body;
		record->m_mode = RESOLVE_PREFLICK;
		return;
	}

	if (data->m_stand_index % 4 == 0 && !data->m_moved)
		++data->m_stand_index;

	switch (data->m_stand_index % 4) {
	case 0:
		data->m_resolved_angle = data->m_walk_body;
		break;
	case 1:
		AntiFreestand(record, data->m_resolved_angle, 1.f);
		break;
	case 2:
		data->m_resolved_angle = away;
		break;
	case 3:
		AntiFreestand(record, data->m_resolved_angle, -1.f);
		break;
	}

	record->m_mode = RESOLVE_STAND;

	ResolveLogic(data, record, previous, state);
}

void Resolver::ResolveLby(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state) {
	data->m_resolved_angle = record->m_body;
}

void Resolver::ResolveAir(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state) {
	if (g_menu.main.misc.nospread.get()) {
		ResolveNS(data, record, state);
		return;
	}

	const float away = GetAwayAngle(record, 180.f);
	const float velocity = GetVelocityAngle(record);

	switch (data->m_air_index % 3) {
	case 0:
		data->m_resolved_angle = record->m_body - data->m_lby_delta;
		break;
	case 1:
		data->m_resolved_angle = velocity + 176.f;
		break;
	case 2:
		data->m_resolved_angle = velocity;
		break;
	}

	record->m_mode = RESOLVE_AIR;
}
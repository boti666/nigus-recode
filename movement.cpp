#include "includes.h"

Movement g_movement{ };;

void Movement::DoMovement() {
	JumpRelated();
	FakeWalk();

	g_hvh.m_do_fake_flick = g_hvh.m_fake_flick
		&& !fabs(g_cl.m_cmd->m_side_move)
		&& !fabs(g_cl.m_cmd->m_forward_move)
		&& g_cl.m_unpred_netvars.velocity.length_2d() <= 30.f
		&& g_menu.main.antiaim.enable.get();

	if (g_hvh.m_do_fake_flick) {
		float side_move = 0.f;
		float forward_move = 0.f;

		const float speed = 15.f * (260.f / (g_cl.m_weapon_info ? (g_cl.m_local->m_bIsScoped() ? g_cl.m_weapon_info->m_max_player_speed_alt : g_cl.m_weapon_info->m_max_player_speed) : 260.f));

		if (!g_cl.m_lag) {
			g_hvh.m_fake_flick_ticks++;
			if (g_hvh.m_fake_flick_ticks >= 4) {
				forward_move = side_move = g_hvh.m_micro_move ? speed : -speed;
			}
		}

		g_aimbot.m_stop = g_hvh.m_fake_flick_ticks < 4;

		g_cl.m_cmd->m_side_move = side_move;
		g_cl.m_cmd->m_forward_move = forward_move;
	}

	if (g_menu.main.movement.quickstop.get() &&
		!fabs(g_cl.m_cmd->m_side_move) &&
		!fabs(g_cl.m_cmd->m_forward_move) &&
		!(g_cl.m_cmd->m_buttons & IN_JUMP) &&
		(g_cl.m_unpred_netvars.flags & FL_ONGROUND))
		QuickStop();

	if (g_aimbot.m_stop && g_menu.main.aimbot.aimbobs.get(5) &&
		!(g_cl.m_cmd->m_buttons & IN_JUMP) &&
		(g_cl.m_unpred_netvars.flags & FL_ONGROUND)) {

		if (g_cl.m_unpred_netvars.velocity.length_2d() > 25.f)
			QuickStop();
		else {
			g_cl.m_cmd->m_forward_move *= 0.1f;
			g_cl.m_cmd->m_side_move *= 0.1f;
		}
	}

	AutoPeek();
	Strafe();
}

void Movement::JumpRelated() {
	if (g_cl.m_local->m_MoveType() == MOVETYPE_NOCLIP)
		return;

	if ((g_cl.m_cmd->m_buttons & IN_JUMP) && !(g_cl.m_unpred_netvars.flags & FL_ONGROUND)) {
		// bhop.
		if (g_menu.main.movement.bhop.get())
			g_cl.m_cmd->m_buttons &= ~IN_JUMP;

		// duck jump ( crate jump ).
		if (g_menu.main.movement.airduck.get())
			g_cl.m_cmd->m_buttons |= IN_DUCK;
	}
}

void Movement::Strafe() {
	vec3_t velocity;
	float  delta /*abs_delta, velocity_angle, velocity_delta, correct*/;

	// don't strafe while noclipping or on ladders..
	if (g_cl.m_local->m_MoveType() == MOVETYPE_NOCLIP || g_cl.m_local->m_MoveType() == MOVETYPE_LADDER)
		return;

	// disable strafing while pressing shift.
	// don't strafe if not holding primary jump key.
	if ((g_cl.m_buttons & IN_SPEED) || !(g_cl.m_buttons & IN_JUMP) || (g_cl.m_unpred_netvars.flags & FL_ONGROUND))
		return;

	// get networked velocity ( maybe absvelocity better here? ).
	// meh, should be predicted anyway? ill see.
	velocity = g_cl.m_unpred_netvars.velocity;

	if (velocity.length_2d() <= 5.f)
		return;

	if (!g_menu.main.movement.autostrafe.get())
		return;

	static float yaw_add = 0.f;
	static const auto cl_sidespeed = g_csgo.m_cvar->FindVar(HASH("cl_sidespeed"));

	bool back = g_cl.m_cmd->m_buttons & IN_BACK;
	bool forward = g_cl.m_cmd->m_buttons & IN_FORWARD;
	bool right = g_cl.m_cmd->m_buttons & IN_MOVELEFT;
	bool left = g_cl.m_cmd->m_buttons & IN_MOVERIGHT;

	if (back) {
		yaw_add = -180.f;
		if (right)
			yaw_add -= 45.f;
		else if (left)
			yaw_add += 45.f;
	}
	else if (right) {
		yaw_add = 90.f;
		if (back)
			yaw_add += 45.f;
		else if (forward)
			yaw_add -= 45.f;
	}
	else if (left) {
		yaw_add = -90.f;
		if (back)
			yaw_add -= 45.f;
		else if (forward)
			yaw_add += 45.f;
	}
	else {
		yaw_add = 0.f;
	}

	g_cl.m_strafe_angles.y += yaw_add;

	delta = math::AngleDiff(g_cl.m_strafe_angles.y, math::rad_to_deg(atan2(velocity.y, velocity.x)));

	g_cl.m_cmd->m_forward_move = 0.f;
	g_cl.m_cmd->m_side_move = delta > 0.f ? -cl_sidespeed->GetFloat() : cl_sidespeed->GetFloat();

	g_cl.m_strafe_angles.y = math::NormalizedAngle(g_cl.m_strafe_angles.y - math::ApproachAngle(0.f, delta, 2.5f));
}

void Movement::ClampMovementSpeed(float speed)
{
	// thanks onetap.
	if (!g_cl.m_cmd || !g_cl.m_processing)
		return;

	g_cl.m_cmd->m_buttons |= IN_SPEED;

	float squirt = std::sqrtf((g_cl.m_cmd->m_forward_move * g_cl.m_cmd->m_forward_move) + (g_cl.m_cmd->m_side_move * g_cl.m_cmd->m_side_move));

	if (squirt > speed) {
		float squirt2 = std::sqrtf((g_cl.m_cmd->m_forward_move * g_cl.m_cmd->m_forward_move) + (g_cl.m_cmd->m_side_move * g_cl.m_cmd->m_side_move));

		float cock1 = g_cl.m_cmd->m_forward_move / squirt2;
		float cock2 = g_cl.m_cmd->m_side_move / squirt2;

		if (speed + 1.f <= g_cl.m_local->m_vecVelocity().length_2d()) {
			g_cl.m_cmd->m_forward_move = g_cl.m_cmd->m_side_move = 0.f;
		}
		else {
			g_cl.m_cmd->m_forward_move = cock1 * speed;
			g_cl.m_cmd->m_side_move = cock2 * speed;
		}
	}
}

void Movement::NullVelocity(CUserCmd* cmd) {
	vec3_t vel = g_cl.m_local->m_vecVelocity();

	if (vel.length_2d() < 15.f) {
		cmd->m_forward_move = cmd->m_side_move = 0.f;
		return;
	}

	ang_t direction;
	math::VectorAngles(vel, direction);

	ang_t view_angles;
	g_csgo.m_engine->GetViewAngles(view_angles);

	direction.y = view_angles.y - direction.y;

	vec3_t forward;
	math::AngleVectors(direction, &forward);

	static ConVar* cl_forwardspeed = g_csgo.m_cvar->FindVar(HASH("cl_forwardspeed"));
	static ConVar* cl_sidespeed = g_csgo.m_cvar->FindVar(HASH("cl_sidespeed"));

	const vec3_t negative_forward_direction = forward * -cl_forwardspeed->GetFloat();
	const vec3_t negative_side_direction = forward * -cl_sidespeed->GetFloat();

	cmd->m_forward_move = negative_forward_direction.x;
	cmd->m_side_move = negative_side_direction.y;
}

void Movement::AutoStop() {

	if (!(g_cl.m_local->m_fFlags() & FL_ONGROUND))
		return;

	if (g_cl.m_cmd->m_buttons & IN_JUMP)
		return;

	if (!g_cl.m_weapon_info)
		return;

	if (!g_cl.m_weapon) // sanity check
		return;

	if (!g_cl.m_local || !g_cl.m_processing)
		return;

	if (!g_aimbot.m_stop)
		return;

	Weapon* wpn = g_cl.m_local->GetActiveWeapon();

	if (!wpn)
		return;

	WeaponInfo* wpn_data = wpn->GetWpnData();


	if (!wpn_data)
		return;

	bool minimal_speed = true;
	float max_speed = std::floor(0.333333f * (g_cl.m_local->m_bIsScoped() ? wpn_data->m_max_player_speed_alt : wpn_data->m_max_player_speed));
	if (minimal_speed)
	{
		if (g_cl.m_local->m_vecVelocity().length_2d() < max_speed)
			ClampMovementSpeed(max_speed);
		else
			NullVelocity(g_cl.m_cmd);
	}
}

void Movement::FixMove(CUserCmd* cmd, const ang_t& wish_angles) {
	vec3_t  move, dir;
	float   delta, len;
	ang_t   move_angle;

	// roll nospread fix.
	if (!(g_cl.m_unpred_netvars.flags & FL_ONGROUND) && cmd->m_view_angles.z != 0.f)
		cmd->m_side_move = 0.f;

	// convert movement to vector.
	move = { cmd->m_forward_move, cmd->m_side_move, 0.f };

	// get move length and ensure we're using a unit vector ( vector with length of 1 ).
	len = move.normalize();
	if (!len)
		return;

	// convert move to an angle.
	math::VectorAngles(move, move_angle);

	// calculate yaw delta.
	delta = math::NormalizedAngle(cmd->m_view_angles.y - wish_angles.y);

	// accumulate yaw delta.
	move_angle.y += delta;

	math::NormalizeAngle(move_angle.y);

	// calculate our new move direction.
	// dir = move_angle_forward * move_length
	math::AngleVectors(move_angle, &dir);

	// scale to og movement.
	dir *= len;

	// strip old flags.
	cmd->m_buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT);

	// fix ladder and noclip.
	if (g_cl.m_local->m_MoveType() == MOVETYPE_LADDER) {
		// invert directon for up and down.
		if (cmd->m_view_angles.x >= 45.f)
			dir.x = -dir.x;

		if (fabs(delta) > 65.f)
			dir.x = -dir.x;

		// write to movement.
		cmd->m_forward_move = dir.x;
		cmd->m_side_move = dir.y;

		// set new button flags.
		if (cmd->m_forward_move > 200.f)
			cmd->m_buttons |= IN_FORWARD;

		else if (cmd->m_forward_move < -200.f)
			cmd->m_buttons |= IN_BACK;

		if (cmd->m_side_move > 200.f)
			cmd->m_buttons |= IN_MOVERIGHT;

		else if (cmd->m_side_move < -200.f)
			cmd->m_buttons |= IN_MOVELEFT;
	}

	// we are moving normally.
	else {
		// we must do this for pitch angles that are out of bounds.
		if (cmd->m_view_angles.x < -90.f || cmd->m_view_angles.x > 90.f)
			dir.x = -dir.x;

		// set move.
		cmd->m_forward_move = dir.x;
		cmd->m_side_move = dir.y;

		int mode = g_menu.main.movement.slidewalk.get();

		if (mode > 1)
			mode = rand() % 2;

		// set new button flags.
		switch (mode) {
		case 0: {
			if (cmd->m_forward_move > 0.f)
				cmd->m_buttons |= IN_FORWARD;

			else if (cmd->m_forward_move < 0.f)
				cmd->m_buttons |= IN_BACK;

			if (cmd->m_side_move > 0.f)
				cmd->m_buttons |= IN_MOVERIGHT;

			else if (cmd->m_side_move < 0.f)
				cmd->m_buttons |= IN_MOVELEFT;
		} break;
		case 1: {
			if (cmd->m_forward_move < 0.f)
				cmd->m_buttons |= IN_FORWARD;

			else if (cmd->m_forward_move > 0.f)
				cmd->m_buttons |= IN_BACK;

			if (cmd->m_side_move < 0.f)
				cmd->m_buttons |= IN_MOVERIGHT;

			else if (cmd->m_side_move > 0.f)
				cmd->m_buttons |= IN_MOVELEFT;
		} break;
		}
	}
}

void Movement::AutoPeek() {
	m_autopeek = g_input.GetKeyState(g_menu.main.movement.autopeek.get());

	if (!m_autopeek) {
		m_target_pos = g_cl.m_local->m_vecOrigin();
		m_reached_target = false;
		return;
	}

	if (g_cl.m_old_shot)
		m_reached_target = false;

	vec3_t dist = m_target_pos - g_cl.m_local->m_vecOrigin();

	if (dist.length_2d() <= 5.f) {
		if (!m_reached_target)
			QuickStop();
		m_reached_target = true;
	}

	if (m_reached_target)
		return;

	g_cl.m_cmd->m_forward_move = 450.f;
	g_cl.m_cmd->m_side_move = 0.f;

	math::VectorAngles(dist, g_cl.m_strafe_angles);
}

void Movement::QuickStop() {
	// convert velocity to angular momentum.
	ang_t angle;
	math::VectorAngles(g_cl.m_unpred_netvars.velocity, angle);

	// get our current speed of travel.
	float speed = g_cl.m_unpred_netvars.velocity.length();

	// fix direction by factoring in where we are looking.
	angle.y = g_cl.m_view_angles.y - angle.y;

	// convert corrected angle back to a direction.
	vec3_t direction;
	math::AngleVectors(angle, &direction);

	vec3_t stop = direction * -speed;

	if (speed >= 15.f) {
		g_cl.m_cmd->m_forward_move = stop.x;
		g_cl.m_cmd->m_side_move = stop.y;
	}
	else {
		g_cl.m_cmd->m_forward_move = 0.f;
		g_cl.m_cmd->m_side_move = 0.f;
	}
}

void Movement::FakeWalk() {
	vec3_t velocity{ g_cl.m_unpred_netvars.velocity };
	int    ticks{ 0 }, max{ g_cl.m_fake_walk_ticks };

	if (!g_cl.m_fake_walk || g_hvh.m_fake_flick)
		return;

	g_cl.m_cmd->m_buttons &= ~IN_SPEED;

	if (!g_cl.m_local->GetGroundEntity())
		return;

	// calculate friction.
	float friction = g_csgo.sv_friction->GetFloat() * g_cl.m_local->m_surfaceFriction();

	float max_speed = 260.f;

	if (g_cl.m_weapon && g_cl.m_weapon_info)
		max_speed = g_cl.m_local->m_bIsScoped() ? g_cl.m_weapon_info->m_max_player_speed_alt : g_cl.m_weapon_info->m_max_player_speed;

	for (; ticks < max; ++ticks) {
		// calculate speed.
		float speed = velocity.length();

		// if too slow return.
		if (speed <= 1.f)
			break;

		// bleed off some speed, but if we have less than the bleed, threshold, bleed the threshold amount.
		// calculate the drop amount.
		float drop = max_speed * friction * g_csgo.m_globals->m_interval;

		// scale the velocity.
		float newspeed = std::max(0.f, speed - drop);

		if (newspeed != speed) {
			// determine proportion of old speed we are using.
			newspeed /= speed;

			// adjust velocity according to proportion.
			velocity *= newspeed;
		}
	}

	if (ticks > ((max - 1) - g_cl.m_lag) || !g_cl.m_lag)
		QuickStop();
}
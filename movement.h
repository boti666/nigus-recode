#pragma once

class Movement {
public:
	bool m_autopeek;
	bool m_reached_target;
	vec3_t m_target_pos;

public:
	void DoMovement();
	void JumpRelated();
	void Strafe();
	void ClampMovementSpeed(float speed);
	void NullVelocity(CUserCmd* cmd);
	void AutoStop();
	void FixMove(CUserCmd* cmd, const ang_t& old_angles);
	void AutoPeek();
	void QuickStop();
	void FakeWalk();
};

extern Movement g_movement;
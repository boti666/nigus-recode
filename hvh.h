#pragma once

class AdaptiveAngle {
public:
	float m_base, m_add;
	float m_dist;

public:
	// ctor.
	__forceinline AdaptiveAngle(float yaw, float add) {
		// set yaw.
		m_base = math::NormalizedAngle(yaw);
		m_add = math::NormalizedAngle(add);

		// init distance.
		m_dist = 0.f;
	}
};

enum AntiAimMode : size_t {
	STAND = 0,
	WALK,
	AIR,
};

class HVH {
public:
	bool   m_inverter;

	int    m_manual_mode;

	size_t m_mode;
	int    m_pitch;
	int    m_yaw;
	float  m_jitter_range;
	float  m_rot_range;
	float  m_rotate;
	float  m_rot_speed;
	float  m_rand_update;
	int    m_dir;
	float  m_dir_custom;
	size_t m_base_angle;
	float  m_auto_time;
	float  m_add;

	bool   m_jitter;
	bool   m_twist;
	bool   m_flip_sway;
	bool   m_step_switch;
	int    m_random_lag;
	float  m_next_random_update;
	float  m_random_angle;
	float  m_direction;
	float  m_auto;
	float  m_auto_dist;
	float  m_auto_last;
	float  m_view;

	float  m_distortion_angle;
	bool   m_flip_distortion;
	int    m_distortion_add;

	bool   m_front;
	bool   m_back;
	bool   m_right;
	bool   m_left;
	bool   m_flip_fake_flick;
	bool   m_fake_flick;
	bool   m_do_fake_flick;
	int    m_fake_flick_ticks;
	bool   m_micro_move;

	float  m_desync_delta;

public:
	void IdealPitch();
	void AntiAimPitch();
	void AutoDirection();
	void GetAntiAimDirection();
	bool DoEdgeAntiAim(Player* player, ang_t& out);
	void DoRealAntiAim();
	void DoFakeAntiAim();
	void AntiAim();
	int  ChokePeek();
	void SendPacket();
};

extern HVH g_hvh;
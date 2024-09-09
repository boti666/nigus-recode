#pragma once

class Sequence {
public:
	float m_time;
	int   m_state;
	int   m_seq;

public:
	__forceinline Sequence() : m_time{}, m_state{}, m_seq{} {};
	__forceinline Sequence(float time, int state, int seq) : m_time{ time }, m_state{ state }, m_seq{ seq } {};
};

class NetPos {
public:
	float  m_time;
	vec3_t m_pos;

public:
	__forceinline NetPos() : m_time{}, m_pos{} {};
	__forceinline NetPos(float time, vec3_t pos) : m_time{ time }, m_pos{ pos } {};
};

class serveranimation_data_t {
public:
	C_AnimationLayer m_layers[13];
	float            m_poses[24];

	bool m_ground;
	bool m_old_ground;

	bool m_moving;
	bool m_old_moving;

	bool m_ladder;
	bool m_old_ladder;

	bool m_strafe_changed;
	float m_strafe_duration;

	int m_old_movestate;

	bool m_landing, m_jumping, m_adjust_started, m_defuse_started, m_plant_anim_started;

	float m_primary_cycle, m_move_weight, m_body, m_body_update, 
		m_ladder_weight, 
		m_ladder_speed,
		m_air_smooth,
		m_spawn_time,
		m_duration_in_air,
		m_land_multiplier,
		m_duck_additive,
		m_last_foot_yaw,
		m_stutter_step,
		m_move_yaw,
		m_move_yaw_ideal,
		m_vel_test_time;

	vec3_t m_pos, m_old_pos, m_acceleration, m_lerped_acceleration, m_vel, m_old_vel, m_vel_normalized, m_vel_normalized_non_zero, m_vel_test;

	Weapon* m_weapon, *m_last_weapon;

	float m_increment;
	float m_last_update;

	bool m_updated;

	int m_buttons, m_old_buttons;

	void reset() {
		m_updated = false;

		m_pos = m_old_pos = m_acceleration = m_lerped_acceleration = m_vel = m_old_vel = m_vel_normalized = m_vel_normalized_non_zero = m_vel_test = { 0,0,0 };

		m_increment = m_last_update = 0.f;
	}
};
class ServerAnimations {
public:
	void SetLayerSequence(CCSGOPlayerAnimState* state, C_AnimationLayer* layer, int act, int overrideseq);

	float GetLayerIdealWeightFromSeqCycle(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer);
	bool IsLayerSequenceCompleted(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer);
	Activity GetLayerActivity(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer);
	void RebuildLayers(Player* player, serveranimation_data_t* data);
	void DoAnimationEvent(Player* player, serveranimation_data_t* data);
	int LookupSequence(Player* e, const char* label);
private:
	void IncrementLayerCycle(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer, bool bAllowLoop);
	void IncrementLayerWeight(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer);
	void IncrementLayerCycleWeightRateGeneric(CCSGOPlayerAnimState* m_pAnimstate, C_AnimationLayer* pLayer);
	void UpdateAnimLayer(C_AnimationLayer* pLayer, int nSequence, float flPlaybackRate, float flWeight, float flCycle);
};

class UnpredNetVars {
public:
	int flags, tickbase;
	vec3_t origin, velocity, view_offset;
	ang_t angles, view_punch, aim_punch;
	float velocity_modifier;

	void store(Player* e) {
		flags = e->m_fFlags();
		tickbase = e->m_nTickBase();
		origin = e->m_vecOrigin();
		velocity = e->m_vecVelocity();
		view_offset = e->m_vecViewOffset();
		angles = e->m_angEyeAngles();
		view_punch = e->m_viewPunchAngle();
		aim_punch = e->m_aimPunchAngle();
		velocity_modifier = e->m_flVelocityModifier();
	}

	void restore(Player* e) {
		e->m_fFlags() = flags;
		e->m_nTickBase() = tickbase;
		e->m_vecOrigin() = origin;
		e->m_vecVelocity() = velocity;
		e->m_vecViewOffset() = view_offset;
		e->m_angEyeAngles() = angles;
		e->m_viewPunchAngle() = view_punch;
		e->m_aimPunchAngle() = aim_punch;
		e->m_flVelocityModifier() = velocity_modifier;
	}
};

class Client {
public:
	// hack thread.
	static ulong_t __stdcall init(void* arg);

	void StoreAnimData(C_AnimationLayer* layers, float* poses, ang_t& angle, float& yaw);
	void SetAnimData(C_AnimationLayer* layers, float* poses, ang_t& angle, float& yaw);

	void UpdateShootPos();
	void UpdateShot();
	void StartMove(CUserCmd* cmd);
	void EndMove(CUserCmd* cmd);
	void BackupPlayers(bool restore);
	void DoMove();
	void DrawHUD();
	void UnlockHiddenConvars();
	void UpdateInformation();
	void StoreLayers(C_AnimationLayer* layers);
	void UpdateLayers(C_AnimationLayer* layers);
	void UpdateAnimations();
	void Skybox();
	void KillFeed();

	void OnPaint();
	void OnMapload();
	void OnTick(CUserCmd* cmd);

	// debugprint function.
	void print(const std::string text, ...);

	// check if we are able to fire this tick.
	bool CanFireWeapon();
	void UpdateRevolverCock();
	void UpdateIncomingSequences();

public:
	bool m_override_resolver;
	bool m_override_resolver_toggle;

	struct incoming_seq_t {
		std::ptrdiff_t m_in_seq{}, m_in_rel_state{};
	};

	std::vector < int > m_whitelist_ids{};

	std::vector < incoming_seq_t > m_in_seq{};

	int              m_taps;
	std::deque<int>  m_sent_cmds;

	UnpredNetVars    m_unpred_netvars;

	bool             m_fake_walk;
	int              m_fake_walk_ticks;

	// local player variables.
	float            m_last_standing_yaw;

	Player* m_local;
	bool	         m_body_update;
	bool	         m_pre_body_update;
	bool	         m_processing;
	vec3_t	         m_shoot_pos;
	bool	         m_player_fire;
	int              m_wanted_ticks = 12;
	int              m_charged_ticks;
	bool	         m_shot;
	bool	         m_old_shot;
	float            m_abs_yaw;
	float            m_poses[24];
	C_AnimationLayer m_layers[13];

	serveranimation_data_t m_server_data;

	// active weapon variables.
	Weapon* m_weapon;
	int         m_weapon_id;
	WeaponInfo* m_weapon_info;
	int         m_weapon_type;
	bool        m_weapon_fire;

	// revolver variables.
	int	 m_revolver_cock;
	int	 m_revolver_query;
	bool m_revolver_fire;

	// general game varaibles.
	bool     m_round_end;
	Stage_t	 m_stage;
	int	     m_max_lag;
	int      m_lag;
	int	     m_old_lag;
	bool     *m_packet;
	bool	 m_old_packet;
	float	 m_lerp;
	float    m_latency;
	int      m_latency_ticks;
	int      m_server_tick;
	int      m_arrival_tick;
	int      m_width, m_height;

	// usercommand variables.
	CUserCmd* m_cmd;
	int	      m_tick;
	int	      m_buttons;
	int       m_old_buttons;
	ang_t     m_view_angles;
	ang_t	  m_strafe_angles;
	vec3_t	  m_forward_dir;

	penetration::PenetrationOutput_t m_pen_data;

	std::deque< Sequence > m_sequences;
	std::deque< NetPos >   m_net_pos;

	// animation variables.
	ang_t  m_angle;
	float  m_body;
	float  m_body_pred;
	int    m_body_updates;
	float  m_speed;
	float  m_anim_time;
	float  m_anim_frame;
	bool   m_ground;
	bool   m_lagcomp;
	bool   m_shot_update;

	// hack username.
	std::string m_user;
};

extern Client g_cl;
extern ServerAnimations g_server_animations;
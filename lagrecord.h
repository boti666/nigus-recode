#pragma once

// pre-declare.
class LagRecord;

class BackupRecord {
public:
	BoneArray* m_bones;
	vec3_t     m_origin, m_abs_origin;
	vec3_t     m_mins;
	vec3_t     m_maxs;
	ang_t      m_abs_ang;

public:
	__forceinline void store(Player* player) {
		// store bone data.

		player->GetBones(m_bones);
		m_origin = player->m_vecOrigin();
		m_mins = player->m_vecMins();
		m_maxs = player->m_vecMaxs();
		m_abs_origin = player->GetAbsOrigin();
		m_abs_ang = player->GetAbsAngles();
	}

	__forceinline void restore(Player* player) {
		// get bone cache ptr.

		player->SetBones(m_bones);

		player->m_vecOrigin() = m_origin;
		player->m_vecMins() = m_mins;
		player->m_vecMaxs() = m_maxs;

		player->SetAbsAngles(m_abs_ang);
		player->SetAbsOrigin(m_origin);
	}
};

class LagRecord {
public:
	// data.
	Player* m_player;
	float   m_immune;
	int     m_tick;
	int     m_lag;

	// netvars.
	float  m_sim_time;
	int    m_flags;
	vec3_t m_origin, m_abs_origin;
	vec3_t m_velocity[2];
	vec3_t m_mins;
	vec3_t m_maxs;
	ang_t  m_eye_angles;
	ang_t  m_abs_ang;
	float  m_body;
	float  m_duck;

	// anim stuff.
	C_AnimationLayer m_layers[13];
	float            m_poses[24];

	int    m_anim_flags;
	vec3_t m_abs_velocity[2];
	float  m_anim_duck;

	// bone stuff.
	bool       m_setup, m_breaking_lc[2], m_predicted;
	BoneArray* m_bones;


	//lagfix stuff
	vec3_t m_pred_origin;


	// resolver stuff.
	size_t m_mode;
	bool   m_shot;
	float  m_away;
	float  m_anim_time;
public:

	// default ctor.
	__forceinline LagRecord() :
		m_setup{ false },
		m_shot{ false },
		m_lag{},
		m_bones{} {
	}

	// ctor.
	__forceinline LagRecord(Player* player) :
		m_setup{ false },
		m_shot{ false },
		m_lag{},
		m_bones{} {

		store(player);
	}

	__forceinline ~LagRecord() {
		// free heap allocated game mem.
		if (m_bones)
			g_csgo.m_mem_alloc->Free(m_bones);
	}

	// function: allocates memory for SetupBones and stores relevant data.
	void store(Player* player) {
		if (!m_bones)
			m_bones = (BoneArray*)g_csgo.m_mem_alloc->Alloc(sizeof(BoneArray) * 128);

		// player data.
		m_player = player;
		m_immune = player->m_fImmuneToGunGameDamageTime();
		m_tick = g_csgo.m_cl->m_server_tick;

		// netvars.
		m_sim_time = player->m_flSimulationTime();
		m_flags = player->m_fFlags();
		m_origin = player->m_vecOrigin();
		m_abs_origin = player->GetAbsOrigin();
		m_eye_angles = player->m_angEyeAngles();
		m_abs_ang = player->GetAbsAngles();
		m_body = player->m_flLowerBodyYawTarget();
		m_mins = player->m_vecMins();
		m_maxs = player->m_vecMaxs();
		m_duck = player->m_flDuckAmount();
		m_velocity[0] = player->m_vecVelocity();

		// save networked animlayers.
		player->GetAnimLayers(m_layers);

		// normalize eye angles.
		m_eye_angles.normalize();
		math::clamp(m_eye_angles.x, -90.f, 90.f);

		// get lag.
		m_lag = game::TIME_TO_TICKS(player->m_flSimulationTime() - player->m_flOldSimulationTime());
		m_anim_time = m_sim_time;
	}

	// function: writes current record to bone cache.
	__forceinline void cache() {
		if (!m_bones)
			return;

		// get bone cache ptr.
		m_player->SetBones(m_bones);

		m_player->m_vecOrigin() = m_origin;
		m_player->m_vecMins() = m_mins;
		m_player->m_vecMaxs() = m_maxs;

		m_player->SetAbsAngles(m_abs_ang);
		m_player->SetAbsOrigin(m_origin);
	}

	__forceinline void restore() {
		if (!m_bones)
			return;

		// get bone cache ptr.
		m_player->SetBones(m_bones);

		m_player->m_vecOrigin() = m_origin;
		m_player->m_vecMins() = m_mins;
		m_player->m_vecMaxs() = m_maxs;

		m_player->SetAbsAngles(m_abs_ang);
		m_player->SetAbsOrigin(m_abs_origin);
	}

	__forceinline bool immune() {
		return m_immune > 0.f;
	}

	bool time_in_bounds() {
		// use prediction curtime for this.
		float curtime = g_cl.m_local && g_cl.m_processing ? game::TICKS_TO_TIME(g_cl.m_local->m_nTickBase()) : g_csgo.m_globals->m_curtime;

		// correct is the amount of time we have to correct game time,
		float correct = g_cl.m_lerp;

		// stupid fake latency goes into the incoming latency.
		if (g_csgo.m_net) {
			correct += g_csgo.m_net->GetLatency(INetChannel::FLOW_OUTGOING);
			correct += g_csgo.m_net->GetLatency(INetChannel::FLOW_INCOMING);
		}

		// check bounds [ 0, sv_maxunlag ]
		math::clamp(correct, 0.f, g_csgo.sv_maxunlag->GetFloat());

		// calculate difference between tick sent by player and our latency based tick.
		// ensure this record isn't too old.
		return std::abs(correct - (curtime - m_sim_time)) <= 0.2f;
	}

	bool valid() {
		if (immune())
			return false;

		int dead_time = game::TICKS_TO_TIME(g_cl.m_arrival_tick) - g_csgo.sv_maxunlag->GetFloat();

		if (m_sim_time < (float)dead_time || m_sim_time > game::TICKS_TO_TIME(g_cl.m_arrival_tick))
			return false;

		return time_in_bounds();
	}
};
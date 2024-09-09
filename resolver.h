#pragma once

class ShotRecord;

class OverrideData {
public:
	float      m_dist;
	vec3_t     m_end;

	LagRecord* m_record;
	float      m_fov;

	float      m_angle;

	vec3_t     m_pos, m_from;

	OverrideData() {}
};

class Resolver {
public:
	enum Modes : size_t {
		RESOLVE_DISABLED,
		RESOLVE_WALK,
		RESOLVE_PREFLICK,
		RESOLVE_STAND,
		RESOLVE_STAND_LOGIC,
		RESOLVE_AIR,
		RESOLVE_FLICK,
		RESOLVE_FAKEFLICK,
		RESOLVE_NOSPREAD,
		RESOLVE_SERVER
	};

public:
	std::array<OverrideData, 64> m_override_data;

	void Override(LagRecord* record);

	LagRecord* FindIdealRecord(AimPlayer* data);
	LagRecord* FindLastRecord(AimPlayer* data);

	float GetAwayAngle(LagRecord* record, float add);
	float GetVelocityAngle(LagRecord* record);

	void MatchShot(AimPlayer* data, LagRecord* record);
	void SetMode(LagRecord* record);
	void FindBestAngle(Player* player, LagRecord* record);

	char* ResolverModeToString(int mode);

	void ResolveAngles(Player* player, LagRecord* record, LagRecord* previous);
	void ResolveNS(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state);
	void AntiFreestand(LagRecord* record, float& current_yaw, float multiplier);
	void ResolveWalk(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state);
	void ResolveLogic(AimPlayer* data, LagRecord* record, LagRecord* previous, CCSGOPlayerAnimState* state);
	void ResolveStand(AimPlayer* data, LagRecord* record, LagRecord* previous, CCSGOPlayerAnimState* state);
	void ResolveLby(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state);
	void ResolveAir(AimPlayer* data, LagRecord* record, CCSGOPlayerAnimState* state);

public:
	vec3_t m_override_pos;
	ang_t m_override_viewangles;

	std::array< vec3_t, 64 > m_impacts;
	bool   m_step_switch;
	int    m_random_lag;
	float  m_next_random_update;
	float  m_random_angle;
	float  m_direction;
	float  m_auto;
	float  m_auto_dist;
	float  m_auto_last;
	float  m_view;

	class PlayerResolveRecord
	{
	public:
		struct AntiFreestandingRecord
		{
			int right_damage = 0, left_damage = 0;
			float right_fraction = 0.f, left_fraction = 0.f;
		};

	public:
		AntiFreestandingRecord m_sAntiEdge;
	};

	PlayerResolveRecord player_resolve_records[33];
};

extern Resolver g_resolver;
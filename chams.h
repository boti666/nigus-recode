#pragma once

class ShotMatrix {
public:
	int m_i;
	float m_time;
	BoneArray m_bones[128];

	ShotMatrix() {
		m_i = 0;
		m_time = 0.f;
	}
	ShotMatrix(int index, float time, BoneArray* bones) {
		m_i = index;
		m_time = time;

		std::memcpy(m_bones, bones, sizeof(BoneArray) * 128);
	}
};

class Chams {
public:
	enum model_type_t : uint32_t {
		invalid = 0,
		player,
		weapon,
		arms,
		view_weapon
	};

public:
	model_type_t GetModelType( const ModelRenderInfo_t& info );
	bool IsInViewPlane( const vec3_t& world );

	void SetColor( Color col, IMaterial* mat = nullptr );
	void SetAlpha( float alpha, IMaterial* mat = nullptr );
	void SetupMaterial( IMaterial* mat, Color col, bool z_flag );

	void init( );

	bool OverridePlayer( int index );
	void RenderHistoryChams( int index );
	bool DrawModel( uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone );
	void SceneEnd( );

	void RenderPlayer( Player* player );
	bool SortPlayers( );

	IMaterial* GetMaterial(int i);

public:
	std::vector< ShotMatrix > m_shots;
	std::vector< Player* > m_players;
	bool m_running;
	IMaterial* metallic;
	IMaterial* debugambientcube;
	IMaterial* debugdrawflat;
};

extern Chams g_chams;
#pragma once

namespace CreateMove {
	using fn = void* (__fastcall*)(void*, int, int, float, bool);

	void __stdcall           CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __fastcall          Hook(void* _this, int, int sequence_number, float input_sample_frametime, bool active);


	inline fn original = nullptr;
}

namespace SkipAnimationFrame {
	using fn = bool(__fastcall*)(void* ecx, void* edx);

	bool __fastcall Hook(void* ecx, void* edx);

	inline fn original = nullptr;
}

namespace GetEyeAngles {
	ang_t* __fastcall Hook(void* ecx, void* edx);
	using fn = ang_t * (__fastcall*)(void*, void*);
	inline fn original = nullptr;
}

namespace ModifyEyePosition {
	using fn = void(__fastcall*)(void*, uintptr_t, vec3_t&);

	void __fastcall Hook(void* ecx, uintptr_t, vec3_t& position);

	inline fn original = nullptr;
}

namespace SetupBones {
	bool __fastcall Hook(void* ecx, void* edx, BoneArray* bone_to_world_out, int max_bones, int bone_mask, float curtime);
	using fn = bool(__fastcall*)(void*, void*, BoneArray*, int, int, float);
	inline fn original = nullptr;
}

namespace UpdateClientAnimations {
	void DoUpdate(Player* pl);

	void __fastcall Hook(void* ecx);
	using fn = void(__fastcall*)(void*);
	inline fn original = nullptr;
}

namespace CLMove {
	void __cdecl Hook(float fSamples, bool bFinalTick);
	using fn = void(__cdecl*)(float, bool);
	inline fn original = nullptr;
}

//namespace SetText {
//	void __fastcall Hook(void* ecx, void* edx, const char* tokenName);
//	using fn = void(__fastcall*)(void* ecx, void* edx, const char* tokenName);
//	inline fn original = nullptr;
//}

namespace InterpolatePart {
	int __fastcall Hook(Entity* ent, void* edx, float& curtime, vec3_t& old_origin, vec3_t& old_angles, int& no_more_changes);
	using fn = int(__fastcall*)(Entity*, void*, float&, vec3_t&, vec3_t&, int&);
	inline fn original = nullptr;
}

namespace ExtraBonesProcessing {
	void __fastcall Hook(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7);
	using fn = void(__fastcall*)(void*, void*, int, int, int, int, int, int);
	inline fn original = nullptr;
}

namespace InterpolateServerEntities {
	void __fastcall Hook();
	using fn = void(__fastcall*)();
	inline fn original = nullptr;
}

namespace DoProceduralFootPlant {
	void __fastcall Hook(void* _this, void* edx, void* boneToWorld, void* pLeftFootChain, void* pRightFootChain, void* pos);
	using fn = void(__fastcall*)(void* _this, void* edx, void* boneToWorld, void* pLeftFootChain, void* pRightFootChain, void* pos);
	inline fn original = nullptr;
}

namespace SendNetMessage {
	bool __fastcall Hook(INetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
	using fn = bool(__thiscall*)(INetChannel* pNetChan, INetMessage& msg, bool bForceReliable, bool bVoice);
	inline fn original;
}

namespace RunCommand {
	void __fastcall Hook(void* ecx, void* edx, Player* player, CUserCmd* ucmd, void* moveHelper);
	using fn = void(__fastcall*)(void*, void*, Player*, CUserCmd*, void*);
	inline fn original = nullptr;
}

namespace WriteUserCmdDeltaToBuffer {
	inline int Charge;
	inline int Shift;
	inline int LastShift;

	bool __fastcall Hook(void* ecx, void* edx, int slot, void* buf, int from, int to, bool is_new_command);
	using fn = bool(__thiscall*)(void*, int, void*, int, int, bool);
	inline fn original = nullptr;
}

namespace SendDataGram {
	int __fastcall Hook(void* ecx, void* edx, void* data);
	using fn = int(__fastcall*)(void*, void*, void*);
	inline fn original = nullptr;
}

namespace ProcessPacket {
	void __fastcall Hook(void* ecx, int edx, void* packet, bool bHasHeader);

	using fn = void(__fastcall*)(void* ecx, int edx, void* packet, bool bHasHeader);
	inline fn original = nullptr;
}

namespace HLTV {
	bool __fastcall Hook(void* ecx, void* edx);
	using fn = bool(__fastcall*)(void* ecx, void* edx);
	inline fn original = nullptr;
}

class Hooks {
public:
	void init();

public:
	// forward declarations
	class IRecipientFilter;

	// prototypes.
	using PaintTraverse_t = void(__thiscall*)(void*, VPANEL, bool, bool);
	using DoPostScreenSpaceEffects_t = bool(__thiscall*)(void*, CViewSetup*);
	using CreateMove_t = bool(__thiscall*)(void*, float, CUserCmd*);
	using LevelInitPostEntity_t = void(__thiscall*)(void*);
	using LevelShutdown_t = void(__thiscall*)(void*);
	using LevelInitPreEntity_t = void(__thiscall*)(void*, const char*);
	using IN_KeyEvent_t = int(__thiscall*)(void*, int, int, const char*);
	using FrameStageNotify_t = void(__thiscall*)(void*, Stage_t);
	using GetActiveWeapon_t = Weapon * (__thiscall*)(void*);
	using CalcViewModelView_t = void(__thiscall*)(void*, vec3_t&, ang_t&);
	using InPrediction_t = bool(__thiscall*)(void*);
	using OverrideView_t = void(__thiscall*)(void*, CViewSetup*);
	using LockCursor_t = void(__thiscall*)(void*);
	//using ProcessPacket_t = void(__thiscall*)(void*, void*, bool);
	using SendDatagram_t = int(__thiscall*)(void*, void*);
	// using CanPacket_t                = bool( __thiscall* )( void* );
	using PlaySound_t = void(__thiscall*)(void*, const char*);
	using GetScreenSize_t = void(__thiscall*)(void*, int&, int&);
	using Push3DView_t = void(__thiscall*)(void*, CViewSetup&, int, void*, void*);
	using SceneEnd_t = void(__thiscall*)(void*);
	using DrawModelExecute_t = void(__thiscall*)(void*, uintptr_t, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using ComputeShadowDepthTextures_t = void(__thiscall*)(void*, const CViewSetup&, bool);
	using GetInt_t = int(__thiscall*)(void*);
	using GetBool_t = bool(__thiscall*)(void*);
	using IsConnected_t = bool(__thiscall*)(void*);
	//using IsHLTV_t = bool(__thiscall*)(void*);
	using OnEntityCreated_t = void(__thiscall*)(void*, Entity*);
	using OnEntityDeleted_t = void(__thiscall*)(void*, Entity*);
	using RenderSmokeOverlay_t = void(__thiscall*)(void*, bool);
	using ShouldDrawFog_t = bool(__thiscall*)(void*);
	using ShouldDrawParticles_t = bool(__thiscall*)(void*);
	using Render2DEffectsPostHUD_t = void(__thiscall*)(void*, const CViewSetup&);
	using OnRenderStart_t = void(__thiscall*)(void*);
	using RenderView_t = void(__thiscall*)(void*, const CViewSetup&, const CViewSetup&, int, int);
	using GetMatchSession_t = CMatchSessionOnlineHost * (__thiscall*)(void*);
	using OnScreenSizeChanged_t = void(__thiscall*)(void*, int, int);
	using OverrideConfig_t = bool(__thiscall*)(void*, MaterialSystem_Config_t*, bool);
	using PostDataUpdate_t = void(__thiscall*)(void*, DataUpdateType_t);
	using TempEntities_t = bool(__thiscall*)(void*, void*);
	using EmitSound_t = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const vec3_t*, const vec3_t*, void*, bool, float, int);
	using PacketStart_t = void(__thiscall*)(void*, int, int);
	// using PreDataUpdate_t            = void( __thiscall* )( void*, DataUpdateType_t );
	using VoiceData_t = void(__thiscall*)(void*, void*);

public:
	void                     hkVoiceData(void* msg);
	void                     PacketStart(int incoming_sequence, int outgoing_acknowledged);
	bool                     TempEntities(void* msg);
	void                     PaintTraverse(VPANEL panel, bool repaint, bool force);
	bool                     DoPostScreenSpaceEffects(CViewSetup* setup);
	void                     LevelInitPostEntity();
	void                     LevelShutdown();
	//int                      IN_KeyEvent( int event, int key, const char* bind );
	void                     LevelInitPreEntity(const char* map);
	void                     FrameStageNotify(Stage_t stage);
	Weapon*                  GetActiveWeapon();
	bool                     InPrediction();
	bool                     ShouldDrawParticles();
	bool                     ShouldDrawFog();
	void                     OverrideView(CViewSetup* view);
	void                     LockCursor();
	void                     PlaySound(const char* name);
	void                     OnScreenSizeChanged(int oldwidth, int oldheight);
	//void                     ProcessPacket(void* packet, bool header);
	//void                     GetScreenSize( int& w, int& h );
	void                     SceneEnd();
	void                     DrawModelExecute( uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone );
	void                     ComputeShadowDepthTextures(const CViewSetup& view, bool unk);
	int                      DebugSpreadGetInt();
	int                      JiggleBonesGetInt();
	int                      ExtrapolateGetInt();
	bool                     NetShowFragmentsGetBool();
	bool                     IsConnected();
	//bool                     IsHLTV();
	void                     EmitSound(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
	void                     RenderSmokeOverlay(bool unk);
	void                     OnRenderStart();
	void                     RenderView(const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw);
	void                     Render2DEffectsPostHUD(const CViewSetup& setup);
	CMatchSessionOnlineHost* GetMatchSession();
	bool                     OverrideConfig(MaterialSystem_Config_t* config, bool update);
	void                     PostDataUpdate(DataUpdateType_t type);

	static LRESULT WINAPI WndProc(HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp);

public:
	// vmts.
	VMT m_panel;
	VMT m_client_mode;
	VMT m_client;
	VMT m_client_state;
	VMT m_engine;
	VMT m_engine_sound;
	VMT m_prediction;
	VMT m_surface;
	VMT m_render;
	VMT m_net_channel;
	VMT m_render_view;
	VMT m_model_render;
	VMT m_shadow_mgr;
	VMT m_view_render;
	VMT m_match_framework;
	VMT m_material_system;
	VMT m_fire_bullets;
	VMT m_net_show_fragments;
	VMT m_jiggle_bones;
	VMT m_extrapolate;

	// player shit.
	std::array< VMT, 64 > m_player;

	// cvars
	VMT m_debug_spread;

	// wndproc old ptr.
	WNDPROC m_old_wndproc;

	// old player create fn.
	GetActiveWeapon_t           m_GetActiveWeapon;

	// netvar proxies.
	RecvVarProxy_t m_Pitch_original;
	RecvVarProxy_t m_Body_original;
	RecvVarProxy_t m_Force_original;
	RecvVarProxy_t m_AbsYaw_original;
};

// note - dex; these are defined in player.cpp.
class CustomEntityListener : public IEntityListener {
public:
	virtual void OnEntityCreated(Entity* ent);
	virtual void OnEntityDeleted(Entity* ent);

	__forceinline void init() {
		g_csgo.AddListenerEntity(this);
	}
};

extern Hooks                g_hooks;
extern CustomEntityListener g_custom_entity_listener;
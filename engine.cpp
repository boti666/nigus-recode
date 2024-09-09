#include "includes.h"

bool Hooks::IsConnected( ) {
	Stack stack;

	static Address IsLoadoutAllowed{ pattern::find( g_csgo.m_client_dll, XOR( "84 C0 75 04 B0 01 5F" ) ) };

	if( g_menu.main.misc.unlock.get( ) && stack.ReturnAddress( ) == IsLoadoutAllowed )
		return false;

	return g_hooks.m_engine.GetOldMethod< IsConnected_t >( IVEngineClient::ISCONNECTED )( this );
}

bool __fastcall HLTV::Hook(void* ecx, void* edx) {

	static Address SetupVelocity{ pattern::find(g_csgo.m_client_dll, XOR("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80")) };
	static Address AccumulateLayers{ pattern::find(g_csgo.m_client_dll, XOR("84 C0 75 0D F6 87")) };

	if( uintptr_t(_ReturnAddress()) == SetupVelocity || uintptr_t(_ReturnAddress()) == AccumulateLayers )
		return true;

	return original(ecx, edx);
}

void Hooks::EmitSound( IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity ) {
	g_hooks.m_engine_sound.GetOldMethod<EmitSound_t>( IEngineSound::EMITSOUND )( this, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity );
}
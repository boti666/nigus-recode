#include "includes.h"

int Hooks::DebugSpreadGetInt( ) {
	return g_hooks.m_debug_spread.GetOldMethod< GetInt_t >( ConVar::GETINT )( this );
}

int Hooks::JiggleBonesGetInt() {
	return 0;
}

int Hooks::ExtrapolateGetInt() {
	return 0;
}

bool Hooks::NetShowFragmentsGetBool( ) {
	return g_hooks.m_net_show_fragments.GetOldMethod< GetBool_t >( ConVar::GETBOOL )( this );
}
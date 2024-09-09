#include "includes.h"

bool Hooks::TempEntities( void *msg ) {
	if( !g_cl.m_processing ) {
		return g_hooks.m_client_state.GetOldMethod< TempEntities_t >( CClientState::TEMPENTITIES )( this, msg );
	}

	const bool ret = g_hooks.m_client_state.GetOldMethod< TempEntities_t >( CClientState::TEMPENTITIES )( this, msg );

	CEventInfo *ei = g_csgo.m_cl->m_events; 
	CEventInfo *next = nullptr;

	if( !ei ) {
		return ret;
	}

	do {
		next = *reinterpret_cast< CEventInfo ** >( reinterpret_cast< uintptr_t >( ei ) + 0x38 );

		uint16_t classID = ei->m_class_id - 1;

		auto m_pCreateEventFn = ei->m_client_class->m_pCreateEvent;
		if( !m_pCreateEventFn ) {
			continue;
		}

		void *pCE = m_pCreateEventFn( );
		if( !pCE ) {
			continue;
		}

		if( classID == 170 ){
			ei->m_fire_delay = 0.0f;
		}
		ei = next;
	} while( next != nullptr );

	return ret;
}

bool AllowPacket(int command) {
	if (!g_cl.m_processing || !g_csgo.m_engine->IsInGame() || g_cl.m_sent_cmds.empty())
		return true;

	for (int i = 0; i < g_cl.m_sent_cmds.size(); ++i) {
		int cmd = g_cl.m_sent_cmds.at(i);
		if (!cmd || cmd != command)
			continue;

		g_cl.m_sent_cmds.erase(g_cl.m_sent_cmds.begin() + i);
		return true;
	}

	return false;
}

void Hooks::PacketStart(int incoming_sequence, int outgoing_acknowledged) {
	if (AllowPacket(outgoing_acknowledged))
		return g_hooks.m_client_state.GetOldMethod< PacketStart_t >(CClientState::PACKETSTART)(this, incoming_sequence, outgoing_acknowledged);
}
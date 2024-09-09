#include "includes.h"

#define NET_FRAMES_BACKUP 64 // must be power of 2. 
#define NET_FRAMES_MASK ( NET_FRAMES_BACKUP - 1 )

int __fastcall SendDataGram::Hook(void* ecx, void* edx, void* data) {
	if (!ecx || !g_csgo.m_engine->IsInGame() || !g_csgo.m_net
		|| !g_cl.m_local || !g_cl.m_local->alive() || (INetChannel*)ecx != g_csgo.m_cl->m_net_channel
		|| !g_cl.m_processing)
		return original(ecx, edx, data);

	bool enabled = g_menu.main.misc.force_fake_latency.get() || g_aimbot.m_fake_latency;

	if (!enabled)
		return original(ecx, edx, data);

	const auto net_chan = (INetChannel*)ecx;

	const auto backup_in_seq = net_chan->m_in_seq;
	const auto backup_in_rel_state = net_chan->m_in_rel_state;

	auto target_ping = (g_aimbot.m_fake_latency ? g_menu.main.misc.fake_latency_amt.get() : g_menu.main.misc.forced_fake_latency_amt.get()) / 1000.f;

	if (g_cl.m_latency < target_ping) {
		auto target_in_seq = net_chan->m_in_seq - game::TIME_TO_TICKS(target_ping - g_cl.m_latency);
		net_chan->m_in_seq = target_in_seq;

		for (auto& seq : g_cl.m_sequences) {
			if (seq.m_seq != target_in_seq)
				continue;

			net_chan->m_in_rel_state = seq.m_state;
		}
	}

	int ret = original(ecx, edx, data);

	net_chan->m_in_seq = backup_in_seq;
	net_chan->m_in_rel_state = backup_in_rel_state;
	return ret;
}

void __fastcall ProcessPacket::Hook( void* ecx, int edx, void* packet, bool bHasHeader ) {
	original(ecx, edx, packet, bHasHeader);

	g_cl.UpdateIncomingSequences( );

	// get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll
	for( CEventInfo* it{ g_csgo.m_cl->m_events }; it != nullptr; it = it->m_next ) {
		if( !it->m_class_id )
			continue;

		// set all delays to instant.
		it->m_fire_delay = 0.f;
	}

	// game events are actually fired in OnRenderStart which is WAY later after they are received
	// effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay).
	g_csgo.m_engine->FireEvents( );
}
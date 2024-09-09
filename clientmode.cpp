#include "includes.h"

bool Hooks::ShouldDrawParticles() {
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawParticles_t >(IClientMode::SHOULDDRAWPARTICLES)(this);
}

bool Hooks::ShouldDrawFog() {
	// remove fog.
	if (g_menu.main.visuals.nofog.get())
		return false;

	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawFog_t >(IClientMode::SHOULDDRAWFOG)(this);
}

void Hooks::OverrideView(CViewSetup* view) {
	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	// g_grenades.think( );
	g_visuals.ThirdpersonThink();

	// call original.
	g_hooks.m_client_mode.GetOldMethod< OverrideView_t >(IClientMode::OVERRIDEVIEW)(this, view);
}

void __stdcall CreateMove::CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket) {
	original(g_csgo.m_client, 0, sequence_number, input_sample_frametime, active);

	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	if (!g_cl.m_local) {
		g_cl.m_server_data.reset();
		return;
	}

	g_cl.m_processing = g_cl.m_local->alive();

	if (!g_cl.m_processing) {
		g_cl.m_server_data.reset();
		return;
	}

	auto cmd = g_csgo.m_input->GetUserCmd(sequence_number);
	auto ver_cmd = g_csgo.m_input->GetVerifiedUserCmd(sequence_number);

	// called from CInput::ExtraMouseSample -> return original.
	if (!cmd || !cmd->m_command_number || !ver_cmd)
		return;

	g_cl.m_packet = &bSendPacket;

	// invoke move function.
	g_cl.OnTick(cmd);

	ver_cmd->m_cmd = *cmd;
	ver_cmd->m_crc = cmd->GetChecksum();

	if (*g_cl.m_packet) {
		g_cl.m_sent_cmds.push_front(cmd->m_command_number);
	}
	else {
		INetChannel* nci = g_csgo.m_cl->m_net_channel;
		if (nci) {
			const int choke = nci->m_choked_packets;

			nci->m_choked_packets = 0;
			nci->SendDatagram(nullptr);
			--nci->m_out_seq;

			nci->m_choked_packets = choke;
		}
	}
}

__declspec(naked) void __fastcall CreateMove::Hook(void* _this, int, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call CreateMove::CreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

bool Hooks::DoPostScreenSpaceEffects(CViewSetup* setup) {
	g_visuals.RenderGlow();

	return g_hooks.m_client_mode.GetOldMethod< DoPostScreenSpaceEffects_t >(IClientMode::DOPOSTSPACESCREENEFFECTS)(this, setup);
}
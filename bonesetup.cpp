#include "includes.h"

Bones g_bones{};;

void Bones::AttachmentHelper(Player* player, CStudioHdr* studio_hdr) {
	const bool backup = player->m_bClientSideAnimation();

	player->m_bClientSideAnimation() = true;

	using AttachmentHelperFn = void(__thiscall*)(Entity*, CStudioHdr*);
	g_csgo.AttachmentHelper.as< AttachmentHelperFn  >()(player, studio_hdr);

	player->m_bClientSideAnimation() = backup;
}

bool Bones::BuildBones(Player* target, int mask, BoneArray* out, bool rebuilt, bool interpolate) {
	if (!target || !out)
		return false;

	CStudioHdr* studio_hdr = target->GetModelPtr();
	if (!studio_hdr)
		return false;

	bool ret = false;

	const bool in_prediction = g_csgo.m_prediction->m_in_prediction;
	const int effects = target->m_fEffects();
	const int eflags = target->m_iEFlags();

	CCSGOPlayerAnimState* animstate = target->m_PlayerAnimState();
	if (animstate)
		animstate->m_weapon_last = animstate->m_weapon;

	Weapon* previous_weapon = animstate ? animstate->m_weapon_last : nullptr;

	for (int i = 0; i <= 13;) {
		auto& elem = target->m_AnimOverlay()[i];

		if (target != elem.m_owner)
			elem.m_owner = target;

		i++;
	}

	g_csgo.m_prediction->m_in_prediction = false;

	mask |= BONE_ALWAYS_SETUP;
	mask |= BONE_USED_BY_ATTACHMENT;

	target->InvalidateBoneCache();

	if (interpolate)
		target->m_fEffects() &= ~EF_NOINTERP;
	else
		target->m_fEffects() |= EF_NOINTERP;

	target->m_iEFlags() |= EFL_SETTING_UP_BONES;

	ret = rebuilt ? RebuiltSetupBones(target, mask, out) : SetupBones::original(target->renderable(), nullptr, out, 128, mask, g_csgo.m_globals->m_curtime);

	AttachmentHelper(target, studio_hdr);

	target->m_fEffects() = effects;
	target->m_iEFlags() = eflags;
	g_csgo.m_prediction->m_in_prediction = in_prediction;

	if (animstate)
		animstate->m_weapon_last = previous_weapon;

	return ret;
}



bool Bones::RebuiltSetupBones(Player* target, int mask, BoneArray* out) {
	vec3_t		     pos[128];
	quaternion_t     q[128];

	// get hdr.
	CStudioHdr* hdr = target->GetModelPtr();
	if (!hdr)
		return false;

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &target->m_BoneAccessor();
	if (!accessor)
		return false;

	// store origial output matrix.
	// likely cachedbonedata.
	BoneArray* backup_matrix = accessor->m_pBones;
	if (!backup_matrix)
		return false;

	// compute transform from raw data.
	matrix3x4_t transform;
	g_csgo.AngleMatrix(target->GetAbsAngles(), transform);
	transform.SetOrigin(target->GetAbsOrigin());

	// set bone array for write.
	accessor->m_pBones = out;

	// compute and build bones.
	target->StandardBlendingRules(hdr, pos, q, g_csgo.m_globals->m_curtime, mask);

	uint8_t computed[0x100];
	std::memset(computed, 0, 0x100);
	target->BuildTransformations(hdr, pos, q, transform, mask, computed);

	// restore old matrix.
	accessor->m_pBones = backup_matrix;

	return true;
}
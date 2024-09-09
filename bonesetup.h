#pragma once

class Bones {
public:
	void AttachmentHelper(Player* player, CStudioHdr* studio_hdr);
	bool BuildBones(Player* target, int mask, BoneArray* out, bool rebuilt, bool interpolate = false);
	bool BuildLocalBones(Player* player, int mask, BoneArray* out, float time);
	bool RebuiltSetupBones(Player* target, int mask, BoneArray* out);
};

extern Bones g_bones;
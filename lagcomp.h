#pragma once
#include "csgo.h"
#include "lagrecord.h"
#include "angle.h"

class AimPlayer;

class LagCompensation {
public:
	enum LagType : size_t {
		INVALID = 0,
		CONSTANT,
		ADAPTIVE,
		RANDOM,
	};

public:
	LagRecord* StartPrediction(AimPlayer* player, LagRecord* pred_record);
	void PlayerMove(Player* target, vec3_t& pred_origin, vec3_t& pred_vel, int& pred_flags);
};

extern LagCompensation g_lagcomp;
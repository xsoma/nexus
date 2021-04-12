#pragma once
#include <deque>
#include "../../includes.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/CBaseAnimState.h"
#include "../../UTILS/vector3D.h"
#include "../../UTILS/interfaces.h"
#include "../../SDK/CGlobalVars.h"
#include "../../UTILS/variables.h"
#include "../../SDK/NetChannel.h"
#include "AutoWall.h"

class Resolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];

	float pitchHit[65];

	void OnCreateMove();
	void Yaw(SDK::CBaseEntity* ent);
	void FrameStage(ClientFrameStage_t stage);
private:
	void AnimationFix(SDK::CBaseEntity* pEnt);
};
extern Resolver g_Resolver;
#include "Resolver.h"
Resolver g_Resolver;

void Resolver::AnimationFix(SDK::CBaseEntity* pEnt)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (pEnt == local_player) {
		pEnt->GetClientSideAnimation();

		auto player_animation_state = pEnt->GetAnimState();

		player_animation_state->m_flLeanAmount = 20;
		player_animation_state->m_flCurrentTorsoYaw += 15;
		pEnt->UpdateClientSideAnimation();
		pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
		pEnt->GetClientSideAnimation();
	}
	else {
		auto player_index = pEnt->GetIndex() - 1;

		pEnt->GetClientSideAnimation();

		auto old_curtime = INTERFACES::Globals->curtime;
		auto old_frametime = INTERFACES::Globals->frametime;

		INTERFACES::Globals->curtime = pEnt->GetSimTime();
		INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;

		auto player_animation_state = pEnt->GetAnimState();
		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
		if (player_animation_state != nullptr && player_model_time != nullptr)
			if (*player_model_time == INTERFACES::Globals->framecount)
				*player_model_time = INTERFACES::Globals->framecount - 1;


		pEnt->UpdateClientSideAnimation();

		INTERFACES::Globals->curtime = old_curtime;
		INTERFACES::Globals->frametime = old_frametime;

		//pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));

		pEnt->GetClientSideAnimation();
	}

}
float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
float ApproachAngle(float target, float value, float speed)
{
	target = flAngleMod(target);
	value = flAngleMod(value);

	float delta = target - value;

	// Speed is assumed to be positive
	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

void update_state(SDK::CBaseAnimState* state, Vector angles) {
	using Fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto fn = reinterpret_cast<Fn>(UTILS::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
	fn(state, nullptr, 0.0f, angles[1], angles[0], nullptr);
}

void HandleBackUpResolve(SDK::CBaseEntity* pEnt) {

	if (!&SETTINGS::settings.resolver)
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (pEnt->GetTeam() == local_player->GetTeam())
		return;

	const auto player_animation_state = pEnt->GetAnimState();

	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v48 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	player_animation_state->m_flGoalFeetYaw = v136;
}

void HandleHits(SDK::CBaseEntity* pEnt)
{
	auto NetChannel = INTERFACES::Engine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (GLOBAL::Shot[pEnt->GetIndex()])
	{
		if (init[pEnt->GetIndex()])
		{
			g_Resolver.pitchHit[pEnt->GetIndex()] = pEnt->GetEyeAngles().x;
			auto net_channel = INTERFACES::Engine->GetNetChannel();
			predTime[pEnt->GetIndex()] = INTERFACES::Globals->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(net_channel->m_nChokedPackets);
			init[pEnt->GetIndex()] = false;
		}

		if (INTERFACES::Globals->curtime > predTime[pEnt->GetIndex()] && !GLOBAL::Hit[pEnt->GetIndex()])
		{
			GLOBAL::MissedShots[pEnt->GetIndex()] += 1;
			GLOBAL::Shot[pEnt->GetIndex()] = false;
		}
		else if (INTERFACES::Globals->curtime <= predTime[pEnt->GetIndex()] && GLOBAL::Hit[pEnt->GetIndex()])
			GLOBAL::Shot[pEnt->GetIndex()] = false;

	}
	else
		init[pEnt->GetIndex()] = true;

	GLOBAL::Hit[pEnt->GetIndex()] = false;
}

void Resolver::OnCreateMove() // cancer v2
{
	if (!SETTINGS::settings.resolver)
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player->IsAlive())
		return;

	for (int i = 1; i < INTERFACES::Engine->GetMaxClients(); ++i)
	{
		SDK::CBaseEntity* pPlayerEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->GetIsDormant()
			|| pPlayerEntity == local_player
			|| pPlayerEntity->GetTeam() == local_player->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->GetIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->GetIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = UTILS::CalcAngle(local_player->GetAbsOrigin(), pPlayerEntity->GetAbsOrigin()).y;
			Vector ViewPoint = local_player->GetAbsOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(UTILS::GRD_TO_BOG(angToLocal))),(45 * cos(UTILS::GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(UTILS::GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(UTILS::GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(UTILS::GRD_TO_BOG(angToLocal))),(50 * cos(UTILS::GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(UTILS::GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(UTILS::GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetAbsOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (AutoWall->CanHit(ViewPoint, OriginAutowall)) // CanHitFloatingPoint
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->GetIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->GetIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (AutoWall->CanHit(OriginAutowall2, OriginAutowall222))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->GetIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->GetIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)
					UseFreestandAngle[pPlayerEntity->GetIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->GetIndex()] = true;
			}
		}
	}
}

void Resolver::FrameStage(ClientFrameStage_t stage)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || !INTERFACES::Engine->IsInGame())
		return;

	static bool  wasDormant[65];

	for (int i = 1; i < INTERFACES::Engine->GetMaxClients(); ++i)
	{
		SDK::CBaseEntity* pPlayerEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->GetIsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			HandleHits(pPlayerEntity);
			AnimationFix(pPlayerEntity);


		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			HandleBackUpResolve(pPlayerEntity);
		}

		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != local_player)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}
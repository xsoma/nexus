#include "../../includes.h"
#include "../../SDK/CUserCmd.h"
#include "fake_walk.h"
#include <time.h>
#include <iostream>
#include "../../SDK/CClientEntityList.h"
#include "../../SDK/IEngine.h"
#define FastSqrt(x)	(sqrt)(x)
#define Square(x) ((x)*(x))
void c_fake_walk::do_fake_walk(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (!SETTINGS::settings.slow_walk || !GetAsyncKeyState(SETTINGS::settings.slow_walk_key))
		return;

	auto weapon_handle = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon_handle)
		return;

	float get_speed = 40.f;

	if (get_speed <= 0.f)
		return;

	float min_speed = (float)(FastSqrt(Square(cmd->forwardmove) + Square(cmd->sidemove) + Square(cmd->upmove)));
	if (min_speed <= 0.f)
		return;

	if (cmd->buttons & IN_DUCK)
		get_speed *= 2.94117647f;

	if (min_speed <= get_speed)
		return;

	float kys = get_speed / min_speed;

	cmd->forwardmove *= kys;
	cmd->sidemove *= kys;
	cmd->upmove *= kys;
}

c_fake_walk* fakewalk = new c_fake_walk();

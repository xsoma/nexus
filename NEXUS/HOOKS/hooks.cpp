#include "..\includes.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/dx9/imgui_impl_dx9.h"
#include "hooks.h"
#include "../UTILS/interfaces.h"
#include "../UTILS/offsets.h"
#include "../UTILS/NetvarHookManager.h"
#include "../FEATURES/rage/bt.h"
#include "../UTILS/render.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../FEATURES/misc/back_drop.h"
#include "../SDK/CPanel.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../FEATURES/configs/config.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CPrediction.h"
#include "../FEATURES/Visuals/Hitmarkers.h"
#include "../FEATURES/misc/Movement.h"
#include "../FEATURES/Visuals/Visuals.h"

#include "../FEATURES/rage/AntiAim.h"
#include "../FEATURES/rage/Resolver.h"
#include "../FEATURES/rage/Aimbot.h"
#include "../FEATURES/rage/Backtracking.h"
#include "../FEATURES/rage/fake_walk.h"
#include "../FEATURES/rage/FakeLag.h"
#include "../FEATURES/rage/EnginePred.h"
#include "../FEATURES/misc/EventListener.h"
#include "../FEATURES/misc/GrenadePrediction.h"
#include "../FEATURES/Changers/GloveChanger.h"
#include "../FEATURES/Changers/SkinChanger.h"
#include "../xor.h"
#include "../shit.h"
#include <chrono>
#include <d3dx9.h>
#include <ShlObj_core.h>
#include <intrin.h>
#include "../FEATURES/menu/menu.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

CEventlog* EventLog = new CEventlog();
void animation_fix(SDK::CBaseEntity* entity, ClientFrameStage_t stage)
{
	bool real_server_update = false;
	bool fake_server_update = false;

	float real_simulation_time = 0.0f;
	float fake_simulation_time = 0.0f;
	if (!entity)
		return;

	if (stage == FRAME_NET_UPDATE_END)
	{
		fake_server_update = false;

		if (entity->GetSimTime() != fake_simulation_time) //-V550 //-V807
		{
			fake_server_update = true;
			fake_simulation_time = entity->GetSimTime();
		}

		entity->GetAnimOverlays()[3].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[3].m_flCycle = 0.0f;
		entity->GetAnimOverlays()[6].m_flPlaybackRate = 0.0f;
		entity->GetAnimOverlays()[12].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[12].m_flCycle = 0.0f;

		//update_fake_animations();
	}
	else if (stage == FRAME_RENDER_START)
	{
		auto animstate = entity->GetAnimState(); //-V807

		if (!animstate)
			return;

		real_server_update = false;

		if (entity->GetSimTime() != real_simulation_time) //-V550
		{
			real_server_update = true;
			real_simulation_time = entity->GetSimTime();
		}

		entity->GetAnimOverlays()[3].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[3].m_flCycle = 0.0f;
		entity->GetAnimOverlays()[6].m_flPlaybackRate = 0.0f;
		entity->GetAnimOverlays()[12].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[12].m_flCycle = 0.0f;

		//update_local_animations(animstate);
	}
	else if (stage == FRAME_RENDER_START)
	{
		auto animstate = entity->GetAnimState(); //-V807

		if (!animstate)
			return;

		real_server_update = false;
		fake_server_update = false;

		if (entity->GetSimTime() != real_simulation_time || entity->GetSimTime() != fake_simulation_time) //-V550
		{
			real_server_update = fake_server_update = true;
			real_simulation_time = fake_simulation_time = entity->GetSimTime();
		}

		entity->GetAnimOverlays()[3].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[3].m_flCycle = 0.0f;
		entity->GetAnimOverlays()[6].m_flPlaybackRate = 0.0f;
		entity->GetAnimOverlays()[12].m_flWeight = 0.0f;
		entity->GetAnimOverlays()[12].m_flCycle = 0.0f;
	}
}

void WaterMark()
{
	int w, h;
	INTERFACES::Engine->GetScreenSize(w, h);
	RENDER::DrawFilledRect(w - 162, 5, 182 + w - 187, 20, CColor(26, 28, 33, 180));
	RENDER::DrawFilledRect(w - 162, 4, 182 + w - 187, 6, CColor(255, 50, 50));
#if _DEBUG
#define VERSION XorStr("Nexus [debug] | ")
#else
#define VERSION XorStr("Nexus [alpha] | ")
#endif

	std::string username = XorStr("Redgy");
	std::string watermark = VERSION + username + XorStr(" | ") + UTILS::currentDateTime();
	RENDER::Text(w - 167 + 7, 6, CColor(255, 255, 255, 255), FONTS::font_watermark, watermark.c_str());
}

static int ground_tick;

IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;

void SaveState(IDirect3DDevice9 * pDevice)
{
	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->GetVertexDeclaration(&vertDec);
	pDevice->GetVertexShader(&vertShader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
}

void RestoreState(IDirect3DDevice9 * pDevice) // not restoring everything. Because its not needed.
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	pDevice->SetVertexDeclaration(vertDec);
	pDevice->SetVertexShader(vertShader);
}

namespace INIT
{
	HMODULE Dll;
	HWND Window;
	WNDPROC OldWindow;
}
static bool d3d_init = false;
bool PressedKeys[256] = {};
const char* merixids[] =
{
	"1","2","3","4","5","6", "7", "8", "9",
	"Q","W","E","R","T","Y","U","I","O","P",
	"A","S","D","F","G","H","J","K","L",
	"Z","X","C","V","B","N","M",".","\\","|", "/","}","{","[","]",
	"<",">","?","'"
};
static char ConfigNamexd[64] = { 0 };
namespace ImGui
{

	static auto vector_getterxd = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArrayxd(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getterxd,
			static_cast<void*>(&values), values.size());
	}
}
ImFont* bigmenu_font;
ImFont* menu_font;
ImFont* smallmenu_font;
//--- Other Globally Used Variables ---///
static bool tick = false;
Vector vecAimPunch, vecViewPunch;
Vector* pAimPunch = nullptr;
Vector* pViewPunch = nullptr;

//--- Declare Signatures and Patterns Here ---///
static auto CAM_THINK = UTILS::FindSignature("client.dll", "85 C0 75 30 38 86");
static auto linegoesthrusmoke = UTILS::FindSignature("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");

//--- Tick Counting ---//
void ground_ticks()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetFlags() & FL_ONGROUND)
		ground_tick++;
	else
		ground_tick = 0;
}

namespace HOOKS
{
	CreateMoveFn original_create_move;
	PaintTraverseFn original_paint_traverse;
	PaintFn original_paint;
	FrameStageNotifyFn original_frame_stage_notify;
	DrawModelExecuteFn original_draw_model_execute;
	SceneEndFn original_scene_end;
	TraceRayFn original_trace_ray;
	OverrideViewFn original_override_view;
	RenderViewFn original_render_view;
	SvCheatsGetBoolFn original_get_bool;
	GetViewmodelFOVFn original_viewmodel_fov;

	vfunc_hook fireevent;
	vfunc_hook directz;

	VMT::VMTHookManager iclient_hook_manager;
	VMT::VMTHookManager panel_hook_manager;
	VMT::VMTHookManager paint_hook_manager;
	VMT::VMTHookManager model_render_hook_manager;
	VMT::VMTHookManager scene_end_hook_manager;
	VMT::VMTHookManager render_view_hook_manager;
	VMT::VMTHookManager trace_hook_manager;
	VMT::VMTHookManager net_channel_hook_manager;
	VMT::VMTHookManager override_view_hook_manager;
	VMT::VMTHookManager input_table_manager;
	VMT::VMTHookManager get_bool_manager;
	std::string sPanel = ("FocusOverlayPanel");

	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	
	bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd)
	{ 
		if (!cmd || cmd->command_number == 0)
			return false;

		uintptr_t* FPointer; __asm { MOV FPointer, EBP }
		byte* SendPacket = (byte*)(*FPointer - 0x1C);
		if (!SendPacket) return false;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return false;
		GLOBAL::cmd = cmd;
		GLOBAL::should_send_packet = *SendPacket;
		GLOBAL::originalCMD = cmd;
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			static SDK::ConVar* impacts = INTERFACES::cvar->FindVar("sv_showimpacts");
			GrenadePrediction::instance().Tick(cmd->buttons);

			fakelag->do_fakelag(cmd);
			g_Resolver.OnCreateMove();
			if (SETTINGS::settings.slow_walk)
				fakewalk->do_fake_walk(cmd);

			if (SETTINGS::settings.remove_duck_cooldown)
				cmd->buttons |= IN_BULLRUSH;


			if (SETTINGS::settings.bullet_impact)
				impacts->SetValue(1);
			else
				impacts->SetValue(0);

			if (SETTINGS::settings.bhop_bool) movement->bunnyhop(cmd);
			if (SETTINGS::settings.duck_bool) movement->duckinair(cmd);
			//if (SETTINGS::settings.autozeus) aimbot->autozeus(cmd);
			if (SETTINGS::settings.misc_clantag) visuals->clan_tag();
			if (SETTINGS::settings.misc_chat_spam) visuals->chat_spam();

			prediction->run_prediction(cmd);
			{

				if (SETTINGS::settings.aim_bool)
				{
					if (SETTINGS::settings.fake_bool)
					{
						if (local_player->IsAlive())
						{
							for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
							{
								auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
								if (!entity || entity == local_player || entity->GetClientClass()->m_ClassID != !entity->IsAlive()) continue;



								float simtime_delta = entity->GetSimTime() - *reinterpret_cast<float*>(uintptr_t(entity) + OFFSETS::m_flSimulationTime) + 0x4;

								int choked_ticks = HOOKS::fine(TIME_TO_TICKS(simtime_delta), 1, 15);
								Vector lastOrig;

								if (lastOrig.Length() != entity->GetVecOrigin().Length())
									lastOrig = entity->GetVecOrigin();

								float delta_distance = (entity->GetVecOrigin() - lastOrig).LengthSqr();
								if (delta_distance > 4096.f)
								{
									Vector velocity_per_tick = entity->GetVelocity() * INTERFACES::Globals->interval_per_tick;
									auto new_origin = entity->GetVecOrigin() + (velocity_per_tick * choked_ticks);
									entity->SetAbsOrigin(new_origin);
								}
							}
						}
					}
					aimbot->run_aimbot(cmd);
					backtracking->backtrack_player(cmd);
				}

				if (SETTINGS::settings.strafe_bool) movement->autostrafer(cmd);

				if (SETTINGS::settings.aim_type == 0 && SETTINGS::settings.aim_bool)
				{
					aimbot->run_aimbot(cmd);
				}	

				if (SETTINGS::settings.aa_bool)
				{
					antiaim->do_antiaim(cmd);
					antiaim->fix_movement(cmd);
				}
			}
			prediction->end_prediction(cmd);


			if (SendPacket)
				GLOBAL::real_angles = cmd->viewangles;
			else
				GLOBAL::fake_angles.y = local_player->GetAnimState()->m_flGoalFeetYaw;
			

		}
		cmd = GLOBAL::cmd;
		*SendPacket = GLOBAL::should_send_packet;
		if (SETTINGS::settings.anti_untrusted)
			UTILS::ClampLemon(cmd->viewangles);
		return false;
	}

	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
	{
		std::string panel_name = INTERFACES::Panel->GetName(VGUIPanel);
		if (panel_name == "HudZoom" && SETTINGS::settings.scope_bool) return;
		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				int W, H, cW, cH;
				INTERFACES::Engine->GetScreenSize(W, H);
				cW = W / 2;
				cH = H / 2;

				int iY = 88;

				int screen_width, screen_height;
				INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

				static bool left = false;
				static bool right = false;
				static bool back = false;
				static bool free = false;

				if (SETTINGS::settings.aa_real_type == 4)
				{

					if (GetAsyncKeyState(SETTINGS::settings.aa_left))
					{
						left = true;
						right = false;
						back = false;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_right))
					{
						left = false;
						right = true;
						back = false;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_back))
					{
						left = false;
						right = false;
						back = true;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_auto))
					{
						left = false;
						right = false;
						back = false;
						free = true;
					}
					else
					{
					}
					{
						if (left)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 50, 50));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 255, 255, 200));
						}
						else if (right)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 50, 50));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 255, 255, 200));
						}
						else if (back)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 50, 50));
						}
					}
				}
				EventLog->Draw();
				GrenadePrediction::instance().Paint();
				pHitmarker->Paint();
				visuals->static_crosshair();
				//daca nu merge circle-u ala, inseamna ca trebuie aici pus.

				if (SETTINGS::settings.esp_bool)
				{
					visuals->Draw();
					visuals->ClientDraw();
				}
			}
			//visuals->LogEvents();
			BackDrop::DrawBackDrop(); 
			WaterMark();
		}

		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);

		const char* pszPanelName = INTERFACES::Panel->GetName(VGUIPanel);

		if (!strstr(pszPanelName, sPanel.data()))
			return;


		INTERFACES::Panel->SetMouseInputEnabled(VGUIPanel, SETTINGS::settings.menu_open);
	}

/*	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
	{
		std::string panel_name = INTERFACES::Panel->GetName(VGUIPanel);
		if (panel_name == "HudZoom" && SETTINGS::settings.scope_bool) return;
		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();
			//if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			//	EventLog->Draw();
			int w, h;
			INTERFACES::Engine->GetScreenSize(w, h);
			RENDER::DrawFilledRect(w - 162, 4, 182 + w - 187, 20, CColor(177, 0, 0, 180));
#if _DEBUG
#define VERSION XorStr("Nexus [debug] | ")
#else
#define VERSION XorStr("Nexus [alpha] | ")
#endif
			std::string username = XorStr("Redgy");
			std::string watermark = VERSION + username + XorStr(" | ") + UTILS::currentDateTime();
			RENDER::DrawF(w - 167 + 7, 5, FONTS::font_watermark, false, false, CColor(255, 255, 255, 255), watermark);

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				int W, H, cW, cH;
				INTERFACES::Engine->GetScreenSize(W, H);
				cW = W / 2;
				cH = H / 2;

				int iY = 88;

				int screen_width, screen_height;
				INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

				static bool left = false;
				static bool right = false;
				static bool back = false;
				static bool free = false;

				if (SETTINGS::settings.aa_real_type == 4)
				{

					if (GetAsyncKeyState(SETTINGS::settings.aa_left))
					{
						left = true;
						right = false;
						back = false;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_right))
					{
						left = false;
						right = true;
						back = false;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_back))
					{
						left = false;
						right = false;
						back = true;
						free = false;
					}
					else
					{
					}
					if (GetAsyncKeyState(SETTINGS::settings.aa_auto))
					{
						left = false;
						right = false;
						back = false;
						free = true;
					}
					else
					{
					}
					{
						if (left)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 50, 50));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 255, 255, 200));
						}
						else if (right)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 50, 50));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 255, 255, 200));
						}
						else if (back)
						{
							RENDER::old_text(cW + 30, cH - 0, L">", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 38, cH + 0, L"<", FONTS::font_indicators, CColor(255, 255, 255, 200));
							RENDER::old_text(cW - 3.5, cH + 30, L"v", FONTS::font_indicators, CColor(255, 50, 50));
						}
					}
				}
				GrenadePrediction::instance().Paint();
				pHitmarker->Paint();
				visuals->static_crosshair();

				//daca nu merge circle-u ala, inseamna ca trebuie aici pus.

				if (SETTINGS::settings.esp_bool)
				{
					visuals->Draw();
					visuals->ClientDraw();
				}
			}
			visuals->LogEvents();
			BackDrop::DrawBackDrop();
		}

		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);

		const char* pszPanelName = INTERFACES::Panel->GetName(VGUIPanel);

		if (!strstr(pszPanelName, sPanel.data()))
			return;


		INTERFACES::Panel->SetMouseInputEnabled(VGUIPanel, SETTINGS::settings.menu_open);
	}
	*/

	void __fastcall HookedFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t stage)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		Vector vecAimPunch, vecViewPunch;
		Vector* pAimPunch = nullptr; Vector* pViewPunch = nullptr;
		GLOBAL::recoilframe = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer())->GetPunchAngles();

		visuals->night_mode();

		switch (stage)
		{
			case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					GloveChanger();
					xdSkinchanger();
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;

						bool is_local_player = entity == local_player;
						bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

						if (is_local_player) continue;
						if (is_teammate) continue;
						if (entity->GetHealth() <= 0) continue;
						if (entity->GetIsDormant()) continue;
						//if (SETTINGS::settings.resolver)
						//	c_override->override_player(entity);
						//g_Resolver.FrameStage(stage);
						if (SETTINGS::settings.resolver)
						{
							int angle1[64];
							int angle2[64];
							int angle3[64];
							static float old_simtime[65];
							if (GLOBAL::should_send_packet)
								angle1[i] = entity->GetEyeAngles().y;
							else
								angle2[i] = entity->GetEyeAngles().y;

							if (angle1[i] != angle2[i])
								using_fake_angles[entity->GetIndex()] = true;
							else
								using_fake_angles[entity->GetIndex()] = false;

							if (using_fake_angles[entity->GetIndex()])
							{
								if (MATH::YawDistance(angle1[i], angle2[i]) < 59)
								{
									resolve_type[i] = 0;
									entity->GetEyeAnglesPtr()->y = entity->GetLowerBodyYaw() - MATH::YawDistance(angle1[i], angle2[i]);
								}
							}
						}
					
					}
				} break;
			case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
				break;
			case FRAME_RENDER_START:

				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;

						bool is_local_player = entity == local_player;
						bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

						if (is_local_player) continue;
						if (is_teammate) continue;
						if (entity->GetHealth() <= 0) continue;
						if (entity->GetIsDormant()) continue;

						g_BacktrackHelper->UpdateBacktrackRecords(entity);

						if (SETTINGS::settings.force_ragdoll)
						{
							bool applied = false;
							if (!applied) {
								auto gamers = INTERFACES::cvar->FindVar("cl_ragdoll_gravity");
								gamers->SetValue(-1000);
								applied = true;
							}
						}
					}
					
					if (in_tp)
					{
						*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(local_player) + 0x31D8) = GLOBAL::real_angles;
					}
					animation_fix(local_player, stage);
					for (int i = 1; i <= 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
						if (!entity) continue;
						if (entity == local_player) continue;

						*(int*)((uintptr_t)entity + 0xA30) = INTERFACES::Globals->framecount;
						*(int*)((uintptr_t)entity + 0xA28) = 0;
					}
				} break;

			case FRAME_NET_UPDATE_START:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					if (SETTINGS::settings.beam_bool)
						visuals->DrawBulletBeams();

				} break;
			case FRAME_NET_UPDATE_END:
				if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
				{
					for (int i = 1; i < 65; i++)
					{
						auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

						if (!entity)
							continue;

						if (!local_player)
							continue;

						bool is_local_player = entity == local_player;
						bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

						if (is_local_player)
							continue;

						if (is_teammate)
							continue;

						if (entity->GetHealth() <= 0)
							continue;

							backtracking->DisableInterpolation(entity);
					}
				}
				break;
		}
		original_frame_stage_notify(ecx, stage);
	}

	void __fastcall HookedDrawModelExecute(void* ecx, void* edx, SDK::IMatRenderContext* context, const SDK::DrawModelState_t& state, const SDK::ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
	{
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			if (!local_player)
				return;

			if (render_info.entity_index == INTERFACES::Engine->GetLocalPlayer() && local_player && in_tp && SETTINGS::settings.esp_bool && local_player->GetIsScoped())
				INTERFACES::RenderView->SetBlend(0.4f);

			if (SETTINGS::settings.no_flash)
			{
				SDK::IMaterial* flash = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);
				SDK::IMaterial* flashWhite = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);

				flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				flashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
			}
			
		}
		original_draw_model_execute(ecx, context, state, render_info, matrix);
	}
	void __fastcall HookedSceneEnd(void* ecx, void* edx)
	{
		original_scene_end(ecx);

		static SDK::IMaterial* material = nullptr;

		material = INTERFACES::MaterialSystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
		material->SetMaterialVarFlag(SDK::MATERIAL_VAR_IGNOREZ, 1);

		if (!material)
			return;

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()) {
			CColor color = CColor(SETTINGS::settings.glow_col[0] * 255, SETTINGS::settings.glow_col[1] * 255, SETTINGS::settings.glow_col[2] * 255, SETTINGS::settings.glow_col[3] * 255), colorTeam = CColor(SETTINGS::settings.teamglow_color[0] * 255, SETTINGS::settings.teamglow_color[1] * 255, SETTINGS::settings.teamglow_color[2] * 255, SETTINGS::settings.teamglow_color[3] * 255), colorlocal = CColor(SETTINGS::settings.glowlocal_col[0] * 255, SETTINGS::settings.glowlocal_col[1] * 255, SETTINGS::settings.glowlocal_col[2] * 255, SETTINGS::settings.glowlocal_col[3] * 255);
			
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			if (!local_player)
				return;

			for (int i = 1; i < 65; i++) {
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);


				if (!entity)
					continue;



				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
				auto ignorezmaterial = SETTINGS::settings.chamstype == 0 ? material : material;
				auto notignorezmaterial = SETTINGS::settings.chamstype == 0 ? material : material;

				if (is_teammate)
					continue;

				if (local_player && SETTINGS::settings.chams_type == 1) {
					INTERFACES::ModelRender->ForcedMaterialOverride(material);
					material->ColorModulate(CColor(255, 255, 255, 255)); //0, 125, 255
					INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.invisible_col[0] * 255, SETTINGS::settings.invisible_col[1] * 255, SETTINGS::settings.invisible_col[2] * 255, SETTINGS::settings.invisible_col[3] * 255)));
					entity->DrawModel(0x1, 255);
					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
				if (SETTINGS::settings.desync_chams) {
					if (entity == local_player)
					{
						Vector fl = local_player->GetAbsAngles2();

						Vector cham_angle = Vector(fl.x, GLOBAL::fake_angles.y, fl.z);

						if (!local_player->IsAlive())
							return;

						Vector OrigAngle = local_player->GetAbsAngles2();
						Vector OrigOrigin = local_player->GetAbsOrigin();

						local_player->SetAbsOrigin(OrigOrigin);
						local_player->SetAbsAngles2(cham_angle);

						INTERFACES::ModelRender->ForcedMaterialOverride(material);
						material->ColorModulate(CColor(255, 255, 255, 255));
						INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.desync_color[0] * 255, SETTINGS::settings.desync_color[1] * 255, SETTINGS::settings.desync_color[2] * 255)));

						local_player->DrawModel(0x1, 255);

						local_player->SetAbsAngles2(OrigAngle);
						local_player->SetAbsOrigin(OrigOrigin);

						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					}
				}

				if (is_local_player)
					continue;

				if (entity && SETTINGS::settings.chams_type == 1)
				{
					INTERFACES::ModelRender->ForcedMaterialOverride(material);
					material->ColorModulate(CColor(255, 255, 255, 255)); //0, 125, 255
					INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.vmodel_col[0] * 255, SETTINGS::settings.vmodel_col[1] * 255, SETTINGS::settings.vmodel_col[2] * 255, SETTINGS::settings.vmodel_col[3] * 255)));
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(material);
					material->ColorModulate(CColor(255, 255, 255, 255)); //0, 125, 255
					INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.invisible_col[0] * 255, SETTINGS::settings.invisible_col[1] * 255, SETTINGS::settings.invisible_col[2] * 255, SETTINGS::settings.invisible_col[3] * 255)));
					entity->DrawModel(0x1, 255);
					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
				else if (entity && SETTINGS::settings.chams_type == 2)
				{
					INTERFACES::ModelRender->ForcedMaterialOverride(material);
					material->ColorModulate(CColor(255, 255, 255, 255)); //0, 125, 255
					INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.vmodel_col[0] * 255, SETTINGS::settings.vmodel_col[1] * 255, SETTINGS::settings.vmodel_col[2] * 255, SETTINGS::settings.vmodel_col[3] * 255)));
					entity->DrawModel(0x1, 255);

					INTERFACES::ModelRender->ForcedMaterialOverride(material);
					material->ColorModulate(CColor(255, 255, 255, 255)); //0, 125, 255
					INTERFACES::RenderView->set_color_modulation((CColor(SETTINGS::settings.invisible_col[0] * 255, SETTINGS::settings.invisible_col[1] * 255, SETTINGS::settings.invisible_col[2] * 255, SETTINGS::settings.invisible_col[3] * 255)));
					entity->DrawModel(0x1, 255);
					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
				}
			}

			for (auto i = 0; i < INTERFACES::GlowObjManager->GetSize(); i++)
			{
				auto &glowObject = INTERFACES::GlowObjManager->m_GlowObjectDefinitions[i];
				auto entity = reinterpret_cast<SDK::CBaseEntity*>(glowObject.m_pEntity);

				if (!entity) continue;
	

				if (glowObject.IsUnused()) continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

				if (is_local_player && in_tp && SETTINGS::settings.glowlocal)
				{
					glowObject.m_nGlowStyle = 0;//0;
					glowObject.m_flRed = colorlocal.RGBA[0] / 255.0f;
					glowObject.m_flGreen = colorlocal.RGBA[1] / 255.0f;
					glowObject.m_flBlue = colorlocal.RGBA[2] / 255.0f;
					glowObject.m_flAlpha = 150.0f / 255.0f;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
					continue;
				}
				else if (!SETTINGS::settings.glowlocal && is_local_player)
					continue;

				if (entity->GetHealth() <= 0) continue;
				if (entity->GetIsDormant())	continue;
				if (entity->GetClientClass()->m_ClassID != 40) continue;

				if (is_teammate && SETTINGS::settings.glowteam)
				{
					glowObject.m_nGlowStyle = 0; //0;
					glowObject.m_flRed = colorTeam.RGBA[0] / 255.0f;
					glowObject.m_flGreen = colorTeam.RGBA[1] / 255.0f;
					glowObject.m_flBlue = colorTeam.RGBA[2] / 255.0f;
					glowObject.m_flAlpha = 150.0f / 255.0f;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
					continue;
				}
				else if (is_teammate && !SETTINGS::settings.glowteam)
					continue;

				if (SETTINGS::settings.glowenable)
				{
					glowObject.m_nGlowStyle = 0;//0;
					glowObject.m_flRed = color.RGBA[0] / 255.0f;
					glowObject.m_flGreen = color.RGBA[1] / 255.0f;
					glowObject.m_flBlue = color.RGBA[2] / 255.0f;
					glowObject.m_flAlpha = 150.0f / 255.0f;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
				}
			}

			if (SETTINGS::settings.smoke_bool)
			{
				std::vector<const char*> vistasmoke_wireframe = { "particle/vistasmokev1/vistasmokev1_smokegrenade" };

				std::vector<const char*> vistasmoke_nodraw =
				{
					"particle/vistasmokev1/vistasmokev1_fire",
					"particle/vistasmokev1/vistasmokev1_emods",
					"particle/vistasmokev1/vistasmokev1_emods_impactdust",
				};

				for (auto mat_s : vistasmoke_wireframe)
				{
					SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, true); //wireframe
				}

				for (auto mat_n : vistasmoke_nodraw)
				{
					SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_n, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				}

				static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
				*(int*)(smokecout) = 0;
			}
		}
	}
	void dothirdperson(SDK::CViewSetup* pSetup) {
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon) return;

		if (local_player->GetIsScoped())
			return;

		if (in_tp && local_player->is_dead())
			in_tp = false;

		if (!SETTINGS::settings.tp_bool)
			pSetup->fov = 110;

		if (SETTINGS::settings.tp_bool && !in_tp)
			pSetup->fov = 110;

		if (SETTINGS::settings.disable_tp_on_nade)
		{
			if (in_tp && weapon->is_grenade())
				in_tp = false;
		}
	}
	void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;

		auto animstate = local_player->GetAnimState();
		if (!animstate) return;

		if (GetAsyncKeyState(SETTINGS::settings.thirdperson_int) & 1)
			in_tp = !in_tp;

		pSetup->angles -= *local_player->GetAimPunchAngle() * 0.9f + *local_player->GetViewPunchAngle();

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::instance().View(pSetup);
			auto GetCorrectDistance = [&local_player](float ideal_distance) -> float //lambda
			{
				Vector inverse_angles;
				INTERFACES::Engine->GetViewAngles(inverse_angles);

				inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

				Vector direction;
				MATH::AngleVectors(inverse_angles, &direction);

				SDK::CTraceWorldOnly filter;
				SDK::trace_t trace;
				SDK::Ray_t ray;

				ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), (local_player->GetVecOrigin() + local_player->GetViewOffset()) + (direction * (ideal_distance + 5.f)));
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				return ideal_distance * trace.flFraction;
			};

			if (SETTINGS::settings.tp_bool && in_tp)
			{
				if (local_player->GetHealth() <= 0)
					local_player->SetObserverMode(5);

				if (!INTERFACES::Input->m_fCameraInThirdPerson)
				{
					INTERFACES::Input->m_fCameraInThirdPerson = true;
					INTERFACES::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, GetCorrectDistance(100));

					Vector camForward;
					MATH::AngleVectors(Vector(INTERFACES::Input->m_vecCameraOffset.x, INTERFACES::Input->m_vecCameraOffset.y, 0), &camForward);
				}
			}
			else
			{
				INTERFACES::Input->m_fCameraInThirdPerson = false;
				INTERFACES::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0);
			}

			auto zoomsensration = INTERFACES::cvar->FindVar("zoom_sensitivity_ratio_mouse");

			if (SETTINGS::settings.fixscopesens)
				zoomsensration->SetValue("0");

			if (SETTINGS::settings.aim_type == 0)
			{
				if (!local_player->GetIsScoped())
					pSetup->fov = SETTINGS::settings.fov_val;
				else if (local_player->GetIsScoped() && SETTINGS::settings.removescoping)
					pSetup->fov = SETTINGS::settings.fov_val;
			}
			else if (!(SETTINGS::settings.aim_type == 0) && !local_player->GetIsScoped())
				pSetup->fov = 90;

			dothirdperson(pSetup);
		}
		original_override_view(ecx, pSetup);
	}
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace)
	{
		original_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			pTrace->surface.flags |= SURF_SKY;
	}
	bool __fastcall HookedGetBool(void* pConVar, void* edx)
	{
		if ((uintptr_t)_ReturnAddress() == CAM_THINK)
			return true;

		return original_get_bool(pConVar);
	}
	float __fastcall GetViewmodelFOV()
	{
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			float player_fov = original_viewmodel_fov();

			if (SETTINGS::settings.esp_bool)
				player_fov = SETTINGS::settings.viewfov_val;

			return player_fov;
		}
	}

	ImFont* tabss;
	ImFont* fDefault;
	void OpenMenu()
	{
		static bool is_down = false;
		static bool is_clicked = false;
		if (GetAsyncKeyState(VK_INSERT))
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(VK_INSERT) && is_down)
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}

		if (is_clicked)
		{
			SETTINGS::settings.menu_open = !SETTINGS::settings.menu_open;

		}
	}




	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			PressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			PressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			PressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			PressedKeys[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			PressedKeys[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			PressedKeys[VK_MBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = true;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = true;
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = false;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = false;
			}
			break;
		}
		case WM_KEYDOWN:
			PressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			PressedKeys[wParam] = false;
			break;
		default: break;
		}

		OpenMenu();

		if (d3d_init && SETTINGS::settings.menu_open && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}

	
	void GUI_Init(IDirect3DDevice9* pDevice)
	{
		ImGui_ImplDX9_Init(INIT::Window, pDevice);
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 14.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255); // Text Color
		style.Colors[ImGuiCol_MenuMain] = ImColor(177, 0, 0, 255);
		style.Colors[ImGuiCol_MenuMain2] = ImColor(177, 0, 0, 255);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(.137f, .137f, .137f, 0.67f); //Menu background color
																			  //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);	
		style.Colors[ImGuiCol_Border] = ImColor(177, 0, 0, 255); //Borders color of the menu
		style.Colors[ImGuiCol_Button] = ImColor(26, 28, 33, 180);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(177, 0, 0, 100); //Buton color when we go with the mouse
		style.Colors[ImGuiCol_ButtonActive] = ImColor(177, 0, 0, 180); //Button color when we press
		style.Colors[ImGuiCol_TitleBg] = ImColor(177, 0, 0, 255); //Title of the menu background color
		style.Colors[ImGuiCol_TitleBgActive] = ImColor(177, 0, 0, 255); //Title of the active menu background color
																		  //style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);

		style.Colors[ImGuiCol_FrameBg] = ImColor(26, 28, 33, 180); //Background color of the checkbox ...
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(177, 0, 0, 100); //Background of the item when hovered
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(177, 0, 0, 255); //Background color of the active item

		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.8f); //Combobox background color
		style.Colors[ImGuiCol_CheckMark] = ImColor(177, 0, 0, 255); //Radiobutton or checkbox mark color

		style.Colors[ImGuiCol_MenuBarBg] = ImColor(177, 0, 0, 255);
		style.Colors[ImGuiCol_ScrollbarBg] = ImColor(177, 0, 0, 255); //Scrollbar background color
		style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(177, 0, 0, 255); //Scrollbar color
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(177, 0, 0, 100); //Scrollbar color when hovered
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(177, 0, 0, 255); //Scrollbar color when grabed
		style.Colors[ImGuiCol_SliderGrab] = ImColor(177, 0, 0, 255); //Slider color 
		style.Colors[ImGuiCol_SliderGrabActive] = ImColor(177, 0, 0, 255);//Slider color when grabed
		style.Colors[ImGuiCol_Header] = ImColor(177, 0, 0, 255);//Color of the selected item in a combolist
		style.Colors[ImGuiCol_HeaderHovered] = ImColor(177, 0, 0, 100);//Color of the hovered item in a combolist
		style.Colors[ImGuiCol_HeaderActive] = ImColor(177, 0, 0, 255);//Color of the selected item in a combolist when pressed


		style.WindowRounding = 0.f;
		style.FramePadding = ImVec2(0, 0);
		style.ScrollbarSize = 6.f;
		style.ScrollbarRounding = 0.f;
		style.GrabMinSize = 6.f;

		bigmenu_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(rawData_compressed_data_base85, 70);
		menu_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(rawData_compressed_data_base85, 18);
		smallmenu_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(smalll_compressed_data_base85, 13);

		d3d_init = true;
	}

	SDK::CBaseWeapon* xd(SDK::CBaseEntity* xz)
	{
		if (!INTERFACES::Engine->IsConnected())
			return nullptr;
		if (!xz->IsAlive())
			return nullptr;

		HANDLE weaponData = *(HANDLE*)((DWORD)xz + OFFSETS::m_hActiveWeapon);
		return (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(weaponData);
	}

	short SafeWeaponID()
	{
		SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!(local_player))
			return 0;

		SDK::CBaseWeapon* WeaponC = xd(local_player);

		if (!(WeaponC))
			return 0;

		return WeaponC->GetItemDefenitionIndex();
	}

	IDirect3DTexture9 *FonBar1 = nullptr;

	long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		static auto ofunc = directz.get_original<EndSceneFn>(42);

		D3DCOLOR rectColor = D3DCOLOR_XRGB(255, 0, 0);
		D3DRECT BarRect = { 1, 1, 1, 1 };
		ImGuiStyle& style = ImGui::GetStyle();
		pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0);

		if (!d3d_init)
			GUI_Init(pDevice);

		static const D3DRENDERSTATETYPE backupStates[] = { D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE };
		static const int size = sizeof(backupStates) / sizeof(DWORD);

		DWORD oldStates[size] = { 0 };

		SaveState(pDevice);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

		ImGui_ImplDX9_NewFrame();

		if (SETTINGS::settings.menu_open)
		{
			pMenu->RenderMenu();
		}

		ImGui::Render();

		RestoreState(pDevice);

		return ofunc(pDevice);
	}

	long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto ofunc = directz.get_original<EndSceneResetFn>(16);

		if (!d3d_init)
			return ofunc(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = ofunc(pDevice, pPresentationParameters);

		ImGui_ImplDX9_CreateDeviceObjects();

		return hr;
	}

	typedef bool(__thiscall *FireEventClientSide)(void*, SDK::IGameEvent*);

	bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, SDK::IGameEvent* pEvent) {

		static auto ofunct = fireevent.get_original<FireEventClientSide>(9);

		if (!pEvent)
			return ofunct(ecx, pEvent);


		/*
		if (g_Options.NewLegitbot.Aimbot.bKillDelay && g_Options.NewLegitbot.Aimbot.Enabled)
		{

		if (strcmp(pEvent->GetName(), "player_death") == 0)
		{
		int attacker = Interfaces.pEngine->GetPlayerForUserID(pEvent->GetInt("attacker"));
		int iLocalPlayer = Interfaces.pEngine->GetLocalPlayer();
		if (attacker == iLocalPlayer)
		{
		G::KillStopDelay = Interfaces.pGlobalVars->curtime + g_Options.NewLegitbot.Aimbot.iKillDelay;
		}
		}
		}
		*/

		return ofunct(ecx, pEvent);
	}




	void InitHooks()
	{
		iclient_hook_manager.Init(INTERFACES::Client);
		original_frame_stage_notify = reinterpret_cast<FrameStageNotifyFn>(iclient_hook_manager.HookFunction<FrameStageNotifyFn>(37, HookedFrameStageNotify));

		panel_hook_manager.Init(INTERFACES::Panel);
		original_paint_traverse = reinterpret_cast<PaintTraverseFn>(panel_hook_manager.HookFunction<PaintTraverseFn>(41, HookedPaintTraverse));

		model_render_hook_manager.Init(INTERFACES::ModelRender);
		original_draw_model_execute = reinterpret_cast<DrawModelExecuteFn>(model_render_hook_manager.HookFunction<DrawModelExecuteFn>(21, HookedDrawModelExecute));

		scene_end_hook_manager.Init(INTERFACES::RenderView);
		original_scene_end = reinterpret_cast<SceneEndFn>(scene_end_hook_manager.HookFunction<SceneEndFn>(9, HookedSceneEnd));

		trace_hook_manager.Init(INTERFACES::Trace);
		original_trace_ray = reinterpret_cast<TraceRayFn>(trace_hook_manager.HookFunction<TraceRayFn>(5, HookedTraceRay));

		override_view_hook_manager.Init(INTERFACES::ClientMode);
		original_override_view = reinterpret_cast<OverrideViewFn>(override_view_hook_manager.HookFunction<OverrideViewFn>(18, HookedOverrideView));
		original_create_move = reinterpret_cast<CreateMoveFn>(override_view_hook_manager.HookFunction<CreateMoveFn>(24, HookedCreateMove));
		original_viewmodel_fov = reinterpret_cast<GetViewmodelFOVFn>(override_view_hook_manager.HookFunction<GetViewmodelFOVFn>(35, GetViewmodelFOV));

		auto sv_cheats = INTERFACES::cvar->FindVar("sv_cheats");
		get_bool_manager = VMT::VMTHookManager(reinterpret_cast<DWORD**>(sv_cheats));
		original_get_bool = reinterpret_cast<SvCheatsGetBoolFn>(get_bool_manager.HookFunction<SvCheatsGetBoolFn>(13, HookedGetBool));

		fireevent.setup(INTERFACES::GameEventManager);
		fireevent.hook_index(9, Hooked_FireEventClientSide);


		while (!(INIT::Window = FindWindowA("Valve001", nullptr)))
			Sleep(100);
		if (INIT::Window)
			INIT::OldWindow = (WNDPROC)SetWindowLongPtr(INIT::Window, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);
		DWORD DeviceStructureAddress = **(DWORD**)(UTILS::FindSignature("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 1);
		if (DeviceStructureAddress) {
			directz.setup((DWORD**)DeviceStructureAddress);
			directz.hook_index(16, Hooked_EndScene_Reset);
			directz.hook_index(42, Hooked_EndScene);

		}
	}
	void EyeAnglesPitchHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

	}
	void EyeAnglesYawHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;
	}
	void InitNetvarHooks()
	{
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitchHook);
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYawHook);
	}
}

#include "..\includes.h"
#include "interfaces.h"
#include "Utils.h"
namespace INTERFACES
{
	SDK::IClient* Client;
	SDK::CInput* Input;
	SDK::CPanel* Panel;
	SDK::EngineVGui* EngineVGui;
	SDK::ISurface* Surface;
	SDK::IEngine* Engine;
	SDK::CClientEntityList* ClientEntityList;
	SDK::CClientEntityListTWO* ClientEntityList2;
	SDK::CGlobalVars* Globals;
	SDK::IVDebugOverlay* DebugOverlay;
	SDK::CTrace* Trace;
	SDK::CModelInfo* ModelInfo;
	SDK::ICVar* cvar;
	SDK::CInputSystem* InputSystem;
	SDK::CModelRender* ModelRender;
	SDK::CRenderView* RenderView;
	SDK::IMaterialSystem* MaterialSystem;
	SDK::IPhysicsProps* PhysicsProps;
	SDK::COverrideView* ClientMode;
	SDK::IGameEventManager* GameEventManager;
	SDK::IViewRenderBeams* ViewRenderBeams;
	SDK::IViewRender* ViewRender;
	SDK::CGlowObjectManager* GlowObjManager;
	SDK::CPrediction* pPrediction;
	SDK::IGameMovement* pMovement;
	SDK::IMoveHelper* pMoveHelper;
	SDK::CClientState* ClientState;
	CUtlVectorSimple *g_ClientSideAnimationList;

	CreateInterfaceFn CaptureFactory(char* FactoryModule)
	{
		CreateInterfaceFn Interface = 0;

		while (!Interface)
		{
			HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

			if (hFactoryModule)
			{
				Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, "CreateInterface"));
			}

			Sleep(100);
		}

		return Interface;
	}

	template<typename T>
	T* CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
	{
		T* dwPointer = nullptr;

		while (!dwPointer)
		{
			dwPointer = (T*)(Interface(InterfaceName, 0));

			Sleep(100);
		}

		return dwPointer;
	}

	typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
	typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
	inline CreateInterfaceFn get_module_factory(HMODULE module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
	}
	auto EnginePointer = get_module_factory(GetModuleHandleW(L"engine.dll"));
	auto PhysPointer = get_module_factory(GetModuleHandleW(L"vphysics.dll"));

	CNetworkStringTableContainer* g_ClientStringTableContainer; //custommodels
	IPhysicsSurfaceProps*  g_Physprops;

	CNetworkStringTableContainer* ClientStringTableContainer()
	{
		if (!g_ClientStringTableContainer)
		{
			g_ClientStringTableContainer = (CNetworkStringTableContainer*)EnginePointer("VEngineClientStringTable001", NULL);
		}
		return g_ClientStringTableContainer;
	}



	IPhysicsSurfaceProps* Physprops()
	{
		if (!g_Physprops)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory("vphysics.dll");
			g_Physprops = CaptureInterface<IPhysicsSurfaceProps>(pfnFactory, "VPhysicsSurfaceProps001");
		}
		return g_Physprops;
	}



	void InitInterfaces()
	{
		Client = reinterpret_cast<SDK::IClient*>(FindInterface("client.dll", "VClient"));
		Input = (SDK::CInput*)*(DWORD*)(UTILS::FindSignature("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
		Panel = reinterpret_cast<SDK::CPanel*>(FindInterface("vgui2.dll", "VGUI_Panel"));
		EngineVGui = reinterpret_cast<SDK::EngineVGui*>(FindInterface("engine.dll", "VEngineVGui001"));
		Surface = reinterpret_cast<SDK::ISurface*>(FindInterface("vguimatsurface", "VGUI_Surface"));
		Engine = reinterpret_cast<SDK::IEngine*>(FindInterface("engine.dll", "VEngineClient"));
		ClientEntityList = reinterpret_cast<SDK::CClientEntityList*>(FindInterface("client.dll", "VClientEntityList"));
		ClientEntityList2 = reinterpret_cast<SDK::CClientEntityListTWO*>(FindInterface("client.dll", "VClientEntityList"));
		DebugOverlay = reinterpret_cast<SDK::IVDebugOverlay*>(FindInterface("engine.dll", "VDebugOverlay"));
		Trace = reinterpret_cast<SDK::CTrace*>(FindInterface("engine.dll", "EngineTraceClient"));
		ModelInfo = reinterpret_cast<SDK::CModelInfo*>(FindInterface("engine.dll", "VModelInfoClient"));
		cvar = reinterpret_cast<SDK::ICVar*>(FindInterface("vstdlib.dll", "VEngineCvar"));
		InputSystem = reinterpret_cast<SDK::CInputSystem*>(FindInterface("inputsystem.dll", "InputSystemVersion"));
		ModelRender = reinterpret_cast<SDK::CModelRender*>(FindInterface("engine.dll", "VEngineModel")); 
		RenderView = reinterpret_cast<SDK::CRenderView*>(FindInterface("engine.dll", "VEngineRenderView"));
		MaterialSystem = reinterpret_cast<SDK::IMaterialSystem*>(FindInterface("materialsystem.dll", "VMaterialSystem"));
		PhysicsProps = reinterpret_cast<SDK::IPhysicsProps*>(FindInterface("vphysics.dll", "VPhysicsSurfaceProps"));
		GameEventManager = reinterpret_cast<SDK::IGameEventManager*>(FindInterfaceEx("engine.dll", "GAMEEVENTSMANAGER002"));
		ViewRender = **reinterpret_cast<SDK::IViewRender***>(UTILS::FindSignature("client.dll", "FF 50 4C 8B 06 8D 4D F4") - 6);
		ViewRenderBeams = *reinterpret_cast<SDK::IViewRenderBeams**>(UTILS::FindSignature("client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08") + 0x1);
		GlowObjManager = *reinterpret_cast<SDK::CGlowObjectManager**>(UTILS::FindSignature("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		pPrediction = reinterpret_cast<SDK::CPrediction*>(FindInterfaceEx("client.dll", "VClientPrediction001"));
		pMovement = reinterpret_cast<SDK::IGameMovement*>(FindInterfaceEx("client.dll", "GameMovement001"));
		pMoveHelper = **reinterpret_cast<SDK::IMoveHelper***>((UTILS::FindSignature("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));
		ClientState = (SDK::CClientState*)(UTILS::FindSignature("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		g_ClientSideAnimationList = (CUtlVectorSimple*)*(DWORD*)(UTILS::FindPatternV2("client.dll", "A1 ? ? ? ? F6 44 F0 04 01 74 0B") + 1);
		Globals = **(SDK::CGlobalVars***)(UTILS::FindPatternV2("client.dll", "A1 ? ? ? ? 5E 8B 40 10") + 1);/***reinterpret_cast<SDK::CGlobalVars***>((*reinterpret_cast< DWORD** >(Client))[0] + 0x1B);*/
		void** pClientTable = *reinterpret_cast<void***>(Client);
		ClientMode = **reinterpret_cast<SDK::COverrideView***>(reinterpret_cast<DWORD>(pClientTable[10]) + 5);
	}

	void* FindInterface(const char* Module, const char* InterfaceName)
	{
		void* Interface = nullptr;
		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(
			GetModuleHandleA(Module), "CreateInterface"));

		char PossibleInterfaceName[1024];
		for (int i = 1; i < 100; i++)
		{
			sprintf(PossibleInterfaceName, "%s0%i", InterfaceName, i);
			Interface = CreateInterface(PossibleInterfaceName, 0);
			if (Interface)
				break;

			sprintf(PossibleInterfaceName, "%s00%i", InterfaceName, i);
			Interface = CreateInterface(PossibleInterfaceName, 0);
			if (Interface)
				break;
		}

		if (!Interface)
			std::cout << InterfaceName << " not found!" << std::endl;
		else
			std::cout << InterfaceName << " 0x" << Interface << std::endl;

		return Interface;
	}
	void* FindInterfaceEx(const char* Module, const char* InterfaceName)
	{
		void* Interface = nullptr;
		auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(
			GetModuleHandleA(Module), "CreateInterface"));

		Interface = CreateInterface(InterfaceName, 0);
		if (!Interface)
			std::cout << InterfaceName << " not found!" << std::endl;

		return Interface;
	}
	void Dump()
	{
		// Ugly macros ugh
#define STRINGIFY_IMPL(s) #s
#define STRINGIFY(s)      STRINGIFY_IMPL(s)
#define PRINT_INTERFACE(name) Utils::ConsolePrint("%-20s: %p\n", STRINGIFY(name), name)

		PRINT_INTERFACE(Client);
		PRINT_INTERFACE(ClientEntityList);
		PRINT_INTERFACE(ClientEntityList2);
		PRINT_INTERFACE(DebugOverlay);
		PRINT_INTERFACE(Engine);
		PRINT_INTERFACE(pMovement);
		PRINT_INTERFACE(pPrediction);
		PRINT_INTERFACE(PhysicsProps);
		PRINT_INTERFACE(MaterialSystem);
		PRINT_INTERFACE(ModelRender);
		PRINT_INTERFACE(InputSystem);
		PRINT_INTERFACE(cvar);
		PRINT_INTERFACE(ModelInfo);
		PRINT_INTERFACE(Trace);
		PRINT_INTERFACE(Panel);
		PRINT_INTERFACE(Surface);
		PRINT_INTERFACE(EngineVGui);
		PRINT_INTERFACE(Input);
	}
}

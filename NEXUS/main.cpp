#include "includes.h"
#include "UTILS/interfaces.h"
#include "HOOKS/hooks.h"
#include "UTILS/offsets.h"
#include "FEATURES/misc/EventListener.h"
#include "SDK/RecvData.h"
#include "SDK/CClientEntityList.h"
#include "FEATURES/visuals/Hitmarkers.h"
#include "SDK/IEngine.h"
#include "FEATURES/changers/recv.h"
#include "HOOKS/anti_leak.h"
#include <chrono>
#include <thread>
#include "UTILS/Utils.h"

bool ErasePEHeader(HMODULE hModule)
{
	hModule = GetModuleHandle(NULL);
	if ((DWORD)hModule == 0) return 0;
	DWORD IMAGE_NT_HEADER = *(int*)((DWORD)hModule + 60);
	for (int i = 0; i < 0x108; i++)
		*(BYTE*)(IMAGE_NT_HEADER + i) = 0;
	for (int i = 0; i < 120; i++)
		*(BYTE*)((DWORD)hModule + i) = 0;
	return 1;
}

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

DWORD WINAPI OnDllAttach(LPVOID base) {
#ifdef _DEBUG
	Utils::AttachConsole();
#endif
	try {
		Utils::ConsolePrint("Initializing...\n");
		INTERFACES::InitInterfaces();
		Utils::ConsolePrint("InitInterfaces Finished.\n");
		OFFSETS::InitOffsets();
		Utils::ConsolePrint("InitOffsets Finished.\n");
		INTERFACES::Dump();
		NetvarHook();
		Utils::ConsolePrint("NetvarHook Finished.\n");
		UTILS::INPUT::input_handler.Init();
		Utils::ConsolePrint("INPUT Finished.\n");
		FONTS::InitFonts();
		Utils::ConsolePrint("InitFonts Finished.\n");
		pHitmarker->Initialize();
		Utils::ConsolePrint("pHitmarker Initialize Finished.\n");
		HOOKS::InitHooks();
		Utils::ConsolePrint("InitHooks Finished.\n");
		HOOKS::InitNetvarHooks();
		Utils::ConsolePrint("InitNetvarHooks Finished.\n");
		CGameEvents::InitializeEventListeners();
		Utils::ConsolePrint("Finished.\n");
	}
	catch (const std::exception& ex) {
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::DetachConsole();

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
	}
}

#include <string>

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Instance);
		CreateThread(nullptr, 0, OnDllAttach, Instance, 0, nullptr);
	default:
		return TRUE;
	}
	return true;
}

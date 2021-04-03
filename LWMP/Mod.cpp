#include "MultiplayerManager.h"
#include "WinSocket.h"
#include "EventHooks.h"

HOOK(void*, __fastcall, GameTick, ASLR(0x4AC3A0), void* This, void* Edx, void* application)
{
    auto* mulMan = csl::fnd::Singleton<app::mp::MultiplayerManager>::GetInstance();
    mulMan->PreUpdate();

    void* result = originalGameTick(This, Edx, application);
    
    mulMan->PostUpdate();

    return result;
}

extern "C" void __declspec(dllexport) __cdecl Init(ModInfo* modInfo)
{
    if (!WinSocket::startup())
    {
        DEBUG_PRINT("Init(): WinSocket::startup() failed\n");
        return;
    }

    INSTALL_HOOK(GameTick);
    app::mp::EventHooks::Init();
	
	app::mp::MultiplayerManager::ms_BaseDir = modInfo->CurrentMod->Path;
    size_t pos = app::mp::MultiplayerManager::ms_BaseDir.find_last_of("\\/");
    if (pos != std::string::npos)
        app::mp::MultiplayerManager::ms_BaseDir.erase(pos + 1);
}
#include "Pch.h"
#include "Messages.h"
#include "MultiplayerManager.h"
#include "MultiplayerService.h"
#undef CreateService

HOOK(void, __fastcall, GameModeStageInitFirstHook, ASLR(0x00916DD0), void* ecx)
{
	originalGameModeStageInitFirstHook(ecx);
	app::GameDocument::GetSingleton()->CreateService<app::mp::MultiplayerService>();
}

namespace app::mp
{
	MultiplayerManager::MultiplayerManager()
	{
        Singleton<fnd::MessageManager>::GetInstance()->Add(this);
		Singleton<fnd::GameServiceTypeRegistry>::GetInstance()->AddService(MultiplayerService::staticClass());
		m_upSession = std::make_unique<Session>();
        bool isHost = true;
        Address address = Address::fromHostName("localhost", 42069);

#ifndef _DEBUG
        INIReader* reader = new INIReader("LWMP.ini");
        if (reader->ParseError() != 0)
            delete reader, reader = new INIReader(ms_BaseDir + "LWMP.ini");

        if (reader->ParseError() != 0)
            MessageBox(NULL, TEXT("Failed to parse LWMP.ini"), NULL, MB_ICONERROR);

        address = Address::fromHostName(reader->Get("Main", "IP", "localhost").c_str(), reader->GetInteger("Main", "Port", 42069));
        isHost = reader->GetBoolean("Main", "IsHost", true);
        delete reader;

        // printf("[LWMP] %d.%d.%d.%d:%d\n", address.numbers[0], address.numbers[1], address.numbers[2], address.numbers[3], address.port);
#else
        const auto result = MessageBoxA(NULL, "Are you the host?", "LWMP", MB_YESNO);
        isHost = result == IDYES;
#endif

        if (isHost)
        {
            m_upSession->openServer(address.port);
        }
        else
        {
            m_upSession->openClient(address);
            m_upSession->requestMessage<MsgHandleConnectRequest>();
        }
	}

	MultiplayerManager::~MultiplayerManager()
	{
        if (m_pMessageManager)
            m_pMessageManager->Remove(this);
	}

    void* MultiplayerManager::MultiplayerManager_init()
    {
        INSTALL_HOOK(GameModeStageInitFirstHook);
        return new(game::GlobalAllocator::GetSingletonAllocator()) MultiplayerManager();
    }

    void MultiplayerManager::MultiplayerManager_destroy(void* instance)
    {
        delete static_cast<BaseObject*>(instance);
    }
	
	DEFINE_SINGLETON(MultiplayerManager);
}
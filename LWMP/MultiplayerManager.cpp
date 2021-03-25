#include "Pch.h"
#include "Messages.h"
#include "MultiplayerManager.h"

#include "MemoryPool.h"
#include "MessageData.h"
#include "MessageRequest.h"
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
        m_spSession = std::make_shared<Session>();
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
            m_spSession->openServer(address.port);
        }
        else
        {
            m_spSession->openClient(address);
            m_spSession->requestMessage<MsgHandleConnectRequest>();
        }
        m_spSession->addListener(*this);
	}

	MultiplayerManager::~MultiplayerManager()
	{
		if (m_spSession->isConnect())
		{
            m_spSession->sendMessage(m_spSession->getPool()->allocate<MsgDisconnect>());
            m_spSession->postUpdate();
		}
	}

	bool MultiplayerManager::OnMessageRequested(const MessageRequest& request)
	{
        if (request.isOfType<MsgHandleConnectRequest>())
        {
            auto connectResponse = m_pOwner->getPool()->allocate<MsgHandleConnectRequest>();
            if (m_PlayerCount != 0)
            {
                connectResponse->reply = MsgHandleConnectRequest::Reply::DENIED;
                connectResponse->playerNum = -1;
            	m_pOwner->sendMessage(connectResponse);
                return true;
            }
        	
            connectResponse->reply = MsgHandleConnectRequest::Reply::ACCEPTED;
            connectResponse->playerNum = ++m_PlayerCount;
            m_pOwner->sendMessage(connectResponse);
            m_pOwner->setRemoteAddress(request.getAddress());
            return true;
        }
		
        return false;
	}

	bool MultiplayerManager::OnMessageReceived(const MessageData& message)
	{
		if (message.isOfType<MsgDisconnect>())
		{
            m_PlayerCount--;
            return true;
		}
		if (message.isOfType<MsgHandleConnectRequest>())
		{
            const auto connectRequest = message.get<MsgHandleConnectRequest>();
            if (connectRequest->reply == MsgHandleConnectRequest::Reply::ACCEPTED)
            {
                m_pOwner->setRemoteAddress(message.getAddress());
                DEBUG_PRINT("Connected to host. :)\n");
            }
			else
            {
                DEBUG_PRINT("Connection denied. :(\n");
            }
			
            return true;
		}
		
        return false;
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
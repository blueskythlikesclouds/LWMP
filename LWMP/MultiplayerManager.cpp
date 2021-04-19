#include "Pch.h"
#include "Messages.h"
#include "MultiplayerManager.h"
#include "MemoryPool.h"
#include "MessageData.h"
#include "MessageRequest.h"
#include "MPUtil.h"
#include "MultiplayerService.h"
#include "MultiplayerSonic.h"

#undef CreateService
#undef SendMessage

HOOK(void, __fastcall, GameModeStageInitFirstHook, ASLR(0x00916DD0), void* ecx)
{
	originalGameModeStageInitFirstHook(ecx);
	app::GameDocument::GetSingleton()->CreateService<app::mp::MultiplayerService>();
}

HOOK(void, __cdecl, RegisterResourceInfosHook, ASLR(0x008F73F0), app::GameDocument* pDocument, void* resInfo, csl::fnd::IAllocator& alloc)
{
	struct ResourceInfo
	{
        void* m_Unk1;
        size_t m_LoadFlags;
	};

	// Force the game to always load super
    static_cast<ResourceInfo*>(resInfo)->m_LoadFlags |= 8;
    originalRegisterResourceInfosHook(pDocument, resInfo, alloc);
}

HOOK(void, __fastcall, GameDocumentAddObject, app::GameDocument::ms_fpAddGameObject, app::GameDocument* pDoc, void* edx, app::GameObject* pObj)
{
    originalGameDocumentAddObject(pDoc, edx, pObj);
    csl::fnd::Singleton<app::mp::MultiplayerManager>::GetInstance()->OnObjectAdded(pObj);
}

namespace app::mp
{
	MultiplayerManager::MultiplayerManager()
	{
        Singleton<fnd::MessageManager>::GetInstance()->Add(this);
		Singleton<fnd::GameServiceTypeRegistry>::GetInstance()->AddService(MultiplayerService::staticClass());
        m_spSession = std::make_shared<Session>();
        Address address = Address::fromHostName("localhost", 42069);

#ifndef _DEBUG
        INIReader* reader = new INIReader("LWMP.ini");
        if (reader->ParseError() != 0)
            delete reader, reader = new INIReader(ms_BaseDir + "LWMP.ini");

        if (reader->ParseError() != 0)
            MessageBox(NULL, TEXT("Failed to parse LWMP.ini"), NULL, MB_ICONERROR);

        address = Address::fromHostName(reader->Get("Main", "IP", "localhost").c_str(), reader->GetInteger("Main", "Port", 42069));
        m_IsHost = reader->GetBoolean("Main", "IsHost", true);
        delete reader;
#else
        const auto result = MessageBoxA(NULL, "Are you the host?", "LWMP", MB_YESNO);
        m_IsHost = result == IDYES;
#endif

        if (m_IsHost)
        {
            m_spSession->openServer(address.port);
        }
        else
        {
            m_spSession->openClient(address);
            m_spSession->requestMessage<MsgHandleConnectRequest>(address);
        }
        m_spSession->addListener(*this);

        xgame::MsgNotifyObjectEvent event{0};
        SendMessage(GetID(), event);
	}

	MultiplayerManager::~MultiplayerManager()
	{
        Singleton<font::FontManager>::GetInstance()->DbgRemoveFontText(m_pDebugFont.get());
		if (m_spSession->isConnect())
		{
            m_spSession->sendMessage(m_spSession->getPool()->allocate<MsgDisconnect>());
            m_spSession->postUpdate();
		}
	}

	bool MultiplayerManager::ProcessMessage(fnd::Message& msg)
	{
        //m_pDebugFont = new font::FontText(Singleton<font::FontManager>::GetInstance()->GetDbgFont(), GetAllocator());
        //const csl::math::Vector2 textPos{ 20, 10 };
        //m_pDebugFont->AddString(textPos, "[LWMP] Init");
        //Singleton<font::FontManager>::GetInstance()->DbgAddFontText(m_pDebugFont.get());
		
        return true;
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
            	m_pOwner->sendMessage(connectResponse, request.getAddress());
                return true;
            }
        	
            connectResponse->reply = MsgHandleConnectRequest::Reply::ACCEPTED;
            connectResponse->playerNum = ++m_PlayerCount;
            m_pOwner->setRemoteAddress(request.getAddress());
            m_pOwner->sendMessage(connectResponse);
            m_ConnectedCallback(connectResponse->playerNum);
        	
            return true;
        }
		
        return false;
	}

	bool MultiplayerManager::OnMessageReceived(const MessageData& message)
	{
		if (message.isOfType<MsgDisconnect>())
		{
            DEBUG_PRINT("Recieved disconnection message.\n");
            m_DisconnectedCallback(m_PlayerCount);
            m_PlayerCount--;
            return true;
		}
		if (message.isOfType<MsgHandleConnectRequest>())
		{
            const auto connectRequest = message.get<MsgHandleConnectRequest>();
            if (connectRequest->reply == MsgHandleConnectRequest::Reply::ACCEPTED)
            {
                m_pOwner->setRemoteAddress(message.getAddress());
                m_ConnectedCallback(++m_PlayerCount);
                m_pOwner->setPlayerNum(m_PlayerCount);
            	
            	DEBUG_PRINT("Connected to host. :)\n");
            }
			else
            {
                DEBUG_PRINT("Connection denied. :(\n");
            }
			
            return true;
		}
        if (message.isOfType<MsgStartStage>())
        {
            if (m_IsHost)
                return false;
        	
            auto* pSeq = ApplicationWin::GetInstance()->GetGame()->GetSequence();
            if (!pSeq)
                return false;

            pSeq->m_StgId = message.get<MsgStartStage>()->stageID;
            pSeq->SeqGotoStage();
        	
            return true;
        }
		
        return false;
	}

    void MultiplayerManager::OnObjectAdded(GameObject* pObj)
    {
        if (!m_pOwner)
            return;

        auto* pSetListener = dynamic_cast<CSetObjectListener*>(pObj);
        if (!pSetListener)
            return;

        if (!pSetListener->GetAdapter() || !pSetListener->GetAdapter()->GetObjectResource().IsValid())
            return;

        const size_t uid = pSetListener->GetAdapter()->GetObjectResource().GetUID();
        if (MPUtil::IsObjTracked(uid))
            return;

        const auto createObjMsg = AllocateMessage<MsgCreateSetObject>();
        createObjMsg->setID = uid;
        m_pOwner->sendMessage(createObjMsg);
        pObj->SetProperty(MPUtil::ms_MPObjLocalPropKey, fnd::PropertyValue(1));
    }
	
	void* MultiplayerManager::MultiplayerManager_init()
    {
        INSTALL_HOOK(GameModeStageInitFirstHook);
        INSTALL_HOOK(RegisterResourceInfosHook);
        INSTALL_HOOK(GameDocumentAddObject);
		
        return new(game::GlobalAllocator::GetSingletonAllocator()) MultiplayerManager();
    }

    void MultiplayerManager::MultiplayerManager_destroy(void* instance)
    {
        delete static_cast<BaseObject*>(instance);
    }
	
	DEFINE_SINGLETON(MultiplayerManager);
}
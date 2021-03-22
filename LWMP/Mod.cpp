#include "MemoryPool.h"
#include "MessageData.h"
#include "Messages.h"
#include "MultiplayerManager.h"
#include "PlayerHandler.h"
#include "Session.h"
#include "Socket.h"
#include "WinSocket.h"

HOOK(void, __fastcall, PlayerUpdate, ASLR(0x852820), CPlayer* This, void* Edx, SUpdateInfo* update)
{
    CPlayer* player = (CPlayer*)((uint32_t)This - 8);

    auto& session = csl::fnd::Singleton<app::mp::MultiplayerManager>::GetInstance()->GetSession();
    const PlayerType type = player->blackBoard->playerNo == 0 ? PlayerType::LOCAL : PlayerType::REMOTE;

    if (session->getPlayer(type) != player)
        session->setPlayer(type, player);

    if (type == PlayerType::LOCAL)
    {
        session->updatePlayer(type);
        originalPlayerUpdate(This, Edx, update);
    }

    if (type == PlayerType::REMOTE)
    {
        // Stop our remote player from interacting with specific triggers
        // This is actual bug in the base game, thanks sonic team
        CGOComponent* collision = getComponent(&player->components, CCollisionGOCString);
        setTriggerEnable(collision, 7, false);

        auto* collector = getGOC(player, GOCCollectorString);
        if(collector)
            updateChangeRequest(collector);

        updateBeforeMove(player->physics, update->deltaTime);

        session->updatePlayer(type);

        updateComponents(&player->components, update->deltaTime);
        updateAfterMove(player->physics, update->deltaTime);
        
        getComponent(&player->components, CVisualGOCString)->Update(update->deltaTime);
        getComponent(&player->components, CEffectGOCString)->Update(update->deltaTime);
        
        updateGroundShapeInfo(player);
        updatePlayerInformation(player);
        updateHistoryData(player->physics, update->deltaTime);
    }
}

HOOK(void*, __fastcall, GameTick, ASLR(0x4AC3A0), void* This, void* Edx, void* application)
{
    auto* mulMan = csl::fnd::Singleton<app::mp::MultiplayerManager>::GetInstance();
    mulMan->PreUpdate();

    void* result = originalGameTick(This, Edx, application);
    
    mulMan->PostUpdate();

    return result;
}

HOOK(void*, __fastcall, SendMessageImm, ASLR(0x49A470), void* This, void* Edx, uint32_t to, xgame::Message* message)
{
    auto& session = csl::fnd::Singleton<app::mp::MultiplayerManager>::GetInstance()->GetSession();
    if (session->getPlayer(PlayerType::LOCAL)->gap8 == This)
    {
        if (message->ID == 0x4001)
        {
            app::CSetObjectListener* listener = getObject(to);

            if (listener)
            {
                auto kickEvent = session->getPool()->allocate<MsgDamageEvent>();
                kickEvent->damagedObject = listener->GetAdapter()->GetObjectResource()->GetID();
                session->sendMessage(kickEvent);
            }
        }
    }
    return originalSendMessageImm(This, Edx, to, message);
}

extern "C" void __declspec(dllexport) __cdecl Init(ModInfo* modInfo)
{
    if (!WinSocket::startup())
    {
        DEBUG_PRINT("Init(): WinSocket::startup() failed\n");
        return;
    }

    INSTALL_HOOK(PlayerUpdate);
    INSTALL_HOOK(GameTick);
    INSTALL_HOOK(SendMessageImm);
    app::mp::MultiplayerManager::ms_BaseDir = modInfo->CurrentMod->Path;
    size_t pos = app::mp::MultiplayerManager::ms_BaseDir.find_last_of("\\/");
    if (pos != std::string::npos)
        app::mp::MultiplayerManager::ms_BaseDir.erase(pos + 1);
}
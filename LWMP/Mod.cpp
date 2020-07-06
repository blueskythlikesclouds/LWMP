#include "MemoryPool.h"
#include "MessageData.h"
#include "Messages.h"
#include "PlayerHandler.h"
#include "Session.h"
#include "Socket.h"
#include "WinSocket.h"

std::unique_ptr<Session> session;

HOOK(void, __fastcall, PlayerUpdate, ASLR(0x852820), CPlayer* This, void* Edx, SUpdateInfo* update)
{
    CPlayer* player = (CPlayer*)((uint32_t)This - 8);

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
    session->preUpdate();

    void* result = originalGameTick(This, Edx, application);
    
    session->postUpdate();

    return result;
}

extern "C" void __declspec(dllexport) __cdecl Init(const void* data)
{
    if (!WinSocket::startup())
    {
        DEBUG_PRINT("Init(): WinSocket::startup() failed\n");
        return;
    }

    INSTALL_HOOK(PlayerUpdate);
    INSTALL_HOOK(GameTick);

    Address address;

    bool isHost = MessageBox(nullptr, "Are you the host?", nullptr, MB_YESNO) == IDYES;
    if (!isHost)
        address = Address::fromHostName("localhost", 42069);

    session = std::make_unique<Session>();
    if (isHost)
        session->openServer(42069);
    else
    {
        session->openClient(address);
        session->requestMessage<MsgHandleConnectRequest>();
    }
}
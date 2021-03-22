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

HOOK(void*, __fastcall, SendMessageImm, ASLR(0x49A470), void* This, void* Edx, uint32_t to, xgame::Message* message)
{
    if (session->getPlayer(PlayerType::LOCAL)->gap8 == This)
    {
        if (message->ID == 0x4001)
        {
            app::CSetObjectListener* listener = getObject(to);

            if (listener)
            {
                auto kickEvent = session->getPool()->allocate<MsgDamageEvent>();
                kickEvent->damagedObject = listener->GetAdapter()->GetObjectResource()->ref().GetID();
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

    std::string dir = modInfo->CurrentMod->Path;

    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos)
        dir.erase(pos + 1);

    INIReader* reader = new INIReader("LWMP.ini");
    if (reader->ParseError() != 0)
        delete reader, reader = new INIReader(dir + "LWMP.ini");

    if (reader->ParseError() != 0)
        MessageBox(NULL, TEXT("Failed to parse LWMP.ini"), NULL, MB_ICONERROR);

    const Address address = Address::fromHostName(reader->Get("Main", "IP", "localhost").c_str(), reader->GetInteger("Main", "Port", 42069));

    // printf("[LWMP] %d.%d.%d.%d:%d\n", address.numbers[0], address.numbers[1], address.numbers[2], address.numbers[3], address.port);

    session = std::make_unique<Session>();
    if (reader->GetBoolean("Main", "IsHost", true))
    {
        session->openServer(address.port);
    }
    else
    {
        session->openClient(address);
        session->requestMessage<MsgHandleConnectRequest>();
    }

    delete reader;
}
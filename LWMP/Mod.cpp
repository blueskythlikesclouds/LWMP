#include "MemoryPool.h"
#include "MessageData.h"
#include "Messages.h"
#include "PlayerHandler.h"
#include "Session.h"
#include "Socket.h"
#include "WinSocket.h"

std::unique_ptr<Session> session;

HOOK(void, __fastcall, PlayerUpdate, ASLR(0x852820), CPlayer* This, void* Edx, void* a2)
{
    CPlayer* player = (CPlayer*)((uint32_t)This - 8);

    const PlayerType type = player->blackBoard->playerNo == 0 ? PlayerType::LOCAL : PlayerType::REMOTE;

    if (session->getPlayer(type) != player)
        session->setPlayer(type, player);

    if (type == PlayerType::LOCAL)
        session->updatePlayer(type);

    originalPlayerUpdate(This, Edx, a2);

    if (type == PlayerType::REMOTE)
        session->updatePlayer(type);
}

HOOK(void*, __fastcall, GameTick, ASLR(0x4AC3A0), void* This, void* Edx, void* application)
{
    session->preUpdate();

    if (session->getSocket()->getType() == SocketType::SERVER)
    {
        for (auto& messageData : session->getReceivedMessages())
        {
            if (messageData.getId() != MsgHandleConnectRequest::ID)
                continue;

            session->setRemoteAddress(messageData.getAddress());
            break;
        }
    }

    else 
        session->sendMessage(session->getPool()->allocate<MsgHandleConnectRequest>());

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
        address = Address("127.0.0.1", 42069);

    session = std::make_unique<Session>();
    if (isHost)
        session->openServer(42069);
    else
        session->openClient(address);
}
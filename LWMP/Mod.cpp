#include "Animations.h"
#include "Client.h"
#include "Functions.h"
#include "MemoryPool.h"
#include "MessageReceiver.h"
#include "Messages.h"
#include "MessageSender.h"
#include "PacketReceiver.h"
#include "PacketSender.h"
#include "Server.h"
#include "WinSocket.h"

#include <thread>

CPlayer* localPlayer;
CPlayer* remotePlayer;

Address address;
bool isHost;
MemoryPool* pool;
MessageReceiver* receiver;
MessageSender* sender;

HOOK(void, __fastcall, PlayerUpdate, ASLR(0x852820), CPlayer* This, void* Edx, void* a2)
{
    // Don't you just love shifted pointers?
    CPlayer* player = (CPlayer*)((char*)This - 8);

    if (player != remotePlayer)
        originalPlayerUpdate(This, Edx, a2);

    CStateGOC* stateGoc = (CStateGOC*)getComponent(&player->components, (void*)ASLR(0xDF77D8));
    CVisualGOC* visualGoc = (CVisualGOC*)getComponent(&player->components, (void*)ASLR(0xE01360));
    GOCAnimationScript* animationScript = visualGoc->currentVisual->gocReferenceHolder[0]->animationScript;

    if (player == localPlayer)
    {
        if (!isHost)
            sender->send(pool->allocate<MsgHandleConnectRequest>(), address);

        auto msgSetPosition = pool->allocate<MsgSetPosition>();
        auto msgSetRotation = pool->allocate<MsgSetRotation>();
        auto msgSetBodyMode = pool->allocate<MsgSetBodyMode>();
        auto msgSetAnimation = pool->allocate<MsgSetAnimation>();
        auto msgSetAnimationFrame = pool->allocate<MsgSetAnimationFrame>();
        auto msgSetRingCount = pool->allocate<MsgSetRingCount>();

        msgSetPosition->position = getTranslation(visualGoc->visualLocaterManager->matrix);
        msgSetRotation->rotation = getQuaternion(visualGoc->visualLocaterManager->matrix);
        msgSetBodyMode->bodyMode = player->blackBoard->bodyMode;
        float animationFrame = (float)getAnimationFrame(animationScript);
        msgSetAnimationFrame->animationFrameUpper = (VarInt)animationFrame;
        msgSetAnimationFrame->animationFrameLower = (uint8_t)((animationFrame - (int32_t)animationFrame) * 255.0f);
        msgSetAnimation->animationIndex = ANIMATION_TO_INDEX_MAP.at(getCurrentAnimationName(animationScript->characterAnimationSingle));
        msgSetRingCount->ringCount = player->blackBoard->ringParameter->ringCount;

        sender->send(msgSetPosition, address);
        sender->send(msgSetRotation, address);
        sender->send(msgSetBodyMode, address);
        sender->send(msgSetAnimation, address);
        sender->send(msgSetAnimationFrame, address);
        sender->send(msgSetRingCount, address);
        sender->update();
    }

    else if (player == remotePlayer)
    {
        receiver->update();

        void* collectorComponent = getGameObjectComponent(player, (char*)ASLR(0xDF7DF4));
        if (collectorComponent)
            updateChangeRequest(collectorComponent);

        Vector3 position = player->physics->position;
        Quaternion rotation = player->physics->rotation;
        
        for (auto& messageData : receiver->getMessages())
        {
            switch (messageData.getId())
            {
            case MsgHandleConnectRequest::ID:
                if (isHost) 
                {
                    address = messageData.getAddress();
                }
                
                break;

            case MsgSetPosition::ID:
                position = messageData.get<MsgSetPosition>()->position;
                break;

            case MsgSetRotation::ID:
                rotation = messageData.get<MsgSetRotation>()->rotation;
                break;

            case MsgSetBodyMode::ID:
            {
                VarInt bodyMode = messageData.get<MsgSetBodyMode>()->bodyMode;
                if (player->blackBoard->bodyMode != bodyMode)
                    changeVisual(stateGoc, bodyMode);
                    
                break;
            }

            case MsgSetAnimation::ID:
            {
                const char* animationName = ANIMATIONS[min((size_t)messageData.get<MsgSetAnimation>()->animationIndex, ANIMATION_COUNT - 1)];
                if (!isCurrentAnimation(animationScript->characterAnimationSingle, animationName))
                    changeAnimation(animationScript->characterAnimationSingle, animationName);
                    
                break;
            }
                    
            case MsgSetAnimationFrame::ID:
            {
                const auto msgSetAnimationFrame = messageData.get<MsgSetAnimationFrame>();
                    
                setAnimationFrame(animationScript->characterAnimationSingle, 
                    (float)msgSetAnimationFrame->animationFrameUpper + msgSetAnimationFrame->animationFrameLower / 255.0f);
                break;
            }

            case MsgSetRingCount::ID:
                break;
                
            default:
                break;
            }
        }

        player->physics->rotation = rotation;
        setPosition(player->physics, position);

        visualGocUpdate(visualGoc, *(int*)a2);
        updatePlayerInformation(player);
        updateHistoryData(player->physics, *(int*)a2);
    }
}

HOOK(CPlayer*, __cdecl, CreatePlayer, ASLR(0x85A8F0), void* gameDocument, void* scInfo)
{
    CPlayer* player = originalCreatePlayer(gameDocument, scInfo);

    if (!localPlayer)
        localPlayer = player;

    else if (!remotePlayer)
        remotePlayer = player;

    return player;
}

extern "C" void __declspec(dllexport) __cdecl Init(const void* data)
{
    if (!WinSocket::startup())
    {
        DEBUG_PRINT("Init(): WinSocket::startup() failed\n");
        return;
    }

    INSTALL_HOOK(CreatePlayer);
    INSTALL_HOOK(PlayerUpdate);

    pool = new MemoryPool(1024);

    isHost = MessageBox(nullptr, "Are you the host?", nullptr, MB_YESNO) == IDYES;
    if (!isHost)
        address = Address("ip", 42069);
    Socket* socket = isHost ? static_cast<Socket*>(new Server(42069)) : new Client();
    PacketReceiver* pReceiver = new PacketReceiver(socket, pool);
    PacketSender* pSender = new PacketSender(socket);
    receiver = new MessageReceiver(pReceiver, pool);
    sender = new MessageSender(pSender, pool);
}

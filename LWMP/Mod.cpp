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
#include "Session.h"
#include "WinSocket.h"

#include <thread>

Address address;
bool isHost;
Session* activeSession;

HOOK(void, __fastcall, PlayerUpdate, ASLR(0x852820), CPlayer* This, void* Edx, void* a2)
{
    // Don't you just love shifted pointers?
    CPlayer* player = (CPlayer*)((char*)This - 8);

    CStateGOC* stateGoc = (CStateGOC*)getComponent(&player->components, (void*)ASLR(0xDF77D8));
    CVisualGOC* visualGoc = (CVisualGOC*)getComponent(&player->components, (void*)ASLR(0xE01360));
    GOCTransform* transformGoc = (GOCTransform*)getGOC(player, (void*)ASLR(0x00D60B44));
    
    GOCAnimationScript* animationScript = visualGoc->currentVisual->gocReferenceHolder[0]->animationScript;

    if (player->blackBoard->playerNo == 0)
    {
        originalPlayerUpdate(This, Edx, a2);

        if (!isHost)
            activeSession->sendMessage(activeSession->getPool()->allocate<MsgHandleConnectRequest>());
        
        auto msgSetPosition = activeSession->getPool()->allocate<MsgSetPosition>();
        auto msgSetRotation = activeSession->getPool()->allocate<MsgSetRotation>();
        auto msgSetBodyMode = activeSession->getPool()->allocate<MsgSetBodyMode>();
        auto msgSetAnimation = activeSession->getPool()->allocate<MsgSetAnimation>();
        auto msgSetAnimationFrame = activeSession->getPool()->allocate<MsgSetAnimationFrame>();
        auto msgSetRingCount = activeSession->getPool()->allocate<MsgSetRingCount>();

        msgSetPosition->position = getTranslation(visualGoc->visualLocaterManager->matrix);
        msgSetRotation->rotation = getQuaternion(visualGoc->visualLocaterManager->matrix);
        msgSetBodyMode->bodyMode = player->blackBoard->bodyMode;
        float animationFrame = (float)getAnimationFrame(animationScript);
        msgSetAnimationFrame->animationFrameUpper = (VarInt)animationFrame;
        msgSetAnimationFrame->animationFrameLower = (uint8_t)((animationFrame - (int32_t)animationFrame) * 255.0f);
        msgSetAnimation->animationIndex = ANIMATION_TO_INDEX_MAP.at(getCurrentAnimationName(animationScript->characterAnimationSingle));
        msgSetRingCount->ringCount = player->blackBoard->ringParameter->ringCount;

        activeSession->sendMessage(msgSetPosition);
        activeSession->sendMessage(msgSetRotation);
        activeSession->sendMessage(msgSetBodyMode);
        activeSession->sendMessage(msgSetAnimation);
        activeSession->sendMessage(msgSetAnimationFrame);
        activeSession->sendMessage(msgSetRingCount);
    }
    else
    {
        void* collectorComponent = getGameObjectComponent(player, (char*)ASLR(0xDF7DF4));
        if (collectorComponent)
            updateChangeRequest(collectorComponent);

        activeSession->preUpdate();
        Vector3 position = player->physics->position;
        Quaternion rotation = player->physics->rotation;
        
        for (auto& messageData : activeSession->getReceivedMessages())
        {
            switch (messageData.getId())
            {
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
            {
                const auto msgSetRingCount = messageData.get<MsgSetRingCount>();
                player->blackBoard->ringParameter->ringCount = msgSetRingCount->ringCount;

                break;
            }
                
            default:
                break;
            }
        }

        
        player->physics->rotation = rotation;
        setPosition(player->physics, position);
        int actor = getPlayerActorId(*GAME_DOCUMENT, 1);
        void* playerActor = getActor(player->messageManager, actor);

        originalPlayerUpdate((CPlayer*)playerActor, Edx, a2);
    }

    activeSession->postUpdate();
}

extern "C" void __declspec(dllexport) __cdecl Init(const void* data)
{
    if (!WinSocket::startup())
    {
        DEBUG_PRINT("Init(): WinSocket::startup() failed\n");
        return;
    }

    INSTALL_HOOK(PlayerUpdate);

    isHost = MessageBox(nullptr, "Are you the host?", nullptr, MB_YESNO) == IDYES;
    if (!isHost)
        address = Address("127.0.0.1", 42069);

    activeSession = new Session();
    if (isHost)
        activeSession->openServer(42069);
    else
        activeSession->openClient(address);
}

extern "C" void __declspec(dllexport) __cdecl OnFrame()
{
    for (MessageData messageData : activeSession->getReceivedMessages())
    {
        if (messageData.getId() == MsgHandleConnectRequest::ID)
        {
            activeSession->setRemoteAddress(messageData.getAddress());
            break;
        }
    }
}
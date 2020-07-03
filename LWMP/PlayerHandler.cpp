#include "Animations.h"
#include "MemoryPool.h"
#include "MessageData.h"
#include "Messages.h"
#include "PlayerHandler.h"
#include "Session.h"

PlayerHandler::PlayerHandler(const PlayerType type, CPlayer* player) : type(type), player(player)
{
}

void PlayerHandler::update(const Session& session)
{
    CStateGOC* stateGoc = (CStateGOC*)getComponent(&player->components, (void*)ASLR(0xDF77D8));
    CVisualGOC* visualGoc = (CVisualGOC*)getComponent(&player->components, (void*)ASLR(0xE01360));
    GOCTransform* transformGoc = (GOCTransform*)getGOC(player, (void*)ASLR(0x00D60B44));

    GOCAnimationScript* animationScript = visualGoc->currentVisual->gocReferenceHolder[0]->animationScript;

    if (type == PlayerType::LOCAL)
    {
        previous = current;

        current.position = getTranslation(visualGoc->visualLocaterManager->matrix);
        current.rotation = getQuaternion(visualGoc->visualLocaterManager->matrix);
        current.bodyMode = player->blackBoard->bodyMode;
        current.animationFrame = (float)getAnimationFrame(animationScript);
        current.animationName = getCurrentAnimationName(animationScript->characterAnimationSingle);
        current.ringCount = player->blackBoard->ringParameter->ringCount;

        if (previous.position != current.position)
        {
            auto msgSetPosition = session.getPool()->allocate<MsgSetPosition>();
            msgSetPosition->position = current.position;
            session.sendMessage(msgSetPosition);
        }

        if (previous.rotation != current.rotation)
        {
            auto msgSetRotation = session.getPool()->allocate<MsgSetRotation>();
            msgSetRotation->rotation = current.rotation;
            session.sendMessage(msgSetRotation);
        }

        if (previous.bodyMode != current.bodyMode)
        {
            auto msgSetBodyMode = session.getPool()->allocate<MsgSetBodyMode>();
            msgSetBodyMode->bodyMode = current.bodyMode;
            session.sendMessage(msgSetBodyMode);
        }

        if (previous.animationFrame != current.animationFrame)
        {
            auto msgSetAnimationFrame = session.getPool()->allocate<MsgSetAnimationFrame>();

            double integralPart;
            double fractionalPart = modf(current.animationFrame, &integralPart);

            msgSetAnimationFrame->animationFrameIntegral = (VarUInt)integralPart;
            msgSetAnimationFrame->animationFrameFractional = (uint8_t)(fractionalPart * 255.0);

            session.sendMessage(msgSetAnimationFrame);
        }

        if (previous.animationName != current.animationName)
        {
            auto msgSetAnimation = session.getPool()->allocate<MsgSetAnimation>();
            msgSetAnimation->animationIndex = ANIMATION_TO_INDEX_MAP.at(current.animationName);
            session.sendMessage(msgSetAnimation);
        }

        if (previous.ringCount != current.ringCount)
        {
            auto msgSetRingCount = session.getPool()->allocate<MsgSetRingCount>();
            msgSetRingCount->ringCount = current.ringCount;
            session.sendMessage(msgSetRingCount);
        }
    }

    else
    {
        Vector3 position = player->physics->position;
        Quaternion rotation = player->physics->rotation;

        for (auto& messageData : session.getReceivedMessages())
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
                    (float)msgSetAnimationFrame->animationFrameIntegral + (float)msgSetAnimationFrame->animationFrameFractional / 255.0f);

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
    }
}

CPlayer* PlayerHandler::getPlayer() const
{
	return player;
}
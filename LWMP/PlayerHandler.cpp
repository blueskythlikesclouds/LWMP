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
	CStateGOC* stateGoc = (CStateGOC*)getComponent(&player->components, CStateGOCString);
	CVisualGOC* visualGoc = (CVisualGOC*)getComponent(&player->components, CVisualGOCString);
	CGOComponent* collector = getGOC(player, GOCCollectorString);
	GOCTransform* transform = (GOCTransform*)getGOC(player, GOCTransformString);

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
			auto result = ANIMATION_TO_INDEX_MAP.find(current.animationName);
			if (result != ANIMATION_TO_INDEX_MAP.end())
			{
				msgSetAnimation->animationIndex = result->second;
				session.sendMessage(msgSetAnimation);
			}
			else
				DEBUG_PRINT("INVALID ANIMATION: %s\n", current.animationName);
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
				local.position = position;
				break;

			case MsgSetRotation::ID:
				rotation = messageData.get<MsgSetRotation>()->rotation;
				local.rotation = rotation;
				break;

			case MsgSetBodyMode::ID:
			{
				VarInt bodyMode = messageData.get<MsgSetBodyMode>()->bodyMode;
				local.bodyMode = bodyMode;

				break;
			}

			case MsgSetAnimation::ID:
			{
				const char* animationName = ANIMATIONS[min((size_t)messageData.get<MsgSetAnimation>()->animationIndex, ANIMATION_COUNT - 1)];
				local.animationName = animationName;
				break;
			}

			case MsgSetAnimationFrame::ID:
			{
				const auto msgSetAnimationFrame = messageData.get<MsgSetAnimationFrame>();
				local.animationFrame = (float)msgSetAnimationFrame->animationFrameIntegral + (float)msgSetAnimationFrame->animationFrameFractional / 255.0f;

				break;
			}

			case MsgSetRingCount::ID:
			{
				const auto msgSetRingCount = messageData.get<MsgSetRingCount>();
				local.ringCount = msgSetRingCount->ringCount;
				player->blackBoard->ringParameter->ringCount = local.ringCount;

				break;
			}

			case MsgDamageEvent::ID:
			{
				const auto msgDamageEvent = messageData.get<MsgDamageEvent>();
				uint32_t objectID = msgDamageEvent->damagedObject;

				MsgDamage msgDamage;
				memset(&msgDamage, 0, sizeof(MsgDamage));
				msgDamageCtor(msgDamage, 0, 8, 3, player->physics->position, player->physics->position);
				sendMessageToSetObject(player, objectID, &msgDamage, false);

				break;
			}

			default:
				break;
			}
		}

		if (local.animationName != nullptr) 
		{
			changeVisual(stateGoc, local.bodyMode);
			changeAnimation(animationScript->characterAnimationSingle, local.animationName);
			setAnimationFrame(animationScript->characterAnimationSingle, local.animationFrame);
		}

		resetPosition(stateGoc, position, rotation);
		//player->physics->rotation = rotation;
		//setPosition(player->physics, position);
		//setLocalTranslationAndRotation(transform, position, rotation);
	}
}

CPlayer* PlayerHandler::getPlayer() const
{
	return player;
}
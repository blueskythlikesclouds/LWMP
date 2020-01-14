#include "Server.h"
#include <cstdio>
#include <thread>
#include <ws2ipdef.h>
#include <WS2tcpip.h>
#include "WinSocket.h"
#include "Client.h"
#include "Window.h"
#include "Packet.h"
#include "MemoryPool.h"
#include "PacketSender.h"
#include "PacketReceiver.h"
#include "Helpers.h"
#include "Messages.h"
#include "Functions.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "Math.h"
#include "Animations.h"

CPlayer* localPlayer;
CPlayer* remotePlayer;

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
			sender->add(std::make_shared<MsgConnect>());
		
		auto msgSetPosition = pool->rent<MsgSetPosition>();
		auto msgSetRotation = pool->rent<MsgSetRotation>();
		auto msgSetBodyMode = pool->rent<MsgSetBodyMode>();
		auto msgSetAnimation = pool->rent<MsgSetAnimation>();
		auto msgSetAnimationFrame = pool->rent<MsgSetAnimationFrame>();
		auto msgSetRingCount = pool->rent<MsgSetRingCount>();

		msgSetPosition->position = getTranslation(visualGoc->visualLocaterManager->matrix);
		msgSetRotation->rotation = getQuaternion(visualGoc->visualLocaterManager->matrix);
		msgSetBodyMode->bodyMode = player->blackBoard->bodyMode;
		float animationFrame = (float)getAnimationFrame(animationScript);
		msgSetAnimationFrame->animationFrameUpper = (varint_t)animationFrame;
		msgSetAnimationFrame->animationFrameLower = (uint8_t)((animationFrame - (int32_t)animationFrame) * 255.0f);
		msgSetAnimation->animationIndex = ANIMATION_TO_INDEX_MAP[getCurrentAnimationName(animationScript->characterAnimationSingle)];
		msgSetRingCount->ringCount = player->blackBoard->ringParameter->ringCount;
		
		sender->add(msgSetPosition);
		sender->add(msgSetRotation);
		sender->add(msgSetBodyMode);
		sender->add(msgSetAnimation);
		sender->add(msgSetAnimationFrame);
		sender->add(msgSetRingCount);
		
		sender->send();
	}

	else if (player == remotePlayer)
	{
		Address address = receiver->receive();
		
		void* collectorComponent = getGameObjectComponent(player, (char*)ASLR(0xDF7DF4));
		if (collectorComponent)
			updateChangeRequest(collectorComponent);

		if (isHost && receiver->contains<MsgConnect>()) 
			sender->setAddress(address);

		if (receiver->contains<MsgSetRotation>())
			player->physics->rotation = receiver->get<MsgSetRotation>()->rotation;

		if (receiver->contains<MsgSetPosition>())
			setPosition(player->physics, receiver->get<MsgSetPosition>()->position);

		if (receiver->contains<MsgSetBodyMode>())
		{
			auto msgSetBodyMode = receiver->get<MsgSetBodyMode>();
			if (player->blackBoard->bodyMode != msgSetBodyMode->bodyMode)
				changeVisual(stateGoc, msgSetBodyMode->bodyMode);
		}

		if (receiver->contains<MsgSetAnimation>())
		{
			auto msgSetAnimation = receiver->get<MsgSetAnimation>();
			char* animationName = (char*)ANIMATIONS[min(msgSetAnimation->animationIndex, _countof(ANIMATIONS) - 1)];
			if (!isCurrentAnimation(animationScript->characterAnimationSingle, animationName))
				changeAnimation(animationScript->characterAnimationSingle, animationName);
		}

		if (receiver->contains<MsgSetAnimationFrame>())
		{
			auto msgSetAnimationFrame = receiver->get<MsgSetAnimationFrame>();
			setAnimationFrame(animationScript->characterAnimationSingle, (float)msgSetAnimationFrame->animationFrameUpper + msgSetAnimationFrame->animationFrameLower / 255.0f);
		}

		if (receiver->contains<MsgSetRingCount>())
			player->blackBoard->ringParameter->ringCount = receiver->get<MsgSetRingCount>()->ringCount;

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
		fprintf(stderr, "Init(): WinSocket::startup() failed\n");
		return;
	}

	INSTALL_HOOK(CreatePlayer);
	INSTALL_HOOK(PlayerUpdate);

	pool = new MemoryPool(1024);

	isHost = MessageBox(NULL, "Are you the host?", NULL, MB_YESNO) == IDYES;
	Address address = isHost ? Address("sajid's ip", 56253) : Address("my ip", 42069);
	Socket* socket = isHost ? (Socket*)new Server(42069) : new Client();
	PacketReceiver* pReceiver = new PacketReceiver(address, socket, pool);
	PacketSender* pSender = new PacketSender(address, socket);
	receiver = new MessageReceiver(pReceiver, pool);
	sender = new MessageSender(pSender, pool);
}

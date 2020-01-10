#pragma once

#include "MessageInfo.h"
#include "Structures.h"

struct MsgSyncPlayerData
{
	Vector3 position;
	Quaternion rotation;
	varint_t bodyMode;
	varint_t animationIndex;
	varint_t animationFrame;
	uint8_t animationFrameFraction;
	varint_t ringCount;

	DEFINE_MESSAGE_INFO()
};
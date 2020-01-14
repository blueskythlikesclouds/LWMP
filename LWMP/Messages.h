#pragma once

#include "MessageInfo.h"
#include "Types.h"
#include "VarInt.h"

struct MsgSetPosition
{
	Vector3 position;

	DEFINE_MESSAGE_INFO()
};

struct MsgSetRotation
{
	Quaternion rotation;

	DEFINE_MESSAGE_INFO()
};

struct MsgSetBodyMode
{
	varint_t bodyMode;

	DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimation
{
	varint_t animationIndex;

	DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimationFrame
{
	varint_t animationFrameUpper;
	uint8_t animationFrameLower;

	DEFINE_MESSAGE_INFO()
};

struct MsgSetRingCount
{
	varint_t ringCount;

	DEFINE_MESSAGE_INFO()
};

struct MsgConnect
{
	uint8_t padding;
	
	DEFINE_MESSAGE_INFO()
};
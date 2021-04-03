#pragma once

#include "MessageInfo.h"

struct Message
{
    uint8_t reserved;
};

struct MsgMetadata : Message
{
    VarUInt playerNum;

    DEFINE_MESSAGE_INFO()
};

struct MsgDummy : Message
{
    uint8_t dummy;

    DEFINE_MESSAGE_INFO()
};

struct MsgHandleConnectRequest : Message
{
    enum class Reply : uint8_t
    {
        ACCEPTED,
        DENIED
    };

    Reply reply;
    uint8_t playerNum;

    DEFINE_MESSAGE_INFO()
};

struct MsgDisconnect : Message
{
    uint8_t dummy;
	
	DEFINE_MESSAGE_INFO()
};

struct MsgPlayerBase : Message
{
	
};

struct MsgSetPosition : MsgPlayerBase
{
    Vector3 position;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetRotation : MsgPlayerBase
{
    Quaternion rotation;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetBodyMode : MsgPlayerBase
{
    VarUInt bodyMode;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimation : MsgPlayerBase
{
    VarUInt animationIndex;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimationFrame : MsgPlayerBase
{
    VarUInt animationFrameIntegral;
    uint8_t animationFrameFractional;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetRingCount : MsgPlayerBase
{
    VarUInt ringCount;

    DEFINE_MESSAGE_INFO()
};

struct MsgDamageEvent : MsgPlayerBase
{
    VarUInt damagedObject;
    VarUInt damage;

    DEFINE_MESSAGE_INFO()
};

struct MsgStartStage : Message
{
    csl::ut::FixedString<16> stageID;

	DEFINE_MESSAGE_INFO()
};
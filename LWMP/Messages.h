#pragma once

#include "MessageInfo.h"

struct Message
{
    uint8_t reserved;
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

    DEFINE_MESSAGE_INFO()
};

struct MsgSetPosition : Message
{
    Vector3 position;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetRotation : Message
{
    Quaternion rotation;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetBodyMode : Message
{
    VarUInt bodyMode;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimation : Message
{
    VarUInt animationIndex;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetAnimationFrame : Message
{
    VarUInt animationFrameIntegral;
    uint8_t animationFrameFractional;

    DEFINE_MESSAGE_INFO()
};

struct MsgSetRingCount : Message
{
    VarUInt ringCount;

    DEFINE_MESSAGE_INFO()
};

struct MsgDamageEvent : Message
{
    VarUInt damagedObject;

    DEFINE_MESSAGE_INFO()
};
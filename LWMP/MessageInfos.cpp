#include "Messages.h"
#include "MessageInfoRegistry.h"

DECLARE_MESSAGE_INFO(MsgHandleConnectRequest, reply)
DECLARE_MESSAGE_INFO(MsgSetPosition, position.x, position.y, position.z)
DECLARE_MESSAGE_INFO(MsgSetRotation, rotation)
DECLARE_MESSAGE_INFO(MsgSetBodyMode, bodyMode)
DECLARE_MESSAGE_INFO(MsgSetAnimation, animationIndex)
DECLARE_MESSAGE_INFO(MsgSetAnimationFrame, animationFrameIntegral, animationFrameFractional)
DECLARE_MESSAGE_INFO(MsgSetRingCount, ringCount)
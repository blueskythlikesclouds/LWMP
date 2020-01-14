#include "Messages.h"

DECLARE_MESSAGE_INFO(MsgSetPosition, position);
DECLARE_MESSAGE_INFO(MsgSetRotation, rotation);
DECLARE_MESSAGE_INFO(MsgSetBodyMode, bodyMode);
DECLARE_MESSAGE_INFO(MsgSetAnimation, animationIndex);
DECLARE_MESSAGE_INFO(MsgSetAnimationFrame, animationFrameUpper, animationFrameLower);
DECLARE_MESSAGE_INFO(MsgSetRingCount, ringCount);
DECLARE_MESSAGE_INFO(MsgConnect, padding);
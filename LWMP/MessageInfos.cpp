#include "Messages.h"
#include "MessageInfoRegistry.h"

DECLARE_MESSAGE_INFO(MsgMetadata, playerNum)
DECLARE_MESSAGE_INFO(MsgDummy, dummy)
DECLARE_MESSAGE_INFO(MsgHandleConnectRequest, reply)
DECLARE_MESSAGE_INFO(MsgDisconnect, dummy)
DECLARE_MESSAGE_INFO(MsgSetPosition, position.x, position.y, position.z)
DECLARE_MESSAGE_INFO(MsgSetRotation, rotation)
DECLARE_MESSAGE_INFO(MsgSetBodyMode, bodyMode)
DECLARE_MESSAGE_INFO(MsgSetAnimation, animationIndex)
DECLARE_MESSAGE_INFO(MsgSetAnimationFrame, animationFrameIntegral, animationFrameFractional)
DECLARE_MESSAGE_INFO(MsgSetRingCount, ringCount)
DECLARE_MESSAGE_INFO(MsgDamageEvent, damagedObject, damage)
DECLARE_MESSAGE_INFO(MsgKickEvent, kickedObject)
DECLARE_MESSAGE_INFO(MsgHitEvent, hitObject, hitUnit, hitShape)
DECLARE_MESSAGE_INFO(MsgNotifyObjectEvent, notifiedObject, event)
DECLARE_MESSAGE_INFO(MsgStartStage, stageID)
DECLARE_MESSAGE_INFO(MsgDamagePlayerEvent, dummy)
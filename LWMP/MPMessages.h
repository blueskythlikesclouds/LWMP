#pragma once
#include "MPUtil.h"

namespace app::mp
{
	class MsgHitEventCollisionMP : public xgame::MsgHitTriggerBase, MPObjHeader
	{
	public:
		inline const static unsigned int MessageID = 0x9000;
		
		MsgHitEventCollisionMP(game::ColliShape* pSelf, game::ColliShape* pOther) : MsgHitTriggerBase(MessageID, pSelf, pOther)
		{
			
		}
	};

	class MsgKickMP : public xgame::MsgKick, MPObjHeader
	{
	public:
		inline const static unsigned int MessageID = 0x4002;
		
		MsgKickMP(EnumSenderType senderType, const Description& desc, const csl::math::Vector3& dir) : MsgKick(senderType, desc, dir)
		{
			
		}
	};

	class MsgDamageMP : public xgame::MsgDamage, MPObjHeader
	{
	public:
		MsgDamageMP(EnumSenderType senderType, uint a3, uint a4, const csl::math::Vector3& pos1, const csl::math::Vector3& pos2)
		: MsgDamage(senderType, a3, a4, pos1, pos2)
		{
			
		}
	};

	class MsgNotifyObjectEventMP : public xgame::MsgNotifyObjectEvent, MPObjHeader
	{
	public:
		inline static uint MessageID = 0x6013;
		
		MsgNotifyObjectEventMP(EEventType event) : MsgNotifyObjectEvent(event)
		{
			
		}
	};

	class MsgGoalMP : public xgame::MsgGoal, MPObjHeader
	{
	public:
		inline static uint MessageID = MsgGoal::MessageID;

		MsgGoalMP() : MsgGoal()
		{

		}
	};
	
	class MsgIsLWMPObject : public fnd::Message
	{
	public:
		inline static uint MessageID = MPUtil::ms_MPSig;

		MsgIsLWMPObject() : Message(MessageID)
		{
			
		}
	};
}
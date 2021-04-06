#include "Pch.h"
#include "EventHooks.h"
#include "Messages.h"
#include "MPMessages.h"
#include "MultiplayerManager.h"

namespace app::mp
{
	HOOK(bool, __fastcall, GameObjectActorProcHook, ASLR(0x0049D0A0), fnd::CActor* pThis, void* edx, int proc, void* pData)
	{
		const bool handled = originalGameObjectActorProcHook(pThis, edx, proc, pData);

		auto* pObj = dynamic_cast<CSetObjectListener*>(pThis);
		if (pObj && handled)
		{
			reinterpret_cast<HookedObject*>(pObj)->ActorProcHooked(proc, pData);
		}

		return handled;
	}

	bool HookedObject::ActorProcHooked(int id, void* data)
	{
		switch (id)
		{
		case 0:
		case 1:
		case 2:
			HookedObject::ProcessMessage(*static_cast<fnd::Message*>(data));
			break;

		default:
			break;
		}
		return true;
	}

	bool HookedObject::ProcessMessage(fnd::Message& msg)
	{
		auto* mpMan = csl::fnd::Singleton<MultiplayerManager>::GetInstance();
		if (msg.IsOfType<xgame::MsgDamage>())
		{
			auto& rDmg = reinterpret_cast<xgame::MsgDamage&>(msg);
			if (!MPUtil::IsMpVariant(rDmg) && GetAdapter()->GetObjectResource().IsValid())
			{
				if (rDmg.m_SenderType == 1)
				{
					const auto spMsg = mpMan->AllocateMessage<MsgDamageEvent>();
					spMsg->damagedObject = GetAdapter()->GetObjectResource()->GetID();
					spMsg->damage = rDmg.m_Damage;
					mpMan->GetSession()->sendMessage(spMsg);
				}
			}
		}
		else if (msg.IsOfType<xgame::MsgKick>())
		{
			auto& rKickMsg = reinterpret_cast<xgame::MsgKick&>(msg);
			if (!MPUtil::IsMpVariant(rKickMsg) && GetAdapter()->GetObjectResource().IsValid())
			{
				const auto spMsg = mpMan->AllocateMessage<MsgKickEvent>();
				spMsg->kickedObject = GetAdapter()->GetObjectResource()->GetID();

				mpMan->GetSession()->sendMessage(spMsg);
			}
		}
		else if (msg.IsOfType<xgame::MsgHitEventCollision>())
		{
			auto& rHitEvent = reinterpret_cast<xgame::MsgHitEventCollision&>(msg);
			if (!MPUtil::IsMpVariant(rHitEvent) && GetAdapter()->GetObjectResource().IsValid())
			{
				const auto spMsg = mpMan->AllocateMessage<MsgHitEvent>();
				spMsg->hitObject = GetAdapter()->GetObjectResource()->GetID();
				spMsg->hitShape = rHitEvent.m_pShape1->GetID();

				mpMan->GetSession()->sendMessage(spMsg);
			}
			return true;
		}
		else if (msg.IsOfType<xgame::MsgNotifyObjectEvent>())
		{
			auto& rNotifyEvent = reinterpret_cast<xgame::MsgNotifyObjectEvent&>(msg);
			if (!MPUtil::IsMpVariant(rNotifyEvent) && GetAdapter()->GetObjectResource().IsValid())
			{
				const auto spMsg = mpMan->AllocateMessage<MsgNotifyObjectEvent>();
				spMsg->notifiedObject = GetAdapter()->GetObjectResource()->GetID();
				spMsg->event = rNotifyEvent.GetEventType();
				mpMan->GetSession()->sendMessage(spMsg);
			}
		}

		return false;
	}

	void EventHooks::Init()
	{
		INSTALL_HOOK(GameObjectActorProcHook);
	}
}

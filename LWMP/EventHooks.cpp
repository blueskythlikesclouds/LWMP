#include "Pch.h"
#include "EventHooks.h"
#include "Messages.h"
#include "MPMessages.h"
#include "MultiplayerManager.h"
#include "MultiplayerService.h"

namespace app::mp
{
	HOOK(bool, __fastcall, GameObjectActorProcHook, ASLR(0x0049D0A0), fnd::CActor* pThis, void* edx, int proc, void* pData)
	{
		const bool handled = originalGameObjectActorProcHook(pThis, edx, proc, pData);

		auto* pObj = dynamic_cast<CSetObjectListener*>(pThis);
		if (pObj && handled)
		{
			static_cast<HookedObject*>(pObj)->ActorProcHooked(proc, pData);
		}

		return handled;
	}

	HOOK(bool, __fastcall, CGamePreProcessMessageHook, ASLR(0x004AC1B0), fnd::CActor* pThis, void* edx, fnd::Message& msg)
	{
		const auto result = originalCGamePreProcessMessageHook(pThis, edx, msg);

		if (msg.IsOfType<xgame::MsgGoal>() && !MPUtil::IsMpVariant(reinterpret_cast<xgame::MsgGoal&>(msg)) && GameDocument::GetSingleton())
		{
			auto* pDoc = GameDocument::GetSingleton();
			auto* pService = pDoc->GetService<MultiplayerService>();
			if (pService)
				pService->GoalReached();
		}
		
		return result;
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
		if (!GetAdapter() || !GetAdapter()->GetObjectResource() || !GetAdapter()->GetObjectResource().IsValid())
		{
			return true;
		}
		
		auto* mpMan = csl::fnd::Singleton<MultiplayerManager>::GetInstance();
		if (msg.IsOfType<xgame::MsgDamage>())
		{
			auto& rDmg = reinterpret_cast<xgame::MsgDamage&>(msg);
			if (!MPUtil::IsMpVariant(rDmg))
			{
				if (rDmg.m_SenderType == 1)
				{
					const auto spMsg = mpMan->AllocateMessage<MsgDamageEvent>();
					spMsg->damagedObject = GetAdapter()->GetObjectResource().GetUID();
					spMsg->damage = rDmg.m_Damage;
					mpMan->GetSession()->sendMessage(spMsg);
				}
			}
		}
		else if (msg.IsOfType<xgame::MsgKick>())
		{
			auto& rKickMsg = reinterpret_cast<xgame::MsgKick&>(msg);
			if (!MPUtil::IsMpVariant(rKickMsg))
			{
				const auto spMsg = mpMan->AllocateMessage<MsgKickEvent>();
				spMsg->kickedObject = GetAdapter()->GetObjectResource().GetUID();

				mpMan->GetSession()->sendMessage(spMsg);
			}
		}
		else if (msg.IsOfType<xgame::MsgHitEventCollision>())
		{
			auto& rHitEvent = reinterpret_cast<xgame::MsgHitEventCollision&>(msg);
			
			if (!MPUtil::IsMpVariant(rHitEvent) 
				&& m_pOwnerDocument->GetService<CLevelInfo>()->GetPlayerID(0) == rHitEvent.m_Sender
				&& MPUtil::IsObjectAllowed(GetAdapter()->GetObjectResource()))
			{
				DEBUG_PRINT("Relaying hit event from %s, hash: %X\n", GetAdapter()->GetObjectResource().GetName(), GetAdapter()->GetObjectResource().GetClassHash());
				const auto spMsg = mpMan->AllocateMessage<MsgHitEvent>();
				spMsg->hitObject = GetAdapter()->GetObjectResource().GetUID();
				spMsg->hitUnit = GetAdapter()->GetActor()->GetUnitNum();
				spMsg->hitShape = rHitEvent.m_pSelf->GetID();

				mpMan->GetSession()->sendMessage(spMsg);
			}
			return true;
		}
		else if (msg.IsOfType<xgame::MsgNotifyObjectEvent>())
		{
			auto& rNotifyEvent = reinterpret_cast<xgame::MsgNotifyObjectEvent&>(msg);
			if (!MPUtil::IsMpVariant(rNotifyEvent))
			{
				const auto spMsg = mpMan->AllocateMessage<MsgNotifyObjectEvent>();
				spMsg->notifiedObject = GetAdapter()->GetObjectResource().GetUID();
				spMsg->event = rNotifyEvent.GetEventType();
				mpMan->GetSession()->sendMessage(spMsg);
			}
		}

		return false;
	}

	void EventHooks::Init()
	{
		INSTALL_HOOK(GameObjectActorProcHook);
		INSTALL_HOOK(CGamePreProcessMessageHook);
	}
}

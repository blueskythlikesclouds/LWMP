#include "Pch.h"
#include "EventHooks.h"


#include "Messages.h"
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
		if (msg.IsOfType<xgame::MsgDamage>())
		{
			auto& rDmg = reinterpret_cast<xgame::MsgDamage&>(msg);
			if (rDmg.m_SenderType == 1)
			{
				auto* mpMan = csl::fnd::Singleton<MultiplayerManager>::GetInstance();

				auto spMsg = mpMan->AllocateMessage<MsgDamageEvent>();
				spMsg->damagedObject = GetAdapter()->GetObjectResource()->GetID();
				spMsg->damage = rDmg.m_Damage;
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

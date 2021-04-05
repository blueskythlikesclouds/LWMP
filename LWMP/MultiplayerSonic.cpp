#include "Pch.h"
#include "MultiplayerSonic.h"
#include "MultiplayerSonicInfo.h"
#include "MessageData.h"
#include "Messages.h"
#include "MultiplayerManager.h"
#include "Animations.h"
#include "MultiplayerService.h"
#include "PlayerData.h"
#include "MPMessages.h"

namespace app::mp
{
	using namespace fnd;
	
	void MultiplayerSonic::AddCallback(GameDocument& document)
	{
		m_pPhysics = new(GetAllocator()) CPhysicsStub();
		m_pBlackboard = new(GetAllocator()) CBlackBoard();
		m_pBlackboard->playerNo = 0;
		m_pBlackboard->bodyMode = Player::BodyMode::Human;
		
		m_pLevelInfo = document.GetService<CLevelInfo>();
		m_pMpService = document.GetService<MultiplayerService>();

		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer)
			return;
		
		GOComponent::Create<GOCVisualContainer>(*this);
		GOComponent::Create<game::GOCAnimationContainer>(*this);
		GOComponent::Create<game::GOCIKConstraint>(*this);
		
		auto* pCollector = GOComponent::Create<Player::GOCCollector>(*this);
		auto* pShadow = GOComponent::Create<Player::GOCPlayerShadow>(*this);
		
		GOComponent::BeginSetup(*this);

		m_pVisual = new(GetAllocator()) Player::CVisualGOC(this);
		Player::CEffectGOC* pEffect = new(GetAllocator()) Player::CEffectGOC(this, m_pVisual);

		m_Components.SetGOC(m_pVisual);
		m_Components.SetGOC(pEffect);

		pCollector->Setup(this);
		RegisterResources(document);
		
		const Player::GOCPlayerShadow::Description shadowDesc{ 2, &m_GravityController };
		pShadow->Setup(shadowDesc);

		const Player::SVisualCinfo visInfo{};
		m_pVisual->Initialize(visInfo);
		pEffect->Initialize();
		
		csl::fnd::Singleton<MultiplayerManager>::GetInstance()->GetSession()->addListener(*this);
		GOComponent::EndSetup(*this);
	}

	bool MultiplayerSonic::OnMessageReceived(const MessageData& message)
	{
		auto playerNum = -1;

		const auto spMeta = message.getMetadata();
		if (!spMeta)
			return false;

		playerNum = spMeta->playerNum;
		if (playerNum == 0)
			playerNum = 1;

		if (playerNum != m_PlayerNum)
			return false;
		
		if (message.isOfType<MsgSetAnimation>())
			return ProcMsgSetAnimation(message.get<MsgSetAnimation>());
		if (message.isOfType<MsgHitEvent>())
			return ProcMsgHitEvent(message.get<MsgHitEvent>());
		if (message.isOfType<MsgDamageEvent>())
			return ProcMsgDamageEvent(message.get<MsgDamageEvent>());
		if (message.isOfType<MsgKickEvent>())
			return ProcMsgKickEvent(message.get<MsgKickEvent>());
		if (message.isOfType<MsgSetBodyMode>())
		{
			const auto pMsg = message.get<MsgSetBodyMode>();

			m_pBlackboard->bodyMode = pMsg->bodyMode;
			m_pVisual->ChangeVisual(static_cast<Player::BodyMode>(pMsg->bodyMode));
			return true;
		}

		return false;
	}

	bool MultiplayerSonic::ProcessMessage(fnd::Message& msg)
	{
		if (msg.IsOfType<xgame::MsgTakeObject>())
		{
			auto& rTakeObj = reinterpret_cast<xgame::MsgTakeObject&>(msg);
			if (rTakeObj.IsValidUserID())
			{
				rTakeObj.m_Taken = true;
			}
			
			return true;
		}
		
		return false;
	}

	void MultiplayerSonic::Update(const fnd::SUpdateInfo& info)
	{
		auto* pCollector = GetComponent<Player::GOCCollector>();
		if (pCollector)
			pCollector->UpdateChangeRequest();

		m_Components.Update(info.deltaTime);
	}

	void MultiplayerSonic::UpdatePhase(const fnd::SUpdateInfo& update_info, fnd::UpdatingPhase phase)
	{
		if (m_pVisual->GetCurrentVisual() && m_pVisual->GetCurrentVisual()->m_pGOCHolder)
		{
			auto& rpAnimation = m_pVisual->m_rpCurrentVisual->m_pGOCHolder->GetUnit(0).m_rpAnimScript;
			if (rpAnimation)
			{
				rpAnimation->SetFrame(m_pMpService->GetPlayerData(m_PlayerNum)->animationFrame);
			}
		}
		
		if (phase == 1)
		{
			auto* playerData = m_pMpService->GetPlayerData(m_PlayerNum);

			// Copy to stack because compiler refuses to align these
			const auto pos = playerData->position;
			const auto rot = playerData->rotation;

			m_pPhysics->m_Rotation = rot;
			m_pPhysics->SetPosition(pos);
		}
	}

	bool MultiplayerSonic::ProcMsgSetAnimation(const std::shared_ptr<MsgSetAnimation> spMsg) const
	{
		if (m_pVisual->GetCurrentVisual() && m_pVisual->GetCurrentVisual()->m_pGOCHolder)
		{
			auto& rpAnimation = m_pVisual->m_rpCurrentVisual->m_pGOCHolder->GetUnit(0).m_rpAnimScript;
			if (rpAnimation)
			{
				rpAnimation->ChangeAnimation(ANIMATIONS[spMsg->animationIndex]);
			}
		}
		
		return true;
	}

	bool MultiplayerSonic::ProcMsgHitEvent(const std::shared_ptr<MsgHitEvent> spMsg) const
	{
		auto* pSetMan = m_pOwnerDocument->GetService<CSetObjectManager>();
		if (!pSetMan)
			return false;

		const auto objHandle = ObjUtil::GetGameObjectHandle(pSetMan, spMsg->hitObject);
		
		if (!objHandle)
			return false;

		auto* pCollider = objHandle->GetComponent<game::GOCCollider>();
		if (!pCollider)
		{
			DEBUG_PRINT("What did you hit??\n");
			return false;
		}
		
		auto* pShape = pCollider->FindColliShape(spMsg->hitShape);
		if (pShape)
		{
			MsgHitEventCollisionMP hitMsg{ pShape, pShape };

			hitMsg.m_Sender = GetID();

			ObjUtil::SendMessageImmToSetObject(*this, spMsg->hitObject, hitMsg, true);
		}
		
		return true;
	}

	bool MultiplayerSonic::ProcMsgDamageEvent(const std::shared_ptr<MsgDamageEvent> spMsg) const
	{
		auto msgDamage = MsgDamageMP{0, 8, 3, pTransform->GetLocalPosition(), pTransform->GetLocalPosition()};
		
		ObjUtil::SendMessageImmToSetObject(*this, spMsg->damagedObject, msgDamage, true);
		return true;
	}

	bool MultiplayerSonic::ProcMsgKickEvent(const std::shared_ptr<MsgKickEvent> spMsg) const
	{
		// Fill the description properly and send message
		auto* pSetMan = m_pOwnerDocument->GetService<CSetObjectManager>();
		if (!pSetMan)
			return false;

		const auto objHandle = ObjUtil::GetGameObjectHandle(pSetMan, spMsg->kickedObject);

		if (!objHandle)
			return false;

		auto* pCollider = objHandle->GetComponent<game::GOCCollider>();
		if (!pCollider)
		{
			DEBUG_PRINT("What did you hit??\n");
			return false;
		}
		auto* pShape = pCollider->GetShape();

		const xgame::MsgKick::Description kickDesc{ pShape, pShape };
		auto msgKick = MsgKickMP{ 0, kickDesc, pTransform->GetLocalPosition() };
		ObjUtil::SendMessageImmToSetObject(*this, spMsg->kickedObject, msgKick, true);
		return true;
	}

	bool MultiplayerSonic::SetupInfo(GameDocument& document, csl::fnd::IAllocator* pAllocator)
	{
		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer)
			return false;

		if (pContainer->IsRegister("MultiplayerSonicInfo"))
			return true;

		auto* pInfo = new (pAllocator) MultiplayerSonicInfo();
		pContainer->Register(pInfo->GetInfoName(), pInfo);
		return true;
	}

	void MultiplayerSonic::RegisterResources(GameDocument& document)
	{
		struct VisualResourceInfo
		{
			typedef void __cdecl FuncType(MultiplayerSonic*, Player::GOCReferenceHolder&, uint);
			
			const char* m_pResName{};
			size_t m_HolderNum{};
			FuncType* m_pRegisterFunc{};
		};

		FUNCTION_PTR(void, __cdecl, fp_RegisterVisualSonicResource, ASLR(0x008FE300), MultiplayerSonic*, Player::GOCReferenceHolder&, uint, bool);
		FUNCTION_PTR(void, __cdecl, fp_RegisterVisualSpinResource, ASLR(0x008FEA20), MultiplayerSonic*, Player::GOCReferenceHolder&, uint, uint, bool);
		FUNCTION_PTR(void, __cdecl, fp_RegisterPhantomSpinResource, ASLR(0x008FD8F0), MultiplayerSonic*, Player::GOCReferenceHolder&, uint, bool);
		
		const VisualResourceInfo resInfos[] =
		{
			{"SuperSonicInfo"     , 2 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FED10))},
			{"PhantomLaserInfo"   , 4 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FD160))},
			{"PhantomRocketInfo"  , 5 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FD5C0))},
			{"PhantomAsteroidInfo", 6 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FC460))},
			{"PhantomEagleInfo"   , 7 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FC9C0))},
			{"PhantomRhythmInfo"  , 9 , reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FD470))},
			{"PhantomBombInfo"    , 10, reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FC660))},
			{"PhantomHoverInfo"   , 11, reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FCAD0))},
			{"PhantomQuakeInfo"   , 12, reinterpret_cast<VisualResourceInfo::FuncType*>(ASLR(0x008FD3A0))},
		};

		auto* pCollector = GetComponent<Player::GOCCollector>();
		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer || !pCollector)
			return;

		fp_RegisterVisualSonicResource(this, pCollector->GetHolder(Player::BodyMode::Human), 0, false);
		fp_RegisterVisualSpinResource(this, pCollector->GetHolder(Player::BodyMode::Spin), 0, 1, true);

		if (pContainer->IsRegister("PhantomSpinInfo"))
			fp_RegisterPhantomSpinResource(this, pCollector->GetHolder(Player::BodyMode::PhantomSpin), 0, true);
		
		for (auto& info : resInfos)
		{
			if (pContainer->IsRegister(info.m_pResName))
			{
				info.m_pRegisterFunc(this, pCollector->GetHolder(info.m_HolderNum), m_PlayerNum + 1);
			}
		}
	}
}

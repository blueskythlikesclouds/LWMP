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

#undef SendMessage

namespace app::mp
{
	using namespace fnd;

	void MultiplayerSonic::AddCallback(GameDocument& document)
	{
		m_pSetMan = document.GetService<CSetObjectManager>();
		m_pPhysics = new(GetAllocator()) CPhysicsStub();
		m_pBlackboard = new(GetAllocator()) CBlackBoard();
		m_pBlackboard->playerNo = 0;
		m_pBlackboard->bodyMode = Player::BodyMode::Human;

		m_pLevelInfo = document.GetService<CLevelInfo>();
		m_pMpService = document.GetService<MultiplayerService>();

		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer)
			return;

		m_pCollider = GOComponent::Create<game::GOCCollider>(*this);
		GOComponent::Create<game::GOCSound>(*this);
		GOComponent::Create<GOCVisualContainer>(*this);
		GOComponent::Create<game::GOCIKConstraint>(*this);
		GOComponent::Create<game::GOCAnimationContainer>(*this);

		auto* pCollector = GOComponent::Create<Player::GOCCollector>(*this);
		auto* pShadow = GOComponent::Create<Player::GOCPlayerShadow>(*this);

		GOComponent::BeginSetup(*this);

		game::GOCSound::SimpleSetup(this, 4, 4);
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

		const game::GOCCollider::Description colDesc{ 2 };
		m_pCollider->Setup(colDesc);

		game::ColliCapsuleShapeCInfo shapeCapsule{};
		shapeCapsule.m_Radius = 2;
		shapeCapsule.m_Height = 20;
		shapeCapsule.m_ShapeID = 6;

		shapeCapsule.m_Flags = 2;
		shapeCapsule.m_Unk3 = 0x00020000;
		shapeCapsule.m_Unk7 = 0x12;
		m_pCollider->CreateShape(shapeCapsule);

		shapeCapsule.m_ShapeID = 0;
		shapeCapsule.m_Unk3 = 0x00030000;
		m_pCollider->CreateShape(shapeCapsule);
		
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
		if (message.isOfType<MsgNotifyObjectEvent>())
			return ProcMsgNotifyObjectEvent(message.get<MsgNotifyObjectEvent>());
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
		if (msg.IsOfType<MsgIsLWMPObject>())
		{
			return true;
		}
		
		if (msg.IsOfType<xgame::MsgTakeObject>())
		{
			auto& rTakeObj = reinterpret_cast<xgame::MsgTakeObject&>(msg);
			rTakeObj.m_Taken = true;

			return true;
		}
		else if (msg.IsOfType<xgame::MsgPLGetHomingTargetInfo>())
		{
			auto& rInfoMsg = reinterpret_cast<xgame::MsgPLGetHomingTargetInfo&>(msg);

			rInfoMsg.m_CursorPosition = m_pTransform->GetLocalPosition();
			rInfoMsg.SetHandledFlag();
			return true;
		}
		else if (msg.IsOfType<xgame::MsgDamage>())
		{
			const auto pMsg = m_pOwner->getPool()->allocate<MsgDamagePlayerEvent>();
			m_pOwner->sendMessage(pMsg);
			
			auto& rDamage = reinterpret_cast<xgame::MsgDamage&>(msg);
			rDamage.SetReplyStatus({0, 0, 0}, true);
			return true;
		}

		return false;
	}

	void MultiplayerSonic::Update(const fnd::SUpdateInfo& info)
	{	
		if (m_pSetMan)
			m_pSetMan->SetBasePos(m_PlayerNum, m_pTransform->GetLocalPosition());

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

			m_pTransform->SetLocalTranslation(pos);
			m_pTransform->SetLocalRotation(rot);

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
		const auto objHandle = ObjUtil::GetGameObjectHandle(*m_pOwnerDocument, spMsg->hitObject, spMsg->hitUnit);

		if (!objHandle)
			return false;

		auto* pCollider = objHandle->GetComponent<game::GOCCollider>();
		if (!pCollider)
		{
			DEBUG_PRINT("What did you hit??\n");
			return false;
		}

		auto* pShape = pCollider->FindColliShape(spMsg->hitShape);
		if (pShape && pShape->IsEnable())
		{
			MsgHitEventCollisionMP hitMsg{ pShape, m_pCollider->GetShape() };

			hitMsg.m_Sender = GetID();

			SendMessageImm(objHandle->GetID(), hitMsg);
		}

		return true;
	}

	bool MultiplayerSonic::ProcMsgDamageEvent(const std::shared_ptr<MsgDamageEvent> spMsg) const
	{
		auto msgDamage = MsgDamageMP{ 0, 8, 3, m_pTransform->GetLocalPosition(), m_pTransform->GetLocalPosition() };

		ObjUtil::SendMessageImmToSetObject(*this, spMsg->damagedObject, msgDamage, true);
		return true;
	}

	bool MultiplayerSonic::ProcMsgKickEvent(const std::shared_ptr<MsgKickEvent> spMsg) const
	{
		// Fill the description properly and send message
		const auto objHandle = ObjUtil::GetGameObjectHandle(*m_pOwnerDocument, spMsg->kickedObject);

		if (!objHandle)
			return false;

		auto* pCollider = objHandle->GetComponent<game::GOCCollider>();
		if (!pCollider)
		{
			DEBUG_PRINT("What did you hit??\n");
			return false;
		}
		auto* pShape = pCollider->GetShape();

		if (pShape && pShape->IsEnable())
		{
			const xgame::MsgKick::Description kickDesc{ m_pCollider->GetShape(), pShape };
			auto msgKick = MsgKickMP{ 0, kickDesc, m_pTransform->GetLocalPosition() };
			ObjUtil::SendMessageImmToSetObject(*this, spMsg->kickedObject, msgKick, true);
		}
		
		return true;
	}

	bool MultiplayerSonic::ProcMsgNotifyObjectEvent(const std::shared_ptr<MsgNotifyObjectEvent> spMsg) const
	{
		MsgNotifyObjectEventMP msgNotify{ spMsg->event };
		ObjUtil::SendMessageImmToSetObject(*this, spMsg->notifiedObject, msgNotify, true);
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

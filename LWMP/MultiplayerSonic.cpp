#include "Pch.h"
#include "MultiplayerSonic.h"
#include "MultiplayerSonicInfo.h"
#include "MessageData.h"
#include "Messages.h"
#include "MultiplayerManager.h"
#include "Animations.h"

namespace app::mp
{
	using namespace fnd;

	void MultiplayerSonic::AddCallback(GameDocument& document)
	{
		m_pLevelInfo = document.GetService<CLevelInfo>();
		if (!SetupInfo(document))
			return;

		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer)
			return;

		auto* pInfo = reinterpret_cast<MultiplayerSonicInfo*>(pContainer->GetInfo("MultiplayerSonicInfo"));

		if (!pInfo)
			return;

		auto* pModel = GOComponent::Create<GOCVisualModel>(*this);
		auto* pAnimScript = GOComponent::Create<game::GOCAnimationScript>(*this);
		GOComponent::BeginSetup(*this);
		
		if (pModel)	
		{
			const GOCVisualModel::Description modelDesc{ pInfo->m_pModel, pInfo->m_pSkeleton };
			pModel->Setup(modelDesc);
		}

		if (pAnimScript)
		{
			const game::GOCAnimationScript::Description animDesc{ &pInfo->m_AnimationContainer };
			pAnimScript->Setup(animDesc);
			pModel->AttachAnimation(pAnimScript);
			pAnimScript->ChangeAnimation("STAND");
		}

		csl::fnd::Singleton<MultiplayerManager>::GetInstance()->GetSession()->addListener(*this);
		GOComponent::EndSetup(*this);
	}

	bool MultiplayerSonic::OnMessageReceived(const MessageData& message)
	{
		if (message.isOfType<MsgSetPosition>())
			return ProcMsgSetPosition(message.get<MsgSetPosition>());
		if (message.isOfType<MsgSetRotation>())
			return ProcMsgSetRotation(message.get<MsgSetRotation>());
		if (message.isOfType<MsgSetAnimation>())
			return ProcMsgSetAnimation(message.get<MsgSetAnimation>());
		if (message.isOfType<MsgSetAnimationFrame>())
			return ProcMsgSetAnimationFrame(message.get<MsgSetAnimationFrame>());
		if (message.isOfType<MsgSetBodyMode>())
		{
			const auto pMsg = message.get<MsgSetBodyMode>();
			GetComponent<GOCVisualModel>()->SetVisibility(pMsg->bodyMode == 0);
			
			return true;
		}

		return false;
	}

	bool MultiplayerSonic::ProcMsgSetPosition(std::shared_ptr<MsgSetPosition> spMsg) const
	{
		const csl::math::Vector3 position{ spMsg->position.x , spMsg->position.y , spMsg->position.z };
		pTransform->SetLocalTranslation(position);
		return true;
	}

	bool MultiplayerSonic::ProcMsgSetRotation(std::shared_ptr<MsgSetRotation> spMsg) const
	{
		const csl::math::Quaternion rotation{ spMsg->rotation.x, spMsg->rotation.y , spMsg->rotation.z , spMsg->rotation.w };
		pTransform->SetLocalRotation(rotation);
		return true;
	}

	bool MultiplayerSonic::ProcMsgSetAnimation(std::shared_ptr<MsgSetAnimation> spMsg) const
	{
		GetComponent<game::GOCAnimationScript>()->ChangeAnimation(ANIMATIONS[spMsg->animationIndex]);
		return true;
	}

	bool MultiplayerSonic::ProcMsgSetAnimationFrame(std::shared_ptr<MsgSetAnimationFrame> spMsg) const
	{
		GetComponent<game::GOCAnimationScript>()->SetFrame((float)spMsg->animationFrameIntegral + (float)spMsg->animationFrameFractional / 255.0f);
		return true;
	}

	bool MultiplayerSonic::SetupInfo(GameDocument& document)
	{
		auto* pContainer = document.GetService<CObjInfoContainer>();
		if (!pContainer)
			return false;

		if (pContainer->IsRegister("MultiplayerSonicInfo"))
			return true;

		auto* pInfo = new (GetAllocator()) MultiplayerSonicInfo();
		pContainer->Register("MultiplayerSonicInfo", pInfo);
		pContainer->LoadRequestAll();
		pInfo->Initialize(document);
		return true;
	}
}

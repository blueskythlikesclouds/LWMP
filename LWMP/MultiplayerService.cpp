#include "Pch.h"
#include "MultiplayerService.h"
#include "Animations.h"
#include "MemoryPool.h"
#include "MultiplayerManager.h"
#include "MultiplayerSonic.h"
#include "MessageData.h"
#include "Messages.h"
#include "Window.h"

namespace app::mp
{
	MultiplayerService::MultiplayerService(MultiplayerManager& rMan) : GameService(0), m_pMuliplayerManager(&rMan)
	{
		PlayerData info{};
		m_PlayersData.push_back(info);
		m_PlayersData.push_back(info);
		
		SetUpdateFlag(0, true);
		SetUpdateFlag(1, true);
		SetUpdateFlag(2, true);
		
		rMan.GetSession()->addListener(*this);
		rMan.m_ConnectedCallback += MakePair(this, &MultiplayerService::ConnectedCallback);
		rMan.m_DisconnectedCallback += MakePair(this, &MultiplayerService::DisconnectedCallback);
	}

	MultiplayerService::~MultiplayerService()
	{
		m_pMuliplayerManager->m_ConnectedCallback -= MakePair(this, &MultiplayerService::ConnectedCallback);
		m_pMuliplayerManager->m_DisconnectedCallback -= MakePair(this, &MultiplayerService::DisconnectedCallback);
	}

	void MultiplayerService::ResolveAccessibleService(GameDocument& document)
	{
		m_pLevelInfo = m_pOwnerDocument->GetService<CLevelInfo>();
	}
	
	void MultiplayerService::StartGame(bool a1)
	{
		if (!a1)
			return;

		const PlayerData dummyData{};
		m_Players.clear();
		m_PlayersData.clear();
		
		m_PlayersData.resize(m_pMuliplayerManager->m_PlayerCount + 1);
		
		for (size_t i = 0; i < m_pMuliplayerManager->m_PlayerCount; i++)
		{
			auto* pSonic = new MultiplayerSonic(i + 1);
			m_Players.push_back(pSonic);
			m_pOwnerDocument->AddGameObject(pSonic);
			m_PlayersData[i + 1] = dummyData;
		}
	}

	void MultiplayerService::Load()
	{
		MultiplayerSonic::SetupInfo(*m_pOwnerDocument, GetAllocator());
		if (m_pOwner)
		{
			const auto msgStart = m_pOwner->getPool()->allocate<MsgStartStage>();
			msgStart->stageID = ApplicationWin::GetInstance()->GetGame()->GetSequence()->m_StgId.c_str();
			m_pOwner->sendMessage(msgStart);
		}
	}

	void MultiplayerService::ConnectedCallback(size_t playerNum)
	{
		auto* pSonic = new MultiplayerSonic(playerNum);
		m_Players.push_back(pSonic);
		m_pOwnerDocument->AddGameObject(pSonic);
		m_PlayersData.resize(playerNum);
	}

	void MultiplayerService::DisconnectedCallback(size_t playerNum)
	{
		m_Players[playerNum - 1]->Kill();
		m_Players.remove(playerNum - 1);
		Window::appear(L"The remote player has left. :(", 1);
	}

	bool MultiplayerService::OnMessageReceived(const MessageData& message)
	{
		auto playerNum = 1;

		const auto spMeta = message.getMetadata();
		if (!spMeta)
			return false;

		playerNum = spMeta->playerNum;
		if (playerNum == 0)
			playerNum = 1;
		
		auto& playerData = m_PlayersData[playerNum];

		if (message.isOfType<MsgSetPosition>())
		{
			const auto spMsgSetPos = message.get<MsgSetPosition>();
			playerData.position = spMsgSetPos->position;

			return true;
		}
		else if (message.isOfType<MsgSetRotation>())
		{
			const auto spMsgSetRot = message.get<MsgSetRotation>();
			playerData.rotation = spMsgSetRot->rotation;
			return true;
		}
		else if (message.isOfType<MsgSetAnimationFrame>())
		{
			const auto spMsgSetFrame = message.get<MsgSetAnimationFrame>();
			playerData.animationFrame = (float)spMsgSetFrame->animationFrameIntegral + (float)spMsgSetFrame->animationFrameFractional / 255.0f;
			
			return true;
		}
		else if (message.isOfType<MsgDamagePlayerEvent>() && m_SonicHandle)
		{
			xgame::MsgDamage msgDamage{ 3, 8, 1, m_SonicHandle->GetComponent<fnd::GOCTransform>()->GetLocalPosition(), {0, 0, 0} };
			m_SonicHandle->SendMessage(msgDamage);
		}
		
		return false;
	}

	void MultiplayerService::Update(const fnd::SUpdateInfo& info)
	{
		if (!m_pLevelInfo)
			return;

		// Look for sonic if we lost him
		if (!m_SonicHandle)
		{
			const auto playerId = m_pLevelInfo->GetPlayerID(0);

			// Ya like C++ casts?
			m_SonicHandle = reinterpret_cast<MultiplayerSonic*>(dynamic_cast<GameObject3D*>(m_pMessageManager->GetActor(playerId)));
			return;
		}

		auto& localData = m_PlayersData[0];
		auto* pVisual = m_SonicHandle->m_Components.GetGOC<Player::CVisualGOC>();
		auto* pLocaterMan = pVisual ? pVisual->m_rpLocatorManager.get() : nullptr;
		
		if (pVisual && pLocaterMan)
		{
			const csl::math::Vector3 position = pLocaterMan->m_Transform.GetTransVector();
			const csl::math::Quaternion rotation(pLocaterMan->m_Transform);

			if (localData.position != position)
			{
				localData.position = position;
				auto msgSetPos = m_pOwner->getPool()->allocate<MsgSetPosition>();
				msgSetPos->position = localData.position;
				m_pOwner->sendMessage(msgSetPos);
			}

			if (localData.rotation != rotation)
			{
				localData.rotation = rotation;
				auto msgSetRot = m_pOwner->getPool()->allocate<MsgSetRotation>();
				msgSetRot->rotation = localData.rotation;
				m_pOwner->sendMessage(msgSetRot);
			}
		}

		if (pVisual && pVisual->GetCurrentVisual() && pVisual->GetCurrentVisual()->m_pGOCHolder)
		{
			auto& rUnit = pVisual->GetCurrentVisual()->m_pGOCHolder->GetUnit(0);
			auto* pAnimScript = rUnit.m_rpAnimScript.get();

			if (m_SonicHandle->m_pBlackboard->bodyMode == Player::BodyMode::Human)
				pAnimScript = pVisual->GetHumanAnimationPtr();
			
			if (pAnimScript)
			{
				if (localData.animationFrame != pAnimScript->GetFrame())
				{
					localData.animationFrame = pAnimScript->GetFrame();
					const auto msgSetFrame = m_pOwner->getPool()->allocate<MsgSetAnimationFrame>();
					double integralPart;
					const double fractionalPart = modf(localData.animationFrame, &integralPart);

					msgSetFrame->animationFrameFractional = static_cast<uint8_t>(fractionalPart * 255.0);
					msgSetFrame->animationFrameIntegral = static_cast<VarUInt>(integralPart);
					m_pOwner->sendMessage(msgSetFrame);
				}

				if (localData.animationName != pAnimScript->GetAnimation())
				{
					localData.animationName = pAnimScript->GetAnimation();

					const auto result = ANIMATION_TO_INDEX_MAP.find(localData.animationName);
					if (result != ANIMATION_TO_INDEX_MAP.end())
					{
						const auto msgSetAnimation = m_pOwner->getPool()->allocate<MsgSetAnimation>();
						msgSetAnimation->animationIndex = result->second;
						m_pOwner->sendMessage(msgSetAnimation);
					}
					else
						DEBUG_PRINT("INVALID ANIMATION: %s\n", localData.animationName);
				}
			}

			if (m_SonicHandle->m_pBlackboard->bodyMode != localData.bodyMode)
			{
				localData.bodyMode = m_SonicHandle->m_pBlackboard->bodyMode;
				const auto msgSetBodyMode = m_pOwner->getPool()->allocate<MsgSetBodyMode>();
				msgSetBodyMode->bodyMode = localData.bodyMode;

				m_pOwner->sendMessage(msgSetBodyMode);
			}
		}
	}
}

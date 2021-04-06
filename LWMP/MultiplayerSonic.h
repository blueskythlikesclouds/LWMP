#pragma once
#include "SessionListener.h"

class MessageData;
struct MsgSetPosition;
struct MsgSetRotation;
struct MsgSetAnimation;
struct MsgSetAnimationFrame;
struct MsgDamageEvent;
struct MsgKickEvent;
struct MsgHitEvent;
struct MsgNotifyObjectEvent;

namespace app::mp
{
	class MultiplayerService;

	class CPhysicsStub : public fnd::ReferencedObject
	{
	public:
		inline static FUNCTION_PTR(void, __thiscall, ms_fpSetPosition, ASLR(0x855650), CPhysicsStub* This, const csl::math::Vector3& position);
		
		INSERT_PADDING(20) {};
		csl::math::Vector3 m_Position{};
		csl::math::Quaternion m_Rotation{};
		csl::math::Matrix34 m_Transform{};
		
		CPhysicsStub()
		{
			ASSERT_OFFSETOF(CPhysicsStub, m_Transform, 64);
		}

		void SetPosition(const csl::math::Vector3& rPos)
		{
			ms_fpSetPosition(this, rPos);
		}
	};
	
	class MultiplayerSonic : public GameObject3D, SessionListener
	{
		friend MultiplayerService;
		
	protected:
		size_t m_PlayerNum{};

		// Please never move these fields
		CGOCCollection<5> m_Components{};
		Player::CVisualGOC* m_pVisual{};
		CLevelInfo* m_pLevelInfo{};
		ut::RefPtr<CPhysicsStub> m_pPhysics{};
		ut::RefPtr<CBlackBoard> m_pBlackboard{};
		MultiplayerService* m_pMpService{};
		Player::GravityController m_GravityController{};
		
	public:
		MultiplayerSonic(size_t playerNum) : m_PlayerNum(playerNum)
		{
			ASSERT_OFFSETOF(MultiplayerSonic, m_Components, 0x32C);
			ASSERT_OFFSETOF(MultiplayerSonic, m_pPhysics, 0x344);
			ASSERT_OFFSETOF(MultiplayerSonic, m_pBlackboard, 0x348);
			
			SetUpdateFlag(0, true);
			SetUpdateFlag(1, true);
			SetUpdateFlag(2, true);
			SetObjectCategory(12);
		}

		static bool SetupInfo(GameDocument& document, csl::fnd::IAllocator* pAllocator);

		void RegisterResources(GameDocument& document);
		void AddCallback(GameDocument& document) override;
		bool OnMessageReceived(const MessageData& message) override;
		bool ProcessMessage(fnd::Message& msg) override;
		void Update(const fnd::SUpdateInfo& info) override;
		void UpdatePhase(const fnd::SUpdateInfo& update_info, fnd::UpdatingPhase phase) override;

	protected:
		bool ProcMsgSetAnimation(const std::shared_ptr<MsgSetAnimation> spMsg) const;
		bool ProcMsgHitEvent(const std::shared_ptr<MsgHitEvent> spMsg) const;
		bool ProcMsgDamageEvent(const std::shared_ptr<MsgDamageEvent> spMsg) const;
		bool ProcMsgKickEvent(const std::shared_ptr<MsgKickEvent> spMsg) const;
		bool ProcMsgNotifyObjectEvent(const std::shared_ptr<MsgNotifyObjectEvent> spMsg) const;
	};
}

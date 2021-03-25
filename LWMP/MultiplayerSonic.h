#pragma once
#include "SessionListener.h"
class MessageData;
struct MsgSetPosition;
struct MsgSetRotation;
struct MsgSetAnimation;
struct MsgSetAnimationFrame;

namespace app::mp
{
	class MultiplayerSonic : public GameObject3D, SessionListener
	{
	protected:
		size_t m_PlayerNum{};
		CLevelInfo* m_pLevelInfo{};
		
	public:
		MultiplayerSonic(size_t playerNum) : m_PlayerNum(playerNum)
		{
			SetObjectCategory(12);
		}

		static bool SetupInfo(GameDocument& document);
		void AddCallback(GameDocument& document) override;
		bool OnMessageReceived(const MessageData& message) override;

	protected:
		bool ProcMsgSetPosition(std::shared_ptr<MsgSetPosition> spMsg) const;
		bool ProcMsgSetRotation(std::shared_ptr<MsgSetRotation> spMsg) const;
		bool ProcMsgSetAnimation(std::shared_ptr<MsgSetAnimation> spMsg) const;
		bool ProcMsgSetAnimationFrame(std::shared_ptr<MsgSetAnimationFrame> spMsg) const;
	};
}

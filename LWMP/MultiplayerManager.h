#pragma once
#undef SendMessage
#include "Session.h"
#include "SessionListener.h"

namespace app::mp
{
	class MultiplayerService;

	class MultiplayerManager : public fnd::ReferencedObject,
		fnd::CLeafActor, SessionListener,
		csl::fnd::Singleton<MultiplayerManager>
	{
		friend MultiplayerService;
		static void* MultiplayerManager_init();
		static void MultiplayerManager_destroy(void* instance);

		DECLARE_SINGLETON
	protected:
		std::shared_ptr<Session> m_spSession{};
		size_t m_PlayerCount{};

	public:
		inline static std::string ms_BaseDir{};
		MultiplayerManager();
		~MultiplayerManager();
		bool OnMessageRequested(const MessageRequest& request) override;
		bool OnMessageReceived(const MessageData& message) override;
		
		const std::shared_ptr<Session> GetSession() const
		{
			return m_spSession;
		}

		void PreUpdate()
		{
			m_spSession->preUpdate();
		}

		void PostUpdate()
		{
			m_spSession->postUpdate();
		}
	};
}

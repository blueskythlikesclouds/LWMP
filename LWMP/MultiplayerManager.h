#pragma once
#undef SendMessage
#include "Session.h"
#include "MemoryPool.h"
#include "SessionListener.h"

namespace app::mp
{
	class MultiplayerService;

	class MultiplayerManager : public fnd::ReferencedObject,
		public fnd::CLeafActor, public SessionListener,
		public csl::fnd::Singleton<MultiplayerManager>
	{
		friend MultiplayerService;
		static void* MultiplayerManager_init();
		static void MultiplayerManager_destroy(void* instance);

		DECLARE_SINGLETON
	protected:
		std::shared_ptr<Session> m_spSession{};
		size_t m_PlayerCount{};
		bool m_IsHost{ true };
		
	public:
		inline static std::string ms_BaseDir{};
		csl::fnd::Delegate<void(size_t playerNum)> m_ConnectedCallback{};
		csl::fnd::Delegate<void(size_t playerNum)> m_DisconnectedCallback{};
		ut::ScopedPtr<font::FontText> m_pDebugFont;
		
		MultiplayerManager();
		~MultiplayerManager();
		bool ProcessMessage(fnd::Message& msg) override;
		bool OnMessageRequested(const MessageRequest& request) override;
		bool OnMessageReceived(const MessageData& message) override;
		
		const std::shared_ptr<Session>& GetSession() const
		{
			return m_spSession;
		}

		template<typename T>
		std::shared_ptr<T> AllocateMessage();
		
		void PreUpdate()
		{
			m_spSession->preUpdate();
		}

		void PostUpdate()
		{
			m_spSession->postUpdate();
		}

	protected:
		void OnDisconnected(Session* pSender, bool timedOut);
	};

	template <typename T>
	std::shared_ptr<T> MultiplayerManager::AllocateMessage()
	{
		return GetSession()->getPool()->allocate<T>();
	}
}

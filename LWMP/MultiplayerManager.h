#pragma once
#undef SendMessage
#include "Session.h"

namespace app::mp
{
	class MultiplayerManager : public fnd::ReferencedObject, fnd::CLeafActor, csl::fnd::Singleton<MultiplayerManager>
	{
		static void* MultiplayerManager_init();
		static void MultiplayerManager_destroy(void* instance);
		
		DECLARE_SINGLETON
	protected:
		std::unique_ptr<Session> m_upSession{};
		
	public:
		inline static std::string ms_BaseDir{};
		MultiplayerManager();
		~MultiplayerManager();

		const std::unique_ptr<Session>& GetSession() const
		{
			return m_upSession;
		}

		void PreUpdate()
		{
			m_upSession->preUpdate();
		}

		void PostUpdate()
		{
			m_upSession->postUpdate();
		}
	};
}

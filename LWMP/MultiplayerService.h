#pragma once
#include "SessionListener.h"
#include "PlayerData.h"

namespace app::mp
{
	class MultiplayerSonic;
	class MultiplayerManager;

	class MultiplayerService : public fnd::GameService, SessionListener
	{
	protected:
		MultiplayerManager* m_pMuliplayerManager{};
		CLevelInfo* m_pLevelInfo{};
		GameObjectHandle<MultiplayerSonic> m_SonicHandle; // Pretend this is CPlayer
		csl::ut::InplaceMoveArray<MultiplayerSonic*, 2> m_Players{ GetAllocator() };
		csl::ut::InplaceObjectMoveArray<PlayerData, 2> m_PlayersData{ GetAllocator() };
		
	public:
		~MultiplayerService();
		MultiplayerService(MultiplayerManager& rMan);
		
		static GameService* MultiplayerService_Create(csl::fnd::IAllocator* pAllocator)
		{
			return new(pAllocator) MultiplayerService(*csl::fnd::Singleton<MultiplayerManager>::GetInstance());
		}

		inline static const fnd::GameServiceClass ms_StaticClass{ "MultiplayerService", MultiplayerService_Create };
		static const fnd::GameServiceClass& staticClass()
		{
			return ms_StaticClass;
		}

		void Load() override;
		void ConnectedCallback(size_t playerNum);
		void DisconnectedCallback(size_t playerNum);
		bool OnMessageReceived(const MessageData& message) override;
		void Update(const fnd::SUpdateInfo& info) override;
		void StartGame(bool a1) override;
		void OnRemovedFromGame() override
		{
			m_pLevelInfo = nullptr;
		}

		const PlayerData* GetPlayerData(size_t player)
		{
			return &m_PlayersData[player];
		}
	};
}

#pragma once

namespace app::mp
{
	class MultiplayerManager;

	class MultiplayerService : public fnd::GameService
	{
	protected:
		MultiplayerManager* m_pMuliplayerManager{};
		
	public:
		MultiplayerService(MultiplayerManager& rMan) : GameService(0), m_pMuliplayerManager(&rMan)
		{
			SetUpdateFlag(0, true);
			SetUpdateFlag(1, true);
			SetUpdateFlag(2, true);
		}
		
		static GameService* MultiplayerService_Create(csl::fnd::IAllocator* pAllocator)
		{
			return new(pAllocator) MultiplayerService(*csl::fnd::Singleton<MultiplayerManager>::GetInstance());
		}

		inline static const fnd::GameServiceClass ms_StaticClass{ "MultiplayerService", MultiplayerService_Create };
		static const fnd::GameServiceClass& staticClass()
		{
			return ms_StaticClass;
		}
		
		void StartGame(bool a1) override;
	};
}

#pragma once

namespace app::mp
{
	class MultiplayerService : public fnd::GameService
	{	
	public:
		MultiplayerService() : GameService(0)
		{
			SetUpdateFlag(0, true);
			SetUpdateFlag(1, true);
			SetUpdateFlag(2, true);
		}
		
		static GameService* MultiplayerService_Create(csl::fnd::IAllocator* pAllocator)
		{
			return new(pAllocator) MultiplayerService();
		}

		inline static const fnd::GameServiceClass ms_StaticClass{ "MultiplayerService", MultiplayerService_Create };
		static const fnd::GameServiceClass& staticClass()
		{
			return ms_StaticClass;
		}

		void StartGame(bool a1) override
		{
			if (!a1)
				return;

			// TODO: Create player objects
		}
	};
}

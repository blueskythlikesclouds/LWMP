#include "Pch.h"
#include "MultiplayerService.h"
#include "MultiplayerManager.h"
#include "MultiplayerSonic.h"

namespace app::mp
{
	void MultiplayerService::StartGame(bool a1)
	{
		if (!a1)
			return;
		
		for (size_t i = 0; i < m_pMuliplayerManager->m_PlayerCount; i++)
		{
			auto* pSonic = new MultiplayerSonic(i + 1);
			document->AddGameObject(pSonic);
		}
	}
}
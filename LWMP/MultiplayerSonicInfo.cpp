#include "Pch.h"
#include "MultiplayerSonicInfo.h"

namespace app::mp
{
	using namespace animation;
	using namespace hh::gfx::res;
	
	void MultiplayerSonicInfo::Load(CObjInfoFileLoader& loader)
	{
		loader.Load("Sonic.pac", 0x4001);
	}

	void MultiplayerSonicInfo::Initialize(GameDocument& document)
	{
		auto pac = ObjUtil::GetPackFile("Sonic.pac");
		m_pModel = pac.Get<ResModel>("chr_Sonic", nullptr);
		m_pSkeleton = pac.Get<ResSkeleton>("chr_Sonic", nullptr);
		auto* pScript = pac.Get<ResCharAnimScript>("player_sonic", nullptr);
		AnimationResContainer::LoadInfo loadInfo{ pScript, m_pSkeleton, nullptr };
		m_AnimationContainer.LoadFromBuffer(loadInfo, pac);
	}
}

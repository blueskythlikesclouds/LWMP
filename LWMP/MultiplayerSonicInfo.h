#pragma once

namespace app::mp
{
	class MultiplayerSonic;

	class MultiplayerSonicInfo : public CObjInfo
	{
		friend MultiplayerSonic;
		
	protected:
		hh::gfx::res::ResModel* m_pModel{};
		hh::gfx::res::ResSkeleton* m_pSkeleton{};
		animation::AnimationResContainer m_AnimationContainer{ *GetAllocator() };
		
	public:
		MultiplayerSonicInfo() {}
		void Load(CObjInfoFileLoader& loader) override;
		void Initialize(GameDocument& document) override;
		const char* GetInfoName() override
		{
			return "MultiplayerSonicInfo";
		}
	};
}
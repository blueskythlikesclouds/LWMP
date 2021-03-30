#pragma once

namespace app::game
{
	class GOCSound;
}

namespace app::Player
{
	class CVisualGOC;
	class GOCReferenceHolder;
	class CPlayer;
	class EffectList;
	class CVisualUnit;
	
	class CVisualBase
	{
	protected:
		CPlayer* m_pOwner{};
		CVisualGOC* m_pVisual{};
		game::GOCSound* m_pSound{};
	};

	class CPlayerVisual : public fnd::ReferencedObject, CVisualBase
	{
	protected:
		void* m_pVftableVisualBase{};
		const char* m_pVisualName{};

	public:
		csl::ut::Bitset<ushort> m_Flags1{};
		csl::ut::Bitset<ushort> m_Flags2{};
		EffectList* m_Effects{}; // ut::RefPtr<EffectList>
		CVisualUnit* m_Unit{}; // ut::RefPtr<CVisualUnit>
		GOCReferenceHolder* m_pGOCHolder{};
		
	public:
		CPlayerVisual()
		{
			
		}

		virtual void Update(float delta) = 0;
		virtual void UpdateMatrix()
		{
			Update(0);
		}
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
	};
}
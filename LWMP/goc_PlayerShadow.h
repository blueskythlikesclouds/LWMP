#pragma once

namespace app::game
{
	class ShadowShape;
	class GOCIKConstraint : public fnd::GOComponent
	{
	public:
		inline static fnd::GOComponentClass* ms_pStaticClass = reinterpret_cast<fnd::GOComponentClass*>(ASLR(0x00FD7748));

		static fnd::GOComponentClass* staticClass()
		{
			return ms_pStaticClass;
		}
	};
}

namespace app::Player
{
	class GravityController : public fnd::ReferencedObject
	{
	public:
		INSERT_PADDING(8){};
		float m_GravitySize{ 1 };
		INSERT_PADDING(8) {};
		csl::math::Vector3 m_Down{ 0, -1, 0 };
		INSERT_PADDING(96){};
		
		GravityController()
		{
			
		}
	};
	
	class GOCPlayerShadow : public fnd::GOComponent
	{
	public:
		struct Description
		{
			size_t m_Count;
			GravityController* m_pGravityController;
		};
		
		csl::ut::ObjectMoveArray<game::ShadowShape*> m_Shapes{ GetAllocator() };
		GravityController* m_pGravityController;
		
		inline static fnd::GOComponentClass* ms_pStaticClass = reinterpret_cast<fnd::GOComponentClass*>(ASLR(0x00FEF340));

		static fnd::GOComponentClass* staticClass()
		{
			return ms_pStaticClass;
		}

		void Setup(const Description& desc)
		{
			m_Shapes.reserve(desc.m_Count);
			m_pGravityController = desc.m_pGravityController;
		}
	};
}
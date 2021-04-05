#pragma once

namespace app::mp
{
	class MPUtil
	{
	protected:
		static bool IsMpVariantImpl(void* pObj, size_t objSize);
		
	public:
		inline static constexpr size_t ms_MPSig = 0x4C574D50;

		template<typename T>
		static bool IsMpVariant(T& pObj)
		{
			return IsMpVariantImpl(&pObj, sizeof(T));
		}
	};

	class MPObjHeader
	{
	public:
		size_t m_Signature{ MPUtil::ms_MPSig };
	};
}

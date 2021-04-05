#include "Pch.h"
#include "MPUtil.h"

namespace app::mp
{
	bool MPUtil::IsMpVariantImpl(void* pObj, size_t objSize)
	{
		auto* pHeader = reinterpret_cast<MPObjHeader*>(static_cast<char*>(pObj) + objSize);
		return pHeader->m_Signature == ms_MPSig;
	}
}

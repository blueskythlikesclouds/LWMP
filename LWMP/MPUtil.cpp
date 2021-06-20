#include "Pch.h"
#include "MPUtil.h"
#undef SendMessage

namespace app::mp
{
	std::unordered_map<size_t, bool> MPUtil::ms_IgnoredObjects =
	{
		// ObjFlowVolume
		{ 0x72E92B38, true }
	};
	
	bool MPUtil::IsMpVariantImpl(void* pObj, size_t objSize)
	{
		auto* pHeader = reinterpret_cast<MPObjHeader*>(static_cast<char*>(pObj) + objSize);
		return pHeader->m_Signature == ms_MPSig;
	}

	bool MPUtil::IsObjectAllowed(const SetEd::CResObject& rObj)
	{
		return ms_IgnoredObjects.find(rObj.GetClassHash()) == ms_IgnoredObjects.end();
	}
}

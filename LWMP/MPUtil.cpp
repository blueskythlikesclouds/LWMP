#include "Pch.h"
#include "MPUtil.h"

namespace app::mp
{
	bool MPUtil::IsMpVariantImpl(void* pObj, size_t objSize)
	{
		auto* pHeader = reinterpret_cast<MPObjHeader*>(static_cast<char*>(pObj) + objSize);
		return pHeader->m_Signature == ms_MPSig;
	}

	void MPUtil::AddObjToTracker(size_t id)
	{
		ms_ObjTrack.insert_or_assign(id, true);
	}

	void MPUtil::RemoveObjFromTracker(size_t id)
	{
		ms_ObjTrack.erase(id);
	}

	bool MPUtil::IsObjTracked(size_t id)
	{
		return ms_ObjTrack.find(id) != ms_ObjTrack.end();
	}

	GameObjectHandle<CSetObjectListener> MPUtil::CreateSetObject(const GameDocument& rDoc, CSetObjectID id)
	{
		auto* pObjMan = rDoc.GetService<CSetObjectManager>();
		if (!pObjMan)
			return nullptr;

		const auto objHandle = ObjUtil::GetGameObjectHandle(rDoc, id);
		if (objHandle)
			return objHandle;

		auto* pAdapter = pObjMan->FindAdapter(id);
		if (!pAdapter)
			return nullptr;

		AddObjToTracker(id);
		auto* pObj = pObjMan->CreateObjectEditing(pAdapter);
		if (pObj)
		{
			pObj->SetProperty(ms_MPObjPropKey, fnd::PropertyValue(true));
		}
		
		RemoveObjFromTracker(id);
		
		return pObj;
	}
	
	void MPUtil::KillSetObject(const GameDocument& rDoc, CSetObjectID id)
	{
		const auto objHandle = ObjUtil::GetGameObjectHandle(rDoc, id);
		if (!objHandle)
			return;

		if (objHandle->GetProperty(ms_MPObjPropKey).getInt())
		{
			objHandle->TerminateImmediately();
		}
	}
}

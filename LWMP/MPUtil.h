#pragma once

namespace app::mp
{
	class MPUtil
	{
	protected:
		static bool IsMpVariantImpl(void* pObj, size_t objSize);
		static void AddObjToTracker(size_t id);
		static void RemoveObjFromTracker(size_t id);
		
	public:
		inline static std::map<size_t, bool> ms_ObjTrack{};
		inline static constexpr size_t ms_MPSig = 0x4C574D50;
		inline static size_t ms_MPObjPropKey = fnd::Property::MapStringToKey("is_mp_obj");
		inline static size_t ms_MPObjLocalPropKey = fnd::Property::MapStringToKey("is_mp_obj_local");

		template<typename T>
		static bool IsMpVariant(T& pObj)
		{
			return IsMpVariantImpl(&pObj, sizeof(T));
		}

		static bool IsObjTracked(size_t id);
		static GameObjectHandle<CSetObjectListener> CreateSetObject(const GameDocument& rDoc, CSetObjectID id);
		static void KillSetObject(const GameDocument& rDoc, CSetObjectID id);
	};

	class MPObjHeader
	{
	public:
		size_t m_Signature{ MPUtil::ms_MPSig };
	};
}

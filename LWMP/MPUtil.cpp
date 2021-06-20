#include "Pch.h"
#include "MPUtil.h"
#undef SendMessage

namespace app::mp
{
	std::unordered_map<size_t, bool> MPUtil::ms_IgnoredObjects =
	{
		{ 0x72E92B38, true }, // ObjFlowVolume
		{ 0xC6DDE0B8, true }, // EventVolumeCameraChange
		{ 0x65ADB907, true }, // RestrictPhantomCollision
		{ 0xC3EBFBA1, true }, // PhantomCancelBox
		{ 0xE037DCFB, true }, // JumpCollision
		{ 0xBDFC726D, true }, // LaunchStar
		{ 0x25D8A66B, true }, // LaunchStarDynamic
		{ 0xA6DBDDD6, true }, // PathSpeedControlCollision
		{ 0x771D7DCD, true }, // CameraCollisionCylinder
		{ 0x17E9EE73, true }, // HintCollision
		{ 0x957118CA, true }, // DashPanel
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

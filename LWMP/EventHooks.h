#pragma once

namespace app::mp
{
	class HookedObject : public CSetObjectListener
	{
	public:
		bool ProcessMessage(fnd::Message& msg) override;
		bool ActorProcHooked(int id, void* data);
	};
	
	class EventHooks
	{
	public:
		static void Init();
	};
}
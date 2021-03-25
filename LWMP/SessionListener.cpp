#include "Pch.h"
#include "SessionListener.h"
#include "Session.h"

namespace app::mp
{
	SessionListener::~SessionListener()
	{
		if (m_pOwner)
			m_pOwner->removeListener(*this);
	}
}
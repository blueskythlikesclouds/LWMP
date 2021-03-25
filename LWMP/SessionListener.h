#pragma once

class Session;
class MessageRequest;
class MessageData;

namespace app::mp
{
	class SessionListener
	{
		friend Session;
		
	protected:
		Session* m_pOwner{};

	public:
		virtual ~SessionListener();

		virtual bool OnMessageRequested(const MessageRequest& request) { return false; }
		virtual bool OnMessageReceived(const MessageData& message) { return false; }
	};
}
#include "Client.h"
#include "MemoryPool.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "Messages.h"
#include "PacketReceiver.h"
#include "PacketSender.h"
#include "PlayerData.h"
#include "Server.h"
#include "Session.h"
#include "SessionListener.h"

void Session::createHandlers()
{
    packetReceiver = std::make_shared<PacketReceiver>(socket, pool);
    packetSender = std::make_shared<PacketSender>(socket);
    messageReceiver = std::make_unique<MessageReceiver>(packetReceiver, pool);
    messageSender = std::make_unique<MessageSender>(packetSender, pool);
}

Session::Session() : pool(std::make_shared<MemoryPool>(128))
{

}

Session::~Session() = default;

void Session::openClient(const Address& address)
{
    closeSocket();

    socket = std::make_shared<Client>();
    remoteAddress = address;

    createHandlers();
}

void Session::openServer(const uint16_t port)
{
    closeSocket();

    socket = std::make_shared<Server>(port);
    createHandlers();
}

void Session::closeSocket()
{
    socket = nullptr;
}

void Session::preUpdate()
{
    messageReceiver->update();
    isConnected = socket->isConnected();
	
    if (messageReceiver->hasData())
    {
        timedOut = false;
        lastUpdate = std::chrono::system_clock::now();
        for (MessageRequest request : getReceivedRequests())
        {
            for (size_t i = 0; i < listeners.size(); i++)
            {
	            auto listener = listeners[i];
                listener->OnMessageRequested(request);
            }
        }

    	for (auto& message : messageReceiver->getMessages())
    	{
            for (size_t i = 0; i < listeners.size(); i++)
            {
                auto listener = listeners[i];
                listener->OnMessageReceived(message);
            }
    	}
    }
}

void Session::postUpdate()
{
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - lastUpdate;
    
    // Request a dummy message to make sure we are still connected
    if (elapsed.count() > timeout && !timedOut) {
        requestMessage<MsgDummy>();
        timedOut = true;
    }

	// Attach metadata
    const auto spMeta = pool->allocate<MsgMetadata>();
    spMeta->playerNum = getPlayerNum();

    sendMessage(spMeta);
    messageSender->update();
    isConnected = socket->isConnected();
    timedOut = !isConnected;
}

const std::shared_ptr<Socket>& Session::getSocket() const
{
    return socket;
}

Address Session::getRemoteAddress() const
{
    return remoteAddress;
}

void Session::setRemoteAddress(const Address& address)
{
    remoteAddress = address;
    addRemoteAddress(address);
}

void Session::addRemoteAddress(const Address& address)
{
    remotes.insert(address);
}

const std::shared_ptr<MemoryPool>& Session::getPool() const
{
    return pool;
}

const std::vector<MessageRequest>& Session::getReceivedRequests() const
{
    return messageReceiver->getRequests();
}

const std::vector<MessageData>& Session::getReceivedMessages() const
{
    return messageReceiver->getMessages();
}

void Session::requestMessage(const MessageInfo* info, const Address& address) const
{
    messageSender->request(info, address);
}

void Session::sendMessage(const MessageInfo* info, const std::shared_ptr<Message>& message,
	const Address& address) const
{
    messageSender->send(info, message, address);
}

void Session::broadcastMessage(const MessageInfo* info, const std::shared_ptr<Message>& message) const
{
    for (auto& address : remotes)
    {
        messageSender->send(info, message, address);
    }
}

void Session::addListener(app::mp::SessionListener& rListener)
{
    rListener.m_pOwner = this;
    listeners.push_back(&rListener);
}

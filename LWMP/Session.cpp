#include "Session.h"
#include "Server.h"
#include "Client.h"

void Session::createHandlers()
{
    packetReceiver = new PacketReceiver(socket, pool);
    packetSender = new PacketSender(socket);
    messageReceiver = new MessageReceiver(packetReceiver, pool);
    messageSender = new MessageSender(packetSender, pool);
}

void Session::deleteHandlers()
{
    delete packetReceiver, packetReceiver = nullptr;
    delete packetSender, packetSender = nullptr;
    delete messageReceiver, messageReceiver = nullptr;
    delete messageSender, messageSender = nullptr;
}

Session::Session() : socket(NULL), isConnected(false), timedOut(false), pool(new MemoryPool(128)),
                     packetReceiver(NULL), packetSender(NULL), messageReceiver(NULL), messageSender(NULL)
{

}

Session::~Session()
{
    delete socket;
    delete pool;
    delete packetReceiver;
    delete packetSender;
    delete messageReceiver;
    delete messageSender;
}

void Session::openClient(const Address& address)
{
    closeSocket();

    socket = static_cast<Socket*>(new Client());
    remoteAddress = address;

    createHandlers();
}

void Session::openServer(const uint16_t port)
{
    closeSocket();

    socket = static_cast<Socket*>(new Server(port));
    createHandlers();
}

void Session::closeSocket()
{
    delete socket, socket = nullptr;
    deleteHandlers();
}

void Session::preUpdate()
{
    messageReceiver->update();
}

void Session::postUpdate()
{
    messageSender->update();
}

const std::vector<MessageRequest>& Session::getReceivedRequests() const
{
    return messageReceiver->getRequests();
}

const std::vector<MessageData>& Session::getReceivedMessages() const
{
    return messageReceiver->getMessages();
}

void Session::requestMessage(const MessageInfo* info) const
{
    messageSender->request(info, remoteAddress);
}

void Session::sendMessage(const MessageInfo* info, std::shared_ptr<Message> message) const
{
    messageSender->send(info, std::move(message), remoteAddress);
}

Socket* Session::getSocket() const
{
    return socket;
}

Address Session::getRemoteAddress() const
{
    return remoteAddress;
}
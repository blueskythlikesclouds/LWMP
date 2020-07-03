﻿#include "Client.h"
#include "MemoryPool.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "PacketReceiver.h"
#include "PacketSender.h"
#include "PlayerHandler.h"
#include "Server.h"
#include "Session.h"

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

void Session::preUpdate() const
{
    messageReceiver->update();
}

void Session::postUpdate() const
{
    messageSender->update();
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

void Session::updatePlayer(PlayerType type) const
{
    PlayerHandler* handler = players[(uint32_t)type].get();

    if (handler)
        handler->update(*this);
}

CPlayer* Session::getPlayer(PlayerType type) const
{
    PlayerHandler* handler = players[(uint32_t)type].get();
    return handler ? handler->getPlayer() : nullptr;
}

void Session::setPlayer(PlayerType type, CPlayer* player)
{
    players[(uint32_t)type] = player ? std::make_unique<PlayerHandler>(type, player) : nullptr;
}

void Session::requestMessage(const MessageInfo* info) const
{
    messageSender->request(info, remoteAddress);
}

void Session::sendMessage(const MessageInfo* info, const std::shared_ptr<Message>& message) const
{
    messageSender->send(info, message, remoteAddress);
}
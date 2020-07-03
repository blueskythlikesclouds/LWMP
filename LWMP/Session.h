#pragma once

#include "MessageData.h"
#include "MessageInfo.h"
#include "MessageReceiver.h"
#include "MessageSender.h"
#include "PacketReceiver.h"
#include "PacketSender.h"
#include "Socket.h"

#include <vector>

class Session
{
    Socket* socket;
    Address remoteAddress;

    bool isConnected;
    bool timedOut;

    MemoryPool* pool;

    PacketReceiver* packetReceiver;
    PacketSender* packetSender;

    MessageReceiver* messageReceiver;
    MessageSender* messageSender;

    void createHandlers();
    void deleteHandlers();

public:
    Session();
    ~Session();
    
    void openClient(const Address& address);
    void openServer(uint16_t port);
    void closeSocket();

    void preUpdate();
    void postUpdate();
    
    Socket* getSocket() const;
    Address getRemoteAddress() const;

    const std::vector<MessageRequest>& getReceivedRequests() const;
    const std::vector<MessageData>& getReceivedMessages() const;

    void requestMessage(const MessageInfo* info) const;
    void sendMessage(const MessageInfo* info, std::shared_ptr<Message> message) const;

    MemoryPool* getPool() {
        return pool;
    }

    template<typename T>
    void requestMessage()
    {
        requestMessage(&T::INFO);
    }

    template<typename T>
    void sendMessage(std::shared_ptr<T> message)
    {
        sendMessage(&T::INFO, std::reinterpret_pointer_cast<T>(message));
    }

    void setRemoteAddress(Address address) 
    {
        remoteAddress = address;
    }
};

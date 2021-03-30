﻿#pragma once

enum class PlayerType;

class MemoryPool;
class MessageData;
class MessageReceiver;
class MessageRequest;
class MessageSender;
class PacketReceiver;
class PacketSender;
class PlayerHandler;
class Socket;

struct Message;
struct MessageInfo;

namespace app::mp
{
    class SessionListener;
}

class Session
{
    std::chrono::system_clock::time_point lastUpdate;
    std::shared_ptr<Socket> socket;
    Address remoteAddress;

    double timeout{5};
    bool isConnected{};
    bool timedOut{};

    std::shared_ptr<MemoryPool> pool;

    std::shared_ptr<PacketReceiver> packetReceiver;
    std::shared_ptr<PacketSender> packetSender;

    std::unique_ptr<MessageReceiver> messageReceiver;
    std::unique_ptr<MessageSender> messageSender;

    std::vector<app::mp::SessionListener*> listeners;
	
    void createHandlers();

public:
    Session();
    ~Session();

    void openClient(const Address& address);
    void openServer(uint16_t port);
    void closeSocket();

    void preUpdate();
    void postUpdate();

    const std::shared_ptr<Socket>& getSocket() const;

    Address getRemoteAddress() const;
    void setRemoteAddress(const Address& address);

    const std::shared_ptr<MemoryPool>& getPool() const;

    const std::vector<MessageRequest>& getReceivedRequests() const;
    const std::vector<MessageData>& getReceivedMessages() const;

    void requestMessage(const MessageInfo* info) const;
    void sendMessage(const MessageInfo* info, const std::shared_ptr<Message>& message) const;

    template<typename T>
    void requestMessage() const
    {
        requestMessage(&T::INFO);
    }

    template<typename T>
    void sendMessage(const std::shared_ptr<T>& message) const
    {
        sendMessage(&T::INFO, std::reinterpret_pointer_cast<T>(message));
    }

    void addListener(app::mp::SessionListener& rListener);

	void removeListener(app::mp::SessionListener& pListener)
    {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), &pListener));
    }

	bool isConnect() const
    {
        return isConnected;
    }
};
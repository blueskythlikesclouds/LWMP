#pragma once

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
    size_t playerNum{};

    std::shared_ptr<MemoryPool> pool;

    std::shared_ptr<PacketReceiver> packetReceiver;
    std::shared_ptr<PacketSender> packetSender;

    std::unique_ptr<MessageReceiver> messageReceiver;
    std::unique_ptr<MessageSender> messageSender;

    std::vector<app::mp::SessionListener*> listeners;
    std::set<Address> remotes{};
	
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
    void addRemoteAddress(const Address& address);

    const std::shared_ptr<MemoryPool>& getPool() const;

    const std::vector<MessageRequest>& getReceivedRequests() const;
    const std::vector<MessageData>& getReceivedMessages() const;

    void requestMessage(const MessageInfo* info, const Address& address) const;
    void sendMessage(const MessageInfo* info, const std::shared_ptr<Message>& message, const Address& address) const;
    void broadcastMessage(const MessageInfo* info, const std::shared_ptr<Message>& message) const;

    template<typename T>
    void requestMessage(const Address& address) const
    {
        requestMessage(&T::INFO, address);
    }

    template<typename T>
    void requestMessage() const
    {
        requestMessage<T>(remoteAddress);
    }
	
    void sendMessage(const MessageInfo* info, const std::shared_ptr<Message>& message) const
    {
        sendMessage(info, message, remoteAddress);
    }

    void requestMessage(const MessageInfo* info) const
    {
        requestMessage(info, remoteAddress);
    }
	
    template<typename T>
    void sendMessage(const std::shared_ptr<T>& message, const Address& address) const
    {
        sendMessage(&T::INFO, std::reinterpret_pointer_cast<T>(message), address);
    }

    template<typename T>
    void sendMessage(const std::shared_ptr<T>& message) const
    {
        sendMessage<T>(message, remoteAddress);
    }
	
    template<typename T>
    void broadcastMessage(const std::shared_ptr<T>& message) const
    {
        broadcastMessage(&T::INFO, std::reinterpret_pointer_cast<T>(message));
    }
	
    void addListener(app::mp::SessionListener& rListener);
    void removeListener(app::mp::SessionListener& pListener);

	bool isConnect() const
    {
        return isConnected;
    }

	size_t getPlayerNum() const
	{
        return playerNum;
	}

	void setPlayerNum(size_t num)
	{
        playerNum = num;
	}
};
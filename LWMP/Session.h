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

class Session
{
    std::shared_ptr<Socket> socket;
    Address remoteAddress;

    bool isConnected{};
    bool timedOut{};

    std::shared_ptr<MemoryPool> pool;

    std::shared_ptr<PacketReceiver> packetReceiver;
    std::shared_ptr<PacketSender> packetSender;

    std::unique_ptr<MessageReceiver> messageReceiver;
    std::unique_ptr<MessageSender> messageSender;

    std::array<std::unique_ptr<PlayerHandler>, 2> players;

    void createHandlers();

public:
    Session();
    ~Session();

    void openClient(const Address& address);
    void openServer(uint16_t port);
    void closeSocket();

    void preUpdate() const;
    void postUpdate() const;

    void updatePlayer(PlayerType type) const;

    const std::shared_ptr<Socket>& getSocket() const;

    Address getRemoteAddress() const;
    void setRemoteAddress(const Address& address);

    const std::shared_ptr<MemoryPool>& getPool() const;

    const std::vector<MessageRequest>& getReceivedRequests() const;
    const std::vector<MessageData>& getReceivedMessages() const;

    CPlayer* getPlayer(PlayerType type) const;
    void setPlayer(PlayerType type, CPlayer* player);

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
};
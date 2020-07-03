#pragma once

class Session;

enum class PlayerType
{
    LOCAL,
    REMOTE
};

struct PlayerData
{
    Vector3 position;
    Quaternion rotation;
    uint32_t bodyMode;
    double animationFrame;
    const char* animationName;
    uint32_t ringCount;
};

class PlayerHandler
{
    PlayerType type;

    PlayerData previous;
    PlayerData current;

    CPlayer* player;
    
public:
    PlayerHandler(PlayerType type, CPlayer* player);

    void update(const Session& session);
    
    CPlayer* getPlayer() const;
};

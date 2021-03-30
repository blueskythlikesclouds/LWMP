#pragma once

class Session;

enum class PlayerType
{
    LOCAL,
    REMOTE
};

namespace app::mp
{
    struct PlayerData
    {
        csl::math::Vector3 position;
        csl::math::Quaternion rotation;
        uint32_t bodyMode;
        float animationFrame;
        const char* animationName;
        uint32_t ringCount;
    };
}

class PlayerHandler
{
    PlayerType type;

    app::mp::PlayerData previous;
    app::mp::PlayerData current;
    app::mp::PlayerData local;

    CPlayer* player;
    
public:
    PlayerHandler(PlayerType type, CPlayer* player);

    void update(const Session& session);
    
    CPlayer* getPlayer() const;
};

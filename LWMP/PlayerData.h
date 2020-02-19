#pragma once

enum PlayerType
{
    LOCAL_PLAYER,
    REMOTE_PLAYER
};

class PlayerData
{
    struct Internal
    {
        Vector3 position;
        Quaternion quaternion;
        uint32_t bodyMode;
        double animationFrame;
        const char* animationName;
        uint32_t ringCount;
    };

    Internal previous;
    Internal current;

    CPlayer* player;
    
public:
    PlayerData(CPlayer* player);

    void update();
    
    CPlayer* getPlayer() const;
};

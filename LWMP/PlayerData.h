#pragma once

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
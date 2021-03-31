#pragma once

namespace app::mp
{
    struct PlayerData
    {
        csl::math::Vector3 position { *csl::math::Vector3::Zero };
        csl::math::Quaternion rotation { *csl::math::Quaternion::Identity };
        uint32_t bodyMode;
        float animationFrame;
        const char* animationName;
        uint32_t ringCount;
    };
}
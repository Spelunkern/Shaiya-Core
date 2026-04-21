#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include "MapPos.h"

namespace shaiya
{
    #pragma pack(push, 1)
    struct BattlefieldMoveInfo
    {
        int32_t levelMin;
        int32_t levelMax;
        std::array<MapPos, 4> mapPos;
    };
    #pragma pack(pop)

    struct BattlefieldLvInRange
    {
        explicit BattlefieldLvInRange(int level)
            : level(level)
        {
        }

        bool operator()(const BattlefieldMoveInfo& info) const
        {
            return level >= info.levelMin && level <= info.levelMax;
        }

        int level;
    };

    inline std::vector<BattlefieldMoveInfo> g_battlefieldMoveData;
}

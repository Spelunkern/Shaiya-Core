#pragma once
#include <string>
#include <vector>

namespace shaiya
{
    struct Vehicle
    {
        int model;
        int bone1;
        int bone2;
        std::string wavFileName;
    };

    inline std::vector<Vehicle> g_vehicles{};
}

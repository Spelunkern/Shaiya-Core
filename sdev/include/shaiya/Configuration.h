#pragma once
#include <filesystem>

namespace shaiya
{
    class Configuration final
    {
    public:

        static void Init();
        static void LoadServerConfig();
        static void LoadBattlefieldMoveData();
        static void LoadItemSetData();
        static void LoadItemSynthesis();
        static void LoadRewardItemEvent();

    private:

        inline static std::filesystem::path m_root{};
    };
}

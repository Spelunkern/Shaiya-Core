#pragma once

namespace shaiya
{
    struct CUser;
}

void Main();
extern "C" __declspec(dllexport) void DllExport();

namespace packet_gem
{
    int use_item_ability_transfer(shaiya::CUser* user, int bag, int slot);
}

namespace hook
{
    void item_effect();
    void utilities();
    void packet_character();
    void packet_exchange();
    void packet_gem();
    void packet_mailbox();
    void packet_main_interface();
    void packet_market();
    void packet_myshop();
    void packet_party();
    void packet_pc();
    void packet_quest();
    void raid_150();
    void packet_reward_item();
    void packet_shop();
    void user_equipment();
    void user_skill();
    void user_shape();
    void user_status();
    void world_thread();
}

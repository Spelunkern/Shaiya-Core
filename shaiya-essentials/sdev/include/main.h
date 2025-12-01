#pragma once

void Main();
extern "C" __declspec(dllexport) void DllExport();

namespace hook
{
    void camera_limit();
    void character();
    void command();
    void custom_game();
    void equipment();
    void exp_view();
    void input();
    void item_icon();
    void name_color();
    void packet();
    void patch();
    void quick_slot();
    void title();
    void vehicle();
    void weapon_step();
    void window();
}

inline int g_showCostumes = false;
inline int g_showPets = false;
inline int g_showWings = false;
inline int g_showEffects = false;
inline int g_showMobEffects = false;
inline float g_cameraLimit = 30.0f;

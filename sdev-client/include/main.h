#pragma once
#include <cstddef>
#include <cstdint>

namespace shaiya
{
    struct Unknown;
}

void Main();
extern "C" __declspec(dllexport) void DllExport();
void set_performance_mode(bool enabled);
bool is_performance_mode_enabled();
inline constexpr unsigned kClientSysMsgWindowMessage = 0x8000 + 0x4A2;
void queue_client_sysmsg(int chatType, int messageNumber);
void flush_client_sysmsg_queue();
void ensure_client_sysmsg_dispatch_ready();
void sync_textbox_utf8_display(void* textBox);
bool append_utf8_textbox_wchar(void* textBox, wchar_t wideChar);
bool backspace_utf8_textbox_char(void* textBox);

namespace hook
{
    void camera_limit();
    void character();
    void command();
    void custom_game();
    void discord();
    void equipment();
    void client_features();
    void exp_view();
    void imgui_layer();
    void input();
    void item_icon();
    void main_stats();
    void name_color();
    void packet();
    void patch();
    void quick_slot();
    void raid();
    void resolutions();
    void select_screen();
    void target_view();
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
inline int g_fpsBoost = false;
inline int g_showTitles = true;
inline int g_showNameColors = true;
inline float g_cameraLimit = 30.0f;
inline shaiya::Unknown* g_uiRoot1 = nullptr;
inline shaiya::Unknown* g_uiRoot2 = nullptr;

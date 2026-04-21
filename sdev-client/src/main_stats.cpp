#include <array>
#include <filesystem>
#include <format>
#include <ranges>
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <util/util.h>
#include <shaiya/include/network/game/incoming/0200.h>
#include "include/main.h"
#include "include/shaiya/CButton.h"
#include "include/shaiya/CMessage.h"
#include "include/shaiya/CNetwork.h"
#include "include/shaiya/CPlayerData.h"
#include "include/shaiya/Static.h"
#include "include/shaiya/CWindow.h"
using namespace shaiya;

namespace
{
    struct BattlefieldMoveInfo
    {
        int mapId;
        int levelMin;
        int levelMax;
    };

    constexpr int kButtonX = 38;
    constexpr int kButtonY = 78;
    constexpr int kButtonWidth = 165;
    constexpr int kButtonHeight = 30;

    std::vector<BattlefieldMoveInfo> g_battlefieldMoveData;
    bool g_battlefieldMoveDataLoaded = false;
    bool g_buttonInitialized = false;
    bool g_mouseWasDown = false;
    CButton g_battlefieldButton{};
    CMessage* g_message = nullptr;
    D2D_POINT_2U g_anchor{};

    std::filesystem::path get_client_root()
    {
        std::wstring buffer(MAX_PATH, L'\0');
        auto count = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        buffer.resize(count);
        return std::filesystem::path(buffer).remove_filename();
    }

    std::filesystem::path find_battlefield_client_ini()
    {
        auto root = get_client_root();
        std::filesystem::path candidates[]
        {
            root / L"BattleFieldMoveInfo_Client.ini",
            root / L"Data" / L"BattleFieldMoveInfo_Client.ini",
            root / L"data" / L"BattleFieldMoveInfo_Client.ini",
        };

        for (const auto& path : candidates)
        {
            if (std::filesystem::exists(path))
                return path;
        }

        return {};
    }

    void load_battlefield_move_data()
    {
        if (g_battlefieldMoveDataLoaded)
            return;

        g_battlefieldMoveDataLoaded = true;
        g_battlefieldMoveData.clear();

        auto path = find_battlefield_client_ini();
        if (path.empty())
            return;

        auto count = static_cast<int>(GetPrivateProfileIntW(L"BATTLEFIELD_INFO", L"BATTLEFIELD_COUNT", 0, path.c_str()));
        if (count <= 0)
            return;

        g_battlefieldMoveData.reserve(count);
        for (int i = 1; i <= count; ++i)
        {
            auto section = std::format(L"BATTLEFIELD_{}", i);
            auto mapId = GetPrivateProfileIntW(section.c_str(), L"MAP_NO", -1, path.c_str());
            if (mapId < 0)
                continue;

            BattlefieldMoveInfo info{};
            info.mapId = mapId;
            info.levelMin = GetPrivateProfileIntW(section.c_str(), L"LEVEL_MIN", 0, path.c_str());
            info.levelMax = GetPrivateProfileIntW(section.c_str(), L"LEVEL_MAX", 0, path.c_str());
            g_battlefieldMoveData.push_back(info);
        }
    }

    int get_battlefield_map_id_for_level(int level)
    {
        load_battlefield_move_data();

        auto it = std::ranges::find_if(g_battlefieldMoveData, [level](const auto& info) {
            return level >= info.levelMin && level <= info.levelMax;
        });

        return it == g_battlefieldMoveData.end() ? -1 : it->mapId;
    }

    void init_battlefield_button()
    {
        if (g_buttonInitialized)
            return;

        CButton::Init(&g_battlefieldButton);
        CButton::Create(
            &g_battlefieldButton, 0, 0,
            kButtonX, kButtonY, 256, 64, 165, 47, 0,
            "main_stats_pvp_button.png", 256, 256,
            1,
            0.0F, 1.0F, 0.0F, 0.25F,
            0.0F, 1.0F, 0.25F, 0.5F,
            0.0F, 1.0F, 0.5F, 0.75F,
            0.0F, 1.0F, 0.5F, 0.75F,
            0.0F, 1.0F, 0.75F, 1.0F,
            0);

        load_battlefield_move_data();
        g_buttonInitialized = true;
    }

    bool is_cursor_over_button()
    {
        POINT cursor{};
        if (!GetCursorPos(&cursor))
            return false;

        auto hwnd = GetForegroundWindow();

        if (!hwnd || !ScreenToClient(hwnd, &cursor))
            return false;

        auto x = static_cast<int>(g_anchor.x) + kButtonX;
        auto y = static_cast<int>(g_anchor.y) + kButtonY;
        return cursor.x >= x && cursor.x <= x + kButtonWidth &&
            cursor.y >= y && cursor.y <= y + kButtonHeight;
    }

    void close_battlefield_message()
    {
        if (!g_message)
            return;

        CMessage::Hide(g_message);
        Static::operator_delete(g_message);
        g_message = nullptr;
    }

    void open_battlefield_confirm()
    {
        if (g_message)
            return;

        auto block = Static::operator_new(sizeof(CMessage));
        if (!block)
            return;

        std::array<char, 256> mapName{};
        auto mapId = get_battlefield_map_id_for_level(g_pPlayerData->level);
        if (mapId != -1)
            CPlayerData::GetMapName(mapName.data(), mapId);
        else
            strcpy_s(mapName.data(), mapName.size(), "Battleground");

        auto text = std::format("{}\n{}", mapName.data(), Static::GetMsg(514));
        g_message = CMessage::Init(block, MB_OKCANCEL, 255, 255, 255, text.c_str(), 0);
    }

    void handle_battlefield_message()
    {
        if (!g_message)
            return;

        CMessage::Show(g_message);

        switch (CMessage::DialogResult(g_message))
        {
        case IDOK:
        {
            GameMovePvPZoneIncoming outgoing{};
            CNetwork::Send(&outgoing, sizeof(outgoing));
            close_battlefield_message();
            break;
        }
        case IDCANCEL:
            close_battlefield_message();
            break;
        default:
            break;
        }
    }

    void draw_battlefield_button(CWindow* window)
    {
        init_battlefield_button();

        g_anchor = window->pos;
        CButton::Draw(&g_battlefieldButton, g_anchor.x, g_anchor.y);

        auto mouseDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        if (!mouseDown && g_mouseWasDown && is_cursor_over_button())
            open_battlefield_confirm();

        g_mouseWasDown = mouseDown;
        handle_battlefield_message();
    }

    unsigned u0x532A2A = 0x532A2A;
    void __declspec(naked) naked_0x532A23()
    {
        __asm
        {
            pushad
            push esi
            call draw_battlefield_button
            add esp,0x4
            popad

            mov ecx,[esp+0x108]
            jmp u0x532A2A
        }
    }

    unsigned u0x5320F3 = 0x5320F3;
    void __declspec(naked) naked_0x5320E6()
    {
        __asm
        {
            pushad
            call close_battlefield_message
            popad

            mov eax,[esi+0x40]
            xor edi,edi
            jmp u0x5320F3
        }
    }
}

void hook::main_stats()
{
    // Battleground buttons.
    // Draws an independent battlefield button over the existing main stats UI.
    // This intentionally does not expand or rewrite CStatusMiniBar layout, so it
    // stays compatible with the EP4 UI support already used by this client.
    util::detour((void*)0x532A23, naked_0x532A23, 7);
    util::detour((void*)0x5320E6, naked_0x5320E6, 5);
}

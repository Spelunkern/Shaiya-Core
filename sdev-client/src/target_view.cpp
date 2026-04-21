#include <algorithm>
#include <cstdio>
#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/CCharacter.h"
#include "include/shaiya/CMonster.h"
#include "include/shaiya/CWindow.h"
#include "include/shaiya/CWorldMgr.h"
#include "include/shaiya/Static.h"
using namespace shaiya;

namespace target_view
{
    constexpr int kTextOffsetX = 66;
    constexpr int kTextOffsetY = 36;

    LPD3DXFONT get_current_game_font()
    {
        // /font updates the camera D3DX font slots. Reuse those instead of
        // creating a private fixed Arial font, so the HP viewer follows the
        // player's configured game font immediately.
        if (g_var->camera.d3dxFont0)
            return g_var->camera.d3dxFont0;
        if (g_var->camera.d3dxFont1)
            return g_var->camera.d3dxFont1;
        if (g_var->camera.d3dxFont2)
            return g_var->camera.d3dxFont2;

        return g_var->camera.d3dxFont3;
    }

    void draw_hp_font_text(int x, int y, D3DCOLOR color, const char* text)
    {
        auto font = get_current_game_font();
        if (!font)
        {
            Static::DrawText_ViewPoint(x, y, color, text);
            return;
        }

        RECT rect{ x, y, x + 200, y + 24 };
        font->DrawTextA(nullptr, text, -1, &rect, DT_NOCLIP, color);
    }

    void draw_hp_text(CWindow* window, uint32_t currentHealth, uint32_t maxHealth)
    {
        if (!window || maxHealth == 0)
            return;

        currentHealth = std::min(currentHealth, maxHealth);

        char text[32]{};
        std::snprintf(text, sizeof(text), "%u/%u", currentHealth, maxHealth);

        // Draw inside the native target HP UI so the text follows the bar if
        // the target window is moved by another UI patch.
        draw_hp_font_text(
            static_cast<int>(window->pos.x) + kTextOffsetX,
            static_cast<int>(window->pos.y) + kTextOffsetY,
            0xFFFFFFFF,
            text);
    }

    void draw_target_hp(CWindow* window)
    {
        if (!g_var->targetId)
            return;

        if (g_var->targetType == TargetType::Mob)
        {
            auto mob = CWorldMgr::FindMob(g_var->targetId);
            if (mob)
                draw_hp_text(window, mob->health, mob->maxHealth);
            return;
        }

        if (g_var->targetType == TargetType::User)
        {
            auto user = CWorldMgr::FindUser(g_var->targetId);
            if (user)
                draw_hp_text(window, user->health, user->maxHealth);
        }
    }
}

unsigned u0x5351BD = 0x5351BD;
void __declspec(naked) naked_target_view_draw_hp()
{
    __asm
    {
        pushad

        push esi // native target window
        call target_view::draw_target_hp
        add esp,0x4

        popad

        // original
        mov ecx,dword ptr [esp+0x1A4]
        jmp u0x5351BD
    }
}

void hook::target_view()
{
    // Render current/max HP in the native target frame for monsters and users.
    // This replaces the temporary ImGui overlay and avoids stealing mouse input.
    util::detour((void*)0x5351B6, naked_target_view_draw_hp, 7);
}

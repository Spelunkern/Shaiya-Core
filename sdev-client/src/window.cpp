#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <util/util.h>
#include <imm.h>
#include <array>
#include <algorithm>
#include "include/main.h"
#include "include/shaiya/CQuickSlot.h"
#include "include/shaiya/Static.h"
#include "include/shaiya/Unknown.h"
using namespace shaiya;
#pragma comment(lib, "imm32.lib")

namespace window
{
    inline HWND g_hookedGameHwnd = nullptr;
    inline WNDPROC g_originalGameWndProc = nullptr;
    constexpr wchar_t kDefaultGameWindowTitle[] = L"Shaiya";
    inline DWORD g_nextTitleRefreshTick = 0;

    void refresh_game_window_title(HWND hwnd, bool force)
    {
        if (!hwnd || !IsWindowUnicode(hwnd))
            return;

        auto now = GetTickCount();
        if (!force && now < g_nextTitleRefreshTick)
            return;

        g_nextTitleRefreshTick = now + 1000;
        DefWindowProcW(hwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(kDefaultGameWindowTitle));
    }

    void copy_composition_string(HIMC context, DWORD index, wchar_t (&output)[64])
    {
        output[0] = L'\0';
        if (!context)
            return;

        auto bytes = ImmGetCompositionStringW(context, index, nullptr, 0);
        if (bytes <= 0)
            return;

        auto bytesToCopy = std::min<LONG>(bytes, static_cast<LONG>(sizeof(output) - sizeof(wchar_t)));
        auto copied = ImmGetCompositionStringW(context, index, output, static_cast<DWORD>(bytesToCopy));
        if (copied <= 0)
        {
            output[0] = L'\0';
            return;
        }

        auto chars = copied / static_cast<LONG>(sizeof(wchar_t));
        output[std::min<std::size_t>(chars, std::size(output) - 1)] = L'\0';
    }

    bool inject_unicode_textbox_input(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // The main GAME window is upgraded to Unicode by patch.cpp. For Unicode
        // HWNDs, composed input such as Vietnamese arrives as real UTF-16
        // WM_CHAR/IME text; feed that directly into the game's UTF-8 textbox
        // buffer instead of letting the stock ANSI path collapse it to '?'.
        if (IsWindowUnicode(hwnd) && msg == WM_CHAR && wParam == VK_BACK)
        {
            auto textBox = &g_var->input.textBox;
            if (backspace_utf8_textbox_char(textBox))
                return true;
        }

        if (msg == WM_CHAR && wParam > 0x7F)
        {
            auto textBox = &g_var->input.textBox;
            return append_utf8_textbox_wchar(textBox, static_cast<wchar_t>(wParam));
        }

        if (msg != WM_IME_COMPOSITION || (lParam & GCS_RESULTSTR) == 0)
            return false;

        auto context = ImmGetContext(hwnd);
        if (!context)
            return false;

        wchar_t result[64]{};
        copy_composition_string(context, GCS_RESULTSTR, result);
        ImmReleaseContext(hwnd, context);

        if (result[0] == L'\0')
            return false;

        auto textBox = &g_var->input.textBox;

        // Feed the final IME/Unicode result as a complete UTF-8 sequence instead
        // of one ANSI byte at a time. This avoids the stock textbox path turning
        // Vietnamese characters into '?' before they reach the chat buffer.
        for (auto* current = result; *current; ++current)
        {
            append_utf8_textbox_wchar(textBox, *current);
        }

        sync_textbox_utf8_display(textBox);
        return true;
    }

    void assign_windows1(Unknown* unknown)
    {
        g_uiRoot1 = unknown;
        auto quickSlot3 = g_pQuickSlot3;
        unknown->windows1.quickSlot3 = quickSlot3;
    }

    void assign_windows2(Unknown* unknown)
    {
        g_uiRoot2 = unknown;
        auto quickSlot3 = g_pQuickSlot3;
        unknown->windows2.quickSlot3 = quickSlot3;
    }

    LRESULT CALLBACK game_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // The UTF-8 window upgrade can leave legacy title writes truncated to
        // "S". Refresh from the actual subclassed GAME HWND so later client
        // rewrites cannot leave the visible caption broken.
        if (msg != WM_SETTEXT)
            refresh_game_window_title(hwnd, false);

        // Run before the original client WndProc so Unicode chat input never
        // reaches the legacy single-byte handler.
        if (inject_unicode_textbox_input(hwnd, msg, wParam, lParam))
            return 0;

        if (msg == kClientSysMsgWindowMessage)
        {
            Static::SysMsgToChatBox(static_cast<ChatType>(wParam), static_cast<int>(lParam), 1);
            return 0;
        }

        if (IsWindowUnicode(hwnd))
            return CallWindowProcW(g_originalGameWndProc, hwnd, msg, wParam, lParam);

        return CallWindowProcA(g_originalGameWndProc, hwnd, msg, wParam, lParam);
    }

    void ensure_client_sysmsg_dispatch_ready()
    {
        auto hwnd = g_var->hwnd;
        if (!hwnd || !IsWindow(hwnd))
            return;

        if (g_hookedGameHwnd == hwnd && g_originalGameWndProc)
            return;

        auto previousProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(game_wnd_proc)));
        if (!previousProc)
            return;

        g_originalGameWndProc = previousProc;
        g_hookedGameHwnd = hwnd;

        refresh_game_window_title(hwnd, true);
    }
}

unsigned u0x42B826 = 0x42B826;
void __declspec(naked) naked_0x42B820() 
{
    __asm
    {
        // original
        mov [esi+0x2A0],ecx

        pushad

        push esi
        call window::assign_windows1
        add esp,0x4

        popad

        jmp u0x42B826
    }
}

unsigned u0x42B9D7 = 0x42B9D7;
void __declspec(naked) naked_0x42B9D1() 
{
    __asm
    {
        // original
        mov [esi+0x370],ecx

        pushad

        push esi
        call window::assign_windows2
        add esp,0x4

        popad

        jmp u0x42B9D7
    }
}

void hook::window()
{
    // assign windows
    util::detour((void*)0x42B820, naked_0x42B820, 6);
    util::detour((void*)0x42B9D1, naked_0x42B9D1, 6);
}

void ensure_client_sysmsg_dispatch_ready()
{
    window::ensure_client_sysmsg_dispatch_ready();
}

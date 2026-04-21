#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <climits>
#include <cstdio>
#include <string>
#include <util/util.h>
#include "include/main.h"

namespace client_features
{
    namespace buffs
    {
        constexpr DWORD kMouseXAddr = 0x7C3C0C;
        constexpr DWORD kMouseYAddr = 0x7C3C10;
        constexpr DWORD kMouseTrampolineReturn = 0x004D73F8;
        constexpr DWORD kNewLocationTrampolineReturn = 0x004D7509;
        constexpr const char* kIniSection = "BUFF";
        constexpr const char* kIniKeyX = "LOCATION_X";
        constexpr const char* kIniKeyY = "LOCATION_Y";

        volatile LONG g_locationX = 0;
        volatile LONG g_locationY = 0;
        int g_savedX = INT_MIN;
        int g_savedY = INT_MIN;

        std::string get_config_ini_path()
        {
            char moduleFileName[MAX_PATH]{};
            if (!GetModuleFileNameA(nullptr, moduleFileName, MAX_PATH))
                return ".\\CONFIG.ini";

            std::string path(moduleFileName);
            auto slashPos = path.find_last_of("\\/");
            if (slashPos != std::string::npos)
                path.resize(slashPos + 1);

            path += "CONFIG.ini";
            return path;
        }

        void load_ini()
        {
            auto iniPath = get_config_ini_path();
            g_locationX = GetPrivateProfileIntA(kIniSection, kIniKeyX, 0, iniPath.c_str());
            g_locationY = GetPrivateProfileIntA(kIniSection, kIniKeyY, 0, iniPath.c_str());
            g_savedX = static_cast<int>(g_locationX);
            g_savedY = static_cast<int>(g_locationY);
        }

        void write_ini_int(const char* key, int value)
        {
            char buffer[16]{};
            std::snprintf(buffer, sizeof(buffer), "%d", value);
            auto iniPath = get_config_ini_path();
            WritePrivateProfileStringA(kIniSection, key, buffer, iniPath.c_str());
        }

        void save_if_changed()
        {
            auto x = static_cast<int>(g_locationX);
            auto y = static_cast<int>(g_locationY);
            if (x == g_savedX && y == g_savedY)
                return;

            write_ini_int(kIniKeyX, x);
            write_ini_int(kIniKeyY, y);
            g_savedX = x;
            g_savedY = y;
        }

        DWORD WINAPI worker(LPVOID)
        {
            for (;;)
            {
                const auto f6 = GetAsyncKeyState(VK_F6);
                const auto leftButton = GetAsyncKeyState(VK_LBUTTON);
                if ((f6 & 0x8000) && (leftButton & 0x8000))
                {
                    g_locationX = *reinterpret_cast<int*>(kMouseXAddr);
                    g_locationY = *reinterpret_cast<int*>(kMouseYAddr);
                    save_if_changed();
                    Sleep(150);
                }
                else
                {
                    Sleep(15);
                }
            }
        }

        __declspec(naked) void mouse_trampoline()
        {
            __asm
            {
                mov edi, dword ptr[g_locationX]
                mov ebx, dword ptr[g_locationY]
                jmp kMouseTrampolineReturn
            }
        }

        __declspec(naked) void new_location_trampoline()
        {
            __asm
            {
                mov edi, dword ptr[g_locationX]
                jmp kNewLocationTrampolineReturn
            }
        }

        void init()
        {
            load_ini();
            util::detour((void*)0x004D73ED, mouse_trampoline, 11);
            util::detour((void*)0x004D7503, new_location_trampoline, 6);
            CreateThread(nullptr, 0, worker, nullptr, 0, nullptr);
        }
    }

}

void hook::client_features()
{
    client_features::buffs::init();
}

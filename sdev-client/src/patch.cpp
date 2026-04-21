#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/Static.h"
#include <cstdint>
#include <cstring>

namespace
{
    constexpr std::int32_t kBuffRowX = 0x00000000;
    constexpr std::uint8_t kBuffSpacing = 0x24;
    constexpr std::uint8_t kBuffCountPerRow = 0x09;
    constexpr std::int32_t kFastTransitionDelay = 0x00000064;
    constexpr std::uint32_t kLoginCrapSkipSplashSleepDelay = 0x00000000;
    constexpr std::uint32_t kLoginCrapSkipPostInitDelay = 0x00000000;
    constexpr char kLoginCrapSkipHiddenCopyrightMessage[100] = "";
    constexpr std::int32_t kClientRessLeaderVisualSeconds = 5;
    float gLogoutGameOverVisualSeconds = 2.0f;
    constexpr std::int32_t kLoginToCharacterSelectionDelayMs = 1000;
    constexpr std::uint32_t kHiddenLevelUpMessageSize = 0x00000000;
    constexpr bool kEnableSubactionMessageCooldown = true;
    constexpr std::uint32_t kSubactionMessageCooldownMs = 20000;
    constexpr std::int32_t kSubactionMessageFirst = 5228;
    constexpr std::int32_t kSubactionMessageLast = 5237;
    constexpr std::int32_t kUtf8CodePage = CP_UTF8;
    constexpr bool kEnableUnicodeWindowTitleFix = true;
    constexpr wchar_t kDefaultGameWindowTitle[] = L"Shaiya";
    inline HWND g_gameWindowHwnd = nullptr;
    // Experimental client-side interface texture redirect.
    // When enabled, known Game.exe interface texture strings that still point to
    // .tga/.jpg are rewritten to .png at runtime. This makes it easy to test a
    // PNG-based interface folder without modifying the original executable on disk.
    // Disable this flag to revert the whole redirect block quickly.
    constexpr bool kEnableExperimentalInterfacePngRedirect = true;
    constexpr char kPngExtension[] = ".png";
    constexpr char kGameWindowClassName[] = "GAME";

    // EP5 interface port constants.
    float g_ep5StatsSpOffset = 72.0f;
    float g_ep5StatsMpOffset = 55.0f;
    float g_ep5StatsHpOffset = 40.0f;
    float g_ep5Float256 = 256.0f;
    float g_ep5Float15 = 15.0f;
    float g_ep5Float26 = 26.0f;
    float g_ep5EnemyBarX = 37.0f;
    float g_ep5EnemyBarY = 41.0f;
    float g_ep5EnemyBarWidth = 100.0f;
    float g_ep5EnemyBarHeight = 14.0f;
    float g_ep5EnemyBarNameY = 56.0f;
    float g_ep5Mall2Button1 = 66.0f;
    float g_ep5Mall2Button2 = 107.0f;
    float g_ep5Mall2Button3 = 74.0f;
    float g_ep5Mall2Button4 = 63.0f;
    float g_ep5Mall2Button5 = 57.0f;
    float g_ep5Mall2Button6 = 53.0f;
    float g_ep5Mall2Button7 = 51.0f;
    float g_ep5Mall2Button8 = 49.0f;
    float g_ep5Mall2RowStep1 = 65.0f;
    float g_ep5Mall2RowStep2 = 65.0f;
    float g_ep5Mall2RowStep3 = 65.0f;
    float g_ep5Mall2RowStep4 = 65.0f;
    float g_ep5Mall2RowStep5 = 65.0f;
    float g_ep5Mall2RowStep6 = 65.0f;
    float g_ep5Mall2RowStep7 = 65.0f;
    float g_ep5Mall2RowStep8 = 65.0f;
    float g_ep5Mall2BarPosX = 763.0f;
    float g_ep5ClockA = 154.0f;
    float g_ep5ClockB = 167.0f;
    float g_ep5ClockC = 55.0f;
    float g_ep5ClockD = 144.0f;
    float g_ep5ClockF = 55.0f;
    float g_ep5ClockG = 144.0f;
    float g_ep5ClockH = 139.0f;
    float g_ep5ClockI = 95.0f;
    float g_ep5ClockJ = 50.0f;
    float g_ep5ClockK = 146.0f;
    float g_ep5ClockL = 20.0f;
    float g_ep5ClockM = 135.0f;
    float g_ep5ClockN = 10.0f;
    std::uint8_t g_viewIdEnabled = 0;
    std::uint32_t g_skipServerSelectionWindow = 0;

    struct F2
    {
        float x;
        float y;
    };

    constexpr char kEp4InterfaceDataPath[] = "data/interface";
    constexpr char kEp4ArrowFileName[] = "arrow.png";
    constexpr char kEp4LevelFormat[] = "%2d";
    constexpr char kEp4ClockFormat[] = "%b %d, %Y %I:%M %p";
    constexpr double kEp4MainStatsBarLength = 145.0;
    float g_ep4MainServerTimeX = 7.0f;
    float g_ep4MainServerTimeY = 150.0f;
    float g_ep4ClockX = 15.0f;
    float g_ep4ClockY = 163.0f;
    float g_ep4ArrowSizePlus = 16.0f;
    float g_ep4ArrowSizeMinus = -16.0f;

    const F2 kEp4MainStatsPatchXY[10] =
    {
        { 256.0f, 128.0f }, // circle
        { 120.0f,  17.0f }, // name
        {  20.0f,  14.0f }, // level
        {  17.0f,  24.0f }, // secondary circle
        {  60.0f,  40.0f }, // HP bar
        {  81.0f,  43.0f }, // HP text
        {  60.0f,  56.0f }, // MP bar
        {  81.0f,  59.0f }, // MP text
        {  60.0f,  72.0f }, // SP bar
        {  81.0f,  75.0f }, // SP text
    };

    const F2 kEp4EnemyBarPatchXY[3] =
    {
        {   0.0f,  5.0f }, // element
        {  40.0f, 37.0f }, // HP
        { 110.0f, 18.0f }, // name
    };

    const F2 kEp4MainMapButtonPatchXY[4] =
    {
        { 135.0f,  19.0f }, // plus
        { 145.0f,  43.0f }, // minus
        { 142.0f,  93.0f }, // map
        {   0.0f, 112.0f }, // invasion
    };

    const F2 kEp4MainMapClockPatchXY[1] =
    {
        { 55.0f, 142.0f },
    };

    const F2 kEp4MainBottomA[2] = { { 22.0f, 49.0f }, { 571.0f, 49.0f } };
    const float kEp4MainBottomB[3] = { 646.0f, 35.0f, 10.0f };
    const F2 kEp4MainBottom8A[2] = { { 16.0f, 52.0f }, { 446.0f, 52.0f } };
    const float kEp4MainBottom8B[3] = { 505.0f, 27.0f, 22.0f };
    const F2 kEp4MainBottom12A[2] = { { 28.0f, 108.0f }, { 715.0f, 108.0f } };
    const float kEp4MainBottom12B[3] = { 808.0f, 43.0f, 58.0f };

    using RegisterClassExAProc = ATOM(WINAPI*)(const WNDCLASSEXA*);
    using CreateWindowExAProc = HWND(WINAPI*)(
        DWORD,
        LPCSTR,
        LPCSTR,
        DWORD,
        int,
        int,
        int,
        int,
        HWND,
        HMENU,
        HINSTANCE,
        LPVOID);
    using RegisterClassExWProc = ATOM(WINAPI*)(const WNDCLASSEXW*);
    using CreateWindowExWProc = HWND(WINAPI*)(
        DWORD,
        LPCWSTR,
        LPCWSTR,
        DWORD,
        int,
        int,
        int,
        int,
        HWND,
        HMENU,
        HINSTANCE,
        LPVOID);
    using SetWindowTextAProc = BOOL(WINAPI*)(HWND, LPCSTR);
    using SendMessageAProc = LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM);

    RegisterClassExAProc g_originalRegisterClassExA = nullptr;
    CreateWindowExAProc g_originalCreateWindowExA = nullptr;
    RegisterClassExWProc g_originalRegisterClassExW = nullptr;
    CreateWindowExWProc g_originalCreateWindowExW = nullptr;
    SetWindowTextAProc g_originalSetWindowTextA = nullptr;
    SendMessageAProc g_originalSendMessageA = nullptr;

    std::string to_lower_copy(std::string value)
    {
        for (auto& c : value)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        return value;
    }

    std::string get_client_config_ini_path()
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

    void load_id_view_setting()
    {
        char buffer[16]{};
        auto iniPath = get_client_config_ini_path();
        GetPrivateProfileStringA("ADVANCED", "IDVIEW", "OFF", buffer, static_cast<DWORD>(sizeof(buffer)), iniPath.c_str());
        g_viewIdEnabled = _stricmp(buffer, "ON") == 0 ? 1 : 0;
    }

    bool load_skip_updater_setting()
    {
        auto iniPath = get_client_config_ini_path();
        return GetPrivateProfileIntA("ADVANCED", "SKIPUPDATER", 0, iniPath.c_str()) != 0;
    }

    std::string trim_copy(std::string value)
    {
        auto first = value.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return {};

        auto last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    std::string load_server_ip_setting()
    {
        char buffer[64]{};
        auto iniPath = get_client_config_ini_path();
        GetPrivateProfileStringA("ADVANCED", "IP", "", buffer, static_cast<DWORD>(sizeof(buffer)), iniPath.c_str());

        auto ip = trim_copy(buffer);
        if (ip.empty())
            return "127.0.0.1";

        if (ip.size() >= sizeof(buffer))
            ip.resize(sizeof(buffer) - 1);

        return ip;
    }

    void patch_login_server_ip_from_config()
    {
        auto ip = load_server_ip_setting();
        auto copyLength = ip.size();
        if (ip == "127.0.0.1")
            return;

        if (copyLength > 63)
            copyLength = 63;

        char serverIp[64]{};
        std::memcpy(serverIp, ip.data(), copyLength);

        // Login/server IP override via CONFIG.ini.
        // ADVANCED -> IP=custom.address rewrites the stock global "127.0.0.1"
        // string. Empty or missing IP leaves the executable default untouched.
        util::write_memory((void*)0x746688, serverIp, copyLength + 1);
    }

    using GetCommandLineAProc = LPSTR(WINAPI*)();
    GetCommandLineAProc g_originalGetCommandLineA = nullptr;
    std::string g_skipUpdaterCommandLine;

    bool command_line_has_start_game(const char* commandLine)
    {
        if (!commandLine)
            return false;

        return to_lower_copy(commandLine).find("start game") != std::string::npos;
    }

    LPSTR WINAPI hooked_get_command_line_a()
    {
        auto commandLine = g_originalGetCommandLineA
            ? g_originalGetCommandLineA()
            : GetCommandLineA();

        if (!load_skip_updater_setting() || command_line_has_start_game(commandLine))
            return commandLine;

        // The stock client uses the "start game" command-line token to know it
        // was launched by Updater.exe. Add it at runtime instead of editing the
        // executable or forcing updater-state flags that can change too late.
        g_skipUpdaterCommandLine = commandLine ? commandLine : "";
        g_skipUpdaterCommandLine += " start game";
        return g_skipUpdaterCommandLine.data();
    }

    void patch_get_command_line_for_skip_updater()
    {
        auto importSlot = reinterpret_cast<GetCommandLineAProc*>(0x746160);
        if (!g_originalGetCommandLineA)
            g_originalGetCommandLineA = *importSlot;

        auto hook = &hooked_get_command_line_a;
        util::write_memory(importSlot, &hook, sizeof(hook));
    }

    bool ends_with_ignore_case(const std::string& value, const char* suffix)
    {
        auto suffixLength = std::strlen(suffix);
        if (value.size() < suffixLength)
            return false;

        auto tail = value.substr(value.size() - suffixLength);
        return to_lower_copy(tail) == suffix;
    }

    std::filesystem::path get_client_data_sah_path()
    {
        char moduleFileName[MAX_PATH]{};
        if (!GetModuleFileNameA(nullptr, moduleFileName, MAX_PATH))
            return {};

        std::filesystem::path path(moduleFileName);
        path = path.parent_path();
        path /= "data.sah";
        return path;
    }

    struct InterfacePngRedirectData
    {
        std::unordered_set<std::string> fileNames;
        std::unordered_set<std::string> relativePaths;
    };

    void add_png_asset_redirect_entries(
        InterfacePngRedirectData& data,
        const std::string& relativePathWithoutExtension)
    {
        auto normalizedPath = relativePathWithoutExtension;
        for (auto& c : normalizedPath)
        {
            if (c == '/')
                c = '\\';
        }

        auto fileNamePos = normalizedPath.find_last_of("\\/");
        auto fileNameWithoutExtension = fileNamePos == std::string::npos
            ? normalizedPath
            : normalizedPath.substr(fileNamePos + 1);

        for (const auto* extension : { ".tga", ".jpg" })
        {
            data.fileNames.insert(fileNameWithoutExtension + extension);
            data.relativePaths.insert(normalizedPath + extension);
        }
    }

    bool set_contains_suffix(
        const std::unordered_set<std::string>& values,
        const std::string& candidate)
    {
        for (const auto& value : values)
        {
            if (candidate.size() < value.size())
                continue;

            if (candidate.compare(candidate.size() - value.size(), value.size(), value) == 0)
                return true;
        }

        return false;
    }

    InterfacePngRedirectData collect_interface_png_redirect_data()
    {
        InterfacePngRedirectData data{};
        auto dataSahPath = get_client_data_sah_path();
        if (dataSahPath.empty() || !std::filesystem::exists(dataSahPath))
            return data;

        std::ifstream stream(dataSahPath, std::ios::binary);
        if (!stream)
            return data;

        auto is_section_token = [](const std::string& token) -> bool
        {
            constexpr const char* relevantSections[]
            {
                "interface",
                "chaoticsquare",
                "worldname",
                "npcface",
                "minimap",
                "gamecard",
                "tarocard",
                "icon",
            };

            for (const auto* section : relevantSections)
            {
                if (token == section)
                    return true;
            }

            return false;
        };

        std::string currentSection;
        std::string token;
        char ch{};
        while (stream.get(ch))
        {
            auto uch = static_cast<unsigned char>(ch);
            if (uch >= 0x20 && uch <= 0x7E)
            {
                token.push_back(static_cast<char>(uch));
                continue;
            }

            if (token.size() >= 3)
            {
                auto lowerToken = to_lower_copy(token);
                if (is_section_token(lowerToken))
                {
                    currentSection = lowerToken;
                }
                else if (currentSection == "chaoticsquare" && lowerToken == "text")
                {
                    currentSection = "chaoticsquare\\text";
                }
                else if (lowerToken.ends_with(".png"))
                {
                    auto stem = lowerToken.substr(0, lowerToken.size() - 4);
                    if (currentSection == "icon")
                    {
                        // Icon handling stays disabled in the redirect for now.
                    }
                    else if (currentSection == "interface")
                    {
                        add_png_asset_redirect_entries(data, stem);
                    }
                    else if (!currentSection.empty())
                    {
                        add_png_asset_redirect_entries(data, currentSection + "\\" + stem);
                    }
                }
            }

            token.clear();
        }

        // These two root interface names are present in the historical lists we
        // built during migration work, but they do not appear cleanly in the
        // client's data.sah string stream. Keep them as explicit root entries so
        // the PNG redirect still covers them.
        add_png_asset_redirect_entries(data, "ch_option_graphic2");
        add_png_asset_redirect_entries(data, "window_button");

        return data;
    }

    bool is_known_interface_group_string(const std::string& lowerString)
    {
        constexpr const char* groupMarkers[]
        {
            "worldname\\",
            "npcface\\",
            "minimap\\",
            "minimap_",
            "gamecard\\",
            "tarocard\\",
            "wm_",
            "wm_button",
            "random_loading",
            "loading",
            "main_bottom",
            "main_bottom_button",
            "main_stats_",
            "chaoticsquare\\",
        };

        for (const auto* marker : groupMarkers)
        {
            if (lowerString.find(marker) != std::string::npos)
                return true;
        }

        return false;
    }

    bool is_known_interface_format_string(const std::string& lowerString)
    {
        constexpr const char* knownFormats[]
        {
            "%d.tga",
            "%d.jpg",
            "%02d.tga",
            "%02d.jpg",
            "%d_%02d.tga",
            "%d_%02d.jpg",
            "minimap_%d.tga",
            "minimap_%d.jpg",
            "minimap_%02d.tga",
            "minimap_%02d.jpg",
            "wm_e%d.tga",
            "wm_e%d.jpg",
            "wm_f%d.tga",
            "wm_f%d.jpg",
            "wm_%d.tga",
            "wm_%d.jpg",
            "wm_%c%d.tga",
            "wm_%c%d.jpg",
            "wm_%s.tga",
            "wm_%s.jpg",
            "%s.tga",
            "%s.jpg",
            "%s%d.tga",
            "%s%d.jpg",
            "%s0%d.tga",
            "%s0%d.jpg",
            "%s00%d.tga",
            "%s00%d.jpg",
            "%s000%d.tga",
            "%s000%d.jpg",
            "random_loading%02d.jpg",
            "game_card%d.tga",
            "tarocard%d.tga",
            "war_%d.tga",
        };

        for (const auto* format : knownFormats)
        {
            if (lowerString == format)
                return true;
        }

        return false;
    }

    bool should_redirect_interface_string(
        const std::string& lowerString,
        const InterfacePngRedirectData& redirectData)
    {
        if (!ends_with_ignore_case(lowerString, ".tga") && !ends_with_ignore_case(lowerString, ".jpg"))
            return false;

        // Keep icon assets on their original formats for now.
        if (lowerString.find("icon\\") != std::string::npos
            || lowerString.find("icon/") != std::string::npos
            || lowerString.find("\\icon\\") != std::string::npos
            || lowerString.find("/icon/") != std::string::npos)
            return false;

        if (is_known_interface_format_string(lowerString))
            return true;

        if (redirectData.fileNames.contains(lowerString))
            return true;

        if (set_contains_suffix(redirectData.fileNames, lowerString))
            return true;

        if (redirectData.relativePaths.contains(lowerString))
            return true;

        if (set_contains_suffix(redirectData.relativePaths, lowerString))
            return true;

        auto lastSlash = lowerString.find_last_of("\\/");
        if (lastSlash != std::string::npos)
        {
            auto fileName = lowerString.substr(lastSlash + 1);
            if (redirectData.fileNames.contains(fileName))
                return true;

            if (set_contains_suffix(redirectData.fileNames, fileName))
                return true;
        }

        if ((lowerString.find("data/interface/") != std::string::npos
            || lowerString.find("data\\interface\\") != std::string::npos)
            && lowerString.find("data/interface/icon/") == std::string::npos
            && lowerString.find("data\\interface\\icon\\") == std::string::npos)
            return true;

        return is_known_interface_group_string(lowerString);
    }

    void patch_module_interface_texture_extensions_to_png()
    {
        auto redirectData = collect_interface_png_redirect_data();
        if (redirectData.fileNames.empty() && redirectData.relativePaths.empty())
            return;

        auto moduleBase = reinterpret_cast<std::uint8_t*>(GetModuleHandleA(nullptr));
        if (!moduleBase)
            return;

        auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(moduleBase);
        if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
            return;

        auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(moduleBase + dosHeader->e_lfanew);
        if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
            return;

        auto imageSize = static_cast<std::size_t>(ntHeaders->OptionalHeader.SizeOfImage);
        for (std::size_t i = 0; i < imageSize; ++i)
        {
            auto current = moduleBase[i];
            if (current < 0x20 || current > 0x7E)
                continue;

            auto start = i;
            while (i < imageSize && moduleBase[i] >= 0x20 && moduleBase[i] <= 0x7E)
                ++i;

            if (i >= imageSize || moduleBase[i] != 0)
                continue;

            auto length = i - start;
            if (length < 5)
                continue;

            std::string candidate(reinterpret_cast<char*>(moduleBase + start), length);
            auto lowerCandidate = to_lower_copy(candidate);
            if (!should_redirect_interface_string(lowerCandidate, redirectData))
                continue;

            if (ends_with_ignore_case(lowerCandidate, ".tga")
                || ends_with_ignore_case(lowerCandidate, ".jpg"))
            {
                // Only rewrite the file extension in-place.
                // The original Game.exe string remains the same path/name, but now
                // points to a .png file instead of .tga/.jpg.
                constexpr char pngExtension[] = ".png";
                util::write_memory(moduleBase + start + length - 4, pngExtension, 4);
            }
        }
    }

    void patch_screenshot_extensions_to_png()
    {
        // Screenshot filename templates used by Game.exe.
        // This does not change the screenshot folder logic; it only rewrites the
        // built-in .jpg/.JPG filename templates to .png so newly generated
        // screenshots are saved with a PNG extension instead.
        //
        // If the client ever needs to go back to JPEG screenshots, remove or
        // comment out this helper call in hook::patch().
        util::write_memory((void*)0x75667C, kPngExtension, 4);
        util::write_memory((void*)0x756688, kPngExtension, 4);
        util::write_memory((void*)0x756694, kPngExtension, 4);
        util::write_memory((void*)0x7566A0, kPngExtension, 4);
        util::write_memory((void*)0x7566AC, kPngExtension, 4);
        util::write_memory((void*)0x7566B8, kPngExtension, 4);
        util::write_memory((void*)0x7566C4, kPngExtension, 4);
        util::write_memory((void*)0x7566D0, kPngExtension, 4);
    }

    bool is_string_game_window_class(LPCSTR value)
    {
        return value
            && HIWORD(reinterpret_cast<std::uintptr_t>(value)) != 0
            && lstrcmpiA(value, kGameWindowClassName) == 0;
    }

    bool convert_ansi_to_wide(LPCSTR source, wchar_t* destination, int destinationCount)
    {
        if (!source || HIWORD(reinterpret_cast<std::uintptr_t>(source)) == 0 || !destination || destinationCount <= 0)
            return false;

        auto count = MultiByteToWideChar(CP_ACP, 0, source, -1, destination, destinationCount);
        if (count <= 0)
        {
            destination[0] = L'\0';
            return false;
        }

        destination[destinationCount - 1] = L'\0';
        return true;
    }

    bool convert_title_to_wide(LPCSTR source, wchar_t* destination, int destinationCount)
    {
        if (!source || HIWORD(reinterpret_cast<std::uintptr_t>(source)) == 0 || !destination || destinationCount <= 0)
            return false;

        // Some Shaiya builds pass a UTF-16LE title pointer through an ANSI API
        // slot. Read as UTF-16 when the byte pattern is S\0h\0...; otherwise
        // the ANSI conversion stops at the first null byte and the title
        // collapses to just "S".
        __try
        {
            if (source[0] != '\0' && source[1] == '\0' && source[2] != '\0' && source[3] == '\0')
            {
                lstrcpynW(destination, reinterpret_cast<LPCWSTR>(source), destinationCount);
                destination[destinationCount - 1] = L'\0';
                return destination[0] != L'\0';
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

        // Window titles coming from external/custom code may already be UTF-8,
        // while the vanilla title is ANSI/ACP. Try strict UTF-8 first and fall
        // back to ACP so the fix remains safe for the stock "Shaiya" title.
        auto count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, source, -1, destination, destinationCount);
        if (count <= 0)
            count = MultiByteToWideChar(CP_ACP, 0, source, -1, destination, destinationCount);

        if (count <= 0)
        {
            destination[0] = L'\0';
            return false;
        }

        destination[destinationCount - 1] = L'\0';
        return true;
    }

    bool is_string_game_window_class(LPCWSTR value)
    {
        return value
            && HIWORD(reinterpret_cast<std::uintptr_t>(value)) != 0
            && lstrcmpiW(value, L"GAME") == 0;
    }

    bool is_game_window_hwnd(HWND hwnd)
    {
        if (!hwnd || !IsWindow(hwnd))
            return false;

        if (g_gameWindowHwnd && hwnd == g_gameWindowHwnd)
            return true;

        if (!IsWindowUnicode(hwnd))
            return false;

        wchar_t className[64]{};
        if (!GetClassNameW(hwnd, className, _countof(className)))
            return false;

        return is_string_game_window_class(className);
    }

    BOOL WINAPI set_window_text_a_utf8_safe(HWND hwnd, LPCSTR text)
    {
        if (!kEnableUnicodeWindowTitleFix || !is_game_window_hwnd(hwnd))
            return g_originalSetWindowTextA(hwnd, text);

        wchar_t wideTitle[256]{};
        if (!convert_title_to_wide(text, wideTitle, _countof(wideTitle)))
            return g_originalSetWindowTextA(hwnd, text);

        return SetWindowTextW(hwnd, wideTitle);
    }

    void force_game_window_title(HWND hwnd)
    {
        if (!kEnableUnicodeWindowTitleFix || !is_game_window_hwnd(hwnd))
            return;

        SetWindowTextW(hwnd, kDefaultGameWindowTitle);
    }

    LRESULT WINAPI send_message_a_utf8_safe(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (!kEnableUnicodeWindowTitleFix || message != WM_SETTEXT || !is_game_window_hwnd(hwnd))
            return g_originalSendMessageA(hwnd, message, wParam, lParam);

        wchar_t wideTitle[256]{};
        if (!convert_title_to_wide(reinterpret_cast<LPCSTR>(lParam), wideTitle, _countof(wideTitle)))
            lstrcpynW(wideTitle, kDefaultGameWindowTitle, _countof(wideTitle));

        return SendMessageW(hwnd, WM_SETTEXT, wParam, reinterpret_cast<LPARAM>(wideTitle));
    }

    ATOM WINAPI register_class_ex_a_utf8_safe(const WNDCLASSEXA* wndClass)
    {
        if (!wndClass || !is_string_game_window_class(wndClass->lpszClassName))
            return g_originalRegisterClassExA(wndClass);

        wchar_t className[64]{};
        wchar_t menuName[260]{};
        convert_ansi_to_wide(wndClass->lpszClassName, className, _countof(className));

        WNDCLASSEXW wideClass{};
        wideClass.cbSize = sizeof(wideClass);
        wideClass.style = wndClass->style;
        wideClass.lpfnWndProc = wndClass->lpfnWndProc;
        wideClass.cbClsExtra = wndClass->cbClsExtra;
        wideClass.cbWndExtra = wndClass->cbWndExtra;
        wideClass.hInstance = wndClass->hInstance;
        wideClass.hIcon = wndClass->hIcon;
        wideClass.hCursor = wndClass->hCursor;
        wideClass.hbrBackground = wndClass->hbrBackground;
        wideClass.lpszClassName = className;
        wideClass.hIconSm = wndClass->hIconSm;

        if (wndClass->lpszMenuName && HIWORD(reinterpret_cast<std::uintptr_t>(wndClass->lpszMenuName)) == 0)
            wideClass.lpszMenuName = MAKEINTRESOURCEW(reinterpret_cast<std::uintptr_t>(wndClass->lpszMenuName) & 0xFFFF);
        else if (convert_ansi_to_wide(wndClass->lpszMenuName, menuName, _countof(menuName)))
            wideClass.lpszMenuName = menuName;

        return RegisterClassExW(&wideClass);
    }

    HWND WINAPI create_window_ex_a_utf8_safe(
        DWORD exStyle,
        LPCSTR className,
        LPCSTR windowName,
        DWORD style,
        int x,
        int y,
        int width,
        int height,
        HWND parent,
        HMENU menu,
        HINSTANCE instance,
        LPVOID param)
    {
        if (!is_string_game_window_class(className))
        {
            return g_originalCreateWindowExA(
                exStyle,
                className,
                windowName,
                style,
                x,
                y,
                width,
                height,
                parent,
                menu,
                instance,
                param);
        }

        wchar_t wideClassName[64]{};
        wchar_t wideWindowName[128]{};
        convert_ansi_to_wide(className, wideClassName, _countof(wideClassName));
        convert_title_to_wide(windowName, wideWindowName, _countof(wideWindowName));

        auto hwnd = CreateWindowExW(
            exStyle,
            wideClassName,
            wideWindowName,
            style,
            x,
            y,
            width,
            height,
            parent,
            menu,
            instance,
            param);

        if (hwnd)
            g_gameWindowHwnd = hwnd;
        force_game_window_title(hwnd);
        return hwnd;
    }

    ATOM WINAPI register_class_ex_w_utf8_safe(const WNDCLASSEXW* wndClass)
    {
        return g_originalRegisterClassExW(wndClass);
    }

    HWND WINAPI create_window_ex_w_utf8_safe(
        DWORD exStyle,
        LPCWSTR className,
        LPCWSTR windowName,
        DWORD style,
        int x,
        int y,
        int width,
        int height,
        HWND parent,
        HMENU menu,
        HINSTANCE instance,
        LPVOID param)
    {
        auto hwnd = g_originalCreateWindowExW(
            exStyle,
            className,
            windowName,
            style,
            x,
            y,
            width,
            height,
            parent,
            menu,
            instance,
            param);

        if (is_string_game_window_class(className))
        {
            if (hwnd)
                g_gameWindowHwnd = hwnd;
            force_game_window_title(hwnd);
        }

        return hwnd;
    }

    void patch_game_window_ansi_creation_to_unicode()
    {
        auto registerClassExAImport = reinterpret_cast<RegisterClassExAProc*>(0x746450);
        auto createWindowExAImport = reinterpret_cast<CreateWindowExAProc*>(0x74644C);
        auto registerClassExWImport = reinterpret_cast<RegisterClassExWProc*>(0x746470);
        auto createWindowExWImport = reinterpret_cast<CreateWindowExWProc*>(0x74646C);
        auto setWindowTextAImport = reinterpret_cast<SetWindowTextAProc*>(0x746494);
        auto sendMessageAImport = reinterpret_cast<SendMessageAProc*>(0x746504);

        if (!g_originalRegisterClassExA)
            g_originalRegisterClassExA = *registerClassExAImport;
        if (!g_originalCreateWindowExA)
            g_originalCreateWindowExA = *createWindowExAImport;
        if (!g_originalRegisterClassExW)
            g_originalRegisterClassExW = *registerClassExWImport;
        if (!g_originalCreateWindowExW)
            g_originalCreateWindowExW = *createWindowExWImport;
        if (!g_originalSetWindowTextA)
            g_originalSetWindowTextA = *setWindowTextAImport;
        if (!g_originalSendMessageA)
            g_originalSendMessageA = *sendMessageAImport;

        auto registerClassExAReplacement = register_class_ex_a_utf8_safe;
        auto createWindowExAReplacement = create_window_ex_a_utf8_safe;
        auto registerClassExWReplacement = register_class_ex_w_utf8_safe;
        auto createWindowExWReplacement = create_window_ex_w_utf8_safe;
        auto setWindowTextAReplacement = set_window_text_a_utf8_safe;
        auto sendMessageAReplacement = send_message_a_utf8_safe;

        // Keep this scoped to the main "GAME" window in the wrappers above.
        // Other ANSI helper windows still flow through the original imports.
        util::write_memory(registerClassExAImport, &registerClassExAReplacement, sizeof(registerClassExAReplacement));
        util::write_memory(createWindowExAImport, &createWindowExAReplacement, sizeof(createWindowExAReplacement));
        util::write_memory(registerClassExWImport, &registerClassExWReplacement, sizeof(registerClassExWReplacement));
        util::write_memory(createWindowExWImport, &createWindowExWReplacement, sizeof(createWindowExWReplacement));
        // Window title fix for the Unicode GAME HWND. Some legacy paths still
        // call SetWindowTextA after the HWND was upgraded to Unicode, which can
        // collapse "Shaiya" to "S". Convert only GAME window titles to UTF-16.
        util::write_memory(setWindowTextAImport, &setWindowTextAReplacement, sizeof(setWindowTextAReplacement));
        util::write_memory(sendMessageAImport, &sendMessageAReplacement, sizeof(sendMessageAReplacement));

        // If the class was registered through the ANSI route before this patch
        // ran, remove it while no game HWND exists so the next registration can
        // be Unicode. Failure is harmless when the class is not registered yet.
        auto instance = GetModuleHandleA(nullptr);
        UnregisterClassA(kGameWindowClassName, instance);
    }

    bool append_utf8_textbox_bytes(void* textBox, const char* utf8, int count)
    {
        if (!textBox || !utf8 || count <= 0)
            return false;

        auto base = reinterpret_cast<std::uint8_t*>(textBox);
        auto maxTextLength = *reinterpret_cast<std::uint32_t*>(base + 0x30);
        auto textSize = *reinterpret_cast<std::uint32_t*>(base + 0xC0);
        auto textCapacity = *reinterpret_cast<std::uint32_t*>(base + 0xC4);
        if (textSize >= 2048 || textCapacity >= 0x100000)
            return false;

        auto textObject = base + 0xAC;
        auto text = textCapacity >= 0x10
            ? *reinterpret_cast<const char**>(textObject + 0x04)
            : reinterpret_cast<const char*>(textObject + 0x04);
        if (!text)
            return false;

        auto nextSize = textSize + static_cast<std::uint32_t>(count);
        if (nextSize >= 2048 || (maxTextLength && nextSize > maxTextLength))
            return false;

        char merged[2048]{};
        if (textSize)
            std::memcpy(merged, text, textSize);
        std::memcpy(merged + textSize, utf8, count);

        using StringAssign = void*(__thiscall*)(void*, const char*, std::uint32_t);
        auto assignString = reinterpret_cast<StringAssign>(0x405670);

        // Avoid the client's ANSI-per-character path for Unicode input. Feed the
        // completed UTF-8 byte sequence into the textbox's narrow string at once;
        // then mirror it into the wide string for rendering in sync_textbox_utf8_display().
        assignString(textObject, merged, nextSize);
        sync_textbox_utf8_display(textBox);
        return true;
    }

    void __stdcall insert_utf8_textbox_char(void* textBox, int wParam, int /*lParam*/)
    {
        append_utf8_textbox_wchar(textBox, static_cast<wchar_t>(wParam));
    }


}

bool append_utf8_textbox_wchar(void* textBox, wchar_t wideChar)
{
    char utf8[8]{};
    auto count = WideCharToMultiByte(kUtf8CodePage, 0, &wideChar, 1, utf8, static_cast<int>(sizeof(utf8)), nullptr, nullptr);
    if (count <= 0)
        return false;

    return append_utf8_textbox_bytes(textBox, utf8, count);
}

bool backspace_utf8_textbox_char(void* textBox)
{
    if (!textBox)
        return false;

    auto base = reinterpret_cast<std::uint8_t*>(textBox);
    auto textSize = *reinterpret_cast<std::uint32_t*>(base + 0xC0);
    auto textCapacity = *reinterpret_cast<std::uint32_t*>(base + 0xC4);
    if (!textSize || textSize >= 2048 || textCapacity >= 0x100000)
        return false;

    auto textObject = base + 0xAC;
    auto text = textCapacity >= 0x10
        ? *reinterpret_cast<const char**>(textObject + 0x04)
        : reinterpret_cast<const char*>(textObject + 0x04);
    if (!text)
        return false;

    auto last = static_cast<unsigned char>(text[textSize - 1]);
    if (last < 0x80)
        return false;

    auto nextSize = textSize - 1;
    while (nextSize > 0 && (static_cast<unsigned char>(text[nextSize]) & 0xC0) == 0x80)
        --nextSize;

    using StringAssign = void*(__thiscall*)(void*, const char*, std::uint32_t);
    auto assignString = reinterpret_cast<StringAssign>(0x405670);

    // Unikey/IME can revise a composed Vietnamese glyph by sending backspace
    // before the final WM_CHAR. The stock ANSI handler removes only one byte,
    // leaving invalid UTF-8 that renders as the white replacement-glyph box.
    assignString(textObject, text, nextSize);
    sync_textbox_utf8_display(textBox);
    return true;
}

void sync_textbox_utf8_display(void* textBox)
{
    if (!textBox)
        return;

    auto base = reinterpret_cast<std::uint8_t*>(textBox);
    auto textSize = *reinterpret_cast<std::uint32_t*>(base + 0xC0);
    auto textCapacity = *reinterpret_cast<std::uint32_t*>(base + 0xC4);
    if (textSize >= 2048 || textCapacity >= 0x100000)
        return;

    auto textObject = base + 0xAC;
    auto text = textCapacity >= 0x10
        ? *reinterpret_cast<const char**>(textObject + 0x04)
        : reinterpret_cast<const char*>(textObject + 0x04);
    if (!text)
        return;

    wchar_t wideText[1024]{};
    auto wideCount = MultiByteToWideChar(kUtf8CodePage, 0, text, static_cast<int>(textSize), wideText, static_cast<int>(std::size(wideText) - 1));
    if (wideCount <= 0)
        return;

    wideText[wideCount] = L'\0';

    using WideTextAssign = void(__thiscall*)(void*, const wchar_t*, int);
    using TextboxRefresh = void(__thiscall*)(void*, int);
    auto assignWideText = reinterpret_cast<WideTextAssign>(0x424FA0);
    auto refreshTextbox = reinterpret_cast<TextboxRefresh>(0x565470);

    // The client keeps both a narrow string (+0xAC) and a wide string (+0x90).
    // The normal key handler updates the narrow side; this mirrors it into the
    // wide side as UTF-8 so chat can render characters without enabling the
    // global Vietnam codepage branch.
    assignWideText(base + 0x90, wideText, wideCount);
    refreshTextbox(textBox, 0);
}

unsigned n0x41F9ED = 0x41F9ED;
unsigned u0x41F9C9 = 0x41F9C9;
void __declspec(naked) naked_0x41F9C0()
{
    __asm
    {
        // character->wings
        cmp dword ptr[esi+0x434],0x0
        jne _0x41F9ED

        // original
        mov edx,[esi+0x10]
        fld dword ptr ds:[0x748160]
        jmp u0x41F9C9

        _0x41F9ED:
        jmp n0x41F9ED
    }
}

unsigned u0x41BB40 = 0x41BB40;
unsigned u0x4110F0 = 0x4110F0;
unsigned u0x41F5E6 = 0x41F5E6;
unsigned u0x41E2CD = 0x41E2CD;
void __declspec(naked) naked_0x41E2BB()
{
    __asm
    {
        // original
        mov ecx,esi
        call u0x41BB40
        test eax,eax
        jne original
        // continue
        jmp u0x41E2CD

        original:
        mov ecx,esi
        call u0x4110F0
        // exit
        jmp u0x41F5E6
    }
}

void __declspec(naked) naked_0x4E6D76()
{
    __asm
    {
        cmp byte ptr ds:[g_viewIdEnabled], 1
        jne disabled
        mov al, byte ptr ds:[0x0090D1D4]
        cmp al, 1
        je originalcode
        cmp al, 2
        je originalcode
        cmp al, 3
        sete al
        ret

        disabled:
        xor al, al
        ret

        originalcode:
        mov al, 1
        ret
    }
}

unsigned u0x567703 = 0x567703;
unsigned u0x56771C = 0x56771C;
unsigned u0x56772D = 0x56772D;
unsigned u0x5648D0 = 0x5648D0;
unsigned u0x564FC6 = 0x564FC6;
void __declspec(naked) naked_0x564FC1()
{
    __asm
    {
        // Original:
        //   call 005648D0
        //
        // Keep the client's global LoginVersion untouched, but make the live
        // textbox use the wide-text path and decode composition text as UTF-8.
        // This scopes Unicode support to chat/input instead of enabling the
        // full Vietnam codepage branch, which also changes unrelated client
        // behavior and blocks non-Vietnam Windows keyboard layouts.
        call u0x5648D0
        mov word ptr [esi+0x1628], 11h
        mov ax, 0FDE9h
        jmp u0x564FC6
    }
}

void __declspec(naked) naked_0x5676FE()
{
    __asm
    {
        // Original branch only used the wide char handler for LoginVersion=3.
        // We are intentionally not using that global codepage mode now; instead,
        // convert non-ASCII WM_CHAR input to UTF-8 bytes and feed the normal
        // textbox handler so ES/US/Vietnam layouts all share one path.
        mov eax, [esp+0x18]
        cmp eax, 0x7F
        jbe normal_handler

        pushad
        mov eax, [esp+0x3C]
        mov edx, [esp+0x38]
        push eax
        push edx
        push esi
        call insert_utf8_textbox_char
        popad
        jmp u0x56772D

        normal_handler:
        jmp u0x56771C
    }
}

unsigned u0x49354A = 0x49354A;
void __declspec(naked) naked_0x493543()
{
    __asm
    {
        // original
        fstp dword ptr [esp+0x4]
        fstp dword ptr [esp]

        // EP5-style main bottom strip layout for main_bottom_button8
        mov dword ptr [esi+0x3000], 43FC8000h // 505.0f
        mov dword ptr [esi+0x3004], 41D80000h // 27.0f
        mov dword ptr [esi+0x3008], 41B00000h // 22.0f
        jmp u0x49354A
    }
}

unsigned u0x493CA3 = 0x493CA3;
void __declspec(naked) naked_0x493C9A()
{
    __asm
    {
        // original
        fxch st(1)
        fstp dword ptr [esp+0x4]
        fstp dword ptr [esp]

        // EP5-style main bottom strip layout for main_bottom_button
        mov dword ptr [esi+0x304C], 44218000h // 646.0f
        mov dword ptr [esi+0x3050], 420C0000h // 35.0f
        mov dword ptr [esi+0x3054], 41200000h // 10.0f
        jmp u0x493CA3
    }
}

unsigned u0x49440D = 0x49440D;
void __declspec(naked) naked_0x494406()
{
    __asm
    {
        // original
        fstp dword ptr [esp+0x4]
        fstp dword ptr [esp]

        // EP5-style main bottom strip layout for main_bottom_button12
        mov dword ptr [esi+0x3094], 444A0000h // 808.0f
        mov dword ptr [esi+0x3098], 422C0000h // 43.0f
        mov dword ptr [esi+0x309C], 42600000h // 56.0f
        jmp u0x49440D
    }
}

// EP4 UI support.
// Ported as a single visual layout block, intentionally excluding inventory
// changes and the server-time format override. The existing CONFIG/clock text
// format stays in control while this block moves the surrounding HUD pieces.
unsigned u0x57B560 = 0x57B560;
unsigned u0x631BE0 = 0x631BE0;
unsigned u0x532024 = 0x532024;
void __declspec(naked) naked_ep4_main_stats()
{
    __asm
    {
        pushad
        lea edi,[esi+0xA4]
        lea esi,kEp4MainStatsPatchXY
        mov ecx,20
        cld
        rep movsd
        popad
        fld dword ptr [esi+0xA8]
        jmp u0x532024
    }
}

unsigned u0x53234B = 0x53234B;
void __declspec(naked) naked_ep4_main_stats_bar_hp()
{
    __asm
    {
        fmul qword ptr [kEp4MainStatsBarLength]
        jmp u0x53234B
    }
}

unsigned u0x532487 = 0x532487;
void __declspec(naked) naked_ep4_main_stats_bar_mp()
{
    __asm
    {
        fmul qword ptr [kEp4MainStatsBarLength]
        jmp u0x532487
    }
}

unsigned u0x5325CC = 0x5325CC;
void __declspec(naked) naked_ep4_main_stats_bar_sp()
{
    __asm
    {
        fmul qword ptr [kEp4MainStatsBarLength]
        jmp u0x5325CC
    }
}

unsigned u0x5328A4 = 0x5328A4;
void __declspec(naked) naked_ep4_main_stats_level()
{
    __asm
    {
        push offset kEp4LevelFormat
        jmp u0x5328A4
    }
}

unsigned u0x5350EB = 0x5350EB;
void __declspec(naked) naked_ep4_enemy_bar()
{
    __asm
    {
        pushad
        lea edi,[esi+0x17C]
        lea esi,kEp4EnemyBarPatchXY
        mov ecx,6
        cld
        rep movsd
        popad
        fld dword ptr [esi+0x17C]
        jmp u0x5350EB
    }
}

unsigned u0x532BCD = 0x532BCD;
void __declspec(naked) naked_ep4_enemy_bar_bg()
{
    __asm
    {
        mov dword ptr [esi+0xC],187
        mov dword ptr [esi+0x10],55
        jmp u0x532BCD
    }
}

unsigned u0x534F2E = 0x534F2E;
void __declspec(naked) naked_ep4_enemy_bar_buff()
{
    __asm
    {
        add ecx,10
        push ecx
        push edx
        push 0xFFFFFFFF
        lea ecx,[esi+0xB4]
        jmp u0x534F2E
    }
}

unsigned u0x534F4D = 0x534F4D;
void __declspec(naked) naked_ep4_enemy_bar_debuff()
{
    __asm
    {
        push 16
        push 16
        add eax,10
        push eax
        jmp u0x534F4D
    }
}

unsigned u0x534F7C = 0x534F7C;
unsigned u0x534F9D = 0x534F9D;
void __declspec(naked) naked_ep4_enemy_bar_buff_mouse_over()
{
    __asm
    {
        mov eax,[0x7C3C0C]
        mov eax,[eax]
        mov ecx,[esp+0xC]
        cmp eax,ecx
        jl exit_code
        lea edx,[ecx+0x10]
        cmp eax,edx
        jg exit_code

        mov eax,[0x7C3C10]
        mov eax,[eax]
        mov edx,[esp+0x24]
        add edx,10
        cmp eax,edx
        jl exit_code
        lea edx,[edx+0x10]
        cmp eax,edx
        jg exit_code

        jmp u0x534F7C

    exit_code:
        jmp u0x534F9D
    }
}

unsigned u0x4DE68B = 0x4DE68B;
void __declspec(naked) naked_ep4_main_map_button()
{
    __asm
    {
        pushad
        mov ebp,esi

        lea edi,[ebp+0x242C]
        lea esi,kEp4MainMapButtonPatchXY
        mov ecx,8
        cld
        rep movsd
        add edi,8

        lea esi,kEp4MainMapClockPatchXY
        mov ecx,2
        rep movsd

        popad
        fld dword ptr [esi+0x2430]
        jmp u0x4DE68B
    }
}

unsigned u0x4DF4B4 = 0x4DF4B4;
void __declspec(naked) naked_ep4_main_map_bg()
{
    __asm
    {
        mov dword ptr [esi+0xC],180
        mov dword ptr [esi+0x10],160
        jmp u0x4DF4B4
    }
}

unsigned u0x4E125A = 0x4E125A;
void __declspec(naked) naked_ep4_map_clock()
{
    __asm
    {
        push offset kEp4ClockFormat
        jmp u0x4E125A
    }
}

unsigned u0x4DDF22 = 0x4DDF22;
void __declspec(naked) naked_ep4_main_map_servertime()
{
    __asm
    {
        mov [ecx+0x2424],edx
        fld dword ptr [g_ep4MainServerTimeX]
        mov edx,[esp+0x4]
        fstp dword ptr [esp]
        mov [ecx+0x2428],eax
        fld dword ptr [g_ep4MainServerTimeY]
        mov eax,[esp]
        fstp dword ptr [esp+0x4]
        mov [ecx+0x242C],edx
        fld dword ptr [g_ep4ClockX]
        mov edx,[esp+0x4]
        fstp dword ptr [esp]
        mov [ecx+0x2430],eax
        fld dword ptr [g_ep4ClockY]
        mov eax,[esp]
        fstp dword ptr [esp+0x4]
        jmp u0x4DDF22
    }
}

unsigned u0x4D9A37 = 0x4D9A37;
void __declspec(naked) naked_ep4_arrow_size_map()
{
    __asm
    {
        fld dword ptr [g_ep4ArrowSizeMinus]
        fst dword ptr [esp+0x70]
        add esi,0x1C
        fst dword ptr [esp+0x60]
        lea edx,[esp+0x54]
        fld dword ptr [g_ep4ArrowSizePlus]
        jmp u0x4D9A37
    }
}

unsigned u0x4E0573 = 0x4E0573;
void __declspec(naked) naked_ep4_arrow_size_minimap()
{
    __asm
    {
        fld dword ptr [g_ep4ArrowSizeMinus]
        fst dword ptr [esp+0x20]
        lea edx,[esp+0x74]
        fst dword ptr [esp+0x38]
        push edx
        fld dword ptr [g_ep4ArrowSizePlus]
        jmp u0x4E0573
    }
}

unsigned u0x4D7A66 = 0x4D7A66;
void __declspec(naked) naked_ep4_load_arrow_map()
{
    __asm
    {
        push 32
        push 32
        push offset kEp4ArrowFileName
        push offset kEp4InterfaceDataPath
        lea ecx,[esi+0x84]
        call u0x57B560
        jmp u0x4D7A66
    }
}

unsigned u0x4DE517 = 0x4DE517;
void __declspec(naked) naked_ep4_load_arrow_minimap()
{
    __asm
    {
        push 32
        push 32
        push offset kEp4ArrowFileName
        push offset kEp4InterfaceDataPath
        lea ecx,[esi+0x3C]
        call u0x57B560
        jmp u0x4DE517
    }
}

unsigned u0x493CC4 = 0x493CC4;
void __declspec(naked) naked_ep4_main_bottom()
{
    __asm
    {
        pushad
        mov ebp,esi

        lea edi,[ebp+0x302C]
        lea esi,kEp4MainBottomA
        mov ecx,4
        cld
        rep movsd

        lea edi,[ebp+0x304C]
        lea esi,kEp4MainBottomB
        mov ecx,3
        rep movsd

        popad
        fld dword ptr [esi+0x3054]
        jmp u0x493CC4
    }
}

unsigned u0x493552 = 0x493552;
void __declspec(naked) naked_ep4_main_bottom_8()
{
    __asm
    {
        pushad
        mov ebp,esi

        lea edi,[ebp+0x2FE4]
        lea esi,kEp4MainBottom8A
        mov ecx,4
        cld
        rep movsd

        lea edi,[ebp+0x3000]
        lea esi,kEp4MainBottom8B
        mov ecx,3
        rep movsd

        popad
        fld dword ptr [esi+0x3008]
        jmp u0x493552
    }
}

unsigned u0x494415 = 0x494415;
void __declspec(naked) naked_ep4_main_bottom_12()
{
    __asm
    {
        pushad
        mov ebp,esi

        lea edi,[ebp+0x3078]
        lea esi,kEp4MainBottom12A
        mov ecx,4
        cld
        rep movsd

        lea edi,[ebp+0x3094]
        lea esi,kEp4MainBottom12B
        mov ecx,3
        rep movsd

        popad
        fld dword ptr [esi+0x309C]
        jmp u0x494415
    }
}

unsigned u0x495B6D = 0x495B6D;
void __declspec(naked) naked_ep4_main_bottom_exp_length()
{
    __asm
    {
        add eax,67
        push eax
        call u0x631BE0
        jmp u0x495B6D
    }
}

unsigned u0x495B56 = 0x495B56;
void __declspec(naked) naked_ep4_main_bottom_exp_width()
{
    __asm
    {
        sub edi,3
        push edi
        fmul qword ptr ds:[0x74E998]
        jmp u0x495B56
    }
}

unsigned u0x494D76 = 0x494D76;
void __declspec(naked) naked_ep4_main_bottom_exp_text()
{
    __asm
    {
        mov [esp+0xC],220
        jmp u0x494D76
    }
}

unsigned u0x495CA1 = 0x495CA1;
void __declspec(naked) naked_ep4_main_bottom_bless()
{
    __asm
    {
        sub eax,5
        add edi,67
        push eax
        fld dword ptr [esp+0x24]
        jmp u0x495CA1
    }
}

unsigned u0x495D7E = 0x495D7E;
void __declspec(naked) naked_ep4_main_bottom_bless_glow()
{
    __asm
    {
        fld dword ptr [esp+0x24]
        fstp dword ptr [esp]
        sub eax,5
        jmp u0x495D7E
    }
}

unsigned u0x495693 = 0x495693;
void __declspec(naked) naked_ep4_main_bottom_8_exp_length()
{
    __asm
    {
        add eax,55
        push eax
        call u0x631BE0
        jmp u0x495693
    }
}

unsigned u0x49567C = 0x49567C;
void __declspec(naked) naked_ep4_main_bottom_8_exp_width()
{
    __asm
    {
        sub edi,1
        push edi
        fmul qword ptr ds:[0x74E9B8]
        jmp u0x49567C
    }
}

unsigned u0x494D68 = 0x494D68;
void __declspec(naked) naked_ep4_main_bottom_8_exp_text()
{
    __asm
    {
        mov [esp+0xC],174
        jmp u0x494D68
    }
}

unsigned u0x4957BA = 0x4957BA;
void __declspec(naked) naked_ep4_main_bottom_8_bless()
{
    __asm
    {
        sub eax,1
        add edi,53
        push eax
        fld dword ptr [esp+0x24]
        jmp u0x4957BA
    }
}

unsigned u0x49588E = 0x49588E;
void __declspec(naked) naked_ep4_main_bottom_8_bless_glow()
{
    __asm
    {
        fld dword ptr [esp+0x24]
        fstp dword ptr [esp]
        sub eax,1
        jmp u0x49588E
    }
}

unsigned u0x496077 = 0x496077;
void __declspec(naked) naked_ep4_main_bottom_12_exp_length()
{
    __asm
    {
        add eax,79
        push eax
        call u0x631BE0
        jmp u0x496077
    }
}

unsigned u0x496060 = 0x496060;
void __declspec(naked) naked_ep4_main_bottom_12_exp_width()
{
    __asm
    {
        sub ebx,3
        push ebx
        fmul qword ptr ds:[0x74E988]
        jmp u0x496060
    }
}

unsigned u0x494D90 = 0x494D90;
void __declspec(naked) naked_ep4_main_bottom_12_exp_text()
{
    __asm
    {
        mov dword ptr [esp+0xC],275
        mov dword ptr [esp+0x8],109
        jmp u0x494D90
    }
}

unsigned u0x4961AB = 0x4961AB;
void __declspec(naked) naked_ep4_main_bottom_12_bless()
{
    __asm
    {
        sub edx,3
        add ebx,79
        push edx
        fld dword ptr [esp+0x24]
        jmp u0x4961AB
    }
}

unsigned u0x49626C = 0x49626C;
void __declspec(naked) naked_ep4_main_bottom_12_bless_glow()
{
    __asm
    {
        fld dword ptr [esp+0x2C]
        fstp dword ptr [esp]
        sub eax,3
        jmp u0x49626C
    }
}

unsigned u0x51F420 = 0x51F420;
void __declspec(naked) naked_ep4_option_main_button()
{
    __asm
    {
        pushad
        lea eax,[esi+0x1AC38]
        mov dword ptr [eax],64
        popad
        mov ecx,[esi+0x1AC38]
        jmp u0x51F420
    }
}

unsigned u0x4CFE60 = 0x4CFE60;
void __declspec(naked) naked_ep4_loadbar()
{
    __asm
    {
        pushad
        lea eax,[esi+0x6610]
        mov dword ptr [eax],0x44020000
        add eax,20
        mov dword ptr [eax],0x42DC0000
        add eax,4
        mov dword ptr [eax],450
        add eax,4
        mov dword ptr [eax],0x425C0000
        add eax,8
        mov dword ptr [eax],0x41F00000
        popad
        fsub dword ptr [esi+0x6624]
        jmp u0x4CFE60
    }
}

unsigned u0x475C88 = 0x475C88;
void __declspec(naked) naked_0x475C83()
{
    __asm
    {
        // Select screen texture layout tweak.
        add eax, 0xFFFFFC00
        sar eax, 0x02
        jmp u0x475C88
    }
}

unsigned u0x475C98 = 0x475C98;
void __declspec(naked) naked_0x475C90()
{
    __asm
    {
        pushad
        lea eax, [ebx-0x04]
        mov dword ptr [eax], 0xC37B0000
        add eax, 0x08
        call u0x631BE0
        popad
        jmp u0x475C98
    }
}

unsigned u0x631AB0 = 0x631AB0;
unsigned u0x532DE4 = 0x532DE4;
void __declspec(naked) naked_ep5_pos_name_ver()
{
    __asm
    {
        add eax, 0x0B
        push eax
        call u0x631AB0
        jmp u0x532DE4
    }
}

unsigned u0x5332FC = 0x5332FC;
void __declspec(naked) naked_ep5_pos_lv_ver()
{
    __asm
    {
        add edx, 45
        cmp ebx, 0x0A
        jmp u0x5332FC
    }
}

unsigned u0x53330C = 0x53330C;
void __declspec(naked) naked_ep5_pos_lv_hor()
{
    __asm
    {
        add eax, 0x20
        mov [esp+0x10], eax
        jmp u0x53330C
    }
}

unsigned u0x53333D = 0x53333D;
void __declspec(naked) naked_ep5_pos_lv_hor2()
{
    __asm
    {
        add ecx, 34
        jmp u0x53333D
    }
}

unsigned u0x533144 = 0x533144;
void __declspec(naked) naked_ep5_sp_bar()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsSpOffset]
        jmp u0x533144
    }
}

unsigned u0x533195 = 0x533195;
void __declspec(naked) naked_ep5_sp_bar2()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsSpOffset]
        jmp u0x533195
    }
}

unsigned u0x5332AD = 0x5332AD;
void __declspec(naked) naked_ep5_sp_bar3()
{
    __asm
    {
        fld dword ptr ds:[g_ep5StatsSpOffset]
        jmp u0x5332AD
    }
}

unsigned u0x532FFF = 0x532FFF;
void __declspec(naked) naked_ep5_mp_bar()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsMpOffset]
        jmp u0x532FFF
    }
}

unsigned u0x533050 = 0x533050;
void __declspec(naked) naked_ep5_mp_bar2()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsMpOffset]
        jmp u0x533050
    }
}

unsigned u0x533252 = 0x533252;
void __declspec(naked) naked_ep5_mp_bar3()
{
    __asm
    {
        fld dword ptr ds:[g_ep5StatsMpOffset]
        jmp u0x533252
    }
}

unsigned u0x532EC0 = 0x532EC0;
void __declspec(naked) naked_ep5_hp_bar()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsHpOffset]
        jmp u0x532EC0
    }
}

unsigned u0x532F0B = 0x532F0B;
void __declspec(naked) naked_ep5_hp_bar2()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5StatsHpOffset]
        jmp u0x532F0B
    }
}

unsigned u0x5331ED = 0x5331ED;
void __declspec(naked) naked_ep5_hp_bar3()
{
    __asm
    {
        fld dword ptr ds:[g_ep5StatsHpOffset]
        jmp u0x5331ED
    }
}

unsigned u0x57BCD3 = 0x57BCD3;
void __declspec(naked) naked_ep5_main_circle_2_bg()
{
    __asm
    {
        push eax
        mov eax, 0x005332DE
        cmp [esp+0x84], eax
        pop eax
        je new_float

        fld dword ptr ds:[ecx+0x08]
        faddp st(7), st(0)
        jmp u0x57BCD3

        new_float:
        fld dword ptr ds:[g_ep5Float256]
        faddp st(7), st(0)
        jmp u0x57BCD3
    }
}

unsigned u0x5333F0 = 0x5333F0;
void __declspec(naked) naked_ep5_circle_face()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float15]
        jmp u0x5333F0
    }
}

unsigned u0x53341B = 0x53341B;
void __declspec(naked) naked_ep5_circle_face2()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float15]
        jmp u0x53341B
    }
}

unsigned u0x533465 = 0x533465;
void __declspec(naked) naked_ep5_circle_face3()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float15]
        jmp u0x533465
    }
}

unsigned u0x5333E1 = 0x5333E1;
void __declspec(naked) naked_ep5_main_circle_pos_y()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float26]
        jmp u0x5333E1
    }
}

unsigned u0x53340C = 0x53340C;
void __declspec(naked) naked_ep5_main_circle_pos_y2()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float26]
        jmp u0x53340C
    }
}

unsigned u0x533456 = 0x533456;
void __declspec(naked) naked_ep5_main_circle_pos_y3()
{
    __asm
    {
        fadd dword ptr ds:[g_ep5Float26]
        jmp u0x533456
    }
}

unsigned u0x5351AA = 0x5351AA;
void __declspec(naked) naked_ep5_enemy_bar_monster()
{
    __asm
    {
        push eax
        mov eax, dword ptr ds:[g_ep5EnemyBarX]
        mov [esi+0x180], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarY]
        mov [esi+0x17C], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarWidth]
        mov [esi+0x184], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarHeight]
        mov [esi+0x188], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarNameY]
        mov [esi+0x194], eax
        pop eax
        cmp eax, ecx
        mov [esp+0x24], eax
        jmp u0x5351AA
    }
}

unsigned u0x535328 = 0x535328;
void __declspec(naked) naked_ep5_enemy_bar_monster_select()
{
    __asm
    {
        add ecx, 0x23
        push ecx
        add edx, 0x40
        jmp u0x535328
    }
}

unsigned u0x535730 = 0x535730;
void __declspec(naked) naked_ep5_enemy_bar_npc()
{
    __asm
    {
        push eax
        mov eax, dword ptr ds:[g_ep5EnemyBarX]
        mov [esi+0x180], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarY]
        mov [esi+0x17C], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarWidth]
        mov [esi+0x184], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarHeight]
        mov [esi+0x188], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarNameY]
        mov [esi+0x194], eax
        pop eax
        cmp eax, ecx
        mov [esp+0x20], eax
        jmp u0x535730
    }
}

unsigned u0x53509A = 0x53509A;
unsigned u0x535447 = 0x535447;
void __declspec(naked) naked_ep5_enemy_bar_select_char()
{
    __asm
    {
        push eax
        mov eax, dword ptr ds:[g_ep5EnemyBarX]
        mov [esi+0x180], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarY]
        mov [esi+0x17C], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarWidth]
        mov [esi+0x184], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarHeight]
        mov [esi+0x188], eax
        mov eax, dword ptr ds:[g_ep5EnemyBarNameY]
        mov [esi+0x194], eax
        pop eax
        mov ebx, eax
        test ebx, ebx
        je enemy_conditional
        jmp u0x535447

        enemy_conditional:
        jmp u0x53509A
    }
}

unsigned u0x493F6F = 0x493F6F;
void __declspec(naked) naked_ep5_fix_mall2_btn_a1()
{
    __asm
    {
        push eax
        mov eax, dword ptr ds:[g_ep5Mall2BarPosX]
        mov dword ptr ds:[esi+0x3094], eax
        pop eax
        fld dword ptr ds:[g_ep5Mall2Button1]
        jmp u0x493F6F
    }
}

unsigned u0x494049 = 0x494049;
void __declspec(naked) naked_ep5_fix_mall2_btn_a2()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button2]
        jmp u0x494049
    }
}

unsigned u0x494122 = 0x494122;
void __declspec(naked) naked_ep5_fix_mall2_btn_a3()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button3]
        jmp u0x494122
    }
}

unsigned u0x4941FB = 0x4941FB;
void __declspec(naked) naked_ep5_fix_mall2_btn_a4()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button4]
        jmp u0x4941FB
    }
}

unsigned u0x4942DC = 0x4942DC;
void __declspec(naked) naked_ep5_fix_mall2_btn_a5()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button5]
        jmp u0x4942DC
    }
}

unsigned u0x4943BF = 0x4943BF;
void __declspec(naked) naked_ep5_fix_mall2_btn_a6()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button6]
        jmp u0x4943BF
    }
}

unsigned u0x4944A0 = 0x4944A0;
void __declspec(naked) naked_ep5_fix_mall2_btn_a7()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button7]
        jmp u0x4944A0
    }
}

unsigned u0x494585 = 0x494585;
void __declspec(naked) naked_ep5_fix_mall2_btn_a8()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2Button8]
        jmp u0x494585
    }
}

unsigned u0x493F45 = 0x493F45;
void __declspec(naked) naked_ep5_fix_mall2_btn_a9()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep1]
        jmp u0x493F45
    }
}

unsigned u0x494025 = 0x494025;
void __declspec(naked) naked_ep5_fix_mall2_btn_a10()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep2]
        jmp u0x494025
    }
}

unsigned u0x4940FE = 0x4940FE;
void __declspec(naked) naked_ep5_fix_mall2_btn_a11()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep3]
        jmp u0x4940FE
    }
}

unsigned u0x4941D7 = 0x4941D7;
void __declspec(naked) naked_ep5_fix_mall2_btn_a12()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep4]
        jmp u0x4941D7
    }
}

unsigned u0x4942C2 = 0x4942C2;
void __declspec(naked) naked_ep5_fix_mall2_btn_a13()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep5]
        jmp u0x4942C2
    }
}

unsigned u0x49439B = 0x49439B;
void __declspec(naked) naked_ep5_fix_mall2_btn_a14()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep6]
        jmp u0x49439B
    }
}

unsigned u0x49447C = 0x49447C;
void __declspec(naked) naked_ep5_fix_mall2_btn_a15()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep7]
        jmp u0x49447C
    }
}

unsigned u0x494561 = 0x494561;
void __declspec(naked) naked_ep5_fix_mall2_btn_a16()
{
    __asm
    {
        fld dword ptr ds:[g_ep5Mall2RowStep8]
        jmp u0x494561
    }
}

unsigned u0x4E26BC = 0x4E26BC;
void __declspec(naked) naked_ep5_fix_clock2_btn_a()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockA]
        jmp u0x4E26BC
    }
}

unsigned u0x4E2752 = 0x4E2752;
void __declspec(naked) naked_ep5_fix_clock2_btn_b()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockB]
        jmp u0x4E2752
    }
}

unsigned u0x4DFF5E = 0x4DFF5E;
void __declspec(naked) naked_ep5_fix_clock2_btn_c()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockC]
        jmp u0x4DFF5E
    }
}

unsigned u0x4DFF4E = 0x4DFF4E;
void __declspec(naked) naked_ep5_fix_clock2_btn_d()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockD]
        jmp u0x4DFF4E
    }
}

unsigned u0x4E0035 = 0x4E0035;
void __declspec(naked) naked_ep5_fix_clock2_btn_f()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockF]
        jmp u0x4E0035
    }
}

unsigned u0x4E0025 = 0x4E0025;
void __declspec(naked) naked_ep5_fix_clock2_btn_g()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockG]
        jmp u0x4E0025
    }
}

unsigned u0x4DFD8F = 0x4DFD8F;
void __declspec(naked) naked_ep5_fix_clock2_btn_h()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockH]
        jmp u0x4DFD8F
    }
}

unsigned u0x4DFD7F = 0x4DFD7F;
void __declspec(naked) naked_ep5_fix_clock2_btn_i()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockI]
        jmp u0x4DFD7F
    }
}

unsigned u0x4DFCA5 = 0x4DFCA5;
void __declspec(naked) naked_ep5_fix_clock2_btn_j()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockJ]
        jmp u0x4DFCA5
    }
}

unsigned u0x4DFCB5 = 0x4DFCB5;
void __declspec(naked) naked_ep5_fix_clock2_btn_k()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockK]
        jmp u0x4DFCB5
    }
}

unsigned u0x4DFBCB = 0x4DFBCB;
void __declspec(naked) naked_ep5_fix_clock2_btn_l()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockL]
        jmp u0x4DFBCB
    }
}

unsigned u0x4DFBDB = 0x4DFBDB;
void __declspec(naked) naked_ep5_fix_clock2_btn_m()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockM]
        jmp u0x4DFBDB
    }
}

unsigned u0x4E26CA = 0x4E26CA;
void __declspec(naked) naked_ep5_fix_clock2_btn_n()
{
    __asm
    {
        fld dword ptr ds:[g_ep5ClockN]
        jmp u0x4E26CA
    }
}

#define DEFINE_STATS_COLOR_HOOK(NAME, RETURN_ADDR, BLUE, GREEN, RED) \
    unsigned NAME##Return = RETURN_ADDR; \
    void __declspec(naked) NAME() \
    { \
        __asm push BLUE \
        __asm fild dword ptr ds:[0x22B1954] \
        __asm push GREEN \
        __asm push RED \
        __asm jmp NAME##Return \
    }

// Colour on stats at (T) window.
// Each hook keeps the original stat value load and only replaces the B/G/R
// color arguments used by the stock render call.
DEFINE_STATS_COLOR_HOOK(naked_stats_color_strength, 0x52A1AA, 0x00, 0x00, 0xFF)
DEFINE_STATS_COLOR_HOOK(naked_stats_color_reaction, 0x52A56A, 0xCE, 0x00, 0xFF)
DEFINE_STATS_COLOR_HOOK(naked_stats_color_intelligence, 0x52A92A, 0xFF, 0x80, 0x80)
DEFINE_STATS_COLOR_HOOK(naked_stats_color_wisdom, 0x52ACEA, 0x00, 0xFF, 0x00)
DEFINE_STATS_COLOR_HOOK(naked_stats_color_dexterity, 0x52B0AA, 0x00, 0x80, 0xFF)
DEFINE_STATS_COLOR_HOOK(naked_stats_color_lucky, 0x52B46A, 0xFF, 0xFF, 0x00)

#undef DEFINE_STATS_COLOR_HOOK

unsigned u0x50CA15 = 0x50CA15;
unsigned u0x50C5F0 = 0x50C5F0;
unsigned u0x50BEA6 = 0x50BEA6;
unsigned u0x50D16D = 0x50D16D;
void __declspec(naked) naked_skip_server_selection()
{
    __asm
    {
        // Skip server selection screen.
        // If the login server returned exactly one available server, select
        // index 0 and call the original CSelectServer::OnSelect path. This
        // keeps the stock connection/state transition logic intact instead of
        // fabricating packets or jumping to character select directly.
        mov dword ptr ds:[g_skipServerSelectionWindow], ecx
        mov eax, dword ptr ds:[0x22EED30]
        test eax, eax
        je original
        cmp dword ptr [eax + 0x274], 1
        jne original
        cmp dword ptr [ecx + 0xBD8], 0xFFFFFFFF
        jne original
        mov dword ptr [ecx + 0xBD8], 0
        call u0x50C5F0
        ret

    original:
        jmp u0x50CA15
    }
}

void __declspec(naked) naked_auto_select_single_server_after_init()
{
    __asm
    {
        // Experimental instant single-server selection.
        // Run after the server-selection UI object is fully initialized, but
        // before its first normal update/render pass. This keeps the original
        // CSelectServer::OnSelect connection path and only moves *when* it is
        // invoked. If this misbehaves, remove only the 0050D164 detour below.
        pushad
        mov eax, dword ptr ds:[0x22EED30]
        test eax, eax
        je done
        cmp dword ptr [eax + 0x274], 1
        jne done
        cmp dword ptr [esi + 0xBD8], 0xFFFFFFFF
        jne done
        mov dword ptr ds:[g_skipServerSelectionWindow], esi
        mov dword ptr [esi + 0xBD8], 0
        mov ecx, esi
        call u0x50C5F0

    done:
        popad
        // Original constructor epilogue instructions overwritten at 0050D164.
        mov eax, esi
        mov ecx, dword ptr [esp + 0x120]
        jmp u0x50D16D
    }
}

void __declspec(naked) naked_hide_single_server_selection_render()
{
    __asm
    {
        mov eax, dword ptr ds:[0x22EED30]
        test eax, eax
        je original
        cmp dword ptr [eax + 0x274], 1
        jne original
        ret

    original:
        // Original CSelectServer render prologue overwritten at 0050BEA0.
        sub esp, 0x18
        push ebx
        push ebp
        push esi
        jmp u0x50BEA6
    }
}

void patch_stats_window_colors()
{
    util::detour((void*)0x52A195, naked_stats_color_strength, 5);
    util::detour((void*)0x52A555, naked_stats_color_reaction, 5);
    util::detour((void*)0x52A915, naked_stats_color_intelligence, 5);
    util::detour((void*)0x52ACD5, naked_stats_color_wisdom, 5);
    util::detour((void*)0x52B095, naked_stats_color_dexterity, 5);
    util::detour((void*)0x52B455, naked_stats_color_lucky, 5);
}

int should_suppress_subaction_sysmsg(int messageNumber)
{
    if (!kEnableSubactionMessageCooldown)
        return false;

    if (messageNumber < kSubactionMessageFirst || messageNumber > kSubactionMessageLast)
        return false;

    const char* playerName = shaiya::g_var->sysmsg_p.data();
    if (!playerName || !playerName[0])
        playerName = "<unknown>";

    static std::unordered_map<std::string, std::uint32_t> lastShownByPlayer;
    auto now = GetTickCount();
    auto& lastShown = lastShownByPlayer[playerName];

    if (lastShown && now - lastShown < kSubactionMessageCooldownMs)
        return true;

    lastShown = now;
    return false;
}

unsigned u0x423155 = 0x423155;
void __declspec(naked) naked_sysmsg_to_chatbox_subaction_cooldown()
{
    __asm
    {
        pushad
        push dword ptr [esp+0x28] // messageNumber
        call should_suppress_subaction_sysmsg
        add esp,0x4
        test eax,eax
        popad

        jne suppress

        // original
        push esi
        mov esi,dword ptr [esp+0x10]
        jmp u0x423155

        suppress:
        ret
    }
}

void hook::patch()
{
    static constexpr unsigned char kPushZero[] = {0x6A, 0x00};
    static constexpr unsigned char kPushMinusOne[] = {0x68, 0xFF, 0xFF, 0xFF, 0xFF};
    static constexpr unsigned char kBypassCharacterCreateNameFormatCheck[] = {0xEB, 0x34, 0x90, 0x90};
    static constexpr unsigned char kBypassCharacterCreateNameSubstringChecks[] = {0xE9, 0x84, 0x00, 0x00, 0x00, 0x90};
    static constexpr unsigned char kBypassCharacterCreateNameVerifiedGate[] = {0xEB};
    static constexpr unsigned char kBypassCharacterCreateNameAvailabilityRequest[] = {0xE9, 0xC2, 0x04, 0x00, 0x00};
    static constexpr unsigned char kTreatCharacterCreateResultBusyAsSuccess[] = {0x74, 0x22};
    static constexpr unsigned char kForceUltimateModeOnCharacterCreate[] =
    {
        0xC7, 0x86, 0xC8, 0x24, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
        0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
        0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
    };
    static constexpr unsigned char kLoginCrapSkipRenderBlock[] = {0xE9, 0xD8, 0x02, 0x00, 0x00};
    static constexpr unsigned char kLoginCrapSkipAfterResourceInit[] = {0xE9, 0x96, 0x02, 0x00, 0x00};

    if (kEnableExperimentalInterfacePngRedirect)
    {
        // Experimental PNG interface support.
        // Rewrites known interface texture references in Game.exe from
        // .tga/.jpg to .png so the client can load a PNG-based interface pack.
        patch_module_interface_texture_extensions_to_png();
    }

    // Screenshot output format.
    // Rewrites the internal screenshot filename templates from .jpg/.JPG to .png.
    patch_screenshot_extensions_to_png();

    // Cooldown for subaction messages (20s).
    // Instead of removing sysmsg 5228-5237 completely, rate-limit the whole
    // subaction message block to one visible message every 20 seconds per
    // player name (<p>). Comment this detour out to restore stock behavior.
    util::detour((void*)0x423150, naked_sysmsg_to_chatbox_subaction_cooldown, 5);

    // Skip updater check via CONFIG.ini.
    // ADVANCED -> SKIPUPDATER=1 makes the client see the same "start game"
    // command-line token normally supplied by Updater.exe. This keeps the
    // stock startup flow intact and avoids patching updater-state flags.
    patch_get_command_line_for_skip_updater();

    // Server IP via CONFIG.ini.
    // ADVANCED -> IP= uses 127.0.0.1 when empty/missing, or the configured
    // address when present. This updates the stock login server string only.
    patch_login_server_ip_from_config();

    // Skip server selection screen.
    // When there is exactly one server in the login server list, choose it
    // automatically after account login and continue to character select.
    // 0050CA10 keeps the stock selection/connect path. 0050BEA0 only hides
    // the single-server panel render so this behaves like a skip instead of an
    // obvious auto-click. The network-list handler is too early and can leave
    // login stuck, so do not fire the selection from there.
    util::detour((void*)0x50CA10, naked_skip_server_selection, 5);
    util::detour((void*)0x50BEA0, naked_hide_single_server_selection_render, 6);
    util::detour((void*)0x50D164, naked_auto_select_single_server_after_init, 9);

    // dungeon wings shadow workaround
    util::detour((void*)0x41F9C0, naked_0x41F9C0, 9);
    // evolution bug
    util::detour((void*)0x41E2BB, naked_0x41E2BB, 7);

    // remove ep6 vehicle section (auction board)
    util::write_memory((void*)0x463FE0, 0x07, 1);
    // speed recreation
    util::write_memory((void*)0x4C4D2F, 0x02, 1);
    // speed enchant
    util::write_memory((void*)0x501600, 0x02, 1);
    util::write_memory((void*)0x501602, 0x02, 1);
    util::write_memory((void*)0x501631, 0x02, 1);
    util::write_memory((void*)0x501633, 0x02, 1);
    util::write_memory((void*)0x501644, 0x03, 1);
    util::write_memory((void*)0x50164D, 0x03, 1);
    // Buff row position is now handled by the movable buff hook in ep6_repo_features.cpp.
    // Keep only spacing/count here so the runtime detours own the X/Y instructions.
    // 004D74E7  add edi,28
    // 004D7509  add dword ptr [esp+10],28
    util::write_memory((void*)0x4D74E9, &kBuffSpacing, sizeof(kBuffSpacing));
    util::write_memory((void*)0x4D750D, &kBuffSpacing, sizeof(kBuffSpacing));
    // 004D74EA  cmp eax,09
    util::write_memory((void*)0x4D74EC, &kBuffCountPerRow, sizeof(kBuffCountPerRow));
    // Fast Transition (Experimental - 100ms)
    // 00436940  cmp eax,00000BB8
    util::write_memory((void*)0x436941, &kFastTransitionDelay, sizeof(kFastTransitionDelay));
    // 004D0340  push 00001388
    util::write_memory((void*)0x4D0341, &kFastTransitionDelay, sizeof(kFastTransitionDelay));
    // view id visibility behavior
    // Controlled by ADVANCED -> IDVIEW=ON/OFF in CONFIG.ini.
    load_id_view_setting();
    util::detour((void*)0x4E5876, naked_0x4E6D76, 5);
    // Login crap skip.
    // Hides the Nexon/copyright startup splash and skips its render/fade loops
    // while preserving the login resource setup that the account screen needs.
    // Do not replace this with a direct state-table jump to login: that looks
    // correct visually, but opens login with missing initialization and crashes
    // after the account login step.
    util::write_memory((void*)0x74A5A0, kLoginCrapSkipHiddenCopyrightMessage, sizeof(kLoginCrapSkipHiddenCopyrightMessage));
    util::write_memory((void*)0x4346B1, &kLoginCrapSkipSplashSleepDelay, sizeof(kLoginCrapSkipSplashSleepDelay));
    util::write_memory((void*)0x434ACD, &kLoginCrapSkipPostInitDelay, sizeof(kLoginCrapSkipPostInitDelay));
    util::write_memory((void*)0x4346BB, kLoginCrapSkipRenderBlock, sizeof(kLoginCrapSkipRenderBlock));
    // 00434A92 calls the login resource init/check on 007C48FC. Once that has
    // run, advance to the original state-1 transition block instead of drawing
    // the remaining splash frames.
    util::write_memory((void*)0x434A97, kLoginCrapSkipAfterResourceInit, sizeof(kLoginCrapSkipAfterResourceInit));
    // Ress leader visual timer.
    // The popup text comes from sysmsg 10068 and its countdown renders using
    // global 007AD434, initialized as 30 seconds at 004D5F41. This is separate
    // from the server-side 5000ms gameplay timer and must be patched in seconds.
    util::write_memory((void*)0x4D5F47, &kClientRessLeaderVisualSeconds, sizeof(kClientRessLeaderVisualSeconds));
    // Logout/game-over visual countdown.
    // The "x seconds left till game over" text uses sysmsg 10083/10084. The
    // countdown itself is a float copied into global 022B045C at 005223A9 and
    // then decremented every frame, so patch the initializer instead of the
    // later display clamp. This keeps the visible number ticking down normally.
    std::uint8_t logoutVisualInitPatch[] = { 0xD9, 0x05, 0x00, 0x00, 0x00, 0x00 };
    auto logoutVisualSecondsAddress = reinterpret_cast<std::uint32_t>(&gLogoutGameOverVisualSeconds);
    std::memcpy(&logoutVisualInitPatch[2], &logoutVisualSecondsAddress, sizeof(logoutVisualSecondsAddress));
    util::write_memory((void*)0x5223A9, logoutVisualInitPatch, sizeof(logoutVisualInitPatch));
    // Login to character selection delay.
    // Keep the safe stock update/selection path, but shorten the hidden
    // single-server bridge timeout from the stock 30000ms to 1000ms.
    // Do not trigger CSelectServer::OnSelect from render/network-list handlers:
    // those contexts can run before the login bridge is ready and leave login
    // stuck.
    util::write_memory((void*)0x50C708, &kLoginToCharacterSelectionDelayMs, sizeof(kLoginToCharacterSelectionDelayMs));
    util::write_memory((void*)0x50C869, &kLoginToCharacterSelectionDelayMs, sizeof(kLoginToCharacterSelectionDelayMs));
    // Remove level up messages.
    // Keep the stock levelup_skillup.tga/levelup_statusup.tga creation flow
    // intact, but force their 256x256 render size arguments to 0x0 so the
    // level-up splash textures are not visible.
    util::write_memory((void*)0x4AC538, &kHiddenLevelUpMessageSize, sizeof(kHiddenLevelUpMessageSize));
    util::write_memory((void*)0x4AC53D, &kHiddenLevelUpMessageSize, sizeof(kHiddenLevelUpMessageSize));
    util::write_memory((void*)0x4AC603, &kHiddenLevelUpMessageSize, sizeof(kHiddenLevelUpMessageSize));
    util::write_memory((void*)0x4AC608, &kHiddenLevelUpMessageSize, sizeof(kHiddenLevelUpMessageSize));
    // Character creation name format check.
    // The creation UI calls the old single-byte name validator at 00564C40.
    // Keep empty/short-name checks and server-side validation intact, but skip
    // the local ASCII-only rejection so UTF-8/special names can reach the next
    // creation step.
    util::write_memory((void*)0x472214, kBypassCharacterCreateNameFormatCheck, sizeof(kBypassCharacterCreateNameFormatCheck));
    // The same flow checks several blocked substrings before sending the name
    // availability request. UTF-8 byte sequences can collide with these legacy
    // single-byte checks, so bypass only that local pattern filter.
    util::write_memory((void*)0x472280, kBypassCharacterCreateNameSubstringChecks, sizeof(kBypassCharacterCreateNameSubstringChecks));
    // The creation panel also has a separate "name already verified" flag
    // ([esi+2B8C]) that normally forces the user through the availability button
    // before the create action is allowed. Skip that UI gate entirely.
    util::write_memory((void*)0x472146, kBypassCharacterCreateNameVerifiedGate, sizeof(kBypassCharacterCreateNameVerifiedGate));
    // The stock client sends a 0x119 name-availability request here, then shows
    // a local confirmation dialog before it actually sends the 0x102 character
    // creation packet. This server build does not use that client-side gate, so
    // jump directly to the 0x102 creation path. Real duplicate/validity checks
    // must live in the server/dbagent character creation path, not in this UI gate.
    util::write_memory((void*)0x47243A, kBypassCharacterCreateNameAvailabilityRequest, sizeof(kBypassCharacterCreateNameAvailabilityRequest));
    // Character creation result handling.
    // Some UTF-8/special names come back through the legacy "busy" result path
    // even after the local availability check was accepted. Keep this client
    // bypass as a compatibility fallback while the real creation validation is
    // handled by the server/dbagent side.
    util::write_memory((void*)0x472A12, kTreatCharacterCreateResultBusyAsSuccess, sizeof(kTreatCharacterCreateResultBusyAsSuccess));
    // Skip mode selection screen and force Ultimate Mode by default.
    // The first detour skips the mode-selection UI block. The inline patch
    // replaces the later mode validation with:
    //   mov dword ptr [esi+0x24C8], 3
    // so character creation proceeds as UM without asking the user to choose.
    util::detour((void*)0x471D4E, (void*)0x471DF7, 169);
    util::write_memory((void*)0x472907, kForceUltimateModeOnCharacterCreate, sizeof(kForceUltimateModeOnCharacterCreate));
    patch_stats_window_colors();
    // Chat UTF-8 support without changing the client's global LoginVersion.
    // The stock Vietnam branch enables UTF-8 display, but it also takes over
    // keyboard layout behavior and prevents ES/US + Unikey users from typing.
    // Instead, create the main GAME window as a Unicode HWND, translate
    // non-ASCII WM_CHAR/IME input into UTF-8 textbox bytes ourselves, and force
    // only the narrow renderer/wrapping checks that are required for chat.
    patch_game_window_ansi_creation_to_unicode();
    util::detour((void*)0x5676FE, naked_0x5676FE, 5);
    // UTF-8 textbox codepage for IME/composition text, scoped to the live
    // textbox fields instead of the whole client codepage/login version.
    util::detour((void*)0x564FC1, naked_0x564FC1, 5);
    // UTF-8 textbox bytes in the normal handler.
    // Keep this permissive because some paths still validate one byte at a time.
    util::write_memory((void*)0x566AC5, "\x90\x90", 2);
    // The textbox renderer uses this helper to step over multibyte characters,
    // but the stock client only enables that UTF-8 logic when LoginVersion == 3.
    // We keep LoginVersion untouched to avoid the Vietnam keyboard-layout lock,
    // and only force the UTF-8 byte-length branch needed for chat wrapping/display.
    util::write_memory((void*)0x407F7E, "\x90\x90", 2);
    // Chat box rendering has a second LoginVersion == 3 branch for the Vietnam
    // text path. Force only that local branch so UTF-8 chat lines are measured
    // and split as multibyte text, without enabling the full global codepage
    // mode that blocks non-Vietnam keyboard layouts.
    util::write_memory((void*)0x422E2B, "\x90\x90\x90\x90\x90\x90", 6);
    // Do not force the client's alternate Unicode window creation branch at
    // 00408650. In this executable that path makes the default "Shaiya" title
    // collapse to "S". The IAT wrapper above upgrades the normal CreateWindowExA
    // path to a Unicode HWND without changing the title source.
    util::write_memory((void*)0x40CA47, "\xEB", 1);
    // Chat send path byte limit.
    // The stock client forcibly writes a string terminator at byte 127 before
    // sending chat. UTF-8 languages use multiple bytes per character, so that
    // cap can cut a valid Vietnamese sequence in half even when the textbox and
    // renderer are already UTF-8 aware. NOP only that forced terminator; do not
    // resize structs or global buffers here.
    util::write_memory((void*)0x47A6BC, "\x90\x90\x90\x90", 4);
    // Background rendering behavior.
    // These byte patches replace the original push arguments with:
    // - push 0
    // - push -1 (0xFFFFFFFF)
    // Keep them grouped here so they are easy to audit or revert together.
    util::write_memory((void*)0x434742, kPushZero, sizeof(kPushZero));
    util::write_memory((void*)0x434880, kPushZero, sizeof(kPushZero));
    util::write_memory((void*)0x434B42, kPushZero, sizeof(kPushZero));
    util::write_memory((void*)0x4347FC, kPushMinusOne, sizeof(kPushMinusOne));
    util::write_memory((void*)0x43493A, kPushMinusOne, sizeof(kPushMinusOne));
    // EP4 UI support.
    // Applies the EP4 HUD/layout package while intentionally leaving inventory
    // and the existing server-time text format untouched.
    util::detour((void*)0x53201E, naked_ep4_main_stats, 6);
    util::detour((void*)0x532345, naked_ep4_main_stats_bar_hp, 6);
    util::detour((void*)0x532481, naked_ep4_main_stats_bar_mp, 6);
    util::detour((void*)0x5325C6, naked_ep4_main_stats_bar_sp, 6);
    util::detour((void*)0x53289F, naked_ep4_main_stats_level, 5);
    util::detour((void*)0x5350E5, naked_ep4_enemy_bar, 6);
    util::detour((void*)0x532BBF, naked_ep4_enemy_bar_bg, 7);
    util::detour((void*)0x534F24, naked_ep4_enemy_bar_buff, 10);
    util::detour((void*)0x534F48, naked_ep4_enemy_bar_debuff, 5);
    util::detour((void*)0x534F57, naked_ep4_enemy_bar_buff_mouse_over, 5);
    util::detour((void*)0x4DE685, naked_ep4_main_map_button, 6);
    util::detour((void*)0x4DF4AD, naked_ep4_main_map_bg, 7);
    util::detour((void*)0x4E1255, naked_ep4_map_clock, 5);
    util::detour((void*)0x4DDEDA, naked_ep4_main_map_servertime, 6);
    util::detour((void*)0x4D9A1C, naked_ep4_arrow_size_map, 6);
    util::detour((void*)0x4E055A, naked_ep4_arrow_size_minimap, 6);
    util::detour((void*)0x4D7A47, naked_ep4_load_arrow_map, 5);
    util::detour((void*)0x4DE4FB, naked_ep4_load_arrow_minimap, 5);
    util::detour((void*)0x493CBE, naked_ep4_main_bottom, 6);
    util::detour((void*)0x49354C, naked_ep4_main_bottom_8, 6);
    util::detour((void*)0x49440F, naked_ep4_main_bottom_12, 6);
    // Keep EXP/Bless bars stock: do not patch main_bottom_bar/main_bottom_bar8/main_bottom_bar12.
    util::detour((void*)0x51F41A, naked_ep4_option_main_button, 6);
    util::detour((void*)0x4CFE5A, naked_ep4_loadbar, 6);
    util::detour((void*)0x493362, (void*)0x493442, 7);
    // costume lag workaround
    util::write_memory((void*)0x56F38D, 0x75, 1);
    util::write_memory((void*)0x583DED, 0x75, 1);
    // pet/wing lag workaround
    util::write_memory((void*)0x5881EE, 0x85, 1);
}

void hook::select_screen()
{
    // Character select screen texture/text adjustments.
    util::detour((void*)0x475C83, naked_0x475C83, 5);
    util::detour((void*)0x475C90, naked_0x475C90, 8);
}

#include <chrono>
#include <cstdint>
#include <cstring>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "include/main.h"
#include "external/discord-rpc/include/discord_rpc.h"

namespace
{
    // Discord RPC setup.
    // Replace this application id with your own Discord developer application
    // if needed. This value is the default fallback used by the client.
    constexpr char kDiscordApplicationId[] = "1402021308056731698";
    // Edit this text if you want a different static Discord RPC message.
    constexpr char kDetailsText[] = "Working on local server";
    constexpr int kDiscordUpdateDelayMs = 5000;

    const std::int64_t kStartTimestamp =
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();

    DWORD WINAPI discord_thread(LPVOID)
    {
        DiscordEventHandlers handlers{};
        Discord_Initialize(kDiscordApplicationId, &handlers, 1, nullptr);

        auto* details = reinterpret_cast<const char8_t*>(kDetailsText);

        while (true)
        {
            DiscordRichPresence presence{};
            presence.details = details;
            presence.startTimestamp = kStartTimestamp;

            Discord_UpdatePresence(&presence);
            Discord_RunCallbacks();
            Sleep(kDiscordUpdateDelayMs);
        }
    }
}

void hook::discord()
{
    CreateThread(nullptr, 0, discord_thread, nullptr, 0, nullptr);
}

#pragma once
#include <stdint.h>

// clang-format off

#if defined(DISCORD_DYNAMIC_LIB)
#  if defined(_WIN32)
#    if defined(DISCORD_BUILDING_SDK)
#      define DISCORD_EXPORT __declspec(dllexport)
#    else
#      define DISCORD_EXPORT __declspec(dllimport)
#    endif
#  else
#    define DISCORD_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  define DISCORD_EXPORT
#endif

// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct DiscordRichPresenceButton {
        const char8_t* label;
        const char8_t* url;
    } DiscordRichPresenceButton;

    typedef struct DiscordRichPresence {
        const char8_t* state;   /* max 128 bytes */
        const char8_t* details; /* max 128 bytes */
        int64_t startTimestamp;
        int64_t endTimestamp;
        const char8_t* largeImageKey;  /* max 32 bytes */
        const char8_t* largeImageText; /* max 128 bytes */
        const char8_t* smallImageKey;  /* max 32 bytes */
        const char8_t* smallImageText; /* max 128 bytes */
        const char8_t* partyId;        /* max 128 bytes */
        int partySize;
        int partyMax;
        int partyPrivacy;
        const char8_t* matchSecret;    /* max 128 bytes */
        const char8_t* joinSecret;     /* max 128 bytes */
        const char8_t* spectateSecret; /* max 128 bytes */
        int8_t instance;
        DiscordRichPresenceButton* buttons[2];
    } DiscordRichPresence;

    typedef struct DiscordUser {
        const char* userId;
        const char* username;
        const char* discriminator;
        const char* avatar;
    } DiscordUser;

    typedef struct DiscordEventHandlers {
        void (*ready)(const DiscordUser* request);
        void (*disconnected)(int errorCode, const char* message);
        void (*errored)(int errorCode, const char* message);
        void (*joinGame)(const char* joinSecret);
        void (*spectateGame)(const char* spectateSecret);
        void (*joinRequest)(const DiscordUser* request);
    } DiscordEventHandlers;

#define DISCORD_REPLY_NO 0
#define DISCORD_REPLY_YES 1
#define DISCORD_REPLY_IGNORE 2
#define DISCORD_PARTY_PRIVATE 0
#define DISCORD_PARTY_PUBLIC 1

    DISCORD_EXPORT void Discord_Initialize(const char* applicationId,
        DiscordEventHandlers* handlers,
        int autoRegister,
        const char* optionalSteamId);
    DISCORD_EXPORT void Discord_Shutdown(void);

    /* checks for incoming messages, dispatches callbacks */
    DISCORD_EXPORT void Discord_RunCallbacks(void);

    /* If you disable the lib starting its own io thread, you'll need to call this from your own */
#ifdef DISCORD_DISABLE_IO_THREAD
    DISCORD_EXPORT void Discord_UpdateConnection(void);
#endif

    DISCORD_EXPORT void Discord_UpdatePresence(const DiscordRichPresence* presence);
    DISCORD_EXPORT void Discord_ClearPresence(void);

    DISCORD_EXPORT void Discord_Respond(const char* userid, /* DISCORD_REPLY_ */ int reply);

    DISCORD_EXPORT void Discord_UpdateHandlers(DiscordEventHandlers* handlers);

#ifdef __cplusplus
} /* extern "C" */
#endif

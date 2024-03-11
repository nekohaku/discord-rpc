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

#define DISCORD_MAX_BUTTONS 2

typedef struct DiscordRichPresence {
    const char* state;   /* max 128 bytes */
    const char* details; /* max 128 bytes */
    int64_t startTimestamp;
    int64_t endTimestamp;
    const char* largeImageKey;  /* max 32 bytes */
    const char* largeImageText; /* max 128 bytes */
    const char* smallImageKey;  /* max 32 bytes */
    const char* smallImageText; /* max 128 bytes */
    const char* partyId;        /* max 128 bytes */
    int partySize;
    int partyMax;
    int partyPrivacy;
    const char* matchSecret;    /* max 128 bytes */
    const char* joinSecret;     /* max 128 bytes */
    const char* spectateSecret; /* max 128 bytes */
    int8_t instance;
    const char* buttonNames[DISCORD_MAX_BUTTONS]; /* max 2 buttons */
    const char* buttonUrls[DISCORD_MAX_BUTTONS];  /* max 2 buttons */
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

// the nik api:
typedef enum NikDiscord_PresenceKey {
    kPresenceKey_First,
    kPresenceKey_State = kPresenceKey_First,
    kPresenceKey_Details,
    kPresenceKey_StartTimestamp,
    kPresenceKey_EndTimestamp,
    kPresenceKey_LargeImageKey,
    kPresenceKey_LargeImageText,
    kPresenceKey_SmallImageKey,
    kPresenceKey_SmallImageText,
    kPresenceKey_PartyId,
    kPresenceKey_PartySize,
    kPresenceKey_PartyMax,
    kPresenceKey_PartyPrivacy,
    kPresenceKey_MatchSecret,
    kPresenceKey_JoinSecret,
    kPresenceKey_SpectateSecret,
    kPresenceKey_Instance,
    kPresenceKey_ButtonName0,
    kPresenceKey_ButtonName1,
    kPresenceKey_ButtonUrl0,
    kPresenceKey_ButtonUrl1,
    kPresenceKey_Last = kPresenceKey_ButtonUrl1,
    kPresenceKey_ForceInt32 = 65536
} NikDiscord_PresenceKey;

typedef enum NikDiscord_EventKey {
    // required for all
    kEventKey_Type,
    // ready, joinRequest
    kEventKey_UserId,
    kEventKey_Username,
    kEventKey_Discriminator,
    kEventKey_Avatar,
    // errored,disconnected
    kEventKey_ErrorCode,
    kEventKey_Message,
    // join
    kEventKey_JoinSecret,
    // spectate
    kEventKey_SpectateSecret,
    // abi crap
    kEventKey_ForceInt32 = 65536
} NikDiscord_EventKey;

DISCORD_EXPORT void NikDiscord_Initialize(const char* pInApplicationIdString,
                                          const char* pInOptionalSteamIdString);

DISCORD_EXPORT void NikDiscord_Shutdown(void);

DISCORD_EXPORT void NikDiscord_RunCallbacks(void);

DISCORD_EXPORT void NikDiscord_SetPresenceKeyString(NikDiscord_PresenceKey inPresenceKey,
                                                    const char* pInUtf8String);

DISCORD_EXPORT void NikDiscord_SetPresenceKeyInt64(NikDiscord_PresenceKey inPresenceKey,
                                                   int64_t inValueInt64);

DISCORD_EXPORT void NikDiscord_CommitPresence(int inDoClearPresence);

DISCORD_EXPORT int NikDiscord_PopEvent(void);

DISCORD_EXPORT const char* NikDiscord_GetEventKey(NikDiscord_EventKey inEventKey,
                                                  int* pOutStringSize);

DISCORD_EXPORT void NikDiscord_Respond(const char* pInUserIdUtf8String, int inReply);

#ifdef __cplusplus
} /* extern "C" */
#endif

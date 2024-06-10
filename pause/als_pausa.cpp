
#include <sourcemod>
#include <sdktools>

#define REQUIRED_VOTES 3
#define PAUSE_DURATION 90.0

public Plugin myinfo = {
    name = "Pause Plugin",
    author = "liano",
    description = "Pause the game if 3 players vote for it during buy time",
    version = "1.0",
    url = "test.com"
};

new Handle:g_PauseMenu;
new bool:g_PauseVotes[MAXPLAYERS + 1];
new int:g_VoteCount;

public OnPluginStart() {
    g_PauseMenu = CreateMenu(PauseMenuHandler);
    SetMenuTitle(g_PauseMenu, "Pause Request");
    AddMenuItem(g_PauseMenu, "yes", "Yes");
    AddMenuItem(g_PauseMenu, "no", "No");

    RegConsoleCmd("sm_pause", Command_Pause);
    HookEvent("round_start", Event_RoundStart);
}

public Action:Command_Pause(client, args) {
    if (!IsPlayerAlive(client) || !IsBuyTime()) {
        PrintToChat(client, "You can only request a pause during buy time.");
        return Plugin_Handled;
    }

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i) && IsPlayerAlive(i)) {
            g_PauseVotes[i] = false; // Reset vote
            DisplayMenu(g_PauseMenu, i, 10);
        }
    }

    g_VoteCount = 0; 
    return Plugin_Handled;
}

public int PauseMenuHandler(Menu menu, MenuAction action, int client, int item) {
    if (action == MenuAction_End) {
        return 0;
    } else if (action == MenuAction_Select) {
        if (item == 0) { // "Yes" option
            g_PauseVotes[client] = true;
            g_VoteCount++;

            // Check if required votes are met
            if (g_VoteCount >= REQUIRED_VOTES) {
                PauseGame();
            }
        }
    }
    return 0;
}

public void PauseGame() {
    PrintToChatAll("Game will be paused for 1 minute and 30 seconds.");
    new Float:duration = PAUSE_DURATION;
    new Float:start_time = GetGameTime();
    new Handle:timer = CreateTimer(duration, Timer_UnpauseGame, _, TIMER_FLAG_NO_MAPCHANGE);

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            // Freeze all players
            new userId = GetClientUserId(i);
            FakeClientCommandEx(client, "cs_freeze %d", userId);
        }
    }
}

public Action:Timer_UnpauseGame(Handle:timer, any:data) {
    PrintToChatAll("Game is now unpaused.");

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            // Unfreeze all players
            new userId = GetClientUserId(i);
            FakeClientCommandEx(client, "cs_unfreeze %d", userId);
        }
    }

    return Plugin_Stop;
}

bool:IsBuyTime() {
    return true;
}

public void Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast) {
    for (int i = 1; i <= MaxClients; i++) {
        g_PauseVotes[i] = false;
    }
    g_VoteCount = 0;
}


#include <sourcemod>
#include <sdktools>

#define REQUIRED_VOTES 3

public Plugin myinfo = {
    name = "Pause Plugin",
    author = "Your Name",
    description = "Pause the game if 3 players vote for it during buy time",
    version = "1.0",
    url = "http://yourwebsite.com"
};

new Handle:g_PauseMenu;
new bool:g_PauseVotes[MAXPLAYERS + 1];
new int:g_VoteCount;
new float:g_RoundStartTime;
new bool:g_IsPaused;

public OnPluginStart() {
    g_PauseMenu = CreateMenu(PauseMenuHandler);
    SetMenuTitle(g_PauseMenu, "Pause Request");
    AddMenuItem(g_PauseMenu, "yes", "Yes");
    AddMenuItem(g_PauseMenu, "no", "No");

    RegConsoleCmd("sm_pause", Command_Pause);
    RegConsoleCmd("sm_unpause", Command_Unpause);
    HookEvent("round_start", Event_RoundStart);
}

public Action:Command_Pause(int client, int args) {
    if (!IsPlayerAlive(client) || !IsBuyTime()) {
        PrintToChat(client, "You can only request a pause during buy time.");
        return Plugin_Handled;
    }

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i) && IsPlayerAlive(i)) {
            g_PauseVotes[i] = false;
            DisplayMenu(g_PauseMenu, i, 10);
        }
    }

    g_VoteCount = 0;
    return Plugin_Handled;
}

public Action:Command_Unpause(int client, int args) {
    if (g_IsPaused) {
        UnpauseGame();
    } else {
        PrintToChat(client, "The game is not currently paused.");
    }
    return Plugin_Handled;
}

public int PauseMenuHandler(Menu menu, MenuAction action, int client, int item) {
    if (action == MenuAction_End) {
        return 0;
    } else if (action == MenuAction_Select) {
        if (item == 0) {
            g_PauseVotes[client] = true;
            g_VoteCount++;

            if (g_VoteCount >= REQUIRED_VOTES) {
                PauseGame();
            }
        }
    }
    return 0;
}

public void PauseGame() {
    PrintToChatAll("The game is now paused. Type !unpause to resume.");
    g_IsPaused = true;

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            int userId = GetClientUserId(i);
            FakeClientCommandEx(i, "cs_freeze %d", userId);
        }
    }
}

public void UnpauseGame() {
    PrintToChatAll("The game is now unpaused.");
    g_IsPaused = false;

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            int userId = GetClientUserId(i);
            FakeClientCommandEx(i, "cs_unfreeze %d", userId);
        }
    }
}

bool:IsBuyTime() {
    float currentTime = GetGameTime();
    if (currentTime - g_RoundStartTime <= 15.0) {
        return true;
    }
    return false;
}

public void Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast) {
    g_RoundStartTime = GetGameTime();
    for (int i = 1; i <= MaxClients; i++) {
        g_PauseVotes[i] = false;
    }
    g_VoteCount = 0;
}


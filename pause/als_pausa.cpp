
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

int g_VoteCount;
new Handle:g_PauseMenu;
new bool:g_PauseVotes[MAXPLAYERS + 1];
new Float:g_RoundStartTime;
new bool:g_IsPaused;

public OnPluginStart() {
    g_PauseMenu = CreateMenu(PauseMenuHandler);
    SetMenuTitle(g_PauseMenu, "Pausa?");
    AddMenuItem(g_PauseMenu, "yes", "Yes");
    AddMenuItem(g_PauseMenu, "no", "No");

    RegConsoleCmd("sm_pause", Command_Pause);
    RegConsoleCmd("sm_unpause", Command_Unpause);
    HookEvent("round_start", Event_RoundStart);
}

public Action:Command_Pause(client, args) {
    if (!IsPlayerAlive(client) || !IsBuyTime()) {
        PrintToChat(client, "Só é possível pausar o jogo no tempo de compra.");
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

public Action:Command_Unpause(client, args) {
    if (g_IsPaused) {
        UnpauseGame();
    } else {
        PrintToChat(client, "O jogo não está pausado.");
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
    PrintToChatAll("Jogo pausado. Digita !unpause para despausar.");
    g_IsPaused = true;

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            new userId = GetClientUserId(i);
            FakeClientCommandEx(i, "cs_freeze %d", userId);  // Using the correct client ID
        }
    }
}

public void UnpauseGame() {
    PrintToChatAll("Joga despausado.");
    g_IsPaused = false;

    for (int i = 1; i <= MaxClients; i++) {
        if (IsClientInGame(i)) {
            new userId = GetClientUserId(i);
            FakeClientCommandEx(i, "cs_unfreeze %d", userId);  // Using the correct client ID
        }
    }
}

bool:IsBuyTime() {
    new Float:currentTime = GetGameTime();
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



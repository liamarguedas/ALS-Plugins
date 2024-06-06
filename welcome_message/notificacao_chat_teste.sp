
#include <sourcemod>
#include <clientprefs>

public Plugin:myinfo = {
    name = "Welcome Message",
    author = "Your Name",
    description = "Displays a welcome message to users when they join the server.",
    version = "1.1",
    url = "http://yourwebsite.com"
};

public void OnPluginStart() {
    HookEvent("client_putinserver", Event_ClientPutInServer);
}

public void Event_ClientPutInServer(int client) {
    if (client <= 0 || client > MaxClients) {
        return;
    }

    if (!IsClientInGame(client)) {
        return;
    }
    
    char playerName[MAX_NAME_LENGTH];
    GetClientName(client, playerName, sizeof(playerName));
    
    char message[256];
    Format(message, sizeof(message), "Welcome to the server, %s!", playerName);
    
    PrintToChat(client, "%s", message);
}
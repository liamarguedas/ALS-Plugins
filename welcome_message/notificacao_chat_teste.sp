
#include <sourcemod>
#include <clientprefs>

public Plugin:myinfo = {
    name = "mensagem_bemvinda",
    author = "liano",
    description = "_",
    version = "1.1",
    url = "teste.com"
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
    Format(message, sizeof(message), "bem-vindo, %s!", playerName);
    
    PrintToChat(client, "%s", message);
}

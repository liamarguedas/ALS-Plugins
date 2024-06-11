#include <sourcemod>
#include <sdktools>

public Plugin:myinfo = 
{
    name = "Mensagem de bem-vida",
    author = "liano",
    description = "Envia uma mensagem de bem-vinda.",
    version = "1.0",
    url = "http://example.com"
};

public OnPluginStart()
{
    HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
}

public Action:Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
    int client = GetClientOfUserId(GetEventInt(event, "userid"));
    
    if (IsClientInGame(client) && !IsFakeClient(client))
    {
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
        PrintToChat(client, "Se inscreve no canal: youtube.com/@servidoralsbr");
    }
    return Plugin_Continue;
}

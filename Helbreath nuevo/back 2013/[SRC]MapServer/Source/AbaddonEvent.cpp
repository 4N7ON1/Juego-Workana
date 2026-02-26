
#include "../Header/Game.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <thread>
#include <algorithm>
#include <set>

extern class cCoreServer * c_map;
extern void PutLogList(char * cMsg);
extern void CharacterLogList(char * cMsg);
extern void ChatLogs(char * cMsg);
extern void Borrado();
extern void ConfigList(char * cMsg);
extern void UpdateConfigList(char* cMsg);
extern char G_cTxt[512];
extern char	G_cData50000[50000];
extern void PutLogHacksFileList(char * cStr);
extern void PutLogOnlinesFileList(char * cStr);
extern void PutLogDrops(char * cStr);
extern void PutGMLogData(char * cStr);
extern void PutEkAndPkLogFileList(char * cStr);
extern void PutLogItemsList(char * cStr);
extern void PutLogTradeFileList(char * cStr);
extern void PutLogCoinsFileList(char * cStr);

int cCoreServer::Aby_MobsCounter()
{
	int npcs = 0;
	for (int i = 1; i < DEF_MAXNPCS; i++)
	{
		auto mob = m_pNpcList[i];
		if (!mob) continue;

		if (mob->m_sType == 15 || mob->m_sType == 19 ||
			mob->m_sType == 20 || mob->m_sType == 24 ||
			mob->m_sType == 25 || mob->m_sType == 26 ||
			mob->m_sType == 67 || mob->m_sType == 68 ||
			mob->m_sType == 69 || mob->m_sType == 90 ||
			mob->m_sType == 91) continue;

		if (string(m_pMapList[mob->m_cMapIndex]->m_cName) != "maze") continue;

		npcs++;
	}

	return npcs;
}


Coord cCoreServer::getRandomSpawn() {
	int idx = DropRollDice(0, spawnCoords.size() - 1);
	return spawnCoords[idx];
}

void cCoreServer::InitAbyEvent()
{
	m_bAbaddonEventActive = false;
	m_dwAbaddonEventStartTime = 0;
	m_sBotLevel = 3;

	SummonApocaMobs();
	mobs_maze = 0;
	mobs_maze = Aby_MobsCounter();
	roa_event_mode = 1;
	m_dwLastGuardiansRespawnTime = 0;

	spawnCoords.clear();

	spawnCoords = {
		{ 46, 35 },
		{ 92, 30 },
		{ 108, 97 },
		{ 53, 104 },
		{ 112, 110 },
		{ 85, 122 },
		{ 55, 133 },
		{ 142, 102 },
		{ 98, 89 },
		{ 67, 90 },
		{ 85, 166 },
		{ 33, 114 },
		{ 169, 102 },
		{ 157, 82 },
		{ 145, 39 }
	};
	
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;

		Coord c = getRandomSpawn();
		RequestTeleportHandler(i, "2   ", "maze", c.x, c.y);
	}

	SendMazeMobCount();
	
	m_bBotProces = true;
}

void cCoreServer::SummonApocaMobs()
{
	CreateApocaMob("Hellclaw", 85, 119, 8, 3, "maze");
	CreateApocaMob("Tigerworm", 102, 116, 8, 3, "maze");
	CreateApocaMob("Dark-Elf", 112, 111, 20, 3, "maze");
	CreateApocaMob("Demon", 96, 108, 20, 3, "maze");
	CreateApocaMob("Hellclaw", 109, 99, 8, 3, "maze");
	CreateApocaMob("Gagoyle", 31, 113, 15, 3, "maze");
	CreateApocaMob("Tigerworm", 38, 110, 8, 3, "maze");	
	CreateApocaMob("Demon", 138, 98, 20, 3, "maze");
	CreateApocaMob("Nizie", 124, 152, 20, 3, "maze");
	CreateApocaMob("Dark-Elf", 81, 96, 20, 3, "maze");
	CreateApocaMob("Fire-Wyvern", 28, 44, 2, 3, "maze");
	CreateApocaMob("Wyvern", 41, 51, 3, 2, "maze");
	CreateApocaMob("Fire-Wyvern", 80, 28, 2, 3, "maze");
	CreateApocaMob("Wyvern", 88, 30, 3, 2, "maze");
	CreateApocaMob("Fire-Wyvern", 95, 36, 2, 3, "maze");
	CreateApocaMob("Wyvern", 103, 32, 2, 3, "maze");
}

void cCoreServer::CreateApocaMob(char * name, short x, short y, int quant, char side, char * smapname)
{
	char   cCity, cName_Master[10], cNpcName[256], cWaypoint[11], cSA, mapname[11];
	int    pX, pY, iNum, iNamingValue;
	BOOL   bMaster;
	ZeroMemory(cNpcName, sizeof(cNpcName));
	ZeroMemory(mapname, sizeof(mapname));
	strcpy(cNpcName, name);
	strcpy(mapname, smapname);
	iNum = 1;

	cSA = 0;
	pX = x;
	pY = y;

	cCity = side;

	auto map = m_pMapList[iGetMapIndex(mapname)];

	int MapIndex = -1;
	for (int z = 0; z < DEF_MAXMAPS; z++) 
	{
		if ((m_pMapList[z] != NULL) && (strcmp(m_pMapList[z]->m_cName, "maze") == 0)) 
		{
			iNamingValue = m_pMapList[z]->iGetEmptyNamingValue();
			if (iNamingValue != -1) 
			{
				MapIndex = z;
				break;
			}
		}
	}

	if (MapIndex != -1)
	{
		for (int i = 0; i < quant; i++)
		{
			iNamingValue = map->iGetEmptyNamingValue();
			if (iNamingValue != -1)
			{
				ZeroMemory(cName_Master, sizeof(cName_Master));
				wsprintf(cName_Master, "XX%d", iNamingValue);
				cName_Master[0] = '_';
				cName_Master[1] = MapIndex + 65;

				if ((bCreateNewNpc(name, cName_Master, smapname, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, NULL, NULL, -1, FALSE, FALSE, FALSE)) == FALSE)
				{
					map->SetNamingValueEmpty(iNamingValue);
				}
			}
		}
	}	
}

void cCoreServer::DeleteApocaNpcs()
{
	int npcs = 0;
	for (int i = 1; i < DEF_MAXNPCS; i++)
	{
		auto mob = m_pNpcList[i];
		if (!mob) continue;

		if (mob->m_sType == 15 || mob->m_sType == 19 ||
			mob->m_sType == 20 || mob->m_sType == 24 ||
			mob->m_sType == 25 || mob->m_sType == 26 ||
			mob->m_sType == 67 || mob->m_sType == 68 ||
			mob->m_sType == 69 || mob->m_sType == 90 ||
			mob->m_sType == 91 ||
			mob->m_sType == 111 || mob->m_sType == 129 || mob->m_sType == 130 || mob->m_sType == 131 || mob->m_sType == 134) continue;

		if (string(m_pMapList[mob->m_cMapIndex]->m_cName) == "maze" || string(m_pMapList[mob->m_cMapIndex]->m_cName) == "abymap")
		{
			DeleteNpc(i, true);
		}
	}
}
void cCoreServer::handleGuardiansRespawnTimer()
{
	if (!g_ev.Is(EventID::RiseOfAbaddon)) return;

	const DWORD dwCurrentTime = timeGetTime();
	DWORD dwFifteenMinutes = 0;

	if (roa_event_mode == 1)
	dwFifteenMinutes = 10 * 60 * 1000;
	else if (roa_event_mode == 2) dwFifteenMinutes = 5 * 60 * 1000;

	//const DWORD dwFifteenMinutes = 15000;

	if (dwCurrentTime - m_dwLastGuardiansRespawnTime < dwFifteenMinutes) {
		return;
	}
	m_dwLastGuardiansRespawnTime = dwCurrentTime;

	const char* targetMap = nullptr;
	if (roa_event_mode == 1) {
		targetMap = "maze";
	}
	else if (roa_event_mode == 2) {
		targetMap = "abymap";
	}
	else return;	

	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		auto cli = m_pClientList[i];
		if (!cli) continue;

		if (cli->m_bHelBot) {
			Coord c = getRandomSpawn();
			RequestTeleportHandler(i, "2   ", const_cast<char*>(targetMap), c.x, c.y);
			cli->m_bReachedGuardPoint = false;
			continue;
		}

		if (!cli->IsInMap(const_cast<char*>(targetMap))) continue;

		const int tarsX = cli->m_sX;
		const int tarsY = cli->m_sY;

		SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_MAGIC,
			cli->m_cMapIndex, tarsX, tarsY, tarsX, tarsY,
			181, cli->m_sType);

		const bool bAmp = (cli->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5);
		const bool bPfm = (cli->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2);

		int penaltyHP = 0;

		if (!bAmp && !bPfm) {
			penaltyHP = DropRollDice(600, 800);
		}
		else if (!bAmp && bPfm) {
			penaltyHP = DropRollDice(200, 400);
		}

		if (penaltyHP > 0) {
			cli->m_iHP -= penaltyHP;
			if (cli->m_iHP < 0) cli->m_iHP = 0;
			
			SendNotifyMsg(NULL, i, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);

			if (cli->m_iHP == 0) {
				ClientKilledHandler(i, NULL, NULL, penaltyHP);
				ShowClientMsg(i, "Has muerto!");
			}
			else {
				wsprintf(G_cTxt, "Perdiste %d de HP", penaltyHP);
				ShowClientMsg(i, G_cTxt);
			}
		}

		SendCommand(i, "/evil_laught", penaltyHP);
		SendAlertMsg(i, "Cuidado los guardianes han vuelto!");
	}
}

void cCoreServer::ChangeRoaEventState()
{
	roa_event_mode = 2;

	spawnCoords.clear();

	spawnCoords = {
		{ 57, 149 },
		{ 39, 118 },
		{ 49, 62 },
		{ 104, 37 },
		{ 149, 51 },
		{ 164, 77 },
		{ 170, 113 },
		{ 158, 127 },
		{ 137, 144 },
		{ 97, 167 },
		{ 72, 167 },
		{ 53, 129 },
		{ 125, 130 },
		{ 161, 94 },
		{ 150, 63 }
	};
	
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;

		if (m_pClientList[i]->IsInMap("maze"))
		{
			SendAlertMsg(i, "Se activo el teleport a Abaddon Map");
		}

		if (!m_pClientList[i]->m_bHelBot) continue;

		Coord c = getRandomSpawn();
		RequestTeleportHandler(i, "2   ", "abymap", c.x, c.y);
	}	
	
	CreateApocaMob("Abaddon", 99, 92, 1, 3, "abymap");
	m_dwLastGuardiansRespawnTime = 0;
}

void cCoreServer::handleGuardianDrop(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = DropRollDice(1, 10);
	if (dado == 5 || dado == 9)
	{
		int dado2 = DropRollDice(1, 3);
		switch (dado2)
		{
		case 1:
			CreateGuardianDrop(client, 2795, DropRollDice(5000, 7500), p->m_sX, p->m_sY); //Ice Segments
			break;

		case 2:
			CreateGuardianDrop(client, 2796, DropRollDice(5000, 7500), p->m_sX, p->m_sY); //LightningSegments
			break;

		case 3:
			CreateGuardianDrop(client, 2797, DropRollDice(5000, 7500), p->m_sX, p->m_sY); //FireSegments
			break;

		default:
			break;
		}
	}
}

void cCoreServer::CreateGuardianDrop(int client, int id, int count, int x, int y)
{
	if (m_pClientList[client] == NULL) return;
	CItem * it = new CItem;
	_bInitItemAttr(it, id);
	guardiandrop(client, id, count, x, y);
	delete it;
}

void cCoreServer::guardiandrop(int client, int iItemID, int count, int iX, int iY) {
	class CItem * pItem;
	pItem = new class CItem;
	if (m_pClientList[client] == NULL) return;
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
	}

	else {

		pItem->m_dwCount = count;
		m_pMapList[m_pClientList[client]->m_cMapIndex]->bSetItem(m_pClientList[client]->m_sX, m_pClientList[client]->m_sY, pItem);
		pItem->m_dwTimeDrop = timeGetTime();
		SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_pClientList[client]->m_cMapIndex, iX, iY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor, pItem->m_dwAttribute);
	}
}

void cCoreServer::SendMazeMobCount()
{
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;

		SendCommand(i, "/mazecount", mobs_maze);
	}
}

void cCoreServer::SendMazeMobCountToClient(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	SendCommand(client, "/mazecount", mobs_maze);
}

void cCoreServer::joinRoaEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->IsShopMap()) return;
	if (p->m_bIsKilled) return;

	if (roa_event_mode != 1 && roa_event_mode != 2) return;

	SendMazeMobCountToClient(client);

	if (roa_event_mode == 1)
	{
		if (p->IsLocation("aresden"))
		{
			RequestTeleportHandler(client, "2   ", "maze", 167, 171);
		}
		else
		{
			RequestTeleportHandler(client, "2   ", "maze", 38, 164);
		}
	}
	else if (roa_event_mode == 2)
	{
		RequestTeleportHandler(client, "2   ", "abymap", -1, -1);
	}
}

void cCoreServer::handleEventWhenAbaddonDeath()
{
	DWORD dwCurrentTime = timeGetTime();

	if (m_bAbaddonEventActive) {
		return;
	}

	m_bAbaddonEventActive = true;
	m_dwAbaddonEventStartTime = dwCurrentTime;

	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;

		SendAlertMsg(i, "Abaddon derrotado! El evento terminara en 1 minuto.");
	}
}

void cCoreServer::checkAbaddonEventTimer()
{
	if (!m_bAbaddonEventActive || !g_ev.Is(EventID::RiseOfAbaddon)) {
		return;
	}

	DWORD dwCurrentTime = timeGetTime();
	DWORD dwOneMinute = 60 * 1000; 

	if (dwCurrentTime - m_dwAbaddonEventStartTime >= dwOneMinute) {
	
		m_bAbaddonEventActive = false;
		if (g_ev.Is(EventID::RiseOfAbaddon)) g_ev.Deactivate(EventID::RiseOfAbaddon);

		for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
			if (m_pClientList[i] == NULL) continue;

			ShowClientMsg(i, "El evento de Abaddon ha terminado.");
		}		
	}
}
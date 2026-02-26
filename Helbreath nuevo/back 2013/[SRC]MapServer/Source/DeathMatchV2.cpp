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

//event config
static int maxkills = 100;
static int ekxkill = 3;
static int winreward1 = 10;

void cCoreServer::EnableDeathMatch()
{
	PutLogList("Deathmatch ON");
	if (ActiveEvent) return;
	ActiveEvent = TRUE;
	DWORD dwTime = timeGetTime();
	dw_DmTime = dwTime - 100;
	vec_dmfighters.clear();

	/*for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;

		join_eventDM(i);			
	}

	m_bBotProces = true;*/
}

void cCoreServer::DisableDeathmatch()
{
	ActiveEvent = FALSE;
	PutLogList("Deathmatch OFF");
	vec_dmfighters.clear();
}


void cCoreServer::join_eventDM(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!g_ev.Is(EventID::Deathmatch)) return;

	if (!p->m_bHelBot)
	{
		if (p->IsInMap("dm")) return;
	}

	if (!is_dmfighter(p->m_cCharName)) add_dmfighter(p->m_cCharName);
	RequestDismissPartyHandler(client);
	notify_dmpoints(client, get_dmkills(p->m_cCharName), get_dmdeaths(p->m_cCharName));
	RequestTeleportHandler(client, "2   ", "dm", -1 - 1);
	p->m_bIsSafeAttackMode = FALSE;
	RemoveFlags(client);
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
	sort_dmfighters();
	send_dmtop10(client);
}

void cCoreServer::getdmranking(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	sort_dmfighters();
	send_dmtop10(client);
}

void cCoreServer::send_dmtop10(int client)
{
	if (!g_ev.Is(EventID::Deathmatch)) return;
	auto p = m_pClientList[client];
	if (!p) return;

	char data[1024];
	auto * cp = data;

	Push(cp, (u32)NOT_DMTOP);
	Push(cp, (u16)0);

	short sendfighters = 0;
	if (vec_dmfighters.size() > 10) sendfighters = 10; else sendfighters = vec_dmfighters.size();

	Push(cp, sendfighters);

	for (int i = 0; i < sendfighters; i++)
	{
		Push(cp, vec_dmfighters[i].fighter, 12);
		Push(cp, vec_dmfighters[i].kills);
		Push(cp, vec_dmfighters[i].deaths);
	}

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::add_dmfighter(char * charname)
{
	if (!g_ev.Is(EventID::Deathmatch)) return;

	st_dmfighters u;

	ZeroMemory(u.fighter, sizeof(u.fighter));
	strcpy(u.fighter, charname);
	u.kills = 0;
	u.deaths = 0;

	vec_dmfighters.push_back(u);
}

bool cCoreServer::is_dmfighter(char * charname)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0) return true;
	}

	return false;
}

int cCoreServer::get_dmkills(char * charname)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0) return vec_dmfighters[i].kills;
	}

	return 0;
}

int cCoreServer::get_dmdeaths(char * charname)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0) return vec_dmfighters[i].deaths;
	}

	return 0;
}

void cCoreServer::remove_dmfighter(char * charname)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0)
			vec_dmfighters.erase(vec_dmfighters.begin() + i);
	}
}

void cCoreServer::sort_dmfighters()
{
	auto sortFunc = [](st_dmfighters a, st_dmfighters b) -> bool {
		return a.kills > b.kills;
	};

	sort(vec_dmfighters.begin(), vec_dmfighters.end(), sortFunc);
}

void cCoreServer::notify_dmpoints(int client, int kills, int deaths)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[512];
	auto * cp = data;

	Push(cp, (u32)NOT_DMPOINTS);
	Push(cp, (u16)0);

	Push(cp, kills);
	Push(cp, deaths);

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}


void cCoreServer::request_dmrevive(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
	SendCommand(client, "/revive");
}

void cCoreServer::dmrevive(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	p->requestrevive = false;
	p->m_bIsKilled = FALSE;
	p->m_iHP = iGetMaxHP(client);
	p->m_iMP = (((m_pClientList[client]->m_iMag + m_pClientList[client]->m_iAngelicMag) * 2) + (m_pClientList[client]->m_iLevel / 2)) + (m_pClientList[client]->m_iInt + m_pClientList[client]->m_iAngelicInt) / 2;
	p->m_iSP = ((m_pClientList[client]->m_iStr + m_pClientList[client]->m_iAngelicStr) * 2) + (m_pClientList[client]->m_iLevel / 2);
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
	RequestTeleportHandler(client, "2   ", p->m_cMapName, -1, -1);
}

void cCoreServer::dmnot_revive(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	p->requestrevive = false;
	p->m_bIsKilled = FALSE;
	p->m_iHP = iGetMaxHP(client);
	p->m_iMP = (((m_pClientList[client]->m_iMag + m_pClientList[client]->m_iAngelicMag) * 2) + (m_pClientList[client]->m_iLevel / 2)) + (m_pClientList[client]->m_iInt + m_pClientList[client]->m_iAngelicInt) / 2;
	p->m_iSP = ((m_pClientList[client]->m_iStr + m_pClientList[client]->m_iAngelicStr) * 2) + (m_pClientList[client]->m_iLevel / 2);
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
	if (p->IsLocation("elvine"))
		RequestTeleportHandler(client, "2   ", "elvine", -1, -1);
	else
		RequestTeleportHandler(client, "2   ", "aresden", -1, -1);
}

void cCoreServer::dmkill_enemy(int att, int tar)
{
	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	if (tar != att)
	{
		attacker->m_iEnemyKillCount += ekxkill;
		SendCommand(attacker->client, "/eks", attacker->m_iEnemyKillCount);

		attacker->m_iEventPoints++;
		SendCommand(attacker->client, "/eventpoints", attacker->m_iEventPoints);

		dmadd_kills(attacker->m_cCharName, 1);
	}

	dmadd_deaths(target->m_cCharName, 1);
	request_dmrevive(tar);
	dmnotify(att, tar);
}

void cCoreServer::dmadd_kills(char * charname, int iadd)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0)
		{
			vec_dmfighters[i].kills += iadd;
			return;
		}
	}
}

void cCoreServer::dmadd_deaths(char * charname, int iadd)
{
	for (int i = 0; i < vec_dmfighters.size(); i++){
		if (strcmp(vec_dmfighters[i].fighter, charname) == 0)
		{
			vec_dmfighters[i].deaths += iadd;
			return;
		}
	}
}

void cCoreServer::dmnotify(int att, int tar)
{
	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	attacker->Send(att, CLIENT_NOTIFY_ENEMYKILLREWARD, tar);
	auto attackerkills = get_dmkills(attacker->m_cCharName);
	notify_dmpoints(att, attackerkills, get_dmdeaths(attacker->m_cCharName));
	notify_dmpoints(tar, get_dmkills(target->m_cCharName), get_dmdeaths(target->m_cCharName));

	if (attackerkills == maxkills)
	{
		SendCommand(att, "/battlewinner");
		DMWinnerReward(att);
	}
}

void cCoreServer::EndDeathmatch(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (pi == p) continue;
		wsprintf(G_cTxt, "%s is the Deathmatch Winner!", p->m_cCharName);
		ShowClientMsg(i, G_cTxt);
	}

	ShowClientMsg(client, "You won DeathMatch!");
	if (g_ev.Is(EventID::Deathmatch)) g_ev.Deactivate(EventID::Deathmatch);

}

void cCoreServer::DMWinnerReward(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_iTokens += winreward1;
	SendCommand(client, "/tokens", p->m_iTokens);
	wsprintf(G_cTxt, "You got %d tokens", winreward1);
	ShowClientMsg(client, G_cTxt);

	EndDeathmatch(client);
}

int cCoreServer::GetIndexByNameDM(char * charname)
{
	int result = -1;

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (string(pi->m_cCharName) != charname) continue;
		result = i;
		break;
	}

	return result;
}
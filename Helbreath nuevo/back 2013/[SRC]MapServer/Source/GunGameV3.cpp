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
static int maxkills = 50;
static int ekxkill = 2;
static int winreward = 7;


void cCoreServer::EnableGunGame()
{
	PutLogList("Gungame ON");
	vec_ggfighters.clear();

	ZeroMemory(cggwinner, sizeof(cggwinner));
	wsprintf(cggwinner, "#");
}

void cCoreServer::DisableGunGame()
{
	FinishGunGameEvent();
}

void cCoreServer::join_ggevent(int client, short combattype, short angelic)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	if (!g_ev.Is(EventID::GunGame)) return;
	if (p->IsInMap("gg")) return;

	RequestDismissPartyHandler(client);

	if (!is_ggfighter(p->m_cCharName)) add_ggfighter(p->m_cCharName);

	int fighter_index = get_ggfighterindex(p->m_cCharName);
	if (fighter_index == -1) return;

	LocalSavePlayerData(client);

	RequestTeleportHandler(client, "2   ", "gg");

	if (p->m_iAdminUserLevel != 0) return;

	if (p->IsInMap("gg") && is_ggfighter(p->m_cCharName))
	{
		handlePlayerForEvent(client, combattype);

		handleAngelicActions(client);
		SavePlayerItemForEvent(client);

		p->ClearBag();

		vec_ggfighters[fighter_index].combattype = combattype;
		vec_ggfighters[fighter_index].client_index = client;
		vec_ggfighters[fighter_index].angelic = angelic;

		MakeItems(client, combattype);

		NotifyGunGameItems(client);
		LearnMagics(client);
		AutoSkill(client);

		if (vec_ggfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			SendCriticals(client);
		}

		RemoveFlags(client);

		notify_ggpoints(client, get_ggkills(p->m_cCharName), get_ggdeaths(p->m_cCharName));

		p->m_bIsSafeAttackMode = FALSE;
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
		/*sort_ggfighters();
		send_top10(client);*/
		getggranking(client);
	}
}

static bool compareKills(const st_ggfighters& a, const st_ggfighters& b) {
	return a.kills > b.kills;
}

void cCoreServer::getggranking(int client) {
	return;
	// Simular cliente y conexión
	auto p = m_pClientList[client];
	if (!p) return;

	// Ordenar el vector por kills de mayor a menor
	std::sort(vec_ggfighters.begin(), vec_ggfighters.end(), compareKills);

	/*// Imprimir el Top 10
	std::cout << "Top 10 jugadores con más kills:\n";
	size_t limit = (vec_ggfighters.size() < 10) ? vec_ggfighters.size() : 10;
	for (size_t i = 0; i < limit; ++i) {
	std::cout << i + 1 << ". " << vec_ggfighters[i].fighter
	<< " - Kills: " << vec_ggfighters[i].kills << "\n";
	}*/


	char data[1024];
	auto * cp = data;

	Push(cp, (u32)NOT_GGTOP);
	Push(cp, (u16)0);

	short sendfighters = 0;
	if (vec_ggfighters.size() > 10) sendfighters = 10; else sendfighters = vec_ggfighters.size();

	Push(cp, sendfighters);

	for (int i = 0; i < sendfighters; i++)
	{
		Push(cp, vec_ggfighters[i].fighter, 12);
		//	PutLogList(vec_ggfighters[i].fighter);
		Push(cp, vec_ggfighters[i].kills);
		//	PutLogList((char*)to_string(vec_ggfighters[i].kills).c_str());
	}

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::send_ggtop10(int client)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::GunGame)) return;

	char data[1024];
	auto * cp = data;

	Push(cp, (u32)NOT_DMTOP);
	Push(cp, (u16)0);

	short sendfighters = 0;
	if (vec_ggfighters.size() > 10) sendfighters = 10; else sendfighters = vec_ggfighters.size();

	Push(cp, sendfighters);

	for (int i = 0; i < sendfighters; i++)
	{
		Push(cp, vec_ggfighters[i].fighter, 12);
		Push(cp, vec_ggfighters[i].kills);
		Push(cp, vec_ggfighters[i].deaths);
	}

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::add_ggfighter(char * charname)
{
	if (!g_ev.Is(EventID::GunGame)) return;

	st_ggfighters u;

	ZeroMemory(u.fighter, sizeof(u.fighter));
	strcpy(u.fighter, charname);
	u.kills = 0;
	u.deaths = 0;
	u.level = 1;

	vec_ggfighters.push_back(u);
}

bool cCoreServer::is_ggfighter(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0) return true;
	}

	return false;
}

int cCoreServer::get_ggkills(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0) return vec_ggfighters[i].kills;
	}

	return 0;
}

int cCoreServer::get_ggfighterindex(char * charname)
{
	int result = -1;
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0)
		{
			result = i;
			break;
		}
	}

	return result;
}

int cCoreServer::get_ggdeaths(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0) return vec_ggfighters[i].deaths;
	}

	return 0;
}

void cCoreServer::remove_ggfighter(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0)
			vec_ggfighters.erase(vec_ggfighters.begin() + i);
	}
}

void cCoreServer::sort_ggfighters()
{
	auto sortFunc = [](st_ggfighters a, st_ggfighters b) -> bool {
		return a.kills > b.kills;
	};

	sort(vec_ggfighters.begin(), vec_ggfighters.end(), sortFunc);
}

void cCoreServer::notify_ggpoints(int client, int kills, int deaths)
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

void cCoreServer::kill_enemyGG(int att, int tar)
{
	if (!g_ev.Is(EventID::GunGame)) return;

	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	if (tar != att)
	{
		/*	attacker->m_iEnemyKillCount += ekxkill;
		c->SendCommand(attacker->client, "/eks", attacker->m_iEnemyKillCount);*/
		add_killsGG(att, 1);
	}

	add_deathsGG(target->m_cCharName, 1);
	request_reviveGG(tar);
	notifyGG(att, tar);
}


void cCoreServer::kill_enemyGG2(int att)
{
	if (!g_ev.Is(EventID::GunGame)) return;
	auto attacker = m_pClientList[att];
	if (!attacker) return;

	if (att)
	{
		add_killsGG(att, 1);
	}

	notifyGG2(att);
}

void cCoreServer::request_reviveGG(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
	SendCommand(client, "/revive");
}

void cCoreServer::reviveGG(int client)
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

	int fighter_index = get_ggfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		//p->ClearBagGG();
		//c->MakeItems(client, vec_ggfighters[fighter_index].combattype);
		//NotifyGunGameItems(client);

		for (int i = 0; i < DEF_MAXITEMS; i++) ReqRepairItemCofirmHandler(client, i, NULL);

		ReloadCandys(client);
		SendCommand(client, "/acomodarcandys");

		if (vec_ggfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			SendCriticals(client);
		}
	}
}

void cCoreServer::not_reviveGG(int client)
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
		RequestTeleportHandler(client, "2   ", "gshop_2", -1, -1);
	else
		RequestTeleportHandler(client, "2   ", "gshop_1", -1, -1);
}

/*

void cCoreServer::add_kills(char * charname, int iadd)
{
int fighter_index = get_fighterindex(charname);
if (fighter_index != -1)
{
//entre 0 y 19 nivel 1
//entre 20 y 39 nivel 2
//entre 40 y 59 nivel 3
//entre 60 y 79 nivel 4
//entre 80 y 100 nivel 5

vec_ggfighters[fighter_index].kills += iadd;

vec_ggfighters[fighter_index].level = ;

}
}
*/

void cCoreServer::add_killsGG(int client, int iadd)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int fighter_index = get_ggfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int previous_level = vec_ggfighters[fighter_index].level;
		vec_ggfighters[fighter_index].kills += iadd;
		int kills = vec_ggfighters[fighter_index].kills;

		if (kills >= 100)
		{
			char message[128];
			wsprintf(message, "¡Felicidades! Has ganado el Gungame.");
			SendAlertMsg(client, message);
			return;
		}

		if (kills >= 0 && kills < 10)
			vec_ggfighters[fighter_index].level = 1;
		else if (kills >= 10 && kills < 20)
			vec_ggfighters[fighter_index].level = 2;
		else if (kills >= 20 && kills < 30)
			vec_ggfighters[fighter_index].level = 3;
		else if (kills >= 30 && kills < 40)
			vec_ggfighters[fighter_index].level = 4;
		else if (kills >= 40)
			vec_ggfighters[fighter_index].level = 5;

		if (vec_ggfighters[fighter_index].level != previous_level)
		{
			ClearBagGG(client);
			MakeItems(client, vec_ggfighters[fighter_index].combattype);
			NotifyGunGameItems(client);

			if (vec_ggfighters[fighter_index].combattype == 1)
			{
				p->m_iSuperAttackLeft = 5000;
				SendCriticals(client);
			}

			char message[128];
			wsprintf(message, "¡Felicidades! Has alcanzado el nivel %d.", vec_ggfighters[fighter_index].level);
			SendAlertMsg(client, message);
		}
	}
}

void cCoreServer::add_deathsGG(char * charname, int iadd)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0)
		{
			vec_ggfighters[i].deaths += iadd;
			return;
		}
	}
}

void cCoreServer::notifyGG(int att, int tar)
{
	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	attacker->Send(att, CLIENT_NOTIFY_ENEMYKILLREWARD, tar);
	auto attackerkills = get_ggkills(attacker->m_cCharName);
	notify_ggpoints(att, attackerkills, get_ggdeaths(attacker->m_cCharName));
	notify_ggpoints(tar, get_ggkills(target->m_cCharName), get_ggdeaths(target->m_cCharName));

	if (attackerkills == maxkills)
	{
		End_GunGame_Event(att);
	}
}


void cCoreServer::notifyGG2(int att)
{
	auto attacker = m_pClientList[att];
	if (!attacker) return;

	auto attackerkills = get_ggkills(attacker->m_cCharName);
	notify_ggpoints(att, attackerkills, get_ggdeaths(attacker->m_cCharName));

	if (attackerkills == maxkills)
	{
		End_GunGame_Event(att);
	}
}

void cCoreServer::End_GunGame_Event(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	ZeroMemory(cggwinner, sizeof(cggwinner));
	wsprintf(cggwinner, p->m_cCharName);

	if (g_ev.Is(EventID::GunGame)) g_ev.Deactivate(EventID::GunGame);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		wsprintf(G_cTxt, "%s es el ganador del gungame!", p->m_cCharName);
		ShowClientMsg(i, G_cTxt);
	}
}

int cCoreServer::getGGPlayerLevel(int client)
{
	auto p = m_pClientList[client];
	if (!p) return -1;

	int result = -1;
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, p->m_cCharName) == 0)
		{
			result = vec_ggfighters[i].level;
			break;
		}
	}

	return result;
}

void cCoreServer::get_joininfoGG(int client, char * data)
{
	short id;
	auto p = m_pClientList[client];
	if (!p) return;

	short combattype = 0;
	short angelic = 0;

	Pop(data, combattype);
	Pop(data, angelic);

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	join_ggevent(client, combattype, angelic);
}

string cCoreServer::findGGLeader() {

	string result = "#";
	int maxKills = 0;
	std::string leaders;

	for (const auto& fighter : vec_ggfighters) {
		if (fighter.kills > maxKills) {
			maxKills = fighter.kills;
		}
	}

	for (const auto& fighter : vec_ggfighters) {
		if (fighter.kills == maxKills && maxKills > 0) {
			if (!leaders.empty()) {
				leaders += " - ";
			}
			leaders += fighter.fighter;
		}
	}

	if (!leaders.empty()) {
		if (leaders.find('-') != std::string::npos) {
			result = "Lideres: ";
			result.append(leaders);
		}
		else {
			result = "Lider: ";
			result.append(leaders);
		}
	}
	else {
		result = "#";
	}

	return result;
}


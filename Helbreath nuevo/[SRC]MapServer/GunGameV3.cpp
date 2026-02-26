#include "../Header/Game.h"
#include "../Header/Rank.h"
#include "..\\Header\\deathmatch.h"
#include "..\\Header\\gungame.h"
#include "..\\Header\\QuestList.h"
#include "..\\Trades.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <thread>
#include <algorithm>
#include <set>

extern class cResolution * c_reso;
extern class cCoreServer * c_map;
extern class CDeathmatch * c_dm;
extern class CGungame * c_gungame;
extern class CRank * c_rank;
extern class Trades * c_trades;
extern class QuestList * c_questlist;
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

//demonslayer
//lb
//kb
//gbh
//devastator

//ms30llf
//resu
//kw
//zw ms10
//zw ms20

cCoreServer::cCoreServer()
{
	vec_ggfighters.clear();
}

cCoreServer::~cCoreServer()
{

}

void cCoreServer::enable()
{
	PutLogList("Gungame ON");
	vec_ggfighters.clear();

	ZeroMemory(cwinner, sizeof(cwinner));
	wsprintf(cwinner, "#");
}

void cCoreServer::disable()
{
	c_map->FinishGunGameEvent();
}

void cCoreServer::join_event(int client, short combattype, short angelic)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	if (!g_ev.Is(EventID::GunGame)) return;
	if (p->IsInMap("gg")) return;

	c_map->RequestDismissPartyHandler(client);

	if (!is_ggfighter(p->m_cCharName)) add_ggfighter(p->m_cCharName);

	int fighter_index = get_fighterindex(p->m_cCharName);
	if (fighter_index == -1) return;

	c_map->LocalSavePlayerData(client);

	c_map->RequestTeleportHandler(client, "2   ", "gg");

	if (p->m_iAdminUserLevel != 0) return;

	if (p->IsInMap("gg") && is_ggfighter(p->m_cCharName))
	{
		c_map->handlePlayerForEvent(client, combattype);

		c_map->handleAngelicActions(client);
		c_map->SavePlayerItemForEvent(client);

		p->ClearBag();

		vec_ggfighters[fighter_index].combattype = combattype;
		vec_ggfighters[fighter_index].client_index = client;
		vec_ggfighters[fighter_index].angelic = angelic;

		c_map->MakeItems(client, combattype);

		c_map->NotifyGunGameItems(client);
		c_map->LearnMagics(client);
		c_map->AutoSkill(client);

		if (vec_ggfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			c_map->SendCriticals(client);
		}

		c_map->RemoveFlags(client);

		notify_points(client, get_kills(p->m_cCharName), get_deaths(p->m_cCharName));

		p->m_bIsSafeAttackMode = FALSE;
		c->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
		/*sort_ggfighters();
		send_top10(client);*/
		getranking(client);
	}
}

static bool compareKills(const st_ggfighters& a, const st_ggfighters& b) {
	return a.kills > b.kills;
}

void cCoreServer::getranking(int client) {
	return;
	// Simular cliente y conexión
	auto c = c_map;
	auto p = c->m_pClientList[client];
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

	c_map->m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::send_top10(int client)
{
	return;
	auto c = c_map;
	if (!g_ev.Is(EventID::GunGame)) return;
	auto p = c->m_pClientList[client];
	if (!p) return;

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

	c_map->m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::add_ggfighter(char * charname)
{
	auto c = c_map;
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

int cCoreServer::get_kills(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0) return vec_ggfighters[i].kills;
	}

	return 0;
}

int cCoreServer::get_fighterindex(char * charname)
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

int cCoreServer::get_deaths(char * charname)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0) return vec_ggfighters[i].deaths;
	}

	return 0;
}

void cCoreServer::remove_fighter(char * charname)
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

void cCoreServer::notify_points(int client, int kills, int deaths)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	char data[512];
	auto * cp = data;

	Push(cp, (u32)NOT_DMPOINTS);
	Push(cp, (u16)0);

	Push(cp, kills);
	Push(cp, deaths);

	c_map->m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::kill_enemy(int att, int tar)
{
	if (!g_ev.Is(EventID::GunGame)) return;

	auto c = c_map;
	auto attacker = c->m_pClientList[att];
	auto target = c->m_pClientList[tar];
	if (!attacker || !target) return;

	if (tar != att)
	{
		/*	attacker->m_iEnemyKillCount += ekxkill;
		c->SendCommand(attacker->client, "/eks", attacker->m_iEnemyKillCount);*/
		add_kills(att, 1);
	}

	add_deaths(target->m_cCharName, 1);
	request_revive(tar);
	notify(att, tar);
}


void cCoreServer::kill_enemy2(int att)
{
	if (!g_ev.Is(EventID::GunGame)) return;
	auto c = c_map;
	auto attacker = c->m_pClientList[att];
	if (!attacker) return;

	if (att)
	{
		add_kills(att, 1);
	}

	notify2(att);
}

void cCoreServer::request_revive(int client)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
	c->SendCommand(client, "/revive");
}

void cCoreServer::revive(int client)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	p->requestrevive = false;
	p->m_bIsKilled = FALSE;
	p->m_iHP = c->iGetMaxHP(client);
	p->m_iMP = (((c->m_pClientList[client]->m_iMag + c->m_pClientList[client]->m_iAngelicMag) * 2) + (c->m_pClientList[client]->m_iLevel / 2)) + (c->m_pClientList[client]->m_iInt + c->m_pClientList[client]->m_iAngelicInt) / 2;
	p->m_iSP = ((c->m_pClientList[client]->m_iStr + c->m_pClientList[client]->m_iAngelicStr) * 2) + (c->m_pClientList[client]->m_iLevel / 2);
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	c->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
	c->RequestTeleportHandler(client, "2   ", p->m_cMapName, -1, -1);

	int fighter_index = get_fighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		//p->ClearBagGG();
		//c->MakeItems(client, vec_ggfighters[fighter_index].combattype);
		//c_map->NotifyGunGameItems(client);

		for (int i = 0; i < DEF_MAXITEMS; i++) c_map->ReqRepairItemCofirmHandler(client, i, NULL);

		c_map->ReloadCandys(client);
		c_map->SendCommand(client, "/acomodarcandys");

		if (vec_ggfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			c_map->SendCriticals(client);
		}
	}
}

void cCoreServer::not_revive(int client)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	p->requestrevive = false;
	p->m_bIsKilled = FALSE;
	p->m_iHP = c->iGetMaxHP(client);
	p->m_iMP = (((c->m_pClientList[client]->m_iMag + c->m_pClientList[client]->m_iAngelicMag) * 2) + (c->m_pClientList[client]->m_iLevel / 2)) + (c->m_pClientList[client]->m_iInt + c->m_pClientList[client]->m_iAngelicInt) / 2;
	p->m_iSP = ((c->m_pClientList[client]->m_iStr + c->m_pClientList[client]->m_iAngelicStr) * 2) + (c->m_pClientList[client]->m_iLevel / 2);
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	c->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
	if (p->IsLocation("elvine"))
		c->RequestTeleportHandler(client, "2   ", "whouse", -1, -1);
	else
		c->RequestTeleportHandler(client, "2   ", "whouse", -1, -1);
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

void cCoreServer::add_kills(int client, int iadd)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	int fighter_index = get_fighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int previous_level = vec_ggfighters[fighter_index].level;
		vec_ggfighters[fighter_index].kills += iadd;
		int kills = vec_ggfighters[fighter_index].kills;

		if (kills >= 100)
		{
			char message[128];
			wsprintf(message, "¡Felicidades! Has ganado el Gungame.");
			c_map->SendAlertMsg(client, message);
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
			c_map->ClearBagGG(client);
			c_map->MakeItems(client, vec_ggfighters[fighter_index].combattype);
			c_map->NotifyGunGameItems(client);

			if (vec_ggfighters[fighter_index].combattype == 1)
			{
				p->m_iSuperAttackLeft = 5000;
				c_map->SendCriticals(client);
			}

			char message[128];
			wsprintf(message, "¡Felicidades! Has alcanzado el nivel %d.", vec_ggfighters[fighter_index].level);
			c_map->SendAlertMsg(client, message);
		}
	}
}

void cCoreServer::add_deaths(char * charname, int iadd)
{
	for (int i = 0; i < vec_ggfighters.size(); i++){
		if (strcmp(vec_ggfighters[i].fighter, charname) == 0)
		{
			vec_ggfighters[i].deaths += iadd;
			return;
		}
	}
}

void cCoreServer::notify(int att, int tar)
{
	auto g = c_map;
	auto attacker = g->m_pClientList[att];
	auto target = g->m_pClientList[tar];
	if (!attacker || !target) return;

	attacker->Send(att, CLIENT_NOTIFY_ENEMYKILLREWARD, tar);
	auto attackerkills = get_kills(attacker->m_cCharName);
	notify_points(att, attackerkills, get_deaths(attacker->m_cCharName));
	notify_points(tar, get_kills(target->m_cCharName), get_deaths(target->m_cCharName));

	if (attackerkills == maxkills)
	{
		End(att);
	}
}


void cCoreServer::notify2(int att)
{
	auto g = c_map;
	auto attacker = g->m_pClientList[att];
	if (!attacker) return;

	auto attackerkills = get_kills(attacker->m_cCharName);
	notify_points(att, attackerkills, get_deaths(attacker->m_cCharName));

	if (attackerkills == maxkills)
	{
		End(att);
	}
}

void cCoreServer::End(int client)
{
	auto c = c_map;
	auto p = c->m_pClientList[client];
	if (!p) return;

	ZeroMemory(cwinner, sizeof(cwinner));
	wsprintf(cwinner, p->m_cCharName);

	if (g_ev.Is(EventID::GunGame)) g_ev.Deactivate(EventID::GunGame);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = c->m_pClientList[i];
		if (!pi) continue;
		wsprintf(G_cTxt, "%s es el ganador del gungame!", p->m_cCharName);
		c->ShowClientMsg(i, G_cTxt);
	}
}

int cCoreServer::getPlayerLevel(int client)
{
	auto g = c_map;
	auto p = g->m_pClientList[client];
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

void cCoreServer::get_joininfo(int client, char * data)
{
	short id;
	auto p = c_map->m_pClientList[client];
	if (!p) return;

	short combattype = 0;
	short angelic = 0;

	Pop(data, combattype);
	Pop(data, angelic);

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	join_event(client, combattype, angelic);
}

string cCoreServer::findLeader() {

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


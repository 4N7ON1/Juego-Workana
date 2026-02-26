#include "../Header/Game.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <set>

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

//DK EVENT START CODE
extern char G_cTxt[512];
//event config

void cCoreServer::EnableDkEvent()
{
	m_idkareskills = 0;
	m_idkelvkills = 0;
	vec_dkfighters.clear();
}

void cCoreServer::DisableDkEvent()
{
	handleActionsWhenEventFinish();
	m_idkareskills = 0;
	m_idkelvkills = 0;
}

void cCoreServer::join_dkevent(int client, short combattype, short angelic)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	if (!g_ev.Is(EventID::DkEvent)) return;
	if (p->IsInMap("dkshop1") || p->IsInMap("dkshop2") || p->IsInMap("dkcity")) return;

	if (p->m_iLevel < 200)
	{
		ShowClientMsg(client, "Solo puedes ir al evento siendo nivel maximo.");
		return;
	}
	
	if (!is_dkfighter(p->m_cCharName)) add_dkfighter(p->m_cCharName);

	int fighter_index = get_dkfighterindex(p->m_cCharName);
	if (fighter_index == -1) return;

	LocalSavePlayerData(client);

	if (p->IsLocation("aresden")) RequestTeleportHandler(client, "2   ", "dkshop2"); else RequestTeleportHandler(client, "2   ", "dkshop1");

	if (p->m_iAdminUserLevel != 0) return;

	if (p->IsInMap("dkshop2") && is_dkfighter(p->m_cCharName) ||
		p->IsInMap("dkshop1") && is_dkfighter(p->m_cCharName))
	{

		p->m_iHP = iGetMaxHP(client);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);

		handlePlayerForDkEvent(client, combattype);

		handleAngelicActions_DkEvent(client);
		SavePlayerItemForEventDkEvent(client);

		p->ClearBag();

		vec_dkfighters[fighter_index].combattype = combattype;
		vec_dkfighters[fighter_index].client_index = client;
		vec_dkfighters[fighter_index].angelic = angelic;

		MakeItemsDkEvent(client, combattype);

		NotifyDkItems(client);
		LearnMagicsDkEvent(client);
		AutoSkill(client);

		if (vec_dkfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			SendCriticals(client);
		}

		RemoveFlags(client);

		if (vec_dkfighters[fighter_index].combattype == 2)
		{
			DkManaMe(client);
		}

		notify_dkeventpoints(client, get_dkeventkills(p->m_cCharName), get_dkdeaths(p->m_cCharName));
		NotifyCitysKills(client);
	}
}

void cCoreServer::add_dkfighter(char * charname)
{
	if (!g_ev.Is(EventID::DkEvent)) return;

	st_dkfighters u;

	ZeroMemory(u.fighter, sizeof(u.fighter));
	strcpy(u.fighter, charname);
	u.kills = 0;
	u.deaths = 0;
	u.level = 1;

	vec_dkfighters.push_back(u);
}

bool cCoreServer::is_dkfighter(char * charname)
{
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0) return true;
	}

	return false;
}

int cCoreServer::get_dkeventkills(char * charname)
{
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0) return vec_dkfighters[i].kills;
	}

	return 0;
}

int cCoreServer::get_dkfighterindex(char * charname)
{
	int result = -1;
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0)
		{
			result = i;
			break;
		}
	}

	return result;
}

int cCoreServer::get_dkdeaths(char * charname)
{
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0) return vec_dkfighters[i].deaths;
	}

	return 0;
}

void cCoreServer::remove_dkfighter(char * charname)
{
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0)
			vec_dkfighters.erase(vec_dkfighters.begin() + i);
	}
}

void cCoreServer::notify_dkeventpoints(int client, int kills, int deaths)
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

void cCoreServer::kill_dkeventenemy(int att, int tar)
{
	if (!g_ev.Is(EventID::DkEvent)) return;

	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	if (tar != att)
	{
		dkeventadd_kills(att, 1);
	}

	dkeventadd_deaths(target->m_cCharName, 1);
	dkeventnotify(att, tar);
}


void cCoreServer::request_revivedkevent(int client)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
	SendCommand(client, "/revive");
}

void cCoreServer::dkeventrevive(int client)
{
	return;
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

	int fighter_index = get_dkfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		for (int i = 0; i < DEF_MAXITEMS; i++) ReqRepairItemCofirmHandler(client, i, NULL);

		ReloadDkCandys(client);
		SendCommand(client, "/acomodarcandys");

		if (vec_dkfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			SendCriticals(client);
		}
	}

	calcItemsEffectsDk(client);
	AutoSkill(client);
}

void cCoreServer::dkeventnot_revive(int client)
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

void cCoreServer::dkeventadd_kills(int client, int iadd)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int fighter_index = get_dkfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int previous_level = vec_dkfighters[fighter_index].level;
		vec_dkfighters[fighter_index].kills += iadd;
	}
}

void cCoreServer::dkeventadd_deaths(char * charname, int iadd)
{
	for (int i = 0; i < vec_dkfighters.size(); i++){
		if (strcmp(vec_dkfighters[i].fighter, charname) == 0)
		{
			vec_dkfighters[i].deaths += iadd;
			return;
		}
	}
}

void cCoreServer::dkeventnotify(int att, int tar)
{
	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	attacker->Send(att, CLIENT_NOTIFY_ENEMYKILLREWARD, tar);
	auto attackerkills = get_dkeventkills(attacker->m_cCharName);
	notify_dkeventpoints(att, attackerkills, get_dkdeaths(attacker->m_cCharName));
	notify_dkeventpoints(tar, get_dkeventkills(target->m_cCharName), get_dkdeaths(target->m_cCharName));

	if (attacker->IsLocation("aresden")) { 
		m_idkareskills++;
	}
	else if (attacker->IsLocation("elvine")) { 
		m_idkelvkills++;
	}
	
	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->IsInMap("dkcity") && !pi->IsInMap("dkshop1") && !pi->IsInMap("dkshop2")) continue;
		NotifyCitysKills(i);
	}

	if (m_idkareskills >= 200)
	{
		if (g_ev.Is(EventID::DkEvent)) g_ev.Deactivate(EventID::DkEvent);

		for (int i = 0; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = m_pClientList[i];
			if (!pi) continue;
			wsprintf(G_cTxt, "Aresden gano el evento Eternal Balance!");
			ShowClientMsg(i, G_cTxt);
		}
	} 
	else if(m_idkelvkills >= 200)
	{
		if (g_ev.Is(EventID::DkEvent)) g_ev.Deactivate(EventID::DkEvent);

		for (int i = 0; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = m_pClientList[i];
			if (!pi) continue;
			wsprintf(G_cTxt, "Elvine gano el evento Eternal Balance!");
			ShowClientMsg(i, G_cTxt);
		}
	}
}

void cCoreServer::handleActionsWhenEventFinish()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (pi->m_iAdminUserLevel != 0) continue;

		if (pi->IsInMap("dkcity") || pi->IsInMap("dkshop") || pi->IsInMap("dkshop"))
		{
			if (pi->m_bIsKilled)
			{
				pi->m_bIsBeingResurrected = TRUE;
				pi->requestrevive = true;
				dkeventnot_revive(i);
			}
			else
			{
				if (pi->IsLocation("elvine"))
					RequestTeleportHandler(i, "2   ", "gshop_2", -1, -1);
				else
					RequestTeleportHandler(i, "2   ", "gshop_1", -1, -1);
			}
		}

		

		int fighter_index = get_dkfighterindex(pi->m_cCharName);
		if (fighter_index != -1)
		{
			auto dkkills = vec_dkfighters[fighter_index].kills;
			if (dkkills != 0)
			{
				int pointrew = dkkills * 3;

				if (pi->m_sTalent == TALENT_FIRE)
				{
					pi->m_iFirePoints += pointrew;
					SendCommand(i, "/firepoints", m_pClientList[i]->m_iFirePoints);
					wsprintf(G_cTxt, "Has recibido %d FirePoints.", pointrew);
					ShowClientMsg(i, G_cTxt);
				}
				else if (pi->m_sTalent == TALENT_ICE)
				{
					pi->m_iIcePoints += pointrew;
					SendCommand(i, "/icepoints", m_pClientList[i]->m_iIcePoints);
					wsprintf(G_cTxt, "Has recibido %d IcePoints.", pointrew);
					ShowClientMsg(i, G_cTxt);
				}
				else if (pi->m_sTalent == TALENT_LIGHTNING)
				{
					pi->m_iLightningPoints += pointrew;
					SendCommand(i, "/lightningpoints", m_pClientList[i]->m_iLightningPoints);
					wsprintf(G_cTxt, "Has recibido %d LightningPoints.", pointrew);
					ShowClientMsg(i, G_cTxt);
				}

				int ekreward = dkkills * 100;
				pi->m_iEnemyKillCount += ekreward;
				SendCommand(i, "/eks", pi->m_iEnemyKillCount);
				wsprintf(G_cTxt, "Has recibido %d Eks.", ekreward);
				ShowClientMsg(i, G_cTxt);
			}
		}
	}

	vec_dkfighters.clear();
}

void cCoreServer::NotifyCitysKills(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	SendCommand(client, "/dkcitykills", m_idkareskills, m_idkelvkills);
}

void cCoreServer::EndDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	ZeroMemory(cwinner, sizeof(cwinner));
	wsprintf(cwinner, p->m_cCharName);

	if (g_ev.Is(EventID::DkEvent)) g_ev.Deactivate(EventID::DkEvent);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		wsprintf(G_cTxt, "%s es el ganador del DkSet event!", p->m_cCharName);
		ShowClientMsg(i, G_cTxt);
	}
}

void cCoreServer::get_joininfodkevent(int client, char * data)
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

	join_dkevent(client, combattype, angelic);
}

void cCoreServer::SavePlayerItemForEventDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		p->m_pEventItemList[i] = NULL;
		p->m_EventItemPosList[i].x = 40;
		p->m_EventItemPosList[i].y = 30;
		p->m_bIsEventItemEquipped[i] = FALSE;
	}

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL && p->m_pEventItemList[i] == NULL)
		{
			p->m_pEventItemList[i] = new class CItem;
			_bInitItemAttr(p->m_pEventItemList[i], p->m_pItemList[i]->m_sIDnum);
			p->m_pEventItemList[i]->m_dwCount = p->m_pItemList[i]->m_dwCount;
			p->m_pEventItemList[i]->m_sTouchEffectType = p->m_pItemList[i]->m_sTouchEffectType;
			p->m_pEventItemList[i]->m_sTouchEffectValue1 = p->m_pItemList[i]->m_sTouchEffectValue1;
			p->m_pEventItemList[i]->m_sTouchEffectValue2 = p->m_pItemList[i]->m_sTouchEffectValue2;
			p->m_pEventItemList[i]->m_sTouchEffectValue3 = p->m_pItemList[i]->m_sTouchEffectValue3;
			p->m_pEventItemList[i]->m_cItemColor = p->m_pItemList[i]->m_cItemColor;
			p->m_pEventItemList[i]->m_sItemSpecEffectValue1 = p->m_pItemList[i]->m_sItemSpecEffectValue1;
			p->m_pEventItemList[i]->m_sItemSpecEffectValue2 = p->m_pItemList[i]->m_sItemSpecEffectValue2;
			p->m_pEventItemList[i]->m_sItemSpecEffectValue3 = p->m_pItemList[i]->m_sItemSpecEffectValue3;
			p->m_pEventItemList[i]->m_wCurLifeSpan = p->m_pItemList[i]->m_wCurLifeSpan;
			p->m_pEventItemList[i]->m_dwAttribute = p->m_pItemList[i]->m_dwAttribute;
			p->m_EventItemPosList[i].x = p->m_ItemPosList[i].x;
			p->m_EventItemPosList[i].y = p->m_ItemPosList[i].y;
			p->m_pEventItemList[i]->SetBagPosition(p->m_pItemList[i]->m_bagPositionX, p->m_pItemList[i]->m_bagPositionY);
			p->m_bIsEventItemEquipped[i] = p->m_bIsItemEquipped[i];
			p->m_pEventItemList[i]->m_wMaxLifeSpan = p->m_pItemList[i]->m_wMaxLifeSpan;
		}
	}
}

void cCoreServer::RestorePlayerItemsDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		p->m_pItemList[i] = NULL;
		p->m_ItemPosList[i].x = 40;
		p->m_ItemPosList[i].y = 30;
		p->m_bIsItemEquipped[i] = FALSE;
	}

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pEventItemList[i] != NULL && p->m_pItemList[i] == NULL)
		{
			p->m_pItemList[i] = new class CItem;
			_bInitItemAttr(p->m_pItemList[i], p->m_pEventItemList[i]->m_sIDnum);
			p->m_pItemList[i]->m_dwCount = p->m_pEventItemList[i]->m_dwCount;
			p->m_pItemList[i]->m_sTouchEffectType = p->m_pEventItemList[i]->m_sTouchEffectType;
			p->m_pItemList[i]->m_sTouchEffectValue1 = p->m_pEventItemList[i]->m_sTouchEffectValue1;
			p->m_pItemList[i]->m_sTouchEffectValue2 = p->m_pEventItemList[i]->m_sTouchEffectValue2;
			p->m_pItemList[i]->m_sTouchEffectValue3 = p->m_pEventItemList[i]->m_sTouchEffectValue3;
			p->m_pItemList[i]->m_cItemColor = p->m_pEventItemList[i]->m_cItemColor;
			p->m_pItemList[i]->m_sItemSpecEffectValue1 = p->m_pEventItemList[i]->m_sItemSpecEffectValue1;
			p->m_pItemList[i]->m_sItemSpecEffectValue2 = p->m_pEventItemList[i]->m_sItemSpecEffectValue2;
			p->m_pItemList[i]->m_sItemSpecEffectValue3 = p->m_pEventItemList[i]->m_sItemSpecEffectValue3;
			p->m_pItemList[i]->m_wCurLifeSpan = p->m_pEventItemList[i]->m_wCurLifeSpan;
			p->m_pItemList[i]->m_dwAttribute = p->m_pEventItemList[i]->m_dwAttribute;
			p->m_ItemPosList[i].x = p->m_EventItemPosList[i].x;
			p->m_ItemPosList[i].y = p->m_EventItemPosList[i].y;
			p->m_pItemList[i]->SetBagPosition(p->m_pEventItemList[i]->m_bagPositionX, p->m_pEventItemList[i]->m_bagPositionY);
			p->m_bIsItemEquipped[i] = p->m_bIsEventItemEquipped[i];
			p->m_pItemList[i]->m_wMaxLifeSpan = p->m_pEventItemList[i]->m_wMaxLifeSpan;
			if (p->m_bIsItemEquipped[i]) bEquipItemHandler(client, i);
		}
	}

	LoadPlayerItems(client);
}

void cCoreServer::handlePlayerForDkEvent(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int member_index = -1;
	for (int i = 0; i < vec_dkbackmembers.size(); i++)
	{
		if (string(vec_dkbackmembers[i].m_cCharName) == p->m_cCharName)
		{
			member_index = i;
			break;
		}
	}

	if (member_index != -1)
	{
		vec_dkbackmembers[member_index].iStr = p->m_iStr;
		vec_dkbackmembers[member_index].iInt = p->m_iInt;
		vec_dkbackmembers[member_index].iVit = p->m_iVit;
		vec_dkbackmembers[member_index].iDex = p->m_iDex;
		vec_dkbackmembers[member_index].iMag = p->m_iMag;
		vec_dkbackmembers[member_index].iChar = p->m_iCharisma;
	}
	else
	{
		st_dkbackmember u;

		ZeroMemory(u.m_cCharName, sizeof(u.m_cCharName));
		strcpy(u.m_cCharName, p->m_cCharName);
		u.iStr = p->m_iStr;
		u.iInt = p->m_iInt;
		u.iVit = p->m_iVit;
		u.iDex = p->m_iDex;
		u.iMag = p->m_iMag;
		u.iChar = p->m_iCharisma;

		vec_dkbackmembers.push_back(u);
	}

	//war
	if (combattype == 1)
	{
		p->m_iStr = 200;
		p->m_iDex = 200;
		p->m_iInt = 36;
		p->m_iVit = 171;
		p->m_iMag = 50;
		p->m_iCharisma = 10;
	}
	else
	{
		//mage
		p->m_iStr = 40;
		p->m_iDex = 50;
		p->m_iInt = 200;
		p->m_iVit = 167;
		p->m_iMag = 200;
		p->m_iCharisma = 10;
	}

	if (p->angelictype == 1) SendCommand(client, "/str", p->m_iStr - p->removestats);
	else SendCommand(client, "/str", p->m_iStr);

	if (p->angelictype == 2) SendCommand(client, "/dex", p->m_iDex - p->removestats);
	else SendCommand(client, "/dex", p->m_iDex);

	if (p->angelictype == 3) SendCommand(client, "/int", p->m_iInt - p->removestats);
	else SendCommand(client, "/int", p->m_iInt);

	SendCommand(client, "/vit", p->m_iVit);

	if (p->angelictype == 4) SendCommand(client, "/mag", p->m_iMag - p->removestats);
	else SendCommand(client, "/mag", p->m_iMag);

	SendCommand(client, "/chr", p->m_iCharisma);
}

void cCoreServer::restorePlayerStatsAfterDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int member_index = -1;

	for (int i = 0; i < vec_dkbackmembers.size(); i++)
	{
		if (string(vec_dkbackmembers[i].m_cCharName) == p->m_cCharName)
		{
			member_index = i;
			break;
		}
	}

	if (member_index != -1)
	{
		p->m_iStr = vec_dkbackmembers[member_index].iStr;
		p->m_iInt = vec_dkbackmembers[member_index].iInt;
		p->m_iVit = vec_dkbackmembers[member_index].iVit;
		p->m_iDex = vec_dkbackmembers[member_index].iDex;
		p->m_iMag = vec_dkbackmembers[member_index].iMag;
		p->m_iCharisma = vec_dkbackmembers[member_index].iChar;
	}

	if (vec_dkfighters.size() == 0) return;

	int fighter_index = -1;
	fighter_index = get_dkfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int strrem = 0;
		int dexrem = 0;
		int intrem = 0;
		int magrem = 0;

		switch (vec_dkfighters[fighter_index].angelic)
		{
		case 1:
			strrem = 20;
			break;
		case 2:
			dexrem = 20;
			break;
		case 3:
			intrem = 20;
			break;
		case 4:
			magrem = 20;
			break;
		}

		SendCommand(client, "/str", p->m_iStr - strrem);
		SendCommand(client, "/dex", p->m_iDex - dexrem);
		SendCommand(client, "/int", p->m_iInt - intrem);
		SendCommand(client, "/vit", p->m_iVit);
		SendCommand(client, "/mag", p->m_iMag - magrem);
		SendCommand(client, "/chr", p->m_iCharisma);
	}
}

void cCoreServer::CreateDkItem(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
{
	auto p = m_pClientList[client];
	if (!p) return;

	class  CItem * pItem;
	char   cItemName[256];
	int    iEraseReq;

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, (char*)itemname.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (!_bAddClientItemList(client, pItem, &iEraseReq)) return;
	pItem->m_cItemColor = color;
	pItem->isdkweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isdkcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isdkcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isdkcandy = true;
	else pItem->isdkcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isdkitem = true;
	pItem->isdkequip = equip;
	pItem->m_bagPositionX = sx;
	pItem->m_bagPositionY = sy;
	pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;

	if (string(pItem->m_cName) == "AngelicPandent(STR)" ||
		string(pItem->m_cName) == "AngelicPandent(DEX)" ||
		string(pItem->m_cName) == "AngelicPandent(INT)" ||
		string(pItem->m_cName) == "AngelicPandent(MAG)")
	{
		pItem->m_sItemSpecEffectValue3 = 5;
	}

	pItem->m_sTouchEffectValue1 = p->m_sCharIDnum1;
	pItem->m_sTouchEffectValue2 = p->m_sCharIDnum2;
	pItem->m_sTouchEffectValue3 = p->m_sCharIDnum3;
}

void cCoreServer::CreateDkCandy(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
{
	auto p = m_pClientList[client];
	if (!p) return;

	class  CItem * pItem;
	char   cItemName[256];
	int    iEraseReq;

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, (char*)itemname.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (!_bAddClientItemList(client, pItem, &iEraseReq)) return;
	pItem->m_cItemColor = color;
	pItem->isdkweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isdkcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isdkcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isdkcandy = true;
	else pItem->isdkcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isdkitem = true;
	pItem->isdkequip = equip;
	pItem->m_bagPositionX = sx;
	pItem->m_bagPositionY = sy;
	pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;

	if (string(pItem->m_cName) == "AngelicPandent(STR)" ||
		string(pItem->m_cName) == "AngelicPandent(DEX)" ||
		string(pItem->m_cName) == "AngelicPandent(INT)" ||
		string(pItem->m_cName) == "AngelicPandent(MAG)")
	{
		pItem->m_sItemSpecEffectValue3 = 5;
	}

	pItem->m_sTouchEffectValue1 = p->m_sCharIDnum1;
	pItem->m_sTouchEffectValue2 = p->m_sCharIDnum2;
	pItem->m_sTouchEffectValue3 = p->m_sCharIDnum3;

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::CreateDkSets(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::DkEvent)) return;

	DWORD pa = 9405952;
	DWORD ma = 9410048;

	DWORD sp = 9001728;
	DWORD mp = 9005824;

	auto sex = p->m_cSex;
	short color = RollDice(1, 15);
	short color2 = RollDice(1, 15);

	char angelicname[22];
	int fighter_index = get_dkfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		switch (vec_dkfighters[fighter_index].angelic)
		{
		case 1:
			CreateDkItem(client, "AngelicPandent(STR)", 4026531840, 9, 73, 0, 1, false, true);
			break;
		case 2:
			CreateDkItem(client, "AngelicPandent(DEX)", 4026531840, 9, 73, 0, 1, false, true);
			break;
		case 3:
			CreateDkItem(client, "AngelicPandent(INT)", 4026531840, 9, 73, 0, 1, false, true);
			break;
		case 4:
			CreateDkItem(client, "AngelicPandent(MAG)", 4026531840, 9, 73, 0, 1, false, true);
			break;
		default:
			break;
		}
	}

	if (combattype == 1)
	{
		CreateDkItem(client, "Cape", 0, 9, 73, color2, 1, false, true);
		CreateDkItem(client, "Shoes", 0, 9, 73, color2, 1, false, true);
		switch (sex)
		{
		case 1:
			CreateDkItem(client, "DarkKnightPlateMail", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightHauberk", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightLeggings", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightFullHelm", 0, 9, 73, 6, 1, false, true);
			break;

		case 2:
			CreateDkItem(client, "DarkKnightPlateMailW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightHauberkW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightLeggingsW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightFullHelmW", 0, 9, 73, 6, 1, false, true);
			break;
		}
	}
	else
	{
		CreateDkItem(client, "Cape", 0, 9, 73, color2, 1, false, true);
		CreateDkItem(client, "Shoes", 0, 9, 73, color2, 1, false, true);

		switch (sex)
		{
		case 1:
			CreateDkItem(client, "DarkMageChainMail", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightHauberk", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightLeggings", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkMageHat(M)", 0, 9, 73, 6, 1, false, true);
			break;

		case 2:
			CreateDkItem(client, "DarkMageChainMailW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightHauberkW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkKnightLeggingsW", 0, 9, 73, 6, 1, false, true);
			CreateDkItem(client, "DarkMageHat(W)", 0, 9, 73, 6, 1, false, true);
			break;
		}
	}
}

void cCoreServer::NotifyDkItems(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL)
		{
			p->m_bIsItemEquipped[i] = p->m_pItemList[i]->isdkequip;
			if (p->m_bIsItemEquipped[i]) bEquipItemHandler(client, i);
		}
	}

	LoadPlayerItems(client);
}

void cCoreServer::CreateDkWeapons(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::DkEvent)) return;

	short color = RollDice(1, 20);
	
	if (combattype == 1)
	{
		CreateDkItem(client, "WizMagicWand(MS20)", 0, 170, 59, 0, 1, true);
		CreateDkItem(client, "NecklaceOfXelima", 0, 170, 89, 0, 1, false, true);
		CreateDkItem(client, "RingoftheAbaddon", 0, 170, 89, 0, 1, false, true);
		CreateDkItem(client, "BlackKnightTemple", -268435456, 92, 84, 9, 1, true, true);
	}
	else
	{
		CreateDkItem(client, "Rapier+1", 0, 170, 59, 0, 1, true);
		CreateDkItem(client, "NecklaceOfLiche", 0, 170, 89, 0, 1, false, true);
		CreateDkItem(client, "RingofArcmage", 0, 170, 89, 0, 1, false, true);
		CreateDkItem(client, "BlackMageTemple", -268435456, 92, 84, 9, 1, true, true);
	}
}

void cCoreServer::ReloadDkCandys(int client)
{
	if (!m_pClientList[client]) return;

	CClient * p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem * item = p->m_pItemList[i];

		if (!item) continue;

		if (item->m_sIDnum == 780)
		{
			ItemDepleteHandler(client, i, FALSE);
		}

		if (item->m_sIDnum == 781)
		{
			ItemDepleteHandler(client, i, FALSE);
		}

		if (item->m_sIDnum == 782)
		{
			ItemDepleteHandler(client, i, FALSE);
		}

	}

	GetDkRedPotions(client);
	GetDkBluePotions(client);
	GetDkGreenPotions(client);
}

void cCoreServer::GetDkBluePotions(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 5)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkCandy(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::GetDkRedPotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 29)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkCandy(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::GetDkGreenPotions(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 5)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkCandy(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

void cCoreServer::MakeItemsDkEvent(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::DkEvent)) return;
	if (!p->IsInMap("dkcity") && !p->IsInMap("dkshop1") && !p->IsInMap("dkshop2")) return;

	CreateDkSets(client, combattype);
	CreateDkWeapons(client, combattype);
	CreateDkCandys(client);
}

void cCoreServer::handleAngelicActions_DkEvent(int client)
{
	auto pClient = m_pClientList[client];
	if (!pClient) return;

	short sItemIndex = pClient->m_sItemEquipmentStatus[DEF_EQUIPPOS_LFINGER];
	if (sItemIndex == -1 || !pClient->m_pItemList[sItemIndex])
	{
		pClient->angelictype = 0;
		pClient->removestats = 0;
		return;
	}

	auto pItem = pClient->m_pItemList[sItemIndex];
	if (!pItem) return;

	int iTemp2 = pItem->m_sItemSpecEffectValue3;
	int iTemp = (pItem->m_dwAttribute & 0xF0000000) >> 28;

	switch (pItem->m_sIDnum)
	{
	case 911:
		pClient->angelictype = 1;
		pClient->removestats = iTemp + iTemp2;
		break;

	case 912:
		pClient->angelictype = 2;
		pClient->removestats = iTemp + iTemp2;
		break;

	case 913:
		pClient->angelictype = 3;
		pClient->removestats = iTemp + iTemp2;
		break;

	case 914:
		pClient->angelictype = 4;
		pClient->removestats = iTemp + iTemp2;
		break;

	default:
		pClient->angelictype = 0;
		pClient->removestats = 0;
		break;
	}
}
/*

void cCoreServer::LearnMagicsDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
	{
		int index = i;
		if (index == 96 || index == 91 || index == 76 || index == 33 || index == 35 ||
			index == 24 || index == 81 || index == 82 || index == 66 || index == 50 ||
			index == 12 || index == 54 || index == 40 || index == 21 ||
			index == 36 || index == 32 || index == 65 || index == 55 || index == 0 || index == 1 || index == 2)
		{
			m_pClientList[client]->m_cBackMagicMastery[i] = m_pClientList[client]->m_cMagicMastery[i];
			m_pClientList[client]->m_cMagicMastery[i] = 1;

		}
		else
		{
			m_pClientList[client]->m_cBackMagicMastery[i] = m_pClientList[client]->m_cMagicMastery[i];
			m_pClientList[client]->m_cMagicMastery[i] = 0;
		}

		int Staus = m_pClientList[client]->m_cMagicMastery[i];

		char cData[20];
		char* cp = cData;

		Push(cp, (u32)NOT_MAGICS);
		Push(cp, (u16)0);
		Push(cp, i);
		Push(cp, Staus);

		m_pClientList[client]->m_pXSock->iSendMsg(cData, cp - cData);
	}
}*/
void cCoreServer::LearnMagicsDkEvent(int client)
{
	auto* p = m_pClientList[client];
	if (!p) return;

	// Lista de magias que deben quedar activas (mastery = 1)
	static const std::unordered_set<int> kAllowedMagics{
		96, 91, 76, 33, 35,
		24, 81, 82, 66, 50,
		12, 54, 40, 21, 63,
		36, 32, 65, 55, 34, 
		13, 31,	0, 1, 2
	};

	for (int i = 0; i < DEF_MAXMAGICTYPE; ++i)
	{
		// Guardar el estado anterior
		p->m_cBackMagicMastery[i] = p->m_cMagicMastery[i];

		// Asignar 1 solo si el índice está en la lista permitida, si no 0
		p->m_cMagicMastery[i] = kAllowedMagics.count(i) ? 1 : 0;

		int status = p->m_cMagicMastery[i];

		char cData[20];
		char* cp = cData;

		Push(cp, (u32)NOT_MAGICS);
		Push(cp, (u16)0);
		Push(cp, i);
		Push(cp, status);

		p->m_pXSock->iSendMsg(cData, cp - cData);
	}
}

void cCoreServer::RestoreMagicsDkEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
	{
		m_pClientList[client]->m_cMagicMastery[i] = m_pClientList[client]->m_cBackMagicMastery[i];
		int Staus = m_pClientList[client]->m_cMagicMastery[i];
		char cData[1000];
		char* cp = cData;

		Push(cp, (u32)NOT_MAGICS);
		Push(cp, (u16)0);
		Push(cp, i);
		Push(cp, Staus);

		m_pClientList[client]->m_pXSock->iSendMsg(cData, cp - cData);
	}
}

void cCoreServer::CreateDkCandys(int client)
{
	if (!m_pClientList[client]) return;

	CClient * p = m_pClientList[client];
	if (!p) return;

	CreateRedPotionsDk(client);
	CreateBluePotionsDk(client);
	CreateGreenPotionsDk(client);
}

void cCoreServer::CreateBluePotionsDk(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 5)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkItem(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::CreateRedPotionsDk(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 29)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkItem(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::CreateGreenPotionsDk(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 5)
			break;
	}

	for (i = 0; i < rem; i++) CreateDkItem(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

// Definir la función de comparación fuera de send_top5
static bool compareDkKills(const st_dkfighters &a, const st_dkfighters &b) {
	return a.kills > b.kills;
}

void cCoreServer::handleDKEventTop5()
{
	return;
	std::sort(vec_dkfighters.begin(), vec_dkfighters.end(), compareDkKills);

	std::vector<st_dkfighters> top5Players;
	short count = vec_dkfighters.size() < 5 ? vec_dkfighters.size() : 5; 
	for (int i = 0; i < count; i++) {
		top5Players.push_back(vec_dkfighters[i]);
	}

	char top5Message[512];
	ZeroMemory(top5Message, sizeof(top5Message));
	char* cp = top5Message;

	Push(cp, (u32)NOT_TOP5);  
	Push(cp, (u16)0);  

	Push(cp, count);

	for (int i = 0; i < count; i++) {
		short pos = i + 1;
		Push(cp, pos);  
		Push(cp, top5Players[i].fighter, 12); 
		Push(cp, top5Players[i].kills);  
	}

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		auto p = m_pClientList[i];
		if (p && p->IsInMap("dk")) {
			if (m_pClientList[i]->m_pXSock) m_pClientList[i]->m_pXSock->iSendMsg(top5Message, cp - top5Message);
		}
	}
}

void cCoreServer::calcItemsEffectsDk(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL)
		{
			if (p->m_pItemList[i] != NULL)
			{
				if (m_pClientList[client]->m_bIsItemEquipped[i])
				{
					ReleaseItemHandler(client, i, TRUE);
					bEquipItemHandler(client, i);
				}
			}
		}
	}
}

void cCoreServer::DkManaMe(int iClientH)
{
#ifdef DEF_DEBUG
	try {
#endif
		char ErrorText[64];

		if (m_pClientList[iClientH] == NULL) return;
		if (!m_pClientList[iClientH]->IsInMap("dkshop1") && !m_pClientList[iClientH]->IsInMap("dkshop2") && !m_pClientList[iClientH]->IsInMap("dkcity")) return;
		m_pClientList[iClientH]->m_bManaMe = TRUE;
		m_pClientList[iClientH]->m_dwManaMe = timeGetTime();

		SetSlateFlag(iClientH, 3, TRUE);
		ShowClientMsg(iClientH, "Te sientes un experto en magia!");
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: ManaMe");
	}
#endif
}

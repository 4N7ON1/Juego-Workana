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


extern char _tmp_cTmpDirX[9];
extern char _tmp_cTmpDirY[9];


void cCoreServer::SavePlayerItemForEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;
	
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

void cCoreServer::RestorePlayerItems(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

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

void cCoreServer::handlePlayerForEvent(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	int member_index = -1;
	for (int i = 0; i < vec_backmembers.size(); i++)
	{
		if (string(vec_backmembers[i].m_cCharName) == p->m_cCharName)
		{
			member_index = i;
			break;
		}
	}

	if (member_index != -1)
	{
		vec_backmembers[member_index].iStr = p->m_iStr;
		vec_backmembers[member_index].iInt = p->m_iInt;
		vec_backmembers[member_index].iVit = p->m_iVit;
		vec_backmembers[member_index].iDex = p->m_iDex;
		vec_backmembers[member_index].iMag = p->m_iMag;
		vec_backmembers[member_index].iChar = p->m_iCharisma;
	}
	else
	{
		st_backmember u;

		ZeroMemory(u.m_cCharName, sizeof(u.m_cCharName));
		strcpy(u.m_cCharName, p->m_cCharName);
		u.iStr = p->m_iStr;
		u.iInt = p->m_iInt;
		u.iVit = p->m_iVit;
		u.iDex = p->m_iDex;
		u.iMag = p->m_iMag;
		u.iChar = p->m_iCharisma;

		vec_backmembers.push_back(u);
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

void cCoreServer::restorePlayerStatsAfterEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	int member_index = -1;

	for (int i = 0; i < vec_backmembers.size(); i++)
	{
		if (string(vec_backmembers[i].m_cCharName) == p->m_cCharName)
		{
			member_index = i;
			break;
		}
	}

	if (member_index != -1)
	{
		p->m_iStr = vec_backmembers[member_index].iStr;
		p->m_iInt = vec_backmembers[member_index].iInt;
		p->m_iVit = vec_backmembers[member_index].iVit;
		p->m_iDex = vec_backmembers[member_index].iDex;
		p->m_iMag = vec_backmembers[member_index].iMag;
		p->m_iCharisma = vec_backmembers[member_index].iChar;		
	}

	if (vec_ggfighters.size() == 0) return;

	int fighter_index = -1;
	fighter_index = get_ggfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int strrem = 0;
		int dexrem = 0;
		int intrem = 0;
		int magrem = 0;

		switch (vec_ggfighters[fighter_index].angelic)
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


void cCoreServer::restorePlayerBattleStatsAfterEvent(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	int member_index = -1;

	for (int i = 0; i < vec_brbackmembers.size(); i++)
	{
		if (string(vec_brbackmembers[i].m_cCharName) == p->m_cCharName)
		{
			member_index = i;
			break;
		}
	}

	if (member_index != -1)
	{
		p->m_iStr = vec_brbackmembers[member_index].iStr;
		p->m_iInt = vec_brbackmembers[member_index].iInt;
		p->m_iVit = vec_brbackmembers[member_index].iVit;
		p->m_iDex = vec_brbackmembers[member_index].iDex;
		p->m_iMag = vec_brbackmembers[member_index].iMag;
		p->m_iCharisma = vec_brbackmembers[member_index].iChar;
	}

	int fighter_index = -1;
	fighter_index = getbattleroyale_fighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		int strrem = 0;
		int dexrem = 0;
		int intrem = 0;
		int magrem = 0;

		switch (vec_brfighters[fighter_index].angelic)
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

void cCoreServer::CreateGunGameItem(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
{
	auto p = m_pClientList[client];
	if (!p) return;
	
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

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

	if (string(pItem->m_cName) == "BlackMageTemple" ||
		string(pItem->m_cName) == "BlackKnightTemple")
	{
		pItem->m_cItemColor = color;
	}

	pItem->isggweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isggcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isggcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isggcandy = true;
	else pItem->isggcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isgungameitem = true;
	pItem->isggequip = equip;
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

void cCoreServer::CreateGunGameCandy(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

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
	pItem->isggweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isggcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isggcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isggcandy = true;
	else pItem->isggcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isgungameitem = true;
	pItem->isggequip = equip;
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

void cCoreServer::CreateSets(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::GunGame)) return;
	
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	DWORD pa = 33536;
	DWORD mr = 29440; //mr

	DWORD sp = 0;
	DWORD mp = 0;

	auto sex = p->m_cSex;
	short color = 0;
	short color2 = 0;

	char angelicname[22];
	int fighter_index = get_ggfighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		switch (vec_ggfighters[fighter_index].angelic)
		{
		case 1:
			CreateGunGameItem(client, "AngelicPandent(STR)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 2:
			CreateGunGameItem(client, "AngelicPandent(DEX)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 3:
			CreateGunGameItem(client, "AngelicPandent(INT)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 4:
			CreateGunGameItem(client, "AngelicPandent(MAG)", 4026531840, 9, 73, color, 1, false, true);
			break;
		default:
			break;
		}
	}

	if (combattype == 1)
	{
		CreateGunGameItem(client, "Cape", sp, 9, 73, color2, 1, false, true);
		CreateGunGameItem(client, "Shoes", sp, 9, 73, color2, 1, false, true);
		switch (sex)
		{
		case 1:
			CreateGunGameItem(client, "Hauberk(M)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "PlateMail(M)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainHose(M)", pa, 9, 73, color, 1, false);
			CreateGunGameItem(client, "Horned-Helm(M)", pa, 9, 73, color, 1, false);

			CreateGunGameItem(client, "Hauberk(M)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "PlateMail(M)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainHose(M)", mr, 54, 73, color, 1, false, true);
			CreateGunGameItem(client, "Horned-Helm(M)", mr, 54, 73, color, 1, false, true);
			break;

		case 2:
			CreateGunGameItem(client, "Hauberk(W)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "PlateMail(W)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainHose(W)", pa, 9, 73, color, 1, false);
			CreateGunGameItem(client, "Horned-Helm(W)", pa, 9, 73, color, 1, false);

			CreateGunGameItem(client, "Hauberk(W)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "PlateMail(W)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainHose(W)", mr, 54, 73, color, 1, false, true);
			CreateGunGameItem(client, "Horned-Helm(W)", mr, 54, 73, color, 1, false, true);
			break;
		}
	}
	else
	{
		CreateGunGameItem(client, "Cape", mp, 9, 73, color2, 1, false, true);
		CreateGunGameItem(client, "Shoes", mp, 9, 73, color2, 1, false, true);

		switch (sex)
		{
		case 1:
			CreateGunGameItem(client, "Hauberk(M)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainMail(M)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainHose(M)", pa, 9, 73, color, 1, false);
			CreateGunGameItem(client, "Wizard-Cap(M)", pa, 9, 73, color, 1, false);

			CreateGunGameItem(client, "Hauberk(M)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainMail(M)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainHose(M)", mr, 54, 73, color, 1, false, true);
			CreateGunGameItem(client, "Wizard-Cap(M)", mr, 54, 73, color, 1, false, true);
			break;

		case 2:
			CreateGunGameItem(client, "Hauberk(W)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainMail(W)", pa, 9, 73, color, 1, false, true);
			CreateGunGameItem(client, "ChainHose(W)", pa, 9, 73, color, 1, false);
			CreateGunGameItem(client, "Wizard-Cap(W)", pa, 9, 73, color, 1, false);

			CreateGunGameItem(client, "Hauberk(W)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainMail(W)", mr, 54, 73, color, 1, false);
			CreateGunGameItem(client, "ChainHose(W)", mr, 54, 73, color, 1, false, true);
			CreateGunGameItem(client, "Wizard-Cap(W)", mr, 54, 73, color, 1, false, true);
			break;
		}
	}
}

void cCoreServer::NotifyGunGameItems(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL)
		{
			p->m_bIsItemEquipped[i] = p->m_pItemList[i]->isggequip;
			if (p->m_bIsItemEquipped[i]) bEquipItemHandler(client, i);
		}
	}

	LoadPlayerItems(client);
}

void cCoreServer::CreateWeapons(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::GunGame)) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	int level = getGGPlayerLevel(client);
	if (level != -1)
	{
		short color = 0;

		CreateGunGameItem(client, "TowerShield", 0, 118, 22, color, 1, false); //pa15
		CreateGunGameItem(client, "TowerShield", 0, 118, 22, color, 1, false); //ma15

		if (combattype == 1)
		{
			CreateGunGameItem(client, "WizMagicWand(MS20)", 0, 170, 59, color, 1, false);
			CreateGunGameItem(client, "NecklaceOfXelima", 0, 170, 89, color, 1, false, true);
			CreateGunGameItem(client, "RingoftheAbaddon", 0, 170, 89, color, 1, false, true);

			if (string(_war_weap[level - 1].weap) == "BlackKnightTemple")
			{
				CreateGunGameItem(client, _war_weap[level - 1].weap, -268435456, 92, 84, 9, 1, true, true); //fix dk +15
			}
			else
			{
				CreateGunGameItem(client, _war_weap[level - 1].weap, 0, 92, 84, color, 1, true, true);
			}			
		}
		else
		{
			CreateGunGameItem(client, "Rapier+1", 0, 170, 59, color, 1, false);
			CreateGunGameItem(client, "NecklaceOfLiche", 0, 170, 89, color, 1, false, true);
			CreateGunGameItem(client, "RingofArcmage", 0, 170, 89, color, 1, false, true);

			if (string(_mag_weap[level - 1].weap) == "BlackMageTemple")
			{
				CreateGunGameItem(client, _mag_weap[level - 1].weap, -268435456, 92, 84, 9, 1, true, true); //fix dk +15
			}
			else
			{
				CreateGunGameItem(client, _mag_weap[level - 1].weap, 0, 92, 84, color, 1, true, true);
			}

			
		}
	}	
}

void cCoreServer::ReloadCandys(int client)
{
	if (!m_pClientList[client]) return;
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

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

	GetRedPotions(client);
	GetBluePotions(client);
	GetGreenPotions(client);
}

void cCoreServer::GetBluePotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 11)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameCandy(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::GetRedPotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 18)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameCandy(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::GetGreenPotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 4)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameCandy(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

void cCoreServer::MakeItems(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::GunGame)) return;
	if (!p->IsInMap("gg")) return;
	if (m_pClientList[client]->m_iAdminUserLevel != 0) return;

	CreateSets(client, combattype);
	CreateWeapons(client, combattype);	
	CreateCandys(client);
}


void cCoreServer::handleAngelicActions(int client)
{
	auto pClient = m_pClientList[client];
	if (!pClient) return;
	if (pClient->m_iAdminUserLevel != 0) return;

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

void cCoreServer::LearnMagics(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;
	for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
	{
		int index = i;
		if (index == 96 || index == 91 || index == 76 || index == 33 || index == 35 ||
			index == 24 || index == 81 || index == 82 || index == 66 || index == 50 ||
			index == 12 || index == 54 || index == 40 || index == 21)
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
}

void cCoreServer::RestoreMagics(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;
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

void cCoreServer::RemoveFlags(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	SetInvisibilityFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetBerserkFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetIceFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetPoisonFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetIllusionFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetDefenseShieldFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetMagicProtectionFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetProtectionFromArrowFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	SetIllusionMovementFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);

	m_pClientList[client]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = NULL;
}

//CAPTURE THE RELIC// CAPTURE THE RELIC
void cCoreServer::handleCTRActions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	// 🟡 CAPTURAR EN MEDIO (RELIC_MIDDLE en 41,182 → zona válida: 42,184 - 43,185)
	if (p->IsLocation("elvine") || p->IsLocation("aresden"))
	{
		if ((p->m_sX == 44 && p->m_sY == 183) ||
			(p->m_sX == 45 && p->m_sY == 183) ||
			(p->m_sX == 44 && p->m_sY == 184) ||
			(p->m_sX == 45 && p->m_sY == 184))
		{
			if (m_sRelicStatus == RELIC_MIDDLE && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				handleRelicStatus(client, RELIC_HOLD);
				notifyRelicCaptured(client);
			}
		}
	}

	// 🟡 CAPTURAR RELIC DEL PISO (sin cambios)
	if (p->IsLocation("elvine") || p->IsLocation("aresden"))
	{
		if ((p->m_sX == m_sFloorRelicX && p->m_sY == m_sFloorRelicY) ||
			(p->m_sX == m_sFloorRelicX + 1 && p->m_sY == m_sFloorRelicY) ||
			(p->m_sX == m_sFloorRelicX && p->m_sY == m_sFloorRelicY + 1) ||
			(p->m_sX == m_sFloorRelicX + 1 && p->m_sY == m_sFloorRelicY + 1))
		{
			if (m_sRelicStatus == RELIC_FLOOR && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				handleRelicStatus(client, RELIC_HOLD);
				notifyRelicCaptured(client);
			}
		}
	}

	// 🟢 PLANTAR EN ELVINE (portal 141,70 → zona: 144,72 - 145,73)
	if (p->IsLocation("elvine"))
	{
		if ((p->m_sX == 144 && p->m_sY == 71) ||
			(p->m_sX == 145 && p->m_sY == 71) ||
			(p->m_sX == 144 && p->m_sY == 72) ||
			(p->m_sX == 145 && p->m_sY == 72))
		{
			if (m_sRelicStatus == RELIC_HOLD && p->m_bRelicHolder && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				m_sElvineRelics++;

				handleRelicStatus(client, RELIC_ELVINE);
				handleRelicReward(client);
				notifyRelicPlanted(client);

				if (m_sElvineRelics == 20)
				{
					for (int i = 1; i < DEF_MAXCLIENTS; i++)
					{
						auto pi = m_pClientList[i];
						if (!pi || !pi->m_bIsInitComplete || pi->m_bLimbo) continue;

						wsprintf(G_cTxt, "Elvine gano el Capture the relic.");
						ShowClientMsg(i, G_cTxt);
					}
					if (g_ev.Is(EventID::CTR)) g_ev.Deactivate(EventID::CTR);
				}
			}
		}
	}

	// 🔴 ROBAR ELVINE DESDE ARESDEN
	if (p->IsLocation("aresden"))
	{
		if ((p->m_sX == 144 && p->m_sY == 71) ||
			(p->m_sX == 145 && p->m_sY == 71) ||
			(p->m_sX == 144 && p->m_sY == 72) ||
			(p->m_sX == 145 && p->m_sY == 72))
		{
			if (m_sRelicStatus == RELIC_ELVINE && !p->m_bRelicHolder && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				handleRelicStatus(client, RELIC_HOLD);
				notifyRelicCaptured(client);
			}
		}
	}

	// 🔵 PLANTAR EN ARESDEN (portal 201,197 → zona: 204,199 - 205,200)
	if (p->IsLocation("aresden"))
	{
		if ((p->m_sX == 204 && p->m_sY == 198) ||
			(p->m_sX == 205 && p->m_sY == 198) ||
			(p->m_sX == 204 && p->m_sY == 199) ||
			(p->m_sX == 205 && p->m_sY == 199))
		{
			if (m_sRelicStatus == RELIC_HOLD && p->m_bRelicHolder && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				m_sAresdenRelics++;


				handleRelicStatus(client, RELIC_ARESDEN);
				handleRelicReward(client);
				notifyRelicPlanted(client);

				if (m_sAresdenRelics == 20)
				{
					for (int i = 1; i < DEF_MAXCLIENTS; i++)
					{
						auto pi = m_pClientList[i];
						if (!pi || !pi->m_bIsInitComplete || pi->m_bLimbo) continue;

						wsprintf(G_cTxt, "Aresden gano el Capture the relic.");
						ShowClientMsg(i, G_cTxt);
					}
					if (g_ev.Is(EventID::CTR)) g_ev.Deactivate(EventID::CTR);
				}
			}
		}
	}

	// 🟠 ROBAR ARESDEN DESDE ELVINE
	if (p->IsLocation("elvine"))
	{
		if ((p->m_sX == 204 && p->m_sY == 198) ||
			(p->m_sX == 205 && p->m_sY == 198) ||
			(p->m_sX == 204 && p->m_sY == 199) ||
			(p->m_sX == 205 && p->m_sY == 199))
		{
			if (m_sRelicStatus == RELIC_ARESDEN && !p->m_bRelicHolder && p->m_iLevel >= m_iPlayerMaxLevel)
			{
				handleRelicStatus(client, RELIC_HOLD);
				notifyRelicCaptured(client);
			}
		}
	}
}


void cCoreServer::checkInviFlag(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	bool isInvi = m_pClientList[client]->m_iStatus | 0x00000010;
	if (isInvi && p->m_bRelicHolder && g_ev.Is(EventID::CTR))
	{
		SetInvisibilityFlag(client, DEF_OWNERTYPE_PLAYER, FALSE);
	}
}

void cCoreServer::handleRelicReward(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = RollDice(1, 8);

	switch (dado)
	{
		case 1:
		{
			int val = RollDice(800, 1500);
			p->fragment_pa += val;
			SendCommand(client, "/fragment_pa", p->fragment_pa);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d Pa frags.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 2:
		{
			int val = RollDice(800, 1500);
			p->fragment_ma += val;
			SendCommand(client, "/fragment_ma", p->fragment_ma);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d Ma frags.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 3:
		{
			int val = RollDice(800, 1500);
			p->fragment_mr += val;
			SendCommand(client, "/fragment_mr", p->fragment_mr);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d MR frags.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 4:
		{
			int val = RollDice(800, 1500);
			p->fragment_hp += val;
			SendCommand(client, "/fragment_hp", p->fragment_hp);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d HP frags.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}
		
		case 5:
		{
			int val = RollDice(5000, 10000);
			p->m_iContribution += val;
			SendCommand(client, "/contrib", p->m_iContribution);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d contrib.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 6:
		{
			int val = 5;
			p->m_iTokens += val;
			SendCommand(client, "/tokens", p->m_iTokens);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d tokens.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 7:
		{
			int val = 30;
			p->m_iEnemyKillCount += val;
			SendCommand(client, "/eks", p->m_iEnemyKillCount);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d eks.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}

		case 8:
		{
			int val = 5;
			p->m_iEventPoints += val;
			SendCommand(client, "/eventpoints", p->m_iEventPoints);
			wsprintf(G_cTxt, "Plantaste la reliquia y recibes %d EventPoints.", val);
			ShowClientMsg(client, G_cTxt);
			break;
		}
	}

	SendCommand(client, "/rewsound");
}

void cCoreServer::notifyRelicPlanted(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		wsprintf(G_cTxt, "El jugador %s de %s planto la reliquia.", p->m_cCharName, p->m_cLocation);
		ShowClientMsg(i, G_cTxt);
	}
}

void cCoreServer::notifyRelicCaptured(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		m_iCTRFinishCount = 0;
		SendCommand(i, "/ctrfinishcount", m_iCTRFinishCount, 1);

		wsprintf(G_cTxt, "El jugador %s de %s capturo la reliquia.", p->m_cCharName, p->m_cLocation);
		ShowClientMsg(i, G_cTxt);
	}
}

void cCoreServer::handleCTRTimer()
{
	if (!g_ev.Is(EventID::CTR)) return;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;

		checkInviFlag(i);

		if (m_iCTRFinishCount != 0)
		{
			if (m_sRelicStatus == RELIC_ELVINE)
			{
				SendCommand(i, "/ctrfinishcount", m_iCTRFinishCount, 1);
			}
			else if (m_sRelicStatus == RELIC_ARESDEN)
			{
				SendCommand(i, "/ctrfinishcount", m_iCTRFinishCount, 2);
			}
		}
	}

	if (m_sRelicStatus == RELIC_ELVINE)
	{
		m_iCTRFinishCount++;

		if (m_iCTRFinishCount == 300)
		{
			for (int i = 1; i < DEF_MAXCLIENTS; i++)
			{
				auto pi = m_pClientList[i];
				if (!pi) continue;

				if (!pi->m_bIsInitComplete) continue;
				if (pi->m_bLimbo) continue;

				//if (pi->m_bRelicHolder) pi->m_bRelicHolder = false;

				wsprintf(G_cTxt, "Elvine gano el Capture the relic.");
				ShowClientMsg(i, G_cTxt);
			}

			if (g_ev.Is(EventID::CTR))
			{
				g_ev.Deactivate(EventID::CTR);
			}
		}
	}

	if (m_sRelicStatus == RELIC_ARESDEN)
	{
		m_iCTRFinishCount++;

		if (m_iCTRFinishCount == 300)
		{
			for (int i = 1; i < DEF_MAXCLIENTS; i++)
			{
				auto pi = m_pClientList[i];
				if (!pi) continue;

				if (!pi->m_bIsInitComplete) continue;
				if (pi->m_bLimbo) continue;

				//if (pi->m_bRelicHolder) pi->m_bRelicHolder = false;

				wsprintf(G_cTxt, "Aresden gano el Capture the relic.");
				ShowClientMsg(i, G_cTxt);
			}

			if (g_ev.Is(EventID::CTR))
			{
				g_ev.Deactivate(EventID::CTR);
			}
		}
	}
}

void cCoreServer::CreateCandys(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	CreateRedPotions(client);
	CreateBluePotions(client);
	CreateGreenPotions(client);
}

void cCoreServer::CreateBluePotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 11)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameItem(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::CreateRedPotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 18)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameItem(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::CreateGreenPotions(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iAdminUserLevel != 0) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 4)
			break;
	}

	for (i = 0; i < rem; i++) CreateGunGameItem(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

void cCoreServer::handleRelicStatus(int client, short newstatus)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (newstatus == RELIC_HOLD && m_sRelicStatus == RELIC_MIDDLE)
	{
		m_sRelicStatus = RELIC_HOLD;
		m_iCTRFinishCount = 0;
		p->m_bRelicHolder = true;
		SendCommand(client, "/relicholder");
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_MIDDLE && m_sRelicStatus == RELIC_HOLD)
	{
		m_sRelicStatus = RELIC_MIDDLE;
		m_iCTRFinishCount = 0;
		p->m_bRelicHolder = false;
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_ELVINE && m_sRelicStatus == RELIC_HOLD)
	{
		m_sRelicStatus = RELIC_ELVINE;
		m_iCTRFinishCount = 0;
		p->m_bRelicHolder = false;
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_ARESDEN && m_sRelicStatus == RELIC_HOLD)
	{
		m_sRelicStatus = RELIC_ARESDEN;
		p->m_bRelicHolder = false;
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_HOLD && m_sRelicStatus == RELIC_ELVINE)
	{
		m_sRelicStatus = RELIC_HOLD;
		m_iCTRFinishCount = 0;
		p->m_bRelicHolder = true;
		SendCommand(client, "/relicholder");
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_HOLD && m_sRelicStatus == RELIC_ARESDEN)
	{
		m_sRelicStatus = RELIC_HOLD;
		m_iCTRFinishCount = 0;
		p->m_bRelicHolder = true;
		SendCommand(client, "/relicholder");
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_HOLD && m_sRelicStatus == RELIC_FLOOR)
	{
		m_sRelicStatus = RELIC_HOLD;
		p->m_bRelicHolder = true;
		SendCommand(client, "/relicholder");
		NotifyRelicStatus();
		send_objects_data();
	}
	else if (newstatus == RELIC_FLOOR && m_sRelicStatus == RELIC_HOLD)
	{
		m_sRelicStatus = RELIC_FLOOR;

		m_sFloorRelicX = p->m_sX;
		m_sFloorRelicY = p->m_sY;
		p->m_bRelicHolder = false;
		NotifyRelicStatus();
		send_objects_data();
	}
}

void cCoreServer::NotifyGunGameLeader(string leader)
{
	char cData[300];
	char* cp = cData;

	Push(cp, (u32)NOT_GG_LEADER);
	Push(cp, (u16)0);

	char cLeaders[240];
	if (leader.size() > 240) return;

	strcpy(cLeaders, (char*)leader.c_str());

	Push(cp, cLeaders, 240);;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (m_pClientList[i]->m_pXSock) m_pClientList[i]->m_pXSock->iSendMsg(cData, cp - cData);
	}
}


void cCoreServer::SendRelicCords(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!g_ev.Is(EventID::CTR)) return;
	if (!p->m_bRelicHolder) return;
	if (m_sRelicStatus != RELIC_HOLD) return;

	char cData[20];
	char* cp = cData;

	Push(cp, (u32)NOT_RELIC_CORDS);
	Push(cp, (u16)0);

	Push(cp, p->m_sX);
	Push(cp, p->m_sY);

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (m_pClientList[i]->m_pXSock) m_pClientList[i]->m_pXSock->iSendMsg(cData, cp - cData);
	}
}

void cCoreServer::SendFloorRelicCords()
{
	if (!g_ev.Is(EventID::CTR)) return;
	if (m_sRelicStatus != RELIC_FLOOR) return;

	char cData[20];
	char* cp = cData;

	Push(cp, (u32)NOT_RELIC_FLOORCORDS);
	Push(cp, (u16)0);

	Push(cp, m_sFloorRelicX);
	Push(cp, m_sFloorRelicY);

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (m_pClientList[i]->m_pXSock) m_pClientList[i]->m_pXSock->iSendMsg(cData, cp - cData);
	}
}

void cCoreServer::ManageEvents() {
	if (!m_bAutoEvents) return;

	time_t now = time(0);
	tm* local_time = localtime(&now);

	int current_hour = local_time->tm_hour;
	int current_minute = local_time->tm_min;

	static int last_hour = -1; // Almacena la última hora procesada
	static int last_minute = -1; // Almacena el último minuto procesado
	static std::unordered_set<EventID> activated_events; // Almacena eventos activados en la hora actual

	// Si la hora o el minuto han cambiado, limpia el conjunto de eventos activados
	if (current_hour != last_hour || current_minute != last_minute) {
		activated_events.clear();
		last_hour = current_hour;
		last_minute = current_minute;
	}

	int current_day = local_time->tm_wday;

	std::vector<EventSchedule> daily_schedule;

	switch (current_day) {
	case 0: // Domingo
		daily_schedule = {
			{ EventID::GunGame, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::BattleRoyale, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::Crusada, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 1: // Lunes
		daily_schedule = {
			{ EventID::Deathmatch, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::Crusada, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 2: // Martes
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::DkEvent, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::GunGame, 23, 0, 59 }
		};
		break;

	case 3: // Miércoles
		daily_schedule = {
			{ EventID::GunGame, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::Crusada, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 4: // Jueves
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::GunGame, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::DkEvent, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::GunGame, 23, 0, 59 }
		};
		break;

	case 5: // Viernes
		daily_schedule = {
			{ EventID::Deathmatch, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 6: // Sábado
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::ExpBooster, 12, 0, 120 },
			{ EventID::DropBooster, 12, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::BattleRoyale, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::Crusada, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;
	}

	for (const auto& schedule : daily_schedule) {
		int end_hour = schedule.start_hour + (schedule.start_minute + schedule.duration_minutes) / 60;
		int end_minute = (schedule.start_minute + schedule.duration_minutes) % 60;

		// Verifica si es hora de activar el evento
		if (current_hour == schedule.start_hour && current_minute == schedule.start_minute) {
			// Si el evento no está activado, actívalo y agrégalo al conjunto
			if (!g_ev.Is(schedule.event) && activated_events.find(schedule.event) == activated_events.end()) {
				g_ev.Activate(schedule.event);
				activated_events.insert(schedule.event); // Marca el evento como activado
			}
		}

		// Verifica si es hora de desactivar el evento
		if (current_hour == end_hour && current_minute == end_minute) {
			if (g_ev.Is(schedule.event)) {
				g_ev.Deactivate(schedule.event);
			}
		}
	}
}
/*

void CMapServer::ManageEvents() {

	if (!m_bAutoEvents) return;

	time_t now = time(0);
	tm* local_time = localtime(&now);

	int current_hour = local_time->tm_hour;
	int current_minute = local_time->tm_min;

	int current_day = local_time->tm_wday;

	std::vector<EventSchedule> daily_schedule;

	switch (current_day) {
	case 0: // Domingo
		daily_schedule = {
			{ EventID::GunGame, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::BattleRoyale, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::Crusada, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 1: // Lunes
		daily_schedule = {
			{ EventID::Deathmatch, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::Crusada, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 2: // Martes
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::DkEvent, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::GunGame, 23, 0, 59 }
		};
		break;

	case 3: // Miércoles
		daily_schedule = {
			{ EventID::GunGame, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::Crusada, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 4: // Jueves
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::Deathmatch, 14, 0, 60 },
			{ EventID::GunGame, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::DkEvent, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::GunGame, 23, 0, 59 }
		};
		break;

	case 5: // Viernes
		daily_schedule = {
			{ EventID::Deathmatch, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::CTR, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::BattleRoyale, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;

	case 6: // Sábado
		daily_schedule = {
			{ EventID::DkEvent, 1, 0, 60 },
			{ EventID::Questx2, 3, 0, 120 },
			{ EventID::Questx2, 10, 0, 120 },
			{ EventID::GunGame, 14, 0, 60 },
			{ EventID::DkEvent, 15, 0, 60 },
			{ EventID::DropBooster, 16, 0, 60 },
			{ EventID::DropInhibition, 17, 0, 120 },
			{ EventID::Ekx50, 17, 0, 120 },
			{ EventID::Questx2, 17, 0, 120 },
			{ EventID::GunGame, 19, 0, 60 },
			{ EventID::BattleRoyale, 20, 0, 60 },
			{ EventID::Questx2, 21, 0, 60 },
			{ EventID::DropBooster, 21, 0, 60 },
			{ EventID::ExpBooster, 21, 0, 60 },
			{ EventID::Crusada, 22, 0, 60 },
			{ EventID::DkEvent, 23, 0, 59 }
		};
		break;
	}

	for (const auto& schedule : daily_schedule) {
		int end_hour = schedule.start_hour + (schedule.start_minute + schedule.duration_minutes) / 60;
		int end_minute = (schedule.start_minute + schedule.duration_minutes) % 60;

		if (current_hour == schedule.start_hour && current_minute == schedule.start_minute) {
			if (!g_ev.Is(schedule.event)) {
				g_ev.Activate(schedule.event);
			}
		}

		if (current_hour == end_hour && current_minute == end_minute) {
			if (g_ev.Is(schedule.event)) {
				g_ev.Deactivate(schedule.event);
			}
		}
	}
}
*/

void cCoreServer::fixItemsDk(int client, int itemindex)
{
	ReleaseItemHandler(client, itemindex, TRUE);
	bEquipItemHandler(client, itemindex);
}


int cCoreServer::GetClientIndexByName(char * charname)
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

void cCoreServer::reqStarterItems(int client, char * data)
{
	short type;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, type);
	GiveStarterItems(client, type);
}

void cCoreServer::GiveStarterItems(int client, short type)
{
	auto p = m_pClientList[client];
	if (!p) return;

	wsprintf(G_cTxt, "¡Lo sentimos! StarterPack esta desactivado!");
	ShowClientMsg(client, G_cTxt);
	return;

	if (HasPlayerReceivedStarterItems(p->m_cCharName))
	{
		wsprintf(G_cTxt, "¡Lo sentimos! Ya has recibido los items iniciales!");
		ShowClientMsg(client, G_cTxt);
		return;
	}

	if (type != 1 && type != 2) return;

	DWORD pa = 34816;
	DWORD ma = 38912;

	std::vector<StarterItem> starterItems;

	if (type == 1)
	{
		if (p->m_cSex == 1)
		{
			starterItems = {
				{ "KlonessBlade", 2693802240, 1 },
				{ "MysticBlade", 2687510784, 1 },
				{ "MysticHammer", 2687510784, 1 },
				{ "PowerBarbarian", 2687510784, 1 },

				//PA 24
				{ "MysticHeroHauberk(M)", pa, 1 },
				{ "MysticHeroLeggs(M)", pa, 1 },
				{ "MysticHeroArmor(M)", pa, 1 },
				{ "MysticHeroHelm(M)", pa, 1 },

				//MA 24
				{ "MysticHeroHauberk(M)", ma, 1 },
				{ "MysticHeroLeggs(M)", ma, 1 },
				{ "MysticHeroArmor(M)", ma, 1 },
				{ "MysticHeroHelm(M)", ma, 1 },

				{ "NeckPowerXelima", 0, 1 },
				{ "RingofEternalWar", 0, 1 },

				{ "MysticHeroCape", ma, 1 },
				{ "Shoes", pa, 1 }
			};
		}
		else if (p->m_cSex == 2)
		{
			starterItems = {
				{ "KlonessBlade", 9447936, 1 },
				{ "MysticBlade", 2687510784, 1 },
				{ "MysticHammer", 2687510784, 1 },
				{ "PowerBarbarian", 2687510784, 1 },


				//PA 18
				{ "MysticHeroHauberk(W)", pa, 1 },
				{ "MysticHeroLeggs(W)", pa, 1 },
				{ "MysticHeroArmor(W)", pa, 1 },
				{ "MysticHeroHelm(W)", pa, 1 },

				//MA 18
				{ "MysticHeroHauberk(W)", ma, 1 },
				{ "MysticHeroLeggs(W)", ma, 1 },
				{ "MysticHeroArmor(W)", ma, 1 },
				{ "MysticHeroHelm(W)", ma, 1 },

				{ "NeckPowerXelima", 0, 1 },
				{ "RingofEternalWar", 0, 1 },

				//MR 42
				{ "MysticHeroCape", ma, 1 },
				{ "Shoes", pa, 1 }
			};
		}
	}
	else if (type == 2)
	{
		if (p->m_cSex == 1)
		{	
			starterItems = {
				{ "KlonessWand(MS.30)", 10752, 1 },
				{ "MysticWand(MS.30)", 2687510784, 1 },
				{ "MysticRapier", 2687510784, 1 },
				{ "PowerRapier", 2687510784, 1 },

				//PA 18
				{ "MysticHeroHauberk(M)", pa, 1 },
				{ "MysticHeroLeggs(M)", pa, 1 },
				{ "MysticHeroRobe(M)", pa, 1 },
				{ "MysticHeroCap(M)", pa, 1 },

				//MA 18
				{ "MysticHeroHauberk(M)", ma, 1 },
				{ "MysticHeroLeggs(M)", ma, 1 },
				{ "MysticHeroRobe(M)", ma, 1 },
				{ "MysticHeroCap(M)", ma, 1 },

				{ "NeckPower(MS.30)", 0, 1 },
				{ "RingofEternalMage", 0, 1 },

				//MR 42
				{ "MysticHeroCape", pa, 1 },
				{ "Shoes", ma, 1 }
			};
		}
		else if (p->m_cSex == 2)
		{
			starterItems = {
				{ "KlonessWand(MS.30)", 10752, 1 },
				{ "MysticWand(MS.30)", 2687510784, 1 },
				{ "MysticRapier", 2687510784, 1 },
				{ "PowerRapier", 2687510784, 1 },

				//PA 18
				{ "MysticHeroHauberk(W)", pa, 1 },
				{ "MysticHeroLeggs(W)", pa, 1 },
				{ "MysticHeroRobe(W)", pa, 1 },
				{ "MysticHeroCap(W)", pa, 1 },

				//MA 18
				{ "MysticHeroHauberk(W)", ma, 1 },
				{ "MysticHeroLeggs(W)", ma, 1 },
				{ "MysticHeroRobe(W)", ma, 1 },
				{ "MysticHeroCap(W)", ma, 1 },

				{ "NeckPower(MS.30)", 0, 1 },
				{ "RingofEternalMage", 0, 1 },

				//MR 42
				{ "MysticHeroCape", pa, 1 },
				{ "Shoes", ma, 1 }
			};
		}
	}

	if (!starterItems.empty())
	{
		int starterSize = starterItems.size();

		if (!HasEnoughInventorySpace(client, starterSize))
		{
			wsprintf(G_cTxt, "¡Necesitas %d espacios libres para recibir los items iniciales!", starterSize);
			ShowClientMsg(client, G_cTxt);
			return;
		}

		AddPlayerToStarterItemsList(p->m_cCharName);

		for (const auto& item : starterItems)
		{
			CreateStarterItem(client, item.itemName, item.itemAttr, item.itemCount);
		}

		wsprintf(G_cTxt, "¡Has recibido tus ítems iniciales!");
		ShowClientMsg(client, G_cTxt);
	}
}

void cCoreServer::CreateStarterItem(int client, string itemname, DWORD atr, int count)
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

	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
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


bool cCoreServer::HasEnoughInventorySpace(int client, int needcount)
{
	auto p = m_pClientList[client];
	if (!p) return false;

	int freeSlots = 0;
	for (int i = 0; i < DEF_MAXITEMS; ++i)
	{
		if (p->m_pItemList[i] == NULL)
		{
			freeSlots++;
		}
	}

	return (freeSlots >= needcount); 
}

void cCoreServer::handleRecallTeleport(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->IsInMap("aresden") && p->IsLocation("elvine")) return;
	if (p->IsInMap("elvine") && p->IsLocation("aresden")) return;

	if (p->m_iLevel < 200 && p->m_iRebirthLevel == 0)
	{		if (p->IsLocation("aresden"))
			RequestTeleportHandler(client, "2   ", "ABarracks", -1, -1);
		else RequestTeleportHandler(client, "2   ", "EBarracks", -1, -1);	
		return;
	}

	if (p->IsInMap("purgecity") && p->m_sPurgeTeam == TEAM_ONE)
	{
		char m_cDestName[22];
		strcpy(m_cDestName, "purgecity");

		if (p->lastX == -1 && p->lastY == -1)
		{
			int dado = RollDice(1, 2);
			switch (dado)
			{
			case 1:
			{
				p->lastX = 223;
				p->lastY = 236;
				RequestTeleportHandler(client, "2   ", m_cDestName, 223, 236);
				break;
			}
			case 2:
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (p->lastX == 223 && p->lastY == 236)
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
			}
			else if (p->lastX == 155 && p->lastY == 154)
			{
				p->lastX = 223;
				p->lastY = 236;
				RequestTeleportHandler(client, "2   ", m_cDestName, 223, 236);
			}
		}
	}
	else if (p->IsInMap("purgecity") && p->m_sPurgeTeam == TEAM_TWO)
	{
		char m_cDestName[22];
		strcpy(m_cDestName, "purgecity");

		if (p->lastX == -1 && p->lastY == -1)
		{
			int dado = RollDice(1, 2);
			switch (dado)
			{
			case 1:
			{
				p->lastX = 91;
				p->lastY = 57;
				RequestTeleportHandler(client, "2   ", m_cDestName, 91, 57);
				break;
			}
			case 2:
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (p->lastX == 155 && p->lastY == 154)
			{
				p->lastX = 91;
				p->lastY = 57;
				RequestTeleportHandler(client, "2   ", m_cDestName, 91, 57);
			}
			else if (p->lastX == 91 && p->lastY == 57)
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
			}
		}
	}

	else if (p->IsInMap("crazyshop2") && p->m_sPurgeTeam == TEAM_ONE)
	{
		p->lastX = 223;
		p->lastY = 236;
		RequestTeleportHandler(client, "2   ", "purgecity", 223, 236);
	}

	else if (p->IsInMap("crazyshop1") && p->m_sPurgeTeam == TEAM_TWO)
	{
		p->lastX = 91;
		p->lastY = 57;
		RequestTeleportHandler(client, "2   ", "purgecity", 91, 57);
	}
	/*if (p->IsInMap("purgecity") && p->m_sPurgeTeam == TEAM_ONE)
	{
		RequestTeleportHandler(client, "2   ", "crazyshop2", -1, -1);
	}
	else if (p->IsInMap("purgecity") && p->m_sPurgeTeam == TEAM_TWO)
	{
		RequestTeleportHandler(client, "2   ", "crazyshop1", -1, -1);
	}
	else if (p->IsInMap("purgecity") && p->m_sPurgeTeam == TEAM_NONE)
	{
		if (p->IsLocation("aresden")) RequestTeleportHandler(client, "2   ", "gshop_1", -1, -1); else RequestTeleportHandler(client, "2   ", "gshop_2", -1, -1);
	}
	else if (p->IsInMap("crazyshop2"))
	{
		if (p->IsLocation("aresden")) RequestTeleportHandler(client, "2   ", "gshop_1", -1, -1); else RequestTeleportHandler(client, "2   ", "gshop_2", -1, -1);
	}
	else if (p->IsInMap("crazyshop1"))
	{
		if (p->IsLocation("aresden")) RequestTeleportHandler(client, "2   ", "gshop_1", -1, -1); else RequestTeleportHandler(client, "2   ", "gshop_2", -1, -1);
	}*/
	else if (p->IsLocation("aresden") && !p->IsInMap("dkcity") && !p->IsInMap("dkshop1") && !p->IsInMap("dkshop2"))
	{
		RequestTeleportHandler(client, "2   ", "aresden", -1, -1);
	}
	else if (p->IsLocation("elvine") && !p->IsInMap("dkcity") && !p->IsInMap("dkshop1") && !p->IsInMap("dkshop2"))
	{
		RequestTeleportHandler(client, "2   ", "elvine", -1, -1);
	}
	else if (p->IsLocation("NONE") && !p->IsInMap("dkcity") && !p->IsInMap("dkshop1") && !p->IsInMap("dkshop2"))
	{
		RequestTeleportHandler(client, "2   ", "default", -1, -1);
	}

	else if (p->IsLocation("aresden"))
	{		
		char m_cDestName[22];
		if (p->IsInMap("dkcity") || p->IsInMap("dkshop1") || p->IsInMap("dkshop2")) strcpy(m_cDestName, "dkcity"); else strcpy(m_cDestName, "city");
		
		if (p->lastX == -1 && p->lastY == -1)
		{
			int dado = RollDice(1, 2);
			switch (dado)
			{
			case 1:
			{
				p->lastX = 223;
				p->lastY = 236;
				RequestTeleportHandler(client, "2   ", m_cDestName, 223, 236);
				break;
			}
			case 2:
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (p->lastX == 223 && p->lastY == 236)
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
			}
			else if (p->lastX == 155 && p->lastY == 154)
			{
				p->lastX = 223;
				p->lastY = 236;
				RequestTeleportHandler(client, "2   ", m_cDestName, 223, 236);
			}
		}
	}
	else if (p->IsLocation("elvine"))
	{
		char m_cDestName[22];
		if (p->IsInMap("dkcity") || p->IsInMap("dkshop1") || p->IsInMap("dkshop2")) strcpy(m_cDestName, "dkcity"); else strcpy(m_cDestName, "city");

		if (p->lastX == -1 && p->lastY == -1)
		{
			int dado = RollDice(1, 2);
			switch (dado)
			{
			case 1:
			{
				p->lastX = 91;
				p->lastY = 57;
				RequestTeleportHandler(client, "2   ", m_cDestName, 91, 57);
				break;
			}
			case 2:
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			if (p->lastX == 155 && p->lastY == 154)
			{
				p->lastX = 91;
				p->lastY = 57;
				RequestTeleportHandler(client, "2   ", m_cDestName, 91, 57);
			}
			else if (p->lastX == 91 && p->lastY == 57)
			{
				p->lastX = 155;
				p->lastY = 154;
				RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
			}
		}		
	}
	else
	{
		char m_cDestName[22];
		if (p->IsInMap("dkcity") || p->IsInMap("dkshop1") || p->IsInMap("dkshop2")) strcpy(m_cDestName, "dkcity"); else strcpy(m_cDestName, "city");

		if (p->lastX == -1 && p->lastY == -1 || p->lastX == 223 && p->lastY == 236)
		{
			p->lastX = 155;
			p->lastY = 154;
			RequestTeleportHandler(client, "2   ", m_cDestName, 155, 154);
		}
		else if (p->lastX == 155 && p->lastY == 154)
		{
			p->lastX = 91;
			p->lastY = 57;
			RequestTeleportHandler(client, "2   ", m_cDestName, 91, 57);
		}
		else if (p->lastX == 91 && p->lastY == 57)
		{
			p->lastX = 223;
			p->lastY = 236;
			RequestTeleportHandler(client, "2   ", m_cDestName, 223, 236);
		}
	}
}

void cCoreServer::loadPhysItemDamageConfig(const std::string& filename) {
	std::ifstream configFile(filename);
	std::string line;

	physItems.clear(); 

	while (std::getline(configFile, line)) {
		std::istringstream iss(line);
		std::string itemName;
		int itemDamage;

		if (iss >> itemName >> itemDamage) {
			physItems.push_back({ itemName, itemDamage });
		}
	}
}

void cCoreServer::updateDamages()
{
	updatePhysConfig("Configs\\physical_damage.cfg");
	updateMagicConfig("Configs\\magic_damage.cfg");
}

void cCoreServer::displayPhysItems() const {
	for (const auto& item : physItems) {
		std::cout << "Physical Item: " << item.name << ", Damage: " << item.damage << std::endl;
	}
}

void cCoreServer::updatePhysConfig(const std::string& filename) {
	loadPhysItemDamageConfig(filename);
	std::cout << "Physical damage config Updated." << std::endl;
}

int cCoreServer::getphysDamage(char * itemName) {
	for (const auto& item : physItems) {
		if (strcmp(item.name.c_str(), itemName) == 0) { 
			return item.damage;
		}
	}
	return 0; 
}

void cCoreServer::loadMagicItemDamageConfig(const std::string& filename) {
	std::ifstream configFile(filename);
	std::string line;

	magicItems.clear(); 

	while (std::getline(configFile, line)) {
		std::istringstream iss(line);
		std::string itemName;
		float itemDamage; 

		if (iss >> itemName >> itemDamage) {
			magicItems.push_back({ itemName, itemDamage });
		}
	}
}

void cCoreServer::updateMagicConfig(const std::string& filename) {
	loadMagicItemDamageConfig(filename);
	std::cout << "Magic damage config Updated." << std::endl;
}

void cCoreServer::displayMagicItems() const {
	for (const auto& item : magicItems) {
		std::cout << "Magic Item: " << item.name << ", Damage: " << item.damage << std::endl;
	}
}

float cCoreServer::getMagicDamage(char* itemName) { 
	for (const auto& item : magicItems) {
		if (strcmp(item.name.c_str(), itemName) == 0) { 
			return item.damage;
		}
	}
	return 0.0f; 
}

void cCoreServer::getConfigDamages() {
	loadPhysItemDamageConfig("Configs\\physical_damage.cfg");
	loadMagicItemDamageConfig("Configs\\magic_damage.cfg");
}

void cCoreServer::getDamages(float & add_magicdamage, float & rem_magicdamage, int & add_physdamage, int & rem_physdamage) 
{
	add_magicdamage = 0.0f;
	rem_magicdamage = 0.0f;
	add_physdamage = 0;
	rem_physdamage = 0;

	ifstream config_file("Configs\\Damages.cfg");
	string line;

	if (config_file.is_open()) {
		while (getline(config_file, line)) {
			// Eliminar comentarios (líneas que empiezan con # o ;)
			size_t comment_pos = line.find_first_of("#;");
			if (comment_pos != string::npos) {
				line = line.substr(0, comment_pos);
			}

			// Ignorar líneas vacías
			if (line.empty()) continue;

			size_t equals_pos = line.find('=');
			if (equals_pos != string::npos) {
				string key = line.substr(0, equals_pos);
				string value = line.substr(equals_pos + 1);

				// Eliminar espacios en blanco
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				// Asignar valores según la clave
				if (key == "add_magicdamage") {
					add_magicdamage = strtof(value.c_str(), nullptr);
				}
				else if (key == "rem_magicdamage") {
					rem_magicdamage = strtof(value.c_str(), nullptr);
				}
				else if (key == "add_physdamage") {
					add_physdamage = strtol(value.c_str(), nullptr, 10);
				}
				else if (key == "rem_physdamage") {
					rem_physdamage = strtol(value.c_str(), nullptr, 10);
				}
			}
		}
		config_file.close();

		// Mostrar los valores leídos
		cout << "Config updated:" << endl;
		cout << "Magic Damage added: " << add_magicdamage << endl;
		cout << "Magic Damage subtracted: " << rem_magicdamage << endl;
		cout << "Physical Damage added: " << add_physdamage << endl;
		cout << "Physical Damage subtracted: " << rem_physdamage << endl;
	}
	else {
		cerr << "Error: No se pudo abrir el archivo config.cfg" << endl;
		return;
	}

	return;
}

void cCoreServer::requestActiveRole(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rol = -1;
	Pop(data, rol);
	
	handleActivateRole(client, rol);
}

void cCoreServer::requestUpgradeRole(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rol = -1;
	Pop(data, rol);

	handleUpgradeRole(client, rol);
}


//HeatoN market
// response market names
void cCoreServer::RequestMarketNames(int client)
{
	if (g_ev.Is(EventID::GunGame)) return;
	if (g_ev.Is(EventID::BattleRoyale)) return;

	if (!m_pClientList[client])
		return;

	if (!m_pClientList[client]->IsShopMap() && !m_pClientList[client]->IsInMap("cityhall_1") && !m_pClientList[client]->IsInMap("cityhall_2")) return;

	vector<string> names;

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (!m_pClientList[i])
			continue;

		int cnt = 0;
		for (int x = 0; x < DEF_MAXMARKETITEMS; x++)
		{
			if (m_pClientList[i]->m_pMarketItem[x])
			{
				cnt++;
				break;
			}
		}

		if (cnt > 0)
		{
			names.push_back(m_pClientList[i]->m_cCharName);
		}
	}

	char cData[1024];
	char* cp = cData;
	Push(cp, (u32)REQUEST_MARKETNAMES);
	Push(cp, (u16)0);

	Push(cp, (u16)names.size());

	for (auto s : names)
	{
		char name[12] = {};
		strcpy(name, s.c_str());
		Push(cp, name, 10);
	}

	m_pClientList[client]->m_pXSock->iSendMsg(cData, cp - cData);
}

// response market items
void cCoreServer::RequestMarketItems(int client, char* pData)
{
	if (g_ev.Is(EventID::GunGame)) return;
	if (g_ev.Is(EventID::BattleRoyale)) return;
	if (!m_pClientList[client])
		return;

	if (!m_pClientList[client]->IsShopMap() && !m_pClientList[client]->IsInMap("cityhall_1") && !m_pClientList[client]->IsInMap("cityhall_2")) return;

	pData += 6;
	char name[12] = {};
	Pop(pData, name, 10);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (!m_pClientList[i])
			continue;

		if (std::string(m_pClientList[i]->m_cCharName) == name)
		{
			char data[10000] = {};
			char* cp = data;
			Push(cp, (u32)REQUEST_MARKETITEMS);
			Push(cp, (u16)0);
			Push(cp, m_pClientList[i]->m_cCharName, 10);

			struct MarketItem
			{
				CItem* item;
				int index;
			};
			vector<MarketItem> item;

			for (int x = 0; x < DEF_MAXMARKETITEMS; x++)
			{
				if (!m_pClientList[i]->m_pMarketItem[x])
					continue;

				item.push_back({ m_pClientList[i]->m_pMarketItem[x], x });
			}

			Push(cp, (u16)item.size());
			for (auto& ite : item)
			{
				Push(cp, ite.index);
				auto it = ite.item;
				Push(cp, it->m_cName, 20);
				Push(cp, it->m_dwCount);
				Push(cp, it->m_cItemType);
				Push(cp, it->m_cEquipPos);
				Push(cp, it->m_sLevelLimit);
				Push(cp, it->m_cGenderLimit);
				Push(cp, it->m_wCurLifeSpan);
				Push(cp, it->m_wWeight);
				Push(cp, it->m_sSprite);
				Push(cp, it->m_sSpriteFrame);
				Push(cp, it->m_cItemColor);
				Push(cp, it->m_sItemSpecEffectValue2);
				Push(cp, it->m_dwAttribute);
				Push(cp, it->m_sItemSpecEffectValue3);
				Push(cp, it->m_iTokenVal);
			}
			if (m_pClientList[client]->m_pXSock) m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
			return;
		}
	}
}

//Market Logs - ChatGPT con ayuda de gabi (?
void cCoreServer::handleMarketLog(char* seller, char* buyer, char* itemname, int coinval, DWORD attr)
{
	// Crear el directorio y archivo de log si no existen GABI
	char logDir[] = "..\\ServerLogs\\Market";
	_mkdir(logDir);

	// Obtener la fecha actual
	time_t now = time(0);
	tm* ltm = localtime(&now);
	char logFilename[64];
	sprintf(logFilename, "%s\\Transfers_%02d_%02d_%04d.txt", logDir, ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);

	// Abrir o crear el archivo de log GABI
	std::ofstream logFile;
	logFile.open(logFilename, std::ios::app);

	if (logFile.is_open())
	{
		// Registrar toda la transferencia en una sola línea con la hora GABI
		logFile << "Hora: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec
			<< ", Cliente: " << buyer
			<< ", Vendedor: " << seller
			<< ", Item: " << itemname
			<< ", Atributo: " << attr
			<< ", Monedas: " << coinval
			<< "\n";
		logFile.close();
	}
}
/*

// get and response market items
void cCoreServer::RequestGetMarketItem(int client, char* pData)
{
if (g_ev.Is(EventID::GunGame)) return;
if (g_ev.Is(EventID::BattleRoyale)) return;
int saveseller = -1;
int savebuyer = -1;

if (!m_pClientList[client])
return;

//	if (m_pClientList[client]->m_iAdminUserLevel != 0)
if (m_pClientList[client]->m_iAdminUserLevel > 0 && m_pClientList[client]->m_iAdminUserLevel < 6)
{
return;
}

/ *if (m_pClientList[client]->m_iAdminUserLevel > 0 && m_pClientList[client]->m_iAdminUserLevel < 4)
{
PutLogList("4");
return;
}* /

if (!m_pClientList[client]->IsShopMap() && !m_pClientList[client]->IsInMap("cityhall_1") && !m_pClientList[client]->IsInMap("cityhall_2")) return;


pData += 6;
char name[12] = {};
Pop(pData, name, 10);

int index;
Pop(pData, index);

if (index < 0 || index >= DEF_MAXMARKETITEMS)
return;

DWORD coins;
Pop(pData, coins);

char itemname[24] = {};
Pop(pData, itemname, 20);

DWORD attr;
Pop(pData, attr);

for (int i = 0; i < DEF_MAXCLIENTS; i++)
{
if (!m_pClientList[i])
continue;

if (string(m_pClientList[i]->m_cCharName) != name)
continue;

if (!m_pClientList[i]->m_pMarketItem[index])
{
ShowClientMsg(client, "This item has expired or has been removed");
return;
}

if (coins != m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal ||
m_pClientList[i]->m_pMarketItem[index]->m_dwAttribute != attr ||
string(m_pClientList[i]->m_pMarketItem[index]->m_cName) != itemname)
{
ShowClientMsg(client, "This item has expired or has been removed");
return;
}

if (_iGetItemSpaceLeft(client) == 0) {

SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
return;
}

if (m_pClientList[i] != m_pClientList[client])
{
if (m_pClientList[client]->m_iTokens < m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal)
{
ShowClientMsg(client, "Not enough tokens");
return;
}

handleMarketLog(m_pClientList[i]->m_cCharName, m_pClientList[client]->m_cCharName, m_pClientList[i]->m_pMarketItem[index]->m_cName, m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal, m_pClientList[i]->m_pMarketItem[index]->m_dwAttribute);

m_pClientList[client]->m_iTokens -= m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal;
m_pClientList[i]->m_iTokens += m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal;

SendCommand(client, "/tokens", m_pClientList[client]->m_iTokens);
SendCommand(i, "/tokens", m_pClientList[i]->m_iTokens);

/ *SendNotifyMsg(client, NULL, CLIENT_NOTIFY_COINS, m_pClientList[client]->m_iCoins, NULL, NULL, NULL);
SendNotifyMsg(i, NULL, CLIENT_NOTIFY_COINS, m_pClientList[i]->m_iCoins, NULL, NULL, NULL);* /

SendF5DataCharacter(client);
SendF5DataCharacter(i);

char cTxt[112] = {};
wsprintf(cTxt, "You have traded %s from market! +%d coins!", m_pClientList[i]->m_pMarketItem[index]->m_cName, m_pClientList[i]->m_pMarketItem[index]->m_iTokenVal);
ShowClientMsg(i, cTxt);

saveseller = i;
savebuyer = client;
}

for (int x = 0; x < DEF_MAXITEMS; x++) {
if (!m_pClientList[client]->m_pItemList[x]) {
m_pClientList[client]->m_pItemList[x] = m_pClientList[i]->m_pMarketItem[index];
m_pClientList[i]->m_pMarketItem[index] = nullptr;

m_pClientList[client]->m_ItemPosList[x].x = 40;
m_pClientList[client]->m_ItemPosList[x].y = 30;

m_pClientList[client]->m_bIsItemEquipped[x] = false;

SendItemNotifyMsg(m_pClientList[client]->client, CLIENT_NOTIFY_ITEMOBTAINED, m_pClientList[client]->m_pItemList[x], NULL);

iCalcTotalWeight(m_pClientList[client]->client);
if (m_pClientList[i] != m_pClientList[client])
{
ShowClientMsg(client, "Item successfully traded from market!");

for (int i = 0; i < DEF_MAXITEMS; i++)
{
ReqRepairItemCofirmHandler2(client, i, NULL);
}

}
else
{
ShowClientMsg(client, "Withdrew item from market");

for (int i = 0; i < DEF_MAXITEMS; i++)
{
ReqRepairItemCofirmHandler2(client, i, NULL);
}
}

break;
}
}
}

if (saveseller != -1) LocalSavePlayerData(saveseller);
LocalSavePlayerData(client);
//if (savebuyer != -1) LocalSavePlayerData(savebuyer);
}*/

void cCoreServer::RequestGetMarketItem(int client, char* pData)
{
	if (g_ev.Is(EventID::GunGame)) return;
	if (g_ev.Is(EventID::BattleRoyale)) return;

	int saveseller = -1;
	int savebuyer = -1;

	if (!m_pClientList[client]) return;

	if (m_pClientList[client]->m_iAdminUserLevel > 0 && m_pClientList[client]->m_iAdminUserLevel < 6) return;

	if (!m_pClientList[client]->IsShopMap() &&
		!m_pClientList[client]->IsInMap("cityhall_1") &&
		!m_pClientList[client]->IsInMap("cityhall_2")) return;

	pData += 6;

	char name[12] = {};
	Pop(pData, name, 10);

	int index;
	Pop(pData, index);
	if (index < 0 || index >= DEF_MAXMARKETITEMS) return;

	DWORD coins;
	Pop(pData, coins);

	char itemname[24] = {};
	Pop(pData, itemname, 20);

	DWORD attr;
	Pop(pData, attr);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (!m_pClientList[i]) continue;
		if (string(m_pClientList[i]->m_cCharName) != name) continue;

		auto* pMarketItem = m_pClientList[i]->m_pMarketItem[index];
		if (!pMarketItem) {
			ShowClientMsg(client, "This item has expired or has been removed");
			return;
		}

		if (coins != pMarketItem->m_iTokenVal ||
			pMarketItem->m_dwAttribute != attr ||
			string(pMarketItem->m_cName) != itemname)
		{
			ShowClientMsg(client, "This item has expired or has been removed");
			return;
		}

		if (_iGetItemSpaceLeft(client) == 0) {
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		if (m_pClientList[i] != m_pClientList[client])
		{
			if (m_pClientList[client]->m_iTokens < pMarketItem->m_iTokenVal)
			{
				ShowClientMsg(client, "Not enough tokens");
				return;
			}

			handleMarketLog(m_pClientList[i]->m_cCharName, m_pClientList[client]->m_cCharName,
				pMarketItem->m_cName, pMarketItem->m_iTokenVal, pMarketItem->m_dwAttribute);

			m_pClientList[client]->m_iTokens -= pMarketItem->m_iTokenVal;
			m_pClientList[i]->m_iTokens += pMarketItem->m_iTokenVal;

			SendCommand(client, "/tokens", m_pClientList[client]->m_iTokens);
			SendCommand(i, "/tokens", m_pClientList[i]->m_iTokens);

			SendF5DataCharacter(client);
			SendF5DataCharacter(i);

			char cTxt[112] = {};
			wsprintf(cTxt, "You have traded %s from market! +%d tokens!", pMarketItem->m_cName, pMarketItem->m_iTokenVal);
			ShowClientMsg(i, cTxt);

			saveseller = i;
			savebuyer = client;
		}

		// Si es countable y ya lo tiene, aumentar cantidad en vez de insertar nuevo
		if (countableItems(itemname))
		{
			for (int k = 0; k < DEF_MAXITEMS; k++)
			{
				auto* pInvItem = m_pClientList[client]->m_pItemList[k];
				if (pInvItem && string(pInvItem->m_cName) == itemname)
				{
					pInvItem->m_dwCount += pMarketItem->m_dwCount;
					delete pMarketItem;
					m_pClientList[i]->m_pMarketItem[index] = nullptr;

					iCalcTotalWeight(client);
					ShowClientMsg(client, "Item stacked from market!");

					SendCommand(client, "/newitemcount", k, pInvItem->m_dwCount);
					//IncreaseItemCount(client, haveItemIndex, pInvItem->m_dwCount);

					if (saveseller != -1) LocalSavePlayerData(saveseller);
					LocalSavePlayerData(client);
					return;
				}
			}
		}

		// Buscar espacio para insertar el ítem si no es acumulable o no lo tiene
		for (int x = 0; x < DEF_MAXITEMS; x++) {
			if (!m_pClientList[client]->m_pItemList[x]) {
				m_pClientList[client]->m_pItemList[x] = pMarketItem;
				m_pClientList[i]->m_pMarketItem[index] = nullptr;

				m_pClientList[client]->m_ItemPosList[x].x = 40;
				m_pClientList[client]->m_ItemPosList[x].y = 30;
				m_pClientList[client]->m_bIsItemEquipped[x] = false;

				SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, m_pClientList[client]->m_pItemList[x], NULL);
				iCalcTotalWeight(client);

				if (m_pClientList[i] != m_pClientList[client]) {
					ShowClientMsg(client, "Item successfully traded from market!");
				}
				else {
					ShowClientMsg(client, "Withdrew item from market");
				}

				for (int r = 0; r < DEF_MAXITEMS; r++) {
					ReqRepairItemCofirmHandler2(client, r, NULL);
				}

				break;
			}
		}
	}

	if (saveseller != -1) LocalSavePlayerData(saveseller);
	LocalSavePlayerData(client);
}



void cCoreServer::ReqRepairItemCofirmHandler2(int client, char cItemID, char* pString)
{
	char* cp, cItemCategory, cData[120];
	double   d1, d2, d3;
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[client] == NULL) return;
	if (m_pClientList[client]->m_bIsInitComplete == FALSE) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	CItem* item = m_pClientList[client]->m_pItemList[cItemID];

	if (!item) return;

	cItemCategory = item->m_cCategory;

	if (((cItemCategory >= 1) && (cItemCategory <= 10)) || ((cItemCategory >= 43) && (cItemCategory <= 50)) ||
		((cItemCategory >= 11) && (cItemCategory <= 13))) {

		item->m_wCurLifeSpan = item->m_wMaxLifeSpan;
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_ITEMREPAIRED2, cItemID, item->m_wCurLifeSpan, NULL, NULL); //WTF
		iCalcTotalWeight(client);
	}
}

// set market item
void cCoreServer::RequestSetMarketItem(int client, char* pData)
{
	if (g_ev.Is(EventID::BattleRoyale)) return;
	if (g_ev.Is(EventID::GunGame)) return;
	auto p = m_pClientList[client];

	if (!p) return;

	pData += 6;
	int index;
	DWORD coins;
	Pop(pData, index);
	Pop(pData, coins);

	if (p->m_pItemList[index] == NULL) return;

	if (coins <= 0 || coins > DEF_MAXMARKETCOINS)
	{
		return;
	}

	if (index < 0 || index > DEF_MAXITEMS)
	{
		return;
	}

	if (p->m_pItemList[index]->m_cItemType == DEF_ITEMTYPE_CONSUME ||
		p->m_pItemList[index]->m_cItemType == DEF_ITEMTYPE_ARROW)
	{
		if (!countableItems(p->m_pItemList[index]->m_cName)) return;
	}


	/*
	if (p->m_pItemList[index]->m_cItemType == DEF_ITEMTYPE_CONSUME ||
	p->m_pItemList[index]->m_cItemType == DEF_ITEMTYPE_ARROW)
	{
	return;
	}*/

	if (!p->IsShopMap() && !p->IsInMap("cityhall_1") && !p->IsInMap("cityhall_2"))
	{
		EnableItem(client, index);
		return;
	}

	//if (p->m_iAdminUserLevel != 0)
	if (p->m_iAdminUserLevel > 0 && p->m_iAdminUserLevel < 6)
	{
		EnableItem(client, index);
		return;
	}

	if ((p->m_pItemList[index]->m_sTouchEffectValue1 == p->m_sCharIDnum1) &&
		(p->m_pItemList[index]->m_sTouchEffectValue2 == p->m_sCharIDnum2) &&
		(p->m_pItemList[index]->m_sTouchEffectValue3 == p->m_sCharIDnum3))
	{
		char cInfoString[112] = {};
		wsprintf(cInfoString, "Prohibited transaction item with ID");
		SendAlertMsg(client, cInfoString);
		EnableItem(client, index);
		return;
	}

	for (int i = 0; i < DEF_MAXMARKETITEMS; i++)
	{
		if (p->m_pMarketItem[i])
			continue;

		p->m_pMarketItem[i] = p->m_pItemList[index];
		p->m_pMarketItem[i]->m_iTokenVal = coins;
		p->m_pItemList[index] = nullptr;
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_GIVEITEMFIN_ERASEITEM, index, 1, NULL, "Howard");
		ShowClientMsg(client, "Item registered in the market!");
		LocalSavePlayerData(client);
		return;
	}

	ShowClientMsg(client, "You don't have any more space in the market!");
	EnableItem(client, index);
}

void cCoreServer::SaveCharacterMarket(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char AscIIDir[10000], charfilename[10000];
	string savefile = "Database\\Market\\";
	wsprintf(charfilename, "\\%s.txt", m_pClientList[client]->m_cCharName);
	wsprintf(AscIIDir, "AscII%d", (unsigned char)m_pClientList[client]->m_cCharName[0], m_pClientList[client]->m_cCharName);
	savefile.append(AscIIDir);
	savefile.append(charfilename);

	string createdir = "Database\\Market\\";
	createdir.append(AscIIDir);

	_mkdir("Database\\Market");
	_mkdir((char*)createdir.c_str());

	/*wsprintf(G_cTxt, "Guardando market de: %s", m_pClientList[client]->m_cCharName);
	PutLogList(G_cTxt);*/

	string item_query;

	if (FILE* f1 = fopen((char*)savefile.c_str(), "wt"))
	{
		for (int i = 0; i < DEF_MAXMARKETITEMS; i++)
		{
			auto item = p->m_pMarketItem[i];
			if (!item) continue;

			char ItemQuery[10000];
			ZeroMemory(ItemQuery, sizeof(ItemQuery));
			sprintf(ItemQuery, "market-item = %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", item->m_cName, (char*)to_string(item->m_dwCount).c_str(),
				(char*)to_string(item->m_sTouchEffectType).c_str(), (char*)to_string(item->m_sTouchEffectValue1).c_str(),
				(char*)to_string(item->m_sTouchEffectValue2).c_str(), (char*)to_string(item->m_sTouchEffectValue3).c_str(),
				(char*)to_string(item->m_cItemColor).c_str(), (char*)to_string(item->m_sItemSpecEffectValue1).c_str(),
				(char*)to_string(item->m_sItemSpecEffectValue2).c_str(), (char*)to_string(item->m_sItemSpecEffectValue3).c_str(),
				(char*)to_string(item->m_wCurLifeSpan).c_str(), (char*)to_string(item->m_dwAttribute).c_str(), (char*)to_string(item->m_wPrice).c_str(),
				(char*)to_string(item->m_iTokenVal).c_str());

			item_query.append(ItemQuery);
		}

		fwrite((char*)item_query.c_str(), item_query.size(), 1, f1);
		fclose(f1);

	}
}

void cCoreServer::bDecodePlayerMarketItemContents(int client, char* pData, DWORD dwSize)
{
	char* pContents, *token, *pOriginContents, cTmpName[11], cTxt[120];
	char   seps[] = "= \t\n";
	char   cReadModeA, cReadModeB;
	int    i, iItemIndex, iItemMarketIndex, iTotalGold, iNotUsedItemPrice;
	class  CStrTok* pStrTok;
	short  sTmpType, sTmpAppr1;

	BOOL   bRet;
	int    iTemp;
	SYSTEMTIME SysTime;
	__int64 iDateSum1, iDateSum2;
	BOOL   bIsNotUsedItemFound = FALSE;

	auto p = m_pClientList[client];

	if (!p) return;

	if (client < 0 || client > DEF_MAXCLIENTS)
	{
		return;
	}

	iTotalGold = 0;
	iItemMarketIndex = 0;
	iNotUsedItemPrice = 0;
	int marketindex = 0;
	cReadModeA = 0;
	cReadModeB = 0;
	char cData[1024 * 100];
	DWORD lpNumberOfBytesRead;
	HANDLE hFile = CreateFile(pData, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == -1) {
		wsprintf(G_cTxt, "(X) CRITICAL ERROR! Cannot open configuration file(%s)!", pData);
		PutLogList(pData);
		return;
	}

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);

	ReadFile(hFile, cData, dwFileSize, &lpNumberOfBytesRead, NULL);
	CloseHandle(hFile);

	pContents = new char[dwFileSize + 1];
	memcpy_secure(pContents, cData, dwFileSize);

	pOriginContents = pContents;

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();

	while (token != NULL) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {

			case 1:
				switch (cReadModeB) {
				case 1:
					if (iItemMarketIndex >= DEF_MAXMARKETITEMS) {
						delete pContents;
						delete pStrTok;
						return;
					}

					if (_bInitItemAttr(m_pClientList[client]->m_pMarketItem[iItemMarketIndex], token) == FALSE) {
						wsprintf(cTxt, "(!!!) Client(%s)-Bank Item(%s) is not existing Item! Conection closed.", m_pClientList[client]->m_cCharName, token);
						PutLogList(cTxt);
						HANDLE hFile;
						DWORD  nWrite;
						hFile = CreateFile("Error.Log", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
						WriteFile(hFile, (char*)pOriginContents, dwSize + 2, &nWrite, NULL);
						CloseHandle(hFile);

						delete pContents;
						delete pStrTok;
						return;
					}
					cReadModeB = 2;
					break;

				case 2:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					iTemp = atoi(token);
					if (iTemp < 0) iTemp = 1;
					if (iGetItemWeight(m_pClientList[client]->m_pMarketItem[iItemMarketIndex], iTemp) > _iCalcMaxLoad(client)) {
						iTemp = 1;
						wsprintf(G_cTxt, "(!) Ä³¸¯ÅÍ(%s) ¾ÆÀÌÅÛ(%s) °³¼ö ¿À¹öÇÃ·Î¿ì", m_pClientList[client]->m_cCharName, m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_cName);
						//PutLogFileList(G_cTxt);
						PutLogList(G_cTxt);
					}

					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_dwCount = (DWORD)iTemp;
					cReadModeB = 3;
					break;

				case 3:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sTouchEffectType = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sTouchEffectValue1 = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sTouchEffectValue2 = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sTouchEffectValue3 = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_cItemColor = atoi(token);
					cReadModeB = 8;
					break;



				case 8:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sItemSpecEffectValue1 = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sItemSpecEffectValue2 = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_sItemSpecEffectValue3 = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_wCurLifeSpan = atoi(token);

					cReadModeB = 12;
					break;

				case 12:
				{
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}

					unsigned __int64 m_dwAttribute64;
					_stscanf((LPCTSTR)token, _T("%I64u"), &m_dwAttribute64);
					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_dwAttribute = m_dwAttribute64;

					cReadModeB = 13;
					break;
				}

				case 13:
				{
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}

					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_wPrice = atoi(token);
					cReadModeB = 14;
					break;
				}


				case 14:
				{
					if (_bGetIsStringIsNumber(token) == FALSE) {
						wsprintf(cTxt, "(!!!) Player(%s) data file error! wrong Data format - Connection closed. ", m_pClientList[client]->m_cCharName);
						PutLogList(cTxt);
						delete pContents;
						delete pStrTok;
						return;
					}

					m_pClientList[client]->m_pMarketItem[iItemMarketIndex]->m_iTokenVal = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					iItemMarketIndex++;
					break;
				}
				}
				break;
			}
		}
		else {
			if (memcmp(token, "market-item", 11) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
				m_pClientList[client]->m_pMarketItem[iItemMarketIndex] = new class CItem;
			}
		}

		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[]pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		wsprintf(cTxt, "(!!!) Player(%s) data file contents error(%d %d)! Connection closed.", m_pClientList[client]->m_cCharName, cReadModeA, cReadModeB);
		PutLogList(cTxt);
		HANDLE hFile2;
		DWORD  nWrite2;
		hFile2 = CreateFile("Error.Log", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		WriteFile(hFile2, (char*)pOriginContents, dwSize + 2, &nWrite2, NULL);
		CloseHandle(hFile2);

		return;
	}
	return;
}

void cCoreServer::ReadMarket(int client)
{
	char cFileName3[500] = {};
	char cDir3[500] = {};
	strcat(cFileName3, "Database\\");
	strcat(cFileName3, "\\");
	strcat(cFileName3, "\\");
	strcat(cFileName3, "Market");
	strcat(cFileName3, "\\");
	strcat(cFileName3, "\\");
	wsprintf(cDir3, "AscII%d", m_pClientList[client]->m_cCharName[0]);
	strcat(cFileName3, cDir3);
	strcat(cFileName3, "\\");
	strcat(cFileName3, "\\");
	strcat(cFileName3, m_pClientList[client]->m_cCharName);
	strcat(cFileName3, ".txt");

	bDecodePlayerMarketItemContents(client, cFileName3, 0);
}


BOOL cCoreServer::revertitemWeight(class CItem* pItem, char* pItemName)
{
#ifdef DEF_DEBUG
	try {
#endif
		register int i;
		char cTmpName[22];

		ZeroMemory(cTmpName, sizeof(cTmpName));
		memcpy_secure(cTmpName, pItemName, 20);

		for (i = 0; i < DEF_MAXITEMTYPES; i++) {
			if (m_pItemConfigList[i] != NULL) {
				if (memcmp(cTmpName, m_pItemConfigList[i]->m_cName, 20) == 0) {
					pItem->m_wWeight = m_pItemConfigList[i]->m_wWeight;
					pItem->m_wCurLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
					pItem->m_wMaxLifeSpan = pItem->m_wCurLifeSpan;
					return TRUE;
				}
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: _bInitItemAttr");
	}
#endif
	return FALSE;
}

bool cCoreServer::getitemname(int client, int cx, int cy)
{
	auto p = m_pClientList[client];
	if (!p) return false;
	auto m = m_pMapList[m_pClientList[client]->m_cMapIndex];
	if (!m)	return false;
	if ((cx < 0 || cx >= m->m_sSizeX || cy < 0 || cy >= m->m_sSizeY)) return false;

	auto pTile = (class CTile *)(m->m_pTile + cx + cy*m->m_sSizeY);
	if (!pTile)	return false;

	auto it = pTile->m_pItem[0];
	if (!it) return false;
	if (string(it->m_cName) == "Gold" || string(it->m_cName) == "CoinBall" ||
		string(it->m_cName) == "RepBall" || string(it->m_cName) == "ContribBall" ||
		string(it->m_cName) == "MajBall" ||

		string(it->m_cName) == "LightShards" ||
		string(it->m_cName) == "StrongShards" ||
		string(it->m_cName) == "HitProbFragments" ||
		string(it->m_cName) == "DRFragments" ||
		string(it->m_cName) == "HPRecFragments " ||
		string(it->m_cName) == "SPRecFragments " ||
		string(it->m_cName) == "MPRecFragments " ||
		string(it->m_cName) == "MRFragments" ||
		string(it->m_cName) == "PaFragments" ||
		string(it->m_cName) == "MaFragments" ||
		string(it->m_cName) == "ExpFragments" ||
		string(it->m_cName) == "Tokens" ||
		string(it->m_cName) == "MajBall") return true;

	return false;
}
/*

void CMapServer::getCriticals(int client)

{

int maxcrits = 5000;

int cost_per_crit = 5;



auto p = m_pClientList[client];

if (!p) return;



int current_crits = p->m_iSuperAttackLeft;

if (current_crits >= maxcrits) return;



int player_contrib = p->m_iContribution;

if (player_contrib <= 0) return;



int crits_needed = maxcrits - current_crits;

int crits_to_buy = (crits_needed < player_contrib) ? crits_needed : player_contrib;



if (crits_to_buy > 0)

{



p->m_iContribution -= ;

p->m_iSuperAttackLeft += crits_to_buy;

SendCriticals(client);

}

}

*/

void cCoreServer::getCriticals(int client)
{
	int maxcrits = 5000;
	int cost_per_crit = 3;
		
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iSuperAttackLeft >= maxcrits) return;
	
	if (p->IsInMap("dkcity") || p->IsInMap("dkshop1") || p->IsInMap("dkshop2"))
	{
		p->m_iSuperAttackLeft = 5000;
		SendCriticals(client);
	}
	else
	{
		int current_crits = p->m_iSuperAttackLeft;
		if (current_crits >= maxcrits) return;

		int player_gold = dwGetItemCount(client, "Gold");
		if (player_gold < cost_per_crit)
		{
			ShowClientMsg(client, "Not enough Gold. Cost: 1 Critical x 3 Gold.");
			return;
		}

		int crits_needed = maxcrits - current_crits;
		int max_crits_affordable = player_gold / cost_per_crit; // Críticos que el jugador puede permitirse
		int crits_to_buy = (crits_needed < max_crits_affordable) ? crits_needed : max_crits_affordable;

		if (crits_to_buy > 0)
		{
			int total_cost = crits_to_buy * cost_per_crit;
			SetItemCount(client, "Gold", player_gold - total_cost);
			p->m_iSuperAttackLeft += crits_to_buy;
			SendCriticals(client);

			// Mensaje al cliente indicando la cantidad comprada y el oro gastado
			char msg[256];
			sprintf(msg, "You bought %d criticals for %d Gold.", crits_to_buy, total_cost);
			ShowClientMsg(client, msg);
		}
		else
		{
			ShowClientMsg(client, "Not enough Gold. Cost: 1 Critical x 3 Gold.");
		}
	}	
}


void cCoreServer::SendMessageToClientUDP(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	string response = "Message";
	sendto(p->serverSocket, response.c_str(), response.size(), 0, (sockaddr*)&p->clientAddr, p->clientAddrLen);
}

void cCoreServer::sendMessageToClient(int client, int msgid) {
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->m_bUdpSocket) return;

	char data[500];
	char* cp = data;

	bool bsend = false;

	Push(cp, msgid);

	switch (msgid)
	{
	case SOCKMSG_WELCOME:
	{
		char test3[5];
		wsprintf(test3, "hola");
		Push(cp, test3, 5);
		bsend = true;
		break;
	}
	case SOCKMSG_PING:
	{
		bsend = true;
		break;
	}
	}

	if (!bsend) return;

	// Calcular el tamaño total del mensaje
	int totalSize = cp - data;

	// Enviar el mensaje
	int bytesSent = sendto(p->serverSocket, data, totalSize, 0, (sockaddr*)&p->clientAddr, p->clientAddrLen);
	if (bytesSent == SOCKET_ERROR) {
		//std::cerr << "Error sending custom message to client." << std::endl;
	}
	else {
		//std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
	}
}

void cCoreServer::SendUDPCommand(int client, const char * command, int val1, int val2)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_bLimbo) return;
	char cCommand[30];
	ZeroMemory(cCommand, sizeof(cCommand));
	strcpy(cCommand, command);

	char data[256];
	auto * cp = data;

	Push(cp, (int)SOCKMSG_COMMAND);

	Push(cp, cCommand, 30);
	Push(cp, val1);
	Push(cp, val2);

	// Calcular el tamaño total del mensaje
	int totalSize = cp - data;

	// Enviar el mensaje
	int bytesSent = sendto(p->serverSocket, data, totalSize, 0, (sockaddr*)&p->clientAddr, p->clientAddrLen);
	if (bytesSent == SOCKET_ERROR) {
		//std::cerr << "Error sending custom message to client." << std::endl;
	}
	else {
		//std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
	}
}

/*

enum ObjectAction
{
ACTION_OBJECTSTOP,
ACTION_OBJECTMOVE,
ACTION_OBJECTRUN,
ACTION_OBJECTATTACK,
ACTION_OBJECTMAGIC,
ACTION_OBJECTGETITEM,
ACTION_OBJECTDAMAGE,
ACTION_OBJECTDAMAGEMOVE,
ACTION_OBJECTATTACKMOVE,
ACTION_OBJECTDYING,
ACTION_OBJECTNULLACTION,
ACTION_OBJECTDEAD
};*/

/*

void CMapServer::updateEntitySync(int owner, short owner_type, short command)
{
char data[512];
auto * cp = data;

Push(cp, (int)SOCKMSG_OBJECTCOMMAND);
Push(cp, (short)owner_type);
Push(cp, (short)command);
bool bsend = false;
switch (owner_type)
{
case DEF_OWNERTYPE_PLAYER:
{
auto owner_player = m_pClientList[owner];
if (!owner_player) return;

switch (command)
{
case ACTION_OBJECTSTOP: { break; }
case ACTION_OBJECTMOVE: { break; }
case ACTION_OBJECTRUN: { break; }
case ACTION_OBJECTATTACK: { break; }
case ACTION_OBJECTMAGIC: { break; }
case ACTION_OBJECTGETITEM: { break; }
case ACTION_OBJECTDAMAGE: { break; }
case ACTION_OBJECTDAMAGEMOVE: { break; }
case ACTION_OBJECTATTACKMOVE: { break; }
case ACTION_OBJECTDYING: { break; }
case ACTION_OBJECTNULLACTION: { break; }
case ACTION_OBJECTDEAD: { break; }
default: return;  break;
}

break;
}
case DEF_OWNERTYPE_NPC:
{
auto owner_mob = m_pNpcList[owner];
if (!owner_mob) return;

switch (command)
{
case ACTION_OBJECTSTOP: { break; }

case ACTION_OBJECTMOVE:
{
Push(cp, (int)owner + 1000);
Push(cp, (short)owner_mob->m_sX);
Push(cp, (short)owner_mob->m_sY);
Push(cp, (short)owner_mob->m_sType);
Push(cp, owner_mob->m_cDir);
Push(cp, owner_mob->m_cName, 5);
Push(cp, (short)owner_mob->m_sAppr2);
Push(cp, (int)owner_mob->m_iStatus);
char killed;
if (owner_mob->m_bIsKilled == TRUE) killed = 1;
else killed = 0;
Push(cp, killed);

bsend = true;

break;
}

case ACTION_OBJECTRUN: { break; }
case ACTION_OBJECTATTACK: { break; }
case ACTION_OBJECTMAGIC: { break; }
case ACTION_OBJECTGETITEM: { break; }
case ACTION_OBJECTDAMAGE: { break; }
case ACTION_OBJECTDAMAGEMOVE: { break; }
case ACTION_OBJECTATTACKMOVE: { break; }
case ACTION_OBJECTDYING: { break; }
case ACTION_OBJECTNULLACTION: { break; }
case ACTION_OBJECTDEAD: { break; }
default: return;  break;
}
break;
}
default:
break;
}

if (!bsend) return;

for (int i = 1; i < DEF_MAXCLIENTS; i++)
{
auto player = m_pClientList[i];
if (!player) continue;
if (!player->m_bIsInitComplete) continue;
if (player->m_bLimbo) continue;
if (!player->m_bUdpSocket) continue;

if (owner_type == DEF_OWNERTYPE_NPC)
{
if (string(m_pNpcList[owner]->m_cMapName) != player->m_cMapName) continue;
//m_pNpcList[owner]->m_sX;
//m_pNpcList[owner]->m_sY;
}

if (owner_type == DEF_OWNERTYPE_PLAYER)
{
if (string(m_pClientList[owner]->m_cMapName) != player->m_cMapName) continue;

}

int totalSize = cp - data;

// Enviar el mensaje
int bytesSent = sendto(player->serverSocket, data, totalSize, 0, (sockaddr*)&player->clientAddr, player->clientAddrLen);
if (bytesSent == SOCKET_ERROR) {
//std::cerr << "Error sending custom message to client." << std::endl;
}
else {
//std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
}
}


}*/

void cCoreServer::updateEntitySync(int owner, short owner_type, short command)
{
	char data[512];
	auto* cp = data;

	Push(cp, (int)SOCKMSG_OBJECTCOMMAND);
	Push(cp, (short)owner_type);
	Push(cp, (short)command);
	bool bsend = false;

	switch (owner_type)
	{
	case DEF_OWNERTYPE_PLAYER:
	{
		auto owner_player = m_pClientList[owner];
		if (!owner_player) return;

		// Filtra comandos para jugadores si es necesario (opcional)
		switch (command)
		{
		case ACTION_OBJECTSTOP: break;
		case ACTION_OBJECTMOVE: break;
		case ACTION_OBJECTRUN: break;
		case ACTION_OBJECTATTACK: break;
		case ACTION_OBJECTMAGIC: break;
		case ACTION_OBJECTGETITEM: break;
		case ACTION_OBJECTDAMAGE: break;
		case ACTION_OBJECTDAMAGEMOVE: break;
		case ACTION_OBJECTATTACKMOVE: break;
		case ACTION_OBJECTDYING: break;
		case ACTION_OBJECTNULLACTION: break;
		case ACTION_OBJECTDEAD: break;
		default: return;
		}

		break;
	}
	case DEF_OWNERTYPE_NPC:
	{
		auto owner_mob = m_pNpcList[owner];
		if (!owner_mob) return;

		switch (command)
		{
		case ACTION_OBJECTSTOP: break;
		case ACTION_OBJECTMOVE:
		{
			Push(cp, (int)owner + 1000);
			Push(cp, (short)owner_mob->m_sX);
			Push(cp, (short)owner_mob->m_sY);
			Push(cp, (short)owner_mob->m_sType);
			Push(cp, owner_mob->m_cDir);
			Push(cp, owner_mob->m_cName, 5);
			Push(cp, (short)owner_mob->m_sAppr2);
			Push(cp, (int)owner_mob->m_iStatus);

			char killed = owner_mob->m_bIsKilled ? 1 : 0;
			Push(cp, killed);

			bsend = true;

			break;
		}
		default: return;
		}
		break;
	}
	default:
		break;
	}

	if (!bsend) return;

	// Rango visual en tiles (800x600 píxeles equivale a 25x18 tiles)
	const int rangeX = 25;
	const int rangeY = 18;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto player = m_pClientList[i];
		if (!player) continue;
		if (!player->m_bIsInitComplete) continue;
		if (player->m_bLimbo) continue;
		if (!player->m_bUdpSocket) continue;

		// Filtrar por rango visual para NPC
		if (owner_type == DEF_OWNERTYPE_NPC)
		{
			if (string(m_pNpcList[owner]->m_cMapName) != player->m_cMapName) continue;

			int deltaX = abs(player->m_sX - m_pNpcList[owner]->m_sX);
			int deltaY = abs(player->m_sY - m_pNpcList[owner]->m_sY);

			if (deltaX > rangeX || deltaY > rangeY) continue;
		}

		// Filtrar por rango visual para jugadores si es necesario
		if (owner_type == DEF_OWNERTYPE_PLAYER)
		{
			if (string(m_pClientList[owner]->m_cMapName) != player->m_cMapName) continue;
		}

		int totalSize = cp - data;

		// Enviar el mensaje
		int bytesSent = sendto(player->serverSocket, data, totalSize, 0, (sockaddr*)&player->clientAddr, player->clientAddrLen);
		if (bytesSent == SOCKET_ERROR)
		{
			//std::cerr << "Error sending custom message to client." << std::endl;
		}
		else
		{
			//std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
		}
	}
}

void cCoreServer::response_ping(int client)
{
	char cData[10];
	char* cp = cData;
	Push(cp, (u32)NOTIFY_PING);
	Push(cp, (u16)0);

	m_pClientList[client]->m_pXSock->iSendMsg(cData, cp - cData);
}

void cCoreServer::SaveAllCharacters()
{
	if (m_bShutDownServer) return;
	//	PutLogList("Save all characters");
	for (short i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {

			if (!m_pClientList[i]->m_bIsInitComplete) continue;
			LocalSavePlayerData(i);
		}
	}
}

void cCoreServer::handleBlockMaps(int client)
{
	/*auto p = m_pClientList[client];
	if (!p) return;

	std::unordered_set<std::string> notAllowedMaps = {
		"gshop_1", "cityhall_1", "bsmith_1", "wrhus_1", "wzdtwr_1", "arewrhus", "aresden", "areuni",
		"gshop_2", "cityhall_2", "wrhus_2", "wzdtwr_2", "bsmith_2", "elvwrhus", "elvine", "elvuni",
		"procella", "middleland", "icebound", "middled1n", "huntzone1", "huntzone2", "huntzone3",
		"huntzone4", "middled1x", "MBarracks"
	};

	if (notAllowedMaps.find(std::string(p->m_cMapName)) != notAllowedMaps.end()) {
		RequestTeleportHandler(client, "2   ", "whouse", -1, -1);
	}*/
}

int cCoreServer::getMapIndex(char * mapname)
{
	int result = -1;
	for (int i = 0; i < DEF_MAXMAPS; i++)
	{
		if (m_pMapList[i] != NULL)
		{
			if (string(mapname) == m_pMapList[i]->m_cName)
			{
				result = i;
				break;
			}
		}
	}

	return result;
}

void cCoreServer::createMineral(int client)
{
	//if (m_iCurMineral == 30
	int mapindex = getMapIndex("city");
	if (mapindex != -1)
	{
		int mineralPoints[][2] = {
			{ 29, 26 }, { 70, 104 }, { 115, 89 }, { 149, 91 }, { 173, 100 }, { 173, 115 }, { 192, 125 }, { 210, 129 }, { 225, 125 },
			{ 107, 78 }, { 127, 41 }, { 169, 32 }, { 102, 83 }, { 134, 57 }, { 176, 91 }, { 67, 139 }, { 36, 170 }, { 138, 183 },
			{ 118, 217 }, { 120, 181 }, { 125, 163 }, { 151, 170 }, { 170, 161 }, { 188, 142 }, { 208, 142 }, { 42, 131 }, { 63, 120 },
			{ 112, 169 }, { 144, 171 }, { 178, 144 }
		};

		for (int i = 0; i < sizeof(mineralPoints) / sizeof(mineralPoints[0]); ++i) {

			int dado1 = iDice(1, 6);
			iCreateMineral(mapindex, mineralPoints[i][0], mineralPoints[i][1], dado1);
		}

	}

	int mapindex2 = getMapIndex("2ndmiddle");
	if (mapindex2 != -1)
	{
		int dado = iDice(1, 6);
		int mineralPoints2[][2] = {
			{ 29, 26 }, { 70, 104 }, { 115, 89 }, { 149, 91 }, { 173, 100 }, { 173, 115 }, { 192, 125 }, { 210, 129 }, { 225, 125 },
			{ 107, 78 }, { 127, 41 }, { 169, 32 }, { 102, 83 }, { 134, 57 }, { 176, 91 }, { 67, 139 }, { 36, 170 }, { 138, 183 },
			{ 118, 217 }, { 120, 181 }, { 125, 163 }, { 151, 170 }, { 170, 161 }, { 188, 142 }, { 208, 142 }, { 42, 131 }, { 63, 120 },
			{ 112, 169 }, { 144, 171 }, { 178, 144 }
		};

		for (int i = 0; i < sizeof(mineralPoints2) / sizeof(mineralPoints2[0]); ++i) {

			int dado2 = iDice(1, 6);
			iCreateMineral(mapindex2, mineralPoints2[i][0], mineralPoints2[i][1], dado2);
		}

	}
}

int cCoreServer::calcTotalMinerals()
{
	int count = 0;
	for (int i = 1; i < DEF_MAXMINERALS; i++) {
		if (m_pMineral[i] == NULL) {
			count++;
		}
	}

	return count;
}
/*

void CMapServer::MineralGen()
{
int minezoneindex = getMapIndex("cave");

int totalmines = 0;

for (int i = 1; i < DEF_MAXMINERALS; i++) {
if (m_pMineral[i] != NULL)
{
if (m_pMineral[i]->m_cMapIndex == minezoneindex)
{
totalmines++;
}
}
}

if (totalmines < 30 && minezoneindex != -1)
{
int mineralIndex = iDice(1, 48) - 1;

int mineralPoints[][2] = {

// Nuevas coordenadas
{ 46, 89 }, { 64, 80 }, { 75, 77 }, { 92, 76 }, { 113, 77 }, { 134, 81 }, { 132, 92 },
{ 160, 101 }, { 146, 109 }, { 136, 113 }, { 115, 115 }, { 95, 111 }, { 77, 117 }, { 60, 113 },
{ 46, 103 }, { 81, 101 }, { 71, 95 }, { 109, 96 }, { 94, 83 }, { 59, 89 }, { 73, 106 },
{ 106, 93 }, { 108, 111 }, { 90, 99 }, { 123, 82 }, { 118, 90 }, { 83, 77 }, { 71, 87 },
{ 134, 106 }, { 102, 78 }
};

int level = iDice(1, 6);
iCreateMineral(minezoneindex, mineralPoints[mineralIndex][0], mineralPoints[mineralIndex][1], level);
}
}
*/


void cCoreServer::MineralGen()
{
	int minezoneindex = getMapIndex("dungeon");

	int totalmines = 0;

	for (int i = 1; i < DEF_MAXMINERALS; i++) {
		if (m_pMineral[i] != NULL)
		{
			if (m_pMineral[i]->m_cMapIndex == minezoneindex)
			{
				totalmines++;
			}
		}
	}

	if (totalmines < 40 && minezoneindex != -1)
	{
		int mineralIndex = iDice(1, 48) - 1;

		int mineralPoints[][2] = {
			{ 47, 32 }, { 104, 34 }, { 139, 39 }, { 81, 41 }, { 64, 46 }, { 31, 47 }, { 96, 48 }, { 138, 51 },
			{ 110, 53 }, { 81, 56 }, { 34, 57 }, { 155, 60 }, { 110, 66 }, { 129, 69 }, { 139, 76 }, { 28, 77 },
			{ 167, 77 }, { 60, 80 }, { 106, 81 }, { 152, 88 }, { 85, 95 }, { 67, 96 }, { 111, 97 }, { 136, 97 },
			{ 65, 122 }, { 144, 106 }, { 99, 108 }, { 118, 108 }, { 68, 110 }, { 172, 114 }, { 38, 118 }, { 134, 120 },
			{ 149, 120 }, { 80, 121 }, { 54, 129 }, { 36, 130 }, { 83, 135 }, { 134, 137 }, { 156, 137 }, { 109, 144 },
			{ 123, 144 }, { 39, 152 }, { 92, 153 }, { 58, 159 }, { 115, 165 }, { 72, 172 }, { 96, 172 }
		};

		int level = iDice(1, 6);
		iCreateMineral(minezoneindex, mineralPoints[mineralIndex][0], mineralPoints[mineralIndex][1], level);
	}
}


void cCoreServer::get_pits()
{
	m_vPits.clear();

	struct PitsData
	{
		static void tokenize(string const &str, const char* delim,
			std::vector<string> &out)
		{
			char *token = strtok(const_cast<char*>(str.c_str()), delim);
			while (token != nullptr)
			{
				out.push_back(string(token));
				token = strtok(nullptr, delim);
			}
		}

		static bool is_wanted_line(const string & line, string str)
		{
			return (line.find(str) != string::npos);
		}

		static string get_line(string file, string value1)
		{
			ifstream fin(file);

			string line;

			while (getline(fin, line))
			{
				if (is_wanted_line(line, value1))
					return line;
			}

			return "#";
		}

		static string getvalue(string val)
		{
			string result = get_line("Configs\\Pits.cfg", val);
			if (string(result) == "#") return result;
			else result.erase(0, val.length());
			return result;
		}

		static void read_data()
		{
			string token;
			int maxpits = 300;
			for (int i = 0; i < maxpits; i++)
			{
				string getval = "pit-";
				getval.append(to_string(i + 1));
				getval.append(" = ");
				token = getvalue(getval);

				if (string(token) == "#") continue;

				const char* delim = " ";
				vector<string> out;
				tokenize(token, delim, out);

				Pits u;
				u.pit_handle = i + 1;

				int count = 0;
				for (auto &token : out) {
					count++;
					switch (count)
					{
					case 1:
						ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
						strcpy(u.m_cMapName, (char*)token.c_str());
						break;

					case 2:
						ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
						strcpy(u.m_cNpcName, (char*)token.c_str());
						break;

					case 3:
						u.x1 = atoi((char*)token.c_str());
						break;

					case 4:
						u.y1 = atoi((char*)token.c_str());
						break;

					case 5:
						u.x2 = atoi((char*)token.c_str());
						break;

					case 6:
						u.y2 = atoi((char*)token.c_str());
						break;

					case 7:
						u.quant = atoi((char*)token.c_str());
						break;

					default: break;
					}
				}

				c_map->m_vPits.push_back(u);
			}
		}
	};

	PitsData::read_data();
}

void cCoreServer::MobSpotGenerator()
{
	if (g_ev.Is(EventID::CTR)) return;
	if (g_ev.Is(EventID::Crusada)) return;
	if (!m_bMobGenerator) return;

	for (int i = 0; i < m_vPits.size(); i++)
	{
		auto pit = m_vPits[i];
		auto count = Checkmobcount(pit.m_cMapName, pit.m_cNpcName, pit.pit_handle);
		auto cantidadsummon = pit.quant - count;
		if (count < pit.quant)
		{
			if (g_ev.Is(EventID::CTR))
			{
				if (string(pit.m_cNpcName) == "Guard_Sword-Elvine" || string(pit.m_cNpcName) == "Guard_Sword-Elvine" ||
					string(pit.m_cNpcName) == "Guard_Archer-Elvine" || string(pit.m_cNpcName) == "Guard_Archer-Elvine" ||
					string(pit.m_cNpcName) == "Guard_Sword-Aresden" || string(pit.m_cNpcName) == "Guard_Sword-Aresden" ||
					string(pit.m_cNpcName) == "Guard_Archer-Aresden" || string(pit.m_cNpcName) == "Guard_Archer-Aresden")
				{
					continue;
				}
			}


			if (string(pit.m_cNpcName) == "EManaStone")
			{
				if (!g_ev.Is(EventID::CTR) && string(pit.m_cNpcName) == "EManaStone") continue;
				if (!m_bSummonStones)
				{
					m_sSummonStones++;
					if (m_sSummonStones == 4) m_bSummonStones = true;
					SummonMob(pit.m_cNpcName, pit.m_cMapName, 0, 0, 10, i, cantidadsummon, pit.pit_handle);
				}
			}
			else
			{
				SummonMob(pit.m_cNpcName, pit.m_cMapName, 0, 0, 10, i, cantidadsummon, pit.pit_handle);
			}

		}
	}
}

int cCoreServer::Checkmobcount(char * mapname, char * npcname, int pitnum)
{
	int count = 0;
	for (int i = 0; i < DEF_MAXNPCS; i++)
	{
		auto npc = m_pNpcList[i];
		if (!npc) continue;
		if (pitnum != npc->pitnum) continue;
		if (string(m_pMapList[npc->m_cMapIndex]->m_cName) == mapname && string(npc->m_cNpcName) == npcname)
			count++;
	}
	return count;
}


void cCoreServer::SummonMob(char * npcname, char * mapname, int x, int y, char side, int npc, int cantidad, int pitnum, bool sademob, short nside)
{
	char   cName_Master[10], cNpcName[256], cWaypoint[11], cSA;
	int    pX, pY, iNum, iNamingValue;
	BOOL   bMaster;
	ZeroMemory(cNpcName, sizeof(cNpcName));
	strcpy(cNpcName, npcname);

	iNum = 1;

	cSA = 0;
	pX = x;
	pY = y;

	for (int i = 0; i < cantidad; i++)
	{
		auto map = m_pMapList[iGetMapIndex(mapname)];
		iNamingValue = map->iGetEmptyNamingValue();
		if (iNamingValue != -1)
		{
			ZeroMemory(cName_Master, sizeof(cName_Master));
			wsprintf(cName_Master, "XX%d", iNamingValue);
			cName_Master[0] = '_';

			int mapindex = getMapIndex(mapname);
			if (mapindex != -1)
			{
				cName_Master[1] = mapindex + 65;
			}
			else return;

			char city;
			if (string(npcname) == "Cat" || string(npcname) == "Rabbit" || string(npcname) == "Unicorn")  city = DEF_NETURAL; else city = -1;
			RECT rcRect = { m_vPits[npc].x1, m_vPits[npc].y1, m_vPits[npc].x2, m_vPits[npc].y2 };

			char SpecAbility = 0;
			BOOL Zerk = FALSE;
			
			int spec = 0;

			if ((bMaster = bCreatePitNpc(pitnum, cNpcName, cName_Master, map->m_cName, (rand() % 3), spec, DEF_MOVETYPE_RANDOMAREA, &pX, &pY, cWaypoint, &rcRect, NULL, city, FALSE, FALSE, Zerk, TRUE, FALSE)) == FALSE)
			{
				map->SetNamingValueEmpty(iNamingValue);
			}
		}
	}
}

BOOL cCoreServer::bCreatePitNpc(int pitnum, char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, BOOL bHideGenMode, BOOL bIsSummoned, BOOL bFirmBerserk, BOOL bIsMaster, int iGuildGUID)
{
#ifdef DEF_DEBUG
	try {
#endif

		register int i, t, j, k, iMapIndex;
		char  cTmpName[11], cTxt[120];
		short sX, sY, sRange;
		BOOL  bFlag;
		SYSTEMTIME SysTime;

		if (strlen(pName) == 0)   return FALSE;
		if (strlen(pNpcName) == 0) return FALSE;

		if (string(pNpcName) == "Guard-Aresden" || string(pNpcName) == "Guard-Elvine" || string(pNpcName) == "Guard-Neutral" ||			
			string(pNpcName) == "Guard_Sword-Aresden" || string(pNpcName) == "Guard_Sword-Elvine" || string(pNpcName) == "Guard_Sword-Neutral" ||			
			string(pNpcName) == "Guard_Axe-Aresden" || string(pNpcName) == "Guard_Axe-Elvine" || string(pNpcName) == "Guard_Axe-Neutral" ||			
			string(pNpcName) == "Guard_Archer-Aresden" || string(pNpcName) == "Guard_Archer-Elvine" || string(pNpcName) == "Guard_Archer-Neutral")	return FALSE;		

		GetLocalTime(&SysTime);

		ZeroMemory(cTmpName, sizeof(cTmpName));
		strcpy_secure(cTmpName, pMapName);
		iMapIndex = -1;

		for (i = 0; i < DEF_MAXMAPS; i++) {
			if (m_pMapList[i] != NULL) {
				if (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)
					iMapIndex = i;
			}
		}
		//lalo Abaddon
		if ((m_bIsApocalypseMode == TRUE) && (memcmp(cTmpName, "abaddon", 7) == 0) && (memcmp(pNpcName, "Abaddon", 7) != 0)) return FALSE;

		for (i = 0; i < DEF_MAXMAPS; i++) {
			if (m_pMapList[i] != NULL) {
				if (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)
					iMapIndex = i;
			}
		}

		if (iMapIndex == -1) return FALSE;

		for (i = 1; i < DEF_MAXNPCS; i++) {
			if (m_pNpcList[i] == NULL) {
				m_pNpcList[i] = new class CNpc(pName);

				if (_bInitNpcAttr(m_pNpcList[i], pNpcName, sClass, cSA) == FALSE) {
					wsprintf(cTxt, "Not existing NPC creation request! (%s) Ignored.", pNpcName);
					PutLogList(cTxt);

					delete m_pNpcList[i];
					m_pNpcList[i] = NULL;
					return FALSE;
				}

				m_pNpcList[i]->m_iMaxHP = m_pNpcList[i]->m_iHP;

				if (m_pNpcList[i]->m_cDayOfWeekLimit < 10) {
					if (m_pNpcList[i]->m_cDayOfWeekLimit != SysTime.wDayOfWeek) {
						delete m_pNpcList[i];
						m_pNpcList[i] = NULL;
						return FALSE;
					}
				}

				ZeroMemory(m_pNpcList[i]->m_cMapName, sizeof(m_pNpcList[i]->m_cMapName));
				strcpy(m_pNpcList[i]->m_cMapName, pMapName);

				
				/*			// Mostrar el m_sType en la consola
				wsprintf(cTxt, "NPC creado %s m_sType: %d", m_pNpcList[i]->m_cNpcName, m_pNpcList[i]->m_sType);
				PutLogList(cTxt); // Imprime en la consola o en el archivo de log
				*/

				switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
				case DEF_MOVETYPE_RANDOM:
					if ((poX != NULL) && (poY != NULL) && (*poX != NULL) && (*poY != NULL)) {
						sX = *poX;
						sY = *poY;
					}
					else {
						for (j = 0; j <= 30; j++) {
							sX = (rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15;
							sY = (rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15;

							bFlag = TRUE;
							for (k = 0; k < DEF_MAXMGAR; k++) {
								if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left != -1) {
									if ((sX >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left) &&
										(sX <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].right) &&
										(sY >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].top) &&
										(sY <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].bottom)) {
										bFlag = FALSE;
									}
								}
							}
							if (bFlag == TRUE) goto GET_VALIDLOC_SUCCESS;
						}
						delete m_pNpcList[i];
						m_pNpcList[i] = NULL;
						return FALSE;

					GET_VALIDLOC_SUCCESS:;
					}
					break;

				case DEF_MOVETYPE_RANDOMAREA:
					sRange = (short)(pArea->right - pArea->left);
					sX = (short)((rand() % sRange) + pArea->left);
					sRange = (short)(pArea->bottom - pArea->top);
					sY = (short)((rand() % sRange) + pArea->top);
					break;

				case DEF_MOVETYPE_RANDOMWAYPOINT:
					sX = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].x;
					sY = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].y;
					break;

				default:
					if ((poX != NULL) && (poY != NULL) && (*poX != NULL) && (*poY != NULL)) {
						sX = *poX;
						sY = *poY;
					}
					else {
						sX = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].x;
						sY = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].y;
					}
					break;
				}

				if (m_pNpcList[i]->m_sAreaSize == 0) {
					if (bGetEmptyPosition(&sX, &sY, iMapIndex) == FALSE) {
						delete m_pNpcList[i];
						m_pNpcList[i] = NULL;
						return FALSE;
					}
				}
				else {
					if (bGetNpcMovementArea(i, sX, sY, iMapIndex, m_pNpcList[i]->m_sAreaSize) == FALSE) {
						delete m_pNpcList[i];
						m_pNpcList[i] = NULL;
						return FALSE;
					}
				}

				if ((bHideGenMode == TRUE) && (_iGetPlayerNumberOnSpot(sX, sY, iMapIndex, 7) != 0)) {
					delete m_pNpcList[i];
					m_pNpcList[i] = NULL;
					return FALSE;
				}

				if ((poX != NULL) && (poY != NULL)) {
					*poX = sX;
					*poY = sY;
				}

				m_pNpcList[i]->m_sX = sX;
				m_pNpcList[i]->m_sY = sY;

				m_pNpcList[i]->m_vX = sX;
				m_pNpcList[i]->m_vY = sY;

				for (t = 0; t < 10; t++)
					m_pNpcList[i]->m_iWayPointIndex[t] = pWaypointList[t];

				m_pNpcList[i]->m_cTotalWaypoint = 0;
				for (t = 0; t < 10; t++)
					if (m_pNpcList[i]->m_iWayPointIndex[t] != -1) m_pNpcList[i]->m_cTotalWaypoint++;

				if (pArea != NULL) {
					// RANDOMAREA Copy
					SetRect(&m_pNpcList[i]->m_rcRandomArea, pArea->left, pArea->top, pArea->right, pArea->bottom);
				}


				if (pitnum != -1)
				{
					m_pNpcList[i]->pitnum = pitnum;
				}

				switch (cMoveType) {
				case DEF_MOVETYPE_GUARD:
					m_pNpcList[i]->m_dX = m_pNpcList[i]->m_sX;
					m_pNpcList[i]->m_dY = m_pNpcList[i]->m_sY;
					break;

				case DEF_MOVETYPE_SEQWAYPOINT:
					m_pNpcList[i]->m_cCurWaypoint = 1;

					m_pNpcList[i]->m_dX = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].x;
					m_pNpcList[i]->m_dY = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].y;
					break;

				case DEF_MOVETYPE_RANDOMWAYPOINT:
					m_pNpcList[i]->m_cCurWaypoint = (rand() % (m_pNpcList[i]->m_cTotalWaypoint - 1)) + 1;

					m_pNpcList[i]->m_dX = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].x;
					m_pNpcList[i]->m_dY = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].y;
					break;

				case DEF_MOVETYPE_RANDOMAREA:
					m_pNpcList[i]->m_cCurWaypoint = 0;

					sRange = (short)(m_pNpcList[i]->m_rcRandomArea.right - m_pNpcList[i]->m_rcRandomArea.left);
					m_pNpcList[i]->m_dX = (short)((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.left);
					sRange = (short)(m_pNpcList[i]->m_rcRandomArea.bottom - m_pNpcList[i]->m_rcRandomArea.top);
					m_pNpcList[i]->m_dY = (short)((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.top);
					break;

				case DEF_MOVETYPE_RANDOM:
					m_pNpcList[i]->m_dX = (short)((rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15);
					m_pNpcList[i]->m_dY = (short)((rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15);
					break;
				}

				m_pNpcList[i]->m_tmp_iError = 0;
				m_pNpcList[i]->m_cMoveType = cMoveType;

				switch (m_pNpcList[i]->m_cActionLimit) {
				case 2:
				case 3:
				case 5:
					m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_STOP;

					switch (m_pNpcList[i]->m_sType) {
					case 15: // ShopKeeper-W
					case 19: // Gandlf
					case 20: // Howard
					case 24: // Tom
					case 25: // William
					case 26: // Kennedy
						m_pNpcList[i]->m_cDir = 4 + iDice(1, 3) - 1;
						break;
					case 91:
						m_pNpcList[i]->m_cDir = 3;
						break;
					case 111:
						m_pNpcList[i]->m_cDir = 0;
						break;
					default:
						m_pNpcList[i]->m_cDir = iDice(1, 8);
						break;
					}
					break;

				default:
					m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
					m_pNpcList[i]->m_cDir = 5;
					break;
				}
				m_pNpcList[i]->m_iFollowOwnerIndex = NULL;
				m_pNpcList[i]->m_iTargetIndex = NULL;
				m_pNpcList[i]->m_cTurn = (rand() % 2);

				switch (m_pNpcList[i]->m_sType) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					m_pNpcList[i]->m_sAppr2 = (short)0xF000;
					m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | ((rand() % 13) << 4); // ����
					m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | (rand() % 9); // ���� 
					break;

				case 36:
				case 37:
					m_pNpcList[i]->m_sAppr2 = 3;
					break;

				case 38: // MS-Aresden/MS-Elvine
				case 39: // DT-Aresden/DT-Elvine
					if ((m_bIsRushNetMode == TRUE) || (RunSummonEvent) || (RunSummonEvent2) || (RunSummonEvent3))
						m_pNpcList[i]->m_sAppr2 = 0;
					else
						m_pNpcList[i]->m_sAppr2 = 3;
					break;

				case 66:
				case 73:
				case 114:
					//case 128:
					//case 120:
					m_pNpcList[i]->m_sAreaSize = 3;
					break;

				case 81:
					// ZeroEoyPnk 
					m_pNpcList[i]->m_sAreaSize = 3;
					if (PriceEvent == TRUE)
						m_pNpcList[i]->m_iHP = DEF_MAXHPRELIQUIA;
					break;

				case 92:
					// ZeroEoyPnk 
					if (m_bIsRushNetMode)
						m_pNpcList[i]->m_iHP = DEF_MAXHPRELIQUIA; //DEF_MAXHPRELIQUIAHOD
					break;
					if (HellOfDeath)
						m_pNpcList[i]->m_iHP = DEF_MAXHPRELIQUIAHOD; //DEF_MAXHPRELIQUIAHOD
					break;

				case 120:
					// ZeroEoyPnk 
					m_pNpcList[i]->m_sAreaSize = 3;
					if (WWPriceEvent == TRUE)
						m_pNpcList[i]->m_iHP = DEF_MAXHPRELIQUIA;
					break;

				case 128:
					// ZeroEoyPnk 
					m_pNpcList[i]->m_sAreaSize = 3;
					if (WWIIPriceEvent == TRUE)
						m_pNpcList[i]->m_iHP = DEF_MAXHPRELIQUIAWWII;
					break;

				default:
					m_pNpcList[i]->m_sAppr2 = 0;
					break;
				}

				m_pNpcList[i]->m_cMapIndex = (char)iMapIndex;
				m_pNpcList[i]->m_dwTime = timeGetTime() + (rand() % 10000);
				m_pNpcList[i]->m_dwActionTime += (rand() % 300);
				m_pNpcList[i]->m_dwMPupTime = timeGetTime();
				m_pNpcList[i]->m_dwHPupTime = m_pNpcList[i]->m_dwMPupTime;
				m_pNpcList[i]->m_sBehaviorTurnCount = 0;
				m_pNpcList[i]->m_bIsSummoned = bIsSummoned;
				m_pNpcList[i]->m_bIsMaster = bIsMaster;
				if (bIsSummoned == TRUE)
					m_pNpcList[i]->m_dwSummonedTime = timeGetTime();

				if (bFirmBerserk == TRUE) {
					m_pNpcList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = 1;
					m_pNpcList[i]->m_iStatus = m_pNpcList[i]->m_iStatus | 0x20; // 100000
					m_pNpcList[i]->m_iExp *= 2;
				}

				if (cChangeSide != -1) m_pNpcList[i]->m_cSide = cChangeSide;

				m_pNpcList[i]->m_cBravery = (rand() % 3) + m_pNpcList[i]->m_iMinBravery;
				m_pNpcList[i]->m_iSpotMobIndex = iSpotMobIndex;
				m_pNpcList[i]->m_iGuildGUID = iGuildGUID;

				if (iGuildGUID != 0) {
					wsprintf(G_cTxt, "Summon War Unit(%d) GUID(%d)", m_pNpcList[i]->m_sType, iGuildGUID);
					PutLogList(G_cTxt);
				}

				if (m_pNpcList[i]->m_sAreaSize == 0)
					m_pMapList[iMapIndex]->SetOwner(i, DEF_OWNERTYPE_NPC, sX, sY);
				else m_pMapList[iMapIndex]->SetBigOwner(i, DEF_OWNERTYPE_NPC, sX, sY, m_pNpcList[i]->m_sAreaSize);

				m_pMapList[iMapIndex]->m_iTotalActiveObject++;
				m_pMapList[iMapIndex]->m_iTotalAliveObject++;

				switch (m_pNpcList[i]->m_sType) {
				case 36:
				case 37:
				case 38:
				case 39:
				case 42:
					m_pMapList[iMapIndex]->bAddCrusadeStructureInfo((char)m_pNpcList[i]->m_sType, sX, sY, m_pNpcList[i]->m_cSide);
					break;
				case 64:	//v2.19 2002-12-16 ���� ��ų ����
					m_pMapList[iMapIndex]->bAddCropsTotalSum();
					return i; // ���� �� �������� �־ ���۹��� ���쿡�� ���⼭ ���� �Ѵ�.
					break;
				}

				SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_NPC, CLIENT_COMMON_COMMAND_LOG, DEF_MSGTYPE_CONFIRM, NULL, NULL, NULL);
				return i;
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: bCreateNewNpc");
	}
#endif
	return FALSE;
}

void cCoreServer::DeleteStones()
{
	for (int i = 1; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != NULL) {
			if (string(m_pNpcList[i]->m_cNpcName) == "EManaStone") DeleteNpc(i);
		}
	}
}

void cCoreServer::DeleteGuards()
{
	for (int i = 1; i < DEF_MAXNPCS; i++) {
		if (m_pNpcList[i] != NULL) {
			if (string(m_pNpcList[i]->m_cNpcName) == "Guard_Sword-Elvine" || string(m_pNpcList[i]->m_cNpcName) == "Guard_Sword-Elvine" ||
				string(m_pNpcList[i]->m_cNpcName) == "Guard_Archer-Elvine" || string(m_pNpcList[i]->m_cNpcName) == "Guard_Archer-Elvine" ||
				string(m_pNpcList[i]->m_cNpcName) == "Guard_Sword-Aresden" || string(m_pNpcList[i]->m_cNpcName) == "Guard_Sword-Aresden" ||
				string(m_pNpcList[i]->m_cNpcName) == "Guard_Archer-Aresden" || string(m_pNpcList[i]->m_cNpcName) == "Guard_Archer-Aresden") DeleteNpc(i);
		}
	}
}

bool cCoreServer::bCheckClientMoveFrequency(int client, BOOL running)
{
	DWORD dwTimeGap;
	DWORD dwTime = timeGetTime();

	CClient * player = m_pClientList[client];
	if (player == NULL) return FALSE;

	if (player->m_iAdminUserLevel > 0) return TRUE;

	/*if (player->m_dwMoveFreqTime == NULL)
	player->m_dwMoveFreqTime = dwTime;
	else {
	if (player->m_bIsMoveBlocked == TRUE) {
	player->m_dwMoveFreqTime = NULL;
	player->m_bIsMoveBlocked = FALSE;
	return TRUE;
	}

	if (player->m_bIsAttackModeChange == TRUE) {
	player->m_dwMoveFreqTime = NULL;
	player->m_bIsAttackModeChange = FALSE;
	return TRUE;
	}

	dwTimeGap = dwTime - player->m_dwMoveFreqTime;
	player->m_dwMoveFreqTime = dwTime;

	// Mostrar el tiempo de movimiento
	if (running)
	{
	player->m_runTime[player->m_runTurn] = dwTimeGap;

	uint32 sum = 0;
	for (int i = 0; i < 7; i++) {
	sum += player->m_runTime[i];
	}

	// Imprimir el tiempo de movimiento en caso de que el jugador esté corriendo
	wsprintf(G_cTxt, "Player %s ran with time gap: %d ms", player->m_cCharName, dwTimeGap);
	PutLogList(G_cTxt);

	if (sum < 200 * 7)
	{
	wsprintf(G_cTxt, "(-~-HACKING-~-) Speed hacker detected(%s) - run-avg(%i). BI banned", player->m_cCharName, sum / 7);
	PutLogList(G_cTxt);
	}
	else if (sum < 220 * 7)
	{
	wsprintf(G_cTxt, "(-~-HACKING-~-) Speed hack suspect(%s) - run-avg(%i)", player->m_cCharName, sum / 7);
	PutLogList(G_cTxt);
	++player->m_runTurn %= 7;
	}
	++player->m_runTurn %= 7;
	}
	else {
	player->m_moveTime[player->m_moveTurn] = dwTimeGap;

	uint32 sum = 0;
	for (int i = 0; i < 7; i++)
	sum += player->m_moveTime[i];

	// Imprimir el tiempo de movimiento en caso de que el jugador no esté corriendo
	wsprintf(G_cTxt, "Player %s moved with time gap: %d ms", player->m_cCharName, dwTimeGap);
	PutLogList(G_cTxt);

	if (sum < 300 * 7)
	{
	wsprintf(G_cTxt, "(-~-HACKING-~-) Speed hacker detected(%s) - move-avg(%i). BI banned", player->m_cCharName, sum / 7);
	PutLogList(G_cTxt);
	}
	else if (sum < 400 * 7)
	{
	wsprintf(G_cTxt, "(-~-HACKING-~-) Speed hack suspect(%s) - move-avg(%i)", player->m_cCharName, sum / 7);
	PutLogList(G_cTxt);
	++player->m_moveTurn %= 7;
	}
	++player->m_moveTurn %= 7;
	}
	}

	return TRUE;*/
	return FALSE;
}


void cCoreServer::handleVipRegenCharacter(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_iCharacterRegen == 0) return;

	if (p->IsInMap("curfarm")) return;
	if (p->IsInMap("dm") ||
		p->IsInMap("battle") ||
		p->IsInMap("dk") ||
		p->IsInMap("gg")) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastSpecCharacterExecutionTime < p->m_iCharacterRegenTime _s)	return;

	p->lastSpecCharacterExecutionTime = currentTime;

	if (p->m_iCharacterRegen == 1)
	{
		if (p->m_iCharacterRegenMode == 1)
		{
			if (!p->m_bIsKilled && p->m_iHP > 0)
			{
				int newhp = p->m_iHP + p->m_iHP;
				if (newhp > iGetMaxHP(client))
					newhp = iGetMaxHP(client);

				p->m_iHP = newhp;

				p->m_iMP = p->GetMaxMP();
				SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
				SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
			}
		}
		else if (p->m_iCharacterRegenMode == 2)
		{
			int newhp = iGetMaxHP(client);

			p->m_iHP = newhp;

			p->m_iSP = p->GetMaxSP();
			p->m_iMP = p->GetMaxMP();
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SP, NULL, NULL, NULL, NULL);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
		}
	}
}


void cCoreServer::handleRegenTimers(int client)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->IsInMap("curfarm")) return;

	if (p->IsInMap("dm") ||
		p->IsInMap("battle") ||
		p->IsInMap("dk") ||
		p->IsInMap("gg")) return;

	if (p->m_iCharacterRegen == 1) return;

	if (p->m_sCurrentMastery == MasteryRegen)
	{
		switch (p->m_sRegenLevel)
		{
		case 1:
			handleRegenTimerLvl1(client);
			break;

		case 2:
			handleRegenTimerLvl2(client);
			break;

		case 3:
			handleRegenTimerLvl3(client);
			break;

		case 4:
			handleRegenTimerLvl4(client);
			break;

		case 5:
			handleRegenTimerLvl5(client);
			break;

		default:
			break;
		}
	}
}

void cCoreServer::handleRegenTimerLvl1(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastRegenLevel1ExecTime < 15000)	return;

	p->lastRegenLevel1ExecTime = currentTime;

	if (!p->m_bIsKilled && p->m_iHP > 0)
	{
		int newhp = p->m_iHP + p->m_iHP;
		if (newhp > iGetMaxHP(client))
			newhp = iGetMaxHP(client);

		p->m_iHP = newhp;

		p->m_iMP = p->GetMaxMP();
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
	}

}



void cCoreServer::handleRegenTimerLvl2(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastRegenLevel2ExecTime < 12000)	return;

	p->lastRegenLevel2ExecTime = currentTime;


	if (!p->m_bIsKilled && p->m_iHP > 0)
	{
		int newhp = p->m_iHP + p->m_iHP;
		if (newhp > iGetMaxHP(client))
			newhp = iGetMaxHP(client);

		p->m_iHP = newhp;

		p->m_iMP = p->GetMaxMP();
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
	}

}

void cCoreServer::handleRegenTimerLvl3(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastRegenLevel3ExecTime < 10000)	return;

	p->lastRegenLevel3ExecTime = currentTime;


	if (!p->m_bIsKilled && p->m_iHP > 0)
	{
		int newhp = p->m_iHP + p->m_iHP;
		if (newhp > iGetMaxHP(client))
			newhp = iGetMaxHP(client);

		p->m_iHP = newhp;

		p->m_iMP = p->GetMaxMP();
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
	}

}

void cCoreServer::handleRegenTimerLvl4(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastRegenLevel4ExecTime < 8000)	return;

	p->lastRegenLevel4ExecTime = currentTime;


	if (!p->m_bIsKilled && p->m_iHP > 0)
	{
		int newhp = p->m_iHP + p->m_iHP;
		if (newhp > iGetMaxHP(client))
			newhp = iGetMaxHP(client);

		p->m_iHP = newhp;

		p->m_iMP = p->GetMaxMP();
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
	}

}

void cCoreServer::handleRegenTimerLvl5(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastRegenLevel5ExecTime < 6000)	return;

	p->lastRegenLevel5ExecTime = currentTime;

	if (!p->m_bIsKilled && p->m_iHP > 0)
	{
		int newhp = p->m_iHP + p->m_iHP;
		if (newhp > iGetMaxHP(client))
			newhp = iGetMaxHP(client);

		p->m_iHP = newhp;

		p->m_iMP = p->GetMaxMP();
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);
	}
}

void cCoreServer::withdraw_DropItem(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int itemIndex = -1;
	Pop(data, itemIndex);

	if (itemIndex >= 0 && itemIndex < p->m_vDropItemList.size())
	{
		auto drop = p->m_vDropItemList[itemIndex];
		handleDropItem(client, drop.m_cItemName, drop.attr, drop.itemcount, itemIndex);
	}
}

bool cCoreServer::countableItems(string itemname)
{
	return string(itemname) == "RepBall" || string(itemname) == "ContribBall" || string(itemname) == "Tokens" || string(itemname) == "CoinBall" ||
		string(itemname) == "LightShards" || string(itemname) == "StrongShards" || string(itemname) == "HitProbFragments" || string(itemname) == "DRFragments" ||
		string(itemname) == "HPRecFragments" || string(itemname) == "SPRecFragments" || string(itemname) == "MPRecFragments" || string(itemname) == "MRFragments" ||
		string(itemname) == "PaFragments" || string(itemname) == "MaFragments" || string(itemname) == "ExpFragments" || string(itemname) == "MajBall" ||
		string(itemname) == "Iron" || string(itemname) == "Copper" || string(itemname) == "Tin" || string(itemname) == "Bronze" || string(itemname) == "Silver" ||
		string(itemname) == "Steel" || string(itemname) == "Platinum" || string(itemname) == "Obsidian" || string(itemname) == "Cobalt" ||
		string(itemname) == "Mithril" || string(itemname) == "Adamantite" || string(itemname) == "Orichalcum" || string(itemname) == "Titanium" ||
		string(itemname) == "Darksteel" || string(itemname) == "Palladium" || string(itemname) == "Aetherium" || string(itemname) == "Dragonite" ||
		string(itemname) == "Fish" || string(itemname) == "RedFish" || string(itemname) == "GreenFish" || string(itemname) == "YellowFish" ||
		string(itemname) == "GoldFish" || string(itemname) == "ClownFish" || string(itemname) == "AngelFish" || string(itemname) == "BettaFish" ||
		string(itemname) == "ParrotFish" || string(itemname) == "LionFish" || string(itemname) == "PufferFish" || string(itemname) == "BluefinFish" ||
		string(itemname) == "Shark" || string(itemname) == "Eclipsium" || string(itemname) == "Voidsteel" || string(itemname) == "Astronyx" || string(itemname) == "Netherite" ||

		string(itemname) == "Bloodstone" || string(itemname) == "Ashbone" ||
		string(itemname) == "Ironshade" || string(itemname) == "Oblivionite" ||
		string(itemname) == "WhiteShark" || string(itemname) == "BlueShark" ||
		string(itemname) == "LemonShark" || string(itemname) == "Sunfish" ||
		string(itemname) == "Blobfish" || string(itemname) == "Viperfish" ||


		string(itemname) == "Ferronyx" || string(itemname) == "Lunacite" ||
		string(itemname) == "Gravitanium" || string(itemname) == "Pyrosite" ||
		string(itemname) == "Crytherium" || string(itemname) == "Voltrium" ||

		string(itemname) == "TetraFish" || string(itemname) == "TangFish" ||
		string(itemname) == "ButterFish" || string(itemname) == "CopperFish" ||
		string(itemname) == "ReefFish" ||

		string(itemname) == "Solvaris" ||
		string(itemname) == "Cryovale" ||
		string(itemname) == "Lumivor" ||
		string(itemname) == "Emberveil" ||
		string(itemname) == "Pyrelith" ||

		string(itemname) == "SkyOctopus" ||
		string(itemname) == "PinkOctopus" ||
		string(itemname) == "RedOctopus" ||
		string(itemname) == "SkyHorse" ||
		string(itemname) == "OrangeHorse" ||
		string(itemname) == "Megalodon" ||

		string(itemname) == "FireSegments" ||
		string(itemname) == "IceSegments" ||
		string(itemname) == "LightningSegments" ||

		// Nuevos ítems añadidos
		string(itemname) == "CyclopsEye" || string(itemname) == "CyclopsHandEdge" || string(itemname) == "CyclopsHeart" || string(itemname) == "CyclopsMeat" ||
		string(itemname) == "CyclopsLeather" || string(itemname) == "HelboundHeart" || string(itemname) == "HelboundLeather" || string(itemname) == "HelboundTail" ||
		string(itemname) == "HelboundTeeth" || string(itemname) == "HelboundClaw" || string(itemname) == "HelboundTongue" || string(itemname) == "LumpofClay" ||
		string(itemname) == "OrcMeat" || string(itemname) == "OrcLeather" || string(itemname) == "OrcTeeth" || string(itemname) == "OgreHair" ||
		string(itemname) == "OgreHeart" || string(itemname) == "OgreMeat" || string(itemname) == "OgreLeather" || string(itemname) == "OgreTeeth" ||
		string(itemname) == "OgreClaw" || string(itemname) == "ScorpionPincers" || string(itemname) == "ScorpionMeat" || string(itemname) == "ScorpionSting" ||
		string(itemname) == "ScorpionSkin" || string(itemname) == "SkeletonBones" || string(itemname) == "SlimeJelly" || string(itemname) == "StoneGolemPiece" ||
		string(itemname) == "TrollHeart" || string(itemname) == "TrollMeat" || string(itemname) == "TrollLeather" || string(itemname) == "TrollClaw" ||
		string(itemname) == "DemonEye" || string(itemname) == "DemonHeart" || string(itemname) == "DemonMeat" || string(itemname) == "DemonLeather" ||
		string(itemname) == "UnicornHeart" || string(itemname) == "UnicornHorn" || string(itemname) == "UnicornMeat" || string(itemname) == "UnicornLeather" ||
		string(itemname) == "WerewolfHeart" || string(itemname) == "WerewolfNail" || string(itemname) == "WerewolfMeat" || string(itemname) == "WerewolfTail" ||
		string(itemname) == "WerewolfTeeth" || string(itemname) == "WerewolfLeather" || string(itemname) == "WerewolfClaw";
}

void cCoreServer::handleDropItem(int client, string itemname, DWORD atr, int count, int itemIndex)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->IsInMap("dkcity") || p->IsInMap("dkshop1") || p->IsInMap("dkshop2") || p->IsInMap("gg") || p->IsInMap("battle")) return;

	if (countableItems(itemname))
	{
		bool haveItem = false;
		int haveItemIndex = -1;
		for (int i = 0; i < DEF_MAXITEMS; i++)
		{
			if (p->m_pItemList[i] != NULL)
			{
				if (string(itemname) == p->m_pItemList[i]->m_cName)
				{
					haveItem = true;
					haveItemIndex = i;
					break;
				}
			}
		}

		if (haveItem && haveItemIndex != -1 && p->m_pItemList[haveItemIndex] != NULL)
		{
			p->m_vDropItemList.erase(p->m_vDropItemList.begin() + itemIndex);
			send_dropitems(client);

			IncreaseItemCount(client, haveItemIndex, count);
			wsprintf(G_cTxt, "Item %s increase count %d points", p->m_pItemList[haveItemIndex]->m_cName, count);
			ShowClientMsg(client, G_cTxt);
			SendCommand(client, "/addnewitem");

		}
		else
		{
			CreateItem(client, itemname, atr, count, itemIndex);
		}
	}
	else
	{
		CreateItem(client, itemname, atr, count, itemIndex);
	}
}

void cCoreServer::IncreaseItemCount(int client, int index, DWORD count)
{
	auto p = m_pClientList[client];
	if (!p) return;

	auto item = p->m_pItemList[index];
	if (!item) return;

	item->m_dwCount += count;

	SendCommand(client, "/additemcount", index, count);

}

void cCoreServer::CreateItem(int client, string itemname, DWORD atr, int count, int itemIndex)
{
	auto p = m_pClientList[client];
	if (!p) return;

	class  CItem * pItem;
	char   cItemName[256];
	int    iEraseReq;

	int starterSize = 1;
	if (!HasEnoughInventorySpace(client, starterSize))
	{
		wsprintf(G_cTxt, "¡Necesitas %d espacio(s) libre para extraer el item!", starterSize);
		ShowClientMsg(client, G_cTxt);
		return;
	}

	p->m_vDropItemList.erase(p->m_vDropItemList.begin() + itemIndex);
	send_dropitems(client);

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, (char*)itemname.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (!_bAddClientItemListWithoutWeight(client, pItem, &iEraseReq)) return;

	if (IsSpecialWeapon(pItem->m_cName))
	{
		int dice = RollDice(1, 5);
		pItem->m_sItemSpecEffectValue3 = dice;
	}

	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::SendDropLogMsg(int client, char * msg, short dropmode)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char cData[208], cTxtMsg[200];

	char* cp = cData;

	Push(cp, (u32)NOT_DROP_LOG);
	Push(cp, (u16)0);

	Push(cp, dropmode);
	strcpy(cTxtMsg, msg);
	Push(cp, cTxtMsg, 200);

	p->m_pXSock->iSendMsg(cData, cp - cData);
}

int cCoreServer::getItemConfigIndex(char * itemname)
{
	int result = -1;
	for (int i = 0; i < DEF_MAXITEMTYPES; i++)
	{
		if (!m_pItemConfigList[i]) continue;
		if (string(m_pItemConfigList[i]->m_cName) == itemname) result = i;
		break;
	}

	return result;
}

void cCoreServer::AddCharacterLuck(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iCharacterLuck == 100)	return;

	p->m_iCharacterLuck++;

	if (p->m_iCharacterLuck == 100)
	{
		ShowClientMsg(client, "Your luck progress is complete.");
	}
	else
	{
		ShowClientMsg(client, "Your luck progress has increased.");
	}

	NotifyCharacterLuck(client);
}

void cCoreServer::RestartCharacterLuck(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_iCharacterLuck = 0;

	ShowClientMsg(client, "Your luck progress has been reset.");

	NotifyCharacterLuck(client);
}

void cCoreServer::NotifyCharacterLuck(int client)
{
	/*auto p = m_pClientList[client];
	if (!p) return;

	char cData[10];

	char* cp = cData;

	Push(cp, (u32)NOT_CHARACTERLUCK);
	Push(cp, (u16)0);

	Push(cp, p->m_iCharacterLuck);

	p->m_pXSock->iSendMsg(cData, cp - cData);*/

}
void cCoreServer::InitializeCraftingRecipes()
{

	recipes = {

		{ "DarkExecutor", 1, 24, 0, {
			// Peces
			{ "BluefinFish", 40 },
			{ "Shark", 35 },
			// Minerales
			{ "Palladium", 40 },
			{ "Eclipsium", 35 },
			{ "Astronyx", 30 },
			// Drops de criaturas
			{ "TrollHeart", 70 },
			{ "OgreClaw", 60 },
			{ "WerewolfNail", 50 },
			// Piedras mágicas
			{ "AbyssalSapphire", 1 },
			{ "CelestialDiamond", 1 }
		} },

		{ "MagicWand(MS30-LLF)", 17, 0, 0, {
			// Peces
			{ "ClownFish", 40 },
			{ "ParrotFish", 35 },
			// Minerales
			{ "Voidsteel", 40 },
			{ "Netherite", 35 },
			{ "Orichalcum", 30 },
			// Drops de criaturas
			{ "CyclopsEye", 70 },
			{ "UnicornHorn", 60 },
			{ "WerewolfTeeth", 50 },
			// Piedras mágicas
			{ "DivineAmethyst", 1 },
			{ "LuminousQuartz", 1 }
		} }
	};
}

void cCoreServer::handleManufactureConfirm(int client, char* data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->IsShopMap())
	{
		ShowClientMsg(client, "You only perform this action on the Whouse map.");
		return;
	}

	int recipeIndex = -1;
	int itemCount = -1;
	std::vector<int> itemIndices;

	Pop(data, recipeIndex);
	Pop(data, itemCount);

	if (recipeIndex == -1 || itemCount == -1 || itemCount < 1) return;

	for (int i = 0; i < itemCount; i++) {
		int itemIndex = -1;
		Pop(data, itemIndex);
		itemIndices.push_back(itemIndex);
	}

	if (recipeIndex >= recipes.size()) {
		char errorMsg[256];
		wsprintf(errorMsg, "Invalid recipe.");
		ShowClientMsg(client, errorMsg);
		return;
	}

	const Recipe &selectedRecipe = recipes[recipeIndex];

	for (int itemIndex : itemIndices) {
		if (itemIndex < 0 || itemIndex >= DEF_MAXITEMS || !p->m_pItemList[itemIndex]) {
			char errorMsg[256];
			wsprintf(errorMsg, "Invalid item in the list.");
			ShowClientMsg(client, errorMsg);
			return;
		}
	}

	std::map<std::string, int> requiredIngredients;
	for (const auto &ingredient : selectedRecipe.ingredients) {
		requiredIngredients[ingredient.first] = ingredient.second;
	}

	for (int itemIndex : itemIndices) {
		auto item = p->m_pItemList[itemIndex];
		if (!item) continue;
		if (requiredIngredients.find(item->m_cName) != requiredIngredients.end()) {
			if (item->m_dwCount < requiredIngredients[item->m_cName]) {
				char errorMsg[256];
				wsprintf(errorMsg, "You do not have enough %s.", item->m_cName);
				ShowClientMsg(client, errorMsg);
				return;
			}
		}
		else {
			char errorMsg[256];
			wsprintf(errorMsg, "The item %s is not needed for the recipe.", item->m_cName);
			ShowClientMsg(client, errorMsg);
			return;
		}
	}

	SendCommand(client, "/disablemanudialog");

	for (int itemIndex : itemIndices) {
		auto item = p->m_pItemList[itemIndex];
		if (!item) continue;
		if (requiredIngredients.find(item->m_cName) != requiredIngredients.end()) {
			int requiredCount = requiredIngredients[item->m_cName];
			if (item->m_dwCount >= requiredCount) {
				item->m_dwCount -= requiredCount;

				if (item->m_dwCount == 0) {
					ItemDepleteHandler(client, itemIndex, FALSE);
				}
				else {
					SendCommand(client, "/newitemcount", itemIndex, item->m_dwCount);
				}
			}
		}
	}

	DWORD itemAttributes = 0;
	CreateManuItem(client, selectedRecipe.itemResult.c_str(), itemAttributes, 1);
	char successMsg[256];
	wsprintf(successMsg, "Recipe completed successfully! You have obtained: %s", selectedRecipe.itemResult.c_str());
	ShowClientMsg(client, successMsg);
}

void cCoreServer::CreateManuItem(int client, string itemname, DWORD atr, int count)
{
	auto p = m_pClientList[client];
	if (!p) return;

	class  CItem * pItem;
	char   cItemName[256];
	int    iEraseReq;

	int starterSize = 1;

	if (!HasEnoughInventorySpace(client, starterSize))
	{
		wsprintf(G_cTxt, "You need %d free space(s) to manufacture the item!", starterSize);
		ShowClientMsg(client, G_cTxt);
		return;
	}

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, (char*)itemname.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (!_bAddClientItemList(client, pItem, &iEraseReq)) return;

	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);

	char broadcastMsg[256];
	wsprintf(broadcastMsg, "%s has manufactured: %s!", p->m_cCharName, itemname.c_str());

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, broadcastMsg, drop_legendary);
		}
	}
}

void cCoreServer::CreateMineralCommand(int client, const string& mineralName, int count)
{
	if (count < 1 || count > 2000000)
	{
		wsprintf(G_cTxt, "The count must be between 1 and 2,000,000!");
		ShowClientMsg(client, G_cTxt);
		return;
	}

	const std::map<string, int> validMinerals = {
		{ "Iron", 3519 },
		{ "Copper", 3520 },
		{ "Tin", 3521 },
		{ "Bronze", 3522 },
		{ "Silver", 3523 },
		{ "Steel", 3524 },
		{ "Platinum", 3525 },
		{ "Obsidian", 3526 },
		{ "Cobalt", 3527 },
		{ "Mithril", 3528 },
		{ "Adamantite", 3529 },
		{ "Orichalcum", 3530 },
		{ "Titanium", 3531 },
		{ "Darksteel", 3532 },
		{ "Palladium", 3533 },
		{ "Aetherium", 3534 },
		{ "Dragonite", 3535 }
	};

	auto it = validMinerals.find(mineralName);
	if (it == validMinerals.end())
	{
		wsprintf(G_cTxt, "The mineral '%s' does not exist!", mineralName.c_str());
		ShowClientMsg(client, G_cTxt);
		return;
	}

	int requiredSpace = 1;
	if (!HasEnoughInventorySpace(client, requiredSpace))
	{
		wsprintf(G_cTxt, "You need %d free space(s) to create the mineral!", requiredSpace);
		ShowClientMsg(client, G_cTxt);
		return;
	}

	CreateMineral(client, mineralName, count);
}

void cCoreServer::CreateMineral(int client, const string& mineralName, int count)
{
	auto p = m_pClientList[client];
	if (!p) return;

	class CItem* pItem;
	char cItemName[256];
	int iEraseReq;

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, mineralName.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (!_bAddClientItemList(client, pItem, &iEraseReq)) return;

	pItem->m_dwCount = count;
	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


bool cCoreServer::IsHeroItem(char * itemname)
{
	return string(itemname) == "eHeroRobe(W)" ||
		string(itemname) == "eHeroHelm(W)" ||
		string(itemname) == "eHeroCap(W)" ||
		string(itemname) == "eHeroHauberk(W)" ||
		string(itemname) == "eHeroLeggings(W)" ||
		string(itemname) == "eHeroArmor(W)" ||

		string(itemname) == "aHeroRobe(W)" ||
		string(itemname) == "aHeroHelm(W)" ||
		string(itemname) == "aHeroCap(W)" ||
		string(itemname) == "aHeroHauberk(W)" ||
		string(itemname) == "aHeroLeggings(W)" ||
		string(itemname) == "aHeroArmor(W)" ||

		string(itemname) == "eHeroRobe(M)" ||
		string(itemname) == "eHeroHelm(M)" ||
		string(itemname) == "eHeroCap(M)" ||
		string(itemname) == "eHeroHauberk(M)" ||
		string(itemname) == "eHeroLeggings(M)" ||
		string(itemname) == "eHeroArmor(M)" ||

		string(itemname) == "aHeroRobe(M)" ||
		string(itemname) == "aHeroHelm(M)" ||
		string(itemname) == "aHeroCap(M)" ||
		string(itemname) == "aHeroHauberk(M)" ||
		string(itemname) == "aHeroLeggings(M)" ||
		string(itemname) == "aHeroArmor(M)";
}

void cCoreServer::RequestUpgradePowerHero(int client, char * data)
{
	int itemIndex, iValue;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, itemIndex);
	if ((itemIndex < 0) || (itemIndex >= DEF_MAXITEMS)) return;

	auto it = p->m_pItemList[itemIndex];
	if (!it) return;

	if (!IsHeroItem(it->m_cName)) return;

	if (!p->IsShopMap())
	{
		ShowClientMsg(client, "Solo puedes realizar esta mejora en el mapa Whouse.");
		return;
	}

	iValue = it->m_sItemSpecEffectValue3;
	if (iValue >= 5)
	{
		ShowClientMsg(client, "Has alcanzado el nivel máximo de mejora para este ítem.");
		return;
	}

	// --- Costos ---
	int nextLevel = iValue + 1;
	int contribCost = nextLevel * 100000;  // 100k por nivel
	int eksCost = 1000;                // fijo

	// --- Verificaciones ---
	if (p->m_iEnemyKillCount < eksCost)
	{
		ShowClientMsg(client, "Necesitas al menos 1000 EKS para realizar esta mejora.");
		return;
	}
	if (p->m_iContribution < contribCost)
	{
		char msg[128];
		wsprintf(msg, "Necesitas %d de Contribución para subir al nivel %d.", contribCost, nextLevel);
		ShowClientMsg(client, msg);
		return;
	}

	// --- Aplicar costos ---
	p->m_iEnemyKillCount -= eksCost;
	p->m_iContribution -= contribCost;

	// --- Enviar actualizaciones ---
	SendCommand(client, "/eks", p->m_iEnemyKillCount);
	SendCommand(client, "/contrib", p->m_iContribution);

	// --- Aplicar mejora ---
	it->m_sItemSpecEffectValue3++;
	SendCommand(client, "/newsv3", itemIndex, it->m_sItemSpecEffectValue3);

	// --- Mensaje de éxito ---
	char msg[256];
	wsprintf(msg, "El ítem %s se ha mejorado con éxito al nivel %d.", it->m_cName, it->m_sItemSpecEffectValue3);
	ShowClientMsg(client, msg);
}

bool cCoreServer::bCheckSODNeck(int client)
{
	auto p = m_pClientList[client];
	if (!p) return false;

	int art = -1;

	for (auto it : m_pClientList[client]->m_pItemList)
	{
		art++;

		if (!it) continue;
		if (m_pClientList[client]->m_bIsItemEquipped[art] != TRUE) continue;

		switch (it->m_sIDnum)
		{
		case 3235:
			/*case 3508: //leo
			case 3097:
			case 3550:
			case 3555:
			case 3556:*/
			return true;
			break;
		}
	}

	return false;
}

bool cCoreServer::bCheckHellShockWaveNeck(int client)
{
	auto p = m_pClientList[client];
	if (!p) return false;

	int art = -1;

	for (auto it : m_pClientList[client]->m_pItemList)
	{
		art++;

		if (!it) continue;
		if (m_pClientList[client]->m_bIsItemEquipped[art] != TRUE) continue;

		switch (it->m_sIDnum)
		{
		case 3853:
			/*case 3508: //leo
			case 3097:
			case 3550:
			case 3555:
			case 3556:*/
			return true;
			break;
		}
	}

	return false;
}

bool cCoreServer::bCheckGhostAnnihilationNeck(int client)
{
	auto p = m_pClientList[client];
	if (!p) return false;

	int art = -1;

	for (auto it : m_pClientList[client]->m_pItemList)
	{
		art++;

		if (!it) continue;
		if (m_pClientList[client]->m_bIsItemEquipped[art] != TRUE) continue;

		switch (it->m_sIDnum)
		{
		case 3854:
			/*case 3508: //leo
			case 3097:
			case 3550:
			case 3555:
			case 3556:*/
			return true;
			break;
		}
	}

	return false;
}

void cCoreServer::SetHasteFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == 0) return;
		if (bStatus)
			m_pClientList[sOwnerH]->m_iStatus |= 0x00040000;
		else m_pClientList[sOwnerH]->m_iStatus ^= 0x00040000;
		//SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

		SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
		break;

	case DEF_OWNERTYPE_NPC:
		break;
	}
}

void cCoreServer::handleFishing(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_bIsKilled) return;
	if (!p->m_bFishing) return;
	if (1 > p->m_iHP) return;
	if (!p->IsInMap("dungeon")) return;

	unsigned long currentTime = GetTickCount();

	if (currentTime - p->lastFishingAction < 6000)	return;
	p->lastFishingAction = currentTime;

	int Dado = 0;

	int less_rate = 0;
	if (p->m_bFishLuck)
	{
		less_rate = 5000;
	}

	if (g_ev.Is(EventID::BOTD))
		Dado = DropRollDice(1, FishingRateBooster - less_rate);
	else
		Dado = DropRollDice(1, FishingRate - less_rate);

	if (Dado <= 300) { //common
		char * it_name = "#";
		int it_count = -1;

		getFishingCommonDrop(it_name, it_count);

		if (string(it_name) != "#" && it_count != -1)
		{
			CreateFishingDrop(client, it_name, drop_common, it_count);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
			return;
		}
	}
	else if (Dado <= 450) { //rare
		char * it_name = "#";
		int it_count = -1;

		getFishingRareDrop(it_name, it_count);

		if (string(it_name) != "#" && it_count != -1)
		{
			CreateFishingDrop(client, it_name, drop_rare, it_count);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
			return;
		}
	}
	else if (Dado <= 525) { //epic
		char * it_name = "#";
		int it_count = -1;

		getFishingEpicDrop(it_name, it_count);

		if (string(it_name) != "#" && it_count != -1)
		{
			CreateFishingDrop(client, it_name, drop_epic, it_count);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
			return;
		}
	}
	else if (Dado <= 550) {
		char * it_name = "#";
		int it_count = -1;

		getFishinLegendDrop(it_name, it_count);

		if (string(it_name) != "#" && it_count != -1)
		{
			CreateFishingDrop(client, it_name, drop_legendary, it_count);
			SendNotifyMsg(NULL, client, CLIENT_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
			return;
		}
	}
	else { return; }
}

void cCoreServer::Request_Fishing(int client, char* data)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (p->m_bIsKilled) return;
	if (1 > p->m_iHP) return;
	if (p->m_bFishing) return;

	if (p->m_bFishing)
	{
		p->m_bFishing = false;
		ShowClientMsg(client, "Fishing cancelled.");
		send_objects_data();
		SendCommand(client, "/fishoff");
		return;
	}

	DWORD currentTime = timeGetTime();
	if (currentTime - p->lastReqFishingAction < 5000) // menos de 5 segundos
	{
		ShowClientMsg(client, "You must wait 5 seconds before fishing again.");
		return;
	}

	// Verificar si otro cliente con la misma IP ya está pescando
	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != NULL &&
			i != client && // No comparar consigo mismo
			m_pClientList[i]->m_bFishing && // Si está pescando
			strcmp(m_pClientList[i]->m_cRealIPaddress, p->m_cRealIPaddress) == 0) // Misma IP
		{
			ShowClientMsg(client, "Error: Another player with your IP is already fishing.");
			return;
		}
	}

	short sX, sY;
	Pop(data, sX);
	Pop(data, sY);

	if (!p->IsInMap("dungeon") || !IsInFishingArea(sX, sY))
	{
		ShowClientMsg(client, "Error, it is not a fishing area.");
		return;
	}

	p->m_bFishing = true;
	p->lastReqFishingAction = currentTime; // Guardar el momento del inicio
	send_objects_data();
	ShowClientMsg(client, "You are fishing.");
}
void cCoreServer::Request_CancelFishing(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_bFishing)
	{
		p->m_bFishing = false;
		ShowClientMsg(client, "Fishing cancelled.");
		send_objects_data();
		SendCommand(client, "/fishoff");
		return;
	}
}

std::map<std::string, int> cCoreServer::LoadDropSettings(const std::string& filename) {
	std::map<std::string, int> settings;
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo de configuración: " << filename << std::endl;
		return settings;
	}

	std::string line;
	while (std::getline(file, line)) {
		// Ignorar líneas vacías o comentarios
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		std::string key;
		if (std::getline(iss, key, '=')) {
			std::string valueStr;
			if (std::getline(iss, valueStr)) {
				// Remover espacios en blanco
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				valueStr.erase(0, valueStr.find_first_not_of(" \t"));
				valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

				int value = std::stoi(valueStr);
				settings[key] = value;
			}
		}
	}

	file.close();
	return settings;
}

void cCoreServer::GetDropSettings()
{
	std::map<std::string, int> dropSettings = LoadDropSettings("Configs\\DropSettings.cfg");

	DropRateNormal = dropSettings["DropRateNormal"];
	DropRateMadness = dropSettings["DropRateMadness"];
	DropRateBooster = dropSettings["DropRateBooster"];
	MineRateNormal = dropSettings["MineRateNormal"];
	MineRateRush = dropSettings["MineRateRush"];
	MineLegendChanceDice = dropSettings["MineLegendChanceDice"];
	MineLegendChanceResult = dropSettings["MineLegendChanceResult"];
	FishingRate = dropSettings["FishingRate"];
	FishingRateBooster = dropSettings["FishingRateBooster"];


	/*std::cout << "" << std::endl;
	std::cout << "Getting drop rates." << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "DropRateNormal: " << DropRateNormal << std::endl;
	std::cout << "DropRateMadness: " << DropRateMadness << std::endl;
	std::cout << "DropRateBooster: " << DropRateBooster << std::endl;
	std::cout << "MineRateNormal: " << MineRateNormal << std::endl;
	std::cout << "MineRateRush: " << MineRateRush << std::endl;
	std::cout << "MineLegendChanceDice: " << MineLegendChanceDice << std::endl;
	std::cout << "MineLegendChanceResult: " << MineLegendChanceResult << std::endl;
	std::cout << "FishingRate: " << FishingRate << std::endl;
	std::cout << "FishingRateBooster: " << FishingRateBooster << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "" << std::endl;*/
}

void cCoreServer::reloadItemsConfig()
{

	std::cout << "Reload item config." << std::endl;

	for (int i = 0; i < DEF_MAXITEMTYPES; i++)
	{
		m_pItemConfigList[i] = NULL;
		delete m_pItemConfigList[i];
	}

	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Item.cfg")) ErrorList("(!!!) STOPPED! Item configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Item2.cfg")) ErrorList("(!!!) STOPPED! Item2 configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Item3.cfg")) ErrorList("(!!!) STOPPED! Item3 configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Fishes.cfg")) ErrorList("(!!!) STOPPED! Fishes configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Minerals.cfg")) ErrorList("(!!!) STOPPED! Minerals configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\SpecialItems.cfg")) ErrorList("(!!!) STOPPED! SpecialItems configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Weapons.cfg")) ErrorList("(!!!) STOPPED! Weapons configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Jewelry.cfg")) ErrorList("(!!!) STOPPED! Jewelry configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Fragments.cfg")) ErrorList("(!!!) STOPPED! Fragments configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Segments.cfg")) ErrorList("(!!!) STOPPED! Fragments configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Clothes.cfg")) ErrorList("(!!!) STOPPED! Clothes configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Balls.cfg")) ErrorList("(!!!) STOPPED! Balls configuration error.");
	if (!_bDecodeItemConfigFileContents("Configs\\Items\\Stones.cfg")) ErrorList("(!!!) STOPPED! Stones configuration error.");
}

void cCoreServer::reloadMagics()
{

	std::cout << "Reload magics config." << std::endl;

	for (int i = 0; i < DEF_MAXITEMTYPES; i++)
	{
		m_pItemConfigList[i] = NULL;
		delete m_pItemConfigList[i];
	}



	for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
	{
		m_pMagicConfigList[i] = NULL;
		delete m_pMagicConfigList[i];
	}

	if (!_bDecodeMagicConfigFileContents("Configs\\Magic.cfg")) ErrorList("(!!!) STOPPED! MAGIC configuration error.");
}

void cCoreServer::ShowPlayerStats(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	wsprintf(G_cTxt, "Pa: %d, Ma: %d, Mr: %d, Dr: %d, Phys Dmg: %d, Magic dmg: %d",
		p->m_iAddAbsPD,
		p->m_iAddAbsMD,
		p->m_iAddMR / 7,
		p->m_iAddDR / 10,
		p->m_iAddPhysicalDamage,
		p->m_iAddMagicalDamage);

	ShowClientMsg(client, G_cTxt);
}


void cCoreServer::ShowPlayerSegments(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	wsprintf(G_cTxt, "Ice: %d, Lighting: %d, Fire: %d", p->m_iIceSegments, p->m_iLightingSegments, p->m_iFireSegments);

	ShowClientMsg(client, G_cTxt);
}


void cCoreServer::Request_WriteStatus(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	short status;
	Pop(data, status);

	if (status == 1) p->m_sWriteStatus = 1;
	else p->m_sWriteStatus = 0;

	send_objects_data();
}

void cCoreServer::SetWriteStatus(int client, bool status)
{
	auto p = m_pClientList[client];
	if (!p) return;

	m_pClientList[client]->m_iStatus = m_pClientList[client]->m_iStatus & 0xFFFFFFF7;

	SendEventToNearClient_TypeA(client, DEF_OWNERTYPE_PLAYER, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
}

void cCoreServer::NotifyAddedStats(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char cData[50];
	char* cp = cData;

	Push(cp, (u32)NOTIFY_ADDED_STATS);
	Push(cp, (u16)0);

	Push(cp, p->m_iAddedPA);
	Push(cp, p->m_iAddedMA);
	Push(cp, p->m_iAddedMR);
	Push(cp, p->m_iAddedDR);
	Push(cp, p->m_iAddedPhysDmg);
	Push(cp, p->m_iAddedPhysHit);
	Push(cp, p->m_iAddedMagDmg);
	Push(cp, p->m_iAddedMagHit);
	Push(cp, p->m_iAddedMS);

	p->m_pXSock->iSendMsg(cData, cp - cData);
}

void cCoreServer::HandleUpgradeStats(int client, char* data) {
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->IsShopMap())
	{
		ShowClientMsg(client, "You only perform this action on the Whouse map.");
		return;
	}

	// Verificación de recursos negativos
	if (p->m_iTokens < 0) p->m_iTokens = 0;
	if (p->m_iCoins < 0) p->m_iCoins = 0;

	char statIncrements[STAT_COUNT] = { 0 };
	auto* cp = data;

	for (int i = 0; i < STAT_COUNT; i++) {
		Pop(cp, statIncrements[i]);
		if (statIncrements[i] < 0) {
			statIncrements[i] = 0; // Ignorar valores negativos
		}
	}

	int totalTokenCost = 0;
	int totalCoinCost = 0;
	bool hasAnyUpgrade = false;
	bool someStatCapped = false;

	// Primera pasada: validaciones y cálculos
	for (int i = 0; i < STAT_COUNT; i++) {
		int currentLevel = p->GetCurrentStatLevel(i);
		int increment = statIncrements[i];

		if (increment <= 0) continue;

		// Verificar límite máximo
		if (currentLevel + increment > statConfigs[i].maxLevel) {
			// Ajustar el incremento al máximo posible
			int possibleIncrement = statConfigs[i].maxLevel - currentLevel;
			if (possibleIncrement > 0) {
				statIncrements[i] = possibleIncrement;
				increment = possibleIncrement;
				someStatCapped = true;
			}
			else {
				statIncrements[i] = 0;
				continue;
			}
		}

		hasAnyUpgrade = true;

		// Calcular costos
		for (int j = 0; j < increment; j++) {
			int targetLevel = currentLevel + j + 1;
			totalTokenCost += statConfigs[i].tokenCost * targetLevel;
			totalCoinCost += statConfigs[i].coinCost * targetLevel;

			if (totalTokenCost < 0 || totalCoinCost < 0) {
				ShowClientMsg(client, "Upgrade cost calculation error");
				return;
			}
		}
	}

	if (!hasAnyUpgrade) {
		ShowClientMsg(client, "No valid stat upgrades available");
		return;
	}

	// Verificación de recursos
	if (p->m_iTokens < totalTokenCost || p->m_iCoins < totalCoinCost) {
		ShowClientMsg(client, "Not enough resources for upgrades");
		return;
	}

	// Segunda pasada: aplicar mejoras
	for (int i = 0; i < STAT_COUNT; i++) {
		if (statIncrements[i] <= 0) continue;

		switch (i) {
		case STAT_PA: p->m_iAddedPA += statIncrements[i]; break;
		case STAT_MA: p->m_iAddedMA += statIncrements[i]; break;
		case STAT_MR: p->m_iAddedMR += statIncrements[i]; break;
		case STAT_DR: p->m_iAddedDR += statIncrements[i]; break;
		case STAT_PHYS_DMG: p->m_iAddedPhysDmg += statIncrements[i]; break;
		case STAT_PHYS_HIT: p->m_iAddedPhysHit += statIncrements[i]; break;
		case STAT_MAG_DMG: p->m_iAddedMagDmg += statIncrements[i]; break;
		case STAT_MAG_HIT: p->m_iAddedMagHit += statIncrements[i]; break;
		case STAT_MS: p->m_iAddedMS += statIncrements[i]; break;
		}
	}

	// Descontar recursos
	p->m_iTokens -= totalTokenCost;
	p->m_iCoins -= totalCoinCost;

	// Mensaje final
	if (someStatCapped) {
		ShowClientMsg(client, "Stats upgraded (some were capped at max level)!");
	}
	else {
		ShowClientMsg(client, "Stats upgraded successfully!");
	}

	SendF5DataCharacter(client);
	NotifyAddedStats(client);
	CalcTotalItemEffect(client, -1, TRUE);
}

const char* cCoreServer::GetStatName(int statIndex) {
	static const char* statNames[] = {
		"PA", "MA", "MR", "DR",
		"PhysDmg", "PhysHit", "MagDmg", "MagHit",
		"MS"
	};

	if (statIndex < 0 || statIndex >= STAT_COUNT) {
		/*Log("Invalid stat index requested: %d", statIndex);*/
		return "Invalid";
	}

	return statNames[statIndex];
}

void cCoreServer::SpecialChatacterReqVit(int client)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	unsigned long currentTime = GetTickCount();
	if (currentTime - p->lastVipReqVit < 500 _ms)	return;

	p->lastVipReqVit = currentTime;

	if (p->m_bIsKilled) return;
	if (p->m_iHP <= 0) return;

	int addhp = 0;
	int dice = RollDice(1, 5);
	switch (dice)
	{
	case 1:
		addhp = 300;
		break;
	case 2:
		addhp = 260;
		break;
	case 3:
		addhp = 280;
		break;
	case 4:
		addhp = 270;
		break;
	case 5:
		addhp = 290;
		break;
	default:
		break;
	}

	int newhp = p->m_iHP + addhp;
	if (newhp > iGetMaxHP(client))
		newhp = iGetMaxHP(client);

	p->m_iHP = newhp;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
}

void cCoreServer::handleRepairPowerItems(int client)
{
	unsigned long currentTime = GetTickCount();
	auto p = m_pClientList[client];
	if (!p) return;
	if (!p->m_bIsInitComplete) return;
	if (p->m_iRegenEndu != 1) return;
	if (currentTime - p->lastRegenEndurance < p->m_iRegenEnduTime _s) return;
	p->lastRegenEndurance = currentTime;
	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		auto item = p->m_pItemList[i];
		if (!item) continue;
		if (!IsPowerHeroItem(item->m_cName)) continue;
		if (item->m_wCurLifeSpan < item->m_wMaxLifeSpan) RepairPowerHero(client, i);
	}
}

void cCoreServer::RepairPowerHero(int client, int itindex)
{
	auto p = m_pClientList[client];
	if (!p) return;
	auto item = p->m_pItemList[itindex];
	if (!item) return;
	item->m_wCurLifeSpan = item->m_wMaxLifeSpan;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_ITEMREPAIRED2, itindex, item->m_wCurLifeSpan, NULL, NULL);
}
void cCoreServer::DeleteBankItem(int client, int itemindex)
{
	auto p = m_pClientList[client];
	if (!p) return;

	auto item = p->m_pItemInBankList[itemindex];
	if (!item) return;

	delete p->m_pItemInBankList[itemindex];
	p->m_pItemInBankList[itemindex] = NULL;

	char data[10];
	auto* cp = data;

	Push(cp, (u32)NOT_DELETEBANKITEM);
	Push(cp, (u16)0);

	Push(cp, itemindex);

	if (m_pClientList[client]->m_pXSock) m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::DeleteMarketItem(int client, int itemindex)
{
	auto p = m_pClientList[client];
	if (!p) return;

	auto item = p->m_pMarketItem[itemindex];
	if (!item) return;

	delete p->m_pMarketItem[itemindex];
	p->m_pMarketItem[itemindex] = NULL;
}

void cCoreServer::handleRefactorServer(int client)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	if (HasProcessedPlayer(p->m_cCharName)) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		auto& destItem = p->m_pItemList[i];
		if (!destItem) continue;

		short type1 = (destItem->m_dwAttribute & 0x00F00000) >> 20;
		short val1 = (destItem->m_dwAttribute & 0x000F0000) >> 16;
		short type2 = (destItem->m_dwAttribute & 0x0000F000) >> 12;
		short val2 = (destItem->m_dwAttribute & 0x00000F00) >> 8;
		DWORD currentLevel = (destItem->m_dwAttribute & 0xF0000000) >> 28;

		bool bChange = false;

		// Resetear type1 y val1 (bits 16-23)
		if (type1 > 0) {
			bChange = true;
			type1 = 0;
			val1 = 0;

			destItem->m_dwAttribute &= ~0x00F00000; // Limpia type1 (bits 20-23)
			destItem->m_dwAttribute &= ~0x000F0000; // Limpia val1 (bits 16-19)
		}

		// Resetear type2 y val2 (bits 8-15)
		if (type2 > 0) {
			bChange = true;
			type2 = 0;
			val2 = 0;

			destItem->m_dwAttribute &= ~0x0000F000; // Limpia type2 (bits 12-15)
			destItem->m_dwAttribute &= ~0x00000F00; // Limpia val2 (bits 8-11)
		}

		if (!destItem->IsAngelic())
		{
			if (currentLevel > 0) {
				bChange = true;
				destItem->m_dwAttribute &= ~0xF0000000;
			}
		}

		if (bChange)
		{
			NotifyItAtr(client, i, destItem->m_dwAttribute);
			ReqRepairItemCofirmHandler2(client, i, NULL);
		}
	}

	for (int i = 0; i < DEF_MAXBANKITEMS; i++)
	{
		auto& destItem = p->m_pItemInBankList[i];
		if (!destItem) continue;

		short type1 = (destItem->m_dwAttribute & 0x00F00000) >> 20;
		short val1 = (destItem->m_dwAttribute & 0x000F0000) >> 16;
		short type2 = (destItem->m_dwAttribute & 0x0000F000) >> 12;
		short val2 = (destItem->m_dwAttribute & 0x00000F00) >> 8;
		DWORD currentLevel = (destItem->m_dwAttribute & 0xF0000000) >> 28;

		bool bChange = false;

		// Resetear type1 y val1 (bits 16-23)
		if (type1 > 0) {
			bChange = true;
			type1 = 0;
			val1 = 0;

			destItem->m_dwAttribute &= ~0x00F00000; // Limpia type1 (bits 20-23)
			destItem->m_dwAttribute &= ~0x000F0000; // Limpia val1 (bits 16-19)
		}

		// Resetear type2 y val2 (bits 8-15)
		if (type2 > 0) {
			bChange = true;
			type2 = 0;
			val2 = 0;

			destItem->m_dwAttribute &= ~0x0000F000; // Limpia type2 (bits 12-15)
			destItem->m_dwAttribute &= ~0x00000F00; // Limpia val2 (bits 8-11)
		}

		if (!destItem->IsAngelic())
		{
			if (currentLevel > 0) {
				bChange = true;
				destItem->m_dwAttribute &= ~0xF0000000;
			}
		}

		if (bChange) NotifyBankItAtr(client, i, destItem->m_dwAttribute);
	}

	for (int i = 0; i < DEF_MAXMARKETITEMS; i++)
	{
		auto& destItem = p->m_pMarketItem[i];
		if (!destItem) continue;

		short type1 = (destItem->m_dwAttribute & 0x00F00000) >> 20;
		short val1 = (destItem->m_dwAttribute & 0x000F0000) >> 16;
		short type2 = (destItem->m_dwAttribute & 0x0000F000) >> 12;
		short val2 = (destItem->m_dwAttribute & 0x00000F00) >> 8;
		DWORD currentLevel = (destItem->m_dwAttribute & 0xF0000000) >> 28;

		bool bChange = false;

		// Resetear type1 y val1 (bits 16-23)
		if (type1 > 0) {
			bChange = true;
			type1 = 0;
			val1 = 0;

			destItem->m_dwAttribute &= ~0x00F00000; // Limpia type1 (bits 20-23)
			destItem->m_dwAttribute &= ~0x000F0000; // Limpia val1 (bits 16-19)
		}

		// Resetear type2 y val2 (bits 8-15)
		if (type2 > 0) {
			bChange = true;
			type2 = 0;
			val2 = 0;

			destItem->m_dwAttribute &= ~0x0000F000; // Limpia type2 (bits 12-15)
			destItem->m_dwAttribute &= ~0x00000F00; // Limpia val2 (bits 8-11)
		}

		if (!destItem->IsAngelic())
		{
			if (currentLevel > 0) {
				bChange = true;
				destItem->m_dwAttribute &= ~0xF0000000;
			}
		}
	}

	p->m_sCurrentMastery = 0;
	p->m_sNpcDamageLevel = 0;
	p->m_sRegenLevel = 0;
	p->m_sVitLevel = 0;
	p->m_sDefenseLevel = 0;

	notifyMasteryData(client);

	AddPlayerToProcessedPlayer(p->m_cCharName);
}

void cCoreServer::NotifyItAtr(int client, int itindex, DWORD atr)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[20];
	auto* cp = data;

	Push(cp, (u32)NOT_NEWATR);
	Push(cp, (u16)0);

	Push(cp, itindex);
	Push(cp, atr);

	if (m_pClientList[client]->m_pXSock) m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::NotifyBankItAtr(int client, int itindex, DWORD atr)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[20];
	auto* cp = data;

	Push(cp, (u32)NOT_NEWBANKATR);
	Push(cp, (u16)0);

	Push(cp, itindex);
	Push(cp, atr);

	if (m_pClientList[client]->m_pXSock) m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}


void cCoreServer::ResetRep(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iContribution < 10000)
	{
		ShowClientMsg(client, "Necesitas 10000 de contrib para resetear la rep");
		return;
	}

	p->m_iContribution -= 10000;
	SendCommand(client, "/contrib", p->m_iContribution);

	p->m_iRating = 0;
	SendCommand(client, "/rep", p->m_iRating);

	ShowClientMsg(client, "Tu reputacion se reinicio con exito.");
}

bool cCoreServer::isPlayerOnline(char * cAcc, char* cName)
{
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		if (!m_pClientList[i]) continue;

		if (string(m_pClientList[i]->m_cAccountName) == cAcc ||
			string(m_pClientList[i]->m_cCharName) == cName) {
			return true;
		}
	}

	return false;
}

void cCoreServer::RequestCreateNewGuild(int iClientH) {
	auto p = m_pClientList[iClientH];
	if (!p) return;

	char cFileName[255], cData[100];
	char cTxt[500], cTxt2[500];
	char cGuildMasterName[11], cGuildLocation[11], cGuildName[21];
	char *cp;
	DWORD dwGuildGUID;
	FILE *pFile;
	SYSTEMTIME SysTime;
	int iRet;

	if (m_pClientList[iClientH] == NULL) return;

	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTxt2, sizeof(cTxt2));
	ZeroMemory(cGuildMasterName, sizeof(cGuildMasterName));
	ZeroMemory(cGuildName, sizeof(cGuildName));
	ZeroMemory(cGuildLocation, sizeof(cGuildLocation));

	memcpy_secure(cGuildMasterName, m_pClientList[iClientH]->m_cCharName, 10);
	memcpy_secure(cGuildLocation, m_pClientList[iClientH]->m_cLocation, 10);
	memcpy_secure(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);
	dwGuildGUID = m_pClientList[iClientH]->m_iGuildGUID;

	if (strcmp(cGuildName, "NONE") == 0) {
		SendGuildCreateResponse(iClientH, FALSE);
		return;
	}

	// Construcción de ruta
	sprintf_s(cFileName, "Database\\Guild\\AscII%d\\%s.txt", *cGuildName, cGuildName);
	_mkdir("Guild");

	char cDir[255];
	sprintf_s(cDir, "Database\\Guild\\AscII%d", *cGuildName);
	_mkdir(cDir);

	pFile = fopen(cFileName, "rt");
	if (pFile != NULL) {
		fclose(pFile);
		wsprintf(cTxt2, "(X) Cannot create new guild - Already existing guild name: Name(%s)", cFileName);
		PutLogList(cTxt2);
		SendGuildCreateResponse(iClientH, FALSE);
		return;
	}

	pFile = fopen(cFileName, "wt");
	if (pFile == NULL) {
		wsprintf(cTxt2, "(X) Cannot create new guild - cannot create file: Name(%s)", cFileName);
		PutLogList(cTxt2);
		SendGuildCreateResponse(iClientH, FALSE);
		return;
	}

	wsprintf(cTxt2, "(O) New guild created: Name(%s)", cFileName);
	PutLogList(cTxt2);

	GetLocalTime(&SysTime);
	fprintf(pFile, ";Guild file - Updated %4d/%02d/%02d %02d:%02d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	fprintf(pFile, "[GUILD-INFO]\n\n");
	fprintf(pFile, "guildmaster-name     = %s\n", cGuildMasterName);
	fprintf(pFile, "guild-GUID           = %d\n", dwGuildGUID);
	fprintf(pFile, "guild-location       = %s\n\n", cGuildLocation);
	fprintf(pFile, "[GUILDSMAN]\n");

	fclose(pFile);

	m_pClientList[iClientH]->m_iGuildRank = 0;

	wsprintf(cTxt2, "New guild(%s) creation success! : character(%s)", cGuildName, cGuildMasterName);
	CharacterLogList(cTxt2);
	CheckCompleteAchivement(iClientH, 7, TRUE); // si no lo querés, podés omitirlo

	SendGuildCreateResponse(iClientH, TRUE);
}

void cCoreServer::SendGuildCreateResponse(int iClientH, bool bSuccess) {
	auto p = m_pClientList[iClientH];
	if (!p) return;

	char cData[100];
	WORD *wp;
	DWORD *dwp;

	dwp = (DWORD *)cData;
	*dwp = CLIENT_RESPONSE_CREATENEWGUILD;

	wp = (WORD *)(cData + DEF_INDEX2_MSGTYPE);
	*wp = bSuccess ? DEF_MSGTYPE_CONFIRM : DEF_MSGTYPE_REJECT;

	int iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	if (iRet <= 0) {
		DeleteClient(iClientH, TRUE, TRUE);
	}

	if (!p) return;

	if (!bSuccess) {
		ZeroMemory(m_pClientList[iClientH]->m_cGuildName, sizeof(m_pClientList[iClientH]->m_cGuildName));
		memcpy_secure(m_pClientList[iClientH]->m_cGuildName, "NONE", 4);
		m_pClientList[iClientH]->m_iGuildRank = -1;
		m_pClientList[iClientH]->m_iGuildGUID = -1;
		SetItemCount(iClientH, "Gold", dwGetItemCount(iClientH, "Gold") + dwCreateGuildGoldCost);
		iCalcTotalWeight(iClientH);
	}
}

void cCoreServer::RequestDisbandGuild(int iClientH)
{
	char cFileName[255], cDir[100], cTxt[120];
	char cGuildName[21], cGuildMasterName[11];
	FILE* pFile;

	if (m_pClientList[iClientH] == NULL) return;

	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cGuildName, sizeof(cGuildName));
	ZeroMemory(cGuildMasterName, sizeof(cGuildMasterName));

	memcpy_secure(cGuildMasterName, m_pClientList[iClientH]->m_cCharName, 10);
	memcpy_secure(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

	// Ruta al archivo de guild
	sprintf_s(cFileName, "Database\\Guild\\AscII%d\\%s.txt", *cGuildName, cGuildName);
	sprintf_s(cDir, "Database\\Guild\\AscII%d", *cGuildName);

	pFile = fopen(cFileName, "rt");
	if (pFile != NULL) {
		fclose(pFile);
		wsprintf(cTxt, "(O) Disband Guild - Deleting guild file... : Name(%s)", cFileName);
		PutLogList(cTxt);

		if (DeleteFile(cFileName)) {
			// ✅ Confirmación exitosa
			wsprintf(cTxt, "Disband guild(%s) success! : character(%s)", cGuildName, cGuildMasterName);
			CharacterLogList(cTxt);

			SendGuildMsg(iClientH, CLIENT_RESPONSE_GUILDDISBANDED, NULL, NULL, NULL);
			SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_CLEARGUILDNAME,
				m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX,
				m_pClientList[iClientH]->m_sY, 0, 0, 0);

			ZeroMemory(m_pClientList[iClientH]->m_cGuildName, sizeof(m_pClientList[iClientH]->m_cGuildName));
			memcpy_secure(m_pClientList[iClientH]->m_cGuildName, "NONE", 4);
			m_pClientList[iClientH]->m_iGuildRank = -1;
			m_pClientList[iClientH]->m_iGuildGUID = -1;

			SendDisbandResponse(iClientH, true);
		}
		else {
			// ❌ No se pudo borrar el archivo
			wsprintf(cTxt, "Disband guild(%s) delete failed! : character(%s)", cGuildName, cGuildMasterName);
			CharacterLogList(cTxt);
			SendDisbandResponse(iClientH, false);
		}
	}
	else {
		// ❌ Archivo no existe
		wsprintf(cTxt, "Disband guild failed - Guild file not found: %s", cFileName);
		PutLogList(cTxt);
		SendDisbandResponse(iClientH, false);
	}
}

void cCoreServer::SendDisbandResponse(int iClientH, bool bSuccess)
{
	char cData[100];
	DWORD* dwp;
	WORD* wp;

	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = CLIENT_RESPONSE_DISBANDGUILD;

	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = bSuccess ? DEF_MSGTYPE_CONFIRM : DEF_MSGTYPE_REJECT;

	int iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	if (iRet <= 0) {
		DeleteClient(iClientH, TRUE, TRUE);
	}
}

void cCoreServer::AddNewGuildsmanToFile(int iClientH)
{
	char cFileName[255], cDir[255], cTxt[100], cLog[100];
	char cGuildName[21], cGuildsmanName[11];
	FILE* pFile;

	if (m_pClientList[iClientH] == NULL) return;

	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cLog, sizeof(cLog));
	ZeroMemory(cGuildName, sizeof(cGuildName));
	ZeroMemory(cGuildsmanName, sizeof(cGuildsmanName));

	memcpy_secure(cGuildsmanName, m_pClientList[iClientH]->m_cCharName, 10);
	memcpy_secure(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

	sprintf_s(cFileName, "Database\\Guild\\AscII%d\\%s.txt", *cGuildName, cGuildName);
	sprintf_s(cDir, "Database\\Guild\\AscII%d", *cGuildName);

	_mkdir("Guild");
	_mkdir(cDir);

	pFile = fopen(cFileName, "at");
	if (pFile != NULL) {
		sprintf_s(cTxt, "guildsman-name       = %s\n", cGuildsmanName);
		fwrite(cTxt, 1, strlen(cTxt), pFile);
		fclose(pFile);

		sprintf_s(cLog, "(O) New Guildsman Add: %s, In Guild: %s", cGuildsmanName, cGuildName);
		PutLogList(cLog);
	}
	else {
		sprintf_s(cLog, "(X) Cannot Add Guildsman, Guild: %s Not Found", cGuildName);
		PutLogList(cLog);
	}
}

void cCoreServer::RemoveGuildsmanFromFile(int iClientH)
{
	char cFileName[255], cDir[255], cLine[100], cGuildName[21], cGuildsmanName[11];
	char cLog[500], cBuffer[5000] = { 0 };
	FILE* pFile;
	bool bFound = false;

	if (m_pClientList[iClientH] == NULL) return;

	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));
	ZeroMemory(cLine, sizeof(cLine));
	ZeroMemory(cGuildName, sizeof(cGuildName));
	ZeroMemory(cGuildsmanName, sizeof(cGuildsmanName));
	ZeroMemory(cLog, sizeof(cLog));

	memcpy_secure(cGuildsmanName, m_pClientList[iClientH]->m_cCharName, 10);
	memcpy_secure(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

	sprintf_s(cFileName, "Database\\Guild\\AscII%d\\%s.txt", *cGuildName, cGuildName);
	sprintf_s(cDir, "Database\\Guild\\AscII%d", *cGuildName);

	_mkdir("Guild");
	_mkdir(cDir);

	pFile = fopen(cFileName, "rt");
	if (pFile != NULL) {
		sprintf_s(cLine, "guildsman-name       = %s\n", cGuildsmanName);

		char tmpLine[100];
		while (fgets(tmpLine, sizeof(tmpLine), pFile)) {
			if (strcmp(tmpLine, cLine) != 0) {
				strcat_s(cBuffer, tmpLine);
			}
			else {
				bFound = true;
			}
		}
		fclose(pFile);

		if (!bFound) {
			sprintf_s(cLog, "(X) Guildsman Not Found: %s, In Guild: %s", cGuildsmanName, cGuildName);
			PutLogList(cLog);
		}
		else {
			SaveInfo(cFileName, cBuffer, 1); // sobrescribe el archivo
			sprintf_s(cLog, "(O) Guildsman: %s, Removed of Guild: %s", cGuildsmanName, cGuildName);
			PutLogList(cLog);
		}
	}
	else {
		sprintf_s(cLog, "(X) Cannot Remove Guildsman, Guild: %s Not Found", cGuildName);
		PutLogList(cLog);
	}
}

void cCoreServer::SaveInfo(char cFileName[255], char *pData, DWORD dwStartSize)
{
#ifdef DEF_DEBUG
	try {
#endif
		FILE * pFile;

		pFile = fopen(cFileName, "wt");
		if (pFile != NULL) {
			if (strlen(pData) > 0) fwrite(pData, dwStartSize, strlen(pData), pFile);
			fclose(pFile);
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		//ErrorLogList("Crash Evitado en: SaveInfo");
	}
#endif
}
void cCoreServer::CheckGuildIntegrity(int iClientH)
{
	auto p = m_pClientList[iClientH];
	if (!p) return;

	if (memcmp(p->m_cGuildName, "NONE", 4) == 0) return;
	if (p->m_iGuildRank == 0) return;

	char cFileName[255], cLine[100], cGuildName[21], cCharName[11];
	FILE* pFile;
	bool bFound = false;

	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cLine, sizeof(cLine));
	ZeroMemory(cGuildName, sizeof(cGuildName));
	ZeroMemory(cCharName, sizeof(cCharName));

	memcpy_secure(cGuildName, p->m_cGuildName, 20);
	memcpy_secure(cCharName, p->m_cCharName, 10);

	sprintf_s(cFileName, "Database\\Guild\\AscII%d\\%s.txt", *cGuildName, cGuildName);

	pFile = fopen(cFileName, "rt");
	if (!pFile) {
		// Archivo no existe → eliminar datos de guild
		goto REMOVE_GUILD_DATA;
	}

	sprintf_s(cLine, "guildsman-name       = %s\n", cCharName);
	char tmpLine[100];

	while (fgets(tmpLine, sizeof(tmpLine), pFile)) {
		if (strcmp(tmpLine, cLine) == 0) {
			bFound = true;
			break;
		}
	}
	fclose(pFile);

	if (bFound) return;

REMOVE_GUILD_DATA:
	ZeroMemory(p->m_cGuildName, sizeof(p->m_cGuildName));
	strcpy_secure(p->m_cGuildName, "NONE");
	p->m_iGuildRank = -1;
	p->m_iGuildGUID = -1;

	SendNotifyMsg(NULL, iClientH, CLIENT_RESPONSE_GUILDDISBANDED, NULL, NULL, NULL, p->m_cGuildName);
}

void cCoreServer::testserverping(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[6];
	char* cp = data;

	Push(cp, (u32)MSG_TESTPING);
	Push(cp, (u16)0);

	m_pClientList[client]->m_pXSock->iSendMsg(data, sizeof(data));
}

void cCoreServer::requestStartEkQuest(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int quest_id = -1;
	Pop(data, quest_id);

	start_ek_quest(client, quest_id);
}

void cCoreServer::requestEnableCharEkQuest(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int quest_id = -1;
	Pop(data, quest_id);

	EnableEkQuestStatus(client, quest_id);
}

void cCoreServer::requestCharEkQuestReward(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->IsInMap("dk") || p->IsInMap("gg") || p->IsInMap("dm")) return;

	int quest_id = -1;
	Pop(data, quest_id);

	HandleReqCharQuestReward(client, quest_id);
}

void cCoreServer::HandleReqCharQuestReward(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->vec_charekquest.empty()) return;

	int quest_index = -1;
	for (size_t i = 0; i < p->vec_charekquest.size(); i++)
	{
		if (p->vec_charekquest[i].m_iParentQuestID == quest_id)
		{
			quest_index = i;
			break;
		}
	}

	if (quest_index != -1)
	{
		HandleCharQuestReward(client, quest_index);
	}
}

bool cCoreServer::strStartsWith(const std::string& str, const std::string& prefix) {
	return str.size() >= prefix.size() &&
		str.compare(0, prefix.size(), prefix) == 0;
}

void cCoreServer::HandleCharQuestReward(int client, int quest_index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	const char* reward = p->vec_charekquest[quest_index].m_cRewardName;
	char msg[200];

	if (p->vec_charekquest[quest_index].m_iKills < p->vec_charekquest[quest_index].m_iMaxKills) return;

	if (strStartsWith(reward, "PaFrags+")) {
		int cantidad = atoi(reward + strlen("PaFrags+"));
		p->fragment_pa += cantidad;
		SendCommand(client, "/fragment_pa", p->fragment_pa);
		wsprintf(msg, "Has recibido %d Fragmentos Pa.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	else if (strStartsWith(reward, "MaFrags+")) {
		int cantidad = atoi(reward + strlen("MaFrags+"));
		p->fragment_ma += cantidad;
		SendCommand(client, "/fragment_ma", p->fragment_ma);
		wsprintf(msg, "Has recibido %d Fragmentos Ma.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	else if (strStartsWith(reward, "MrFrags+")) {
		int cantidad = atoi(reward + strlen("MrFrags+"));
		p->fragment_mr += cantidad;
		SendCommand(client, "/fragment_mr", p->fragment_mr);
		wsprintf(msg, "Has recibido %d Fragmentos Mr.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	else if (strStartsWith(reward, "DrFrags+")) {
		int cantidad = atoi(reward + strlen("DrFrags+"));
		p->fragment_dr += cantidad;
		SendCommand(client, "/fragment_dr", p->fragment_dr);
		wsprintf(msg, "Has recibido %d Fragmentos Dr.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	else if (strStartsWith(reward, "HitPFrags+")) {
		int cantidad = atoi(reward + strlen("HitPFrags+"));
		p->fragment_hp += cantidad;
		SendCommand(client, "/fragment_hp", p->fragment_hp);
		wsprintf(msg, "Has recibido %d Fragmentos Hit Prob.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	/*else if (strStartsWith(reward, "Contribution+")) {
		int cantidad = atoi(reward + strlen("Contribution+"));
		p->m_iContribution += cantidad;
		SendCommand(client, "/contrib", p->m_iContribution);
		wsprintf(msg, "Has recibido %d de contribucion.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}
	else if (strStartsWith(reward, "Tokens+")) {
		int cantidad = atoi(reward + strlen("Tokens+"));
		p->m_iTokens += cantidad;
		SendCommand(client, "/tokens", p->m_iTokens);
		wsprintf(msg, "Has recibido %d tokens.", cantidad);
		ShowClientMsg(client, msg);
		DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
	}*/
	else {
		if (CreateCharQuestItemReward(client, reward)) {
			wsprintf(msg, "Has recibido el item %s.", reward);
			ShowClientMsg(client, msg);
			DeleteCompletedEkQuest(client, p->vec_charekquest[quest_index].m_iParentQuestID);
		}
	}
}

bool cCoreServer::CreateCharQuestItemReward(int client, string itemname)
{
	auto p = m_pClientList[client];
	if (!p) return false;

	class  CItem * pItem;
	char   cItemName[256];
	int    iEraseReq;

	if (!HasEnoughInventorySpace(client, 1))
	{
		wsprintf(G_cTxt, "Necesitas %d espacio libre para extraer el item.", 1);
		ShowClientMsg(client, G_cTxt);
		return false;
	}

	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, (char*)itemname.c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return false;
	}

	if (!_bAddClientItemList(client, pItem, &iEraseReq)) return false;

	pItem->m_dwAttribute = 0;
	pItem->m_dwCount = 1;

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
	return true;
}

void cCoreServer::requestDisableCharEkQuest(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int quest_id = -1;
	Pop(data, quest_id);

	DisableEkQuestStatus(client, quest_id);
}

void cCoreServer::requestDeleteCharEkQuest(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int quest_id = -1;
	Pop(data, quest_id);

	DeleteEkQuest(client, quest_id);
}

void cCoreServer::save_quest_data(int client)
{
	if (client < 0 || client >= DEF_MAXCLIENTS) return;

	auto p = m_pClientList[client];
	if (!p || p->m_cCharName[0] == 0) return;

	char cFileName[256] = {};
	char cDir[64] = {};

	sprintf_s(cFileName, "Database\\Quest\\");
	_mkdir("Database\\Quest");

	sprintf_s(cDir, "AscII%d", p->m_cCharName[0]);
	strcat_s(cFileName, cDir);
	strcat_s(cFileName, "\\");
	_mkdir(cFileName);

	strcat_s(cFileName, p->m_cCharName);
	strcat_s(cFileName, ".cfg");

	FILE* fp = fopen(cFileName, "w");
	if (!fp) return;

	std::string buffer;
	buffer.reserve(8192); // Reserva anticipada

	for (int i = 0; i < p->vec_charquest.size(); ++i)
	{
		const auto& quest = p->vec_charquest[i];
		buffer += "quest-num-" + std::to_string(i + 1) + " = ";
		buffer += std::to_string(quest.m_sParentQuestID) + " ";
		buffer += quest.m_cNpcName; buffer += " ";
		buffer += quest.m_cMapName; buffer += " ";
		buffer += quest.m_cRealMapName; buffer += " ";
		buffer += std::to_string(quest.m_iKills) + " ";
		buffer += std::to_string(quest.m_iMaxKills) + " ";
		buffer += std::to_string(quest.m_iRewardContribution) + " ";
		buffer += quest.m_cQuestReward1; buffer += " ";
		buffer += quest.m_cQuestReward2; buffer += " ";
		buffer += quest.m_cQuestReward3; buffer += " ";
		buffer += std::to_string(quest.m_sStatus) + " ";
		buffer += std::to_string(quest.m_sCompleted) + "\n";
	}

	fwrite(buffer.c_str(), 1, buffer.size(), fp);
	fclose(fp);
}

void cCoreServer::save_ekquest_data(int client)
{
	if (client < 0 || client >= DEF_MAXCLIENTS) return;

	auto p = m_pClientList[client];
	if (!p || p->m_cCharName[0] == 0) return;

	char cFileName[256] = {};
	char cDir[64] = {};

	sprintf_s(cFileName, "Database\\EkQuest\\");
	_mkdir("Database\\EkQuest");

	sprintf_s(cDir, "AscII%d", p->m_cCharName[0]);
	strcat_s(cFileName, cDir);
	strcat_s(cFileName, "\\");
	_mkdir(cFileName);

	strcat_s(cFileName, p->m_cCharName);
	strcat_s(cFileName, ".cfg");

	FILE* fp = fopen(cFileName, "w");
	if (!fp) return;

	std::string buffer;
	buffer.reserve(4096);

	for (size_t i = 0; i < p->vec_charekquest.size(); ++i)
	{
		const auto& quest = p->vec_charekquest[i];
		buffer += "ekquest-num-" + std::to_string(i + 1) + " = ";
		buffer += std::to_string(quest.m_iParentQuestID) + " ";
		buffer += quest.m_cMapName; buffer += " ";
		buffer += quest.m_cShowMapName; buffer += " ";
		buffer += quest.m_cRewardName; buffer += " ";
		buffer += std::to_string(quest.m_iKills) + " ";
		buffer += std::to_string(quest.m_iMaxKills) + " ";
		buffer += std::to_string(quest.status ? 1 : 0) + " ";
		buffer += std::to_string(quest.m_sCompleted) + "\n";
	}

	fwrite(buffer.c_str(), 1, buffer.size(), fp);
	fclose(fp);
}

void cCoreServer::save_drops_data(int client)
{
	if (client < 0 || client >= DEF_MAXCLIENTS) return;

	auto p = m_pClientList[client];
	if (!p || p->m_cCharName[0] == 0) return;

	char cFileName[256] = {};
	char cDir[64] = {};

	sprintf_s(cFileName, "Database\\Drops\\");
	_mkdir("Database\\Drops");

	sprintf_s(cDir, "AscII%d", p->m_cCharName[0]);
	strcat_s(cFileName, cDir);
	strcat_s(cFileName, "\\");
	_mkdir(cFileName);

	strcat_s(cFileName, p->m_cCharName);
	strcat_s(cFileName, ".cfg");

	FILE* fp = fopen(cFileName, "w");
	if (!fp) return;

	std::string buffer;
	buffer.reserve(4096);

	for (size_t i = 0; i < p->m_vDropItemList.size(); ++i)
	{
		const auto& vDrops = p->m_vDropItemList[i];
		buffer += "drop-item-" + std::to_string(i + 1) + " = ";
		buffer += vDrops.m_cItemName;
		buffer += " " + std::to_string(vDrops.itemcount);
		buffer += " " + std::to_string(vDrops.attr);
		buffer += "\n";
	}

	fwrite(buffer.c_str(), 1, buffer.size(), fp);
	fclose(fp);
}

void cCoreServer::joinPurgeEvent(int client)
{
	if (!g_ev.Is(EventID::ThePurge)) return;

	auto p = m_pClientList[client];	
	if (!p || !p->IsShopMap()) return;

	if (p->m_iAdminUserLevel != 0)
	{
		p->m_sPurgeTeam = TEAM_NONE;
	}
	else
	{
		int team1count = 0; int team2count = 0;
		for (int i = 1; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = m_pClientList[i];
			if (!pi || pi->m_iAdminUserLevel != 0) continue;
			if (!pi->IsInMap("purgecity") && !pi->IsInMap("crazyshop2") && !pi->IsInMap("crazyshop1")) continue;

			if (pi->m_sPurgeTeam == TEAM_ONE) team1count++;
			else if (pi->m_sPurgeTeam == TEAM_TWO) team2count++;
		}

		if (team1count > team2count) p->m_sPurgeTeam = TEAM_TWO;
		else if (team2count > team1count) p->m_sPurgeTeam = TEAM_ONE;
		else p->m_sPurgeTeam = RollDice(TEAM_ONE, TEAM_TWO);
	}

	

	RequestDismissPartyHandler(client);
	p->m_bIsSafeAttackMode = FALSE;
	RemoveFlags(client);
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);

	const char* mapName = nullptr;
	switch (p->m_sPurgeTeam)
	{
	case TEAM_NONE: mapName = "purgecity"; break;
	case TEAM_ONE:  mapName = "crazyshop2";   break;
	case TEAM_TWO:  mapName = "crazyshop1";   break;
	default:        return;
	}

	SendCommand(client, "/purgeteam", p->m_sPurgeTeam);
	RequestTeleportHandler(client, "2   ", (char*)mapName, -1, -1);
}

void cCoreServer::DisableBloorRite()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (m_pClientList[i]->m_iAdminUserLevel != 0) continue;

		if (pi->IsInMap("purgecity") || pi->IsInMap("crazyshop2") || pi->IsInMap("crazyshop1"))
		{
			if (pi->m_bIsKilled)
			{
				pi->m_bIsBeingResurrected = TRUE;
				pi->requestrevive = true;
				not_revive_purge(i);
			}
			else
			{
				if (pi->IsLocation("aresden"))
				RequestTeleportHandler(i, "2   ", "gshop_1", -1, -1);
				else RequestTeleportHandler(i, "2   ", "gshop_2", -1, -1);
			}
		}
	}
}

void cCoreServer::not_revive_purge(int client)
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

		if (p->IsLocation("aresden"))
			RequestTeleportHandler(client, "2   ", "gshop_1", -1, -1);
		else RequestTeleportHandler(client, "2   ", "gshop_2", -1, -1);
}

void cCoreServer::not_revive_purge_newround(int client)
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

	const char* mapName = nullptr;
	switch (p->m_sPurgeTeam)
	{
	case TEAM_NONE: mapName = "purgecity"; break;
	case TEAM_ONE:  mapName = "crazyshop2";   break;
	case TEAM_TWO:  mapName = "crazyshop1";   break;
	default:        return;
	}

	SendCommand(client, "/purgeteam", p->m_sPurgeTeam);
	RequestTeleportHandler(client, "2   ", (char*)mapName, -1, -1);
}

void cCoreServer::HandlePurgeNewRound()
{
	if (!g_ev.Is(EventID::ThePurge)) return;

	purge_newround++;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		//if (!pi->IsInMap("purge") && !pi->IsInMap("brshop1") && !pi->IsInMap("brshop2")) continue;

		SendCommand(i, "/purgeroundtime", purge_newround);
	}

	if (purge_newround == 600)
	{
		purge_newround = 0;
		for (int i = 1; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = m_pClientList[i];
			if (!pi) continue;

			if (m_pClientList[i]->m_iAdminUserLevel != 0) continue;

			int team1count = 0; int team2count = 0;
			for (int b = 1; b < DEF_MAXCLIENTS; b++)
			{
				auto pb = m_pClientList[b];
				if (!pb || pb->m_iAdminUserLevel != 0) continue;
				if (!pb->IsInMap("purgecity") && !pb->IsInMap("crazyshop2") && !pb->IsInMap("crazyshop1")) continue;

				if (pb->m_sPurgeTeam == TEAM_ONE) team1count++;
				else if (pb->m_sPurgeTeam == TEAM_TWO) team2count++;
			}

			if (team1count > team2count) pi->m_sPurgeTeam = TEAM_TWO;
			else if (team2count > team1count) pi->m_sPurgeTeam = TEAM_ONE;
			else pi->m_sPurgeTeam = RollDice(TEAM_ONE, TEAM_TWO);

			if (pi->IsInMap("purgecity") || pi->IsInMap("crazyshop2") || pi->IsInMap("crazyshop1"))
			{
				if (pi->m_bIsKilled)
				{
					pi->m_bIsBeingResurrected = TRUE;
					pi->requestrevive = true;
					not_revive_purge_newround(i);
				}
				else
				{
					const char* mapName = nullptr;
					switch (pi->m_sPurgeTeam)
					{
					case TEAM_NONE: mapName = "purgecity"; break;
					case TEAM_ONE:  mapName = "crazyshop2";   break;
					case TEAM_TWO:  mapName = "crazyshop1";   break;
					default:        return;
					}

					SendCommand(i, "/purgeteam", pi->m_sPurgeTeam);
					RequestTeleportHandler(i, "2   ", (char*)mapName, -1, -1);
				}
			}

			ShowClientMsg(i, "Ha comenzado una nueva ronda en The Purge");
		}
	}
	
}

void cCoreServer::send_charekquest_data_byindex(int client, int index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[20];
	auto * cp = data;
	u32 msgid = -1;
	u16 nulled = 0;

	msgid = NOT_CHAREKQUESTSBYINDEX;

	Push(cp, msgid);
	Push(cp, nulled);

	Push(cp, index);
	Push(cp, p->vec_charekquest[index].m_iKills);
	short sStatus = p->vec_charekquest[index].status ? 1 : 0;
	Push(cp, sStatus);
	Push(cp, p->vec_charekquest[index].m_sCompleted);

	p->m_pXSock->iSendMsg(data, cp - data);
}


void cCoreServer::AdminRequestBanUserName(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (string(p->m_cCharName) != "HeatoN")
	{
		ShowClientMsg(client, "Error! Solo los HeatoN puede banear jugadores.");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);
	
	bool bBanned = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			bBanned = true;

			AddBanEntry("name", pi->m_cCharName);
			
			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bBanned)
	{
		wsprintf(G_cTxt, "Se banedo el jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al banear al jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}

EventID getEventIDByName(const char* name)
{
	for (int i = 0; i < (int)EventID::Max; ++i)
	{
		if (_stricmp(_eventNameShort[i], name) == 0)
			return (EventID)i;
	}
	return (EventID)99;
}

void cCoreServer::AdminRequestChangeEventStatus(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iAdminUserLevel == 0) return;

	char m_cEventName[50];
	Pop(data, m_cEventName, 50);

	EventID id = getEventIDByName(m_cEventName);
	if (id == (EventID)99) return;

	if (g_ev.Is(id))
	{
		g_ev.Deactivate(id);
	}
	else
	{
		g_ev.Activate(id);
	}
}

void cCoreServer::AdminRequestDisconnectPlayer(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (string(p->m_cCharName) != "HeatoN")
	{
		ShowClientMsg(client, "Error! Solo los HeatoN puede desconectar jugadores.");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);

	bool bBanned = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			bBanned = true;

			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bBanned)
	{
		wsprintf(G_cTxt, "Se desconecto el jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al desconectar al jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}

void cCoreServer::AdminRequestBanAll(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (string(p->m_cCharName) != "HeatoN")
	{
		ShowClientMsg(client, "Error! Solo los HeatoN puede banear jugadores.");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);

	bool bBanned = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			bBanned = true;

			AddBanEntry("name", pi->m_cCharName);
			AddBanEntry("ip", pi->m_cIPaddress);
			AddBanEntry("mac", pi->m_cMacAdrees);

			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bBanned)
	{
		wsprintf(G_cTxt, "Se banedo el jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al banear al jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}

void cCoreServer::AdminRequestBanMAC(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (string(p->m_cCharName) != "HeatoN")
	{
		ShowClientMsg(client, "Error! Solo los HeatoN puede banear jugadores.");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);

	bool bBanned = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			bBanned = true;

			AddBanEntry("mac", pi->m_cMacAdrees);

			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bBanned)
	{
		wsprintf(G_cTxt, "Se banedo el MAC del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al banear el mac del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}

void cCoreServer::AdminRequestBanIP(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (string(p->m_cCharName) != "HeatoN")
	{
		ShowClientMsg(client, "Error! Solo los HeatoN puede banear jugadores.");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);

	bool bBanned = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			bBanned = true;

			AddBanEntry("ip", pi->m_cIPaddress);

			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bBanned)
	{
		wsprintf(G_cTxt, "Se banedo el IP del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al banear el IP del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}

void cCoreServer::AdminRequestChangeCity(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iAdminUserLevel == 0)
	{
		ShowClientMsg(client, "Error! Solo los GM pueden usar ChangeCity");
		return;
	}

	char m_cCharName[12];
	Pop(data, m_cCharName, 12);
	short city = 0;
	Pop(data, city);

	bool bChange = false;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (p == pi) continue;
		if (string(pi->m_cCharName) == m_cCharName)
		{
			if (city == 1 && pi->IsLocation("aresden")) continue;
			if (city == 2 && pi->IsLocation("elvine")) continue;

			bChange = true;

			ZeroMemory(pi->m_cGuildName, sizeof(pi->m_cGuildName));
			memcpy_secure(pi->m_cGuildName, "NONE", 4);

			pi->m_iGuildRank = -1;
			pi->m_iGuildGUID = -1;

			if (city == 1)
			{
				ZeroMemory(pi->m_cLocation, sizeof(pi->m_cLocation));
				memcpy_secure(pi->m_cLocation, "aresden", 10);
			}
			if (city == 2)
			{
				ZeroMemory(pi->m_cLocation, sizeof(pi->m_cLocation));
				memcpy_secure(pi->m_cLocation, "elvine", 10);
			}
			
			pi->m_bForceDisconnect = true;
			DeleteClient(i, true, true);
			break;
		}
	}

	if (bChange)
	{
		wsprintf(G_cTxt, "Se cambio la ciudad del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
	else
	{
		wsprintf(G_cTxt, "Error al cambiar la ciudad del jugador %s", m_cCharName);
		ShowClientMsg(client, G_cTxt);
	}
}
void cCoreServer::HandleSendTotalPlayerList(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	vPlayers.clear();

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;

		short tmpside = 0;
		if (strcmp(pi->m_cLocation, "aresden") == 0) tmpside = 1;
		else if (strcmp(pi->m_cLocation, "elvine") == 0) tmpside = 2;

		st_players u;
		u.m_sSide = tmpside;

		ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
		strcpy(u.m_cMapName, pi->m_cMapName);

		ZeroMemory(u.m_cCharName, sizeof(u.m_cCharName));
		strcpy(u.m_cCharName, pi->m_cCharName);

		vPlayers.push_back(u);
	}

	SendTotalPlayerList(client);
}

void cCoreServer::SendTotalPlayerList(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	const size_t playerSize = sizeof(short) + 24;
	size_t bufferSize = sizeof(u32) + sizeof(u16) + sizeof(int) + vPlayers.size() * playerSize;

	char* cData = new char[bufferSize];
	auto cp = cData;

	Push(cp, (u32)NOT_PLAYERS_DATA);
	Push(cp, (u16)0);
	Push(cp, (int)vPlayers.size());

	for (const auto& player : vPlayers)
	{
		Push(cp, player.m_sSide);
		Push(cp, player.m_cMapName, 12);
		Push(cp, player.m_cCharName, 12);
	}

	p->m_pXSock->iSendMsg(cData, cp - cData);
	delete[] cData;
}

void cCoreServer::HandleGivePointsCommand(int iAdminH, int iTargetH, const char* pointType, int amount)
{
	auto* pAdmin = m_pClientList[iAdminH];
	auto* pTarget = m_pClientList[iTargetH];

	if (!pAdmin || !pTarget) return;

	if (strcmpi(pointType, "ek") == 0) {
		pTarget->m_iEnemyKillCount += amount;
		SendCommand(iTargetH, "/eks", pTarget->m_iEnemyKillCount);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Eks");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "contrib") == 0) {
		pTarget->m_iContribution += amount;
		SendCommand(iTargetH, "/contrib", pTarget->m_iContribution);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Contrib");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "rep") == 0) {
		pTarget->m_iRating += amount;
		SendCommand(iTargetH, "/rep", pTarget->m_iRating);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Rep");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "maj") == 0) {
		pTarget->m_iGizonItemUpgradeLeft += amount;
		SendCommand(iTargetH, "/majs", pTarget->m_iGizonItemUpgradeLeft);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Majestics");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "tok") == 0) {
		pTarget->m_iTokens += amount;
		SendCommand(iTargetH, "/tokens", pTarget->m_iTokens);

		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Tokens");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "fragpa") == 0) {
		pTarget->fragment_pa += amount;
		SendCommand(iTargetH, "/fragment_pa", pTarget->fragment_pa);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Pa Frags");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "fragma") == 0) {
		pTarget->fragment_ma += amount;
		SendCommand(iTargetH, "/fragment_ma", pTarget->fragment_ma);
		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Ma Frags");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "fragmr") == 0) {
		pTarget->fragment_mr += amount;
		SendCommand(iTargetH, "/fragment_mr", pTarget->fragment_mr);

		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Mr Frags");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "fragdr") == 0) {
		pTarget->fragment_dr += amount;
		SendCommand(iTargetH, "/fragment_dr", pTarget->fragment_dr);

		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Dr Frags");
		ShowClientMsg(iTargetH, msg);
	}
	else if (strcmpi(pointType, "fraghp") == 0) {
		pTarget->fragment_hp += amount;
		SendCommand(iTargetH, "/fragment_hp", pTarget->fragment_hp);

		// Feedback para el jugador

		char msg[128];
		sprintf(msg, "Has recibido +%d puntos de %s", amount, "Hit Frags");
		ShowClientMsg(iTargetH, msg);
	}
	else {
		ShowClientMsg(iAdminH, "Tipo de punto inválido.");
		return;
	}

	// Feedback para el admin
	char msg2[128];
	sprintf(msg2, "Puntos otorgados a %s: +%d (%s)", pTarget->m_cCharName, amount, pointType);
	ShowClientMsg(iAdminH, msg2);
}

void cCoreServer::LoadBanLists()
{
	g_vBannedIPs.clear();
	g_vBannedMACs.clear();
	g_vBannedNames.clear();

	FILE* fp;

	char line[64];

	// Crear carpeta si no existe
	CreateDirectory("Bans", NULL);

	// IPs
	fp = fopen("Bans/IPBan.txt", "r");
	if (fp)
	{
		while (fgets(line, sizeof(line), fp))
		{
			line[strcspn(line, "\r\n")] = 0; // eliminar salto
			if (strlen(line) > 0)
				g_vBannedIPs.push_back(line);
		}
		fclose(fp);
	}

	// MACs
	fp = fopen("Bans/MACBan.txt", "r");
	if (fp)
	{
		while (fgets(line, sizeof(line), fp))
		{
			line[strcspn(line, "\r\n")] = 0;
			if (strlen(line) > 0)
				g_vBannedMACs.push_back(line);
		}
		fclose(fp);
	}

	// Nombres
	fp = fopen("Bans/NameBan.txt", "r");
	if (fp)
	{
		while (fgets(line, sizeof(line), fp))
		{
			line[strcspn(line, "\r\n")] = 0;
			if (strlen(line) > 0)
				g_vBannedNames.push_back(line);
		}
		fclose(fp);
	}
}

void cCoreServer::AddBanEntry(const char* type, const std::string& value)
{
	FILE* fp = nullptr;

	if (strcmpi(type, "ip") == 0)
	{
		g_vBannedIPs.push_back(value);
		fp = fopen("Bans/IPBan.txt", "a+");
	}
	else if (strcmpi(type, "mac") == 0)
	{
		g_vBannedMACs.push_back(value);
		fp = fopen("Bans/MACBan.txt", "a+");
	}
	else if (strcmpi(type, "name") == 0)
	{
		g_vBannedNames.push_back(value);
		fp = fopen("Bans/NameBan.txt", "a+");
	}

	if (fp)
	{
		fprintf(fp, "%s\n", value.c_str());
		fclose(fp);
	}
}

void cCoreServer::getClientMacAddress(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char m_cMacAddress[18];
	Pop(data, m_cMacAddress, 18);
	
	char m_cIpAddress[32];
	Pop(data, m_cIpAddress, 32);

	ZeroMemory(p->m_cMacAdrees, sizeof(p->m_cMacAdrees));
	strcpy(p->m_cMacAdrees, m_cMacAddress);

	ZeroMemory(p->m_cRealIPaddress, sizeof(p->m_cRealIPaddress));
	strcpy(p->m_cRealIPaddress, m_cIpAddress);

	if (string(p->m_cRealIPaddress) == "0.0.0.0")
	{
		ZeroMemory(p->m_cRealIPaddress, sizeof(p->m_cRealIPaddress));
		strcpy(p->m_cRealIPaddress, p->m_cIPaddress);
	}

	// Log en consola del servidor
	wsprintf(G_cTxt, "(!) Jugador conectado: %s - Mac: %s - IP: %s", p->m_cCharName, p->m_cMacAdrees, p->m_cRealIPaddress);
	PutLogList(G_cTxt);

/*
	// Comprobación de ban MAC
	for (const auto& banned : g_vBannedMACs)
	{
		if (_stricmp(p->m_cMacAdrees, banned.c_str()) == 0)
		{
			p->m_bForceDisconnect = true;
			DeleteClient(client, true, true);
			return;
		}
	}

	// Comprobación de ban IP
	for (const auto& banned : g_vBannedIPs)
	{
		if (_stricmp(p->m_cIPaddress, banned.c_str()) == 0)
		{
			p->m_bForceDisconnect = true;
			DeleteClient(client, true, true);
			return;
		}
	}		*/

/*
	// Log en archivo EnterGame.txt
	SYSTEMTIME st;
	GetLocalTime(&st);

	char logEntry[256];
	sprintf(logEntry, "[%04d-%02d-%02d %02d:%02d:%02d] %s - MAC: %s - IP: %s\n",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond,
		p->m_cCharName, p->m_cMacAdrees, p->m_cIPaddress);

	// Crear carpeta Logs si no existe
	CreateDirectory("Logs", NULL);

	FILE* fp = fopen("Logs/EnterGame.txt", "a+");
	if (fp)
	{
		fputs(logEntry, fp);
		fclose(fp);
	}
*/

}

void cCoreServer::FinishGunGameEvent()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;

		if (pi->m_iAdminUserLevel != 0) continue;

		if (pi->IsInMap("gg"))
		{
			if (pi->m_bIsKilled)
			{
				pi->m_bIsBeingResurrected = TRUE;
				pi->requestrevive = true;
				not_reviveGG(i);
			}
			else
			{
				if (pi->IsLocation("elvine")) RequestTeleportHandler(i, "2   ", "gshop_2", -1, -1);
				else RequestTeleportHandler(i, "2   ", "gshop_1", -1, -1);
			}
		}

		int fighter_index = get_ggfighterindex(pi->m_cCharName);
		if (fighter_index != -1)
		{
			auto ggkills = vec_ggfighters[fighter_index].kills;
			if (ggkills != 0)
			{
				int ekreward = ggkills * 5;
				pi->m_iEnemyKillCount += ekreward;
				SendCommand(i, "/eks", pi->m_iEnemyKillCount);
				wsprintf(G_cTxt, "Recibiste %d Eks de gungame event.", ekreward);
				ShowClientMsg(i, G_cTxt);
			}
		}
	}

	std::sort(vec_ggfighters.begin(), vec_ggfighters.end(), [](const st_ggfighters& a, const st_ggfighters& b) {
		return a.kills > b.kills;
	});

	if (!vec_ggfighters.empty())
	{
		const int maxWinners = 3;
		const int rewards[maxWinners] = { 10, 7, 5 };

		for (int i = 0; i < maxWinners; ++i)
		{
			if (i >= (int)vec_ggfighters.size()) break;

			if (vec_ggfighters[i].kills == 0) continue; 

			const char* fighterName = vec_ggfighters[i].fighter;
			if (!fighterName || fighterName[0] == 0) continue;

			int clientIndex = GetClientIndexByName((char*)fighterName);
			if (clientIndex == -1) continue;

			auto client = m_pClientList[clientIndex];
			if (!client) continue;

			int tokensAward = rewards[i];
			client->m_iTokens += tokensAward;

			SendCommand(clientIndex, "/tokens", client->m_iTokens);

			wsprintf(G_cTxt, "%d puesto en GunGame, ganaste +%d Tokens.", i + 1, tokensAward);
			ShowClientMsg(clientIndex, G_cTxt);
		}
	}

	vec_ggfighters.clear();
}

void cCoreServer::FinishDKSetEvent()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (pi->m_iAdminUserLevel != 0) continue;

		if (pi->IsInMap("dk"))
		{
			if (pi->m_bIsKilled)
			{
				pi->m_bIsKilled = false;
				pi->m_iHP = iGetMaxHP(i);
				pi->m_iMP = (((m_pClientList[i]->m_iMag + m_pClientList[i]->m_iAngelicMag) * 2) + (m_pClientList[i]->m_iLevel / 2)) + (m_pClientList[i]->m_iInt + m_pClientList[i]->m_iAngelicInt) / 2;
				pi->m_iSP = ((m_pClientList[i]->m_iStr + m_pClientList[i]->m_iAngelicStr) * 2) + (m_pClientList[i]->m_iLevel / 2);
				pi->m_bIsBeingResurrected = false;
				pi->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
				pi->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
				SendNotifyMsg(NULL, i, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
			}

			if (pi->IsLocation("elvine")) RequestTeleportHandler(i, "2   ", "gshop_2", -1, -1);
			else RequestTeleportHandler(i, "2   ", "gshop_1", -1, -1);
		}

		int fighter_index = get_dkfighterindex(pi->m_cCharName);
		if (fighter_index != -1)
		{
			auto dkkills = vec_dkfighters[fighter_index].kills;
			if (dkkills != 0)
			{
				int ekreward = dkkills * iekdkxkill;
				pi->m_iEnemyKillCount += ekreward;
				SendCommand(i, "/eks", pi->m_iEnemyKillCount);

				int eventpointsrew = dkkills;
				pi->m_iEventPoints += eventpointsrew;
				SendCommand(i, "/eventpoints", pi->m_iEventPoints);

				wsprintf(G_cTxt, "Recibiste %d Eks y %d EventPoints.", ekreward, eventpointsrew);
				ShowClientMsg(i, G_cTxt);
			}
		}
	}

	std::sort(vec_dkfighters.begin(), vec_dkfighters.end(), [](const st_dkfighters& a, const st_dkfighters& b) {
		return a.kills > b.kills;
	});

	if (!vec_dkfighters.empty())
	{
		const int maxWinners = 3;
		const int rewards[maxWinners] = { 15, 10, 5 };

		for (int i = 0; i < maxWinners; ++i)
		{
			if (i >= (int)vec_dkfighters.size()) break;

			if (vec_dkfighters[i].kills == 0) continue; 

			const char* fighterName = vec_dkfighters[i].fighter;
			if (!fighterName || fighterName[0] == 0) continue;

			int clientIndex = GetClientIndexByName((char*)fighterName);
			if (clientIndex == -1) continue;

			auto client = m_pClientList[clientIndex];
			if (!client) continue;

			int tokensAward = rewards[i];
			client->m_iTokens += tokensAward;

			SendCommand(clientIndex, "/tokens", client->m_iTokens);

			wsprintf(G_cTxt, "%d puesto en DKEvent, ganaste +%d Tokens.", i + 1, tokensAward);
			ShowClientMsg(clientIndex, G_cTxt);
		}
	}

	vec_dkfighters.clear();
}

void cCoreServer::printCharacters() {
	for (size_t i = 0; i < g_characters.size(); ++i) {
		CharacterInfo& c = g_characters[i];
		std::cout << "Name: " << c.name
			<< " | EK: " << c.ekCount
			<< " | Total EK: " << c.ekTotal
			<< " | Rating: " << c.rating
			<< " | Contribution: " << c.contribution
			<< " | PK: " << c.pkCount << std::endl;
	}
}
void cCoreServer::handleRequestRanking(int client, short rankmode)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	if (g_characters.empty()) return;
	if (m_bCargandoDatos) return;

	std::vector<CharacterInfo> sorted = g_characters;

	// Ordenar de mayor a menor según el rankmode
	switch (rankmode)
	{
	case RANK_EK:
		std::sort(sorted.begin(), sorted.end(), [](const CharacterInfo& a, const CharacterInfo& b) {
			return b.ekCount < a.ekCount;
		});
		break;

	case RANK_TOTALEK:
		std::sort(sorted.begin(), sorted.end(), [](const CharacterInfo& a, const CharacterInfo& b) {
			return b.ekTotal < a.ekTotal;
		});
		break;

	case RANK_REP:
		std::sort(sorted.begin(), sorted.end(), [](const CharacterInfo& a, const CharacterInfo& b) {
			return b.rating < a.rating;
		});
		break;

	case RANK_CONTRIB:
		std::sort(sorted.begin(), sorted.end(), [](const CharacterInfo& a, const CharacterInfo& b) {
			return b.contribution < a.contribution;
		});
		break;

	case RANK_PK:
		std::sort(sorted.begin(), sorted.end(), [](const CharacterInfo& a, const CharacterInfo& b) {
			return b.pkCount < a.pkCount;
		});
		break;

	default:
		return;
	}

	// Limitar al top 50
	if (sorted.size() > 50) {
		sorted.resize(50);
	}

	const size_t playerSize = sizeof(short) + 12 + sizeof(int); // Side + Nombre (12) + Puntos
	size_t bufferSize = sizeof(u32) + sizeof(u16) + sizeof(int) + sorted.size() * playerSize;

	char* cData = new char[bufferSize];
	auto cp = cData;

	Push(cp, (u32)NOT_RANK_DATA);
	Push(cp, (u16)rankmode);
	Push(cp, (int)sorted.size());

	for (const auto& player : sorted)
	{
		char nameBuffer[12] = { 0 };
		strncpy(nameBuffer, player.name.c_str(), 12);
		Push(cp, nameBuffer, 12);

		int points = 0;
		switch (rankmode)
		{
		case RANK_EK:       points = player.ekCount; break;
		case RANK_TOTALEK:  points = player.ekTotal; break;
		case RANK_REP:      points = player.rating; break;
		case RANK_CONTRIB:  points = player.contribution; break;
		case RANK_PK:       points = player.pkCount; break;
		}

		Push(cp, points);
	}

	p->m_pXSock->iSendMsg(cData, cp - cData);
	delete[] cData;
}

void cCoreServer::processCharacterFile(const std::string& filePath, const std::string& charName)
{
	return;
	// Filtrar nombres prohibidos
	if (IsBlockedCharacter(charName.c_str())) return;

	HANDLE hFile = CreateFileA(
		filePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL
		);

	if (hFile == INVALID_HANDLE_VALUE) return;

	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize) || fileSize.QuadPart > 16384) {
		CloseHandle(hFile);
		return;
	}

	const DWORD bufSize = static_cast<DWORD>(fileSize.QuadPart);
	char* buffer = static_cast<char*>(malloc(bufSize + 1));
	if (!buffer) {
		CloseHandle(hFile);
		return;
	}

	DWORD bytesRead;
	if (!ReadFile(hFile, buffer, bufSize, &bytesRead, NULL)) {
		free(buffer);
		CloseHandle(hFile);
		return;
	}
	buffer[bytesRead] = '\0';
	CloseHandle(hFile);

	CharacterInfo info;
	info.name = charName; // Nombre del archivo (sin extensión)

	char* lineStart = buffer;
	char* bufferEnd = buffer + bytesRead;

	while (lineStart < bufferEnd) {
		char* lineEnd = strchr(lineStart, '\n');
		if (!lineEnd) lineEnd = bufferEnd;

		*lineEnd = '\0';

		char* str = lineStart;
		fast_ltrim(str);

		if (*str != '\0') {
			char* eqPos = strchr(str, '=');
			if (eqPos) {
				*eqPos = '\0';
				char* value = eqPos + 1;
				fast_ltrim(value);

				if (strncmp(str, "character-EK-Count", 18) == 0) {
					info.ekCount = fast_atoi(value);
				}
				else if (strncmp(str, "character-EK-TotalCount", 23) == 0) {
					info.ekTotal = fast_atoi(value);
				}
				else if (strncmp(str, "character-RATING", 16) == 0) {
					info.rating = fast_atoi(value);
				}
				else if (strncmp(str, "character-contribution", 22) == 0) {
					info.contribution = fast_atoi(value);
				}
				else if (strncmp(str, "character-PK-Count", 18) == 0) {
					info.pkCount = fast_atoi(value);
				}
			}
		}

		lineStart = lineEnd + 1;
	}

	free(buffer);
	g_characters.push_back(std::move(info));
}

void cCoreServer::processDirectory(const std::string& directory) {
	return;
	WIN32_FIND_DATAA findFileData;
	std::string searchPath = directory + "\\*";

	HANDLE hFind = FindFirstFileExA(
		searchPath.c_str(),
		FindExInfoBasic,
		&findFileData,
		FindExSearchNameMatch,
		NULL,
		FIND_FIRST_EX_LARGE_FETCH
		);

	if (hFind == INVALID_HANDLE_VALUE) return;

	do {
		const char* name = findFileData.cFileName;

		// Saltar . y ..
		if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'))) {
			continue;
		}

		// Construir ruta completa
		char fullPath[MAX_PATH];
		strcpy(fullPath, directory.c_str());
		strcat(fullPath, "\\");
		strcat(fullPath, name);

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			processDirectory(fullPath);
		}
		else {
			const char* ext = strrchr(name, '.');
			if (ext && _stricmp(ext, ".txt") == 0) {

				// Obtener el nombre sin extensión
				size_t len = ext - name;
				char charName[MAX_PATH];
				memcpy(charName, name, len);
				charName[len] = '\0';

				// Llamar a processCharacterFile con el nombre correcto
				processCharacterFile(fullPath, charName);
			}
		}
	} while (FindNextFileA(hFind, &findFileData) != 0);

	FindClose(hFind);

	m_bCargandoDatos = false;
}
bool cCoreServer::IsBlockedCharacter(const char* name)
{
	static const char* blockedNames[] = {
		"GM1",
		"zamu-GM",
		"RxGM",
		"[Mr.Burns]",
		"Owner",
		"Heaton",
		"SukunaGM"
	};

	for (const auto& blocked : blockedNames)
	{
		if (_stricmp(name, blocked) == 0) return true;
	}

	return false;
}

void cCoreServer::send_charquest_data_byindex(int client, int index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[20];
	auto * cp = data;
	u32 msgid = -1;
	u16 nulled = 0;

	msgid = NOT_CHARQUESTS2;

	Push(cp, msgid);
	Push(cp, nulled);

	Push(cp, index);

	Push(cp, p->vec_charquest[index].m_iKills);
	Push(cp, p->vec_charquest[index].m_sStatus);
	Push(cp, p->vec_charquest[index].m_sCompleted);

	if (p->m_pXSock) p->m_pXSock->iSendMsg(data, cp - data);
}

bool cCoreServer::IsSpecialWeapon(char* itname)
{
	return strcmp(itname, "BlazingSword") == 0 ||
		strcmp(itname, "RapierofEmbers") == 0 ||
		strcmp(itname, "AsteroidHammer") == 0 ||

		strcmp(itname, "LightningSword") == 0 ||
		strcmp(itname, "HammerOfThor") == 0 ||
		strcmp(itname, "ThunderRapier") == 0 ||

		strcmp(itname, "IceExecutor") == 0 ||
		strcmp(itname, "FrozenHammer") == 0 ||
		strcmp(itname, "FireWizardStaff") == 0 ||
		strcmp(itname, "LightningWizStaff") == 0 ||
		strcmp(itname, "IceWizardStaff") == 0 ||
		
		strcmp(itname, "IceBladeOfAby") == 0 ||
		strcmp(itname, "FireBladeOfAby") == 0 ||
		strcmp(itname, "LightBladeOfAby") == 0 ||

		strcmp(itname, "IceStaffOfAby") == 0 ||
		strcmp(itname, "FireStaffOfAby") == 0 ||
		strcmp(itname, "LightStaffOfAby") == 0 ||

		strcmp(itname, "IceHammerOfAby") == 0 ||
		strcmp(itname, "FireHammerOfAby") == 0 ||
		strcmp(itname, "LightHammerOfAby") == 0 ||

		strcmp(itname, "EsterkOfFrost") == 0;
}

bool cCoreServer::IsFireWeapon(char* itname)
{
	return strcmp(itname, "BlazingSword") == 0 ||
		strcmp(itname, "RapierofEmbers") == 0 ||
		strcmp(itname, "FireWizardStaff") == 0 ||

		strcmp(itname, "FireBladeOfAby") == 0 ||
		strcmp(itname, "FireStaffOfAby") == 0 ||
		strcmp(itname, "FireHammerOfAby") == 0 ||

		strcmp(itname, "AsteroidHammer") == 0;
}

bool cCoreServer::IsLightningWeapon(char* itname)
{
	return strcmp(itname, "LightningSword") == 0 ||
		strcmp(itname, "HammerOfThor") == 0 ||
		strcmp(itname, "LightningWizStaff") == 0 ||
		
		strcmp(itname, "LightBladeOfAby") == 0 ||
		strcmp(itname, "LightStaffOfAby") == 0 ||
		strcmp(itname, "LightHammerOfAby") == 0 ||

		strcmp(itname, "ThunderRapier") == 0;
}

bool cCoreServer::IsIceWeapon(char* itname)
{
	return strcmp(itname, "IceExecutor") == 0 ||
		strcmp(itname, "FrozenHammer") == 0 ||
		strcmp(itname, "IceWizardStaff") == 0 ||

		strcmp(itname, "IceBladeOfAby") == 0 ||
		strcmp(itname, "IceStaffOfAby") == 0 ||
		strcmp(itname, "IceHammerOfAby") == 0 ||

		strcmp(itname, "EsterkOfFrost") == 0;
}

void cCoreServer::RequestUpgradeSpecialItems(int client, char * data)
{
	double dV1, dV2, dV3;
	DWORD dwTemp, iValue;
	int itemIndex;

	auto p = m_pClientList[client];
	if (!p) return;

	ShowClientMsg(client, "Por el momento no podes upgradear este item.");
	return;

	Pop(data, itemIndex);
	if ((itemIndex < 0) || (itemIndex >= DEF_MAXITEMS)) return;
	auto it = p->m_pItemList[itemIndex];
	if (!it) return;
	if (!IsSpecialWeapon(it->m_cName)) return;

	iValue = it->m_sItemSpecEffectValue3;

	if (iValue >= 30)
	{
		ShowClientMsg(client, "Has alcanzado el nivel maximo de este item.");
		return;
	}

	int cost = iValue * 300;
	if (cost == 0) cost = 300;

	if (IsIceWeapon(it->m_cName))
	{
		if (p->m_iIceSegments < cost) {
			ShowClientMsg(client, "No hay suficientes Ice Segments para mejorar el item.");
			return;
		}


		p->m_iIceSegments -= cost;
		SendCommand(client, "/icesegments", p->m_iIceSegments);
	}
	else if (IsLightningWeapon(it->m_cName))
	{
		if (p->m_iLightingSegments < cost) {
			ShowClientMsg(client, "No hay suficientes Lightning Segments para mejorar el item.");
			return;
		}
		
		p->m_iLightingSegments -= cost;
		SendCommand(client, "/lightningsegments", p->m_iLightingSegments);
	}
	else if (IsFireWeapon(it->m_cName))
	{
		if (p->m_iFireSegments < cost) {
			ShowClientMsg(client, "No hay suficientes Fire Segments para mejorar el item.");
			return;
		}

		p->m_iFireSegments -= cost;
		SendCommand(client, "/firesegments", p->m_iFireSegments);
	}
	
	it->m_sItemSpecEffectValue3++;
	SendCommand(client, "/newsv3", itemIndex, it->m_sItemSpecEffectValue3);

	char msg[256];
	wsprintf(msg, "El item %s se ha mejorado con exito.", it->m_cName);
	ShowClientMsg(client, msg);
}

void cCoreServer::SendEventList(int client, char * msg, int eventmode)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[220];
	char sendmsg[200];
	auto * cp = data;
	u32 msgid = SENDEVENTLIST;
	u16 nulled = 0;

	ZeroMemory(sendmsg, sizeof(sendmsg));
	strcpy(sendmsg, msg);
	
	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, msg, 200);
	Push(cp, eventmode);

	if (p->m_pXSock) p->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::requestChangeTalent(int client, char * data)
{
	return;
	short m_sChangeTalent;
	auto p = m_pClientList[client];
	if (!p) return;

	ShowClientMsg(client, "Por el momento no podes elegir un talento.");
	return;

	Pop(data, m_sChangeTalent);

	if (m_sChangeTalent < TALENT_FIRE || m_sChangeTalent > TALENT_ICE) return;

	if (p->m_sTalent == m_sChangeTalent)
	{
		SendEventList(client, "Error. Ya usas este talento.", 11);
		return;
	}

	const int cost = 30000;

	switch (m_sChangeTalent)
	{
	case TALENT_FIRE:
		if (p->m_iFireSegments < cost)
		{
			SendEventList(client, "No tienes suficientes Fire Segments.", 11);
			return;
		}
		p->m_iFireSegments -= cost;
		SendCommand(client, "/firesegments", p->m_iFireSegments);
		break;

	case TALENT_LIGHTNING:
		if (p->m_iLightingSegments < cost)
		{
			SendEventList(client, "No tienes suficientes Lightning Segments.", 11);
			return;
		}
		p->m_iLightingSegments -= cost;
		SendCommand(client, "/lightningsegments", p->m_iLightingSegments);
		break;

	case TALENT_ICE:
		if (p->m_iIceSegments < cost)
		{
			SendEventList(client, "No tienes suficientes Ice Segments.", 11);
			return;
		}
		p->m_iIceSegments -= cost;
		SendCommand(client, "/icesegments", p->m_iIceSegments);
		break;
	}

	// aplicar cambio
	p->m_sTalent = m_sChangeTalent;
	SendCommand(client, "/talent", p->m_sTalent);
	SendEventList(client, "El talento se ha seleccionado con exito.", 11);
	send_objects_data();
}

void cCoreServer::requestUpgradeTalent(int client, char * data)
{
	return;
	short m_sUpgradeTalent;
	auto p = m_pClientList[client];
	if (!p) return;

	ShowClientMsg(client, "Por el momento no podes upgradear un talento.");
	return;

	Pop(data, m_sUpgradeTalent);

	if (m_sUpgradeTalent < TALENT_FIRE || m_sUpgradeTalent > TALENT_ICE) return;

	if (m_sUpgradeTalent == TALENT_FIRE)
	{
		if (p->m_sFireTalentLvl == 3)
		{
			SendEventList(client, "Error. Ya tienes el nivel maximo.", 11);
			return;
		}
	}

	if (m_sUpgradeTalent == TALENT_LIGHTNING)
	{
		if (p->m_sLightningTalentLvl == 3)
		{
			SendEventList(client, "Error. Ya tienes el nivel maximo.", 11);
			return;
		}
	}

	if (m_sUpgradeTalent == TALENT_ICE)
	{
		if (p->m_sIceTalentLvl == 3)
		{
			SendEventList(client, "Error. Ya tienes el nivel maximo.", 11);
			return;
		}
	}

	const int cost = 750;

	switch (m_sUpgradeTalent)
	{
	case TALENT_FIRE:
		if (p->m_iFirePoints < cost)
		{
			SendEventList(client, "No tienes suficientes FirePoints.", 11);
			return;
		}

		p->m_iFirePoints -= cost;
		SendCommand(client, "/firepoints", p->m_iFirePoints);
		p->m_sFireTalentLvl++;
		SendCommand(client, "/firetalentlvl", p->m_sFireTalentLvl);
		break;

	case TALENT_LIGHTNING:
		if (p->m_iLightningPoints < cost)
		{
			SendEventList(client, "No tienes suficientes LightningPoints.", 11);
			return;
		}
		p->m_iLightningPoints -= cost;
		SendCommand(client, "/lightningpoints", p->m_iLightningPoints);
		p->m_sLightningTalentLvl++;
		SendCommand(client, "/lightningtalentlvl", p->m_sLightningTalentLvl);
		break;

	case TALENT_ICE:
		if (p->m_iIcePoints < cost)
		{
			SendEventList(client, "No tienes suficientes IcePoints.", 11);
			return;
		}
		p->m_iIcePoints -= cost;
		SendCommand(client, "/icepoints", p->m_iIcePoints);
		p->m_sIceTalentLvl++;
		SendCommand(client, "/icetalentlvl",p->m_sIceTalentLvl);
		break;
	}

	SendEventList(client, "El talento se ha upgradeado con exito.", 11);
	send_objects_data();
}

void cCoreServer::HelBotBehavior_Move(int iNpcH)
{
#ifdef DEF_DEBUG
	try {
#endif
		char  cDir;
		short sX, sY, dX = 0, dY = 0, absX, absY;
		short sTarget, sDistance;
		char  cTargetType;

		if (m_pNpcList[iNpcH] == NULL) return;
		if (m_pNpcList[iNpcH]->m_bIsKilled == TRUE) return;
		if ((m_pNpcList[iNpcH]->m_bIsSummoned == TRUE) && (m_pNpcList[iNpcH]->m_iSummonControlMode == 1)) return;
		if (m_pNpcList[iNpcH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) return;

		switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 2:
		case 3:
		case 4: //sacado v13
		case 5:
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_STOP;
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			return;
		}

		int iStX, iStY;
		if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex] != NULL) {
			iStX = m_pNpcList[iNpcH]->m_sX / 20;
			iStY = m_pNpcList[iNpcH]->m_sY / 20;
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
		}

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;
		if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 5) {
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;

			absX = abs(m_pNpcList[iNpcH]->m_vX - m_pNpcList[iNpcH]->m_sX);
			absY = abs(m_pNpcList[iNpcH]->m_vY - m_pNpcList[iNpcH]->m_sY);

			if ((absX <= 2) && (absY <= 2))
				CalcNextWayPointDestination(iNpcH);

			m_pNpcList[iNpcH]->m_vX = m_pNpcList[iNpcH]->m_sX;
			m_pNpcList[iNpcH]->m_vY = m_pNpcList[iNpcH]->m_sY;
		}

		HelBotTargetSearch(iNpcH, &sTarget, &cTargetType);
		if (sTarget != NULL) {
			if (m_pNpcList[iNpcH]->m_dwActionTime < 1000) {
				if (iDice(1, 3) == 3) {
					m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
					m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
					return;
				}
			}
			else {
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
				m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
				return;
			}
		}

		if ((m_pNpcList[iNpcH]->m_bIsMaster == TRUE) && (iDice(1, 3) == 2)) return;

		if (m_pNpcList[iNpcH]->m_cMoveType == DEF_MOVETYPE_FOLLOW) {
			sX = m_pNpcList[iNpcH]->m_sX;
			sY = m_pNpcList[iNpcH]->m_sY;
			switch (m_pNpcList[iNpcH]->m_cFollowOwnerType) {
			case DEF_OWNERTYPE_PLAYER:
				if (m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex] == NULL) {
					m_pNpcList[iNpcH]->m_cMoveType = DEF_MOVETYPE_RANDOM;
					return;
				}

				dX = m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sX;
				dY = m_pClientList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sY;
				break;

			case DEF_OWNERTYPE_NPC:
				if (m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex] == NULL) {
					m_pNpcList[iNpcH]->m_cMoveType = DEF_MOVETYPE_RANDOM;
					m_pNpcList[iNpcH]->m_iFollowOwnerIndex = NULL;
					return;
				}

				dX = m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sX;
				dY = m_pNpcList[m_pNpcList[iNpcH]->m_iFollowOwnerIndex]->m_sY;
				break;
			}

			if (abs(sX - dX) >= abs(sY - dY))
				sDistance = abs(sX - dX);
			else sDistance = abs(sY - dY);

			if (sDistance >= 3) {
				if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
					cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
				}
				else cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError, m_pNpcList[iNpcH]->m_sAreaSize, iNpcH);

				if (cDir != 0) {
					dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
					dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
					if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(3, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
					}
					else {
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetBigOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY, m_pNpcList[iNpcH]->m_sAreaSize);
					}
					m_pNpcList[iNpcH]->m_sX = dX;
					m_pNpcList[iNpcH]->m_sY = dY;
					m_pNpcList[iNpcH]->m_cDir = cDir;
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
					///updateEntitySync(iNpcH, DEF_OWNERTYPE_NPC, ACTION_OBJECTMOVE);
				}
			}
		}
		else {
			if (m_pNpcList[iNpcH]->m_sAreaSize == 0)
				cDir = cGetNextMoveDir(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_dX, m_pNpcList[iNpcH]->m_dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
			else cDir = cGetNextMoveDir(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_dX, m_pNpcList[iNpcH]->m_dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError, m_pNpcList[iNpcH]->m_sAreaSize, iNpcH);

			if (cDir == 0) {
				if (iDice(1, 10) == 3) CalcNextWayPointDestination(iNpcH);
			}
			else {
				dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
				dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];

				if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(4, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
				}
				else {
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetBigOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY, m_pNpcList[iNpcH]->m_sAreaSize);
				}
				m_pNpcList[iNpcH]->m_sX = dX;
				m_pNpcList[iNpcH]->m_sY = dY;
				m_pNpcList[iNpcH]->m_cDir = cDir;
				SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
				//updateEntitySync(iNpcH, DEF_OWNERTYPE_NPC, ACTION_OBJECTMOVE);
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcBehavior_Move");
	}
#endif
}

void cCoreServer::HelBotBehavior_Attack(int iNpcH)
{
#ifdef DEF_DEBUG
	try {
#endif
		int   iMagicType;
		short sX, sY, dX, dY;
		char  cDir;
		DWORD dwTime = timeGetTime();
		BOOL bFly = FALSE;

		if (m_pNpcList[iNpcH] == NULL) return;
		if (m_pNpcList[iNpcH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) return;
		if (m_pNpcList[iNpcH]->m_bIsKilled == TRUE) return;

		switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 1:
		case 2:
		case 3:
		case 4:
			return;
		}

		int iStX, iStY;
		if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex] != NULL) {
			iStX = m_pNpcList[iNpcH]->m_sX / 20;
			iStY = m_pNpcList[iNpcH]->m_sY / 20;
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iMonsterActivity++;
		}

		if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount == 0)
			m_pNpcList[iNpcH]->m_iAttackCount = 0;

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;
		if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 20) {
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;

			if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == FALSE))
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;

			return;
		}

		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;

		switch (m_pNpcList[iNpcH]->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex] == NULL) {
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;

		case DEF_OWNERTYPE_NPC:
			if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex] == NULL) {
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
				return;
			}
			dX = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
		}

		if ((iGetDangerValue(iNpcH, dX, dY) > m_pNpcList[iNpcH]->m_cBravery) &&
			(m_pNpcList[iNpcH]->m_bIsPermAttackMode == FALSE) &&
			(m_pNpcList[iNpcH]->m_cActionLimit != 5)) {

			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
			return;
		}

		if ((m_pNpcList[iNpcH]->m_iHP <= 2) && (iDice(1, m_pNpcList[iNpcH]->m_cBravery) <= 3) &&
			(m_pNpcList[iNpcH]->m_bIsPermAttackMode == FALSE) &&
			(m_pNpcList[iNpcH]->m_cActionLimit != 5)) {

			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
			return;
		}

		cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
		if ((abs(sX - dX) <= 1) && (abs(sY - dY) <= 1)) {
			if (cDir == 0) return;
			m_pNpcList[iNpcH]->m_cDir = cDir;

			if (m_pNpcList[iNpcH]->m_cActionLimit == 5) {
				//Para evitar que ataquen las torres de la sade, cuando no fueron construidas - ZeroEoyPnk
				if (m_pNpcList[iNpcH]->m_sAppr2 == 0) {
					switch (m_pNpcList[iNpcH]->m_sType) {
					case 36: // Crossbow Guard Tower: 
					case 87:
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 2);
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2, FALSE, FALSE, FALSE);
						break;

					case 37: // Cannon Guard Tower: 
					case 89:
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
						NpcMagicHandler(iNpcH, dX, dY, 61);
						break;
					}
				}
			}

			else {
				SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ���������� �����ϴ� �ǹ� 
				iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 1); // ���ݿ� ���� ȿ���� �����Ѵ�. 
				switch (m_pNpcList[iNpcH]->m_sType) {
				case 65: // Ice Golem
				case 98: // Ice Lizzard
					if (m_pNpcList[iNpcH]->m_cTargetType == DEF_OWNERTYPE_PLAYER) {
						if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) {
							if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
								bool bFrozenFlag = true;
								int FrozenTime = 5000; // 5 Segundos de freeze como default
								int NecklaceIndex = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->GetWearedItemIndex((int)DEF_EQUIPPOS_NECK);

								if (NecklaceIndex > -1) {
									if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex] != NULL) {

										if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 643) // Ice Elemental Necklace	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3228) // FullPowerNeck	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3235) // NeckMasterDef	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3248) // NeckMasterDef2	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3249) // NeckMasterDef3	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3250) // NeckMasterDef4	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 3251) // NeckMasterDef5	
											bFrozenFlag = false;
										else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_pItemList[NecklaceIndex]->m_sIDnum == 642) // Ice Pro Necklace									
											FrozenTime = 1250; // 25% * 5000									
									}
								}

								if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != NULL)
									if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
										FrozenTime = 1250; // 25% * 5000
									else if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5)
										bFrozenFlag = false;

								if (bFrozenFlag == true) {
									m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, TRUE);

									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE,
										dwTime + FrozenTime,
										m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER,
										NULL, NULL, NULL, 1, NULL, NULL);//	NULL, NULL, NULL, 1, NULL, NULL);

									SendNotifyMsg(NULL, m_pNpcList[iNpcH]->m_iTargetIndex,
										CLIENT_NOTIFY_MAGICEFFECTON,
										DEF_MAGICTYPE_ICE, 1, NULL, NULL);
								}
							}
						}
					}
					break;
				}
			}
			m_pNpcList[iNpcH]->m_iAttackCount++;

			if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == FALSE) && (m_pNpcList[iNpcH]->m_cActionLimit == 0)) {
				switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
				case DEF_ATTACKAI_EXCHANGEATTACK:
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
					break;

				case DEF_ATTACKAI_TWOBYONEATTACK:
					if (m_pNpcList[iNpcH]->m_iAttackCount >= 2) {
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
					}
					break;
				}
			}
		}
		else {
			cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
			if (cDir == 0) return;
			m_pNpcList[iNpcH]->m_cDir = cDir;

			if ((m_pNpcList[iNpcH]->m_cMagicLevel > 0) && (iDice(1, 2) == 1) &&
				(abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
				iMagicType = -1;
				switch (m_pNpcList[iNpcH]->m_cMagicLevel) { // �� ��Ŭ���� ���� ������ �������� �Ǵ��� ó��. 
				case 1:
					if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 0;
					break;

				case 2:
					if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 10;
					else if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 0;
					break;

				case 3: // Orc Mage
					if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 20; // Fire-Ball
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 10; // Energy-Bolt
					break;

				case 4: // ????
					if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 30; // Fire-Strike
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 37; // Lightning-Arrow
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 20; // Fire-Ball
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 10; // Energy-Bolt
					break;

				case 5: // Hellbound, Cyclops, Cannibal-Plant, Rudolph
					if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 43; // Lightning
					else if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 30; // Fire-Strike
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 37; // Lightning-Arrow
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 20; // Fire-Ball
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 10; // Energy-Bolt
					break;

				case 6: // Liche, Tentocle
					if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 51; // Lightning-Bolt
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 43; // Lightning
					else if (m_pMagicConfigList[30]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 30; // Fire-Strike
					else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 37; // Lightning-Arrow
					else if (m_pMagicConfigList[20]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 20; // Fire-Ball
					else if (m_pMagicConfigList[10]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 10; // Energy-Bolt
					break;

				case 7: // Demon, Gagoyle, Hellclaw, Barlog, Fire-Wyvern, MasterMage-Orc, LWB-Aresden, LWB-Elvine, GHK, GHKABS, TK, BG
					if ((m_pMagicConfigList[70]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 5) == 3)) iMagicType = 70; // Bloody-Shock-Wave
					else if (m_pMagicConfigList[61]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 61; // Mass-Fire-Strike
					else if (m_pMagicConfigList[60]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 60; // Energy-Strike
					else if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 51; // Lightning-Bolt
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 43; // Lightning
					break;

				case 8: // Unicorn, Centaurus, Giant-Lizard, 
					if ((m_pMagicConfigList[35]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 35; // Paralyze
					else if (m_pMagicConfigList[60]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 60; // Energy-Strike
					else if (m_pMagicConfigList[51]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 51; // Lightning-Bolt
					else if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 43; // Lightning
					break;

				case 9: // Tiger-Worm
					if ((m_pMagicConfigList[74]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 74; // Lightning-Strike
					break;

				case 10: // Frost, Nizie
					if ((m_pMagicConfigList[57]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 57; // Ice-Strike
					break;

				case 12: // 09/05/06 22:51 Wyvern
					if ((m_pMagicConfigList[91]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 91; // Blizzard
					else if (m_pMagicConfigList[63]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 63; // Mass-Child-Wind
					break;

				case 13: // Abaddon
					if ((m_pMagicConfigList[96]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 96; // Earth Shock Wave
					else if (m_pMagicConfigList[81]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 81; // Meteor Strike
					break;

				case 14: // LightWorm
					if ((m_pMagicConfigList[97]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 97; // StormOfDeath
					else if (m_pMagicConfigList[56]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 56; // Meteor Strike
					break;

				case 15: // Death Wyvern
					if ((m_pMagicConfigList[76]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 76; // cancel
					else if (m_pMagicConfigList[96]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 96; // esw
					else if (m_pMagicConfigList[35]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 35; // Lize
					break;

				case 16: // Hell Liche
					if ((m_pMagicConfigList[81]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 81; // meteor
					else if (m_pMagicConfigList[74]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 74; // LightingStrike
					else if (m_pMagicConfigList[83]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 83; // inibition
					break;

				case 17: // Hell Abaddon
					if ((m_pMagicConfigList[76]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 76; // cancel
					else if (m_pMagicConfigList[97]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 97; // StormOfDeath
					else if (m_pMagicConfigList[96]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 96; // esw
					else if (m_pMagicConfigList[83]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 83; // inibition
					break;
				case 18: // Hell hound
					if ((m_pMagicConfigList[85]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 85; // Fury
					else if (m_pMagicConfigList[65]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 65; // AMP
					break;
				case 19: // GhostAba
					if ((m_pMagicConfigList[76]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 5) == 1)) iMagicType = 76; // cancel
					else if ((m_pMagicConfigList[35]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 5) == 2)) iMagicType = 35; // Lize
					else if ((m_pMagicConfigList[96]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 5) == 3)) iMagicType = 96; // esw
					break;
				case 20: // BlackWyvern
					if ((m_pMagicConfigList[76]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 1)) iMagicType = 76; // cancel
					else if ((m_pMagicConfigList[92]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 2)) iMagicType = 92; // Lize
					else if ((m_pMagicConfigList[98]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) && (iDice(1, 3) == 3)) iMagicType = 98; // esw
					break;
				}

				if (iMagicType != -1) {
					if (m_pNpcList[iNpcH]->m_iAILevel >= 2) {
						switch (m_pNpcList[iNpcH]->m_cTargetType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2) {
								if ((abs(sX - dX) > m_pNpcList[iNpcH]->m_iAttackRange) || (abs(sY - dY) > m_pNpcList[iNpcH]->m_iAttackRange)) {
									m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
									m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								}
								else goto NBA_CHASE;
							}
							if ((iMagicType == 35) && (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)) goto NBA_CHASE;
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2) {
								if ((abs(sX - dX) > m_pNpcList[iNpcH]->m_iAttackRange) || (abs(sY - dY) > m_pNpcList[iNpcH]->m_iAttackRange)) {
									m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
									m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
									return;
								}
								else goto NBA_CHASE;
							}
							if ((iMagicType == 35) && (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)) goto NBA_CHASE;
							break;
						}
					}

					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ���������� �����ϴ� �ǹ� 
					NpcMagicHandler(iNpcH, dX, dY, iMagicType);
					m_pNpcList[iNpcH]->m_dwTime = dwTime + 2000;
					return;
				}
			}

			if ((m_pNpcList[iNpcH]->m_cMagicLevel < 0) && (iDice(1, 2) == 1) &&
				(abs(sX - dX) <= 9) && (abs(sY - dY) <= 7)) {
				iMagicType = -1;
				if (m_pMagicConfigList[43]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 43;
				else if (m_pMagicConfigList[37]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 37;
				else if (m_pMagicConfigList[0]->m_sValue1 <= m_pNpcList[iNpcH]->m_iMana) iMagicType = 0;

				if (iMagicType != -1) {
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir], m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir], 1); // 1 : Į���� ���������� �����ϴ� �ǹ� 
					NpcMagicHandler(iNpcH, dX, dY, iMagicType);
					m_pNpcList[iNpcH]->m_dwTime = dwTime + 2000;
					return;
				}
			}

			if ((m_pNpcList[iNpcH]->m_iAttackRange > 1) &&
				(abs(sX - dX) <= m_pNpcList[iNpcH]->m_iAttackRange) && (abs(sY - dY) <= m_pNpcList[iNpcH]->m_iAttackRange)) {

				cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
				if (cDir == 0) return;
				m_pNpcList[iNpcH]->m_cDir = cDir;

				if (m_pNpcList[iNpcH]->m_cActionLimit == 5) {
					//Para evitar que ataquen las torres de la sade, cuando no fueron construidas - ZeroEoyPnk
					if (m_pNpcList[iNpcH]->m_sAppr2 == 0) {
						switch (m_pNpcList[iNpcH]->m_sType) {
						case 36: // Crossbow Guard Tower
						case 87: // Cross Torret
							SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2);
							iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2);
							break;

						case 37: // Cannon Guard Tower
						case 89: // Ancient Gran Canion
							SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
							m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
							NpcMagicHandler(iNpcH, dX, dY, 61);
							break;
						}
					}
				}
				else {
					switch (m_pNpcList[iNpcH]->m_sType) {
					case 51: // v2.05 Catapult: �Ž� ���̾� ��Ʈ����ũ ���� 
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 1);
						m_pNpcList[iNpcH]->m_iMagicHitRatio = 1000;
						NpcMagicHandler(iNpcH, dX, dY, 61);
						break;

					case 54: // Dark Elf: Ȱ ������ �Ѵ�.
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 2); // 2: Ȱ���� 
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 2);
						break;

					case 63: //Frost
					case 53: //Beholder
					case 79: //Nizie
						switch (m_pNpcList[iNpcH]->m_cTargetType) {
						case DEF_OWNERTYPE_PLAYER:
							if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex] == NULL) goto NBA_BREAK1;
							if ((m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
								(bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, m_pNpcList[iNpcH]->m_iMagicHitRatio) == FALSE)) {
								if (m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0 && m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sLightingStatus == 0 && m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sFireStatus == 0) {
									m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, TRUE);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
										m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_PLAYER, NULL, NULL, NULL, 1, NULL, NULL);
									SendNotifyMsg(NULL, m_pNpcList[iNpcH]->m_iTargetIndex, CLIENT_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, NULL, NULL);
								}
							}
							break;

						case DEF_OWNERTYPE_NPC:
							if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex] == NULL) goto NBA_BREAK1;
							if ((m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_iHP > 0) &&
								(bCheckResistingIceSuccess(m_pNpcList[iNpcH]->m_cDir, m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_iMagicHitRatio) == FALSE)) {
								if (m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0) {
									m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
									SetIceFlag(m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, TRUE);
									bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (5 * 1000),
										m_pNpcList[iNpcH]->m_iTargetIndex, DEF_OWNERTYPE_NPC, NULL, NULL, NULL, 1, NULL, NULL);
								}
							}
							break;
						}
					NBA_BREAK1:;
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20); // 20: �ʻ���
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;

					default:
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, 20); // 20: �ʻ��� 
						iCalculateAttackEffect(m_pNpcList[iNpcH]->m_iTargetIndex, m_pNpcList[iNpcH]->m_cTargetType, iNpcH, DEF_OWNERTYPE_NPC, dX, dY, 20);
						break;
					}
				}
				m_pNpcList[iNpcH]->m_iAttackCount++;

				if ((m_pNpcList[iNpcH]->m_bIsPermAttackMode == FALSE) && (m_pNpcList[iNpcH]->m_cActionLimit == 0)) {
					switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
					case DEF_ATTACKAI_EXCHANGEATTACK:
						m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
						m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
						break;

					case DEF_ATTACKAI_TWOBYONEATTACK:
						if (m_pNpcList[iNpcH]->m_iAttackCount >= 2) {
							m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
							m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_FLEE;
						}
						break;
					}
				}
				return;
			}

		NBA_CHASE:;

			if (m_pNpcList[iNpcH]->m_cActionLimit != 0) return;
			m_pNpcList[iNpcH]->m_iAttackCount = 0;

			if (m_pNpcList[iNpcH]->m_sAreaSize == 0)
				cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
			else cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError, m_pNpcList[iNpcH]->m_sAreaSize, iNpcH);

			if (cDir == 0) return;
			dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
			dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
			if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(9, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
			}
			else {
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetBigOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY, m_pNpcList[iNpcH]->m_sAreaSize);
			}
			m_pNpcList[iNpcH]->m_sX = dX;
			m_pNpcList[iNpcH]->m_sY = dY;
			m_pNpcList[iNpcH]->m_cDir = cDir;
			SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcBehavior_Attack");
	}
#endif
}

void cCoreServer::HelBotBehavior_Flee(int iNpcH)
{
#ifdef DEF_DEBUG
	try {
#endif
		char cDir;
		short sX, sY, dX = 0, dY = 0;
		short sTarget;
		char  cTargetType;

		if (m_pNpcList[iNpcH] == NULL) return;
		if (m_pNpcList[iNpcH]->m_bIsKilled == TRUE) return;

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;

		switch (m_pNpcList[iNpcH]->m_iAttackStrategy) {
		case DEF_ATTACKAI_EXCHANGEATTACK: // ���� -> ���� -> ... 
		case DEF_ATTACKAI_TWOBYONEATTACK: // ���� -> ���� -> ���� -> ...
			if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 2) {
				m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				return;
			}
			break;

		default:
			if (iDice(1, 2) == 1) NpcRequestAssistance(iNpcH);
			break;
		}

		if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount > 10) {
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
			m_pNpcList[iNpcH]->m_tmp_iError = 0;
			if (m_pNpcList[iNpcH]->m_iHP <= 3) {
				m_pNpcList[iNpcH]->m_iHP += iDice(1, m_pNpcList[iNpcH]->m_iHitDice);
				if (m_pNpcList[iNpcH]->m_iHP <= 0) m_pNpcList[iNpcH]->m_iHP = 1;
			}
			return;
		}

		HelBotTargetSearch(iNpcH, &sTarget, &cTargetType);
		if (sTarget != NULL) {
			m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
			m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
		}

		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;
		switch (m_pNpcList[iNpcH]->m_cTargetType) {
		case DEF_OWNERTYPE_PLAYER:
			dX = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pClientList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
		case DEF_OWNERTYPE_NPC:
			dX = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sX;
			dY = m_pNpcList[m_pNpcList[iNpcH]->m_iTargetIndex]->m_sY;
			break;
		}
		dX = sX - (dX - sX);
		dY = sY - (dY - sY);

		if (m_pNpcList[iNpcH]->m_sAreaSize == 0)
			cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError);
		else cDir = cGetNextMoveDir(sX, sY, dX, dY, m_pNpcList[iNpcH]->m_cMapIndex, m_pNpcList[iNpcH]->m_cTurn, &m_pNpcList[iNpcH]->m_tmp_iError, m_pNpcList[iNpcH]->m_sAreaSize, iNpcH);

		if (cDir != 0) {
			dX = m_pNpcList[iNpcH]->m_sX + _tmp_cTmpDirX[cDir];
			dY = m_pNpcList[iNpcH]->m_sY + _tmp_cTmpDirY[cDir];
			if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY);
			}
			else {
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetBigOwner(iNpcH, DEF_OWNERTYPE_NPC, dX, dY, m_pNpcList[iNpcH]->m_sAreaSize);
			}
			m_pNpcList[iNpcH]->m_sX = dX;
			m_pNpcList[iNpcH]->m_sY = dY;
			m_pNpcList[iNpcH]->m_cDir = cDir;
			SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcBehavior_Flee");
	}
#endif
}


void cCoreServer::HelBotBehavior_Stop(int iNpcH)
{
#ifdef DEF_DEBUG
	try {
#endif
		char  cTargetType;
		short sTarget = NULL;
		BOOL  bFlag;

		if (m_pNpcList[iNpcH] == NULL) return;

		m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;

		switch (m_pNpcList[iNpcH]->m_cActionLimit) {
		case 5:
			switch (m_pNpcList[iNpcH]->m_sType) {
			case 38: // Mana Collector
				if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					bFlag = _bNpcBehavior_ManaCollector(iNpcH);
					if (bFlag == TRUE) {
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
					}
				}
				break;

			case 39: // Detector
				if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					bFlag = _bNpcBehavior_Detector(iNpcH);
					if (bFlag == TRUE) {
						SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
					}
				}
				break;

			case 40: // Energy Shield Generator
				break;

			case 41: // Grand Magic Generator
				if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
					m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
					_NpcBehavior_GrandMagicGenerator(iNpcH);
				}
				break;

			case 42: // ManaStone: v2.05 ���������� ���������� �������� 5�� �����Ѵ�.
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				m_pNpcList[iNpcH]->m_iV1 += 5;
				if (m_pNpcList[iNpcH]->m_iV1 >= 5) m_pNpcList[iNpcH]->m_iV1 = 5;
				break;

			default:
				HelBotTargetSearch(iNpcH, &sTarget, &cTargetType);
				break;
			}
			break;
		}

		if ((sTarget != NULL)) {
			m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
			m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
			return;
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcBehavior_Stop");
	}
#endif
}


void cCoreServer::HelBotTargetSearch(int iNpcH, short * pTarget, char * pTargetType)
{
#ifdef DEF_DEBUG
	try {
#endif
		register int ix, iy, iPKCount;
		register short sX, sY, rX, rY, dX = 0, dY = 0;
		short sOwner, sTargetOwner, sDistance, sTempDistance, TempOwner;
		char  cOwnerType, cTargetType, cTargetSide = 0;
		int   iInv = 0, iSearchType;
		BOOL ASS;

		sTargetOwner = NULL;
		cTargetType = NULL;
		sDistance = 100;

		sX = m_pNpcList[iNpcH]->m_sX;
		sY = m_pNpcList[iNpcH]->m_sY;
		
		// Aumentar el rango de búsqueda (ejemplo: duplicar el rango)
		int extendedRange = m_pNpcList[iNpcH]->m_cTargetSearchRange * 2;
		rX = m_pNpcList[iNpcH]->m_sX - extendedRange;
		rY = m_pNpcList[iNpcH]->m_sY - extendedRange;

		iSearchType = NULL;

		for (ix = rX; ix < rX + extendedRange * 2 + 1; ix++) {
			for (iy = rY; iy < rY + extendedRange * 2 + 1; iy++) {
				m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, ix, iy);
				if (sOwner != NULL) {

					if ((sOwner == iNpcH) && (cOwnerType == DEF_OWNERTYPE_NPC)) break;
					TempOwner = NULL;
					iPKCount = 0;

					switch (cOwnerType) {
					case DEF_OWNERTYPE_PLAYER:
						if (m_pClientList[sOwner] == NULL) {
							m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(5, sOwner, DEF_OWNERTYPE_PLAYER, ix, iy);
						}
						else {
							if (m_pClientList[sOwner]->m_iAdminUserLevel > 0) goto SKIP_SEARCH;
							if ((m_pClientList[sOwner]->m_cSide == DEF_NETURAL) && (!m_pClientList[sOwner]->Assasain)) goto SKIP_SEARCH;

							switch (iSearchType) {
							case 1: goto SKIP_SEARCH; break;
							case NULL: break;
							}

							dX = m_pClientList[sOwner]->m_sX;
							dY = m_pClientList[sOwner]->m_sY;
							cTargetSide = m_pClientList[sOwner]->m_cSide;
							iPKCount = m_pClientList[sOwner]->m_iPKCount;
							iInv = m_pClientList[sOwner]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY];
							TempOwner = sOwner;
						}
						break;

					case DEF_OWNERTYPE_NPC:
						if (m_pNpcList[sOwner] == NULL) {
							m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(6, sOwner, DEF_OWNERTYPE_NPC, ix, iy);
						}
						else {
							if (RunSummonEvent || RunSummonEvent2 || RunSummonEvent3) {
								if ((memcmp(m_pMapList[m_pNpcList[sOwner]->m_cMapIndex]->m_cName, "ESAresden", 9) == 0) ||
									(memcmp(m_pMapList[m_pNpcList[sOwner]->m_cMapIndex]->m_cName, "ESElvine", 8) == 0) ||
									(memcmp(m_pMapList[m_pNpcList[sOwner]->m_cMapIndex]->m_cName, "ESAssasain", 10) == 0))
									goto SKIP_SEARCH;
							}
							switch (iSearchType) {
							case 1:
								switch (m_pNpcList[sOwner]->m_sType) {
								case 36:
								case 37:
								case 38:
								case 39:
								case 40:
								case 41:
									break;

								default:
									goto SKIP_SEARCH;
									break;
								}
								break;
							case NULL:
								break;
							}

							dX = m_pNpcList[sOwner]->m_sX;
							dY = m_pNpcList[sOwner]->m_sY;
							cTargetSide = m_pNpcList[sOwner]->m_cSide;
							iPKCount = 0;
							iInv = m_pNpcList[sOwner]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY];

							if (m_pNpcList[iNpcH]->m_sType == 21) {
								if (_iCalcPlayerNum(m_pNpcList[sOwner]->m_cMapIndex, dX, dY, 2) != 0) {
									sOwner = NULL;
									cOwnerType = NULL;
								}
							}
						}
						break;
					}

					if (TempOwner != NULL) ASS = m_pClientList[TempOwner]->Assasain;
					else ASS = FALSE;

					if (m_pNpcList[iNpcH]->m_cSide < 10) {
						if (cTargetSide == 0) {
							if (iPKCount == 0 && ASS == FALSE) goto SKIP_SEARCH;
						}
						else {
							if ((iPKCount == 0 && ASS == FALSE) && (cTargetSide == m_pNpcList[iNpcH]->m_cSide)) goto SKIP_SEARCH;
							if (m_pNpcList[iNpcH]->m_cSide == 0) goto SKIP_SEARCH;
						}
					}
					else {
						if (m_pNpcList[iNpcH]->m_cSide == 11) {
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (cTargetSide == 0 && ASS == TRUE)) goto SKIP_SEARCH;
							if ((cOwnerType == DEF_OWNERTYPE_NPC) && (cTargetSide == 0)) goto SKIP_SEARCH;
						}
						else {
							if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (cTargetSide == 0 && ASS == FALSE)) goto SKIP_SEARCH;
							if ((cOwnerType == DEF_OWNERTYPE_NPC) && (cTargetSide == 0)) goto SKIP_SEARCH;
						}

						if (cTargetSide == m_pNpcList[iNpcH]->m_cSide) goto SKIP_SEARCH;
					}

					if ((iInv != 0) && (m_pNpcList[iNpcH]->m_cSpecialAbility != 1)) goto SKIP_SEARCH;

					if (abs(sX - dX) >= abs(sY - dY)) sTempDistance = abs(sX - dX);
					else sTempDistance = abs(sY - dY);

					if (sTempDistance < sDistance) {
						sDistance = sTempDistance;
						sTargetOwner = sOwner;
						cTargetType = cOwnerType;
					}
				SKIP_SEARCH:;
				}
			}
		}

		*pTarget = sTargetOwner;
		*pTargetType = cTargetType;
		return;
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: TargetSearch");
	}
#endif
}

void cCoreServer::joinRebirthFarm(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->IsShopMap()) return;
	if (p->m_bIsKilled) return;
	if (p->m_sRebirthStatus != 1 || p->m_sRebirthEnabled != 1 || p->m_iLevel == 200) return;

	if (p->IsLocation("aresden"))
	{
		RequestTeleportHandler(client, "2   ", "rfarm", 50, 205);
	}
	else
	{
		RequestTeleportHandler(client, "2   ", "rfarm", 189, 42);
	}

}

void cCoreServer::handleSpecialMagic(int client, char * data)
{
	char m_cTartetName[12];
	int magicType = -1;
	short sX = -1;
	short sY = -1;

	Pop(data, m_cTartetName, 12);
	Pop(data, magicType);
	Pop(data, sX);
	Pop(data, sY);

	magicType = magicType - 100;
/*

	if (magicType != 35) return;*/

	PlayerMagicHandler(client, sX, sY, magicType, 0, 0, m_cTartetName);
}

void cCoreServer::send_objects_data()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		// Para cada cliente, generamos su propia lista filtrada
		std::vector<st_objects> filteredObjects;

		for (int j = 1; j < DEF_MAXCLIENTS; j++)
		{
			auto pj = m_pClientList[j];
			if (!pj) continue;
			if (!pj->m_bIsInitComplete) continue;
			if (pj->m_cMapIndex != pi->m_cMapIndex) continue;
			if (pj->m_bLimbo) continue;

			short tmpside = 0;
			if (strcmp(pj->m_cLocation, "aresden") == 0) tmpside = 1;
			else if (strcmp(pj->m_cLocation, "elvine") == 0) tmpside = 2;

			/*int tmp_talent_level = 1;
			if (pj->m_sTalent == TALENT_FIRE)
			{
			tmp_talent_level = pj->m_sFireTalentLvl;
			}
			else if (pj->m_sTalent == TALENT_LIGHTNING)
			{
			tmp_talent_level = pj->m_sLightningTalentLvl;
			}
			else if (pj->m_sTalent == TALENT_ICE)
			{
			tmp_talent_level = pj->m_sIceTalentLvl;
			}*/

			st_objects u;
			u.client = pj->client;
			u.m_sSide = tmpside;
			u.sLevel = pj->m_iLevel;
			u.sRebirthLevel = pj->m_iRebirthLevel;
			u.sRelicHolder = pj->m_bRelicHolder ? 1 : 0;
			u.m_sFisher = pj->m_bFishing ? 1 : 0;
			u.m_sWriteStatus = pj->m_sWriteStatus;
			u.m_sPoisonStatus = pj->m_bIsPoisoned ? 1 : 0;
			u.m_sPurgeTeam = pj->m_sPurgeTeam;
			/*u.m_sPlayerRole = pj->m_sPlayerRole;
			u.m_sRaiderLvl = pj->m_sRaiderLvl;
			u.m_sHunterLvl = pj->m_sHunterLvl;
			u.m_sPurgeTeam = pj->m_sPurgeTeam;
			u.m_iSpecialItem = pj->m_iSpecialItem;
			u.m_sFireStatus = pj->m_sFireStatus;
			u.m_sLightingStatus = pj->m_sLightingStatus;
			u.m_sTalent = pj->m_sTalent;
			u.m_sTalentLvl = tmp_talent_level;*/
			filteredObjects.push_back(u);
		}

		if (!filteredObjects.empty())
			SendObjectsData(i, filteredObjects);
	}
}

void cCoreServer::SendObjectsData(int client, const std::vector<st_objects>& vObjects)
{
	auto p = m_pClientList[client];
	if (!p || vObjects.empty()) return;

	const size_t objectSize = sizeof(int) /*+ sizeof(int)*/ + sizeof(short) * 8;
	size_t bufferSize = sizeof(u32) + sizeof(u16) + sizeof(int) + vObjects.size() * objectSize;

	char* cData = new char[bufferSize];
	auto cp = cData;

	Push(cp, (u32)NOT_OBJECTS_DATA);
	Push(cp, (u16)0);
	Push(cp, (int)vObjects.size());

	for (const auto& obj : vObjects)
	{
		Push(cp, obj.client);
		Push(cp, obj.m_sSide);
		Push(cp, obj.sLevel);
		Push(cp, obj.sRebirthLevel);
		Push(cp, obj.sRelicHolder);
		Push(cp, obj.m_sFisher);
		Push(cp, obj.m_sWriteStatus);
		Push(cp, obj.m_sPoisonStatus);
		Push(cp, obj.m_sPurgeTeam);
		/*Push(cp, obj.m_sPlayerRole);
		Push(cp, obj.m_sRaiderLvl);
		Push(cp, obj.m_sHunterLvl);
		Push(cp, obj.m_iSpecialItem);
		Push(cp, obj.m_sFireStatus);
		Push(cp, obj.m_sLightingStatus);
		Push(cp, obj.m_sTalent);
		Push(cp, obj.m_sTalentLvl);*/
	}

	if (p->m_pXSock) p->m_pXSock->iSendMsg(cData, cp - cData);
	delete[] cData;
}

void cCoreServer::SendPlayersShines()
{
	if (!g_ev.Is(EventID::Shinning)) return;

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;
		
		RefreshParty(i, 3);

		std::vector<st_player_shine> vShines;

		for (int j = 1; j < DEF_MAXCLIENTS; j++)
		{
			if (i == j) continue;
			auto pj = m_pClientList[j];
			if (!pj) continue;
			if (!pj->m_bIsInitComplete) continue;
			if (pj->m_bLimbo) continue;
			if (pj->m_cMapIndex != pi->m_cMapIndex) continue;

			st_player_shine shine;
			shine.posX = pj->m_sX;
			shine.posY = pj->m_sY;
			vShines.push_back(shine);
		}

		if (!vShines.empty())
			SendShinesData(i, vShines);
	}
}

void cCoreServer::SendShinesData(int client, const std::vector<st_player_shine>& shines)
{
	auto p = m_pClientList[client];
	if (!p || shines.empty()) return;

	size_t bufferSize = sizeof(u32) + sizeof(u16) + sizeof(int) + shines.size() * (sizeof(short) * 2);
	char* cData = new char[bufferSize];
	auto cp = cData;

	Push(cp, (u32)NOT_PLAYERS_SHINES);
	Push(cp, (u16)0);
	Push(cp, (int)shines.size());

	for (const auto& s : shines)
	{
		Push(cp, s.posX);
		Push(cp, s.posY);
	}

	if (p->m_pXSock) p->m_pXSock->iSendMsg(cData, cp - cData);

	delete[] cData;
}

#include "../Header/Game.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <vector>
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

const int registration_time = 300;
const int freeze_time = 5;
const int safe_time = 30;
const int max_rountime = 300;
static int center_x = 95; //center x position for radius
static int center_y = 104; //center y position for radius
static int min_players = 2;

void cCoreServer::enableBattleRoyaleEvent()
{
	m_iMapIndexBr = calculateMapIndexBattleRoyale();
	m_iElapsedRegistrationTime = 0;
	m_iElapsedFreezeTime = 0;
	m_iElapsedSafeTime = 0;
	m_iElapsedRoundTime = 0;
	m_sCurrentRound = 0;
	m_sMaxRounds = 12;
	m_radiusInitializedBr = false;
	m_radiusBr = 0.0f;	

	changeBattleRoyaleStatus(BATTLEROYALE_REGISTRATION_TIME);
	notifyBattleRoyaleStatus();
	
	InitializeRadiusBattleRoyale();

	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		auto pi = m_pClientList[i];
		if (!pi) continue;

		notifyRadiusBattleRoyale(i);
	}

	InitializeCoordinateBattleRoyale();

	// Inicialización de _randomitemcord
	_randomitemcord = {
		{ 32, 105 }, { 51, 95 }, { 63, 82 }, { 81, 69 }, { 101, 72 }, { 129, 75 }, { 155, 95 },
		{ 148, 110 }, { 128, 103 }, { 131, 112 }, { 145, 126 }, { 115, 122 }, { 108, 110 },
		{ 107, 135 }, { 83, 137 }, { 95, 118 }, { 85, 111 }, { 71, 129 }, { 53, 126 }, { 84, 95 },
		{ 97, 91 }
	};

	// Inicialización de _items
	_items = {
		{ 1780 },  // DeathWand(MS.30)
		{ 861 },  // BerserkWand(MS.30)
		{ 2090 }, // PowerHeroWand
		{ 863 },  // KlonessWand(MS.30)
		{ 848 },  // LightingBlade
		{ 849 },  // KlonessBlade
		{ 850 },  // KlonessAxe
		{ 762 },  // GiantBattleHammer
		{ 846 },  // The_Devastator
		{ 845 },  // StormBringer
		{ 861 },  // BerserkWand(MS.30)

		{ 861 },  // BerserkWand(MS.30)
		{ 2090 }, // PowerHeroWand
		{ 846 },  // The_Devastator
		{ 863 },  // KlonessWand(MS.30)
		{ 848 },  // LightingBlade
		{ 849 },  // KlonessBlade
		{ 850 },  // KlonessAxe
		{ 762 },  // GiantBattleHammer
		{ 846 },  // The_Devastator
		{ 845 },  // StormBringer
	};

	SendTextMessageToAllBattleRoyale("Battle Royale Event Enabled!");
}

void cCoreServer::InitializeCoordinateBattleRoyale() {
	m_coordinatesBr = {
		{ 40, 100 }, { 65, 84 }, { 91, 69 }, { 109, 73 }, { 129, 81 },
		{ 148, 103 }, { 123, 117 }, { 97, 134 }, { 58, 122 }, { 81, 99 },
		{ 92, 111 }, { 111, 100 }, { 95, 95 }, { 95, 102 }, { 134, 122 }
	};

	m_coordinateIndicesBr.resize(m_coordinatesBr.size());
	std::iota(m_coordinateIndicesBr.begin(), m_coordinateIndicesBr.end(), 0);
	ShuffleCoordinatesBattleRoyale();
	m_currentIndexBr = 0;
}

void cCoreServer::ShuffleCoordinatesBattleRoyale() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(m_coordinateIndicesBr.begin(), m_coordinateIndicesBr.end(), g);
}

void cCoreServer::handleBattleRoyale()
{
	if (!g_ev.Is(EventID::BattleRoyale)) return;

	notifyBattleRoyaleStatus();

	switch (m_sBattleRoyalStatus)
	{
	case BATTLEROYALE_REGISTRATION_TIME:
		handleEventOnRegistrationTime();
		break;
	case BATTLEROYALE_FREEZE_TIME:
		handleEventOnFreezeTime();
		break;
	case BATTLEROYALE_SAFE_TIME:
		handleEventOnSafeTime();
		break;
	case BATTLEROYALE_COMBAT:
		handleEventOnRound();
		break;
	default:
		break;
	}
}


void cCoreServer::changeBattleRoyaleStatus(short status)
{
	m_sBattleRoyalStatus = status;

	if (m_sBattleRoyalStatus == BATTLEROYALE_REGISTRATION_TIME)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo de registro ha comenzado.");
	}

	if (m_sBattleRoyalStatus == BATTLEROYALE_FREEZE_TIME)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo de congelamiento ha comenzado.");
	}

	if (m_sBattleRoyalStatus == BATTLEROYALE_SAFE_TIME)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo seguro ha comenzado.");
	}
}

void cCoreServer::handleEventOnRegistrationTime()
{
	m_iElapsedRegistrationTime++;
	int checktime = registration_time - m_iElapsedRegistrationTime;
	
	notifyBattleRoyaleRegisterTime(checktime);

	//Tiempo de registro terminado
	if (checktime == 0)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo de registro ha finalizado.");
		handleNewRoundTasksBattleRoyale();
		notifyBattleRoyaleStatus();		
	}
}


void cCoreServer::handleEventOnFreezeTime()
{
	m_iElapsedFreezeTime++;
	int checktime = freeze_time - m_iElapsedFreezeTime;

	notifyBattleRoyaleFreezeTime(checktime);

	//freeze terminado
	if (checktime == 0)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo de congelamiento ha finalizado.");
		changeBattleRoyaleStatus(BATTLEROYALE_SAFE_TIME);
		notifyBattleRoyaleStatus();
	}
}


void cCoreServer::handleEventOnSafeTime()
{
	m_iElapsedSafeTime++;
	int checktime = safe_time - m_iElapsedSafeTime;

	notifyBattleRoyaleSafeTime(checktime);

	//freeze terminado
	if (checktime == 0)
	{
		SendTextMessageToAllBattleRoyale("Battle Royale: El tiempo seguro ha finalizado.");
		changeBattleRoyaleStatus(BATTLEROYALE_COMBAT);
		notifyBattleRoyaleStatus();
	}
}


void cCoreServer::handleEventOnRound()
{
	m_iElapsedRoundTime++;

	monitorRoundDurationBattleRoyale();
	handleMapRadiusBattleRoyale();
}

void cCoreServer::getJoinInfoBattleRoyale(int client, char * data)
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

	join_battleroyale(client, combattype, angelic);
}

void cCoreServer::TeleportToArenaLoby(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int sX = 136; int sY = 167;

	int iRoll = RollDice(1, 5);
	switch (iRoll)
	{
	case 1:
		sX = 136; sY = 167;
		break;
	case 2:
		sX = 149; sY = 173;
		break;
	case 3:
		sX = 148; sY = 164;
		break;
	case 4:
		sX = 151; sY = 156;
		break;
	case 5:
		sX = 163; sY = 164;
		break;
	default:
		break;

	}

	RequestTeleportHandler(client, "2   ", "battle", sX, sY);
}

bool cCoreServer::is_brfighter(char * charname)
{
	for (int i = 0; i < vec_brfighters.size(); i++){
		if (strcmp(vec_brfighters[i].fighter, charname) == 0) return true;
	}

	return false;
}

void cCoreServer::add_brfighterBattleRoyale(char * charname)
{

	if (!g_ev.Is(EventID::BattleRoyale)) return;

	st_brfighters u;

	ZeroMemory(u.fighter, sizeof(u.fighter));
	strcpy(u.fighter, charname);
	u.kills = 0;
	u.lobykills = 0;

	vec_brfighters.push_back(u);
}

int cCoreServer::getbattleroyale_fighterindex(char * charname)
{
	int result = -1;
	for (int i = 0; i < vec_brfighters.size(); i++){
		if (strcmp(vec_brfighters[i].fighter, charname) == 0)
		{
			result = i;
			break;
		}
	}

	return result;
}

void cCoreServer::join_battleroyale(int client, short combattype, short angelic)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (angelic < 1 && angelic > 4) return;
	if (combattype < 1 || combattype > 2) return;

	if (!g_ev.Is(EventID::BattleRoyale)) return;
	if (p->IsInMap("battle")) return;

	RequestDismissPartyHandler(client);

	if (!is_brfighter(p->m_cCharName)) add_brfighterBattleRoyale(p->m_cCharName);

	int fighter_index = getbattleroyale_fighterindex(p->m_cCharName);
	if (fighter_index == -1) return;

	LocalSavePlayerData(client);

	if (p->m_iAdminUserLevel != 0){
		RequestTeleportHandler(client, "2   ", "battle", -1, -1);
		return;
	}
	//ShuffleCoordinates();

	if (m_sBattleRoyalStatus == BATTLEROYALE_COMBAT)
	{
		p->m_bInArenaLoby = true;
		TeleportToArenaLoby(client);
	}
	else
	{
		auto coords = m_coordinatesBr[m_coordinateIndicesBr[m_currentIndexBr]];
		m_currentIndexBr = (m_currentIndexBr + 1) % m_coordinateIndicesBr.size();
		RequestTeleportHandler(client, "2   ", "battle", coords.first, coords.second);
	}

	if (p->IsInMap("battle") && is_brfighter(p->m_cCharName))
	{
		handlePlayerForBattleRoyale(client, combattype);
		handleBattleAngelicActionsBattleRoyale(client);
		SavePlayerItemForBattleRoyale(client);

		vec_brfighters[fighter_index].combattype = combattype;
		vec_brfighters[fighter_index].client_index = client;
		vec_brfighters[fighter_index].angelic = angelic;

		handlePlayerItemsBattleRoyale(client, combattype);
		LearnBattleMagicsBattleRoyale(client);

		AutoSkill(client);

		if (vec_brfighters[fighter_index].combattype == 1)
		{
			p->m_iSuperAttackLeft = 5000;
			SendCriticals(client);
		}

		RemoveFlags(client);

		p->m_bIsSafeAttackMode = FALSE;
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
	}
}

void cCoreServer::handlePlayerItemsBattleRoyale(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->ClearBag();

	MakeItemsBattleRoyale(client, combattype);
	NotifyItemsBattleRoyale(client);
}

void cCoreServer::SavePlayerItemForBattleRoyale(int client)
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


void cCoreServer::restorePlayerPointsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsKilled = FALSE;
	p->m_iHP = iGetMaxHP(client);
	p->m_iMP = (((m_pClientList[client]->m_iMag + m_pClientList[client]->m_iAngelicMag) * 2) + (m_pClientList[client]->m_iLevel / 2)) +
		(m_pClientList[client]->m_iInt + m_pClientList[client]->m_iAngelicInt) / 2;
	p->m_iSP = ((m_pClientList[client]->m_iStr + m_pClientList[client]->m_iAngelicStr) * 2) + (m_pClientList[client]->m_iLevel / 2);
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
}

void cCoreServer::handlePlayerForBattleRoyale(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;

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
		vec_brbackmembers[member_index].iStr = p->m_iStr;
		vec_brbackmembers[member_index].iInt = p->m_iInt;
		vec_brbackmembers[member_index].iVit = p->m_iVit;
		vec_brbackmembers[member_index].iDex = p->m_iDex;
		vec_brbackmembers[member_index].iMag = p->m_iMag;
		vec_brbackmembers[member_index].iChar = p->m_iCharisma;
	}
	else
	{
		st_brbackmember u;

		ZeroMemory(u.m_cCharName, sizeof(u.m_cCharName));
		strcpy(u.m_cCharName, p->m_cCharName);
		u.iStr = p->m_iStr;
		u.iInt = p->m_iInt;
		u.iVit = p->m_iVit;
		u.iDex = p->m_iDex;
		u.iMag = p->m_iMag;
		u.iChar = p->m_iCharisma;

		vec_brbackmembers.push_back(u);
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

void cCoreServer::handleBattleAngelicActionsBattleRoyale(int client)
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

void cCoreServer::MakeItemsBattleRoyale(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!g_ev.Is(EventID::BattleRoyale)) return;
	if (!p->IsInMap("battle")) return;

	CreateSetsBattleRoyale(client, combattype);
	CreateWeaponsBattleRoyale(client, combattype);
	CreateCandysBattleRoyale(client);
}

void cCoreServer::CreateSetsBattleRoyale(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::BattleRoyale)) return;

	DWORD pa = 9405952;
	DWORD ma = 9410048;

	DWORD sp = 9001728;
	DWORD mp = 9005824;

	auto sex = p->m_cSex;
	short color = RollDice(1, 15);
	short color2 = RollDice(1, 15);

	char angelicname[22];
	int fighter_index = getbattleroyale_fighterindex(p->m_cCharName);
	if (fighter_index != -1)
	{
		switch (vec_brfighters[fighter_index].angelic)
		{
		case 1:
			CreateItemBattleRoyale(client, "AngelicPandent(STR)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 2:
			CreateItemBattleRoyale(client, "AngelicPandent(DEX)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 3:
			CreateItemBattleRoyale(client, "AngelicPandent(INT)", 4026531840, 9, 73, color, 1, false, true);
			break;
		case 4:
			CreateItemBattleRoyale(client, "AngelicPandent(MAG)", 4026531840, 9, 73, color, 1, false, true);
			break;
		default:
			break;
		}
	}

	if (combattype == 1)
	{
		CreateItemBattleRoyale(client, "Cape", sp, 9, 73, color2, 1, false, true);
		CreateItemBattleRoyale(client, "Shoes", sp, 9, 73, color2, 1, false, true);
		switch (sex)
		{
		case 1:
			CreateItemBattleRoyale(client, "Hauberk(M)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "PlateMail(M)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainHose(M)", pa, 9, 73, color, 1, false);
			CreateItemBattleRoyale(client, "Horned-Helm(M)", pa, 9, 73, color, 1, false);

			CreateItemBattleRoyale(client, "Hauberk(M)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "PlateMail(M)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainHose(M)", ma, 54, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "Horned-Helm(M)", ma, 54, 73, color, 1, false, true);
			break;

		case 2:
			CreateItemBattleRoyale(client, "Hauberk(W)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "PlateMail(W)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainHose(W)", pa, 9, 73, color, 1, false);
			CreateItemBattleRoyale(client, "Horned-Helm(W)", pa, 9, 73, color, 1, false);

			CreateItemBattleRoyale(client, "Hauberk(W)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "PlateMail(W)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainHose(W)", ma, 54, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "Horned-Helm(W)", ma, 54, 73, color, 1, false, true);
			break;
		}
	}
	else
	{
		CreateItemBattleRoyale(client, "Cape", mp, 9, 73, color2, 1, false, true);
		CreateItemBattleRoyale(client, "Shoes", mp, 9, 73, color2, 1, false, true);

		switch (sex)
		{
		case 1:
			CreateItemBattleRoyale(client, "Hauberk(M)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainMail(M)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainHose(M)", pa, 9, 73, color, 1, false);
			CreateItemBattleRoyale(client, "Wizard-Cap(M)", pa, 9, 73, color, 1, false);

			CreateItemBattleRoyale(client, "Hauberk(M)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainMail(M)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainHose(M)", ma, 54, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "Wizard-Cap(M)", ma, 54, 73, color, 1, false, true);
			break;

		case 2:
			CreateItemBattleRoyale(client, "Hauberk(W)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainMail(W)", pa, 9, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "ChainHose(W)", pa, 9, 73, color, 1, false);
			CreateItemBattleRoyale(client, "Wizard-Cap(W)", pa, 9, 73, color, 1, false);

			CreateItemBattleRoyale(client, "Hauberk(W)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainMail(W)", ma, 54, 73, color, 1, false);
			CreateItemBattleRoyale(client, "ChainHose(W)", ma, 54, 73, color, 1, false, true);
			CreateItemBattleRoyale(client, "Wizard-Cap(W)", ma, 54, 73, color, 1, false, true);
			break;
		}
	}
}

void cCoreServer::CreateItemBattleRoyale(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
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
	pItem->isbrweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isbrcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isbrcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isbrcandy = true;
	else pItem->isbrcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isbritem = true;
	pItem->isbrequip = equip;
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

//esta es para cuando revive 
void cCoreServer::CreateCandyBattleRoyale(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip)
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
	pItem->isbrweap = isweap;
	if (string(itemname) == "RedCandy") pItem->isbrcandy = true;
	else if (string(itemname) == "GreenCandy") pItem->isbrcandy = true;
	else if (string(itemname) == "BlueCandy") pItem->isbrcandy = true;
	else pItem->isbrcandy = false;
	pItem->m_dwAttribute = atr;
	pItem->m_dwCount = count;
	pItem->isbritem = true;
	pItem->isbrequip = equip;
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

void cCoreServer::CreateWeaponsBattleRoyale(int client, short combattype)
{
	auto p = m_pClientList[client];
	if (!p) return;
	if (!g_ev.Is(EventID::BattleRoyale)) return;

	short color = RollDice(1, 20);

	CreateItemBattleRoyale(client, "TowerShield", 8619264, 118, 22, color, 1, false);
	CreateItemBattleRoyale(client, "TowerShield", 8623360, 118, 22, color, 1, false);

	if (combattype == 1)
	{
		CreateItemBattleRoyale(client, "WizMagicWand(MS20)", 0, 170, 59, color, 1, true, false);
		CreateItemBattleRoyale(client, "NecklaceOfXelima", 0, 170, 89, color, 1, false, true);
		CreateItemBattleRoyale(client, "RingoftheAbaddon", 0, 170, 89, color, 1, false, true);
		CreateItemBattleRoyale(client, "DemonSlayer", 0, 92, 84, color, 1, true, true);
	}
	else
	{
		CreateItemBattleRoyale(client, "Rapier+1", 0, 170, 59, color, 1, true, false);
		CreateItemBattleRoyale(client, "NecklaceOfLiche", 0, 170, 89, color, 1, false, true);
		CreateItemBattleRoyale(client, "RingofArcmage", 0, 170, 89, color, 1, false, true);
		CreateItemBattleRoyale(client, "MagicWand(MS30-LLF)", 0, 92, 84, color, 1, true, true);
	}
}

void cCoreServer::CreateCandysBattleRoyale(int client)
{
	if (!m_pClientList[client]) return;

	CClient * p = m_pClientList[client];
	if (!p) return;

	CreateRedPotionsBattleRoyale(client);
	CreateBluePotionsBattleRoyale(client);
	CreateGreenPotionsBattleRoyale(client);
}

void cCoreServer::CreateBluePotionsBattleRoyale(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 11)
			break;
	}

	for (i = 0; i < rem; i++) CreateItemBattleRoyale(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::CreateRedPotionsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 17)
			break;
	}

	for (i = 0; i < rem; i++) CreateItemBattleRoyale(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::CreateGreenPotionsBattleRoyale(int client)
{
	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 4)
			break;
	}

	for (i = 0; i < rem; i++) CreateItemBattleRoyale(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

void cCoreServer::NotifyItemsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL)
		{
			p->m_bIsItemEquipped[i] = p->m_pItemList[i]->isbrequip;
			if (p->m_bIsItemEquipped[i]) bEquipItemHandler(client, i);
		}
	}

	LoadPlayerItems(client);
}

void cCoreServer::LearnBattleMagicsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
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


void cCoreServer::ClearMapBattleRoyale(string mapname) {

	short sRemainItemSprite, sRemainItemSpriteFrame, dX, dY;
	char cRemainItemColor;
	CItem *pItem;

	for (int i = 1; i < DEF_MAXMAPS; i++) {
		auto map = m_pMapList[i];
		if (!map) continue;
		if (string(map->m_cName) != mapname) continue;
		int m_x = map->m_sSizeX;
		int m_y = map->m_sSizeY;
		for (int j = 1; j < m_x; j++) {
			for (int k = 1; k < m_y; k++) {
				do {
					pItem = map->pGetItem(j, k, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
					if (pItem != NULL) {
						delete pItem;
					}
				} while (pItem != NULL);
			}
		}
	}
	return;
}

void cCoreServer::DropitemsBattleRoyale()
{
	std::vector<int> itemIndices(_items.size());
	std::iota(itemIndices.begin(), itemIndices.end(), 0);
	std::random_shuffle(itemIndices.begin(), itemIndices.end());

	int itemIndex = 0;

	for (int i = 0; i < 21; i++)
	{
		int itemId = _items[itemIndices[itemIndex]].item_id;
		handleItemDropBattleRoyale("battle", itemId, _randomitemcord[i].sx, _randomitemcord[i].sy);
		itemIndex = (itemIndex + 1) % _items.size();
	}
}

void cCoreServer::handleItemDropBattleRoyale(std::string map, int id, int x, int y)
{
	CMap * pmap = nullptr;
	for (auto * m : m_pMapList)
	{
		if (!m)
			continue;

		if (m->m_cName == map)
		{
			pmap = m;
		}
	}

	if (!pmap)
		return;

	CItem * it = new CItem;
	_bInitItemAttr(it, id);

	ItemDropBattleRoyale(pmap, id, x, y, 0, 0, it->m_cItemColor, true);
	delete it;
}


void cCoreServer::ItemDropBattleRoyale(CMap * map, int iItemID, int iX, int iY, int dwType, int dwValue, int cColor, bool isbattle) {
	class CItem * pItem;
	pItem = new class CItem;
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
	}

	else {
		if (dwType <= 10){

			pItem->m_dwAttribute = NULL;
			dwType = dwType << 20;
			dwValue = dwValue << 16;
			pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
		}
		else if (dwType > 10){

			dwType = dwType << 12;
			dwValue = dwValue << 8;
			pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
		}

		pItem->m_cItemColor = cColor;

		if (isbattle)
		{
			pItem->isbritem = true;
			pItem->isbrdropitem = true;
		}

		map->bSetItem(iX, iY, pItem);
		//pItem->m_dwTimeDrop = timeGetTime();
		SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_iMapIndexBr, iX, iY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor, pItem->m_dwAttribute);
	}
}

void cCoreServer::ReloadCandysBattleRoyale(int client)
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

	GetRedPotionsBattleRoyale(client);
	GetBluePotionsBattleRoyale(client);
	GetGreenPotionsBattleRoyale(client);
}

void cCoreServer::GetBluePotionsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 11)
			break;
	}

	for (i = 0; i < rem; i++) CreateCandyBattleRoyale(client, "BlueCandy", 0, 31, 17, 0, 1, false);
}

void cCoreServer::GetRedPotionsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 17)
			break;
	}

	for (i = 0; i < rem; i++) CreateCandyBattleRoyale(client, "RedCandy", 0, 0, 17, 0, 1, false);
}

void cCoreServer::GetGreenPotionsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int rem = 0, i = 0;

	for (i = 0; i < DEF_MAXITEMS; i++){
		if (!m_pClientList[client]->m_pItemList[i])
			rem++;

		if (rem >= 4)
			break;
	}

	for (i = 0; i < rem; i++) CreateCandyBattleRoyale(client, "GreenCandy", 0, 62, 17, 0, 1, false);
}

void cCoreServer::UpdatePlayersWithinRadiusToAllBattleRoyale()
{
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->IsInMap("battle")) continue;
		if (pi->m_bInArenaLoby) continue;
		if (pi->m_iAdminUserLevel != 0) continue;

		int dx = pi->m_sX - center_x;
		int dy = pi->m_sY - center_y;
		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance > m_radiusBr)
		{
			int dado = RollDice(50, 200);
			pi->m_iHP -= dado;
			wsprintf(G_cTxt, "Cuidado, estas fuera de los limites. Perdiste %d de HP", dado);
			SendTextMessageBattleRoyale(i, G_cTxt);
			SendNotifyMsg(NULL, i, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);

			if (pi->m_iHP <= 0)
			{
				wsprintf(G_cTxt, "Has sido eliminado por estar fuera de los limites.");
				SendTextMessageBattleRoyale(i, G_cTxt);
				wsprintf(G_cTxt, "El jugador %s fue eliminado por el evento.", pi->m_cCharName);
				SendTextMessageToAllBattleRoyale(G_cTxt);

				EventKilledHandler(i);
			}
		}
	}
}

void cCoreServer::UpdatePlayersWithinRadiusBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p || !p->IsInMap("battle")) return;
	if (p->m_bInArenaLoby) return;
	if (p->m_iAdminUserLevel != 0) return;

	int dx = p->m_sX - center_x;
	int dy = p->m_sY - center_y;
	float distance = std::sqrt(dx * dx + dy * dy);

	if (distance > m_radiusBr)
	{
		int dado = RollDice(50, 200);
		p->m_iHP -= dado;
		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);

		wsprintf(G_cTxt, "Cuidado, estas fuera de los limites. Perdiste %d de HP", dado);
		SendTextMessageBattleRoyale(client, G_cTxt);

		if (p->m_iHP <= 0)
		{
			wsprintf(G_cTxt, "Has sido eliminado por estar fuera de los limites.");
			SendTextMessageBattleRoyale(client, G_cTxt);

			wsprintf(G_cTxt, "El jugador %s fue eliminado por el evento.", p->m_cCharName);
			SendTextMessageToAllBattleRoyale(G_cTxt);

			for (int i = 0; i < DEF_MAXITEMS; i++)
			{
				CItem * item = p->m_pItemList[i];

				if (!item) continue;
				if (item->isbrdropitem)
				{
					DropItemHandler(client, i, -1, p->m_pItemList[i]->m_cName);
					break;
				}
			}

			EventKilledHandler(client);
		}
	}
}

void cCoreServer::SendTextMessageToAllBattleRoyale(char * message)
{
	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;

		ShowClientMsg(i, message);
	}
}

void cCoreServer::SendTextMessageBattleRoyale(int client, char * message)
{
	auto p = m_pClientList[client];
	if (!p) return;
	ShowClientMsg(client, message);
}

void cCoreServer::KillEnemyBattleRoyale(int attacker, int target)
{
	auto att = m_pClientList[attacker];
	if (!att) return;

	auto tar = m_pClientList[target];
	if (!tar) return;

	int attacker_index = getbattleroyale_fighterindex(att->m_cCharName);
	if (attacker_index == -1) return;

	if (att->m_bInArenaLoby)
	{
		vec_brfighters[attacker_index].lobykills++;
	}
	else
	{
		vec_brfighters[attacker_index].kills++;
	}

	handleKillActionBattleRoyale(attacker, target);
}

void cCoreServer::handleKillActionBattleRoyale(int att, int tar)
{
	auto attacker = m_pClientList[att];
	auto target = m_pClientList[tar];
	if (!attacker || !target) return;

	attacker->Send(att, CLIENT_NOTIFY_ENEMYKILLREWARD, tar);

	wsprintf(G_cTxt, "Has sido eliminado.");
	SendTextMessageBattleRoyale(tar, G_cTxt);

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem * item = target->m_pItemList[i];

		if (!item) continue;
		if (item->isbrdropitem)
		{
			DropItemHandler(tar, i, -1, target->m_pItemList[i]->m_cName);
			break;
		}
	}

	target->m_bInArenaLoby = true;

	if (checkIfLastPlayer() && g_ev.Is(EventID::BattleRoyale) && m_sBattleRoyalStatus == BATTLEROYALE_COMBAT)
	{
		RequestReviveInBattleRoyaleWithouthDialog(tar);
		BattleRoyale_ReviveNewRound(tar);

		int winnerClient = -1; int count = 0;
		for (int i = 0; i < DEF_MAXCLIENTS; i++) {
			auto pi = m_pClientList[i];
			if (pi && pi->IsInMap("battle") && pi->m_iAdminUserLevel == 0 && pi->m_bIsInitComplete && !pi->m_bInArenaLoby) {
				winnerClient = i;
				count++;
			}
		}

		if (winnerClient != -1 && count == 1) //fix 
		{
			auto winner = m_pClientList[winnerClient];
			wsprintf(G_cTxt, "Felicitaciones has ganado la ronda %d.", m_sCurrentRound);
			SendCommand(winnerClient, "/battlewinner");

			brwinners.push_back(winner->m_cCharName);
			SendTextMessageBattleRoyale(winnerClient, G_cTxt);

			wsprintf(G_cTxt, "%s ha ganado la ronda %d.", winner->m_cCharName, m_sCurrentRound);
			SendTextMessageToAllBattleRoyale(G_cTxt);
			handleNewRoundTasksBattleRoyale();
		}
		return;
	}

	RequestReviveInBattleRoyale(tar);
}

void cCoreServer::RequestReviveInBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
	SendCommand(client, "/revive");
}


void cCoreServer::RequestReviveInBattleRoyaleWithouthDialog(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = TRUE;
	p->requestrevive = true;
}

void cCoreServer::BattleRoyale_ReviveYes(int client)
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
	
	p->m_bInArenaLoby = true;
	TeleportToArenaLoby(client);	

	if (p->m_iAdminUserLevel == 0)
	{
		int fighter_index = getbattleroyale_fighterindex(p->m_cCharName);
		if (fighter_index != -1)
		{
			for (int i = 0; i < DEF_MAXITEMS; i++) ReqRepairItemCofirmHandler(client, i, NULL);

			ReloadCandysBattleRoyale(client);
			SendCommand(client, "/acomodarcandys");

			if (vec_brfighters[fighter_index].combattype == 1)
			{
				p->m_iSuperAttackLeft = 5000;
				SendCriticals(client);
			}

			calcItemsEffectsBattleRoyale(client);
		}
	}	
}


void cCoreServer::calcItemsEffectsBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
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
		

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		if (p->m_pItemList[i] != NULL)
		{
			if (p->m_pItemList[i]->isbrweap && !p->m_pItemList[i]->isbrequip)
			{
			}

			if (p->m_pItemList[i]->isbrweap && p->m_pItemList[i]->isbrequip)
			{
			}
		}
	}
}


void cCoreServer::BattleRoyale_ReviveNewRound(int client)
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

	if (p->m_iAdminUserLevel == 0)
	{
		int fighter_index = getbattleroyale_fighterindex(p->m_cCharName);
		if (fighter_index != -1)
		{
			for (int i = 0; i < DEF_MAXITEMS; i++) ReqRepairItemCofirmHandler(client, i, NULL);

			ReloadCandysBattleRoyale(client);
			SendCommand(client, "/acomodarcandys");

			if (vec_brfighters[fighter_index].combattype == 1)
			{
				p->m_iSuperAttackLeft = 5000;
				SendCriticals(client);
			}

			calcItemsEffectsBattleRoyale(client);
		}
	}
}

void cCoreServer::BattleRoyale_ReviveOnEndEvent(int client)
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


void cCoreServer::BattleRoyale_NotRevive(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	p->requestrevive = false;
	p->m_bIsKilled = FALSE;
	p->m_iHP = iGetMaxHP(client);
	p->m_iMP = p->GetMaxMP();
	p->m_iSP = p->GetMaxSP();
	p->m_bIsBeingResurrected = FALSE;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_HP, NULL, NULL, NULL, NULL);
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_SP, NULL, NULL, NULL, NULL);
	SendNotifyMsg(NULL, client, CLIENT_NOTIFY_MP, NULL, NULL, NULL, NULL);

	if (p->IsLocation("elvine"))
		RequestTeleportHandler(client, "2   ", "elvine", -1, -1);
	else
		RequestTeleportHandler(client, "2   ", "aresden", -1, -1);
}

void cCoreServer::EventKilledHandler(int client)
{
	if (m_pClientList[client] == NULL) return;
	if (m_pClientList[client]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[client]->m_bIsKilled == TRUE) return;

	m_pClientList[client]->m_bIsKilled = TRUE;
	m_pClientList[client]->m_iHP = 0;
	m_pClientList[client]->m_bInArenaLoby = true;

	if (checkIfLastPlayer() && g_ev.Is(EventID::BattleRoyale) && m_sBattleRoyalStatus == BATTLEROYALE_COMBAT)
	{
		RequestReviveInBattleRoyaleWithouthDialog(client);
		BattleRoyale_ReviveNewRound(client);

		int winnerClient = -1;
		for (int i = 0; i < DEF_MAXCLIENTS; i++) {
			auto pi = m_pClientList[i];
			if (pi && pi->IsInMap("battle") && pi->m_iAdminUserLevel == 0 && pi->m_bIsInitComplete && !pi->m_bInArenaLoby) {
				winnerClient = i;
				break;
			}
		}

		if (winnerClient != -1)
		{
			auto winner = m_pClientList[winnerClient];
			wsprintf(G_cTxt, "Felicitaciones has ganado la ronda %d.", m_sCurrentRound);
			SendCommand(winnerClient, "/battlewinner");

			brwinners.push_back(winner->m_cCharName);
			SendTextMessageBattleRoyale(winnerClient, G_cTxt);

			wsprintf(G_cTxt, "%s ha ganado la ronda %d.", winner->m_cCharName, m_sCurrentRound);
			SendTextMessageToAllBattleRoyale(G_cTxt);
			handleNewRoundTasksBattleRoyale();
		}
		return;
	}

	shinning_clear(client);

	if (m_pClientList[client]->IsInMap("battle"))
	{
		ServerKillEnemyBattleRoyale(client);
		m_pClientList[client]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = NULL;
		bRemoveFromDelayEventList(client, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
		RemoveFromTarget(client, DEF_OWNERTYPE_PLAYER);

		m_pMapList[m_pClientList[client]->m_cMapIndex]->ClearOwner(12, client, DEF_OWNERTYPE_PLAYER, m_pClientList[client]->m_sX, m_pClientList[client]->m_sY);
		m_pMapList[m_pClientList[client]->m_cMapIndex]->SetDeadOwner(client, DEF_OWNERTYPE_PLAYER, m_pClientList[client]->m_sX, m_pClientList[client]->m_sY);

		char cAttackerName[10];
		strcpy(cAttackerName, "Server");

		SendNotifyMsg(NULL, client, CLIENT_NOTIFY_KILLED, NULL, NULL, NULL, cAttackerName);
		return;
	}
}

void cCoreServer::ServerKillEnemyBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	handleServerKillActionBattleRoyale(client);
}

void cCoreServer::handleServerKillActionBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem * item = p->m_pItemList[i];

		if (!item) continue;
		if (item->isbrdropitem)
		{
			DropItemHandler(client, i, -1, p->m_pItemList[i]->m_cName);
			break;
		}
	}

	RequestReviveInBattleRoyale(client);
	p->m_bInArenaLoby = true;
}

bool cCoreServer::checkIfLastPlayer()
{
	int count = 0;
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		auto pi = m_pClientList[i];
		if (pi && pi->IsInMap("battle") && pi->m_iAdminUserLevel == 0 && !pi->m_bInArenaLoby) {
			count++;
		}
	}

	if (count == 1) return true; else return false;
}


void cCoreServer::handleNewRoundTasksBattleRoyale() 
{
	// Incrementar el contador de rondas
	m_sCurrentRound++;

	// Verificar si se alcanzó el número máximo de rondas
	if (m_sCurrentRound > m_sMaxRounds) {
		EndEventBattleRoyale();
		return;
	}

	// Iniciar la nueva ronda
	onBattleRoyaleRoundStarted();
}

void cCoreServer::RestartRoundBattleRoyale()
{
	m_iElapsedFreezeTime = 0;
	m_iElapsedSafeTime = 0;
	m_iElapsedRoundTime = 0;
	m_radiusInitializedBr = false;
	m_radiusBr = 0.0f;
	changeBattleRoyaleStatus(BATTLEROYALE_FREEZE_TIME);
	notifyBattleRoyaleStatus();
}

void cCoreServer::EndEventBattleRoyale()
{
	wsprintf(G_cTxt, "Evento Battle Royale finalizado.");

	SendTextMessageToAllBattleRoyale(G_cTxt);
	if (g_ev.Is(EventID::BattleRoyale)) g_ev.Deactivate(EventID::BattleRoyale);
}

void cCoreServer::InitializeRadiusBattleRoyale()
{
	if (m_sBattleRoyalStatus == BATTLEROYALE_COMBAT) return;

	if (m_pMapList[m_iMapIndexBr] != NULL)
	{
		if (strcmp(m_pMapList[m_iMapIndexBr]->m_cName, "battle") == 0)
		{
			const int mapWidth = m_pMapList[m_iMapIndexBr]->m_sSizeX;
			const int mapHeight = m_pMapList[m_iMapIndexBr]->m_sSizeY;

			const int centerX = center_x;
			const int centerY = center_y;

			m_radiusBr = std::sqrt(std::pow(mapWidth - centerX, 2) + std::pow(mapHeight - centerY, 2));
			m_radiusInitializedBr = true;
			m_remainingIntervalsBr = max_rountime;
		}
	}
}

void cCoreServer::onBattleRoyaleRoundStarted() {

	RestartRoundBattleRoyale();
	ClearMapBattleRoyale("battle");
	DropitemsBattleRoyale();
	InitializeRadiusBattleRoyale();
	ShuffleCoordinatesBattleRoyale();

	m_currentIndexBr = 0;

	int count = 0;
	for (int i = 0; i < DEF_MAXCLIENTS; i++) {
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->IsInMap("battle")) continue;

		RequestReviveInBattleRoyaleWithouthDialog(i);
		BattleRoyale_ReviveNewRound(i);

		for (int b = 0; b < DEF_MAXITEMS; b++)
		{
			auto item = pi->m_pItemList[b];
			if (!item) continue;

			if (item->isbrdropitem)
			{
				ItemDepleteHandler(i, b, FALSE);
				break;
			}
		}

		pi->m_bInArenaLoby = false;

		auto coords = m_coordinatesBr[m_coordinateIndicesBr[m_currentIndexBr]];
		m_currentIndexBr = (m_currentIndexBr + 1) % m_coordinateIndicesBr.size();
		RequestTeleportHandler(i, "2   ", "battle", coords.first, coords.second);

		SendCommand(i, "/closerevivepanel");

		notifyCurrentRound(i); // Notificar al jugador sobre la nueva ronda
		// Restaurar los items del jugador
		int fighter_index = getbattleroyale_fighterindex(pi->m_cCharName);
		if (fighter_index != -1)
		{
			if (vec_brfighters[fighter_index].combattype == 1)
			{
				pi->m_iSuperAttackLeft = 5000;
				SendCriticals(i);
			}

			RemoveFlags(i);

			pi->m_bIsSafeAttackMode = FALSE;
			SendNotifyMsg(NULL, i, CLIENT_NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
		}

		count++;
	}

	if (count < min_players)
	{
		wsprintf(G_cTxt, "El evento termino porque no habia suficientes jugadores.");
		SendTextMessageToAllBattleRoyale(G_cTxt);
		EndEventBattleRoyale();
		return;
	}

	SendTextMessageToAllBattleRoyale("¡Preparate! La ronda comenzará en 5 segundos.");	
}

void cCoreServer::handleMapRadiusBattleRoyale()
{
	if (m_sBattleRoyalStatus == BATTLEROYALE_COMBAT)
	{
		if (m_pMapList[m_iMapIndexBr] != NULL)
		{
			if (strcmp(m_pMapList[m_iMapIndexBr]->m_cName, "battle") == 0)
			{
				if (!m_radiusInitializedBr)
				{
					InitializeRadiusBattleRoyale();
				}

				if (m_remainingIntervalsBr > 0)
				{
					float reductionFactor = m_radiusBr / m_remainingIntervalsBr;
					m_radiusBr -= reductionFactor;

					for (int i = 0; i < DEF_MAXCLIENTS; i++) {
						auto pi = m_pClientList[i];
						if (!pi) continue;
						if (!pi->IsInMap("battle")) continue;
						/*if (pi->m_bInArenaLoby) continue;*/

						notifyRadiusBattleRoyale(i);
					}

					if (m_radiusBr < 0.0f)
					{
						m_radiusBr = 0.0f;
					}

					m_remainingIntervalsBr--;
					UpdatePlayersWithinRadiusToAllBattleRoyale();
				}
			}
		}
	}
}

void cCoreServer::notifyRadiusBattleRoyale(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[50];
	auto * cp = data;

	Push(cp, (u32)NOT_BR_RADIUS);
	Push(cp, (u16)0);

	Push(cp, m_radiusBr);

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}


void cCoreServer::notifyBattleRoyaleRegisterTime(int time)
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_REGISTERTIME);
	Push(cp, (u16)0);

	Push(cp, time);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (pi->m_pXSock) pi->m_pXSock->iSendMsg(data, cp - data);
	}
}

void cCoreServer::notifyBattleRoyaleStatus()
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_BATTLEROYALESTATUS);
	Push(cp, (u16)0);

	Push(cp, m_sBattleRoyalStatus);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (pi->m_pXSock) pi->m_pXSock->iSendMsg(data, cp - data);
	}
}


void cCoreServer::notifyBattleRoyaleFreezeTime(int time)
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_FREEZETIME);
	Push(cp, (u16)0);

	Push(cp, time);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (pi->m_pXSock) pi->m_pXSock->iSendMsg(data, cp - data);
	}
}

void cCoreServer::notifyBattleRoyaleSafeTime(int time)
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_SAFETIME);
	Push(cp, (u16)0);

	Push(cp, time);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (pi->m_pXSock) pi->m_pXSock->iSendMsg(data, cp - data);
	}
}

void cCoreServer::notifyRoundTime(int time)
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_ROUNDTIME);
	Push(cp, (u16)0);

	Push(cp, time);

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi) continue;
		if (!pi->m_bIsInitComplete) continue;
		if (pi->m_bLimbo) continue;

		if (pi->m_pXSock) pi->m_pXSock->iSendMsg(data, cp - data);
	}
}

void cCoreServer::notifyCurrentRound(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOT_BATTLEROYALEROUND);
	Push(cp, (u16)0);

	Push(cp, m_sCurrentRound);

	p->m_pXSock->iSendMsg(data, cp - data);

}

void cCoreServer::FinishBattleRoyaleEvent()
{
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		CClient* pi = m_pClientList[i];
		if (!pi) continue;
		if (pi->m_iAdminUserLevel != 0) continue;

		if (pi->IsInMap("battle"))
		{
			BattleRoyale_ReviveNewRound(i);
			BattleRoyale_ReviveOnEndEvent(i);

			pi->m_bInArenaLoby = false;
		}

		int fighter_index = getbattleroyale_fighterindex(pi->m_cCharName);
		if (fighter_index != -1)
		{
			int brkills = vec_brfighters[fighter_index].kills;
			int lobykills = vec_brfighters[fighter_index].lobykills;

			int eventpointsrew = brkills; // 1 BR kill = 1 EventPoint
			int eksBR = brkills * 5;       // 1 BR kill = 5 EK
			int eksArena = lobykills * 1;  // 1 Arena kill = 1 EK

			int totalEks = eksBR + eksArena;

			if (eventpointsrew > 0)
			{
				pi->m_iEventPoints += eventpointsrew;
				SendCommand(i, "/eventpoints", pi->m_iEventPoints);
			}

			if (totalEks > 0)
			{
				pi->m_iEnemyKillCount += totalEks;
				SendCommand(i, "/eks", pi->m_iEnemyKillCount);

				char G_cTxt[256];
				wsprintf(G_cTxt, "Recibiste %d EKs y %d EventPoints.", totalEks, eventpointsrew);
				ShowClientMsg(i, G_cTxt);
			}
		}
	}

	std::unordered_map<std::string, int> winnerCounts;
	for (size_t j = 0; j < brwinners.size(); j++)
	{
		const std::string& winnerName = brwinners[j];
		winnerCounts[winnerName]++;
	}

	for (auto it = winnerCounts.begin(); it != winnerCounts.end(); ++it)
	{
		const std::string& winnerName = it->first;
		int winCount = it->second;

		for (int i = 1; i < DEF_MAXCLIENTS; i++)
		{
			CClient* pi = m_pClientList[i];
			if (!pi) continue;
			if (pi->m_iAdminUserLevel != 0) continue;

			if (strcmp(pi->m_cCharName, winnerName.c_str()) == 0)
			{
				int iwinreward = 5;
				int totalReward = iwinreward * winCount;
				pi->m_iTokens += totalReward;
				SendCommand(i, "/tokens", pi->m_iTokens);
				char G_cTxt[256];
				wsprintf(G_cTxt, "Ganaste %d tokens por %d rondas en BattleRoyale.", totalReward, winCount);
				ShowClientMsg(i, G_cTxt);
				break;
			}
		}
	}

	g_lastBattleRoyaleTime = std::chrono::steady_clock::now();
	brwinners.clear();
	vec_brfighters.clear();
}

void cCoreServer::monitorRoundDurationBattleRoyale()
{	
	int checktime = max_rountime - m_iElapsedRoundTime;
	if (m_sBattleRoyalStatus == BATTLEROYALE_COMBAT)
	{
		int alivePlayers = 0;
		int winnerClient = -1;

		for (int i = 0; i < DEF_MAXCLIENTS; i++) {
			auto pi = m_pClientList[i];
			if (!pi) continue;
			if (pi->m_iAdminUserLevel != 0) continue;
			if (!pi->IsInMap("battle")) continue;
			UpdatePlayersWithinRadiusBattleRoyale(i);
			if (!pi->m_bInArenaLoby) {
				alivePlayers++;
				winnerClient = i;
			}
		}

		if (alivePlayers == 1 && winnerClient != -1) {
			auto winner = m_pClientList[winnerClient];
			if (winner) {
				wsprintf(G_cTxt, "Felicitaciones, has ganado la ronda %d.", winner->m_cCharName, m_sCurrentRound);
				SendTextMessageBattleRoyale(winnerClient, G_cTxt);

				SendCommand(winnerClient, "/battlewinner");
				// Agregar al jugador ganador al vector brwinners
				brwinners.push_back(winner->m_cCharName);

				// Iniciar una nueva ronda
				handleNewRoundTasksBattleRoyale();
				return;
			}
		}

		notifyRoundTime(checktime);

		if (checktime == 0)
		{
			// Verificar si algún jugador está en la posición (95, 104)
			int winnerClient = -1;

			for (int i = 0; i < DEF_MAXCLIENTS; i++) {
				auto pi = m_pClientList[i];
				if (!pi) continue;
				if (pi->m_iAdminUserLevel != 0) continue;
				if (!pi->IsInMap("battle")) continue;

				if (!pi->m_bInArenaLoby) {
					if (pi->m_sX == 95 && pi->m_sY == 104) {
						winnerClient = i;
						break;
					}
				}
			}

			if (winnerClient != -1) {
				auto winner = m_pClientList[winnerClient];
				if (winner) {
					wsprintf(G_cTxt, "Has ganado la ronda por estar en la posición (95, 104).", winner->m_cCharName, m_sCurrentRound);
					SendTextMessageBattleRoyale(winnerClient, G_cTxt);

					wsprintf(G_cTxt, "%s gano la ronda %d por estar en la posición (95, 104).", winner->m_cCharName, m_sCurrentRound);
					SendTextMessageToAllBattleRoyale(G_cTxt);

					SendCommand(winnerClient, "/battlewinner");
					// Agregar al jugador ganador al vector brwinners
					brwinners.push_back(winner->m_cCharName);
				}
			}
			else {
				SendTextMessageToAllBattleRoyale("La ronda ha terminado sin ganador.");
			}

			// Iniciar una nueva ronda
			handleNewRoundTasksBattleRoyale();
		}
	}		
}

int cCoreServer::calculateMapIndexBattleRoyale()
{
	int result = -1;

	for (int i = 0; i < DEF_MAXMAPS; i++) {
		if (m_pMapList[i] != NULL) {
			if (strcmp(m_pMapList[i]->m_cName, "battle") == 0)
			{
				result = i;
				break;
			}
		}
	}

	return result;
}
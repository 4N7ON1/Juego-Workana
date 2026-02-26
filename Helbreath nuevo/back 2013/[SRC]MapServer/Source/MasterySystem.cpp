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

void cCoreServer::SetActiveMastery(int client, short mastery)
{
	return;

	auto player = m_pClientList[client];
	if (!player) return;

	if (mastery == player->m_sCurrentMastery)
	{
		ShowClientMsg(client, "You can't activate same mastery that you have active.");
		return;
	}

	if (mastery >= MasteryNone && mastery <= MasteryDefense)
	{
		short* currentLevel = nullptr;
		const char* masteryName = nullptr;

		switch (mastery)
		{
		case MasteryNpcDamage:
			currentLevel = &player->m_sNpcDamageLevel;
			masteryName = "NPC Damage";
			break;
		case MasteryRegen:
			currentLevel = &player->m_sRegenLevel;
			masteryName = "Regeneration";
			break;
		case MasteryVit:
			currentLevel = &player->m_sVitLevel;
			masteryName = "Vitality";
			break;
		case MasteryDefense:
			currentLevel = &player->m_sDefenseLevel;
			masteryName = "Defense";
			break;
		default:
			player->m_sCurrentMastery = MasteryNone;
			return;
		}

		if (*currentLevel > 0)
		{
			const int activationCost = 50;

			if (player->m_iTokens >= activationCost)
			{
				player->m_iTokens -= activationCost;

				player->m_sCurrentMastery = mastery;

				char msg[128];
				_snprintf(msg, sizeof(msg), "You have activated %s Mastery for 50 tokens!", masteryName);
				ShowClientMsg(client, msg);

				notifyMasteryData(client);
				send_objects_data();
				LocalSavePlayerData(client);
			}
			else
			{
				char msg[128];
				_snprintf(msg, sizeof(msg), "You do not have enough Tokens to activate %s.", masteryName);
				ShowClientMsg(client, msg);
			}
		}
		else
		{
			char msg[128];
			_snprintf(msg, sizeof(msg), "You cannot activate %s because its level is 0.", masteryName);
			ShowClientMsg(client, msg);
		}
	}
	else
	{
		ShowClientMsg(client, "Could not activate the selected Mastery.");
	}
}

void cCoreServer::IncreaseMasteryLevel(int client, short mastery)
{
	return;

	auto player = m_pClientList[client];
	if (!player) return;

	if (mastery > MasteryNone && mastery <= MasteryDefense)
	{
		short* currentLevel = nullptr;
		const char* masteryName = nullptr;

		switch (mastery)
		{
		case MasteryNpcDamage:
			currentLevel = &player->m_sNpcDamageLevel;
			masteryName = "NPC Damage";
			break;
		case MasteryRegen:
			currentLevel = &player->m_sRegenLevel;
			masteryName = "Regeneration";
			break;
		case MasteryVit:
			currentLevel = &player->m_sVitLevel;
			masteryName = "Vitality";
			break;
		case MasteryDefense:
			currentLevel = &player->m_sDefenseLevel;
			masteryName = "Defense";
			break;
		default:
			return;
		}

		int cost = 0;
		int calcost = (*currentLevel) + 1;
		cost = calcost * 200;

		if (player->m_iTokens >= cost && (*currentLevel) < MAXMASTERYLEVEL)
		{
			player->m_iTokens -= cost;

			(*currentLevel)++;

			char msg[128];
			_snprintf(msg, sizeof(msg), "You have increased %s to level %d!", masteryName, *currentLevel);
			ShowClientMsg(client, msg);

			notifyMasteryData(client);
			send_objects_data();
			LocalSavePlayerData(client);
		}
		else if ((*currentLevel) >= MAXMASTERYLEVEL)
		{
			char msg[128];
			_snprintf(msg, sizeof(msg), "You have reached the maximum level of %s!", masteryName);
			ShowClientMsg(client, msg);
		}
		else
		{
			char msg[128];
			_snprintf(msg, sizeof(msg), "You do not have enough Tokens to increase %s.", masteryName);
			ShowClientMsg(client, msg);
		}
	}
}

void cCoreServer::notifyMasteryData(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[30];
	char* cp = data;

	Push(cp, (u32)NOTIFY_MASTERYDATA);
	Push(cp, (u16)0);

	Push(cp, p->m_sCurrentMastery);
	Push(cp, p->m_sNpcDamageLevel);
	Push(cp, p->m_sRegenLevel);
	Push(cp, p->m_sVitLevel);
	Push(cp, p->m_sDefenseLevel);
	Push(cp, p->m_iTokens);

	m_pClientList[client]->m_pXSock->iSendMsg(data, sizeof(data));
}

void cCoreServer::reqUpgradeMastery(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	short option;
	
	Pop(data, option);
	
	IncreaseMasteryLevel(client, option);
}


void cCoreServer::reqActivateMastery(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	short option;

	Pop(data, option);

	SetActiveMastery(client, option);
}
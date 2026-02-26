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

string cCoreServer::getekquestvalue(int client, string val)
{
	auto p = m_pClientList[client];
	if (!p) return "";

	char cFileName[112] = {};
	char cDir[112] = {};

	strcat(cFileName, "Database\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "EkQuest");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", p->m_cCharName[0]);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, p->m_cCharName);
	strcat(cFileName, ".cfg");

	string result = get_line(cFileName, val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

void cCoreServer::read_ekquest_data(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->vec_charekquest.clear();

	for (int i = 0; i < DEF_MAXCHARQUESTS; i++)
	{
		char temp[64];
		wsprintf(temp, "ekquest-num-%d = ", i + 1);

		std::string token = getekquestvalue(client, temp);
		if (token == "#" || token.empty()) continue;

		std::vector<std::string> out;
		tokenize(token, " ", out);

		if (out.size() < 7) continue; // Se requieren 7 campos exactos

		CharacterEk_Quests q;
		q.m_iParentQuestID = atoi(out[0].c_str());

		ZeroMemory(q.m_cMapName, sizeof(q.m_cMapName));
		strncpy_s(q.m_cMapName, out[1].c_str(), sizeof(q.m_cMapName) - 1);

		ZeroMemory(q.m_cShowMapName, sizeof(q.m_cShowMapName));
		strncpy_s(q.m_cShowMapName, out[2].c_str(), sizeof(q.m_cShowMapName) - 1);

		ZeroMemory(q.m_cRewardName, sizeof(q.m_cRewardName));
		strncpy_s(q.m_cRewardName, out[3].c_str(), sizeof(q.m_cRewardName) - 1);

		q.m_iKills = atoi(out[4].c_str());
		q.m_iMaxKills = atoi(out[5].c_str());
		q.status = atoi(out[6].c_str()) != 0;
		q.m_sCompleted = atoi(out[7].c_str());

		p->vec_charekquest.push_back(q);
	}

	send_charkillquests(client);
}


string cCoreServer::getvalue(int client, string val)
{
	auto p = m_pClientList[client];
	if (!p) return "";

	char cFileName[112] = {};
	char cDir[112] = {};

	strcat(cFileName, "Database\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "Quest");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", p->m_cCharName[0]);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, p->m_cCharName);
	strcat(cFileName, ".cfg");

	string result = get_line(cFileName, val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

void cCoreServer::read_quest_data(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->vec_charquest.clear();
	for (int i = 0; i < DEF_MAXCHARQUESTS; i++)
	{
		char temp[512];
		wsprintf(temp, "quest-num-%d = ", i + 1);
		string token = getvalue(client, temp);
		if (string(token) == "#") continue;
		const char* delim = " ";
		vector<string> out;
		tokenize(token, delim, out);

		st_charquest u;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				u.m_sParentQuestID = atoi((char*)token.c_str());
				break;

			case 2:
				ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
				strcpy(u.m_cNpcName, (char*)token.c_str());
				break;

			case 3:
				ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
				strcpy(u.m_cMapName, (char*)token.c_str());
				break;

			case 4:
				ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
				strcpy(u.m_cRealMapName, (char*)token.c_str());
				break;

			case 5:
				u.m_iKills = atoi((char*)token.c_str());
				break;

			case 6:

				u.m_iMaxKills = atoi((char*)token.c_str());
				break;

			case 7:
				u.m_iRewardContribution = atoi((char*)token.c_str());
				break;

			case 8:
				ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
				strcpy(u.m_cQuestReward1, (char*)token.c_str());
				break;

			case 9:
				ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
				strcpy(u.m_cQuestReward2, (char*)token.c_str());
				break;

			case 10:
				ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
				strcpy(u.m_cQuestReward3, (char*)token.c_str());
				break;

			case 11:
				u.m_sStatus = atoi((char*)token.c_str());
				break;

			case 12:
				u.m_sCompleted = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		if (string(u.m_cMapName) == "areuni") continue;
		if (string(u.m_cMapName) == "Middleland") continue;
		p->vec_charquest.push_back(u);
	}

	send_charquest_data(client);
}

void cCoreServer::send_charquest_data(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[4096];
	auto * cp = data;
	u32 msgid = -1;
	u16 nulled = 0;
	short iSize = 0;

	msgid = NOT_CHARQUESTS;
	iSize = p->vec_charquest.size();

	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, iSize);

	for (int i = 0; i < iSize; i++)
	{
		Push(cp, p->vec_charquest[i].m_sParentQuestID);
		Push(cp, p->vec_charquest[i].m_cNpcName, 21);
		Push(cp, p->vec_charquest[i].m_cMapName, 12);
		Push(cp, p->vec_charquest[i].m_cRealMapName, 12);
		Push(cp, p->vec_charquest[i].m_iKills);
		Push(cp, p->vec_charquest[i].m_iMaxKills);
		Push(cp, p->vec_charquest[i].m_iRewardContribution);
		Push(cp, p->vec_charquest[i].m_cQuestReward1, 21);
		Push(cp, p->vec_charquest[i].m_cQuestReward2, 21);
		Push(cp, p->vec_charquest[i].m_cQuestReward3, 21);
		Push(cp, p->vec_charquest[i].m_sStatus);
		Push(cp, p->vec_charquest[i].m_sCompleted);
	}

	if (p->m_pXSock) p->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::req_dismiss_quest(int client, char * data)
{
	int index;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, index);

	if (index > p->vec_charquest.size() || index < 0) return;

	remove_quest(client, index);
	ShowClientMsg(client, "The quest was successfully removed.");
	send_charquest_data(client);
}

void cCoreServer::req_pause_quest(int client, char * data)
{
	int index;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, index);
	pause_quest(client, index);
	ShowClientMsg(client, "The quest was successfully paused.");
	send_charquest_data(client);
}

void cCoreServer::req_active_quest(int client, char * data)
{
	int index;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, index);

	active_quest(client, index);
}

void cCoreServer::active_quest(int client, int index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (index > p->vec_charquest.size() || index < 0) return;

	for (int i = 0; i < p->vec_charquest.size(); i++)
	{
		if (i == index) continue;
		p->vec_charquest[i].m_sStatus = 0;
	}

	p->vec_charquest[index].m_sStatus = 1;
	ShowClientMsg(client, "The quest was successfully activated.");
	send_charquest_data(client);
}

void cCoreServer::req_reward_quest(int client, char * data)
{
	int index;
	short selected;
	auto p = m_pClientList[client];
	if (!p) return;

	Pop(data, index);
	Pop(data, selected);
	reward_quest(client, index, selected);
}

static void deleteText(std::string& str, const std::string& toRemove) {
	size_t pos = str.find(toRemove);
	if (pos != std::string::npos) {
		str.erase(pos, toRemove.length());
	}
}

static bool startsWith(const std::string& str, const std::string& prefix) {
	return str.rfind(prefix, 0) == 0; // Verifica si `prefix` es el inicio de `str`
}

void cCoreServer::reward_quest(int client, int index, short rew)
{
	char tmp_rew[21];
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->vec_charquest.size() == 0) return;
	if (index >= p->vec_charquest.size() || index < 0) return;
	if (p->vec_charquest[index].m_sCompleted == 0) return;

	switch (rew)
	{
	case 1:
		strcpy(tmp_rew, p->vec_charquest[index].m_cQuestReward1);
		break;

	case 2:
		strcpy(tmp_rew, p->vec_charquest[index].m_cQuestReward2);
		break;

	case 3:
		strcpy(tmp_rew, p->vec_charquest[index].m_cQuestReward3);
		break;

	default: return; break;
	}

	if (string(tmp_rew) == "None") return;

	string reward = tmp_rew;
	string contrib_text = "Contrib+";
	string rep_text = "Rep+";
	string mastery_text = "Mastery+";
	string coin_text = "Coin+";
	string maj_text = "Majestic+";

	if (startsWith(reward, "PaFrags+")) {
		deleteText(reward, "PaFrags+");
		int cantidad = atoi(reward.c_str());
		p->fragment_pa += cantidad;
		SendCommand(client, "/fragment_pa", p->fragment_pa);
		wsprintf(G_cTxt, "Has recibido %d Fragmentos Pa.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "MaFrags+")) {
		deleteText(reward, "MaFrags+");
		int cantidad = atoi(reward.c_str());
		p->fragment_ma += cantidad;
		SendCommand(client, "/fragment_ma", p->fragment_ma);
		wsprintf(G_cTxt, "Has recibido %d Fragmentos Ma.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "MrFrags+")) {
		deleteText(reward, "MrFrags+");
		int cantidad = atoi(reward.c_str());
		p->fragment_mr += cantidad;
		SendCommand(client, "/fragment_mr", p->fragment_mr);
		wsprintf(G_cTxt, "Has recibido %d Fragmentos Mr.", cantidad);
		ShowClientMsg(client, G_cTxt);
		
		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "DrFrags+")) {
		deleteText(reward, "DrFrags+");
		int cantidad = atoi(reward.c_str());
		p->fragment_dr += cantidad;
		SendCommand(client, "/fragment_dr", p->fragment_dr);
		wsprintf(G_cTxt, "Has recibido %d Fragmentos Dr.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "HitProbFrags+")) {
		deleteText(reward, "HitProbFrags+");
		int cantidad = atoi(reward.c_str());
		p->fragment_hp += cantidad;
		SendCommand(client, "/fragment_hp", p->fragment_hp);
		wsprintf(G_cTxt, "Has recibido %d Fragmentos Hit Prob.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "Contribution+")) {
		deleteText(reward, "Contribution+");
		int cantidad = atoi(reward.c_str());
		p->m_iContribution += cantidad;
		SendCommand(client, "/contrib", p->m_iContribution);
		wsprintf(G_cTxt, "Has recibido %d de contribucion.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
	else if (startsWith(reward, "Tokens+")) {
		deleteText(reward, "Tokens+");
		int cantidad = atoi(reward.c_str());
		p->m_iTokens += cantidad;
		SendCommand(client, "/tokens", p->m_iTokens);
		wsprintf(G_cTxt, "Has recibido %d tokens.", cantidad);
		ShowClientMsg(client, G_cTxt);

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}

	else if (startsWith(reward, contrib_text))
	{
		deleteText(reward, contrib_text);
		int contrib = atoi(reward.c_str());

		if (contrib > 0) {
			wsprintf(G_cTxt, "You completed quest and received %d contribution.", contrib);
			ShowClientMsg(client, G_cTxt);
			p->m_iContribution += contrib;
			SendCommand(client, "/contrib", p->m_iContribution);

			remove_quest(client, index);
			send_charquest_data(client);
			save_quest_data(client);
		}
	}
	else if (startsWith(reward, maj_text))
	{
		deleteText(reward, maj_text);
		int majs = atoi(reward.c_str());

		if (majs > 0) {
			wsprintf(G_cTxt, "You completed quest and received %d majestics.", majs);
			ShowClientMsg(client, G_cTxt);
			p->m_iGizonItemUpgradeLeft += majs;
			SendCommand(client, "/majs", p->m_iGizonItemUpgradeLeft);

			remove_quest(client, index);
			send_charquest_data(client);
			save_quest_data(client);
		}
	}
	else if (startsWith(reward, coin_text))
	{
		deleteText(reward, coin_text);
		int coins = atoi(reward.c_str());

		if (coins > 0) {
			wsprintf(G_cTxt, "You completed quest and received %d coins.", coins);
			ShowClientMsg(client, G_cTxt);
			p->m_iCoins += coins;

			SendCommand(client, "/coins", p->m_iCoins);

			remove_quest(client, index);
			send_charquest_data(client);
			save_quest_data(client);
		}
	}
	else if (startsWith(reward, mastery_text))
	{
		deleteText(reward, mastery_text);
		int mastery = atoi(reward.c_str());

		if (mastery > 0) {
			wsprintf(G_cTxt, "You completed quest and received %d Tokens.", mastery);
			ShowClientMsg(client, G_cTxt);
			p->m_iTokens += mastery;
			/*SendCommand(client, "/rep", p->m_iRating);*/
			notifyMasteryData(client);

			remove_quest(client, index);
			send_charquest_data(client);
			save_quest_data(client);
		}
	}
	else if (startsWith(reward, rep_text))
	{
		deleteText(reward, rep_text);
		int rep = atoi(reward.c_str());

		if (rep > 0) {
			wsprintf(G_cTxt, "You completed quest and received %d reputation.", rep);
			ShowClientMsg(client, G_cTxt);
			p->m_iRating += rep;
			SendCommand(client, "/rep", p->m_iRating);

			remove_quest(client, index);
			send_charquest_data(client);
			save_quest_data(client);
		}
	}
	else
	{
		if (_iGetItemSpaceLeft(client) < 1)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			wsprintf(G_cTxt, "Error. Your inventory is full.", p->vec_charquest[index].m_iRewardContribution);
			ShowClientMsg(client, G_cTxt);
			return;
		}

		wsprintf(G_cTxt, "You have completed the quest and received %s.", (char*)reward.c_str());
		ShowClientMsg(client, G_cTxt);
		CreateQuestItem(client, (char*)reward.c_str());

		remove_quest(client, index);
		send_charquest_data(client);
		save_quest_data(client);
	}
}

void cCoreServer::pause_quest(int client, int index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (index > p->vec_charquest.size() || index < 0) return;

	p->vec_charquest[index].m_sStatus = 0;

	ShowClientMsg(client, "The quest was successfully activated.");
	send_charquest_data(client);
}

void cCoreServer::remove_quest(int client, int index)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (index > p->vec_charquest.size() || index < 0) return;

	p->vec_charquest.erase(p->vec_charquest.begin() + index);
}

void cCoreServer::CreateQuestItem(int client, char *  itemname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char   seps[] = "= \t\n";
	char   * cp, cItemName[256], cData[256], cTemp[256], cAttribute[256];
	SYSTEMTIME SysTime;
	class  CStrTok * pStrTok;
	class  CItem * pItem;
	short  * sp;
	int    iRet, iTemp, iEraseReq, iValue;
	DWORD * dwp;
	WORD  * wp, wTemp;
	double dV1, dV2, dV3;
	
	ZeroMemory(cItemName, sizeof(cItemName));
	strcpy(cItemName, itemname);

	ZeroMemory(cAttribute, sizeof(cAttribute));
	strcpy(cAttribute, std::to_string(0).c_str());

	pItem = new class CItem;

	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	iValue = 0;
	if (strlen(cAttribute) > 0 && strlen(cAttribute) <= 11) {
		pItem->m_dwAttribute = (DWORD)std::stoull(string(cAttribute));
		if (pItem->m_dwAttribute == 1) {
			if ((iValue >= 1) && (iValue <= 200)) {

				pItem->m_cItemColor = 2;
				pItem->m_sItemSpecEffectValue2 = iValue - 100;

				dV2 = (double)pItem->m_sItemSpecEffectValue2;
				dV3 = (double)pItem->m_wMaxLifeSpan;
				dV1 = (dV2 / 100.0f)*dV3;

				iTemp = (int)pItem->m_wMaxLifeSpan;
				iTemp += (int)dV1;

				if (iTemp <= 0)
					wTemp = 1;
				else wTemp = (WORD)iTemp;

				if (wTemp <= pItem->m_wMaxLifeSpan * 2) {

					pItem->m_wMaxLifeSpan = wTemp;
					pItem->m_sItemSpecEffectValue1 = (short)wTemp;
					pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
				}
				else pItem->m_sItemSpecEffectValue1 = (short)pItem->m_wMaxLifeSpan;

			}
			else pItem->m_dwAttribute = NULL;
		}
		else {

			if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
				switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
				case 6:	pItem->m_cItemColor = 2; break;
				case 8: pItem->m_cItemColor = 3; break;
				case 1:	pItem->m_cItemColor = 5; break;
				case 5:	pItem->m_cItemColor = 1; break;
				case 3:	pItem->m_cItemColor = 7; break;
				case 2:	pItem->m_cItemColor = 4; break;
				case 7:	pItem->m_cItemColor = 6; break;
				case 9:	pItem->m_cItemColor = 8; break;
				}
			}
			else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
				switch ((pItem->m_dwAttribute & 0xF00000) >> 20) {
				case 10: pItem->m_cItemColor = 5; break;
				}
			}
		}
	}

	switch (pItem->m_sIDnum) {
	case 511:
	case 513:
	case 515:
	case 517:
	case 530:
	case 531:
	case 532:
	case 533:
	case 534:
		GetLocalTime(&SysTime);
		pItem->m_sTouchEffectType = DEF_ITET_DATE;

		pItem->m_sTouchEffectValue1 = (short)SysTime.wMonth;
		pItem->m_sTouchEffectValue2 = (short)SysTime.wDay;
		pItem->m_sTouchEffectValue3 = 24;
		break;

	default:
		GetLocalTime(&SysTime);
		pItem->m_sTouchEffectType = DEF_ITET_ID;
		pItem->m_sTouchEffectValue1 = iDice(1, 100000);
		pItem->m_sTouchEffectValue2 = iDice(1, 100000);

		ZeroMemory(cTemp, sizeof(cTemp));
		wsprintf(cTemp, "%d%2d", (short)SysTime.wMonth, (short)SysTime.wDay);
		pItem->m_sTouchEffectValue3 = atoi(cTemp);
		break;
	}

	ZeroMemory(cData, sizeof(cData));

	if (_bAddClientItemList(client, pItem, &iEraseReq) == TRUE) {
		SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);

		if (iEraseReq == 1) delete pItem;
		return;
	}
	else {
		delete pItem;
		return;
	}
}

string cCoreServer::getDropValue(int client, string val)
{
	auto p = m_pClientList[client];
	if (!p) return "";

	char cFileName[112] = {};
	char cDir[112] = {};

	strcat(cFileName, "Database\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, "Drops");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", p->m_cCharName[0]);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, p->m_cCharName);
	strcat(cFileName, ".cfg");

	string result = get_line(cFileName, val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

void cCoreServer::read_drop_items(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	p->m_vDropItemList.clear();
	for (int i = 0; i < DEF_MAXDROPITEMS; i++)
	{
		char temp[512];
		wsprintf(temp, "drop-item-%d = ", i + 1);
		string token = getDropValue(client, temp);
		if (token == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenize(token, delim, out);

		st_dropitems u;

		if (out.size() >= 1) {
			strncpy(u.m_cItemName, out[0].c_str(), sizeof(u.m_cItemName) - 1);
		}
		if (out.size() >= 2) {
			u.itemcount = static_cast<DWORD>(stoul(out[1]));
		}
		if (out.size() >= 3) {
			u.attr = static_cast<DWORD>(stoul(out[2]));
		}

		p->m_vDropItemList.push_back(u);
	}
}


void cCoreServer::handleActivateRole(int client, short role)
{
	return;
	auto p = m_pClientList[client];
	if (!p) return;

	ShowClientMsg(client, "Por el momento no podes elegir este role.");
	return;

	if (p->IsInMap("gg") || p->IsInMap("dk") || p->IsInMap("dm")) return;

	if (p->m_iEnemyKillCount < 250)
	{
		ShowClientMsg(client, "Necesitas 250 eks para activar el role.");
		return;
	}

	if (p->m_sPlayerRole == role)
	{
		ShowClientMsg(client, "Error, ya tienes activado este role.");
		return;
	}

	if (role != ROLE_RAIDER && role != ROLE_HUNTER)
	{
		ShowClientMsg(client, "Ha ocurrido un error.");
		return;
	}

	p->m_iEnemyKillCount -= 250;	
	SendCommand(client, "/eks", p->m_iEnemyKillCount);

	switch (role)
	{
	case 1:
	{
		if (p->m_sRaiderLvl == 0)
		{
			ShowClientMsg(client, "Debes ser nivel 1 o mas para activar el role.");
			return;
		}

		p->m_sPlayerRole = ROLE_RAIDER;
		send_role_data(client);
		LocalSavePlayerData(client);
		ShowClientMsg(client, "Has activado Raider Role.");
		send_objects_data();
		break;
	}
	case 2:
	{
		if (p->m_sHunterLvl == 0)
		{
			ShowClientMsg(client, "Debes ser nivel 1 o mas para activar el role.");
			return;
		}

		p->m_sPlayerRole = ROLE_HUNTER;
		send_role_data(client);
		LocalSavePlayerData(client);
		ShowClientMsg(client, "Has activado Hunter Role.");
		send_objects_data();
		break;
	}

	default:
		ShowClientMsg(client, "Ha ocurrido un error.");
		return;
		break;
	}
}

void cCoreServer::handleUpgradeRole(int client, short role)
{
	auto p = m_pClientList[client];
	if (!p) return;

	ShowClientMsg(client, "Por el momento no podes upgradear este role.");
	return;

	if (p->IsInMap("gg") || p->IsInMap("dk") || p->IsInMap("dm")) return;

	if (role != ROLE_RAIDER && role != ROLE_HUNTER)
	{
		ShowClientMsg(client, "Ha ocurrido un error.");
		return;
	}

	if (role == ROLE_RAIDER)
	{
		if (p->m_sRaiderLvl == 2)
		{
			ShowClientMsg(client, "Por ahora el nivel esta limitado a 2.");
			return;
		}

		if (p->m_iEnemyKillCount < 15000)
		{
			ShowClientMsg(client, "Necesitas 15000 eks para subir el nivel.");
			return;
		}

		p->m_iEnemyKillCount -= 15000;
		SendCommand(client, "/eks", p->m_iEnemyKillCount);

		p->m_sRaiderLvl++;

		wsprintf(G_cTxt, "Has subido al nivel %d de Raider.", p->m_sRaiderLvl);
		ShowClientMsg(client, G_cTxt);
		send_role_data(client);
		LocalSavePlayerData(client);
		send_objects_data();
	}
	else if (role == ROLE_HUNTER)
	{
		if (p->m_sHunterLvl >= 3)
		{
			ShowClientMsg(client, "Por ahora el nivel está limitado a 3.");
			return;
		}

		int cost = (p->m_sHunterLvl + 1) * 5000;

		if (p->m_iEnemyKillCount < cost)
		{
			ShowClientMsg(client, "Necesitas más EKs para subir el nivel.");
			return;
		}

		p->m_iEnemyKillCount -= cost;
		SendCommand(client, "/eks", p->m_iEnemyKillCount);

		p->m_sHunterLvl++;

		char msg[64];
		sprintf_s(msg, "Has subido al nivel %d de Hunter.", p->m_sHunterLvl);
		ShowClientMsg(client, msg);

		send_role_data(client);
		LocalSavePlayerData(client);
		send_objects_data();
	}

	else
	{
		ShowClientMsg(client, "Ha ocurrido un error.");
	}
}

void cCoreServer::send_role_data(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[12];
	auto * cp = data;
	u32 msgid = -1;
	u16 nulled = 0;
	short iSize = 0;

	msgid = NOT_PLAYERROLEDATA;

	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, p->m_sPlayerRole);
	Push(cp, p->m_sRaiderLvl);
	Push(cp, p->m_sHunterLvl);

	if (p->m_pXSock) p->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::ClearBagGG(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		auto item = p->m_pItemList[i];

		if (!item) continue;
		ItemDepleteHandler(client, i, FALSE);
	}
}

void cCoreServer::ClearBagDK(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		auto item = p->m_pItemList[i];

		if (!item) continue;
		//if (!item->isgungameitem) continue;
		ItemDepleteHandler(client, i, FALSE);
	}
}

void cCoreServer::ClearBagBR(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		auto item = p->m_pItemList[i];

		if (!item) continue;
		if (!item->isbritem) continue;
		ItemDepleteHandler(client, i, FALSE);
	}
}

void cCoreServer::EnableItem(int client, i8 handle)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[56];
	char* cp = data;

	Push(cp, u32(NOTIFY_ENABLE_ITEM));
	Push(cp, handle);

	if (p->m_pXSock) p->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::send_charkillquests(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->vec_charekquest.empty()) { SendCommand(client, "/clearcharekquests"); return; }

	size_t iSize = p->vec_charekquest.size();
	size_t perQuestSize = 12 + 22 + 50 + sizeof(int) * 3 + sizeof(short) + sizeof(short);
	size_t bufferSize = sizeof(u32) + sizeof(u16) + sizeof(short) + (iSize * perQuestSize);

	char* cData = new char[bufferSize];
	char* cp = cData;

	u32 msgid = NOT_CHAREKQUESTS;
	u16 nulled = 0;

	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, (short)iSize);

	for (const auto& q : p->vec_charekquest)
	{
		Push(cp, q.m_iParentQuestID);
		Push(cp, q.m_cMapName, 12);
		Push(cp, q.m_cShowMapName, 22);
		Push(cp, q.m_cRewardName, 50);
		Push(cp, q.m_iKills);
		Push(cp, q.m_iMaxKills);
		short sStatus = q.status ? 1 : 0;
		Push(cp, sStatus);
		Push(cp, q.m_sCompleted);
	}

	if (p->m_pXSock) p->m_pXSock->iSendMsg(cData, cp - cData);
	delete[] cData;
}

void cCoreServer::start_ek_quest(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int quest_index = -1;
	for (int i = 0; i < _countof(ek_quests); ++i) {
		if (ek_quests[i].m_iQuestID == quest_id) {
			quest_index = i;
			break;
		}
	}
	if (quest_index == -1) return;

	for (const auto& q : p->vec_charekquest) {
		if (q.m_iParentQuestID == quest_id) {
			return;
		}
	}

	CharacterEk_Quests quest;
	quest.m_iParentQuestID = quest_id;
	strcpy_s(quest.m_cMapName, ek_quests[quest_index].m_cMapName);
	strcpy_s(quest.m_cShowMapName, ek_quests[quest_index].m_cShowMapName);
	strcpy_s(quest.m_cRewardName, ek_quests[quest_index].m_cRewardName);
	quest.m_iKills = 0;
	quest.m_iMaxKills = ek_quests[quest_index].m_iMaxKills;
	quest.status = false;
	quest.m_sCompleted = false;

	p->vec_charekquest.push_back(quest);

	char msg[128];
	sprintf_s(msg, sizeof(msg), "Has comenzado una nueva quest: %s", quest.m_cRewardName);
	ShowClientMsg(client, msg);

	for (auto& q : p->vec_charekquest) {
		if (q.m_iParentQuestID == quest_id) {
			q.status = true;
		}
		else {
			q.status = false;
		}
	}

	save_ekquest_data(client);
	send_charkillquests(client);
}

void cCoreServer::EnableEkQuestStatus(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (auto& q : p->vec_charekquest) {
		if (q.m_iParentQuestID == quest_id && q.m_sCompleted != 1) {
			q.status = true;
		}
		else {
			q.status = false;
		}
	}

	save_ekquest_data(client);
	send_charkillquests(client);
}

void cCoreServer::DisableEkQuestStatus(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (auto& q : p->vec_charekquest) {
		if (q.m_iParentQuestID == quest_id) {
			q.status = false;
			break;
		}
	}

	save_ekquest_data(client);
	send_charkillquests(client);
}

void cCoreServer::DeleteEkQuest(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	// Borrar solo si la quest NO está completada (m_sCompleted != 1)
	auto it = std::remove_if(p->vec_charekquest.begin(), p->vec_charekquest.end(),
		[quest_id](const CharacterEk_Quests& q) {
		return q.m_iParentQuestID == quest_id && q.m_sCompleted != 1;
	});

	if (it != p->vec_charekquest.end()) {
		p->vec_charekquest.erase(it, p->vec_charekquest.end());
		save_ekquest_data(client);
		send_charkillquests(client);
	}
	else {
		ShowClientMsg(client, "No puedes borrar una mision completada.");
	}
}

void cCoreServer::DeleteCompletedEkQuest(int client, int quest_id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	auto it = std::remove_if(p->vec_charekquest.begin(), p->vec_charekquest.end(),
		[quest_id](const CharacterEk_Quests& q) {
		return q.m_iParentQuestID == quest_id;
	});

	if (it != p->vec_charekquest.end()) {
		p->vec_charekquest.erase(it, p->vec_charekquest.end());
		save_ekquest_data(client);
		send_charkillquests(client);
	}
}

#include <tchar.h>
#include "Header/Game.h"
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

static bool is_wantedtrade_line(const string & line, string str)
{
	return (line.find(str) != string::npos);
}

static string gettrade_line(string file, string value1)
{
	ifstream fin(file);

	string line;

	while (getline(fin, line))
	{
		if (is_wantedtrade_line(line, value1))
			return line;
	}

	return "#";
}

static string gettradevalue(string val, short trade)
{
	string result;

	switch (trade)
	{
	case DEF_TRADEEK: result = gettrade_line("Configs\\trades\\tradeek.cfg", val); break;
	case DEF_TRADECONTRIB: result = gettrade_line("Configs\\trades\\tradecontrib.cfg", val); break;
	case DEF_TRADECOIN: result = gettrade_line("Configs\\trades\\tradecoin.cfg", val); break;
	case DEF_TRADEMAJS: result = gettrade_line("Configs\\trades\\trademajs.cfg", val); break;
	case DEF_TRADETOKENS: result = gettrade_line("Configs\\trades\\tradetokens.cfg", val); break;
	case DEF_TRADEEVENTPOINTS: result = gettrade_line("Configs\\trades\\tradeeventpoints.cfg", val); break;
	default:
		break;
	}

	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

static void tokenizetrade(string const &str, const char* delim,
	std::vector<string> &out)
{
	char *token = strtok(const_cast<char*>(str.c_str()), delim);
	while (token != nullptr)
	{
		out.push_back(string(token));
		token = strtok(nullptr, delim);
	}
}

void cCoreServer::get_trade_ek()
{
	vec_trade_ek.clear();
	PutLogList("(!) Reading trade ek data...");
	for (int i = 0; i < MAXTRADEEKITEMS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADEEK);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;

			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_ek u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.eks = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_ek.push_back(u);
	}
}


void cCoreServer::get_trade_eventpoints()
{
	vec_trade_eventpoints.clear();
	PutLogList("(!) Reading trade event points data...");
	for (int i = 0; i < MAXTRADEEVENTPOINTS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADEEVENTPOINTS);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;

			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_eventpoints u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.eventpoints = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_eventpoints.push_back(u);
	}
}

void cCoreServer::get_trade_tokens()
{
	vec_trade_tokens.clear();
	PutLogList("(!) Reading trade tokens data...");
	for (int i = 0; i < MAXTRADETOKENSITEMS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADETOKENS);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;

			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_token u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.tokens = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_tokens.push_back(u);
	}
}


void cCoreServer::get_trade_contrib()
{
	vec_trade_contrib.clear();

	PutLogList("(!) Reading trade contrib data...");
	for (int i = 0; i < MAXTRADECONTRIBITEMS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADECONTRIB);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;
			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_contrib u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.contrib = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_contrib.push_back(u);
	}
}

void cCoreServer::get_trade_coin()
{
	vec_trade_coin.clear();

	PutLogList("(!) Reading trade coin data...");
	for (int i = 0; i < MAXTRADECOINITEMS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADECOIN);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;
			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_coin u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.coins = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_coin.push_back(u);
	}
}



void cCoreServer::get_trade_majs()
{
	vec_trade_majs.clear();

	PutLogList("(!) Reading trade majestics data...");
	for (int i = 0; i < MAXTRADECOINITEMS; i++)
	{
		wsprintf(G_cTxt, "trade-num-%d = ", i + 1);
		string token = gettradevalue(G_cTxt, DEF_TRADEMAJS);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenizetrade(token, delim, out);
		string tempname;
		int tempval;
		short spr;
		short sprframe;
		short itemcolor;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				tempname = token;
				break;

			case 2:
				tempval = atoi((char*)token.c_str());
				break;
			case 3:
				spr = atoi((char*)token.c_str());
				break;

			case 4:
				sprframe = atoi((char*)token.c_str());
				break;

			case 5:
				itemcolor = atoi((char*)token.c_str());
				break;

			default: break;
			}
		}

		st_trade_majs u;

		u.index = i + 1;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		strcpy(u.itemname, (char*)tempname.c_str());
		u.majs = tempval;
		u.sprite = spr;
		u.spriteframe = sprframe;
		u.itemcolor = itemcolor;

		vec_trade_majs.push_back(u);
	}
}

void cCoreServer::send_trade_data(int client, short trade)
{
	u32 msgid = -1;
	u16 nulled = 0;
	int iSize = 0;

	size_t required_size = sizeof(msgid) + sizeof(nulled) + sizeof(iSize);

	switch (trade)
	{
	case DEF_TRADEEK:
		msgid = NOT_TRADE_EK;
		iSize = vec_trade_ek.size();
		break;

	case DEF_TRADECONTRIB:
		msgid = NOT_TRADE_CONTRIB;
		iSize = vec_trade_contrib.size();
		break;

	case DEF_TRADECOIN:
		msgid = NOT_TRADE_COIN;
		iSize = vec_trade_coin.size();
		break;

	case DEF_TRADEMAJS:
		msgid = NOT_TRADE_MAJS;
		iSize = vec_trade_majs.size();
		break;

	case DEF_TRADETOKENS:
		msgid = NOT_TRADE_TOKENS;
		iSize = vec_trade_tokens.size();
		break;

	case DEF_TRADEEVENTPOINTS:
		msgid = NOT_TRADE_EVENTPOINTS;
		iSize = vec_trade_eventpoints.size();
		break;

	default:
		break;
	}

	required_size += iSize * (21 + sizeof(int) + sizeof(short) * 3);

	std::vector<char> buffer(required_size);
	auto* cp = buffer.data();

	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, iSize);

	for (int i = 0; i < iSize; i++)
	{
		int val = -1;
		char itemname[21];
		ZeroMemory(itemname, sizeof(itemname));
		short spr = -1;
		short sprframe = -1;
		short itemcolor;

		switch (trade)
		{
		case DEF_TRADEEK:
			strcpy(itemname, vec_trade_ek[i].itemname);
			val = vec_trade_ek[i].eks;
			spr = vec_trade_ek[i].sprite;
			sprframe = vec_trade_ek[i].spriteframe;
			itemcolor = vec_trade_ek[i].itemcolor;
			break;

		case DEF_TRADECONTRIB:
			strcpy(itemname, vec_trade_contrib[i].itemname);
			val = vec_trade_contrib[i].contrib;
			spr = vec_trade_contrib[i].sprite;
			sprframe = vec_trade_contrib[i].spriteframe;
			itemcolor = vec_trade_contrib[i].itemcolor;
			break;

		case DEF_TRADECOIN:
			strcpy(itemname, vec_trade_coin[i].itemname);
			val = vec_trade_coin[i].coins;
			spr = vec_trade_coin[i].sprite;
			sprframe = vec_trade_coin[i].spriteframe;
			itemcolor = vec_trade_coin[i].itemcolor;
			break;

		case DEF_TRADEMAJS:
			strcpy(itemname, vec_trade_majs[i].itemname);
			val = vec_trade_majs[i].majs;
			spr = vec_trade_majs[i].sprite;
			sprframe = vec_trade_majs[i].spriteframe;
			itemcolor = vec_trade_majs[i].itemcolor;
			break;

		case DEF_TRADETOKENS:
			strcpy(itemname, vec_trade_tokens[i].itemname);
			val = vec_trade_tokens[i].tokens;
			spr = vec_trade_tokens[i].sprite;
			sprframe = vec_trade_tokens[i].spriteframe;
			itemcolor = vec_trade_tokens[i].itemcolor;
			break;

		case DEF_TRADEEVENTPOINTS:
			strcpy(itemname, vec_trade_eventpoints[i].itemname);
			val = vec_trade_eventpoints[i].eventpoints;
			spr = vec_trade_eventpoints[i].sprite;
			sprframe = vec_trade_eventpoints[i].spriteframe;
			itemcolor = vec_trade_eventpoints[i].itemcolor;
			break;

		default:
			break;
		}

		Push(cp, itemname, 21);
		Push(cp, val);
		Push(cp, spr);
		Push(cp, sprframe);
		Push(cp, itemcolor);
	}

	m_pClientList[client]->m_pXSock->iSendMsg(buffer.data(), cp - buffer.data());
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

bool cCoreServer::IsFragment(char * itname)
{
		return strcmp(itname, "LightShards") == 0 ||
		strcmp(itname, "StrongShards") == 0 ||
		strcmp(itname, "HitProbFragments") == 0 ||
		strcmp(itname, "DRFragments") == 0 ||
		strcmp(itname, "HPRecFragments") == 0 ||
		strcmp(itname, "SPRecFragments") == 0 ||
		strcmp(itname, "MPRecFragments") == 0 ||
		strcmp(itname, "MRFragments") == 0 ||
		strcmp(itname, "PaFragments") == 0 ||
		strcmp(itname, "MaFragments") == 0 ||
		strcmp(itname, "ExpFragments") == 0;
}


void cCoreServer::req_trade(int client, char * pData)
{
	int index = -1;
	int quant = 1;
	short mode = -1;

	auto p = m_pClientList[client];
	if (!p) return;

	Pop(pData, mode);
	Pop(pData, index);
	Pop(pData, quant);

	switch (mode)
	{
	case DEF_TRADEEK:
	{
		if (index > vec_trade_ek.size() || index < 0) return;
		if (quant < 1 || quant > 50) return;

		if (_iGetItemSpaceLeft(client) < quant)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		auto icost = vec_trade_ek[index].eks * quant;

		if (p->m_iEnemyKillCount < icost || p->m_iEnemyKillCount < 0)
		{
			ShowClientMsg(client, "Not Enough Eks");
			return;
		}

		class CItem * pItem;
		int iItemID, iEraseReq;

		for (int i = 0; i < quant; i++)
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_ek[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			if (string(pItem->m_cName) == "MagicEmerald" ||
				string(pItem->m_cName) == "MagicDiamond" ||
				string(pItem->m_cName) == "MagicSapphire")
			{
				pItem->m_sItemSpecEffectValue1 = 300;
				pItem->m_sItemSpecEffectValue2 = 100;
				pItem->m_wCurLifeSpan = 300;
				pItem->m_sTouchEffectType = 2;
				pItem->m_dwAttribute = 1;
			}

			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
		}

		p->m_iEnemyKillCount -= icost;
		SendCommand(client, "/eks", p->m_iEnemyKillCount);

		break;
	}

	case DEF_TRADETOKENS:
	{
		if (index >= vec_trade_tokens.size() || index < 0) return;
		if (quant < 1 || quant > 50) return;

		if (_iGetItemSpaceLeft(client) < quant)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		auto icost = vec_trade_tokens[index].tokens * quant;

		if (p->m_iTokens < icost || p->m_iTokens < 0)
		{
			ShowClientMsg(client, "Not Enough Tokens");
			return;
		}

		class CItem* pItem;
		int iItemID, iEraseReq;
		bool itemExists = false;

		for (int i = 0; i < DEF_MAXITEMS; i++) {
			auto item = p->m_pItemList[i];
			if (!item) continue;

			if (string(item->m_cName) == vec_trade_tokens[index].itemname)
			{
				if (countableItems(item->m_cName))
				{
					DWORD amountToAdd = quant;

					if (IsFragment(item->m_cName)) {
						amountToAdd *= 150;
					}

					item->m_dwCount += amountToAdd;
					itemExists = true;

					SendCommand(client, "/newitemcount", i, item->m_dwCount);
					wsprintf(G_cTxt, "You now have %d %s.", item->m_dwCount, item->m_cName);
					ShowClientMsg(client, G_cTxt);
				}
				break;
			}
		}

		if (itemExists)
		{
			p->m_iTokens -= icost;
			SendCommand(client, "/tokens", p->m_iTokens);
			return;
		}

		if (IsFragment(vec_trade_tokens[index].itemname))
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_tokens[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			pItem->m_dwCount = quant * 150;
			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);

			p->m_iTokens -= icost;
			SendCommand(client, "/tokens", p->m_iTokens);
			return;
		}

		for (int i = 0; i < quant; i++)
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_tokens[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			if (string(pItem->m_cName) == "MagicEmerald" ||
				string(pItem->m_cName) == "MagicDiamond" ||
				string(pItem->m_cName) == "MagicSapphire")
			{
				pItem->m_sItemSpecEffectValue1 = 300;
				pItem->m_sItemSpecEffectValue2 = 100;
				pItem->m_wCurLifeSpan = 300;
				pItem->m_sTouchEffectType = 2;
				pItem->m_dwAttribute = 1;
			}

			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
		}

		p->m_iTokens -= icost;
		SendCommand(client, "/tokens", p->m_iTokens);

		break;
	}


	case DEF_TRADECONTRIB:
	{
		if (index > vec_trade_contrib.size() || index < 0) return;
		if (quant < 1 || quant > 50) return;

		if (_iGetItemSpaceLeft(client) < quant)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		auto icost = vec_trade_contrib[index].contrib * quant;

		if (p->m_iContribution < icost || p->m_iContribution < 0)
		{
			ShowClientMsg(client, "Not Enough Contribution");
			return;
		}

		class CItem * pItem;
		int iItemID, iEraseReq;

		for (int i = 0; i < quant; i++)
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_contrib[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
		}

		p->m_iContribution -= icost;
		SendCommand(client, "/contrib", p->m_iContribution);
		break;
	}

	case DEF_TRADEEVENTPOINTS:
	{
		if (index > vec_trade_eventpoints.size() || index < 0) return;
		if (quant < 1 || quant > 50) return;

		if (_iGetItemSpaceLeft(client) < quant)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		auto icost = vec_trade_eventpoints[index].eventpoints * quant;

		if (p->m_iEventPoints < icost || p->m_iEventPoints < 0)
		{
			ShowClientMsg(client, "Not Enough Event Points");
			return;
		}

		class CItem * pItem;
		int iItemID, iEraseReq;

		for (int i = 0; i < quant; i++)
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_eventpoints[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
		}

		p->m_iEventPoints -= icost;
		SendCommand(client, "/eventpoints", p->m_iEventPoints);
		break;
	}

	case DEF_TRADECOIN:
	{
		if (index > vec_trade_coin.size() || index < 0) return;
		if (quant < 1 || quant > 50) return;

		string reward = vec_trade_coin[index].itemname;
		string contrib_text = "Contrib+";
		string maj_text = "Majestic+";
		string rep_text = "Rep+";
		
		if (startsWith(reward, contrib_text))
		{
			deleteText(reward, contrib_text);
			int contrib = atoi(reward.c_str());
			if (quant > 1) return;

			auto icost = vec_trade_coin[index].coins;

			if (p->m_iCoins < icost || p->m_iCoins < 0)
			{
				ShowClientMsg(client, "Not Enough Coins");
				return;
			}

			p->m_iCoins -= icost;
			SendCommand(client, "/coins", p->m_iCoins);

			if (contrib > 0) {
				wsprintf(G_cTxt, "You get %d contribution from trade coin.", contrib);
				ShowClientMsg(client, G_cTxt);
				p->m_iContribution += contrib;
				SendCommand(client, "/contrib", p->m_iContribution);
			}
		}
		else if (startsWith(reward, maj_text))
		{
			deleteText(reward, maj_text);
			int majs = atoi(reward.c_str());
			if (quant > 1) return;
			auto icost = vec_trade_coin[index].coins;

			if (p->m_iCoins < icost || p->m_iCoins < 0)
			{
				ShowClientMsg(client, "Not Enough Coins");
				return;
			}

			p->m_iCoins -= icost;
			SendCommand(client, "/coins", p->m_iCoins);

			if (majs > 0) {
				wsprintf(G_cTxt, "You get %d majestics from trade coin.", majs);
				ShowClientMsg(client, G_cTxt);
				p->m_iGizonItemUpgradeLeft += majs;
				SendCommand(client, "/majs", p->m_iGizonItemUpgradeLeft);
			}
		}
		else if (startsWith(reward, rep_text))
		{
			deleteText(reward, rep_text);
			int rep = atoi(reward.c_str());
			if (quant > 1) return;
			auto icost = vec_trade_coin[index].coins;

			if (p->m_iCoins < icost || p->m_iCoins < 0)
			{
				ShowClientMsg(client, "Not Enough Coins");
				return;
			}

			p->m_iCoins -= icost;
			SendCommand(client, "/coins", p->m_iCoins);

			if (rep > 0) {
				wsprintf(G_cTxt, "You get %d reputation from trade coin.", rep);
				ShowClientMsg(client, G_cTxt);
				p->m_iRating += rep;
				SendCommand(client, "/rep", p->m_iRating);
			}
		}
		else
		{
			if (_iGetItemSpaceLeft(client) < quant)
			{
				SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
				return;
			}

			auto icost = vec_trade_coin[index].coins * quant;

			if (p->m_iCoins < icost || p->m_iCoins < 0)
			{
				ShowClientMsg(client, "Not Enough Coins");
				return;
			}

			p->m_iCoins -= icost;
			SendCommand(client, "/coins", p->m_iCoins);

			class CItem * pItem;
			int iItemID, iEraseReq;

			for (int i = 0; i < quant; i++)
			{
				pItem = new class CItem;
				if (!_bInitItemAttr(pItem, vec_trade_coin[index].itemname))
				{
					delete pItem;
					return;
				}

				if (!_bAddClientItemList(client, pItem, &iEraseReq))
				{
					delete pItem;
					return;
				}

				SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
			}
		}
				
		break;
	}

	case DEF_TRADEMAJS:
	{
		if (index > vec_trade_majs.size() || index < 0) return;
		if (quant < 1 || quant > 20) return;

		if (_iGetItemSpaceLeft(client) < quant)
		{
			SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
			return;
		}

		auto icost = vec_trade_majs[index].majs * quant;

		if (p->m_iGizonItemUpgradeLeft < icost || p->m_iGizonItemUpgradeLeft < 0)
		{
			ShowClientMsg(client, "Not Enough Majestics");
			return;
		}

		class CItem * pItem;
		int iItemID, iEraseReq;

		for (int i = 0; i < quant; i++)
		{
			pItem = new class CItem;
			if (!_bInitItemAttr(pItem, vec_trade_majs[index].itemname))
			{
				delete pItem;
				return;
			}

			if (!_bAddClientItemList(client, pItem, &iEraseReq))
			{
				delete pItem;
				return;
			}

			if (string(pItem->m_cName) == "AngelicPandent(STR)" ||
				string(pItem->m_cName) == "AngelicPandent(DEX)" ||
				string(pItem->m_cName) == "AngelicPandent(INT)" ||
				string(pItem->m_cName) == "AngelicPandent(MAG)")
			{
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = p->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = p->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = p->m_sCharIDnum3;
			}
			
			SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
		}

		p->m_iGizonItemUpgradeLeft -= icost;
		SendCommand(client, "/majs", p->m_iGizonItemUpgradeLeft);
		break;
	}

	default: return; break;
	}
}

void cCoreServer::req_tradeball(int client, char * data)
{
	auto p = m_pClientList[client];
	if (!p) return;

	short ballmode;
	int quant;

	Pop(data, ballmode);
	Pop(data, quant);

	switch (ballmode)
	{
		case BALL_CONTRIB:
		{
			getContributionBall(client, quant);
			break;
		}

		case BALL_COIN:
		{
			getCoinBall(client, quant);
			break;
		}
	
		case BALL_REP:
		{
			getReputationBall(client, quant);
			break;
		}

		case BALL_MAJS:
		{
			getMajesticBall(client, quant);
			break;
		}

		case BALL_LIGHT:
		{
			getLightShardsBall(client, quant);
			break;
		}

		case BALL_STRONG:
		{
			getStrongShardsBall(client, quant);
			break;
		}

		case BALL_HITPROB:
		{
			getHitProbFragmentsBall(client, quant);
			break;
		}

		case BALL_DR:
		{
			getDRFragmentsBall(client, quant);
			break;
		}

		case BALL_HPREC:
		{
			getHPRecFragmentsBall(client, quant);
			break;
		}

		case BALL_SPREC:
		{
			getSPRecFragmentsBall(client, quant);
			break;
		}

		case BALL_MPREC:
		{
			getMPRecFragmentsBall(client, quant);
			break;
		}

		case BALL_MR:
		{
			getMRFragmentsBall(client, quant);
			break;
		}

		case BALL_PA:
		{
			getPAFragmentsBall(client, quant);
			break;
		}

		case BALL_MA:
		{
			getMAFragmentsBall(client, quant);
			break;
		}

		case BALL_EXP:
		{
			getEXPFragmentsBall(client, quant);
			break;
		}

		case BALL_MASTERY:
		{
			getMasteryBall(client, quant);
			break;
		}

		case BALL_FIRESEGMENT:
		{
			getFireSegmentBall(client, quant);
			break;
		}
		case BALL_LIGHTNINGSEGMENT:
		{
			getLightningSegmentBall(client, quant);
			break;
		}
		case BALL_ICESEGMENT:
		{
			getIceSegmentBall(client, quant);
			break;
		}

	default: return; break;
	}
}

void cCoreServer::getLightShardsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->shard_light <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes light shards.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->shard_light < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes light shards para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->shard_light -= totalCost;
	SendCommand(client, "/shard_light", p->shard_light);

	char itemname[21];
	wsprintf(itemname, "LightShards");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getStrongShardsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->shard_strong <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes strong shards.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->shard_strong < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes strong shards para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->shard_strong -= totalCost;
	SendCommand(client, "/shard_strong", p->shard_strong);

	char itemname[21];
	wsprintf(itemname, "StrongShards");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getHitProbFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_hp <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes hp fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_hp < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes hp fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_hp -= totalCost;
	SendCommand(client, "/fragment_hp", p->fragment_hp);

	char itemname[21];
	wsprintf(itemname, "HitProbFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getDRFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_dr <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes dr fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_dr < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes dr fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_dr -= totalCost;
	SendCommand(client, "/fragment_dr", p->fragment_dr);

	char itemname[21];
	wsprintf(itemname, "DRFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getHPRecFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_hprec <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes hprec fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_hprec < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes hprec fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_hprec -= totalCost;
	SendCommand(client, "/fragment_hprec", p->fragment_hprec);

	char itemname[21];
	wsprintf(itemname, "HPRecFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getSPRecFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_sprec <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes sprec fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_sprec < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes sprec fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_sprec -= totalCost;
	SendCommand(client, "/fragment_sprec", p->fragment_sprec);

	char itemname[21];
	wsprintf(itemname, "SPRecFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getMPRecFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_mprec <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes mprec fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_mprec < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes mprec fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_mprec -= totalCost;
	SendCommand(client, "/fragment_mprec", p->fragment_mprec);

	char itemname[21];
	wsprintf(itemname, "MPRecFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getMRFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_mr <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes MR fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_mr < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes MR fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_mr -= totalCost;
	SendCommand(client, "/fragment_mr", p->fragment_mr);

	char itemname[21];
	wsprintf(itemname, "MRFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getPAFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_pa <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes PA fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_pa < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes PA fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_pa -= totalCost;
	SendCommand(client, "/fragment_pa", p->fragment_pa);

	char itemname[21];
	wsprintf(itemname, "PaFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getMAFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_ma <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes MA fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_ma < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes MA fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_ma -= totalCost;
	SendCommand(client, "/fragment_ma", p->fragment_ma);

	char itemname[21];
	wsprintf(itemname, "MaFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getEXPFragmentsBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->fragment_exp <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes EXP fragments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->fragment_exp < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes EXP fragments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->fragment_exp -= totalCost;
	SendCommand(client, "/fragment_exp", p->fragment_exp);

	char itemname[21];
	wsprintf(itemname, "ExpFragments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getContributionBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iContribution <= 0)
	{
		ShowClientMsg(client, "No tienes suficiente contribución.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant; 
	if (p->m_iContribution < totalCost)
	{
		ShowClientMsg(client, "No tienes suficiente contribución para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iContribution -= totalCost;
	SendCommand(client, "/contrib", p->m_iContribution);

	char itemname[21];
	wsprintf(itemname, "ContribBall");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant; 

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL); 	
}

void cCoreServer::getCoinBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iCoins <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes coins.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iCoins < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes coins para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iCoins -= totalCost;
	SendCommand(client, "/coins", p->m_iCoins);

	char itemname[21];
	wsprintf(itemname, "CoinBall");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemListWithoutWeight(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getFireSegmentBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iFireSegments <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes Fire Segments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iFireSegments < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes Fire Segments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iFireSegments -= totalCost;
	SendCommand(client, "/firesegments", p->m_iFireSegments);

	char itemname[21];
	wsprintf(itemname, "FireSegments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemListWithoutWeight(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getLightningSegmentBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iLightingSegments <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes Lightning Segments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iLightingSegments < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes Lightning Segments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iLightingSegments -= totalCost;
	SendCommand(client, "/lightningsegments", p->m_iLightingSegments);

	char itemname[21];
	wsprintf(itemname, "LightningSegments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemListWithoutWeight(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getIceSegmentBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iIceSegments <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes Ice Segments.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iIceSegments < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes Ice Segments para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iIceSegments -= totalCost;
	SendCommand(client, "/icesegments", p->m_iIceSegments);

	char itemname[21];
	wsprintf(itemname, "IceSegments");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemListWithoutWeight(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getMasteryBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iTokens <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes masterys.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iTokens < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes masterys para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iTokens -= totalCost;
	SendCommand(client, "/tokens", p->m_iTokens);

	char itemname[21];
	wsprintf(itemname, "Tokens");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}


void cCoreServer::getReputationBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iRating <= 0)
	{
		ShowClientMsg(client, "No tienes suficiente reputacion.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iRating < totalCost)
	{
		ShowClientMsg(client, "No tienes suficiente reputacion para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iRating -= totalCost;
	SendCommand(client, "/rep", p->m_iRating);

	char itemname[21];
	wsprintf(itemname, "RepBall");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

void cCoreServer::getMajesticBall(int client, int quant)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_iGizonItemUpgradeLeft <= 0)
	{
		ShowClientMsg(client, "No tienes suficientes majestics.");
		return;
	}

	if (quant < 1)
	{
		ShowClientMsg(client, "La cantidad debe ser al menos 1.");
		return;
	}

	int totalCost = quant;
	if (p->m_iGizonItemUpgradeLeft < totalCost)
	{
		ShowClientMsg(client, "No tienes suficientes majestics para esta cantidad.");
		return;
	}

	if (_iGetItemSpaceLeft(client) < 1)
	{
		SendItemNotifyMsg(client, CLIENT_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	p->m_iGizonItemUpgradeLeft -= totalCost;
	SendCommand(client, "/majs", p->m_iGizonItemUpgradeLeft);

	char itemname[21];
	wsprintf(itemname, "MajBall");

	class CItem* pItem;
	int iEraseReq;

	pItem = new class CItem;
	if (!_bInitItemAttr(pItem, itemname))
	{
		delete pItem;
		return;
	}

	pItem->m_dwCount = quant;

	if (!_bAddClientItemList(client, pItem, &iEraseReq))
	{
		delete pItem;
		return;
	}

	SendItemNotifyMsg(client, CLIENT_NOTIFY_ITEMOBTAINED, pItem, NULL);
}

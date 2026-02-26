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

static bool is_wantedquest_line(const string & line, string str)
{
	return (line.find(str) != string::npos);
}

static string get_questline(string file, string value1)
{
	ifstream fin(file);

	string line;

	while (getline(fin, line))
	{
		if (is_wantedquest_line(line, value1))
			return line;
	}

	return "#";
}

static string getquestvalue(string val)
{
	string result = get_questline("Configs\\QuestList.cfg", val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

static void tokenize_quest(string const &str, const char* delim,
	std::vector<string> &out)
{
	char *token = strtok(const_cast<char*>(str.c_str()), delim);
	while (token != nullptr)
	{
		out.push_back(string(token));
		token = strtok(nullptr, delim);
	}
}

void cCoreServer::get_quests()
{
	vec_questlist.clear();
	PutLogList("(!) Reading QuestList settings data...");
	for (int i = 0; i < DEF_MAXQUESTLIST; i++)
	{
		wsprintf(G_cTxt, "quest-num-%d = ", i + 1);
		string token = getquestvalue(G_cTxt);
		if (string(token) == "#") continue;

		const char* delim = " ";
		vector<string> out;
		tokenize_quest(token, delim, out);

		st_quest u;

		int count = 0;
		for (auto &token : out) {
			count++;
			switch (count)
			{
			case 1:
				u.m_sQuestID = atoi((char*)token.c_str());
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
				u.m_iMaxKills = atoi((char*)token.c_str());
				break;

			case 6:
				ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
				strcpy(u.m_cQuestReward1, (char*)token.c_str());
				break;

			case 7:
				ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
				strcpy(u.m_cQuestReward2, (char*)token.c_str());
				break;

			case 8:
				ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
				strcpy(u.m_cQuestReward3, (char*)token.c_str());
				break;

			default: break;
			}
		}

		vec_questlist.push_back(u);
	}
}

void cCoreServer::send_quest_data(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;

	char data[8096];
	auto * cp = data;
	u32 msgid = -1;
	u16 nulled = 0;
	short iSize = 0;

	msgid = NOT_QUESTLIST;

	for (int i = 0; i < vec_questlist.size(); i++)
	{
		if (string(p->m_cLocation) == "elvine" && string(vec_questlist[i].m_cMapName) == "aresden") continue;
		if (string(p->m_cLocation) == "aresden" && string(vec_questlist[i].m_cMapName) == "elvine") continue;

		iSize++;
	}
	
	Push(cp, msgid);
	Push(cp, nulled);
	Push(cp, iSize);

	for (int i = 0; i < vec_questlist.size(); i++)
	{
		if (string(p->m_cLocation) == "elvine" && string(vec_questlist[i].m_cMapName) == "aresden") continue;
		if (string(p->m_cLocation) == "aresden" && string(vec_questlist[i].m_cMapName) == "elvine") continue;

		Push(cp, vec_questlist[i].m_sQuestID);
		Push(cp, vec_questlist[i].m_cNpcName, 21);
		Push(cp, vec_questlist[i].m_cMapName, 12);
		Push(cp, vec_questlist[i].m_cRealMapName, 12);
		Push(cp, vec_questlist[i].m_iMaxKills);
		Push(cp, vec_questlist[i].m_iRewardContribution);
		Push(cp, vec_questlist[i].m_cQuestReward1, 21);
		Push(cp, vec_questlist[i].m_cQuestReward2, 21);
		Push(cp, vec_questlist[i].m_cQuestReward3, 21);
	}

	m_pClientList[client]->m_pXSock->iSendMsg(data, cp - data);
}

void cCoreServer::get_quest_data(int client, char * data)
{
	short id;
	auto p = m_pClientList[client];
	if (!p) return;
	
	Pop(data, id);
	start_quest(client, id);
}

void cCoreServer::start_quest(int client, short id)
{
	auto p = m_pClientList[client];
	if (!p) return;

	for (int i = 0; i < p->vec_charquest.size(); i++)
	{
		auto charquest = p->vec_charquest[i];

		if (id == charquest.m_sParentQuestID) 
		{ 
			ShowClientMsg(client, "Error. The quest is already active.");
			return;
		}
	}

	if (p->vec_charquest.size() == DEF_MAXCHARQUESTS)
	{
		ShowClientMsg(client, "Error. Maximum number of simultaneous quests reached.");
		return;
	}

	short tmpindex = -1;
	for (int i = 0; i < vec_questlist.size(); i++)
	{
		auto quest = vec_questlist[i];
		if (id == quest.m_sQuestID)
		{
			tmpindex = i;
		}
	}

	if (tmpindex == -1) return;

	for (int i = 0; i < p->vec_charquest.size(); i++)
	{
		auto charquest = p->vec_charquest[i];
		charquest.m_sStatus = 0;
	}

	st_charquest u;

	auto questlist = vec_questlist[tmpindex];
	u.m_sParentQuestID = questlist.m_sQuestID;
	ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
	strcpy(u.m_cNpcName, questlist.m_cNpcName);
	ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
	strcpy(u.m_cMapName, questlist.m_cMapName);
	ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
	strcpy(u.m_cRealMapName, questlist.m_cRealMapName);
	u.m_iKills = 0;
	u.m_iMaxKills = questlist.m_iMaxKills;
	u.m_iRewardContribution = questlist.m_iRewardContribution;
	ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
	strcpy(u.m_cQuestReward1, questlist.m_cQuestReward1);
	ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
	strcpy(u.m_cQuestReward2, questlist.m_cQuestReward2);
	ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
	strcpy(u.m_cQuestReward3, questlist.m_cQuestReward3);
	u.m_sStatus = 1;
	u.m_sCompleted = 0;

	p->vec_charquest.push_back(u);

	ShowClientMsg(client, "The mission started successfully!");
	save_quest_data(client);
	send_charquest_data(client);
}
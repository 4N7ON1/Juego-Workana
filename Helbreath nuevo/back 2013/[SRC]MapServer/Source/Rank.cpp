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

void cCoreServer::clear_rank()
{
	for (int i = 0; i < MAXRANKEXP; i++)
	{
		m_iMaxrankexp[i] = 0;
	}
}

void cCoreServer::init_rank()
{
	PutLogList("(!) Reading Rank settings...");
	clear_rank();
	read();
}

void cCoreServer::update_rank()
{
	PutLogList("(!) Update Rank settings...");
	clear_rank();
	read();
}

bool cCoreServer::is_wantedrank_line(const string & line, string str)
{
	return (line.find(str) != string::npos);
}

string cCoreServer::getrank_line(string file, string value1)
{
	ifstream fin(file);

	string line;

	while (getline(fin, line))
	{
		if (is_wantedrank_line(line, value1))
			return line;
	}

	return "#";
}

string cCoreServer::getrankvalue(string val)
{
	string result = getrank_line("Configs\\RankSettings.cfg", val);
	if (string(result) == "#") return result;
	else result.erase(0, val.length());
	return result;
}

void cCoreServer::read()
{
	for (int i = 0; i < MAXRANKEXP; i++)
	{
		string stmp;
		char ctemp[50];
		wsprintf(ctemp, "maxrankexp-level%d = ", i + 1);
		stmp = getrankvalue(ctemp);
		if (string(stmp) == "#") continue;
		m_iMaxrankexp[i] = atoi((char*)stmp.c_str());
	}		
}
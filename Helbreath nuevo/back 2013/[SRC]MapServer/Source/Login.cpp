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


#define WORLDNAMELS   "YAP"
#define WORLDNAMELS2   "YAP-YAP"
#define DEF_MAXACCOUNTS				3000 // Old 1000 -> New 3000
#define DEF_MAXCHARACTER			4000 //4000


BOOL bCheckValidName(char *pStr)
{
	int i, iLen;
	iLen = strlen(pStr);
	for (i = 0; i < iLen; i++)
	{
		if (pStr[i] < 0)	return FALSE;
		if ((pStr[i] == ',') || (pStr[i] == '=') || (pStr[i] == ' ') || (pStr[i] == '\n') ||
			(pStr[i] == '\t') || (pStr[i] == '.') || (pStr[i] == '\\') || (pStr[i] == '/') ||
			(pStr[i] == ':') || (pStr[i] == '*') || (pStr[i] == '?') || (pStr[i] == '<') ||
			(pStr[i] == '>') || (pStr[i] == '|') || (pStr[i] == '"') || (pStr[i] == '`') ||
			(pStr[i] == ';') || (pStr[i] == '=') || (pStr[i] == '@') || (pStr[i] == '[') ||
			(pStr[i] == ']') || (pStr[i] == '^') || (pStr[i] == '_') || (pStr[i] == '\'')) return FALSE;
		if ((pStr[i] < '0') || (pStr[i] > 'z')) return FALSE;
	}
	return TRUE;
}

void cCoreServer::RequestLogin(int iClientH, char * pData)
{
	auto lcl = m_pLoginClients[iClientH];
	if (iClientH < 0 || iClientH >= MAXCLIENTLOGINSOCK || !lcl) return;

	char cName[11] = {};
	char cPassword[11] = {};
	char world_name[32] = {};

	auto cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 10);
	Pop(cp, cPassword, 10);
	Pop(cp, world_name, 30);

	if (string(world_name) != WORLDNAMELS)
		return;

	if (!bCheckValidName(cName) || !bCheckValidName(cPassword) || !bCheckValidName(world_name))
		return;

	wsprintf(G_cTxt, "(!) Account Request Login: %s", cName);
	PutLogList(G_cTxt);

	vector<string> chars;
	auto status = AccountLogIn(cName, cPassword, chars);
	switch (status)
	{
	case LogIn::Ok:
	{
		char pData[512] = {};
		char * cp2 = pData;
		Push(cp2, (int)chars.size());
		GetCharList(cName, cp2, chars);

		SendLoginMsg(DEF_MSGTYPE_CONFIRM, DEF_MSGTYPE_CONFIRM, pData, cp2 - pData, iClientH);
		break;
	}

	case LogIn::NoAcc:
		SendLoginMsg(NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, 0, 0, iClientH);
		break;

	case LogIn::NoPass:
		SendLoginMsg(NUCLEO_RESPONSE_PASSWORDMISMATCH, NUCLEO_RESPONSE_PASSWORDMISMATCH, 0, 0, iClientH);
		break;
	}
}

void cCoreServer::GetCharList(string acc, char*& cp2, vector<string> chars)
{
	for (int i = 0; i < chars.size(); i++)
	{
		char seps[] = "= \t\n";
		char cFileName[112] = {};
		char cDir[64] = {};

		// Crear carpetas si no existen
		mkdir("Database");
		mkdir("Database\\Character");

		// Construir ruta del archivo del personaje
		ZeroMemory(cFileName, sizeof(cFileName));
		strcat(cFileName, "Database\\Character\\");
		wsprintf(cDir, "AscII%d", chars[i][0]); // Carpeta según primer char ASCII
		strcat(cFileName, cDir);
		strcat(cFileName, "\\");
		strcat(cFileName, (char*)chars[i].c_str());
		strcat(cFileName, ".txt");

		// Abrir archivo
		HANDLE hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			wsprintf(G_cTxt, "(!) Character file not found: %s", cFileName);
			PutLogList(G_cTxt);
			continue;
		}

		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize == 0) {
			CloseHandle(hFile);
			continue;
		}

		auto cp = new char[dwFileSize + 1];
		ZeroMemory(cp, dwFileSize + 1);
		DWORD lpNumberOfBytesRead;
		ReadFile(hFile, cp, dwFileSize, &lpNumberOfBytesRead, NULL);
		CloseHandle(hFile);

		auto pStrTok = new class CStrTok(cp, seps);
		char* token = pStrTok->pGet();

		short sAppr1 = 0, sAppr2 = 0, sAppr3 = 0, sAppr4 = 0;
		WORD cSex = 0, cSkin = 0;
		WORD iLevel = 0;
		DWORD iExp = 0;
		int iApprColor = 0;
		char cMapName[21] = {};
		int cReadModeA = 0;

		while (token != NULL)
		{
			if (cReadModeA != 0)
			{
				switch (cReadModeA)
				{
				case 1: sAppr1 = atoi(token); break;
				case 2: sAppr2 = atoi(token); break;
				case 3: sAppr3 = atoi(token); break;
				case 4: sAppr4 = atoi(token); break;
				case 5: cSex = atoi(token); break;
				case 6: cSkin = atoi(token); break;
				case 7: iLevel = atoi(token); break;
				case 14: iExp = atoi(token); break;
				case 15: iApprColor = atoi(token); break;
				case 17: strncpy(cMapName, token, 20); break;
				}
				cReadModeA = 0;
			}
			else {
				if (strcmp(token, "appr1") == 0) cReadModeA = 1;
				else if (strcmp(token, "appr2") == 0) cReadModeA = 2;
				else if (strcmp(token, "appr3") == 0) cReadModeA = 3;
				else if (strcmp(token, "appr4") == 0) cReadModeA = 4;
				else if (strcmp(token, "sex-status") == 0) cReadModeA = 5;
				else if (strcmp(token, "skin-status") == 0) cReadModeA = 6;
				else if (strcmp(token, "character-LEVEL") == 0) cReadModeA = 7;
				else if (strcmp(token, "character-EXP") == 0) cReadModeA = 14;
				else if (strcmp(token, "appr-color") == 0) cReadModeA = 15; // ← CAMBIO HECHO
				else if (strcmp(token, "character-loc-map") == 0) cReadModeA = 17;
			}

			token = pStrTok->pGet();
		}

		delete pStrTok;
		delete[] cp;

		// Agregar datos al buffer de respuesta
		char cName[11] = {};
		strncpy(cName, chars[i].c_str(), 10);

		Push(cp2, cName, 10);
		Push(cp2, sAppr1);
		Push(cp2, sAppr2);
		Push(cp2, sAppr3);
		Push(cp2, sAppr4);
		Push(cp2, cSex);
		Push(cp2, cSkin);
		Push(cp2, iLevel);
		Push(cp2, iExp);
		Push(cp2, iApprColor);
		Push(cp2, cMapName, 10);
	}
}

LogIn cCoreServer::AccountLogIn(string acc, string pass, vector<string>& chars)
{
	char file_name[255], cDir[63], cTxt[50];
	char* cp, *token;
	char seps[] = "= \t\n";
	class CStrTok* pStrTok;

	char cReadModeA = 0;

	ZeroMemory(cTxt, sizeof(cTxt));
	wsprintf(cTxt, "account-character-YAP");

	if (acc.empty()) return LogIn::NoAcc;
	if (pass.empty()) return LogIn::NoPass;

	// Construir ruta del archivo: ..\Nucleo\Account\AscIIxx\<acc>.txt
	ZeroMemory(file_name, sizeof(file_name));
	ZeroMemory(cDir, sizeof(cDir));

	strcat(file_name, "Database\\Account\\");
	wsprintf(cDir, "AscII%d", acc[0]); // acc[0] es el primer carácter (por ej: '1' -> 49)
	strcat(file_name, cDir);
	strcat(file_name, "\\");
	strcat(file_name, (char*)acc.c_str());
	strcat(file_name, ".txt");

	// Verificar existencia del archivo
	HANDLE file_handle = CreateFile(file_name, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		wsprintf(G_cTxt, "(!) Account Does not Exist [invalid handle] (%s)", file_name);
		PutLogList(G_cTxt);
		return LogIn::NoAcc;
	}

	DWORD file_sz = GetFileSize(file_handle, NULL);
	CloseHandle(file_handle);

	if (file_sz == 0) {
		wsprintf(G_cTxt, "(!) Login File sz = 0 (%s)", file_name);
		PutLogList(G_cTxt);
		return LogIn::NoAcc;
	}

	FILE* file = fopen(file_name, "rt");
	if (file == NULL) {
		wsprintf(G_cTxt, "(!) Account Does not Exist (%s)", file_name);
		PutLogList(G_cTxt);
		return LogIn::NoAcc;
	}

	// Leer archivo completo en memoria
	cp = new char[file_sz + 1];
	ZeroMemory(cp, file_sz + 1);
	fread(cp, file_sz, 1, file);

	pStrTok = new class CStrTok(cp, seps);
	token = pStrTok->pGet();

	while (token != NULL)
	{
		if (cReadModeA != 0)
		{
			switch (cReadModeA)
			{
			case 1: // account-name
				if (strlen(token) == 0 || acc != token) {
					PutLogList("(!) Wrong Acc name");
					delete pStrTok; delete[] cp; fclose(file);
					return LogIn::NoAcc;
				}
				cReadModeA = 0;
				break;

			case 2: // account-password
				if (strlen(token) == 0 || pass != token) {
					PutLogList("(!) Wrong pass");
					delete pStrTok; delete[] cp; fclose(file);
					return LogIn::NoPass;
				}
				cReadModeA = 0;
				break;

			case 3: // account-character-YAP
				chars.push_back(string(token));
				if (chars.size() > 4) {
					PutLogList("(!) Charlist exceeds 4 in acc cfg!");
					delete pStrTok; delete[] cp; fclose(file);
					return LogIn::NoAcc;
				}
				cReadModeA = 0;
				break;
			}
		}
		else {
			if (strcmp(token, "account-name") == 0) cReadModeA = 1;
			else if (strcmp(token, "account-password") == 0) cReadModeA = 2;
			else if (strcmp(token, cTxt) == 0) cReadModeA = 3;
		}

		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] cp;
	fclose(file);

	return LogIn::Ok;
}

BOOL bNewCharCheckValidName(char* pStr)
{
	int iLen = strlen(pStr);
	for (int i = 0; i < iLen; i++)
	{
		unsigned char c = pStr[i];
		if (c < 33 || c > 126) return FALSE; // Solo ASCII imprimibles (evita espacio, \n, \r, etc.)

		switch (c)
		{
		case '\\': case '/': case ':': case '*':
		case '?': case '\"': case '<': case '>': case '|':
		case '=': case ',': case ';':
			return FALSE; // caracteres problemáticos para archivos / INI
		}
	}
	return TRUE;
}

void cCoreServer::CreateNewCharacter(int iClientH, char* pData) {
	char cName[11] = {};
	char cAcc[11] = {};
	char cPassword[11] = {};
	char world_name[32] = {};
	char gender, skin, hairstyle, haircolor, under, str, vit, dex, intl, mag, chr;

	auto cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 10);
	Pop(cp, cAcc, 10);
	Pop(cp, cPassword, 10);
	Pop(cp, world_name, 30);
	Pop(cp, gender);
	Pop(cp, skin);
	Pop(cp, hairstyle);
	Pop(cp, haircolor);
	Pop(cp, under);
	Pop(cp, str);
	Pop(cp, vit);
	Pop(cp, dex);
	Pop(cp, intl);
	Pop(cp, mag);
	Pop(cp, chr);

	if (string(world_name) != WORLDNAMELS) return;
	if (string(cName) == "NONE") return;
	std::string sName = cName;
	sName.erase(remove_if(sName.begin(), sName.end(), ::isspace), sName.end());
	if (sName.empty()) return;

	if (!bCheckValidName(cAcc) || !bCheckValidName(cPassword) || !bNewCharCheckValidName(cName)) return;

	wsprintf(G_cTxt, "(!) Request create new Character: %s", cName);
	PutLogList(G_cTxt);

	vector<string> chars;
	auto status = AccountLogIn(cAcc, cPassword, chars);
	if (status != LogIn::Ok || chars.size() >= 4) return;

	char cDir[64] = {};
	wsprintf(cDir, "AscII%d", cName[0]);

	char cFileName[128] = {};
	wsprintf(cFileName, "Database\\Character\\%s\\%s.txt", cDir, cName);

	HANDLE hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		SendLoginMsg(NUCLEO_RESPONSE_NEWCHARACTERFAILED, NUCLEO_RESPONSE_NEWCHARACTERFAILED, 0, 0, iClientH);
		return;
	}

	_mkdir("Database\\Character");
	_mkdir((string("Database\\Character\\") + cDir).c_str());

	FILE* file = fopen(cFileName, "wt");
	if (!file) {
		SendLoginMsg(NUCLEO_RESPONSE_NEWCHARACTERFAILED, NUCLEO_RESPONSE_NEWCHARACTERFAILED, 0, 0, iClientH);
		return;
	}


	char cFile[8192] = {};
	char cTxt[256] = {};
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);

	// --- [FILE-DATE] ---
	strcat(cFile, "[FILE-DATE]\n\n");
	wsprintf(cTxt, "file-saved-date:  Time(%d/%d/%d/%d/%d)", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cFile, cTxt);
	strcat(cFile, "\n\n");

	// --- [NAME-ACCOUNT] ---
	strcat(cFile, "[NAME-ACCOUNT]\n\n");
	strcat(cFile, "character-name     = "); strcat(cFile, cName); strcat(cFile, "\n");
	strcat(cFile, "account-name       = "); strcat(cFile, cAcc); strcat(cFile, "\n");
	strcat(cFile, "account-password   = "); strcat(cFile, cPassword); strcat(cFile, "\n\n");

	// --- [STATUS] ---
	strcat(cFile, "[STATUS]\n\n");
	strcat(cFile, "character-location   = NONE\n");
	strcat(cFile, "character-guild-name = NONE\n");
	strcat(cFile, "character-guild-rank = -1\n\n");

	strcat(cFile, "character-loc-map  = default\n");
	strcat(cFile, "character-loc-x\t  = -1\n");  // Espacios en lugar de tabs
	strcat(cFile, "character-loc-y\t  = -1\n\n");

	strcat(cFile, "character-HP       = 37\n");
	strcat(cFile, "character-MP       = 27\n");
	strcat(cFile, "character-SP       = 22\n");
	strcat(cFile, "character-LEVEL    = 1\n");

	wsprintf(cTxt, "character-STR      = %d", str); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "character-INT      = %d", intl); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "character-VIT      = %d", vit); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "character-DEX      = %d", dex); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "character-MAG      = %d", mag); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "character-CHARISMA = %d", chr); strcat(cFile, cTxt); strcat(cFile, "\n");

	strcat(cFile, "character-LUCK     = 10\n");
	strcat(cFile, "character-EXP      = 0\n");
	strcat(cFile, "character-LU_Pool    = 0\n");
	strcat(cFile, "character-EK-Count  = 0\n");
	strcat(cFile, "character-PK-Count  = 0\n");

	wsprintf(cTxt, "sex-status       = %d", gender); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "skin-status      = %d", skin); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "hairstyle-status = %d", hairstyle); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "haircolor-status = %d", haircolor); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "underwear-status = %d", under); strcat(cFile, cTxt); strcat(cFile, "\n\n");

	wsprintf(cTxt, "appr1 = %d", 358); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "appr2 = %d", 0); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "appr3 = %d", 0); strcat(cFile, cTxt); strcat(cFile, "\n");
	wsprintf(cTxt, "appr4 = %d", 0); strcat(cFile, cTxt); strcat(cFile, "\n\n");

	strcat(cFile, "[ITEMLIST]\n\n");
	strcat(cFile, "character-item = BarrackSword         1 0 0 0 0 0 0 0 0 500 0 0 7\n");
	strcat(cFile, "character-item = RedCandy             1 0 0 0 0 0 0 0 0 300 0 0 -2\n");
	strcat(cFile, "character-item = RedCandy             1 0 0 0 0 0 0 0 0 300 0 0 -2\n");
	strcat(cFile, "character-item = RedCandy             1 0 0 0 0 0 0 0 0 300 0 0 -2\n");
	strcat(cFile, "character-item = RedCandy             1 0 0 0 0 0 0 0 0 300 0 0 -2\n");
	strcat(cFile, "character-item = RedCandy             1 0 0 0 0 0 0 0 0 300 0 0 -2\n");
	strcat(cFile, "character-item = BlueCandy            1 0 0 0 0 0 0 0 0 300 0 41 -2\n");
	strcat(cFile, "character-item = BlueCandy            1 0 0 0 0 0 0 0 0 300 0 41 -2\n");
	strcat(cFile, "character-item = BlueCandy            1 0 0 0 0 0 0 0 0 300 0 41 -2\n");
	strcat(cFile, "character-item = BlueCandy            1 0 0 0 0 0 0 0 0 300 0 41 -2\n");
	strcat(cFile, "character-item = BlueCandy            1 0 0 0 0 0 0 0 0 300 0 41 -2\n");
	strcat(cFile, "character-item = GreenCandy           1 0 0 0 0 0 0 0 0 300 0 83 -3\n");
	strcat(cFile, "character-item = GreenCandy           1 0 0 0 0 0 0 0 0 300 0 83 -3\n");
	strcat(cFile, "character-item = GreenCandy           1 0 0 0 0 0 0 0 0 300 0 83 -3\n");
	strcat(cFile, "character-item = GreenCandy           1 0 0 0 0 0 0 0 0 300 0 83 -3\n");
	strcat(cFile, "character-item = GreenCandy           1 0 0 0 0 0 0 0 0 300 0 83 -3\n");
	strcat(cFile, "character-item = Gold                 1000000 0 0 0 0 0 0 0 0 600 0 138 89\n\n");

	/*bool war = false;
	if (str > mag) war = true;

	bool male = false;
	if (gender == 1) male = true;

	if (war)
	{
		if (male)
		{
			strcat(cFile, "character-item = MasterHeroHelm(M)	  1 2 5352 530 100549167 0 0 0 0 4500 35072 47 90\n");
			strcat(cFile, "character-item = MasterHeroHelm(M)     1 2 31909 530 100550393 0 0 0 0 4500 39168 0 91\n");
			strcat(cFile, "character-item = MasterHeroArmor(M)    1 2 2154 530 100554469 0 0 0 0 5500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroArmor(M)    1 2 27406 530 10055537 0 0 0 0 5500 39168 2 86\n");
			strcat(cFile, "character-item = MasterHeroHauberk(M)  1 2 8647 530 100605867 0 0 0 0 3500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroHauberk(M)  1 2 6782 530 100606661 0 0 0 0 3500 39168 0 82\n");
			strcat(cFile, "character-item = MasterHeroLeggs(M)    1 2 5114 530 100612392 0 0 0 0 4000 35072 44 83\n");
			strcat(cFile, "character-item = MasterHeroLeggs(M)    1 2 2949 530 10061370 0 0 0 0 4000 39168 0 81\n");
			strcat(cFile, "character-item = HunterSword		      1 2 8259 624 135021905 5 0 0 0 4800 0 99 71\n");
			strcat(cFile, "character-item = RedDevastator         1 2 8259 624 135021905 9 0 0 0 4800 -1610600704 99 71\n");
			strcat(cFile, "character-item = RingofMasterwar       1 2 32261 624 135030921 0 0 0 0 300 0 3 37\n");
			strcat(cFile, "character-item = NeckofMasterwar       1 2 29330 624 135042976 0 0 0 0 300 0 46 45\n");
		}
		else
		{
			strcat(cFile, "character-item = MasterHeroHelm(W)      1 2 5352 530 100549167 0 0 0 0 4500 35072 47 90\n");
			strcat(cFile, "character-item = MasterHeroHelm(W)      1 2 31909 530 100550393 0 0 0 0 4500 39168 0 91\n");
			strcat(cFile, "character-item = MasterHeroArmor(W)     1 2 2154 530 100554469 0 0 0 0 5500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroArmor(W)     1 2 27406 530 10055537 0 0 0 0 5500 39168 2 86\n");
			strcat(cFile, "character-item = MasterHeroHauberk(W)   1 2 8647 530 100605867 0 0 0 0 3500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroHauberk(W)   1 2 6782 530 100606661 0 0 0 0 3500 39168 0 82\n");
			strcat(cFile, "character-item = MasterHeroLeggs(W)     1 2 5114 530 100612392 0 0 0 0 4000 35072 44 83\n");
			strcat(cFile, "character-item = MasterHeroLeggs(W)     1 2 2949 530 10061370 0 0 0 0 4000 39168 0 81\n");
			strcat(cFile, "character-item = HunterSword            1 2 8259 624 135021905 5 0 0 0 4800 0 99 71\n");
			strcat(cFile, "character-item = RedDevastator          1 2 8259 624 135021905 9 0 0 0 4800 -1610600704 99 71\n");
			strcat(cFile, "character-item = RingofMasterwar        1 2 32261 624 135030921 0 0 0 0 300 0 3 37\n");
			strcat(cFile, "character-item = NeckofMasterwar        1 2 29330 624 135042976 0 0 0 0 300 0 46 45\n");
		}
	}
	else
	{
		if (male)
		{
			strcat(cFile, "character-item = MasterHeroCap(M)      1 2 5352 530 100549167 0 0 0 0 4500 35072 47 90\n");
			strcat(cFile, "character-item = MasterHeroCap(M)      1 2 31909 530 100550393 0 0 0 0 4500 39168 0 91\n");
			strcat(cFile, "character-item = MasterHeroRobe(M)     1 2 2154 530 100554469 0 0 0 0 5500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroRobe(M)     1 2 27406 530 10055537 0 0 0 0 5500 39168 2 86\n");
			strcat(cFile, "character-item = MasterHeroHauberk(M)  1 2 8647 530 100605867 0 0 0 0 3500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroHauberk(M)  1 2 6782 530 100606661 0 0 0 0 3500 39168 0 82\n");
			strcat(cFile, "character-item = MasterHeroLeggs(M)    1 2 5114 530 100612392 0 0 0 0 4000 35072 44 83\n");
			strcat(cFile, "character-item = MasterHeroLeggs(M)    1 2 2949 530 10061370 0 0 0 0 4000 39168 0 81\n");
			strcat(cFile, "character-item = HunterWand(MS.50)     1 2 8259 624 135021905 5 0 0 0 4800 0 99 71\n");
			strcat(cFile, "character-item = RedZerkWand           1 2 8259 624 135021905 9 0 0 0 4800 -1610600704 99 71\n");
			strcat(cFile, "character-item = RingofMastermage      1 2 32261 624 135030921 0 0 0 0 300 0 3 37\n");
			strcat(cFile, "character-item = NeckofMastermage      1 0 0 0 0 0 0 0 0 1200 0 46 45\n");
		}
		else
		{
			strcat(cFile, "character-item = MasterHeroCap(W)      1 2 5352 530 100549167 0 0 0 0 4500 35072 47 90\n");
			strcat(cFile, "character-item = MasterHeroCap(W)      1 2 31909 530 100550393 0 0 0 0 4500 39168 0 91\n");
			strcat(cFile, "character-item = MasterHeroRobe(W)     1 2 2154 530 100554469 0 0 0 0 5500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroRobe(W)     1 2 27406 530 10055537 0 0 0 0 5500 39168 2 86\n");
			strcat(cFile, "character-item = MasterHeroHauberk(W)  1 2 8647 530 100605867 0 0 0 0 3500 35072 47 83\n");
			strcat(cFile, "character-item = MasterHeroHauberk(W)  1 2 6782 530 100606661 0 0 0 0 3500 39168 0 82\n");
			strcat(cFile, "character-item = MasterHeroLeggs(W)    1 2 5114 530 100612392 0 0 0 0 4000 35072 44 83\n");
			strcat(cFile, "character-item = MasterHeroLeggs(W)    1 2 2949 530 10061370 0 0 0 0 4000 39168 0 81\n");
			strcat(cFile, "character-item = HunterWand(MS.50)     1 2 8259 624 135021905 5 0 0 0 4800 0 99 71\n");
			strcat(cFile, "character-item = RedZerkWand           1 2 8259 624 135021905 9 0 0 0 4800 -1610600704 99 71\n");
			strcat(cFile, "character-item = RingofMastermage      1 2 32261 624 135030921 0 0 0 0 300 0 3 37\n");
			strcat(cFile, "character-item = NeckofMastermage      1 0 0 0 0 0 0 0 0 1200 0 46 45\n");
		}
	}

	strcat(cFile, "character-item = MagicEmerald         1 2 8010 422 60409110 9 300 100 0 300 1 40 30\n");
	strcat(cFile, "character-item = MagicDiamond         1 2 19701 424 1519688 9 300 100 0 300 1 40 30\n");
	strcat(cFile, "character-item = MagicSapphire        1 2 26582 410 24434560 9 300 100 0 300 1 40 30\n");
	strcat(cFile, "character-item = eShadowKnightCape    1 2 28477 530 100644828 0 0 0 0 300 35072 48 88\n");
	strcat(cFile, "character-item = aShadowKnightCape	 1 2 5693 530 100645486 0 0 0 0 300 39168 3 87\n");
	strcat(cFile, "character-item = MagicWand(MS0)       1 0 0 0 0 0 0 0 0 1200 0 120 76\n\n");*/

	// --- [MAGIC-SKILL-MASTERY] ---
	strcat(cFile, "[MAGIC-SKILL-MASTERY]\n\n");
	strcat(cFile, "magic-mastery     = 0000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n\n");

	strcat(cFile, "skill-mastery     = ");
	for (int i = 0; i < 60; i++) strcat(cFile, "0 ");
	strcat(cFile, "\n");

	strcat(cFile, "skill-SSN     = ");
	for (int i = 0; i < 60; i++) strcat(cFile, "0 ");
	strcat(cFile, "\n\n");

	// --- [ITEM-EQUIP-STATUS] ---
	strcat(cFile, "[ITEM-EQUIP-STATUS]\n\n");
	strcat(cFile, "item-equip-status = 10000000000000000000000000000000000000000000000000\n\n");

	// --- [EOF] ---
	strcat(cFile, "[EOF]\n\n\n\n");

	fwrite(cFile, 1, strlen(cFile), file);
	fclose(file);

	SaveAccountInfo(0, cAcc, nullptr, cName, 1, iClientH);

	char cData[512] = {};
	char* cp2 = cData;
	Push(cp2, cName, 10);
	chars.push_back(cName);
	Push(cp2, (int)chars.size());
	GetCharList(cAcc, cp2, chars);
	SendLoginMsg(NUCLEO_RESPONSE_NEWCHARACTERCREATED, NUCLEO_RESPONSE_NEWCHARACTERCREATED, cData, cp2 - cData, iClientH);
}

void cCoreServer::DeleteCharacter(int iClientH, char * pData)
{
	char cName[11] = {};
	char cAcc[11] = {};
	char cPassword[11] = {};
	char world_name[32] = {};

	auto cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 10);
	Pop(cp, cAcc, 10);
	Pop(cp, cPassword, 10);
	Pop(cp, world_name, 30);

	wsprintf(G_cTxt, "(!) Request delete Character: %s", cName);
	PutLogList(G_cTxt);

	vector<string> chars;
	auto status = AccountLogIn(cAcc, cPassword, chars);
	if (status != LogIn::Ok)
		return;

	if (chars.size() == 0)
		return;

	char cDir[112] = {};
	char cTxt[112] = {};
	char cFileName[112] = {};
	strcat(cFileName, "Database\\Character");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cTxt, "AscII%d", *cName);
	strcat(cFileName, cTxt);
	strcpy(cDir, cFileName);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cName);
	strcat(cFileName, ".txt");

	DeleteFile(cFileName);


	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));
	strcat(cFileName, "Database\\Account");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", *cAcc);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, cAcc);
	strcat(cFileName, ".txt");

	ifstream in(cFileName, ios::in);
	if (!in.is_open())
	{
		PutLogList("in is_opem = false");
		return;
	}
	string wordToReplace("account-character-new = ");
	wordToReplace.append(cName);

	string wordToReplace2("account-character-new  = ");
	wordToReplace2.append(cName);
	string wordToReplaceWith("");

	stringstream ss;
	bool done = false;
	string line;
	size_t len = wordToReplace.length();
	while (getline(in, line))
	{
		string repl = wordToReplace;
		size_t pos = line.find(repl);
		len = repl.length();
		if (pos == string::npos)
		{
			repl = wordToReplace2;
			pos = line.find(repl);
			len = repl.length();
		}

		if (pos != string::npos)
		{
			line.replace(pos, len, wordToReplaceWith);
			done = true;
		}

		ss << line << '\n';
	}
	in.close();

	ofstream out(cFileName);
	out << ss.str();


	if (done)
	{
		char cData[512] = {};
		char * cp2 = cData;
		for (auto it = chars.begin(); it != chars.end();)
		{
			if (cName == *it)
			{
				it = chars.erase(it);
				continue;
			}
			else ++it;
		}
		Push(cp2, (int)chars.size());
		GetCharList(cAcc, cp2, chars);
		SendLoginMsg(NUCLEO_RESPONSE_CHARACTERDELETED, NUCLEO_RESPONSE_CHARACTERDELETED, cData, cp2 - cData, iClientH);
	}
}

void cCoreServer::ChangePassword(int iClientH, char * pData)
{
	char cAcc[11] = {};
	char cPassword[11] = {};
	char cNewPw[11] = {};
	char cNewPwConf[11] = {};

	auto cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
	Pop(cp, cAcc, 10);
	Pop(cp, cPassword, 10);
	Pop(cp, cNewPw, 10);
	Pop(cp, cNewPwConf, 10);

	wsprintf(G_cTxt, "(!) Request change password: %s", cAcc);
	PutLogList(G_cTxt);

	vector<string> chars;
	auto status = AccountLogIn(cAcc, cPassword, chars);
	if (status != LogIn::Ok)
		return;

	if (string(cNewPw) != cNewPwConf)
	{
		SendLoginMsg(DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL, DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL, 0, 0, iClientH);
		return;
	}


	char cBuffer[1024] = {};
	char cBuffer2[1024] = {};
	char cTmp[1024] = {};
	char cTxt[1024] = {};
	char cTxt2[1024] = {};
	int iTest = -1;
	wsprintf(cTmp, "Database\\Account\\AscII%d\\%s.txt", cAcc[0], cAcc);
	HANDLE  hFile = CreateFile(cTmp, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		SendLoginMsg(DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL, DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL, 0, 0, iClientH);
		CloseHandle(hFile);
		return;
	}
	auto iSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	char cFileName[512] = {};
	char cDir[112] = {};
	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));
	strcat(cFileName, "Database\\Account");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", *cAcc);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, cAcc);
	strcat(cFileName, ".txt");

	ifstream in(cFileName, ios::in);
	if (!in.is_open())
	{
		PutLogList("in is_opem = false");
		return;
	}
	string wordToReplace("account-password = ");
	wordToReplace.append(cPassword);

	string wordToReplaceWith("account-password = ");
	wordToReplaceWith.append(cNewPw);

	stringstream ss;
	bool done = false;
	string line;
	size_t len = wordToReplace.length();
	while (getline(in, line))
	{
		string repl = wordToReplace;
		size_t pos = line.find(repl);
		len = repl.length();

		if (pos != string::npos)
		{
			line.replace(pos, len, wordToReplaceWith);
			done = true;
		}

		ss << line << '\n';
	}
	in.close();

	ofstream out(cFileName);
	out << ss.str();
	if (done)
	{
		SendLoginMsg(DEF_LOGRESMSGTYPE_PASSWORDCHANGESUCCESS, DEF_LOGRESMSGTYPE_PASSWORDCHANGESUCCESS, 0, 0, iClientH);
	}
}

void cCoreServer::CreateNewAccount(int iClientH, char * pData)
{
	auto lcl = m_pLoginClients[iClientH];
	if (iClientH < 0 || iClientH >= MAXCLIENTLOGINSOCK || !lcl) return;

	char * cp;
	FILE * pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	SYSTEMTIME SysTime;
	char cFile[20000], cBuffer[1024], cFn[1024];
	char cName[12] = {};
	char cPassword[12] = {};
	char cEmailAddr[52] = {};
	char cQuiz[45] = {};
	char cAnswer[20] = {};

	GetLocalTime(&SysTime);

	cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
	Pop(cp, cName, 10);
	Pop(cp, cPassword, 10);
	Pop(cp, cEmailAddr, 50);
	Pop(cp, cQuiz, 45);
	Pop(cp, cAnswer, 20);

	if ((strlen(cName) == 0) || (strlen(cPassword) == 0) ||
		(strlen(cEmailAddr) == 0) || (strlen(cQuiz) == 0) ||
		(strlen(cAnswer) == 0))
		return;

	wsprintf(G_cTxt, "(!) Request create new Account: %s", cName);
	PutLogList(G_cTxt);

	if (!bCheckValidName(cName) || !bCheckValidName(cPassword))
		return;

	ZeroMemory(cFn, sizeof(cFn));
	wsprintf(cFn, "Database\\Account\\AscII%d\\%s.txt", cName[0], cName);
	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	else
	{
		CloseHandle(hFile);
		SendLoginMsg(DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED, DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED, 0, 0, iClientH);
		return;
	}

	mkdir("Database\\");
	mkdir("Database\\Account");
	char Aux = 0;
	Aux = cName[0];
	ZeroMemory(cFn, sizeof(cFn));
	wsprintf(cFn, "Database\\Account\\AscII%d", Aux);
	_mkdir(cFn);

	ZeroMemory(cFn, sizeof(cFn));
	wsprintf(cFn, "Database\\Account\\AscII%d\\%s.txt", Aux, cName);
	pFile = fopen(cFn, "wt");
	if (pFile == NULL)
	{
		SendLoginMsg(DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED, DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED, 0, 0, iClientH);
		return;
	}

	ZeroMemory(cFile, sizeof(cFile));
	strcat(cFile, "Account-generated: ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "Time(%d:%d/%d/%d/%d) IP(%s)", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, m_pLoginClients[iClientH]->_ip);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "[Account Info]");
	strcat(cFile, "\n");

	strcat(cFile, "account-name     = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	memcpy_secure(cBuffer, cName, 10);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "account-password = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	memcpy_secure(cBuffer, cPassword, 10);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "account-Email    = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "%s", cEmailAddr);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "account-Quiz = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "%s", cQuiz);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "account-Answer = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "%s", cAnswer);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");

	strcat(cFile, "account-change-password = ");
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "%d %d %d", SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	strcat(cFile, cBuffer);
	strcat(cFile, "\n");
	strcat(cFile, "\n");

	strcat(cFile, "[CHARACTERS]");
	strcat(cFile, "\n");

	fwrite(cFile, 1, strlen(cFile), pFile);
	fclose(pFile);

	SendLoginMsg(DEF_LOGRESMSGTYPE_NEWACCOUNTCREATED, DEF_LOGRESMSGTYPE_NEWACCOUNTCREATED, 0, 0, iClientH);
}

BOOL cCoreServer::SaveAccountInfo(int iAccount, char * cAccountName, char * cTemp, char * cCharName, char cMode, int iClientH)
{
	char * g_txt = &G_cTxt[0];

	char cFileName[255], cDir[63], cTxt[50], cTxt2[2000], cData[2000];
	int iLine, i;
	int iSize;
	short iMinus;
	int iCharPos = -1;
	int iTest = -1;
	int bDeleteLine;
	HANDLE hFile;
	fpos_t pos;
	DWORD dwFileSize;
	FILE * pFile;

	memset(cData, 0, 2000);
	memset(cTxt2, 0, 2000);
	dwFileSize = 0;
	iLine = 0;
	bDeleteLine = FALSE;
	iMinus = 0;
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTxt2, sizeof(cTxt2));
	ZeroMemory(cData, sizeof(cData));
	ZeroMemory(cFileName, sizeof(cFileName));
	ZeroMemory(cDir, sizeof(cDir));

	strcat(cFileName, "Database\\Account");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	wsprintf(cDir, "AscII%d", *cAccountName);
	strcat(cFileName, cDir);
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cAccountName);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	iSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	switch (cMode) {
	case 1:
		pFile = fopen(cFileName, "at");
		if (pFile == NULL) {
			wsprintf(g_txt, "(X) Account none exist: Name(%s)", cAccountName);
			PutLogList(g_txt);
			return FALSE;
		}
		// Línea con dos espacios antes del =
		wsprintf(cTxt, "\naccount-character-YAP  = %s", cCharName);
		fwrite(cTxt, 1, strlen(cTxt), pFile);
		fclose(pFile);
		return TRUE;

	case 2:
		wsprintf(g_txt, "(X) PasswordChange(%s)", cTemp);
		PutLogList(g_txt);
		wsprintf(cTxt, "account-password = %s", cTemp);
		pFile = fopen(cFileName, "rt");
		if (pFile == NULL) {
			SendLoginMsg(NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, 0, 0, iClientH);
			return FALSE;
		}
		fgetpos(pFile, &pos);
		iSize = fread(cData, 1, iSize, pFile);
		for (i = 0; i < iSize; i++) {
			if (memcmp((char*)cData + i, "[CHARACTERS]", 12) == 0)
				iCharPos = i;
			if (memcmp((char*)cData + i, "account-password = ", 19) == 0)
				iTest = i;
		}
		if (iTest == -1) {
			SendLoginMsg(NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, 0, 0, iClientH);
			fclose(pFile);
			return FALSE;
		}
		else {
			memcpy_secure(cTxt2, cData, iTest + 19);
			memcpy_secure(cTxt2 + iTest + 19, cTxt, strlen(cTxt));
			memcpy_secure(cTxt2 + iTest + 19 + strlen(cTxt), cData + iTest + 19 + strlen(cTemp), iSize - 19 - iTest - strlen(cTemp));
			SaveInfo(cFileName, cTxt2, 1);
		}
		fclose(pFile);
		return TRUE;

	case 3:
	{
		char cTxt3[112] = {};
		wsprintf(g_txt, "(X) Character Delete(%s)", cCharName);
		PutLogList(g_txt);
		wsprintf(cTxt, "account-character-YAP  = %s", cCharName);
		wsprintf(cTxt3, "account-character-YAP  = %s", cCharName);

		pFile = fopen(cFileName, "rt");
		if (pFile == NULL) {
			SendLoginMsg(NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, NUCLEO_RESPONSE_NOTEXISTINGACCOUNT, 0, 0, iClientH);
			return FALSE;
		}
		fgetpos(pFile, &pos);
		iSize = fread(cData, 1, iSize, pFile);
		for (int i = 0; i < iSize; i++) {
			if (memcmp((char*)cData + i, "[CHARACTERS]", 12) == 0)
				iCharPos = i;
			if (memcmp((char*)cData + i, cTxt, strlen(cTxt)) == 0)
				iTest = i;
			if (memcmp((char*)cData + i, cTxt3, strlen(cTxt3)) == 0)
				iTest = i;
		}
		if (iTest == -1) {
			SendLoginMsg(NUCLEO_RESPONSE_NOTEXISTINGCHARACTER, NUCLEO_RESPONSE_NOTEXISTINGCHARACTER, 0, 0, iClientH);
			fclose(pFile);
			return FALSE;
		}
		else {
			memcpy_secure(cTxt2, cData, iTest - 1);
			memcpy_secure(cTxt2 + iTest - 1, cData + iTest + strlen(cTxt), iSize - strlen(cTxt) - iTest);
			SaveInfo(cFileName, cTxt2, 1);
		}
		fclose(pFile);
		return TRUE;
	}
	}
	return FALSE;
}

void cCoreServer::SendLoginMsg(DWORD msg_id, WORD msg_type, char * data, int sz, int iClientH)
{

	int iRet;
	DWORD * dwp;
	char * cp;
	WORD * wp;
	int index = iClientH;

	auto lcl = m_pLoginClients[index];
	if (index < 0 || index >= MAXCLIENTLOGINSOCK || !lcl) return;

	ZeroMemory(G_cData50000, sizeof(G_cData50000));

	dwp = (DWORD *)(G_cData50000 + DEF_INDEX4_MSGID);
	*dwp = msg_id;
	wp = (WORD *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
	*wp = msg_type;

	cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

	memcpy_secure((char *)cp, data, sz);
	if (!lcl->_sock || !IsSocketValid(lcl->_sock)) { DeleteLoginClient(index); return; }

	
	iRet = lcl->_sock->iSendMsg(G_cData50000, sz + 6);

	switch (iRet)
	{
	case DEF_XSOCKEVENT_QUENEFULL:
	case DEF_XSOCKEVENT_SOCKETERROR:
	case DEF_XSOCKEVENT_CRITICALERROR:
	case DEF_XSOCKEVENT_SOCKETCLOSED:
		wsprintf(G_cTxt, "(!) Login Connection Lost on Send (%d)", index);
		PutLogList(G_cTxt);
		DeleteLoginClient(index);
		return;
		break;
	}
	
}

void cCoreServer::SendLoginInfo(int client)
{
	auto p = m_pClientList[client];
	if (!p) return;
}


// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_
#pragma warning(disable : 4996)
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CMD(x)  memcmp(cp, x, strlen(x)) == 0
#include <algorithm>
#include <cctype>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <memory.h>
#include <direct.h>
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard
#include <chrono> // Para manejo de tiempo

#include <numeric> // Para std::iota
#include <utility>
#include <algorithm>
#include <random> // Para generar números aleatorios
#include <cmath> // Para sqrt y pow
#include <ctime> // Para time y localtime

#include "Winmain.h"
#include "StrTok.h"
#include "Xsocket.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "Misc.h"
#include "Msg.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObject.h"
#include "DelayEvent.h"
#include "Fish.h"
#include "DynamicObject.h"
#include "Potion.h"
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "TeleportLoc.h"
#include "teleport.h"
#include "PartyManager.h"

#include "../General/ActionID.h"
#include "../../Shared/netmsg.h"
#include "../General/DynamicObjectID.h"
#include "../General/GlobalDef.h"
#include "../General/MessageIndex.h"
#include "../../Shared/netmsg.h"
#include "../General/UserMessages.h"
#include "../General/Version.h"

#include "../language/englishitem.h"
#include "Event.h"
//#include "../../Shared/shared.h"

//DK EVENT
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

#define DEF_MAXVIEW_X 22    // 19 visibles + 3 margen
#define DEF_MAXVIEW_Y 16    // 14 visibles + 2 margen

using std::vector;

struct st_ggfighters
{
	st_ggfighters() {}
	char fighter[12];
	int client_index;
	short kills;
	short deaths;
	short level;
	short combattype;
	short angelic;
};

struct st_dmfighters
{
	st_dmfighters() {}
	char fighter[12];
	int kills;
	int deaths;
};

enum class LogIn
{
	Ok,
	NoAcc,
	NoPass,
};

using namespace std;

#define MAXRANKEXP 31//20

struct st_trade_ek
{
	st_trade_ek() {}
	short index;
	char itemname[21];
	int eks;
	short sprite;
	short spriteframe;
	short itemcolor;
};

struct st_trade_contrib
{
	st_trade_contrib() {}
	short index;
	char itemname[21];
	int contrib;
	short sprite;
	short spriteframe;
	short itemcolor;
};

// Agrega esta estructura para tracking de objetivos (puede ir en cCoreServer.h)
struct BotTargetAssignment {
	int playerHandle;
	int botCount;
	DWORD lastUpdateTime;
};


struct st_trade_coin
{
	st_trade_coin() {}
	short index;
	char itemname[21];
	int coins;
	short sprite;
	short spriteframe;
	short itemcolor;
};


struct st_trade_token
{
	st_trade_token() {}
	short index;
	char itemname[21];
	int tokens;
	short sprite;
	short spriteframe;
	short itemcolor;
};


struct st_trade_eventpoints
{
	st_trade_eventpoints() {}
	short index;
	char itemname[21];
	int eventpoints;
	short sprite;
	short spriteframe;
	short itemcolor;
};


struct st_trade_majs
{
	st_trade_majs() {}
	short index;
	char itemname[21];
	int majs;
	short sprite;
	short spriteframe;
	short itemcolor;
};



#define MAXTRADEEKITEMS 200
#define MAXTRADETOKENSITEMS 200
#define MAXTRADEEVENTPOINTS 200
#define MAXTRADECONTRIBITEMS 200
#define MAXTRADECOINITEMS 200

enum TradeType
{
	DEF_TRADEEK,
	DEF_TRADECONTRIB,
	DEF_TRADECOIN,
	DEF_TRADEMAJS,
	DEF_TRADEBALLS,
	DEF_STARTERPACK,
	DEF_TRADETOKENS,
	DEF_TRADEEVENTPOINTS,
	DEF_NOTSELECTED
};

struct st_quest
{
	st_quest() {}
	short m_sQuestID;
	char  m_cNpcName[21];
	char  m_cMapName[12];
	char  m_cRealMapName[12];
	int	  m_iMaxKills;
	int   m_iRewardContribution;
	char   m_cQuestReward1[21];
	char   m_cQuestReward2[21];
	char   m_cQuestReward3[21];
};

struct GuildBank {
	std::string guildName;
	int tokens;
	int contribution;
	int majestics;
	std::string password;

	GuildBank() : tokens(0), contribution(0), majestics(0), password("") {}
};

struct st_dkfighters
{
	st_dkfighters() {}
	char fighter[12];
	int client_index;
	short kills;
	short deaths;
	short level;
	short combattype;
	short angelic;
};

struct st_dkbackmember
{
	st_dkbackmember() {}
	char m_cCharName[12];
	int iStr;
	int iInt;
	int iVit;
	int iDex;
	int iMag;
	int iChar;
};


struct st_brfighters
{
	st_brfighters() {}
	char fighter[12];
	int client_index;
	int kills;
	int lobykills;
	short combattype;
	short angelic;
};

struct Coord {
	int x, y;
};

enum DropModes {
	drop_common,
	drop_lesscommon,
	drop_rare,
	drop_epic,
	drop_legendary
};


/*
struct DropItem {
	const char* name;
	int count;
	int attribute;
};

struct NPCDropInfo {
	const char* npcName;
	int baseRep;
	int baseContrib;
	int baseMasteryTokens;
	int baseCoins;
	int baseFragments;
	std::vector<DropItem> legendaryDrops;
};
*/

struct DropItem {
	const char* name;
	int count;
	int attribute;
};

struct NPCDropInfo {
	const char* npcName;
	int baseContrib;
	int baseMajs;
	int baseFragments;
	int baseMaterials;
	std::vector<DropItem> legendaryDrops;

	/*NPCDropInfo(const char* name,
		int contrib,
		int majs,
		int fragments,
		int materials,
		std::vector<DropItem> legendary)

		: npcName(name),
		baseContrib(contrib),
		baseMajs(majs),
		baseFragments(fragments),
		baseMaterials(materials),
		legendaryDrops(std::move(legendary))
	 {
	 }*/
};

struct st_player_shine
{
	short posX;
	short posY;
};

extern Event::Handler g_ev;

#define MAXCLIENTLOGINSOCK		100
#define DEF_SERVERSOCKETBLOCKLIMIT	300//5000 //300//500
#define MAX_MANUFACTURE_ITEMS	10
#define DEF_MAXCLIENTS			200
#define DEF_MAXNPCS				1500
#define DEF_MAXMAPS				110
#define DEF_MAXITEMTYPES		7000
#define DEF_MAXNPCTYPES			900
#define DEF_MAXBUILDITEMS		900
#define DEF_CLIENTTIMEOUT		1000*10
#define DEF_AUTOSAVETIME		300000//600000
#define DEF_HPUPTIME			1000*15
#define DEF_MPUPTIME			1000*15
#define DEF_SPUPTIME			1000*10

#define DEF_HUNGERTIME			1000*60
#define DEF_POISONTIME			1000*12
#define DEF_SUMMONTIME			60000*5
#define DEF_NOTICETIME			80000
#define DEF_PLANTTIME			60000*5	

#define DEF_EXPSTOCKTIME		1000*10	
#define DEF_MSGQUENESIZE		500000 //200000	
#define DEF_AUTOEXPTIME			10000*60*6//1000
#define DEF_TOTALLEVELUPPOINT	3

#define DEF_MAXQUESTLIST 60
#define DEF_MAXCHARQUESTS 30
#define DEF_MAXDROPITEMS 100

#define DEF_MAXDYNAMICOBJECTS	60000//8000 //99999
#define DEF_MAXDELAYEVENTS		60000//8000 //99999
#define DEF_GUILDSTARTRANK		12

#define DEF_CHARPOINTLIMIT		250
#define DEF_RAGPROTECTIONTIME	7000
#define DEF_MAXREWARDGOLD		99999999

#define DEF_ATTACKAI_NORMAL				1
#define DEF_ATTACKAI_EXCHANGEATTACK		2
#define DEF_ATTACKAI_TWOBYONEATTACK		3

#define DEF_MAXFISHS					200
#define DEF_MAXMINERALS					100
#define DEF_MAXENGAGINGFISH				30
#define DEF_MAXPORTIONTYPES				500

#define DEF_MOBEVENTTIME				300000
#define DEF_MAXQUESTTYPE				500

#define DEF_MAXSUBLOGSOCK				10

#define DEF_ITEMLOG_GIVE				1
#define DEF_ITEMLOG_DROP				2
#define DEF_ITEMLOG_GET					3
#define DEF_ITEMLOG_DEPLETE				4
#define DEF_ITEMLOG_NEWGENDROP			5
#define DEF_ITEMLOG_DUPITEMID			6
#define DEF_ITEMLOG_BUY					7 
#define DEF_ITEMLOG_SELL				8     
#define DEF_ITEMLOG_RETRIEVE			9
#define DEF_ITEMLOG_DEPOSIT				10
#define DEF_ITEMLOG_EXCHANGE			11
#define DEF_ITEMLOG_MAGICLEARN			12
#define DEF_ITEMLOG_MAKE				13
#define DEF_ITEMLOG_SUMMONMONSTER		14
#define DEF_ITEMLOG_POISONED			15
#define DEF_ITEMLOG_SKILLLEARN			16
#define DEF_ITEMLOG_REPAIR				17
#define DEF_ITEMLOG_JOINGUILD           18
#define DEF_ITEMLOG_BANGUILD            19
#define DEF_ITEMLOG_RESERVEFIGZONE      20
#define DEF_ITEMLOG_APPLY               21
#define DEF_ITEMLOG_SHUTUP              22
#define DEF_ITEMLOG_CLOSECONN			23
#define DEF_ITEMLOG_SPELLFIELD			24
#define DEF_ITEMLOG_CREATEGUILD			25
#define DEF_ITEMLOG_GUILDDISMISS		26
#define DEF_ITEMLOG_SUMMONPLAYER        27
#define DEF_ITEMLOG_CREATE				28
#define DEF_ITEMLOG_UPGRADEFAIL         29
#define DEF_ITEMLOG_UPGRADESUCCESS      30
#define DEF_ITEMLOG_BUYMARKET			31

#define DEF_CRUSADELOG_ENDCRUSADE       1
#define DEF_CRUSADELOG_STARTCRUSADE     2
#define DEF_CRUSADELOG_SELECTDUTY       3
#define DEF_CRUSADELOG_GETEXP           4

#define DEF_PKLOG_BYPLAYER				1
#define DEF_PKLOG_BYPK					2
#define DEF_PKLOG_BYENERMY				3
#define DEF_PKLOG_BYNPC					4
#define DEF_PKLOG_BYOTHER				5
#define DEF_PKLOG_REDUCECRIMINAL        6

#define DEF_MAXDUPITEMID				100 //5000

#define DEF_MAXGUILDS					200 //1000
#define DEF_MAXONESERVERUSERS			200 //800	

#ifdef DEF_TAIWAN
#define DEF_MAXCONSTRUCTNUM				7
#else
#define DEF_MAXCONSTRUCTNUM				10
#endif
#define MAX_HELDENIANTOWERARESDEN       47
#define MAX_HELDENIANTOWERELVINE        47
#define DEF_MAXSCHEDULE					10
#define MAX_HELDENIANTOWERS              100
#define DEF_TIMEFINALHELDENIAN   300000

#define DEF_MAXFIGHTZONE 10 

//============================			
#define DEF_LEVELLIMIT		20
//============================

//============================
#define DEF_MINIMUMHITRATIO 0
//============================		

//============================
#define DEF_MAXIMUMHITRATIO	99
//============================

//============================
#define DEF_PLAYERMAXLEVEL	150
//============================

//============================
#define DEF_GMGMANACONSUMEUNIT	15
//============================

#define DEF_MAXCONSTRUCTIONPOINT 30000

#define DEF_MAXWARCONTRIBUTION	 900000 //ver

#define DEF_CRUSADELOG_STARTHAPPYHOUR	17
#define DEF_CRUSADELOG_ENDHAPPYHOUR		18

//Nuevo Happy Day LaloRamos
#define DEF_CRUSADELOG_STARTHAPPYDAY	19
#define DEF_CRUSADELOG_ENDHAPPYDAY		20

#define DEF_MAXPARTYMEMBERS		15
#define DEF_MAXPARTYNUM			5000

#define DEF_MAXGIZONPOINT		99999

#define DEF_NETURAL             0
#define DEF_ARESDEN             1
#define DEF_ELVINE              2
#define DEF_BOTHSIDE			100

//v2.19 2002-11-19 
#define DEF_PK					0
#define DEF_NONPK				1
#define DEF_NEVERNONPK			2

enum Pandoras
{
	PANDORA_SMALL,
	PANDORA_MEDIUM,
	PANDORA_BIG,
};


// Definición de la estructura para representar un ítem inicial
struct StarterItem {
	std::string itemName;   // Nombre del ítem
	DWORD itemAttr;   // Atributos del ítem
	int itemCount;          // Cantidad del ítem
};

struct Pits
{
	int pit_handle;
	char m_cMapName[12];
	char m_cNpcName[21];
	short x1;
	short y1;
	short x2;
	short y2;
	int quant;
};

#ifdef DEF_TAIWAN
	#define DEF_MAX_CRUSADESUMMONMOB	5
#else
	#define DEF_MAX_CRUSADESUMMONMOB	500
#endif

#define DEF_LIMITHUNTERLEVEL 100
#define DEF_GUILDCOST	2000
#define DEF_CLEANTIME 600000
////////////////////////////////////
#define DEF_PASSWORD "12011339++"
#define DEF_HIGH////////////////////

#define clWhite							0xFFFFFF
#define clGreen							0x00FF00
#define clRed							0x0000FF

////////////////////////////////////

#define ZERK_ME_MAJESTICS		300 // Cantidad de majestics que necesita para el comando /zerkme
#define MANA_ME_MAJESTICS		50 // Cantidad de majestics que necesita para el comando /maname
#define ZERK_ME_DURATION		600 // Cantidad de segundos que dura el efecto de /zerkme
#define MANA_ME_DURATION		1800 // Cantidad de segundos que dura el efecto de /maname
#define EXP_ME_DURATION			15 // Cantidad de segundos que dura el efecto de /maname
#define DEF_MAXPORTALS			11

static void memcpy_secure(void* dst, const void* src, size_t n) {
	if (!dst || !src || n == 0) return;

	__try {
		memset(dst, 0, n);
		memcpy(dst, src, n);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		// Falla de acceso a memoria, ignorar copia
	}
}

static char * strcpy_secure(char * s1, const char * s2) {
	memcpy_secure(s1, s2, strlen(s2) + 1);
	return s1;
}

struct BattlePlayers {
	std::string name; // Nombre del jugador
	int client_index; // Índice del cliente
	bool isAlive;     // Estado del jugador (vivo o muerto)
};

// Estados del evento
enum EventState {
	BATTLEROYALE_NOT_STARTED,   // El evento no ha comenzado
	BATTLEROYALE_REGISTRATION_TIME,  // Fase de registro
	BATTLEROYALE_FREEZE_TIME,   // Freeze time
	BATTLEROYALE_SAFE_TIME,     // Tiempo seguro
	BATTLEROYALE_COMBAT,        // Fase de combate
	BATTLEROYALE_ENDED          // Evento finalizado
};

struct LoginClient
{
	LoginClient(HWND hWnd)
	{
		_sock = new (std::nothrow) XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
		if (_sock != nullptr)
		{
			_sock->bInitBufferSize(DEF_MSGBUFFERSIZE);
		}
		else
		{
			PutLogList("ERROR: No se pudo crear XSocket en LoginClient");
		}
		_timeout_tm = 0;
	}

	~LoginClient()
	{
		if (_sock != nullptr)
		{
			delete _sock;
			_sock = nullptr;
		}
	}

	u32 _timeout_tm;
	XSocket* _sock;
	char _ip[21];
};


class cCoreServer  
{
public:

	vector<Coord> spawnCoords;
	LoginClient * m_pLoginClients[MAXCLIENTLOGINSOCK];
	std::vector<LoginClient*> m_vLoginClients_Disconn;
	struct PhysItem {
		std::string name;
		int damage;
	};

	std::vector<PhysItem> physItems;

	struct MagicItem {
		std::string name;
		float damage;
	};

	std::vector<MagicItem> magicItems;

	struct CharacterInfo {
		std::string name;
		int ekCount = 0;
		int ekTotal = 0;
		int rating = 0;
		int contribution = 0;
		int pkCount = 0;
	};

	std::vector<CharacterInfo> g_characters;

	// Versión ultra-rápida de trim izquierdo
	inline void fast_ltrim(char*& str) {
		while (*str == ' ' || *str == '\t') str++;
	}

	// Atoi optimizado para nuestro caso específico
	inline int fast_atoi(const char* str) {
		int val = 0;
		while (*str >= '0' && *str <= '9') {
			val = val * 10 + (*str++ - '0');
		}
		return val;
	}

	

	struct st_backmember
	{
		st_backmember() {}
		char m_cCharName[12];
		int iStr;
		int iInt;
		int iVit;
		int iDex;
		int iMag;
		int iChar;
	};

	vector<st_backmember> vec_backmembers;

	std::vector<std::string> m_playersWithStarterItems;
	
	std::vector<std::string> m_processedPlayer;
	std::vector<std::string> m_processedItems;
	std::vector<std::string> m_ProcessedBankItems;
	std::vector<std::string> m_processedMarketItems;
	std::vector<std::string> m_processedDropItems;

	std::vector<std::string> m_sPlayerChangeCordsWhouse;
	std::vector<int> m_vSaveClients;

	std::vector<std::string> g_vBannedIPs;
	std::vector<std::string> g_vBannedMACs;
	std::vector<std::string> g_vBannedNames;

	vector<Recipe> recipes;

	vector<int> m_vHBotClients;

	bool m_bAbaddonEventActive;     // Si el evento está activo
	DWORD m_dwAbaddonEventStartTime; // Tiempo de inicio del evento

	//HelBot
	bool m_bHelBots;
	void startReader();
	
	void updateEntitySync(int owner, short owner_type, short command);
	void response_ping(int client);
	void SaveAllCharacters();
	void OnTimer(char cType);
	void ParseCommand(char* pMsg);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnStartGameSignal();
	BOOL bOnClose();
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL bInit();
	void WriteTradeLog();
	std::string GetProgramDirectory();
	void Quit();
	void DisplayInfo(HDC hdc);
	void ClientMotionHandlerParams(int iClientH, WORD wCommand, short sX, short sY, char cDir, short dX, short dY, short wType, WORD wTargetObjectID, DWORD dwClientTime);
	BOOL bAccept(class XSocket * pXSock);
	char m_cGameServerAddr[16];
	int  m_iGameServerPort;

	cCoreServer(HWND hWnd);
	virtual ~cCoreServer();

	void InitCore(HWND hWnd);
	//private:
	BOOL ConnectSocketMain;

	std::unordered_map<int, DWORD> m_mLastCallTime; // Último tiempo de llamada por facción
	// Agrega este mapa miembro a cCoreServer (puede ir en cCoreServer.h)
	std::unordered_map<int, BotTargetAssignment> m_mTargetAssignments;

	short m_sRelicStatus;
	short m_sAresdenRelics;
	short m_sElvineRelics;

	int m_iElvineKills;
	int m_iAresdenKills;

	void DropItemID(int iClientH, short ItemPos,int iItemID,int iX, int iY, int dwType, int dwValue,int cColor, int Type);
	void SummonCelebrate(int iClientH);

	//ZeroEoyPnk
	bool m_bBotProces;

	void StartCaptureTheFlag();
    void RequestCheckFlag(int iClientH);
    BOOL bCheckIfIsFlagCarrier(int iClientH);
    void SetFlagCarrierFlag(int iClientH, BOOL bFlagMode);
    void EndCaptureTheFlag(short Winner);
//    void _CreateCTFGUID(int iWinnerSide);

	//ZeroEoyPnk
//	void GlobalCountFlag(short Flag);
	void ActualizarCountFlag(short Flag, BOOL Client = FALSE);
	//BOOL ActivarTP;

	//50Cent - Capture The Flag
    BOOL m_bIsCTFMode;
    char m_sElvineFlagCount;
    char m_sAresdenFlagCount;
    BOOL m_bIsElvineFlagCaptured;
    BOOL m_bIsAresdenFlagCaptured;
    int m_iFlagCarrierIndex;

	//RushNet - ZeroEoyPnk
	void LocalStartRushNetMode();
	void LocalEndRushNetMode(int Loser);
	void RemoveRushNetStructures();
	void UpdateRushNet(int Structures, bool Send = false);
	void SendHpReliquia();
	BOOL m_bIsRushNetMode, RushPrice;
	DWORD EndRushPrice_Min, EndRushPrice_Hour;
//	void CheckRushPrice();
	BOOL bReadRushStructureConfigFile(char * cFn);
	void CreateRushStructures();	
	char m_iTowerAresden, m_iTowerElvine;
	BOOL AttackElvine, AttackAresden;

	BOOL ActiveEvent;

	//Abaddon Event
	void InitAbaddonEvent();
	void CloseAbaddonEvent();
//	void ClosePortalAbaddon();
	BOOL AbaddonEvent;
	DWORD dw_abaddon_nospawn;

	//AutoStartEvent
	void EventosAutomaticos();
	void Event_Crusada();
	void Event_TowerDefense();
	void Event_Rush();
	void Event_CatptureTheFlag();
	void Event_AllVsAll();
	void Event_Abbadon();
	void Event_CloseAbbadon();
	void Event_CityVsCity();
	void Event_WorldWar();
	void Event_WorldWarII();
	void Event_Hod();
	void Event_Summon();

	void r_flooritem(int iClientH, char * dat);
	void send_objects_data();
	void SendShinesData(int client, const std::vector<st_player_shine>& shines);
	//void SendObjectsData(int client);
	void NotifyRankData(int client);
	void setrankstats(int iClientH, char * dat);
	void CharacterRankingReset(int client);
	void UnEquipItems(int client);
	void get_object(int client, char * data);
	void ApplyResolution(int iClientH);
	short getMasteryLevel(int client);
	void ReqMobHP(int client, int npc);
	void notifyNpcHealth(int client, int npc);
	void shinning_clear(int client);
	void showShinningStatus();
	void notifyShinningStatus(int client);
	void DkMapUnEquipItems(int client);
	//void StartDkEvent();
	void StartDkEvent(int iClientH);
	//void EndDkEvent();
	void EndAllEks();
	void StartAllDkEks();
	void EndAllDkEks();

	//HeatoN 2024
	void SendCommand(int client, const char * command, int val1 = 0, int val2 = 0);
	void SendUDPCommand(int client, const char * command, int val1 = 0, int val2 = 0);
	void SendAlertMsg(int client, char * pMsg);
	std::vector<int> activePlayers;
	//HeatoN Enchanting
	void handleEnchantingExtract(int client, char * data);
	void AddShard(int client, short type, int points);
	void AddFragment(int client, short type, int points);
	bool PermitedType1(short type);
	bool PermitedType2(short type);
	bool IsPowerHeroItem(char * itemname);
	bool IsMasterHeroItem(char* itemname);
	bool IsMetalItem(char* itemname);
	bool IsShadowKnightSet(char * itemname);
	bool isCape(char * itemname);
	bool isBlockedWeapon(char * itemname);
	CItem* getItemConfigByName(const char* name);
	void handleEnchantingUpgrade(int client, char * data);
	void NotifyNewWeight(int client, short destitem, WORD weight);
	void NotifyNewEndurance(int client, short destitem, int endu, int maxendu);
	void NotifyFragments(int iClientH);

	//HeatoN rebirth config
	struct RebirthLevelSettings {
		int level;
		int exp;
		int goldcost;
		int majcost;
	};

	std::vector<RebirthLevelSettings> vRebirthLevels;

	

	vector<Pits> m_vPits;

	void requestRebirth(int client);
	void switchRebirth(int client);
	int getRebirthIndex(int level);
	void unequipItems(int client);
	void notifyLevelAndStats(int client);
	void copyStats(int client);
	void restoreStats(int client);
	void copyLevel(int client);
	void copyRebirthLevel(int client);
	void enableRebirth(int client);
	void disableRebirth(int client);
	void notify_hp_mp_sp(int client);
	void restoreLevel(int client);
	void restoreRebirthLevel(int client);
	void disableRebirthStatus(int client);
	void enableRebirthStatus(int client);
	void calculatePoints(int client);
	void copyStatsPoints(int client);
	void copyExp(int client);
	void notifyLevelChange(int client);
	char getReadMode(char * token);
	void restoreStatsPoints(int client);
	void restoreExp(int client);
	void calcStatsPoints(int client);
	void calcStatsPointsRebirth(int client);
	void notifyRebirthConfig(int client);

	//void notifyAllShinningStatus();
	void shinning_update(int client);
	//void r_flooritem(int iClientH, char * pdata); //ESTE ES EL FINAL
	//void r_flooritem(int iClientH, char * data);
	//Tower Defence - Event
	void InitTowerDefence();
	void EndTowerDefenceMode(int Loser);
	void CreateTowerAresden();
	void CreateTowerElvine();
	void UpdateTowerDefence(int Structure);
	void RemoveTowerStructures();
	BOOL bReadTDStructureConfigFile(char * cFn);
	BOOL TowerDefense;
	BOOL FlagElvine, FlagAresden;
	void IniciarAllVsAll();
	void EndTowerDefenseTime();

	//All Vs All
	void InitAllVsAll();
	void EndAllVsAll(int Char);
	void ChequearAllVsAll();
	int TotalCompetidores;
	int TotalPrice;
	BOOL AllVsAllPortals, AllVsAll;
	DWORD dwTimeTP, dwTimeOpenTP;

	void DeleteEnergy();

	//Fake Users
	short TotalAdded,TotalDeleted;
	BOOL AddUser, DeletedUser;

	char m_iTowerDefenceElvine, m_iTowerDefenceAresden;
	int DrawTowerDefence;

	BOOL CheckActiveMaps(char * pMapName);

	

	int GolesElvine, GolesAresden;
	BOOL Soccer;
	//BOOL RespawnON;
	void ChequearPartido();

	/* Original*/
	void PlayerCommandCheckAdmins(int iClientH);
	void AdminOrder_CheckRep(int iClientH, char *pData,DWORD dwMsgSize);
	int m_iAddUser;
	BOOL bGetEmptyArea(short, char, short, short, char);
	BOOL bGetNpcMovementArea(short, short, short, char, char);
	void AdminOrder_ChangeCity(int, char *, DWORD);
	BOOL bReadDupeConfigFile(char *);
	void _RevertRareItemValue(CItem* pItem);
	void AdminOrder_Weather(int, char*, DWORD);
	void AdminOrder_Time(int, char *, DWORD);
	void SetAfkFlag(int, char *, DWORD);
	void SetAccountMsg(int, char *, DWORD);
	BOOL bReadMessageConfigFile(char *);
	void GetDkHandler(int iClientH, int iOpcion);
	int m_iExtHGServerPort;
	DWORD m_dwMinute;
	BOOL bAddMagicAngelInt(int iClientH);
	BOOL bCheckAngelInt(int iClientH);
	int iCheckLugar(int iClientH);
	void AutoClearMap();
	void AdminOrder_MonsterCount(int iClientH, char* pData, DWORD dwMsgSize);
	DWORD m_dwAnuncios;
	void Anuncios();
	void AdminOrder_SetZerk(int iClientH, char *pData, DWORD dwMsgSize);
	void MultiplicadorExp(WORD Client, DWORD Exp);
	void AdminOrder_GoTo(int iClientH, char* pData, DWORD dwMsgSize);
	
	BOOL bDecodeTeleportList(char *pFn);
	class CTeleport * m_pTeleportConfigList[DEF_MAXTELEPORTLIST];
	void RequestTeleportListHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestChargedTeleportHandler(int iClientH, char *pData, DWORD dwMsgSize);
	
    DWORD dwCreateGuildGoldCost;
	
	////// COMANDOS //////
	void AdminOrder_Pushplayer(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CleanMap(int iClientH, char * pData, DWORD dwMsgSize);
	void ShowClientMsg(int iClientH, char* pMsg);
	void ShowVersion(int iClientH);
	void AdminOrder_ClearNpc(int iClientH);
	void AdminOrder_ClearMazeNpc(int iClientH);
	void DeleteNpcs();
	void SetInhibitionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	
	void AdminOrder_SetStatus(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_Kill(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Revive(int iClientH, char * pData, DWORD dwMsgSize);

	bool CheckGuildCooldownAndUpdate(const std::string& guildName);
	GuildBank LoadGuildBank(const std::string& guildName);
	void SaveGuildBank(const GuildBank& bank);
	bool GuildDeposit(const std::string& guildName, int tokens, int contrib, int majestics);
	bool GuildWithdraw(const std::string& guildName, int tokens, int contrib, int majestics);
	bool GetGuildBankStatus(const std::string& guildName, GuildBank& outBank);
	void RequestReadGuildBankData(int client);
	void notifyGuildBankPoints(int client);
	void RequestDepositGuildBankPoints(int client, char * data);

	void RequestWithdrawGuildBankPoints(int client, char* data);
    
	void RequestChangeGuildBankPassword(int client, char * data);
	void NotifyGuildMembers(int client, const char* msg);
	bool AuthenticateLogin(XSocket * sock);
	void DeleteLoginClient(int iClientH);
	bool IsSocketValid(XSocket* sock);
	void OnClientLoginRead(int iClientH);
	BOOL bCharCheckValidName(char* pStr);
	void handleSpecialCharacter(int client);
	void handlePowerNeckHealth(int client);
	void handleEagleSpecialCharacter(int client);
	void send_dropitems(int client);
	void add_rare_drop(int client, char* itemname, DWORD itemcount, DWORD attribute);
	void ClearDrops(int client);
	bool DisconnectAll();
	void reqAssambleStones(int client, char* data);
	void CreateAssamblyStone(int client, string itemname);
	void send_charquest_data_byindex(int client, int index);
	bool IsSpecialWeapon(char* itname);
	bool IsFireWeapon(char* itname);
	bool IsIceWeapon(char* itname);
	bool IsLightningWeapon(char* itname);
	void RequestUpgradeSpecialItems(int client, char * data);
	void SendEventList(int client, char * msg, int eventmode);
	void requestChangeTalent(int client, char * data);
	void requestUpgradeTalent(int client, char * data);
	int getNearbyPlayerIndex(int iBot);
	bool handleWhouseMovement(int iBot, CClient* hbot, DWORD dwNow);
	void updateBotTarget(int iBot, CClient* hbot, DWORD dwNow);
	void handleIdleMovement(int iBot, CClient* hbot, DWORD dwNow);
	bool executeBotMovement(int iBot, CClient* hbot, short targetX, short targetY, DWORD dwNow);
	bool executeBotAttack(int iBot, CClient* hbot, CClient* targetPlayer, int targetIdx, DWORD dwNow);
	int calculateDistance(CClient* source, CClient* target);
	void handleBotStateIdle(CClient* hbot, CClient* targetPlayer, DWORD dwNow, int distance);
	void handleBotStateMoving(int iBot, CClient* hbot, CClient* targetPlayer, DWORD dwNow, int distance);
	void handleBotStateAttacking(int iBot, CClient* hbot, CClient* targetPlayer, int targetIdx, DWORD dwNow, int distance);
	int getWandIndex(int iBot);
	void AdminOrder_AmpPlayer(int iClientH, char *pData, DWORD dwMsgSize);
	int getSwordIndex(int iBot);
	void handleCancelation(int iBot);
	bool handleMagicStatus(int iBot, DWORD dwNow);
	bool handleAbymapMovement(int iBot, CClient* hbot, DWORD dwNow);
	void handleBotPotions(int iBot, DWORD dwNow);
	bool bHandleBotWhenDeath(int iBot, CClient* hbot);
	bool bHandleBotTarget(int iBot, CClient* hbot, int targetIdx, CClient* targetPlayer, DWORD dwNow);
	int getNearbyNpcIndex(int iBot);
	void send_charkillquests(int client);
	void start_ek_quest(int client, int quest_id);
	void EnableEkQuestStatus(int client, int quest_id);
	void DisableEkQuestStatus(int client, int quest_id);
	void DeleteEkQuest(int client, int quest_id);
	void DeleteCompletedEkQuest(int client, int quest_id);
	void save_quest_data(int client);
	void save_ekquest_data(int client);
	void save_drops_data(int client);
	void joinPurgeEvent(int client);
	void DisableBloorRite();
	void not_revive_purge(int client);

	void send_charekquest_data_byindex(int client, int index);
	DWORD m_dwLastGuardiansRespawnTime; 
	short roa_event_mode;
	void AdminRequestBanUserName(int client, char * data);
	void AdminRequestChangeEventStatus(int client, char * data);
	void AdminRequestDisconnectPlayer(int client, char * data);
	void AdminRequestBanAll(int client, char * data);
	void AdminRequestBanMAC(int client, char * data);
	void AdminRequestBanIP(int client, char * data);
	void AdminRequestChangeCity(int client, char * data);
	void getClientMacAddress(int client, char * data);
	void HandleSendTotalPlayerList(int client);
	void SendTotalPlayerList(int client);
	void HandleGivePointsCommand(int iAdminH, int iTargetH, const char* pointType, int amount);
	void LoadBanLists();
	void AddBanEntry(const char* type, const std::string& value);
	void not_revive_purge_newround(int client);
	void HandlePurgeNewRound();

	bool isPlayerOnline(char * cAcc, char* cName);
	void RequestCreateNewGuild(int iClientH);
	void SendGuildCreateResponse(int iClientH, bool bSuccess);
	void RequestDisbandGuild(int iClientH);
	void SendDisbandResponse(int iClientH, bool bSuccess);
	void AddNewGuildsmanToFile(int iClientH);
	void RemoveGuildsmanFromFile(int iClientH);
	void CheckGuildIntegrity(int client);
	void testserverping(int client);
	void ReqCreateCraftingHandler(int iClientH, char *pData);
//	void GetCrits(int iClientH);
	class CPortion * m_pCraftingConfigList[DEF_MAXPORTIONTYPES];
	void ResetRep(int client);
	std::string GetGuildBankSubdir(const std::string& guildName);
	void EnsureGuildBankDirectory(const std::string& asciiFolder);
	void GetAngelHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);
	
	std::map<std::string, DWORD> g_guildCooldowns;
	
	void Desbug(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_banjail(int iClientH, char * pData, DWORD dwMsgSize);
    void AdminOrder_ban(int iClientH, char * pData, DWORD dwMsgSize);
	
	void RequestResurrectPlayer(int iClientH, BOOL bResurrect);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	int  iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex);
	BOOL bCheckMagicInt(int iClientH);	// v2.24 11/04/06 17:03
	BOOL bChangeState(char cStateChange, char* cStr, char *cVit,char *cDex,char *cInt,char *cMag,char *cChar); // v2.24 11/04/06 17:03 
	void StateChangeHandler(int iClientH, char * pData, DWORD dwMsgSize); // v2.24 11/04/06 17:03 
	void SetHeroFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetPoisonFlag(short sOwnerH, char cOwnerTpe, BOOL bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	BOOL bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, 
				short sBaseX, short sBaseY, int iItemSpreadType, int iSpreadRange,
				int		*iItemIDs, POINT *BasePos, int *iNumItem); // v2.23 14/03/06 20:50
	int  iSetSide(int iClientH); // 2002-11-15 ClientÀÇ side, onTown, onShop Á¤º¸¸¦ ¼³Á¤
	void RequestHuntmode(int iClientH); // 2002-11-14 »ç³É²Û ¸ðµå Ãß°¡
	void SetNoHunterMode(int iClientH,BOOL bSendMSG = FALSE); 	// v2.20 2002-12-31 ¹Î°£ÀÎ ¸ðµå ·¹º§ Á¦ÇÑ °ú ÀÌµ¿Áö¿ª Á¦ÇÑ Ãß°¡ 
//	BOOL _bCrusadeLog(int iAction,int iClientH,int iData, char * cName);
	void SetForceRecallTime(int iClientH) ; // v2.17 2002-7-15 
	bool bCheckClientMoveFrequency(int iClientH, BOOL running); // v2.171
	void handleVipRegenCharacter(int client);
	void handleRegenTimers(int client);
	void handleRegenTimerLvl1(int client);
	void handleFishing(int client);
	void handleRegenTimerLvl2(int client);
	void handleRegenTimerLvl3(int client);
	void handleRegenTimerLvl4(int client);
	void handleRegenTimerLvl5(int client);
	void withdraw_DropItem(int client, char * data);
	bool countableItems(string itemname);
	void handleDropItem(int client, string itemname, DWORD atr, int count, int itemIndex);
	void IncreaseItemCount(int client, int index, DWORD count);
	void CreateItem(int client, string itemname, DWORD atr, int count, int itemIndex);
	void SendDropLogMsg(int client, char * msg, short dropmode);
	int getItemConfigIndex(char * itemname);
	void AddCharacterLuck(int client);
	void RestartCharacterLuck(int client);
	void NotifyCharacterLuck(int client);
	bool canCraft(int iClientH, const Recipe& recipe);
	void craftItem(int iClientH, const Recipe& recipe);
	void InitializeCraftingRecipes();
	void handleManufactureConfirm(int client, char* data);
	void CreateManuItem(int client, string itemname, DWORD atr, int count);
	void CreateMineralCommand(int client, const string& mineralName, int count);
	void CreateMineral(int client, const string& mineralName, int count);
	bool IsHeroItem(char * itemname);
	void RequestUpgradePowerHero(int client, char * data);
	void SetHasteFlag(short sOwnerH, char cOwnerType, bool bStatus);

	void Request_Fishing(int client, char * data);
	void Request_CancelFishing(int client);
	std::map<std::string, int> LoadDropSettings(const std::string& filename);
	void GetDropSettings();
	void reloadItemsConfig();
	void reloadMagics();
	void ShowPlayerStats(int client);
	void Request_WriteStatus(int client, char * data);
	void SetWriteStatus(int client, bool status);
	void NotifyAddedStats(int client);
	void HandleUpgradeStats(int client, char * data);
	const char* GetStatName(int statIndex);
	void SpecialChatacterReqVit(int client);
	void handleRepairPowerItems(int client);
	bool bCheckSODNeck(int client);
	bool bCheckHellShockWaveNeck(int client);
	bool bCheckGhostAnnihilationNeck(int client);
	int getRareItem(short npctype, short lastTYpe);
	int DropRollDice(int min, int max);
	bool isWyvDrop(char * npcname);
	bool isBossNpc(char * npcname);
	bool isKinightSetNpcs(char * npcname);
	int iRoll(int min, int max);
	void rollDropInBooster(int client, char * npcname);
	void CreateCommonDropItem(int client, char* itemname, char* npcname);
	std::string getCommonItem();
	void rollDropInDropMadness(int client, char* npcname);
	void rollNormalDrop(int client, char * npcname);
	void rollMysticHeroDrop(int client, char * npcname);
	void handleDropRareItem(int client, char * npcname);
	void rollKnightSetDrop(int client, char * npcname);
	void rollBossDrop(int client, char * npcname);
	void rollWyvDrop(int client, char * npcname);
	void DropRareItem(int client, char * npcname, short dropmode);
	NPCDropInfo getNPCDropInfo(const char* npcname);
	int getMultiplierForDropMode(short dropmode);
	DropItem getDropItem(int dado, const NPCDropInfo& npcInfo, int multiplier, short dropmode);
	void handleLegendaryDrop(int client, const char* npcname, const NPCDropInfo& npcInfo);
	void CreateDropItem(int client, char * itemname, DWORD itemcount, DWORD attribute, short dropmode, char * npcname);
	void CreateDropItem(int client, const char* itemname, DWORD itemcount, DWORD attribute, short dropmode, const char* npcname);
	const char* getDropTypeString(short dropmode);
	void broadcastDropMessage(int client, const char* npcname, const char* itemname, DWORD itemcount, const char* dropType, short dropmode);
	void FakeBroadCastMessageHammer();
	void FakeBroadCastMessageSB();
	void FakeDropPlayer(char * charname, char itemname, char npcname);
	void CreateFakeDropItem(char* charname, char* itemname, char* npcname);
	bool IsCurePotion(string itname);
	bool IsSegment(string itname);
	void handleDropByLevel(int client, short mode);
	char * getMineDropName(short dropmode);
	void getFishingCommonDrop(char * &itemname, int &itemcount);
	void getFishingEpicDrop(char * &itemname, int &itemcount);
	void getFishinLegendDrop(char * &itemname, int &itemcount);
	void getFishingRareDrop(char * &itemname, int &itemcount);
	int getMineDropCount(short dropmode);
	char * getMineCommonDropName();
	void CreateMineralDrop(int client, char* itemname, short mode, int count);
	void CreateFishingDrop(int client, char* itemname, short mode, int count);
	void handleFirstDrop(int npc);
	short getType1();
	short getType1Val();
	short getType2();
	short clotesType1();
	short clotesType2();
	short clotesType1Val();
	short clotesType2Val();
	string getClothes();
	string getWeapon();
	short getType2Val();
	void handleItemDrop(std::string map, int id, int x, int y);
	void ItemFirstDrop(CMap* map, int iItemID, int iX, int iY, int dwType, int dwValue, DWORD atr);
	BOOL bCheckClientAttackFrequency(int iClientH, DWORD dwClientTime); // v2.171//anti hack magias
	BOOL bCheckClientMagicFrequency(int iClientH, DWORD dwClientTime); //anti hack magias
	void RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex); // v2.171
	void ArmorLifeDecrement(int iClientH, int sTargetH, char cOwnerType, int iValue);
	BOOL bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH,BOOL bBonus = FALSE) ;
	void RequestItemUpgradeHandler(int iClientH, int iItemIndex);
	void GetExp(int iClientH, int iExp, BOOL bIsAttackerOwn = FALSE);
	void RequestAcceptJoinPartyHandler(int iClientH, int iResult);
	void RefreshAllParties(short Type);
	//	void RequestDeletePartyHandler(int iClientH);
	void RequestDismissPartyHandler(int iClientH);
	void RequestJoinPartyHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestCreatePartyHandler(int iClientH);
	BOOL bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex);
	void ResurrectPlayer(int iClientH);
	void AdminOrder_GetFightzoneTicket(int iClientH);
	void KillCrusadeObjects();

	BOOL bCopyItemContents(class CItem * pOriginal, class CItem * pCopy);

	int  iGetMapLocationSide(char * pMapName);

	void ManualEndCrusadeMode(int iWinnerSide);
	void RemoveClientShortCut(int iClientH);
	BOOL bAddClientShortCut(int iClientH);
	void RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName);
	void CheckCommanderConstructionPoint(int iClientH);
//	void GlobalStartCrusadeMode();
//	void GlobalEndCrusadeMode();
	void SyncMiddlelandMapInfo();
//	void _GrandMagicLaunchMsgSend(int iType, char cAttackerSide);

	void GrandMagicResultHandler(char * cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iSTCount,char * pData);
	void CalcMeteorStrikeEffectHandler(int iMapIndex);
	void DoMeteorStrikeDamageHandler(int iMapIndex);
	
//	void ServerStockMsgHandler(char * pData);

	void RemoveCrusadeStructures();

	void RecallHunterPlayer();

	void _SendMapStatus(int iClientH);
	void MapStatusHandler(int iClientH, int iMode, char * pMapName);
	void SelectCrusadeDutyHandler(int iClientH, int iDuty);
	
	void CheckConnectionHandler(int iClientH, char *pData);
	void RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode);
	void RequestGuildTeleportHandler(int iClientH);
	void RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName);
	void MeteorStrikeHandler(int iMapIndex);
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void _NpcBehavior_GrandMagicGenerator(int iNpcH);
	void CollectedManaHandler(WORD wAresdenMana, WORD wElvineMana);
	void SendCollectedMana();
	void CreateCrusadeStructures();

	BOOL bReadCrusadeStructureConfigFile(char * cFn);
	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(DWORD dwGuildGUID);
	void CheckCrusadeResultCalculation(int iClientH);
	BOOL _bNpcBehavior_Detector(int iNpcH);
	BOOL _bNpcBehavior_ManaCollector(int iNpcH);
	BOOL __bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH);
	BOOL __bSetAgricultureItem(int iMapIndex, int dX, int dY, int iType, int iSsn,int iClientH);   //v2.19 2002-12-16 ³ó»ç½ºÅ³
	BOOL bCropsItemDrop(int iClientH, short iTargetH,BOOL bMobDropPos = FALSE);												//v2.19 2002-12-16 ³ó»ç½ºÅ³
	int bProbabilityTable(int x,int y,int iTable);												//v2.19 2002-12-16 ³ó»ç ½ºÅ³ °ü·Ã
	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();

//	BOOL bGetItemNameWhenDeleteNpc(int & iItemID, short sNpcType, int iItemprobability);

	int iGetItemWeight(class CItem * pItem, int iCount);
	void CancelQuestHandler(int iClientH, short Type);
	void ActivateSpecialAbilityHandler(int iClientH);
//	void EnergySphereProcessor(BOOL bIsAdminCreate = FALSE, int iClientH = NULL);
	BOOL bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType);
	void GoalPoint(int GoalPoint, short Goleador);
	void CreateEnergy();
	void JoinPartyHandler(int iClientH, int iV1, char * pMemberName);
	void NewJoinPartyHandler(int iClientH, int iV1, char * pMemberName);
	void RequestSellItemListHandler(int iClientH, char * pData);
	void HandleFakeDropCommand(int iClientH, char *pData, DWORD dwMsgSize);
	bool checkNpcName(char * npcname);
	bool checkItemName(char * itemname);
	void AdminOrder_CreateItem(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestRestartHandler(int iClientH);
	bool CheckMakeString(char *pStr);
	void AdminOrder_SetObserverMode(int iClientH);
	void SetObserverMode(int client, bool status);
	int iRequestPanningMapDataRequest(int iClientH, char * pData);
	void GetMagicAbilityHandler(int iClientH);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, BOOL bExp, int iAttr, BOOL bFly, int iMagicNum = -1);
	void _TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY);
//	void RequestCheckAccountPasswordHandler(char * pData, DWORD dwMsgSize);
	int _iTalkToNpcResult_Guard(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void SetIceFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void _bDecodeNoticementFileContents(char * cFn);
	void RequestNoticementHandler(int iClientH, char * pData);
	void _AdjustRareItemValue(class CItem * pItem);
	void NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType);
	int GetNpcGenLevel(short npcType);
	CItem* GenerateGoldDrop(CNpc* pNpc, CClient* pAttacker);
	CItem* GenerateStandardDrop(int iNpcH);
	CItem* GenerateSpecialDrop(int iGenLevel);
	int GenerateMeleeWeapon(int iGenLevel);
	int GenerateMagicWand(int iGenLevel);
	int GenerateArmor(int iGenLevel);
	void ProcessGeneratedItem(CItem* pItem, int iGenLevel, BOOL& SaveDrop, CNpc* pNpc);
	int  iGetPlayerABSStatus(int iWhatH, int iRecvH); // 2002-12-2
	void AdminOrder_DisconnectAll(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Summon(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_LizePlayer(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_SummonAll(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_SummonPlayer(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_deslogpull(int iClientH, char * pData, DWORD dwMsgSize);

	char _cGetSpecialAbility(int iKindSA);
	void AdminOrder_UnsummonBoss(int iClientH);
	void AdminOrder_UnsummonAll(int iClientH);
	void AdminOrder_SetAttackMode(int iClientH, char * pData, DWORD dwMsgSize);
	int RollDice(int min, int max);
	void handleTreasure(int client);
	void PandoraBox(int client, short type);
	void AdminOrder_SetForceRecallTime(int iClientH, char * pData, DWORD dwMsgSize);
	
	void BuildItemHandler(int iClientH, char * pData);
	BOOL _bDecodeBuildItemConfigFileContents(char * cFn);
	BOOL _bCheckSubLogSocketIndex();
//	void _CheckGateSockConnection();

	std::chrono::steady_clock::time_point g_lastBattleRoyaleTime;
	std::chrono::steady_clock::time_point g_lastBattleRoundRoyaleTime;
	BOOL _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, BOOL bForceItemLog = FALSE) ;
	BOOL _bItemLog(int iAction, int iClientH, char * cName, class CItem * pItem, BOOL Save = TRUE);

	BOOL _bPKLog(int iAction, int iAttackerH, int iVictumH, char * cNPC, BOOL Blocked = FALSE) ;
	BOOL _bCheckGoodItem( class CItem * pItem ); 

	void OnSubLogRead(int iIndex);
	void GetMapInitialPoint(int iMapIndex, short * pX, short * pY, char * pPlayerLocation = NULL);
	void AdminOrder_SummonDemon(int iClientH);
	int  iGetMaxHP(int iClientH);
	void _ClearQuestStatus(int iClientH, short Type);
	void SendItemNotifyMsg(int iClientH, WORD wMsgType, class CItem * pItem, int iV1);

	BOOL _bCheckIsQuestCompleted(int iClientH, short Type);
	void _CheckQuestEnvironment(int iClientH);
	void _SendQuestContents(int iClientH);
	void QuestAcceptedHandler(int iClientH);
	BOOL _bDecodeQuestConfigFileContents(char * cFn);
	void CancelExchangeItem(int iClientH);
	BOOL bAddItem(int iClientH, class CItem * pItem, char cMode);
	void ConfirmExchangeItem(int iClientH);
	void SetExchangeItem(int iClientH, int iItemIndex, int iAmount);
	void ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char * pItemName);
	void _BWM_Command_Shutup(char * pData);
	
	void CheckUniqueItemEquipment(int iClientH);
	void _SetItemPos(int iClientH, char * pData);
	void GetHeroMantleHandler(int iClientH, int iItemID); //void GetHeroMantleHandler(int iClientH);
	int  CheckHeroItemEquipHandler(int iClientH);
	
	int  _iComposeFlagStatusContents(char * pData);
	void SetSummonMobAction(int iClientH, int iMode, DWORD dwMsgSize, char * pData = NULL);
	BOOL __bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, BOOL bAdminFlag);
	BOOL _bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iClientH, int iSkillIndex);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	int  _iGetWeaponSkillType(int iClientH);
	void AdminOrder_GetNpcStatus(int iClientH, char * pData, DWORD dwMsgSize);
	void CheckFireBluring(char cMapIndex, int sX, int sY);
	void NpcTalkHandler(int iClientH, int iWho, int Quest);
	BOOL bDeleteMineral(int iIndex);
	void _CheckMiningAction(int iClientH, int dX, int dY);
	int iCreateMineral(char cMapIndex, int tX, int tY, char cLevel);
	void handleMineralAction(int iMinIndex);
	void MineralGenerator();
	BOOL _bDecodePortionConfigFileContents(char * cFn);
	void ReqCreatePortionHandler(int iClientH, char * pData);
	void _CheckAttackType(int iClientH, short * spType);
	void AdminOrder_SetInvi(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_Polymorph(int iClientH, char * pData, DWORD dwMsgSize);
	void ForceDisconnectAccount(char * pAccountName, WORD wCount);
	void NpcRequestAssistance(int iNpcH);
	void ToggleSafeAttackModeHandler(int iClientH);
	void AdminOrder_CheckIP(int iClientH, char * pData, DWORD dwMsgSize);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SpecialEventHandler();
	int _iForcePlayerDisconect();
	void AdminOrder_Teleport(int iClientH, char * pData, DWORD dwMsgSize);
	int iGetMapIndex(char * pMapName);
	int iGetNpcRelationship(int iWhatH, int iRecvH);
	int iGetPlayerRelationship(int iClientH, int iOpponentH);
	int iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus);
	void WhetherProcessor();
	int _iCalcPlayerNum(char cMapIndex, short dX, short dY, char cRadius);
	void FishGenerator();
	void ReqGetFishThisTimeHandler(int iClientH);
	void AdminOrder_CreateFish(int iClientH, char * pData, DWORD dwMsgSize);
	void FishProcessor();
	int iCheckFish(int iClientH, char cMapIndex, short dX, short dY);
	BOOL bDeleteFish(int iHandle, int iDelMode);
	int  iCreateFish(char cMapIndex, short sX, short sY, short sDifficulty, class CItem * pItem, int iDifficulty, DWORD dwLastTime);
	void UserCommand_BanGuildsman(int iClientH, char * pData, DWORD dwMsgSize);
	void RemoveFromGuild(int iClientH);

	void ShowPlayerSegments(int client);

	void AdminOrder_ReserveFightzone(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CloseConn(int iClientH, char * pData, DWORD dwMsgSize);
	void AdminOrder_CallGuard(int iClientH, char * pData, DWORD dwMsgSize);
	int iGetExpLevel(int iExp);
	void ___RestorePlayerRating(int iClientH);
	void CalcExpStock(int iClientH);
	void ResponseSavePlayerDataReplyHandler(char * pData, DWORD dwMsgSize);
	void SetPlayerReputation(int iClientH, char * pMsg, char cValue, DWORD dwMsgSize);
	void ShutUpPlayer(int iClientH, char * pMsg, DWORD dwMsgSize);
	void CheckDayOrNightMode();
	BOOL bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void PoisonEffect(int iClientH, int iV1);
	void bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, BOOL bIsPermAttack);
	BOOL _bGetIsPlayerHostile(int iClientH, int sOwnerH);
	BOOL bAnalyzeCriminalAction(int iClientH, short dX, short dY, BOOL bIsCheck = FALSE);
	int _iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange);
	void CalcTotalItemEffect(int iClientH, int iEquipItemID, BOOL bNotify = TRUE);
	void ___RestorePlayerCharacteristics(int iClientH);
	void GetPlayerProfile(int iClientH, char * pMsg, DWORD dwMsgSize);
	void SetPlayerProfile(int iClientH, char * pMsg, DWORD dwMsgSize);
	void ToggleWhisperPlayer(int iClientH, char * pMsg, DWORD dwMsgSize);
	void CheckAndNotifyPlayerConnection(int iClientH, char * pMsg, DWORD dwSize);
	int iCalcTotalWeight(int client);
	void ReqRepairItemCofirmHandler(int iClientH, char cItemID, char * pString);
	void ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char * pString);
	void ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char * pString);
	void ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char * pItemName);
	void UseSkillHandler(int iClientH, int iV1, int iV2, int iV3);
	int  iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY);
	void ClearSkillUsingStatus(int iClientH);
	bool IsInFishingArea(int x, int y);
	void DynamicObjectEffectProcessor();
	void SendObjectMotionRejectMsg(int iClientH);
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	BOOL bRemoveFromDelayEventList(int iH, char cType, int iEffectType);
	void DelayEventProcessor();
	BOOL bRegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int  _iCalcSkillSSNpoint(int iLevel);
	
	
	void rank_operation(int iClientH);
	void notify_rankup(int iClientH);
	void notify_rankexp(int iClientH);
	BOOL bCheckTotalSkillMasteryPoints(int iClientH, int iSkill);
	BOOL bSetItemToBankItem(int iClientH, class CItem * pItem);
	void NpcMagicHandler(int iNpcH, short dX, short dY, short sType);
	BOOL bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	BOOL bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio, char cNpc = 0);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, BOOL bExp, int iAttr = NULL, int iMagicNum = -1);
	void UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID);
	void NpcBehavior_Stop(int iNpcH);
	// v2.15 
	void ItemDepleteHandler(int iClientH, short sItemIndex, BOOL bIsUseItemResult, BOOL bIsLog = TRUE);
	int _iGetArrowItemIndex(int iClientH);
	void RequestFullObjectData(int iClientH, char * pData);
	void DeleteNpc(int iNpcH, BOOL bHeld = TRUE, BOOL Drop = TRUE);
	void CalcNextWayPointDestination(int iNpcH);
	void MobGenerator();
	void CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue);
	void CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue);
	void CheckDynamicObjectList();
	int  iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwLastTime, int iV1 = NULL);
	int _iCalcMaxLoad(int iClientH);
	void GetRewardMoneyHandler(int iClientH);
	void _PenaltyItemDrop(int iClientH, int iTotal, BOOL bIsSAattacked = FALSE,BOOL bItemDrop = FALSE);
	void ApplyCombatKilledPenalty(int iClientH, char cPenaltyLevel, BOOL bIsSAattacked = FALSE, BOOL bItemDrop = FALSE);
	void CreateItemDrop(int client, string map, int id, int x, int y);
	
	void CreateItemDrop(int client, string map, int id, int count, int x, int y);
	void dropitem(int client, int iItemID, int iX, int iY, int dwType, int dwValue, int cColor);
	void EnemyKillRewardHandler(int iAttackerH, int iClientH);
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumLevel);
	BOOL bSetItemToBankItem(int iClientH, short sItemIndex);
	void RequestRetrieveItemHandler(int iClientH, char * pData);
	void RequestCivilRightHandler(int iClientH, char * pData);
	void RequestCivilHandler(int iCliehtH);
	BOOL bCheckLimitedUser(int iClientH);
	void LevelUpSettingsHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void FightzoneReserveHandler(int iClientH, char * pData, DWORD dwMsgSize);
	BOOL bCheckLevelUp(int iClientH);
	int iGetLevelExp(int iLevel);
	void TimeManaPointsUp(int iClientH);
	void TimeStaminarPointsUp(int iClientH);
	BOOL __bReadMapInfo(int iMapIndex);
	void TrainSkillResponse(BOOL bSuccess, int iClientH, int iSkillNum, int iSkillLevel);
	int _iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost);
	void RequestStudyMagicHandler(int iClientH, char * pName, BOOL bIsPurchase = TRUE);
	BOOL _bDecodeSkillConfigFileContents(char * cFn);
	BOOL _bDecodeMagicConfigFileContents(char * cFn);
	void ReleaseFollowMode(short sOwnerH, char cOwnerType);
	BOOL bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType);
	void RequestTeleportHandler(int iClientH, char * pData, char * cMapName = NULL, int dX = -1, int dY = -1);
	void PlayerMagicHandler(int iClientH, int dX, int dY, short sType, BOOL bItemEffect = FALSE, int iV1 = NULL, char * m_cTargetName = NULL);
	int  iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir);
	void ToggleCombatModeHandler(int iClientH); 
	void SendGuildMsg(int iClientH, WORD wNotifyMsgType, short sV1, short sV2, char * pString);
	void TimeHitPointsUp(int iClientH);
	void handleBlockMaps(int client);
	int getMapIndex(char * mapname);
	void createMineral(int client);
	int calcTotalMinerals();
	void MineralGen();
	void handleRelicStatus(int client, short newstatus);
	void get_pits();
	void MobSpotGenerator();
	bool bCreateNewNpc2(int pitnum, char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned = FALSE, bool bFirmBerserk = FALSE, bool bIsMaster = FALSE, int iGuildGUID = NULL);

	void SummonMob(char * npcname, char * mapname, int x, int y, char side, int npc, int cantidad, int pitnum, bool sademob = false, short nside = -1);
	int Checkmobcount(char * mapname, char * npcname, int pitnum);
	int iDice(int iThrow, int iRange);
	BOOL _bInitNpcAttr(class CNpc * pNpc, char * pNpcName, short sClass, char cSA);
	BOOL _bDecodeNpcConfigFileContents(char * cFn);
	void ReleaseItemHandler(int iClientH, short sItemIndex, BOOL bNotice);
	void ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage);
	int  SetItemCount(int iClientH, char * pItemName, DWORD dwCount);
	int  SetItemCount(int iClientH, int iItemIndex, DWORD dwCount);
	void EnableCVC();
	void SendCVCKillsData();
	void SendCVCKillsDataOnePlayer(int client);
	void Purge_KillEnemy(int att, int tar);
	DWORD dwGetItemCount(int iClientH, char* pName);
	void DismissGuildRejectHandler(int iClientH, char * pName);
	void DismissGuildApproveHandler(int iClientH, char * pName);
	void JoinGuildRejectHandler(int iClientH, char * pName);			    
	void JoinGuildApproveHandler(int iClientH, char * pName);
	void SendNotifyMsg(int iFromH, int iToH, WORD wMsgType, DWORD sV1, DWORD sV2, DWORD sV3, char * pString, DWORD sV4 = NULL, DWORD sV5 = NULL, DWORD sV6 = NULL, DWORD sV7 = NULL, DWORD sV8 = NULL, DWORD sV9 = NULL, char * pString2 = NULL, char cValueA = NULL);
	void GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char * pItemName);
	void RequestPurchaseItemHandler(int iClientH, char * pItemName, int iNum);
//	void RequestPurchaseItemHandler2(int iClientH, char * pItemName, int iNum); // MORLA 2.4 - Para el trade items
	void ResponseDisbandGuildHandler(char * pData, DWORD dwMsgSize);
	void RequestDisbandGuildHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestCreateNewGuildHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void ResponseCreateNewGuildHandler(char * pData, DWORD dwMsgSize);
	int  iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir);
	void NotifyWhenSpecialItem(int client/*, int itemIndex*/);
	void SetFireStatus(int client, bool status, int attacker = -1);
	void SetNewIceStatus(int client, bool status);
	void SetLightingStatus(int client, bool status);
	void handleFireStatusActions(int client);
	void handleLightingStatusActions(int client);
	void CreateCandyItem(int client, string itemname);
	void CheckCombatTimeout(int client);
	void keepAlive(int client);
	bool IsDkMap(char * mapname);
	bool IsInSafeZone(int client, short type);
	void handleRecallPoint(int client, char * data);
	void handleRelay();
	void EkQuestOperation(int client);
	int getAttackerIndex(char * cFireIndex);
	void NotifyFireStatus(int client);
	BOOL bEquipItemHandler(int iClientH, short sItemIndex, BOOL bNotify = TRUE);
	BOOL _bAddClientItemList(int iClientH, class CItem * pItem, int * pDelReq);
	BOOL _bAddClientItemListWithoutWeight(int iClientH, class CItem * pItem, int * pDelReq);
	int  iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir);
	void DropItemHandler(int iClientH, short sItemIndex, int iAmount, char * pItemName, BOOL bByPlayer = FALSE);
	void ClientCommonHandler(int iClientH, char * pData);
	BOOL __fastcall bGetMsgQuene(char * pFrom, char * pData, DWORD * pMsgSize, int * pIndex, char * pKey);
	void MsgProcess();
	BOOL __fastcall bPutMsgQuene(char cFrom, char * pData, DWORD dwMsgSize, int iIndex, char cKey);
	void NpcBehavior_Flee(int iNpcH);
	void handleReviveOnEvents(int client);
	void handleNotReviveOnEvents(int client);
	void reqStarterItems(int client, char * data);
	int iGetDangerValue(int iNpcH, short dX, short dY);
	void NpcBehavior_Dead(int iNpcH, BOOL Drop = TRUE);
	void NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage, BOOL bHeld = TRUE);
	int  iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, BOOL bNearAttack = FALSE, BOOL bIsDash = FALSE, BOOL bArrowDirection = FALSE);
	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = NULL);
	void NpcBehavior_Attack(int iNpcH);
	void TargetSearch(int iNpcH, short * pTarget, char * pTargetType);
	void NpcBehavior_Move(int iNpcH);
	BOOL bGetEmptyPosition(short * pX, short * pY, char cMapIndex);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int * pError, char cArea = 0, short sOwnerH = 0);
	int  iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, WORD wTargetObjectID, BOOL bRespose = TRUE, BOOL bIsDash = FALSE);
	void ChatMsgHandler(int iClientH, char * pData, DWORD dwMsgSize);
	void NpcProcess();
	BOOL bCreateNewNpc(char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, BOOL bHideGenMode, BOOL bIsSummoned = FALSE, BOOL bFirmBerserk = FALSE, BOOL bIsMaster = FALSE, int iGuildGUID = NULL);
	void handleCommonNpc(int iNpc);

	void HelBotProcess();
	bool isPlayerInSafeZone(CClient* player);
	void handleHelBots(int iBot);
	void eliminarCliente(int iClientH);
	std::string getRandomMessage(const std::vector<std::string>& messages);
	void sendBotKillMessages(int iBot);
	void sendBotDeathMessages(int iBot);
	void ShowBotMsg(int iBot, int iClientH, char* pMsg);
	void handleHelBotReward(int client, char * botname);
	void CallBotsForFaction(int iClient);
	bool handleBotCallMovement(int iBot, CClient* hbot, DWORD dwNow);
	void AutoElvLoginAll();
	void handleLoginBots();
	void restoreBotToSafe();
	void AutoAresLoginAll();
	void AutoLoginAll1();
	void AutoLoginAll2();
	void AutoLoginAll();
	char GetOppositeDirection(char cDir);
	void HelBotBehavior_Move(int iNpcH);
	void HelBotBehavior_Attack(int iNpcH);
	void HelBotBehavior_Flee(int iNpcH);
	void HelBotBehavior_Stop(int iNpcH);
	void HelBotTargetSearch(int iNpcH, short * pTarget, char * pTargetType);
	BOOL bCreatePitNpc(int pitnum, char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, BOOL bHideGenMode, BOOL bIsSummoned = FALSE, BOOL bFirmBerserk = FALSE, BOOL bIsMaster = FALSE, int iGuildGUID = NULL);
	void DeleteStones();
	BOOL _bReadMapInfoFiles(int iMapIndex);
	bool m_bSummonStones;
	short m_iCTRFinishCount;
	short m_sSummonStones;
	short m_sBotLevel;
	void DeleteGuards();
	
	void SendRelicCords(int client);
	void SendFloorRelicCords();
	void ManageEvents();
	void fixItemsDk(int client, int itemindex);
	void FinishDKSetEvent();/*
	void processCharacterFile(const std::string& filePath);
	void processDirectory(const std::string& directory);*/
	void printCharacters();
	void handleRequestRanking(int client, short rankmode);
	void processCharacterFile(const std::string& filePath, const std::string& charName);
	void processDirectory(const std::string& directory);
	bool IsBlockedCharacter(const char* name);
	bool IsNameBlocked(const char* name);
	int GetClientIndexByName(char * charname);
	void GiveStarterItems(int client, short type);
	void NotifyGunGameLeader(string leader);
	void FinishBattleRoyaleEvent();
	void handleCTRActions(int client);
	void checkInviFlag(int client);
	void LearnMagics(int client);
	void RestoreMagics(int client);
	void RemoveFlags(int client);
	bool IsInsideZone(int x, int y, int zoneX, int zoneY);
	void SavePlayerItemForEvent(int client);
	void RestorePlayerItems(int client);
	void NotRelicStatus(int client);
	void handleRelicReward(int client);
	void notifyRelicPlanted(int client);
	void notifyRelicCaptured(int client);
	void handleCTRTimer();
	void CreateCandys(int client);
	void CreateBluePotions(int client);
	void CreateRedPotions(int client);
	void CreateGreenPotions(int client);
	void handlePlayerForEvent(int client, short combattype);
	void restorePlayerStatsAfterEvent(int client);
	void CreateGunGameItem(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	void CreateGunGameCandy(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	void CreateStarterItem(int client, string itemname, DWORD atr, int count);
	void LoadPlayersWithStarterItems();
	void SavePlayersWithStarterItems();
	bool HasPlayerReceivedStarterItems(const std::string& playerName);
	void AddPlayerToStarterItemsList(const std::string& playerName);
	bool HasEnoughInventorySpace(int client, int needcount);
	void handleRecallTeleport(int client);
	void loadPhysItemDamageConfig(const std::string& filename);
	void updateDamages();
	void displayPhysItems() const;
	void displayMagicItems() const;
	float getMagicDamage(const std::string& itemName);
	float getMagicDamage(char* itemName);
	void displayItems() const;
	void updatePhysConfig(const std::string& filename);
	int getphysDamage(char * itemname);
	void loadMagicItemDamageConfig(const std::string& filename);
	void updateMagicConfig(const std::string& filename);
	void getConfigDamages();
	void getDamages(float & add_magicdamage, float & rem_magicdamage, int & add_physdamage, int & rem_physdamage);
	void requestActiveRole(int client, char * data);
	void requestUpgradeRole(int client, char * data);
	void getDamagesCorrection();
	void CreateSets(int client, short combattype);
	void NotifyGunGameItems(int client);
	void CreateWeapons(int client, short combattype);
	void ReloadCandys(int client);
	void GetBluePotions(int client);
	void GetRedPotions(int client);
	void GetGreenPotions(int client);
	void MakeItems(int client, short combattype);
	void FinishGunGameEvent();
	void handleAngelicActions(int client);
	void DeleteAngelicStats(int client);
	BOOL _bGetIsStringIsNumber(char * pStr);
	BOOL _bInitItemAttr(class CItem * pItem, char * pItemName);
	// v2.17 2002-7-31 ¾ÆÀÌÅÛÀ» ¾ÆÀÌÅÛ °íÀ¯¹øÈ£·Î »ý¼ºÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
	BOOL _bInitItemAttr(class CItem * pItem, int iItemID);
	BOOL bReadProgramConfigFile(char * cFn);
	void GameProcess();
	void InitPlayerData(int iClientH, char * pData, DWORD dwSize, bool isBot = false);
	void NotifyRelicStatus();
	void checkShadowSetEndurance(int client);
	void NotifyCInsert(int client);
	void NotifyCDelete(int client);
	//agregado lalov9
	BOOL bCheckClientStatLvl(int iClientH);
	void ResponsePlayerDataHandler(char * pData, DWORD dwSize);

	BOOL bSendMsgToLS(DWORD dwMsg, int iClientH, BOOL bFlag = TRUE,char * pData = NULL );
	void OnMainLogRead();
	void CheckClientResponseTime();
	
	void RemoveOccupyFlags(int iMapIndex);
	int iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char * pData);
	void AutoSaveClientsAsync();
	void setupAutoSave();
	void handleAutoSave();
	void handleTimers(DWORD dwTime);
	bool IsWinsockInitialized();
	void SendEventToNearClient_TypeB(DWORD dwMsgID, WORD wMsgType, char cMapIndex, short sX, short sY, short sV1 = 0, short sV2 = 0, short sV3 = 0, short sV4 = 0);
	void SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, DWORD dwMsgID, WORD wMsgType, short sV1, short sV2, short sV3);
	void DeleteClient(int iClientH, BOOL bSave, BOOL bNotify, BOOL bCountLogout = TRUE, BOOL bForceCloseConn = FALSE);
	int  iComposeInitMapData(short sX, short sY, int iClientH, char * pData);
//	bool WriteTileData(char * buffer, int & iSize, int iClientH, CTile * srcTile, int ix, int iy);
	void RequestInitDataHandler(int iClientH, char * pData, char cKey, BOOL bIsNoNameCheck = FALSE);
	void RequestInitPlayerHandler(int iClientH, char * pData, char cKey);
	int  iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, BOOL bIsRun, BOOL BIsDamage = FALSE);
	
	BOOL _bCheckClientCanCarryItem(int iClientH);
	void RequestInitPlayerAutoLogin(int iClientH, const char* cCharName, const char* cAccountName, const char* cAccountPassword, BOOL bIsObserverMode, short resolution);
	//Nuevos Codigos ChatGPT
	bool isValidClient(int iClientH, short sX, short sY, char cDir);
	void updateClientActivity(int iClientH);
	void getNewCoordinates(short sX, short sY, char cDir, short& dX, short& dY);
	BOOL canMoveToLocation(int iClientH, short dX, short dY);
	void handleValidMove(int iClientH, short dX, short dY, char cDir, BOOL bIsRun, BOOL BIsDamage);
	void handleSpikeDamage(int iClientH);
	void sendMoveConfirmMessage(int iClientH, short dX, short dY, char cDir, BOOL bIsRun, BOOL BIsDamage);
	void handleRunStamina(int iClientH, BOOL bIsRun, char*& cp);
	void ClientMotionHandler(int iClientH, char* pData);
	void RequestCivilHandler(int iClientH, short city);


	void OnClientRead(int iClientH);

	void GetFightzoneTicketHandler(int iClientH);

	void FightzoneReserveProcessor() ;

	void AutoSkill(int iClientH);

	void ReqCreateSlateHandler(int iClientH, char* pData);
	void SetSlateFlag(int iClientH, short sType, bool bFlag);

	char m_cServerName[12];
	

	char m_cLogServerAddr[16];
	int  m_iLogServerPort;
	int  m_iWorldLogServerPort;

	int  m_iLimitedUserExp, m_iLevelExp20;

	BOOL _bDecodeItemConfigFileContents(char * cFn);
	int _iComposePlayerDataFileContents(int iClientH, char * pData);
	BOOL _bDecodePlayerDatafileContents(int iClientH, char * pData, DWORD dwSize);
	void LoadKnightSetProcessedPlayers();
	void SaveKnightSetProcessedPlayer(const std::string& playerName);
	bool IsShadowKnightPlayerProcessed(const std::string& playerName);

	std::vector<std::string> m_processedPlayers;
	CRITICAL_SECTION m_csShadowColors; // Para thread safety
	void RestoreShadowKnightColor(int client);
	BOOL bDecodeOhterPlayerData(int iClientH, char * pData, DWORD dwSize);
	bool BlockedTeleport(int client);
	void AdminCommand_Event(int client, char *pData, DWORD dwMsgSize);
	void handleCheckTimer(int client);
	void AdminOrder_UnLimbo(int client, char* pData, DWORD dwMsgSize);
	void EnableBot(int client);
	void EnableHelBot(int client);
	void DisableHelBot(int client);
	void DisableBot(int client);
	void SetLimboAfkFlag(int iClientH, int iStatus);
	void RequestMarketNames(int client);
	void RequestMarketItems(int client, char* pData);
	void handleMarketLog(char* seller, char* buyer, char* itemname, int coinval, DWORD attr);
	void RequestGetMarketItem(int client, char* pData);
	void ReqRepairItemCofirmHandler2(int client, char cItemID, char* pString);
	void RepairPowerHero(int client, int itindex);
	void checkBlockedItems(int client);
	bool IsBlockedItem(char* itemname);
	void DeleteBankItem(int client, int itemindex);
	void DeleteMarketItem(int client, int itemindex);
	void handleRefactorServer(int client);
	void NotifyItAtr(int client, int itindex, DWORD atr);
	void NotifyBankItAtr(int client, int itindex, DWORD atr);
	void requestStartEkQuest(int client, char * data);
	void requestEnableCharEkQuest(int client, char * data);
	void requestCharEkQuestReward(int client, char * data);
	void HandleReqCharQuestReward(int client, int quest_id);
	bool strStartsWith(const std::string& str, const std::string& prefix);
	void HandleCharQuestReward(int client, int quest_index);
	void requestDisableCharEkQuest(int client, char * data);
	void requestDeleteCharEkQuest(int client, char * data);
	bool CreateCharQuestItemReward(int client, string itemname);
	void LoadProcessedPlayers();
	void SaveProcessedPlayers();
	bool HasProcessedPlayer(const std::string& playerName);
	void AddPlayerToProcessedPlayer(const std::string& playerName);

	void LoadProcessedItems();
	void SaveProcessedItems();
	bool HasProcessedItems(const std::string& playerName);
	void AddPlayerToProcessedItems(const std::string& playerName);


	void LoadProcessedBankItems();
	void SaveProcessedBankItems();
	bool HasProcessedBankItems(const std::string& playerName);
	void AddPlayerToProcessedBankItems(const std::string& playerName);


	void LoadProcessedMarketItems();
	void SaveProcessedMarketItems();
	bool HasProcessedMarketItems(const std::string& playerName);
	void AddPlayerToProcessedMarketItems(const std::string& playerName);


	void LoadProcessedDropItems();
	void SaveProcessedDropItems();
	bool HasProcessedDropItems(const std::string& playerName);
	void AddPlayerToProcessedDropItems(const std::string& playerName);

	bool IsPlayerProcessed(const std::string& playerName);
	void AddProcessedPlayer(const std::string& playerName);
	void RequestSetMarketItem(int client, char* pData);
	void SaveCharacterMarket(int client);
	void bDecodePlayerMarketItemContents(int client, char* pData, DWORD dwSize);
	void ReadMarket(int client);
	BOOL revertitemWeight(class CItem* pItem, char* pItemName);
	void handleNewReadModes(char*& token, char& cReadModeA);
	BOOL _bRegisterMap(char * pName);
	bool getitemname(int client, int cx, int cy);
	void getCriticals(int client);
	void SendMessageToClientUDP(int client);
	void sendMessageToClient(int client, int msgid);
	class CClient * m_pClientList[DEF_MAXCLIENTS];
	class CNpc    * m_pNpcList[DEF_MAXNPCS];
	class CMap    * m_pMapList[DEF_MAXMAPS];
	class CDynamicObject * m_pDynamicObjectList[DEF_MAXDYNAMICOBJECTS];
	class CDelayEvent    * m_pDelayEventList[DEF_MAXDELAYEVENTS];
	bool m_bAutoEvents;
	bool m_bMobGenerator;
	bool m_bRareDrops;
	class CMsg    * m_pMsgQuene[DEF_MSGQUENESIZE];
	int             m_iQueneHead, m_iQueneTail;
	int             m_iTotalMaps;
	class XSocket * m_pMainLogSock, * m_pRAdminSock;
	class CMisc     m_Misc;
	BOOL			m_bIsGameStarted;
	BOOL            m_bIsLogSockAvailable, m_bIsWLServerAvailable;
	BOOL 			m_bIsOnlyClientAvailable;
	class CItem   * m_pItemConfigList[DEF_MAXITEMTYPES];
	class CNpc    * m_pNpcConfigList[DEF_MAXNPCTYPES];
	class CMagic  * m_pMagicConfigList[DEF_MAXMAGICTYPE];
	class CSkill  * m_pSkillConfigList[DEF_MAXSKILLTYPE];
	class CQuest  * m_pQuestConfigList[DEF_MAXQUESTTYPE];
	char            m_pMsgBuffer[DEF_MSGBUFFERSIZE+1];

	int DropRateNormal;
	int DropRateMadness;
	int DropRateBooster;
	int MineRateNormal;
	int MineRateRush;
	int MineLegendChanceDice;
	int MineLegendChanceResult;
	int FishingRate;
	int FishingRateBooster;

	HWND  m_hWnd;
	int   m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;
	SYSTEMTIME m_MaxUserSysTime;

	BOOL  m_bF1pressed, m_bF4pressed, m_bF12pressed;
	BOOL  m_bOnExitProcess;
	DWORD m_dwExitProcessTime;
	size_t currentIndex;
	DWORD m_dwCheckClientTime, m_dwWhetherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwGameTime9, m_dwGameTime10, m_dwGameTime11, m_dwAutoSaveTime, m_dwAutoSaveTime2, m_dwGameTime8, m_dwFishTime, m_dwGameProcTime, m_dwNpcsTime, m_dwExitTime, m_dwGGTime, m_dwEventsTime, m_dwHelBotTime;
	//v13
	DWORD m_dwNewTime; 
	int purge_newround;
	DWORD m_dwNewRoundPurge;
	DWORD m_dwCheckLoginSock;
	bool m_bShutDownServer;
	bool m_bCargandoDatos;
	bool m_bExitServer;
	BOOL  m_cDayOrNight;
 	int   m_iSkillSSNpoint[102];

	DWORD m_dwSpecialEventTime;
	BOOL  m_bIsSpecialEventTime;
	char  m_cSpecialEventType;

	int   m_iLevelExpTable[300];
 	class CFish * m_pFish[DEF_MAXFISHS];
	class CPortion * m_pPortionConfigList[DEF_MAXPORTIONTYPES];

	BOOL  m_bIsServerShutdowned;
	char  m_cShutDownCode;
	class CMineral * m_pMineral[DEF_MAXMINERALS];
    int   m_iWhouseMapIndex;
	int   m_iMiddlelandMapIndex;
	int   m_iAresdenMapIndex;
	int	  m_iElvineMapIndex;
	int   m_iAresdenOccupyTiles;
	int   m_iElvineOccupyTiles;
	int   m_iCurMsgs, m_iMaxMsgs;

	DWORD m_dwCanFightzoneReserveTime;
	int  m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	int  m_iFightzoneNoForceRecall;

	struct {
		__int64 iFunds;
		__int64 iCrimes;
		__int64 iWins;

	} m_stCityStatus[3];
	
	int	  m_iStrategicStatus;	

	int add_physdamage;
	int rem_physdamage;
	float add_magicdamage;
	float rem_magicdamage;

	short m_sFloorRelicX;
	short m_sFloorRelicY;

	class XSocket * m_pSubLogSock[DEF_MAXSUBLOGSOCK];
	int   m_iSubLogSockInitIndex;
	BOOL  m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK];
	int	  m_iCurSubLogSockIndex;
	int   m_iSubLogSockFailCount;
	int   m_iSubLogSockActiveCount;
	int   m_iAutoRebootingCount;

	class CBuildItem * m_pBuildItemList[DEF_MAXBUILDITEMS];
	class CItem * m_pDupItemIDList[DEF_MAXDUPITEMID];

	char * m_pNoticementData;
	DWORD  m_dwNoticementDataSize;

	DWORD  m_dwMapSectorInfoTime;
	int    m_iMapSectorInfoUpdateCount;

	int	   m_iCrusadeCount;
	BOOL   m_bIsCrusadeMode;

	struct {
		char cMapName[11];
		char cType;	
		int  dX, dY;

	} m_stCrusadeStructures[DEF_MAXCRUSADESTRUCTURES];

	BOOL	m_bIsApocalypseMode;//laloapoca

	int m_iCollectedMana[3];
	int m_iAresdenMana, m_iElvineMana;

	class CTeleportLoc m_pGuildTeleportLoc[DEF_MAXGUILDS];
	//

	WORD  m_wServerID_GSS;
	int   m_iIndexGSS;

	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	struct {
		char cType;
		char cSide;
		short sX, sY;
	} m_stMiddleCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iTotalMiddleCrusadeStructures;
 
	int m_iClientShortCut[DEF_MAXCLIENTS+1];

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	DWORD m_dwCrusadeGUID;
	int   m_iCrusadeWinnerSide;   
	int	  m_iWinnerSide;
	int	  m_iNonAttackArea;

	int   m_iWorldMaxUser;

	short m_sForceRecallTime;

	BOOL  m_bIsCrusadeWarStarter;
	int   m_iFinalShutdownCount;

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCrusadeWarSchedule[DEF_MAXSCHEDULE];

	class CItem * m_pGold;

	bool	m_bReceivedItemList;
	
	char m_cMsg[25][101];
	BOOL m_bAnunciosActivo;
	BOOL EventAuto;

	int __iSearchForQuest(int iClientH, int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	int _iTalkToNpcResult_Cityhall(int iClientH, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange);
	void _ClearExchangeStatus(int iClientH);
	int _iGetItemSpaceLeft(int iClientH);
	void AdminOrder_SummonGuild(int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_Assasain(int iClientH, char *pData, DWORD dwMsgSize);
	void ManualStartHappyHour(int iClientH, char *pData, DWORD dwMsgSize);
	void ManualStartHappyDay(int iClientH, char *pData, DWORD dwMsgSize);

	DWORD m_dwCleanMap;
	int HeldenianType2;

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stRushStructures[MAX_RUSHSTRUCTURES];

	void GoldPorLevel(int iClientH);

	//New Command Portal - ZeroEoyPnk
	void AdminOrder_Portal(int iClientH, char * pData, DWORD dwMsgSize);
	void ClosePortal(int PortalNum);
	int dest_dx_portal[DEF_MAXPORTALS], ini_dx_portal[DEF_MAXPORTALS];
	int dest_dy_portal[DEF_MAXPORTALS], ini_dy_portal[DEF_MAXPORTALS];
	int LevelLimit_portal[DEF_MAXPORTALS], MaxPlayer_portal[DEF_MAXPORTALS], PortalPlayers[DEF_MAXPORTALS];
	DWORD dw_TimeOpenPortal[DEF_MAXPORTALS], dw_TimePortal[DEF_MAXPORTALS];
	char Portal_MapName[DEF_MAXPORTALS][256], Portal_Init[DEF_MAXPORTALS][11];
	BOOL PortalAbility[DEF_MAXPORTALS];
	
	int LastPortal;

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stDefenceStructures[MAX_DEFENCESTRUCTURES];

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stDefenceStructuresx[MAX_DEFENCESTRUCTURES];


	//Trade TOTAL hbnet 22/12
	void GetMajesticsItemsHandler(int iClientH,int iItemID,char * pString);
	void ZerkMe(int iClientH);
	void ManaMe(int iClientH);
	void ExpMe(int iClientH);
	//Rep command
	void PlayerOrder_ChangeCity(int iClientH);

	BOOL VerificarItem(int iItemID);

	void NetBoxLotery(int iClientH, int ItemType);
	void HandleReward(int iClientH, int ItemType, int level);
	void DistributeReward(int iClientH, int ItemType, const std::vector<int>& probabilities);
	void ApplyReward(int iClientH, int ItemType, int rewardType);
	void Resultado1(int iClientH, int ItemType);
	void Resultado2(int iClientH, int ItemType);
	void Resultado3(int iClientH, int ItemType);
	void Resultado4(int iClientH, int ItemType);
	void Resultado5(int iClientH, int ItemType);

	BOOL Actualizar;

	int m_iPlayerMaxLevel;
	void _ArmarQuestContens(int iClientH, int iIndex);

	void PlayerReceivedCoords(int iClientH, int CoordX, int CoordY);

//	void DeleteItems(); // Test ZeroEoyPnk
	void DefineWHPage(int iClientH, short iV1);

	//NewCityVsCity
	void StartCityVsCity();
	void EndCityVsCity();
	void CloseEventMap();
	short ContadorElvine, ContadorAresden, ContadorAssesino;
	BOOL CityVsCity, PriceEvent;
	char SideWinner;
	DWORD cvcEventTime, EventTime, PriceTime;
	DWORD AllEksTime;
	DWORD AllDkEksTime;
	short LastContElv, LastContAres;

	//WorldWar LaloRamos
	void StartWorldWar();
	void EndWorldWar();
	void CloseWWEventMap();
	BOOL WorldWar, WWPriceEvent;
	void ActualizarWWContador();
	short WWContadorElvine, WWContadorAresden, WWContadorAssesino;
	char WWSideWinner;
	DWORD WorldWarTime, WWPriceTime;
	short WWLastContElv, WWLastContAres;

	//WorldWar II
	void StartWorldWarII();
	void EndWorldWarII();
	void CloseWWEventMapII();
	BOOL WorldWarII, WWIIPriceEvent;
	void ActualizarWWIIContador();
	short WWIIContadorElvine, WWIIContadorAresden, WWIIContadorAssesino;
	DWORD WorldWarTimeII, WWIIPriceTime;
	char WWIISideWinner;
	short WWIILastContElv, WWIILastContAres;


	bool m_bShinning;
	 
	BOOL bReadCVCStructureConfigFile(char * cFn);
	void CreateCVCStructures();
	void AbrirPortales(short Event);
	void AllClosePortals();
	void ActualizarContador();
//	void GlobalCvcCount(char CountType);

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stCVCStructures[MAX_CVCSTRUCTURES];

	void DeleteCliente(int iClientH, char *pData, DWORD dwMsgSize);

	void CallMercenary(short Map);

	int m_iAFKUsers;
	int m_bAFKLimbo;

	//New data send - ZeroEoyPnk
	void SendPlayerData(int iClientH);
	void LoadPlayerItems(int iClientH);
	void SendRestPlayerData(int iClientH);
	void SendLasData(int iClientH);
	void SendEnemyKills(int iClientH);
	void SendContribution(int iClientH);
	void SendCriticals(int iClientH);
	void SendEventOpenData(int iClientH);

	//Crash Lalo
	short sTotalConnection;

	//Reemplazado por una funcion, asi reduzco codigo.. y anda mejor :3
	void HammerStrip(int sAttackerH, char cAttackerType, int sTargetH, int iHitPoint);

	// Summon Event 
	void LaunchSummonEvent();
	void CreateMSStructures();
	void SummonAresden();
	void SummonElvine();
	void SummonAssasain();
	void EndSummonEvent();
	void NextRound(short Side, short Type, short Total);
//	void GlobalNextRound(short Side, short Type, short Total);

	BOOL bReadSummonStructureConfigFile(char * cFn);
	BOOL bReadRoundSummonConfigFile(char * cFn, short CFG);

	BOOL RunSummonEvent, RunSummonEvent2, RunSummonEvent3, SummonLaunched;
	char MSTotal, TotalRounds;
	char AresdenRound, ElvineRound, AssasainRound;
	DWORD AresdenSummon, ElvineSummon, AssasainSummon, SummonEventTime;
	short SelectedRounds;

	void ReloadSummonEventConfig();

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stSummonStructures[MAX_DEFENCESTRUCTURESA];

	struct {
		short Cant;	
		char cNpcName[21]; // VERGA!!!!			
	} SummonRound[DEF_MAXCRUSADESTRUCTURES];

	void SetFuryWarFlag(short sOwnerH, char cOwnerType, BOOL bStatus);

	//Assasain
	void ChangeToAssasain(int iClientH);
	void ApplyPkPoint(int iAttackerH, int iClientH, BOOL Disc = FALSE);
	void GetAssasainHero(int iClientH, int iV1);
	void ResetAssasain(int iClientH, int iV1);
	void ChangePkToEk(int iClientH, int iV1);
	void ChangeLocation(int iClientH, DWORD wResult);

	//Base defense event
	BOOL bReadHellOfDeathConfigFile(char * cFn);
	void CreateHellOfDeathStructures();

	struct {
		char cMapName[11];			
		char cType;					
		int  dX, dY;				

	} m_stHellOfDeathStructures[MAX_DEFENCESTRUCTURES];

	BOOL HellOfDeath, PriceHODEvent;
	DWORD dw_HODTime;
	DWORD dw_PriceHODTime;
	
	void InitHellOfDeath();
	void RemoveHODStructures();
	void EndHellOfDeathMode(int Loser);
	void ActualizarStructures();
	void CloseHODMap();

	void SearchAssasainPosition(int iClientH);

	BOOL VipMap;
	BOOL AllEks;
	BOOL AllDkEks;

//	void WrongClient(int iClient);

	struct {
		char m_cName[21];
		int m_wPrice;
		BOOL unique;
	} ItemsTrade[2000];
	BOOL ReadTradeItems(char * cFn);
	void RequestTradeItemHandler(int iClientH, char * pItemName, int iNum, short Type);
	BOOL TradeID[4];

	void RequestRemoveItemExchange(int iClientH, int iItemIndex);
	int CalcWeightExchange(int iClientH, int iExH);
	BOOL CalcTotalBagExchange(int iClientH, int iExH);

	void CheckItemStatedEquipment(int iClientH);

	BOOL Masacre;
	
	void CheckCompleteAchivement(int iClientH, short AchivementID, BOOL Check = FALSE, BOOL New = TRUE);
	void CompleteAchivementList(int iClientH);

	void ClientFakeMoveTimmer();
	void ClientFakeMove(int iClientH);

	void SearchStatusFriend(int iClientH, short Position, char CharName[10]);
	void RequestGuildMembers(int iClientH);

	void SendF5DataCharacter(int iClientH);
	int Random(int Minimo, int Maximo);

	void CheckProcess(int iClientH);
	void DataProcessReceived(int iClientH, char * pData, DWORD dwMsgSize);
	void CheckProcessClients();
	void CheckHability(int iClientH, char *pData, DWORD dwMsgSize);
	DWORD m_dwCheckProcess;

	BOOL ReadProcessAndDllBlockedFile(char * cFn, BOOL Type);
	void SendProcessBlocked(int iClientH);
	void SendDllsBlocked(int iClientH);
	char Process[100][50];
	char Dlls[100][50];
	void UpdateProcessClient();

	void ChangeHeroSide(int iClientH);
	void DeleteWhItem(int iClientH, short sItemIndex);
	BOOL SendBankItemData(int iClientH, class CItem * pItem, short Position, BOOL Actualizar = FALSE);

	void CheckHeroData(int iClientH, short ItemIndex, BOOL WH);

	BOOL _bReadItemsEventCFG(char * cFn);
	char ItemsEvent[100][21];
	void InitItemsClient(int iClientH);
	void AddCandys(int iClientH);
	void RestoreItems(int iClientH);

	void InitArenaEvent();
	void CloseArenaEvent();
	BOOL FinalFight;
	struct {
		char CharName[11];
		short Nivel;
		short Kills;
		short Deaths;
	} PvpContador[10];
	void ArmarEventTop();
	void CheckNewNivel(int iClientH, BOOL Init);
	void NivelObteined(int iClientH);

	void Event_FinalFight();
	void Close_FinalFight();
	short FinalFight_ID;

	void CreateFinalFightID();
	BOOL bReadFinalFightConfigFile(char * cFn);


	BOOL ReadQuestItemsCFG(char * cFn);

	void Event_EK40();
	void EndEvent_EK40();

	void Event_EK100();
	void EndEvent_EK100();

	void Event_Masacre();
	void EndEvent_Masacre();

	void Event_TowerHell();
	void EndEvent_TowerHell();

	void Event_HuntMap();
	void EndEvent_HuntMap();

	void Event_Questx2();
	void EndEvent_Questx2();
	void Event_Questx3();
	void EndEvent_Questx3();

	void EventAllEks();
	void EventAllDkEks();

	void EventDM();

	BOOL DKEvent;

	BOOL TowerHell;
	BOOL Questx2;
	BOOL Questx3;

	void RequestLogOutHandler(int iClientH);
	void RequestCancelLogOutHandler(int iClientH);

	//int MaxClientID;

	void CheckUsersIP (int iClientH, char *pData, DWORD dwMsgSize);
	void AdminOrder_BanIP(int iClientH, char *pData, DWORD dwMsgSize);
	void LoadBannedList(int iClientH);

	char BannedIP[100][21];

	short TotalAnnoucement;

//	void ReqPing(int iClientH, int pData);

	// Events Wins
	struct {
		short AresdenWin;
		short ElvineWin;
		short Draw;
		short LastWin;
	} EventCount[10];

	BOOL bReadEventResults(char * cFn, short Event);
	void SendEventData(int iClientH);
	DWORD dw_CrusadeTime, dw_TowerTime, dw_CaptureTime, dw_RushTime, dw_DmTime;

	void CreateEventResults(int iWinnerSide, short Event);

	void LocalStarHappyHour(BOOL StatusHappy);
	void LocalStarHappyDay(BOOL StatusHappy);

	// Adds Points Admin
	void AdminOrder_SetPoints (int iClientH, char *pData, DWORD dwMsgSize, short Type);

	// New System -> Market Place -> HB Arg v4.4
	void bSetItemToSellList(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestItemsList(int iClientH, char * pData, DWORD dwMsgSize);
	void SendDataListItems(int SendTo, int iClientH, short Position, short TotalSell);
	void RequestDefineSellList(int iClientH, char * pData, DWORD dwMsgSize);
	void RequestBuyItem(int iClientH, char * pData, DWORD dwMsgSize);
	void ActualizarSellingList(int BuyTo, int iClientH);
	
	// Summon Balls -> ZeroEoyPnk
/*	void SearchMapToSummon(short SummonBoss, short iClientH);
	void RequestCreateSummonBallNPC(short SummonBoss, short iClientH);
	void CreateSummonBallNPC(short SummonBoss, short ServerID, short iClientH, char * Guild);*/

	//new objects data (all clients)
	struct st_objects
	{
		st_objects() {}
		int client;
		short m_sSide;
		short m_sRankLevel;
		short sLevel;
		short sRebirthLevel;
		short sRelicHolder;
		short m_sFisher;
		short m_sWriteStatus;
		short m_sPoisonStatus;
		short m_sMerienHero;
		short m_sPlayerRole;
		short m_sRaiderLvl;
		short m_sHunterLvl;
		short m_sPurgeTeam; 
		int m_iSpecialItem;
		short m_sFireStatus;
		short m_sLightingStatus;
		short m_sTalent;
		short m_sTalentLvl;
	};

	vector<st_objects> vObjects;


	struct st_players
	{
		st_players() {}
		short m_sSide;
		char  m_cMapName[12];
		char  m_cCharName[12];
	};

	vector<st_players> vPlayers;

	struct {
		DWORD dwOpenSummonMap;
		bool ActiveSummon;
		char GuildName[22];
	} SummonBalls[10];

	BOOL ReadMapCFGs(char * cFn);
	char MapList[DEF_MAXMAPS][12];

	struct {
		char Name[11];
		Citizenship Side;
		int Points;
	} Top[6][15];

	void SendObjectsData(int client, const std::vector<st_objects>& vObjects);
	void SendPlayersShines();
	void HandleRequestMemoryTops(int iClientH, short Top, BOOL Send = TRUE, BOOL Actualizar = TRUE);

	void EkAnnoucer(char cAttackerName[12], char cVictimun[12], BOOL EK);
	void SendEkAnnoucer(char cEKMsg[30], BOOL EK);

//	void GlobalSendHPStructures(short Type);
	void SendHPStructures(short Type);
	short ReliquiaAresden, ReliquiaElvine;

//	void GlobalStartTpEvent(short Event);
//	void GlobalEndTpEvent(short Event);

	void RequestBuyAssasainPoints(int iClientH, char * pData, DWORD dwMsgSize);

	class PartyManager * m_pPartyManager[DEF_MAXPARTYNUM];

	void RefreshParty(int iClientH, short Type);

	void ObteinStarterKit(int iClientH);
	void RemoveItemUniqueID(int iClientH, short itempos, short ItemUse);
	//Nuevo Change Master ID
	void RemoveItemMasterID(int iClientH, short itempos, short ItemUse);
	//Sacar ID
	void RemoveTotalID(int iClientH, short itempos, short ItemUse);
	int RandomEventID;
	bool MarketEnabled;

	//DK EVENT
	vector<st_dkbackmember> vec_dkbackmembers;
	int m_idkareskills;
	int m_idkelvkills;
	char cwinner[12];
	void EnableDkEvent();
	void DisableDkEvent();
	void notify_dkeventpoints(int client, int kills, int deaths);
	void kill_dkeventenemy(int att, int tar);
	void request_revivedkevent(int client);
	void dkeventrevive(int client);
	void dkeventnot_revive(int client);
	void dkeventadd_kills(int client, int iadd);
	void dkeventadd_deaths(char * charname, int iadd);
	void dkeventnotify(int att, int tar);
	void handleActionsWhenEventFinish();
	void NotifyCitysKills(int client);
	void EndDkEvent(int client);
	void WinnerReward(int client);

	void MakeItemsDkEvent(int client, short combattype);
	void handleEventOnEventDisable();
	void handleAngelicActions_DkEvent(int client);
	void LearnMagicsDkEvent(int client);
	void RestoreMagicsDkEvent(int client);
	void CreateDkCandys(int client);
	void CreateBluePotionsDk(int client);
	void CreateRedPotionsDk(int client);
	void CreateGreenPotionsDk(int client);
	void handleDKEventTop5();
	void calcItemsEffectsDk(int client);
	void DkManaMe(int iClientH);
	void get_joininfodkevent(int client, char * data);
	void SavePlayerItemForEventDkEvent(int client);
	void RestorePlayerItemsDkEvent(int client);
	void handlePlayerForDkEvent(int client, short combattype);
	void restorePlayerStatsAfterDkEvent(int client);
	void restorePlayerBattleStatsAfterEvent(int client);
	void CreateDkSets(int client, short combattype);
	void NotifyDkItems(int client);
	void CreateDkWeapons(int client, short combattype);
	void ReloadDkCandys(int client);
	void GetDkBluePotions(int client);
	void GetDkRedPotions(int client);
	void GetDkGreenPotions(int client);
	void join_dkevent(int client, short combattype, short angelic);
	void add_dkfighter(char * charname);
	void remove_dkfighter(char * charname);
	bool is_dkfighter(char * charname);
	int get_dkeventkills(char * charname);
	int get_dkfighterindex(char * charname);
	int get_dkdeaths(char * charname);

	void CreateDkItem(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	void CreateDkCandy(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	int imaxdkeventkills;
	int iekdkxkill;
	int idkeventwinreward;

	vector<st_dkfighters> vec_dkfighters;

	//BATTLE ROYALE

	struct st_brbackmember
	{
		st_brbackmember() {}
		char m_cCharName[12];
		int iStr;
		int iInt;
		int iVit;
		int iDex;
		int iMag;
		int iChar;
	};

	vector<st_brbackmember> vec_brbackmembers;

	struct ItemCoordinate {
		int sx;
		int sy;
	};

	struct Items {
		int item_id;
	};

	vector<ItemCoordinate> _randomitemcord;
	vector<Items> _items;

	void restorePlayerPointsBattleRoyale(int client);
	void handlePlayerForBattleRoyale(int client, short combattype);
	void handleBattleAngelicActionsBattleRoyale(int client);
	void MakeItemsBattleRoyale(int client, short combattype);
	void CreateSetsBattleRoyale(int client, short combattype);
	void EnableBattleRoyale();                // Método para iniciar el evento
	void ChangeEventStatusBr(short status);
	void NotifySafeTimeStatusBr();
	void NotifySafeTimeStatusToClientBr(int client);
	void NotifyEventStatusBr(int client);
	void join_eventBr(int client, short combattype, short angelic);
	void handlePlayerItemsBattleRoyale(int client, short combattype);
	void SavePlayerItemForBattleRoyale(int client);
	void SavePlayerItemForBr(int client);
	void handleBattleRoyaleBr();    // Maneja el inicio de la ronda y el temporizador
	void handleFreezeTimeBr();      // Maneja el tiempo de espera antes de la ronda
	void deleteDropedItemsBr(int client);
	void RepairItemsBr(int client);
	void handleTimeRemaningBr();    // Actualiza el temporizador
	void NotifyFreezeTimeRemainingBr(char * time);
	void NotifyTimeRemainingBr(char * time);
	void monitorRoundDurationBattleRoyale();  // Monitorea los 10 minutos de duración de la ronda
	void NotifyRoundTimeBr(char * time);
	std::string getTimeRemainingBr() const; // Devuelve el tiempo restante en formato "mm:ss"
	void incrementElapsedSecondsBr();       // Incrementa los segundos transcurridos
	bool isRoundStartedBr() const;          // Devuelve si la ronda ha comenzado
	int m_durationSecondsBr;        // Duración del temporizador en segundos (5 minutos)
	int m_elapsedSecondsBr;         // Segundos transcurridos durante la cuenta regresiva
	int m_elapsedRoundSecondsBr;    // Segundos transcurridos desde que comenzó la ronda
	int m_iSafeTimeBr;
	bool m_bRoundStartedBr;         // Indica si la ronda ha comenzado
	bool m_bIsSafeTimeBr;
	int m_iMapIndexBr;
	int m_iMaxRoundTimeBr;
	bool bNoNotifyBr;
	std::string m_sTimeRemainingBr; // Tiempo restante en formato "mm:ss"
	short m_sEventStatusBr;
	int m_sRoundBr;                 // Número de la ronda actual
	short m_sMaxRoundsBr;
	std::vector<st_brfighters> vec_brfighters; // Lista de combatientes
	std::vector<string> brwinners;
	int m_lastUpdateTimeBr;
	std::vector<std::pair<int, int>> m_coordinatesBr; // Lista de coordenadas
	std::vector<int> m_coordinateIndicesBr;          // Índices de las coordenadas disponibles
	int m_currentIndexBr;                            // Índice actual para asignar coordenadas
	float m_radiusBr;            // Radio actual del círculo
	bool m_radiusInitializedBr;  // Bandera para saber si el radio ha sido inicializado
	int m_remainingIntervalsBr;  // Número de intervalos restantes para reducir el radio

	// Variables para el freeze time
	int m_freezeTimeBr;          // Duración del freeze time en segundos (5 segundos)
	int m_elapsedFreezeBrSeconds; // Segundos transcurridos durante el freeze time

	std::string formatTimeBr(int minutes, int seconds) const;
	void InitializeRadiusBattleRoyale();
	void onBattleRoyaleRoundStarted();
	void ReviveBr(int client);
	void teleportPlayersBr();
	void handleReviveBr(int client);
	void InitializeCoordinatesBr();
	void ShuffleCoordinatesBr();
	void handleNewRoundBr();
	void NotifyRoundBr(int client);
	void onRoundDurationExceededBr();
	void handleNewRoundTasksBr();
	void EndEventBattleRoyale();
	void SendTextMessageToAllBr(char * message);
	void SendTextMessageBr(int client, char * message);
	bool is_brfighter(char * charname);
	void add_brfighterBattleRoyale(char * charname);
	int getbattleroyale_fighterindex(char * charname);
	int calculateMapIndexBattleRoyale();
	void KillEnemyBr(int attacker, int target);
	void handleKillActionBr(int att, int tar);
	void handleCheckLastManBr();
	void InitializeRadiusBr();
	void handleMapRadiusBattleRoyale();
	void notifyRadiusBattleRoyale(int client);
	void UpdatePlayersWithinRadiusToAllBr();
	void restorePlayerPointsBr(int client);
	void CreateItemBattleRoyale(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	void CreateCandyBattleRoyale(int client, string itemname, DWORD atr, short sx, short sy, short color, int count, bool isweap, bool equip = false);
	void CreateWeaponsBattleRoyale(int client, short combattype);
	void CreateCandysBattleRoyale(int client);
	void CreateBluePotionsBattleRoyale(int client);
	void CreateRedPotionsBattleRoyale(int client);
	void CreateGreenPotionsBattleRoyale(int client);
	void NotifyItemsBattleRoyale(int client);
	void LearnBattleMagicsBattleRoyale(int client);
	void LearnBattleMagicsBr(int client);
	void getJoinInfoBr(int client, char * data);
	void ClearMapBattleRoyale(string mapname);
	void DropitemsBattleRoyale();
	void DropitemsBr();
	void handleItemDropBattleRoyale(std::string map, int id, int x, int y);
	void ItemDropBattleRoyale(CMap * map, int iItemID, int iX, int iY, int dwType, int dwValue, int cColor, bool isbattle = false);
	void ReloadCandysBattleRoyale(int client);
	void GetBluePotionsBattleRoyale(int client);
	void GetRedPotionsBattleRoyale(int client);
	void GetGreenPotionsBattleRoyale(int client);
	void UpdatePlayersWithinRadiusToAllBattleRoyale();
	void UpdatePlayersWithinRadiusBattleRoyale(int client);
	void SendTextMessageToAllBattleRoyale(char * message);
	void SendTextMessageBattleRoyale(int client, char * message);
	void KillEnemyBattleRoyale(int attacker, int target);
	void handleKillActionBattleRoyale(int att, int tar);
	void RequestReviveInBattleRoyale(int client);
	void RequestReviveInBattleRoyaleWithouthDialog(int client);
	void BattleRoyale_ReviveYes(int client);
	void calcItemsEffectsBattleRoyale(int client);
	void fixItemsBattleRoyale(int client, int itemindex);
	void BattleRoyale_ReviveNewRound(int client);
	void BattleRoyale_ReviveOnEndEvent(int client);
	void BattleRoyale_NotRevive(int client);
	void handleCheckLastManBattleRoyale();
	void UpdatePlayersWithinRadiusBr(int client);
	void request_reviveBr(int client);
	void reviveNowBr(int client);
	void checkClientsObserverMode();
	void request_reviveeventbr(int client);
	void eventreviveBr(int client);
	void EventKilledHandler(int client);
	void ServerKillEnemyBattleRoyale(int client);
	void handleServerKillActionBattleRoyale(int client);
	bool checkIfLastPlayer();
	void handleNewRoundTasksBattleRoyale();
	void RestartRoundBattleRoyale();
	void initializeBattleRoyale();

	//new br
	short m_sBattleRoyalStatus;

	int m_iElapsedRegistrationTime;
	int m_iElapsedFreezeTime;
	int m_iElapsedSafeTime;
	int m_iElapsedRoundTime;
	short m_sCurrentRound;
	short m_sMaxRounds;
	bool m_bSafeTime;

	void enableBattleRoyaleEvent();
	void InitializeCoordinateBattleRoyale();
	void ShuffleCoordinatesBattleRoyale();
	void handleBattleRoyale();
	void handleEventOnRegistrationTime();
	void notifyBattleRoyaleRegisterTime(int time);
	void handleEventOnFreezeTime();
	void notifyBattleRoyaleFreezeTime(int time);
	void handleEventOnSafeTime();
	void notifyBattleRoyaleSafeTime(int time);
	void handleEventOnRound();
	void handleEventOnRound(int time);
	void notifyRoundTime(int time);
	void notifyCurrentRound(int client);
	void getJoinInfoBattleRoyale(int client, char * data);
	void TeleportToArenaLoby(int client);
	void join_battleroyale(int client, short combattype, short angelic);
	void notifyBattleRoyaleStatus();
	void changeBattleRoyaleStatus(short status);
	void handleEventOnRoundTime();
	void SetActiveMastery(int client, short mastery);
	void IncreaseMasteryLevel(int client, short mastery);
	void notifyMasteryData(int client);
	void reqUpgradeMastery(int client, char * data);
	void reqActivateMastery(int client, char * data);
	string getekquestvalue(int client, string val);
	void read_ekquest_data(int client);
	string getvalue(int client, string val);
	void read_quest_data(int client);
	void send_charquest_data(int client);
	void req_dismiss_quest(int client, char * data);
	void req_pause_quest(int client, char * data);
	void req_active_quest(int client, char * data);
	void active_quest(int client, int index);
	void req_reward_quest(int client, char * data);
	void reward_quest(int client, int index, short rew);
	void pause_quest(int client, int index);
	void remove_quest(int client, int index);
	void CreateQuestItem(int client, char * itemname);
	string getDropValue(int client, string val);
	void read_drop_items(int client);
	void handleActivateRole(int client, short rol);
	void handleUpgradeRole(int client, short role);
	void send_role_data(int client);
	void ClearBagGG(int client);
	void ClearBagDK(int client);
	void ClearBagBR(int client);
	bool SpecialCharacter(int client);
	void EnableItem(int client, i8 handle);
	//loginserver
	void RequestLogin(int iClientH, char * pData);
	void GetCharList(string acc, char *& cp2, vector<string> chars);
	LogIn AccountLogIn(string name, string pass, std::vector<string> & chars);
	void CreateNewCharacter(int iClientH, char * pData);
	void DeleteCharacter(int iClientH, char * pData);
	void ChangePassword(int iClientH, char * pData);
	void RequestEnterGame(int iClientH, char * pData);
	void handleNassaCharacter(int client);
	void CreateNewAccount(int iClientH, char * pData);
	BOOL SaveAccountInfo(int iAccount, char * cAccountName, char * cTemp, char * cCharName, char cMode, int iClientH);
	void SaveInfo(char cFileName[255], char *pData, DWORD dwStartSize);
	void SendLoginMsg(DWORD msgid, WORD msgtype, char * data, int sz, int iClientH);
	void LocalSavePlayerData(int iClientH);
	void Activated();
	bool ReadCoreServerSettings(char * name);
	void Init();
	void SendLoginInfo(int client);
	int SaveDataFileContents(int client, char * pData);
	bool GetDataFileContents(int client, char * pData, DWORD dwSize);
	//gungame
	char cggwinner[12];
	void EnableGunGame();
	void DisableGunGame();
	void notify_ggpoints(int client, int kills, int deaths);
	void kill_enemyGG(int att, int tar);
	void kill_enemyGG2(int att);
	void request_reviveGG(int client);
	void reviveGG(int client);
	void not_reviveGG(int client);
	void add_killsGG(int client, int iadd);
	void add_deathsGG(char * charname, int iadd);
	void notifyGG(int att, int tar);
	void notifyGG2(int att);
	void End_GunGame_Event(int client);
	void WinnerGGReward(int client);
	int getGGPlayerLevel(int client);
	void get_joininfoGG(int client, char * data);
	std::string findGGLeader();
	void calcItemsEffects(int client);
	void join_ggevent(int client, short combattype, short angelic);
	void getggranking(int client);
	void send_ggtop10(int client);
	void add_ggfighter(char * charname);
	void remove_ggfighter(char * charname);
	void sort_ggfighters();
	bool is_ggfighter(char * charname);
	int get_ggkills(char * charname);
	int get_ggfighterindex(char * charname);
	int get_ggdeaths(char * charname);
	vector<st_ggfighters> vec_ggfighters;

	//deathmatch
	void EnableDeathMatch();
	void DisableDeathmatch();
	void notify_dmpoints(int client, int kills, int deaths);
	void dmkill_enemy(int att, int tar);
	void request_dmrevive(int client);
	void dmrevive(int client);
	void dmnot_revive(int client);
	void dmadd_kills(char * charname, int iadd);
	void dmadd_deaths(char * charname, int iadd);
	void dmnotify(int att, int tar);
	void EndDeathmatch(int client);
	void DMWinnerReward(int client);
	int GetIndexByNameDM(char * charname);
	void join_eventDM(int client);
	void getdmranking(int client);
	void send_dmtop10(int client);
	void add_dmfighter(char * charname);
	void remove_dmfighter(char * charname);
	void sort_dmfighters();
	bool is_dmfighter(char * charname);
	int get_dmkills(char * charname);
	int get_dmdeaths(char * charname);
	void get_quests();
	void send_quest_data(int client);
	void get_quest_data(int client, char * data);
	void start_quest(int client, short id);
	vector<st_dmfighters> vec_dmfighters;
	//heaton dm timer
	DWORD m_dwDeathmatchTimer;

	vector<st_quest> vec_questlist;

	void get_trade_ek();
	void get_trade_eventpoints();
	void get_trade_tokens();
	void get_trade_contrib();
	void get_trade_coin();
	void get_trade_majs();
	void send_trade_data(int client, short trade);
	bool IsFragment(char* itname);
	void req_trade(int client, char* pData);
	void req_tradeball(int client, char * data);
	void getLightShardsBall(int client, int quant);
	void getStrongShardsBall(int client, int quant);
	void getHitProbFragmentsBall(int client, int quant);
	void getDRFragmentsBall(int client, int quant);
	void getHPRecFragmentsBall(int client, int quant);
	void getSPRecFragmentsBall(int client, int quant);
	void getMPRecFragmentsBall(int client, int quant);
	void getMRFragmentsBall(int client, int quant);
	void getPAFragmentsBall(int client, int quant);
	void getMAFragmentsBall(int client, int quant);
	void getEXPFragmentsBall(int client, int quant);
	void getContributionBall(int client, int quant);
	void getCoinBall(int client, int quant);
	void getFireSegmentBall(int client, int quant);
	void getLightningSegmentBall(int client, int quant);
	void getIceSegmentBall(int client, int quant);
	void getMasteryBall(int client, int quant);
	void getReputationBall(int client, int quant);
	void getMajesticBall(int client, int quant);
	
	vector<st_trade_majs> vec_trade_majs;
	vector<st_trade_eventpoints> vec_trade_eventpoints;
	vector<st_trade_token> vec_trade_tokens;
	vector<st_trade_coin> vec_trade_coin;
	vector<st_trade_contrib> vec_trade_contrib;
	vector<st_trade_ek> vec_trade_ek;

	void clear_rank();
	void init_rank();
	void update_rank();
	void read();
	bool is_wantedrank_line(const string & line, string str);
	string getrank_line(string file, string value1);
	string getrankvalue(string val);
	int Aby_MobsCounter();
	Coord getRandomSpawn();
	void InitAbyEvent();
	int mobs_maze;
	void SummonApocaMobs();
	void CreateApocaMob(char * name, short x, short y, int quant, char side, char * mapname);
	void DeleteApocaNpcs();
	void handleGuardiansRespawnTimer();
	void ChangeRoaEventState();
	void handleGuardianDrop(int client);
	void CreateGuardianDrop(int client, int id, int count, int x, int y);
	void guardiandrop(int client, int iItemID, int count, int iX, int iY);
	void SendMazeMobCount();
	void SendMazeMobCountToClient(int client);
	void joinRoaEvent(int client);
	void joinRebirthFarm(int client);
	void handleSpecialMagic(int client, char * data);
	void CoutLogList(string str);
	void handleEventWhenAbaddonDeath();
	void checkAbaddonEventTimer();
	int m_iMaxrankexp[MAXRANKEXP];
};

#endif // !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)

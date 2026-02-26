
#include <windows.h>
#include <string>
#include <Tlhelp32.h>
#include <fstream>
#include <iostream>
#include <iosfwd>
#include <vector>
#include <sstream>

#include <ctime>
#include <unordered_map>
#include <string>
#include <thread>
#include <chrono>

#define VK_1 0x31 // '1'
#define VK_2 0x32 // '2'
//#define ANTIPULL
#define SHOPBOTS
#define FIXBUGDC
#define DEF_MAXREBIRTH         30

#define DEF_SERVERVERSION 5
#define MAXMASTERYLEVEL 5
#define STATS_DIALOG_NUMBER 88
#define STATS_MODE_SELECT 0
#define STATS_MODE_CONFIRM 1
#define STATS_MODE_COMPLETE 2

#define VOICE_DIALOG_NUMBER 89
#define GUILDBANK_DIALOG_NUMBER 90

enum PlayerRol {
	ROLE_NONE,
	ROLE_RAIDER, 
	ROLE_HUNTER
};

enum PlayerTalents {
	TALENT_FIRE,
	TALENT_LIGHTNING,
	TALENT_ICE
};

enum ePurgeTeam {
	TEAM_NONE,
	TEAM_ONE,
	TEAM_TWO
};


enum RankingMode {
	RANK_NONE,
	RANK_EK,
	RANK_TOTALEK,
	RANK_REP,
	RANK_CONTRIB,
	RANK_PK
};

// Definir índices para cada estadística
enum STAT_INDEX {
	STAT_PA = 0,    // Physical Absorption
	STAT_MA,        // Magic Absorption
	STAT_MR,        // Magic Resist
	STAT_DR,        // Defense Ratio
	STAT_PHYS_DMG,  // Physical Damage
	STAT_PHYS_HIT,  // Physical Hiting
	STAT_MAG_DMG,   // Magic Damage
	STAT_MAG_HIT,   // Magic Hiting
	STAT_MS,        // ManaSave
	STAT_COUNT      // Total de estadísticas
};

// Estructura para configurar límites y costos de cada stat
struct StatConfig {
	int maxLevel;    // Nivel máximo para esta stat
	int tokenCost;   // Costo base en tokens por nivel
	int coinCost;    // Costo base en coins por nivel
};

// Configuración de cada stat (valores ajustados para economía con jugadores que tienen 50k+)
const StatConfig statConfigs[STAT_COUNT] = {
	{ 15, 1000, 1000 },   // STAT_PA (Physical Absorption) - Límite bajo pero costoso
	{ 15, 1000, 1000 },   // STAT_MA (Magic Absorption) - Límite bajo pero costoso
	{ 15, 700, 700 },   // STAT_MR (Magic Resist)
	{ 15, 700, 700 },   // STAT_DR (Defense Ratio)
	{ 150, 750, 750 },   // STAT_PHYS_DMG (Physical Damage)
	{ 200, 200, 200 },   // STAT_PHYS_HIT (Physical Hiting)
	{ 150, 750, 750 },   // STAT_MAG_DMG (Magic Damage)
	{ 200, 200, 200 },   // STAT_MAG_HIT (Magic Hiting)
	{ 25, 600, 600 }     // STAT_MS (ManaSave) - Más accesible
}; 

enum Side
{
	OBJNEUTRAL,
	OBJARESDEN,
	OBJELVINE
};

#define MAX_HP_DEFAULT(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_VITLV1(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_VITLV2(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_VITLV3(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_VITLV4(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_VITLV5(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2)* 1.3)
#define MAX_HP_DKEVENT(vit, lvl, str) ((vit * 2 + lvl * 2 + (str) / 2) * 1.3)

#define INIT(x) ZeroMemory(x, sizeof(x))

#define _ms					_s / 1000
#define _s					* CLOCKS_PER_SEC
#define _m					_s * 60
#define _h					_m * 60
#define _d					_h * 24

#define MAXRANKS 31 //21
#define C800x600 1
#define C640x480 2
#define C1024x768 3

using namespace std;
typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned long u32;
typedef signed long i32;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned char u8;
typedef signed char i8;

template <typename T, class = typename enable_if<!is_pointer<T>::value>::type >
static void Push(char * &cp, T value) {
	auto p = (T *)cp;
	*p = (T)value;
	cp += sizeof(T);
}

template <typename T, class = typename enable_if<!is_pointer<T>::value>::type >
static void Pop(char * &cp, T &v) {
	T * p = (T *)cp;
	v = *p;
	cp += sizeof(T);
}

static void memcpy_sec(void * cIn, const void * cOut, size_t n) {
	if ((cIn == NULL) || (cOut == NULL) || (n <= 0) || IsBadReadPtr(cIn, n) || IsBadReadPtr(cOut, n)) return;
	memcpy(cIn, cOut, n);
}

static void Push(char * &dest, const char * src, u32 len) {
	memcpy_sec(dest, src, len);
	dest += len;
}

static void Push(char * &dest, const char * src) {

	strcpy(dest, src);
	dest += strlen(src) + 1;
}

static void Push(char * &dest, const string & str) {
	strcpy(dest, str.c_str());
	dest += str.length() + 1;
}

static void Pop(char * &src, char * dest, u32 len) {
	memcpy_sec(dest, src, len);
	src += len;
}
static void Pop(char * &src, char * dest) {

	u32 len = strlen(src) + 1;
	memcpy_sec(dest, src, len);
	src += len;
}

static void Pop(char * &src, string & str) {
	str = src;
	src += str.length() + 1;
}

//HeatoN events
enum class EventMsg : u16
{
	Status,
	Enter,
};

enum class EventID : u8
{
	Deathmatch,
	GunGame,
	Questx2,
	EkQuestx2,
	Ekx10,
	Ekx20,
	Ekx30,
	Ekx50,
	Crusada,
	Shinning,
	ExpBooster,
	DropInhibition,
	CTR,
	Rush,
	Apocalypse,
	BattleRoyale,
	DkEvent,
	DropBooster,
	Ekx100,
	Questx3,
	DropMadness,
	MiningRush,
	Ekx150,
	Ekx200,
	BOTD,
	CVC,
	ThePurge,
	Abaddon,
	EkQuestx3,
	RiseOfAbaddon,
	GlobalSilence,
	Max
};

static char * _eventName[(int)EventID::Max] =
{
	"Deathmatch",
	"GunGame v3", 
	"Quest x2",
	"EkQuest x2",
	"Ek x10",
	"Ek x20",
	"Ek x30",
	"Ek x50",
	"Crusada",
	"Shinning",
	"Exp Booster",
	"Drop Inhibition",
	"Capture the Relic",
	"Rush",
	"Apocalypse",
	"Battle Royale",
	"Eternal Balance",
	"Drop Booster",
	"Ek x100",
	"Epic Quest Surge",
	"Drop Madness",	
	"Mining Rush",
	"Ekx150",
	"Ekx200",
	"Blessing of the Deep",
	"City vs City",
	"The Purge",
	"Abaddon Event",
	"War of Glory",
	"Rise of Abaddon",
	"Global Silence"
};

static char * _eventNameShort[(int)EventID::Max] =
{
	"dm",
	"gungame",
	"questx2",
	"ekquestx2",
	"ekx10",
	"ekx20",
	"ekx30",
	"ekx50",
	"crusada",
	"shinning",
	"expbooster",
	"nodrop",
	"ctr",
	"rush",
	"apoca", 
	"br",
	"dk",
	"dropbooster",
	"ekx100",
	"questx3",
	"dropmadness",
	"miningrush",
	"ekx150",
	"ekx200",
	"botd",
	"cvc",
	"purge",
	"aby",
	"ekquestx3",
	"roa",
	"gs"
};

static char * getEventName(EventID id)
{
	return _eventName[(int)id];
}

static char * getEventNameShort(EventID id)
{
	return _eventNameShort[(int)id];
}


static EventID _tpEvents[] =
{
	EventID::Deathmatch,
	EventID::GunGame,
};


// Tipo de dato para manejar los horarios
struct EventSchedule {
	EventID event;
	int start_hour;
	int start_minute;
	int duration_minutes;
};


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
};


static bool hasEnding(string fullString, string ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

static struct war_weap
{
	string weap;
	short level;
} _war_weap[] =
{
	{ "The_Devastator", 1 },
	{ "LightingBlade", 2 },
	{ "DemonSlayer", 3 },
	{ "DarkExecutor", 4 },
	{ "BlackKnightTemple", 5 },
};

static struct mag_weap
{
	string weap;
	short level;
} _mag_weap[] =
{
	{ "PowerHeroWand", 1 },
	{ "BerserkWand(MS.30)", 2 },
	{ "DeathWand(MS.30)", 3 },
	{ "MagicWand(MS30-LLF)", 4 },
	{ "BlackMageTemple", 5 },
};

enum RelicStatus
{
	RELIC_MIDDLE,
	RELIC_HOLD,
	RELIC_ARESDEN,
	RELIC_ELVINE,
	RELIC_FLOOR
};

enum BattleRoyaleStatus
{
	BATTLE_COUNTING_TO_START_EVENT,
	BATTLE_COUNTING_FREEZETIME,
	BATTLE_COUNTING_TO_ENABLEDAMAGE,
	BATTLE_IN_ROUND
};

enum MasteryStatus
{
	MasteryNone,
	MasteryNpcDamage,
	MasteryRegen,
	MasteryVit,
	MasteryDefense
};

struct Recipe {
	std::string itemResult; // Nombre del ítem resultante
	int itemSpr; // Nombre del ítem resultante
	int itemSprFrame; // Nombre del ítem resultante
	char itemColor; // Nombre del ítem resultante
	std::vector<std::pair<std::string, int>> ingredients; // Lista de {material, cantidad requerida}
};

static struct EnemyKills_Quests
{
	int m_iQuestID;
	char  m_cMapName[12];
	char  m_cShowMapName[22];
	char  m_cRewardName[50];
	int m_iMaxKills;
}

ek_quests[] =
{
	{ 14, "city", "City", "DeathWand(MS.30)", 700 },
	{ 15, "city", "City", "DemonSlayer", 700 },

	{ 1, "city", "City", "MagicNecklace(MS22)", 30 },
	{ 2, "city", "City", "WarNecklace(HitP25)", 30 },
	{ 3, "city", "City", "MagNecklace(HitP25)", 30 },
	{ 4, "city", "City", "RingoftheAbaddon", 50 },
	{ 5, "city", "City", "RingofArcmage", 50 },
	{ 6, "city", "City", "DarkExecutor", 150 },
	{ 7, "city", "City", "MagicWand(MS30-LLF)", 150 },
	{ 8, "city", "City", "NecklaceOfXelima", 50 },

	{ 9, "city", "City", "PaFrags+2500", 20 },
	{ 10, "city", "City", "MaFrags+2500", 20 },
	{ 11, "city", "City", "MrFrags+2500", 20 },
	{ 12, "city", "City", "DrFrags+2500", 20 },
	{ 13, "city", "City", "HitPFrags+2500", 20 }
};


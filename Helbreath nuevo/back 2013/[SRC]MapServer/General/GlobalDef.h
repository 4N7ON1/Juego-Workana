
typedef unsigned long long uint64;
typedef signed long long int64;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;

#define DEF_SECURITY_LOGIN		865456432
#define DEF_ANTI_HACKERS		123245456

/***********************************************/

#define	DEF_ACTIVE_ANTILAM			true // false -> Anti Lam OFF / true -> Anti Lam ON

/***********************************************/

#define DEF_DEBUG

/***********************************************/
#define DEFENSE_LIMIT 9
#define DEF_LIMIT_MAX_FAKE_USERS		350 // numero maximo que se desea
#define DEF_LIMIT_STATSTONE				1 //numero max stones
#define DEF_LIMIT_REP					2 // * 1k 
#define DEF_LIMIT_ENCHANTING_UPGRADE    6
#define DEF_LIMIT_ENCHANTING_UPGRADE_SPECIAL    3
#define DEF_LIMIT_ENCHANTING_UPGRADE_HP    3
#define DEF_EXP_DIVIDER_PER_REBIRTH     0.85f // Reduce la experiencia en un 5% por cada nivel de renacimiento 0.95f
#define DEF_MAXMARKETCOINS              999999
/***********************************************/

#define DEF_EVENT_TIME_PRICE			51 // indicador del porcentaje del tiempo

/***********************************************/

// Purchase Assasains Points
#define DEF_ASSASAINPOINTS				5000

#define DEF_PURCHASE_ASSASAINPOINTS		5000
#define DEF_AP_EKNEED					150000
#define DEF_AP_CONTRIBNEED				0
#define DEF_AP_COINSNEED				0
#define DEF_AP_EXPRANKNEED				0

/***********************************************/

// Starter Kit
#define DEF_ACTIVE_KITSTARTER			false // true = ON / false OFF

#define DEF_TOTALITEMSTARTERKIT			12 // Remueve esta candidad de candys en el caso de no tener lugar en bag.

#define DEF_WAR_ITEM_STARTERKIT1		0 //3083  PowerBarbarian
#define DEF_WAR_ITEM_STARTERKIT2		0 //4909 WhiteMWand(MS.60)
#define DEF_WAR_ITEM_STARTERKIT3		0 //DemonSlayer 616 --- 762	GiantBattleHammer
#define DEF_WAR_ITEM_STARTERKIT4		0 //RingoftheAbaddon 631 --- 4964	RingofPowerWar
#define DEF_WAR_ITEM_STARTERKIT5		0 //860	NecklaceOfXelima //4968	NeckPowerXelima

#define DEF_MAGE_ITEM_STARTERKIT1		0 //4909 // WhiteMWand(MS.60)
#define DEF_MAGE_ITEM_STARTERKIT2		0 //3084 // PowerRapier
#define DEF_MAGE_ITEM_STARTERKIT3		0 //863	//KlonessWand(MS.30)
#define DEF_MAGE_ITEM_STARTERKIT4		0 //RingofArcmage 734 ---- 4965	RingofPowerMage
#define DEF_MAGE_ITEM_STARTERKIT5		0 //895	//MagicNecklace(MS18) //4966	NeckPower(MS.30)

#define DEF_TOTAL_CONTRIB_STARTERKIT	0
#define DEF_TOTAL_MPS_STARTERKIT		0 //No estaban puestos :P
#define DEF_TOTAL_CRITS_STARTERKIT		15000 //No estaban puestos :P
//Agregar Rep a nuevos personajes.
#define DEF_TOTAL_REP_STARTERKIT		0 //5000 //Nuevo

#define DEF_TOTAL_STARTERKIT_ZEMS		false // true = ON / false OFF

/***********************************************/

#define DEF_EVENTMAP_ALLVSALL		"avaevent"
#define DEF_EVENTMAP_HELLOFDEAT		"NewEvent"
#define DEF_EVENTMAP_TOWERDEFENSE	"TowerMap"
#define DEF_PRICE_EVENT_CITYVSCITY	"EventMap"
#define DEF_EVENTMAP_PVPEVENT		"Counter"
#define DEF_EVENTMAP_MARKETPLACE	"market"
#define DEF_EVENTMAP_WORLDWAR		"FightMap"
#define DEF_EVENTMAP_TOWER			"towerofh"
#define DEF_EVENTMAP_WORLDWARII		"WorldMap"
#define DEF_EVENTMAP_EVENTDK		"EventDk"
#define DEF_EVENTMAP_ALLEK			"evento13"
#define DEF_EVENTMAP_ALLDKEK		"evento14"

/***********************************************/

#define DEF_SUMMONMAP_HELLCLAW		"asdasdasda"
#define DEF_SUMMONMAP_TIGERWORM		"asdasdasda"
#define DEF_SUMMONMAP_WYVERN		"asdasdasda"
#define DEF_SUMMONMAP_FIRE_WYVERN	"asdasdasda"
#define DEF_SUMMONMAP_ABADDON		"asdasdasda"
#define DEF_SUMMONMAP_HELLDEATH		"asdasdasda"
#define DEF_SUMMONMAP_LIGHTWORM		"asdasdasda"

/***********************************************/

#define DEF_SADE					0 // Crusade
#define DEF_CTF						1 // CaptureTheFlag
#define DEF_RUSH					2 // Rush
#define DEF_TD						3 // TowerDefense
#define DEF_CVC						4 // City Vs City
#define DEF_HOD						5 // Hell Of Death
#define DEF_WW						6 // WorldWar
#define DEF_WWII					7 //World War II

/***********************************************/

#define DEF_MAXCRUSADESTRUCTURES		300
#define MAX_RUSHSTRUCTURES				100
#define MAX_CVCSTRUCTURES				120//100
#define MAX_DEFENCESTRUCTURESA			50
#define MAX_DEFENCESTRUCTURESE			50
#define MAX_DEFENCESTRUCTURES			100
#define DEF_MAXAGRICULTURE				200		//v2.19 2002-12-16
#define DEF_MAXTELEPORTLIST				100

// NPC drops
#define DEF_ITEMSPREAD_RANDOM	1
#define DEF_ITEMSPREAD_FIXED	2
#define MAX_NPCITEMDROP			25

#define DEF_STR            0x01
#define DEF_DEX            0x02
#define DEF_INT            0x03
#define DEF_VIT            0x04
#define DEF_MAG            0x05
#define DEF_CHR            0x06

/* GENERALS */
#define DEF_MSGTYPE_CONFIRM					0x0F14
#define DEF_MSGTYPE_REJECT					0x0F15

#define DEF_ENGLISHITEMNAME

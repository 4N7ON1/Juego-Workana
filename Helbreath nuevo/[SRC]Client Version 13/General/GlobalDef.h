//Helbreath Cursed Client

//#define DEF_TESTOFICIAL
//#define DEF_LOCAL
#define DEF_OFICIAL

#define DEF_SECURITY_LOGIN		865456432
#define DEF_ANTI_HACKERS		123245456

#define DEF_USING_LAUNCHER      0 // 1 -> ON // 0 -> OFF
#ifdef DEF_OFICIAL

//#define DEF_SERVER_IP	 		"149.104.76.237" 
//#define DEF_SERVER_IP	 		"38.60.242.233" 
//#define DEF_SERVER_IP	 		"35.235.114.103"//usa host
#define DEF_SERVER_IP	 		"192.168.0.59" //LAN
//#define DEF_SERVER_IP	 		"192.168.0.116"
//#define DEF_SERVERCOR_IP 		"38.54.40.231"
#define DEF_SERVERBR_IP			"192.168.0.59" //br
//#define DEF_SERVERARG_IP 		"149.104.76.237"
#define DEF_SERVERARG_IP 		"192.168.0.59" //arg
//#define DEF_SERVERMAL_IP 		"38.60.192.46"

#define DEF_SERVER_PORT			9735
#define NAME_WORLDNAME1			"YAP"
#define MSG_WORLDNAME1			"Helbreath Cursed"
#define MSG_HOMEPAGE			"http://www.hbcursed.com"
#define MSG_FORUMPAGE			"http://www.hbcursed.com"
#define MSG_CREATEACC			"http://hbcursed.com/index.php?c=Menu&a=CreateAcc"
#define MSG_CHANGEPASS			"http://hbcursed.com/index.php?c=Menu&a=cambiarpass"

#define DEF_UPPERVERSION			888888888
#define DEF_LOWERVERSION			999999999
#define DEF_ARGENTVERSION			555555555

#define DEF_STATS_LIMIT         200
#define DEF_UPPERVERSION_FANTASMA    14
#define DEF_LOWERVERSION_FANTASMA	 1
#define DEF_LOWERVERSION_FANTASMA2	 2
#define DEF_SMALLBAR			0 // 0 Barra grande - 1 Barra chica
//#define DEF_USE_DNS
#define DEF_SERVER_DNS 
//#define DEF_USE_LOGIN			// Login.cfg option
//#define  _DEBUGS 
#endif
//#define DEF_USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAXDIALOG				1
//#define DEF_SELECTSERVER		// Show/Hides server selection screen. (Activar para Proxys)

// #define DEF_COMMA_GOLD		// Allows to show comma nbe as original HB (ie: 1,200,000)
// #define DEF_4XXP_PER_LVL		// limits xp niv 140, then 4 x XP per lvl (affects XP shown as % on status)
            		
//#define DEF_MIXEDTORNER// listo le cambie eso por hacksDEF_WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....) 
#define DEF_MSGTYPE_CONFIRM								0x0F14
#define DEF_MSGTYPE_REJECT								0x0F15

/*** Special conditions   **/
#ifdef _DEBUGS
	#define DEF_MIXEDTORNER //DEF_WINDOWED_MODE es este c
#endif

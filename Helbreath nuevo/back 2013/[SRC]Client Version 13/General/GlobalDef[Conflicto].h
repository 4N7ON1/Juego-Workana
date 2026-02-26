//HELBREATH ARGENTINA LALO RAMOS Cliente V5
//PROGRAMMED BY ZEROEOYPNK
//#define DEF_TESTOFICIAL
//#define DEF_LOCAL
#define DEF_OFICIAL


#define DEF_SECURITY_LOGIN		546899787
#define DEF_USING_LAUNCHER      0 // 1 -> ON // 0 -> OFF
#ifdef DEF_OFICIAL
#define DEF_SERVER_IP	 		"74.91.113.207" // "74.91.112.80" //74.91.113.207 "lzworld.ddns.net"
#define DEF_SERVER_PORT			8469
#define NAME_WORLDNAME1			"JFN"
#define MSG_WORLDNAME1			"Helbreath Argentina Versión 5.00"
#define MSG_HOMEPAGE			"http://www.helbreathargentina.com"
#define MSG_FORUMPAGE			"http://www.helbreathargentina.com"
#define MSG_CREATEACC			"http://helbreathargentina.com/index.php?c=Menu&a=CreateAcc"
#define MSG_CHANGEPASS			"http://helbreathargentina.com/index.php?c=Menu&a=cambiarpass"
#define DEF_UPPERVERSION		4
#define DEF_LOWERVERSION		02
#define DEF_STATS_LIMIT         200
#define DEF_UPPERVERSION_FANTASMA    5
#define DEF_LOWERVERSION_FANTASMA	 0
#define DEF_LOWERVERSION_FANTASMA2	 0
#define DEF_SMALLBAR			0 // 0 Barra grande - 1 Barra chica
//#define DEF_USE_DNS
#define DEF_SERVER_DNS 
//#define DEF_USE_LOGIN			// Login.cfg option
//#define  _DEBUGS 
#endif
//#define DEF_USING_WIN_IME		// Used for unicode characters (unchecked)
#define DEF_MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
#define MAXDIALOG				1
//#define DEF_SELECTSERVER		// Show/Hides server selection screen. (Activar para Proxys)

// #define DEF_COMMA_GOLD		// Allows to show comma nbe as original HB (ie: 1,200,000)
// #define DEF_4XXP_PER_LVL		// limits xp niv 140, then 4 x XP per lvl (affects XP shown as % on status)


            		
//#define DEF_WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)
#define DEF_MSGTYPE_CONFIRM								0x0F14
#define DEF_MSGTYPE_REJECT								0x0F15


/*** Special conditions   ***/
#ifdef _DEBUGS
	#define DEF_WINDOWED_MODE
#endif

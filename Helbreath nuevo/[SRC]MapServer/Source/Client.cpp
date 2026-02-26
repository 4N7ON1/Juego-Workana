#include <windows.h>
#include <direct.h> 
#include <string>
#include "../Header/Client.h"
#include "../Header/Game.h"
#include <iostream>
#include <sstream> // Para construir cadenas de texto
#include <cstring> // Para memset
#include <thread> // Para std::thread


extern cCoreServer * c_map;
extern char            G_cTxt[512];
#pragma warning(disable : 4996)

CClient::CClient(HWND hWnd, int handle) 
{
	register int i;

	client = handle;
	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);
	m_bRealClient = false;

	m_iSpecialItem = 0;

	m_bAntiPull = false;
	m_iAntiPullCount = true;
	m_bHP_Neck = false;

	//HelBots
	m_bHelBot = false;
	m_mBlockedDirections.clear();
	m_iTargetPlayer = -1; 
	m_dwTargetSearchTime = 0;
	m_dwTargetSearchDelay = 1000;
	m_dwLastHealTime = timeGetTime();
	m_dwLastDebuffTime = timeGetTime();
	fakemovedir = -1;
	weapontype = 1;
	m_bAltDebuffStep = false;
	m_bBeingCalled = false;
	m_bReachedGuardPoint = false;
	m_iAssignedGuardPoint = -1;
	m_sCallX = -1;
	m_sCallY = -1;
	m_dwLastMoveTime = 0;
	m_dwLastAttackTime = 0;
	m_dwMoveDelay = 305;    // 250ms entre movimientos
	m_dwAttackDelay = 510;  // 320ms entre ataques
	m_iPots = 40;
	botState = BOT_IDLE;
	lastActionTime = 0;
	m_dwLastKnockbackTime = 0;
	m_sForceTeleportShop = 1;
	//

	m_bFishLuck = false;
	m_bMerienHero = false;
	m_bLuck = false;
	m_iEventPoints = 0;
	m_iAddedPA = 0;
	m_iAddedMA = 0;
	m_iAddedMR = 0;
	m_iAddedDR = 0;
	m_iAddedPhysDmg = 0;
	m_iAddedPhysHit = 0;
	m_iAddedMagDmg = 0;
	m_iAddedMagHit = 0;
	m_iAddedMS = 0;
	m_sWriteStatus = 0;
	m_sPurgeTeam = 0;
	m_iCharacterRegen = 0;
	m_iCharacterRegenTime = 0;
	m_iCharacterRegenMode = 0;

	m_iCharacterQuestx2 = 0;
	m_iCharacterQuestx3 = 0;
	m_iCharacterGoodRate = 0;

	m_iRegenEndu = 0;
	m_iRegenEnduTime = 0;

	m_sPlayerRole = 0;
	m_sRaiderLvl = 0;
	m_sHunterLvl = 0;

	ZeroMemory(m_cMacAdrees, sizeof(m_cMacAdrees));
	strcpy(m_cMacAdrees, "0");

	ZeroMemory(m_cProfile, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");

	ZeroMemory(m_cFireIndex, sizeof(m_cFireIndex));
	strcpy(m_cFireIndex, "#");
	m_sFireTime = 0;
	m_sFireStatus = 0;

	ZeroMemory(m_cLightingIndex, sizeof(m_cLightingIndex));
	strcpy(m_cLightingIndex, "#");
	m_sLightingTime = 0;
	m_sLightingStatus = 0;
	m_dwLastKeepAlive = GetTickCount();
	lastAttackerID = -1;
	lastDamageTime = 0;
	m_bIsHunterPlayer = false;

	m_sTalent = -1;
	spec_phys_damage = 0;
	spec_magic_damage = 0;

	m_bUdpSocket = false;
	sResolution = C1024x768;

	m_bInArenaLoby = false;
	m_bFishing = false;
	//mastery
	m_sCurrentMastery = MasteryNone;
	m_sDropRateLevel = 0;
	m_sNpcDamageLevel = 0;
	m_sRegenLevel = 0;
	m_sVitLevel = 0;
	m_sDefenseLevel = 0;
	m_iTokens = 0;
	
	//talent level
	m_sFireTalentLvl = 1;
	m_sIceTalentLvl = 1;
	m_sLightningTalentLvl = 1;

	//heaton fake limbo
	m_bForceDisconnect = false;
	m_bLimbo = false;
	m_bRelicHolder = false;
	//new dynamic resolution prlucas
	res_sX = 0;
	res_sY = 0;
	res_tilX = 0;
	res_tilY = 0;

	m_bIsDead = false;

	angelictype = 0;
	removestats = 0;

	m_sCandyInsert = 0;
	m_sCandyDelete = 0;
	m_sNoDrop = 0;

	m_iFirePoints = 0;
	m_iIcePoints = 0;
	m_iLightningPoints = 0;
	m_sFirstTimeNewColors = 1;

	//rank system hbarg

	m_sRankLevel = 0;
	m_iRankPoints = 0;
	m_iRankExp = 0;

	m_sRankAddStr = 0;
	m_sRankAddVit = 0;
	m_sRankAddInt = 0;
	m_sRankAddDex = 0;
	m_sRankAddMag = 0;

	//heaton 2024
	shard_light = 0;
	shard_strong = 0;

	fragment_hp = 0;
	fragment_dr = 0;
	fragment_hprec = 0;
	fragment_sprec = 0;
	fragment_mprec = 0;
	fragment_mr = 0;
	fragment_pa = 0;
	fragment_ma = 0;
	fragment_exp = 0;

	//HeatoN rebirth 2024
	m_iRebirthLevel = 0;
	m_sRebirthStatus = 0;
	m_sRebirthEnabled = 0;
	m_iBackRebirthLevel = 0;

	m_iBackExp = 0;
	m_iBackStatsPoint = 0;
	m_iBackLevel = 0;
	m_iBackStr = 0;
	m_iBackVit = 0;
	m_iBackInt = 0;
	m_iBackDex = 0;
	m_iBackChr = 0;
	m_iBackMag = 0;

	m_vDropItemList.clear();

	//heaton prevent lammers quest and exp
	m_iCheckCount = 0;

	//dm
	requestrevive = false;

	//tolerancia speed hack move
	m_iFailedAttempts = 0;

	vec_charquest.clear();
	vec_charekquest.clear();

	//AntiHack
	m_sNoCheckSpeed = false;
	m_sCheckCount = 0;
	ZeroMemory(m_cCharName, sizeof(m_cCharName));
	ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	ZeroMemory(m_cLocation, sizeof(m_cLocation));
	ZeroMemory(m_cMapName, sizeof(m_cMapName));

	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;
	TotalGuildMembers = -1;

	m_bIsInitComplete = FALSE;
	m_bActivarComandos = FALSE;
	m_iEnemyKillCount = 0;
	m_iEnemyKillTotalCount = 0;
	m_iPKCount = 0;
	m_iCityFriendKilled = 0;
	AssasainPoint = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_bIsOnlyClientAvailable = FALSE;
	m_bIsSafeAttackMode = FALSE;
	//No recall por 10 segundos - ZeroEoyPnk
	m_bRecall = true;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i] = NULL;
		m_ItemPosList[i].x = 40;
		m_ItemPosList[i].y = 30;
		m_bIsItemEquipped[i] = FALSE;
	}

	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pEventItemList[i] = NULL;
		m_EventItemPosList[i].x = 40;
		m_EventItemPosList[i].y = 30;
		m_bIsEventItemEquipped[i] = FALSE;
	}

	m_cArrowIndex = -1;

	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = NULL;
	}

	//heaton market
	for (i = 0; i < DEF_MAXMARKETITEMS; i++) {
		m_pMarketItem[i] = NULL;
	}

	// Magic - Skill 
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = NULL;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = NULL;

	for (i = 0; i < DEF_MAXACHIVEMENTS; i++)
	{
		Achivements[i].Contador = 0;
		Achivements[i].Complete = FALSE;
	}

	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = FALSE;
		m_iSkillUsingTimeID[i] = NULL;
	}

	for (int i = 0; i < 7; i++)
		m_moveTime[i] = 540;
	m_moveTurn = 0;
	for (int i = 0; i < 7; i++)
		m_runTime[i] = 300;
	m_runTurn = 0;

	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5;
	m_sType = 0;
	m_sOriginalType = 0;
	m_sAppr1 = 0;
	m_sAppr2 = 0;
	m_sAppr3 = 0;
	m_sAppr4 = 0;
	m_iApprColor = 0; // v1.4
	m_iApprColor2 = 0; // New Colors -> ZeroEoyPnk
	m_iStatus = 0;
	m_iLU_Pool = 0;
	bPHackWPE = FALSE;
	m_cSex = 0;
	m_cSkin = 0;
	m_cHairStyle = 0;
	m_cHairColor = 0;
	m_cUnderwear = 0;

	m_iIceSegments = 0;
	m_iLightingSegments = 0;
	m_iFireSegments = 0;

	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM = 0;
	m_cAttackBonus_L = 0;

	m_cSide = 0;
	m_iHitRatio = 0;
	m_iDefenseRatio = 0;
	m_iLastPing = 0;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i] = 0;
	m_iDamageAbsorption_Shield = 0;

	m_iHPstock = 0;
	m_iHPStatic_stock = 0;
	m_bIsKilled = FALSE;

	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++)
		m_cMagicEffectStatus[i] = 0;

	m_iWhisperPlayerIndex = -1;
	ZeroMemory(m_cWhisperPlayerName, sizeof(m_cWhisperPlayerName));

	m_iHungerStatus = 100;
	m_bIsWarLocation = FALSE;

	m_bIsPoisoned = FALSE;
	m_iPoisonLevel = NULL;

	m_iAdminUserLevel = 0;
	m_iRating = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall = 0;
	m_iTimeLeft_FirmStaminar = 0;

	m_bIsOnServerChange = FALSE;

	m_iExpStock = 0;

	m_iAllocatedFish = NULL;
	m_iFishChance = 0;

	m_iCharacterLuck = 0;

	ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress));
	ZeroMemory(m_cRealIPaddress, sizeof(m_cRealIPaddress));
	ZeroMemory(m_cIP2, sizeof(m_cIP2));
	m_b5MinutosFinales = FALSE;
	m_bIsOnWaitingProcess = FALSE;
	m_iSuperAttackLeft = 0;
	m_iSuperAttackCount = 0;

	m_sUsingWeaponSkill = 5;

	m_iManaSaveRatio = 0;
	m_iAddResistMagic = 0;
	m_iAddMagicHitting = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage = 0;
	m_bIsLuckyEffect = FALSE;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsAir = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire = 0;
	m_iAddAbsWater = 0;
	m_iAngelicStr = 0;
	m_iAngelicInt = 0;
	m_iAngelicDex = 0;
	m_iAngelicMag = 0;
	m_iComboAttackCount = 0;
	m_iDownSkillIndex = -1;

	m_iMagicDamageSaveItemIndex = -1;

	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

	m_iPartyRank = -1; // v1.42
	m_iPartyMemberCount = 0;
	m_iPartyGUID = 0;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}

	lastX = -1;
	lastY = -1;

	m_iAbuseCount = 0;
	m_bIsExchangeMode = FALSE;
	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime = 0;
	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0;

	m_iExchangeH = NULL;
	for (i = 0; i < DEF_MAXITEMSEXCHANGE; i++) {
		m_cExchangeItemIndex[i] = -1;
		m_iExchangeItemAmount[i] = 0;
	}
	ZeroMemory(m_cExchangeName, sizeof(m_cExchangeName));
	ZeroMemory(m_cExchangeItemName, sizeof(m_cExchangeItemName));
	m_bIsExchangeConfirm = FALSE;

	m_iQuest = NULL;
	m_iQuestID = NULL;
	m_iAskedQuest = NULL;
	m_iCurQuestCount = NULL;
	m_iItemQuest = NULL;
	m_iItemQuestID = NULL;
	m_iItemQuestCount = NULL;
	m_iItemQuestRewardType = NULL;
	m_iStatedQuest = NULL;
	m_iStatedQuestID = NULL;
	m_iStatedQuestCount = NULL;
	m_iStatedQuestRewardType = NULL;

	m_iQuestRewardType = NULL;
	m_iQuestRewardAmount = NULL;

	m_bMineLuck = false;
	m_iContribution = NULL;
	m_bQuestMatchFlag_Loc = FALSE;

	m_bIsQuestCompleted[0] = m_bIsQuestCompleted[1] = m_bIsQuestCompleted[2] = FALSE;

	m_bIsNeutral = FALSE;
	m_bIsObserverMode = FALSE;

	m_iSpecialEventID = 200081;

	m_iSpecialWeaponEffectType = 0;
	m_iSpecialWeaponEffectValue = 0;

	m_iAddHP = m_iAddSP = m_iAddMP = 0;
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddMR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;

	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;		// DEF_SPECABLTYTIMESEC 초마다 한번씩 특수 능력을 쓸 수 있다.
	m_iSpecialAbilityType = NULL;
	m_bIsSpecialAbilityEnabled = FALSE;
	m_iSpecialAbilityLastSec = 0;

	m_iSpecialAbilityEquipPos = 0;

	m_iMoveMsgRecvCount = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount = 0;
	m_iSkillMsgRecvCount = 0;

	m_iActivity = TRUE;

	m_iAlterItemDropIndex = -1;

	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;

	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;

	ZeroMemory(m_cLockedMapName, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = NULL;
	m_iDeadPenaltyTime = NULL;

	m_iCrusadeDuty = NULL;
	m_dwCrusadeGUID = NULL;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}
	m_iCSIsendPoint = NULL;

	m_bIsSendingMapStatus = FALSE;
	ZeroMemory(m_cSendingMapName, sizeof(m_cSendingMapName));

	m_iConstructionPoint = NULL;

	ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;

	m_dwFightzoneDeadTime = NULL;

	m_iPartyID = NULL;
	m_iPartyStatus = DEF_PARTYSTATUS_NULL;

	m_iReqJoinPartyClientH = NULL;
	ZeroMemory(m_cReqJoinPartyName, sizeof(m_cReqJoinPartyName));

	m_dwLastActionTime = NULL;
	m_bIsCheckingWhisperPlayer = FALSE;
	m_bIsBankModified = FALSE;

	m_iGizonItemUpgradeLeft = 0;

	m_dwAttackFreqTime = m_dwMagicFreqTime = m_dwMoveFreqTime = NULL; // v2.171
	m_bIsMoveBlocked = FALSE; // v2.171
	m_bIsAttackModeChange = FALSE; // v2.172 2002-7-2
	m_iIsOnTown = FALSE;
	m_bIsOnShop = FALSE; // v2.182 2002-11-15 사고 팔수 있는 곳인지 체크하는 변수 추가 
	m_bIsOnTower = FALSE; // v2.20 2002-12-23 마법을 배울수 있는곳인지 체크 한다.
	m_bIsOnWarehouse = FALSE; // v2.20 2002-12-23 창고인지 체크 한다.
	m_bIsHunter = FALSE; // v2.182 2002-11-15 사냥군인지 체크하는 변수 추가 
	m_dwWarmEffectTime = NULL; // v2.172 
	m_bIsInBuilding = FALSE;
	m_bIsOnCombatantZone = FALSE;
	m_cHeroBonus = 0;	// v2.23 18/02/06 17:15
	m_bInhibition = FALSE; // v2.95 31/05/06 18:40 Inhibition-Casting 
	m_bIsBeingResurrected = FALSE; // v2.95 02/06/06 16:19 Resurrection 
	m_bIsImpossibleZone = FALSE;

	m_bManaMe = FALSE;
	m_dwManaMe = NULL;


	m_bExpMe = FALSE;
	m_dwExpMe = NULL;

	//Auto Save ZeroEoyPnk
	b_AutoSaveTime = FALSE;

	RecallCoordX = -1;
	RecallCoordY = -1;
	TimeCoord = NULL;

	WHPage = 0;
	m_bSpecFeatures = false;
	m_bVipFeatures = false;

	NoClear = FALSE;
	TimerMove = -1;
	Moves = 0;
	Active = 0;

	KilledClientAFK = -1;
	Assasain = FALSE;
	FirstPosition = FALSE;

	m_iCoins = 0;

	TotalMobsKilled = 0;
	TotalDeath = 0;
	TotalClientKilled = 0;
	TotalQuestCompleted = 0;
	TotalStructuresKilled = 0;

	HourPlaying = MinPlaying = EventMinPlaying = 0;

	ActiveAvA = -1;

	CheckProcess = FALSE;
	Heredado = FALSE;

	Event_Killed = 0;
	Event_Death = 0;
	EventLevel = 0;
	EventID = 0;

	RndEventID = 0;

	Temporal_Rating = 0;

	TimeFlagCarrier = -1;

	//Antipull
	NoDelete = TRUE;
	TimeToDelete = -1;
	LogOut = FALSE;
	dw_LastMsg = 0;

	// New System -> Market Place -> HB Arg v4.4
	OpenSell = FALSE;
	for (i = 0; i < DEF_MAXSELLITEMS; i++) {
		MarketItem[i].sIndex = -1;
		MarketItem[i].m_wPrice = -1;
	}
	SellType = -1;
	BuyTo = -1;
	TotalItemSell = 0;
}


CClient::~CClient()
{
 int i;

	if (running) {
		running = false;

		// Enviar un paquete "dummy" para desbloquear recvfrom si est� esperando
		sendto(serverSocket, "", 1, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

		// Cerrar el socket
		closesocket(serverSocket);
	}

	if (m_pXSock != NULL) delete m_pXSock;
	for (i = 0; i < DEF_MAXITEMS; i++)
	if (m_pItemList[i] != NULL) {
		if (!m_pItemList[i]) return;
		delete m_pItemList[i];
		m_pItemList[i] = NULL;
	}

	/*for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pGungameItems[i] != NULL) {
			if (!m_pGungameItems[i]) return;
			delete m_pGungameItems[i];
			m_pGungameItems[i] = NULL;
		}*/

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
	if (m_pItemInBankList[i] != NULL) {
		delete m_pItemInBankList[i];
		m_pItemInBankList[i] = NULL;
	}

	for (i = 0; i < DEF_MAXMARKETITEMS; i++)
	{
		if (m_pMarketItem[i])
		{
			delete m_pMarketItem[i];
			m_pMarketItem[i] = NULL;
		}
	}
}

void CClient::AsignSocket(HWND hWnd) {
	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	m_bRealClient = true;
}

BOOL CClient::bCreateNewParty()
{
	int i;

	if (m_iPartyRank != -1) return FALSE;

	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}

	return TRUE;
}

int CClient::GetWearedItemIndex(short ItemID)
{
	short ItemIndex = 0;

	for (int i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
	{
		ItemIndex = m_sItemEquipmentStatus[i];

		if (ItemIndex > -1 && ItemIndex < DEF_MAXITEMEQUIPPOS)
			if (m_pItemList[ItemIndex] != NULL)
				if (m_pItemList[ItemIndex]->m_sIDnum == ItemID)
					return ItemIndex;
	}

	return -1;
}

short CClient::GetWearedItemIndex(int EquipPoss)
{
	return m_sItemEquipmentStatus[EquipPoss];
}

bool CClient::WearingItem(short ItemID)
{
	if (GetWearedItemIndex(ItemID) > -1) return true;
	return false;
}

bool CClient::WearingItem(short ItemID, int EquipPoss)
{
	short it = GetWearedItemIndex(EquipPoss);

	if (it > -1
		&& m_pItemList[it] != NULL
		&& m_pItemList[it]->m_sIDnum == ItemID)
		return true;

	return false;
}

void CClient::ResetClient() {
	register int i;

	m_pXSock = NULL;
	/*m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);*/

	m_bRealClient = false;

	ZeroMemory(m_cProfile, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");

	ZeroMemory(m_cCharName, sizeof(m_cCharName));
	ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	ZeroMemory(m_cLocation, sizeof(m_cLocation));
	ZeroMemory(m_cMapName, sizeof(m_cMapName));

	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;
	TotalGuildMembers = -1;

	m_bIsInitComplete = FALSE;
	m_bActivarComandos = FALSE;
	m_iEnemyKillCount = 0;
	m_iEnemyKillTotalCount = 0;
	m_iPKCount = 0;
	m_iCityFriendKilled = 0;
	AssasainPoint = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_bIsOnlyClientAvailable = FALSE;
	m_bIsSafeAttackMode = FALSE;
	//No recall por 10 segundos - ZeroEoyPnk
	m_bRecall = true;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_sItemEquipmentStatus[i] = -1;

	for (i = 0; i < DEF_MAXITEMS; i++) {
		if (m_pItemList[i] != NULL) {
			delete m_pItemList[i];
			m_pItemList[i] = NULL;
		}
	}

	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		if (m_pItemInBankList[i] != NULL) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i] = NULL;
		}
	}

	m_cArrowIndex = -1;

	// Magic - Skill 
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = NULL;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = NULL;

	for (i = 0; i < DEF_MAXACHIVEMENTS; i++)
	{
		Achivements[i].Contador = 0;
		Achivements[i].Complete = FALSE;
	}

	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = FALSE;
		m_iSkillUsingTimeID[i] = NULL;
	}

	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5;
	m_sType = 0;
	m_sOriginalType = 0;
	m_sAppr1 = 0;
	m_sAppr2 = 0;
	m_sAppr3 = 0;
	m_sAppr4 = 0;
	m_iApprColor = 0; // v1.4
	m_iApprColor2 = 0; // New Colors -> ZeroEoyPnk
	m_iStatus = 0;
	m_iLU_Pool = 0;
	bPHackWPE = FALSE;
	m_cSex = 0;
	m_cSkin = 0;
	m_cHairStyle = 0;
	m_cHairColor = 0;
	m_cUnderwear = 0;

	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM = 0;
	m_cAttackBonus_L = 0;

	m_cSide = 0;
	m_iHitRatio = 0;
	m_iDefenseRatio = 0;
	m_iLastPing = 0;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i] = 0;
	m_iDamageAbsorption_Shield = 0;

	m_iHPstock = 0;
	m_iHPStatic_stock = 0;
	m_bIsKilled = FALSE;

	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++)
		m_cMagicEffectStatus[i] = 0;

	m_iWhisperPlayerIndex = -1;
	ZeroMemory(m_cWhisperPlayerName, sizeof(m_cWhisperPlayerName));

	m_iHungerStatus = 100;
	m_bIsWarLocation = FALSE;

	m_bIsPoisoned = FALSE;
	m_iPoisonLevel = NULL;

	m_iAdminUserLevel = 0;
	m_iRating = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall = 0;
	m_iTimeLeft_FirmStaminar = 0;

	m_bIsOnServerChange = FALSE;

	m_iExpStock = 0;

	m_iAllocatedFish = NULL;
	m_iFishChance = 0;

	ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress));
	ZeroMemory(m_cRealIPaddress, sizeof(m_cRealIPaddress));
	ZeroMemory(m_cIP2, sizeof(m_cIP2));
	m_b5MinutosFinales = FALSE;
	m_bIsOnWaitingProcess = FALSE;
	m_iSuperAttackLeft = 0;
	m_iSuperAttackCount = 0;

	m_sUsingWeaponSkill = 5;

	m_iManaSaveRatio = 0;
	m_iAddResistMagic = 0;
	m_iAddMagicHitting = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage = 0;
	m_bIsLuckyEffect = FALSE;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsAir = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire = 0;
	m_iAddAbsWater = 0;
	m_iAngelicStr = 0;
	m_iAngelicInt = 0;
	m_iAngelicDex = 0;
	m_iAngelicMag = 0;
	m_iComboAttackCount = 0;
	m_iDownSkillIndex = -1;

	m_iMagicDamageSaveItemIndex = -1;

	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

	m_iPartyRank = -1; // v1.42
	m_iPartyMemberCount = 0;
	m_iPartyGUID = 0;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}

	m_iAbuseCount = 0;
	m_bIsExchangeMode = FALSE;
	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime = 0;
	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0;

	m_iExchangeH = NULL;
	for (i = 0; i < DEF_MAXITEMSEXCHANGE; i++) {
		m_cExchangeItemIndex[i] = -1;
		m_iExchangeItemAmount[i] = 0;
	}
	ZeroMemory(m_cExchangeName, sizeof(m_cExchangeName));
	ZeroMemory(m_cExchangeItemName, sizeof(m_cExchangeItemName));
	m_bIsExchangeConfirm = FALSE;

	m_iQuest = NULL;
	m_iQuestID = NULL;
	m_iAskedQuest = NULL;
	m_iCurQuestCount = NULL;
	m_iItemQuest = NULL;
	m_iItemQuestID = NULL;
	m_iItemQuestCount = NULL;
	m_iItemQuestRewardType = NULL;
	m_iStatedQuest = NULL;
	m_iStatedQuestID = NULL;
	m_iStatedQuestCount = NULL;
	m_iStatedQuestRewardType = NULL;

	m_iQuestRewardType = NULL;
	m_iQuestRewardAmount = NULL;

	m_iContribution = NULL;
	m_bQuestMatchFlag_Loc = FALSE;

	m_bIsQuestCompleted[0] = m_bIsQuestCompleted[1] = m_bIsQuestCompleted[2] = FALSE;

	m_bIsNeutral = FALSE;
	m_bIsObserverMode = FALSE;

	m_iSpecialEventID = 200081;

	m_iSpecialWeaponEffectType = 0;
	m_iSpecialWeaponEffectValue = 0;

	m_iAddHP = m_iAddSP = m_iAddMP = 0;
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddMR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;

	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;		// DEF_SPECABLTYTIMESEC 초마다 한번씩 특수 능력을 쓸 수 있다.
	m_iSpecialAbilityType = NULL;
	m_bIsSpecialAbilityEnabled = FALSE;
	m_iSpecialAbilityLastSec = 0;

	m_iSpecialAbilityEquipPos = 0;

	m_iMoveMsgRecvCount = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount = 0;
	m_iSkillMsgRecvCount = 0;

	m_iActivity = TRUE;

	m_iAlterItemDropIndex = -1;

	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;

	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;

	ZeroMemory(m_cLockedMapName, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = NULL;
	m_iDeadPenaltyTime = NULL;

	m_iCrusadeDuty = NULL;
	m_dwCrusadeGUID = NULL;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}
	m_iCSIsendPoint = NULL;

	m_bIsSendingMapStatus = FALSE;
	ZeroMemory(m_cSendingMapName, sizeof(m_cSendingMapName));

	m_iConstructionPoint = NULL;

	ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;

	m_dwFightzoneDeadTime = NULL;

	m_iPartyID = NULL;
	m_iPartyStatus = DEF_PARTYSTATUS_NULL;

	m_iReqJoinPartyClientH = NULL;
	ZeroMemory(m_cReqJoinPartyName, sizeof(m_cReqJoinPartyName));

	m_dwLastActionTime = NULL;
	m_bIsCheckingWhisperPlayer = FALSE;
	m_bIsBankModified = FALSE;

	m_iGizonItemUpgradeLeft = 0;

	m_dwAttackFreqTime = m_dwMagicFreqTime = m_dwMoveFreqTime = NULL; // v2.171
	m_bIsMoveBlocked = FALSE; // v2.171
	m_bIsAttackModeChange = FALSE; // v2.172 2002-7-2
	m_iIsOnTown = FALSE;
	m_bIsOnShop = FALSE; // v2.182 2002-11-15 사고 팔수 있는 곳인지 체크하는 변수 추가 
	m_bIsOnTower = FALSE; // v2.20 2002-12-23 마법을 배울수 있는곳인지 체크 한다.
	m_bIsOnWarehouse = FALSE; // v2.20 2002-12-23 창고인지 체크 한다.
	m_bIsHunter = FALSE; // v2.182 2002-11-15 사냥군인지 체크하는 변수 추가 
	m_dwWarmEffectTime = NULL; // v2.172 
	m_bIsInBuilding = FALSE;
	m_bIsOnCombatantZone = FALSE;
	m_cHeroBonus = 0;	// v2.23 18/02/06 17:15
	m_bInhibition = FALSE; // v2.95 31/05/06 18:40 Inhibition-Casting 
	m_bIsBeingResurrected = FALSE; // v2.95 02/06/06 16:19 Resurrection 
	m_bIsImpossibleZone = FALSE;

	this->m_bManaMe = FALSE;
	this->m_dwManaMe = NULL;

	//Auto Save ZeroEoyPnk
	b_AutoSaveTime = FALSE;

	RecallCoordX = -1;
	RecallCoordY = -1;
	TimeCoord = NULL;

	WHPage = 0;

	NoClear = FALSE;
	TimerMove = -1;
	Moves = 0;
	Active = 0;

	KilledClientAFK = -1;
	Assasain = FALSE;
	FirstPosition = FALSE;

	m_iCoins = 0;

	TotalMobsKilled = 0;
	TotalDeath = 0;
	TotalClientKilled = 0;
	TotalQuestCompleted = 0;
	TotalStructuresKilled = 0;

	HourPlaying = MinPlaying = EventMinPlaying = 0;

	ActiveAvA = -1;

	CheckProcess = FALSE;
	Heredado = FALSE;

	Event_Killed = 0;
	Event_Death = 0;
	EventLevel = 0;
	EventID = 0;

	RndEventID = 0;

	Temporal_Rating = 0;

	TimeFlagCarrier = -1;

	//Antipull
	NoDelete = TRUE;
	TimeToDelete = -1;
	LogOut = FALSE;
	dw_LastMsg = 0;

	// New System -> Market Place -> HB Arg v4.4
	OpenSell = FALSE;
	for (i = 0; i < DEF_MAXSELLITEMS; i++) {
		MarketItem[i].sIndex = -1;
		MarketItem[i].m_wPrice = -1;
	}
	SellType = -1;
	BuyTo = -1;
	TotalItemSell = 0;
	dw_LastSG = 0;
}

int CClient::GetStr()
{
	return m_iStr + m_iAngelicStr + m_sRankAddStr;
}

int CClient::GetMag()
{
	return m_iMag + m_iAngelicMag + m_sRankAddMag;
}

int CClient::GetVit()
{
	return m_iVit + m_sRankAddVit;
}

int CClient::GetDex()
{
	return m_iDex + m_iAngelicDex + m_sRankAddDex;
}

int CClient::GetInt()
{
	return m_iDex + m_iAngelicDex + m_sRankAddDex;
}

//HeatoN rebirth
int CClient::getRebirthStats()
{
	int result = 0;
	if (m_iRebirthLevel != 0)
	{
		result = m_iRebirthLevel * 3;
	}

	return result;
}


int CClient::GetMaxMP() const
{
	int iRet;

	iRet = (2 * (GetMag())) + (2 * m_iLevel) + ((GetInt()) / 2);

	return iRet;
}

int CClient::GetMaxSP() const
{
	int iRet;

	iRet = (2 * GetStr()) + (2 * m_iLevel);

	return iRet;
}

bool CClient::IsInsideCh()
{
	return strcmp(m_cMapName, "cityhall_1") == 0 || strcmp(m_cMapName, "cityhall_2") == 0;
}


void CClient::Notify(WORD msg, DWORD sV1 /*= 0*/, DWORD sV2 /*= 0*/, DWORD sV3 /*= 0*/, char * pString /*= nullptr*/, DWORD sV4 /*= 0*/, DWORD sV5 /*= 0*/, DWORD sV6 /*= 0*/, DWORD sV7 /*= 0*/, DWORD sV8 /*= 0*/, DWORD sV9 /*= 0*/, char * pString2 /*= nullptr*/, char cValueA /*= 0*/)
{
	c_map->SendNotifyMsg(NULL, client, msg, sV1, sV2, sV3, pString,
		sV4, sV5, sV6, sV7, sV8, sV9, pString2);
}

bool CClient::IsLocation(char * location)
{
	return string(m_cLocation) == location;
}

bool CClient::IsShopMap()
{
	return string(m_cMapName) == "gshop_1" || 
		string(m_cMapName) == "gshop_2" ||
		string(m_cMapName) == "wrhus_1" ||
		string(m_cMapName) == "wrhus_2";
}


bool CClient::IsInMap(char * mapname)
{
	return string(m_cMapName) == mapname;
}

void CClient::Send(int h, WORD msg, DWORD sV1, DWORD sV2, DWORD sV3,
	char * pString, DWORD sV4, DWORD sV5,
	DWORD sV6, DWORD sV7, DWORD sV8, DWORD sV9, char * pString2)
{

	c_map->SendNotifyMsg(NULL, h, msg, sV1, sV2, sV3, pString,
		sV4, sV5, sV6, sV7, sV8, sV9, pString2);
}

bool CClient::IsInsideDM()
{
	return strcmp(m_cMapName, "dm") == 0 && g_ev.Is(EventID::Deathmatch);
}

//HeatoN Auto fake system (limbo player) 
bool CClient::IsLimboMap()
{
	return strcmp(m_cMapName, "whouse") == 0
		|| strcmp(m_cMapName, "gshop_1") == 0 || strcmp(m_cMapName, "gshop_2") == 0
		|| strcmp(m_cMapName, "cityhall_1") == 0 || strcmp(m_cMapName, "cityhall_2") == 0
		|| strcmp(m_cMapName, "bsmith_1") == 0 || strcmp(m_cMapName, "bsmith_2") == 0
		|| strcmp(m_cMapName, "wrhus_1") == 0 || strcmp(m_cMapName, "wrhus_2") == 0
		|| strcmp(m_cMapName, "wzdtwr_1") == 0 || strcmp(m_cMapName, "wzdtwr_2") == 0;
}


void CClient::ClearBag()
{
	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem * item = m_pItemList[i];

		if (!item)	continue;

		c_map->ItemDepleteHandler(client, i, FALSE);
	}
}

int CClient::GetCurrentStatLevel(int statIndex) const {
	switch (statIndex) {
	case STAT_PA:      return m_iAddedPA;
	case STAT_MA:      return m_iAddedMA;
	case STAT_MR:      return m_iAddedMR;
	case STAT_DR:      return m_iAddedDR;
	case STAT_PHYS_DMG: return m_iAddedPhysDmg;
	case STAT_PHYS_HIT: return m_iAddedPhysHit;
	case STAT_MAG_DMG:  return m_iAddedMagDmg;
	case STAT_MAG_HIT:  return m_iAddedMagHit;
	case STAT_MS:       return m_iAddedMS;
	default:           return 0;
	}
}


// NPC Kill Counter System - HeatoN 2024
void CClient::AddNpcKill(int npcType, const char* npcName)
{
    if (npcName == NULL) return;

    // [DEBUG] Log NPC name to player
    char szDebug[256];
    wsprintf(szDebug, "[DEBUG] Killed '%s' (Type: %d)", npcName, npcType);
    c_map->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_STRINGMSG, NULL, NULL, NULL, szDebug);

    // Incrementar contador usando el NOMBRE como key
    std::string npcKey(npcName);
    m_mapNpcKillCount[npcKey]++;

    int currentKills = m_mapNpcKillCount[npcKey];
    int currentLevel = m_mapNpcHuntLevel[npcKey];
    
    int requiredKills = c_map->GetRequiredKillsByName(npcName, currentLevel);

    // If already at max level, we might want to stop counting
    if (c_map->m_mapNpcHuntConfig.count(npcKey) > 0)
    {
        auto& config = c_map->m_mapNpcHuntConfig[npcKey];
        if (currentLevel >= (int)config.m_iRequiredKills.size())
        {
            return; // Max level reached
        }
    }

    if (currentKills >= requiredKills)
    {
        // Resetear contador y subir nivel
        m_mapNpcKillCount[npcKey] = 0;
        m_mapNpcHuntLevel[npcKey]++;
        
        // [SOUND] Trigger character level up sound on client
        c_map->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_LEVELUP, NULL, NULL, NULL, "Level Tier!");

        // Notificar al usuario via chat local
        char szMsg[128];
        wsprintf(szMsg, "Hunting level UP for %s! (Level %d)", npcName, m_mapNpcHuntLevel[npcKey]);
        c_map->SendNotifyMsg(NULL, client, CLIENT_NOTIFY_STRINGMSG, NULL, NULL, NULL, szMsg);
    }
}

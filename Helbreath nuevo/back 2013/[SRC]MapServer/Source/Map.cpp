// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////
#include "../Header/Map.h"
#pragma warning(disable : 4996)
extern void PutLogFileList(char * cStr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap::CMap(class cCoreServer * pGame)
{
 int i, ix, iy;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = NULL;

	for (i = 0; i < DEF_MAXWAYPOINTCFG; i++) {
		m_WaypointList[i].x = -1;
		m_WaypointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMGAR; i++) {
		m_rcMobGenAvoidRect[i].top  = -1;
		m_rcMobGenAvoidRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXNMR; i++) {
		m_rcNoAttackRect[i].top  = -1;
		m_rcNoAttackRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXSPOTMOBGENERATOR; i++) {
		m_stSpotMobGenerator[i].bDefined = FALSE;
		m_stSpotMobGenerator[i].iTotalActiveMob = 0;
	}

	for (i = 0; i < DEF_MAXFISHPOINT; i++) {
		m_FishPointList[i].x = -1;
		m_FishPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMINERALPOINT; i++) {
		m_MineralPointList[i].x = -1;
		m_MineralPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXINITIALPOINT; i++) {
		m_pInitialPoint[i].x = -1;
		m_pInitialPoint[i].y = -1;
	}
 
	for (i = 0; i < 1000; i++)
		m_bNamingValueUsingStatus[i] = FALSE;

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		m_pOccupyFlag[i] = NULL;

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		m_pStrategicPointList[i] = NULL;

	for (i = 0; i < DEF_MAXENERGYSPHERES; i++) {
		m_stEnergySphereCreationList[i].cType = NULL;
		m_stEnergySphereGoalList[i].cResult   = NULL;
	}

	for (i = 0; i < DEF_MAXITEMEVENTS; i++) {
		ZeroMemory(m_stItemEventList[i].cItemName, sizeof(m_stItemEventList[i].cItemName));
		m_stItemEventList[i].iAmount = 0;
		m_stItemEventList[i].iTotalNum = 0;
		m_stItemEventList[i].iCurNum = 0;
		m_stItemEventList[i].iMonth  = 0;
		m_stItemEventList[i].iDay    = 0;
	}
	m_iTotalItemEvents = 0;
	
	m_iTotalActiveObject = 0;
	m_iTotalAliveObject  = 0;

	//m_sInitialPointX = 0;
	//m_sInitialPointY = 0;

	m_bIsFixedDayMode = FALSE;
	m_bIsFixedSnowMode = FALSE ;

	m_iTotalFishPoint = 0;
	m_iMaxFish = 0;
	m_iCurFish = 0;

	m_iTotalMineralPoint = 0;
	m_iMaxMineral = 0;
	m_iCurMineral = 0;

	m_pTile = NULL;

	m_cWhetherStatus = NULL;
	m_cType          = DEF_MAPTYPE_NORMAL;

	m_pGame = pGame;

	m_iLevelLimit = 0;
	m_iUpperLevelLimit = 0; // v1.4
	m_bMineralGenerator = FALSE;

	m_iTotalOccupyFlags = 0;
	
	m_bIsAttackEnabled = TRUE;
	m_cRandomMobGeneratorLevel = 0;

	m_bIsFightZone = FALSE;
	
	m_iTotalEnergySphereCreationPoint = 0;
	m_iTotalEnergySphereGoalPoint = 0;

	m_bIsEnergySphereGoalEnabled = FALSE;
	m_iCurEnergySphereGoalPointIndex = -1;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stSectorInfo[ix][iy].iPlayerActivity  = 0;

		m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stTempSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stTempSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
	
	m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].dX  = 0;
		m_stStrikePoint[i].dY  = 0;
		m_stStrikePoint[i].iHP = 0;
		m_stStrikePoint[i].iMapIndex = -1;
		ZeroMemory(m_stStrikePoint[i].cRelatedMapName, sizeof(m_stStrikePoint[i].cRelatedMapName));
	}
	m_iTotalStrikePoints = 0;
	m_bIsDisabled = FALSE;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}
	m_iTotalCrusadeStructures = 0;
	m_iTotalAgriculture = 0;

	m_bIsEnergySphereAutoCreation = FALSE;

	sMobEventAmount = 15 ;   
	m_bIsCitizenLimit = 0;
	m_iApocalypseMobGenType = 0;
	m_iApocalypseBossMobNpcID = 0;
	m_cDynamicGateType = 0;
	m_sDynamicGateX1 = 0;
	m_sDynamicGateX2 = 0;
	m_sDynamicGateY1 = 0;
	m_sDynamicGateY2 = 0;
	m_bIsGateAvailable = FALSE;
	m_bIsApocalypseMobSpawn = FALSE;
	m_iTotalDynamicGate = 0;
	m_sDynamicGateTgtX = 0;
	m_sDynamicGateTgtY = 0;
	m_bIsRecallImpossible = FALSE;
	m_bIsApocalypseMap = FALSE;
	m_bIsBossInMap = FALSE;
	m_bIsGateMap = FALSE;
	m_bIsInferniaAGate = FALSE;
	m_bIsInferniaBGate = FALSE;
	m_bIsProcellaGate = FALSE;
	m_iMaximumObjectDefault = FALSE;
	ZeroMemory(m_cDynamicGateDestMap, sizeof(m_cDynamicGateDestMap));
	TotalChars = 0;
	//m_pClearMap = new CClearMap;
}

CMap::~CMap()
{
	
 int i;
	
	if (m_pTile != NULL) 
		delete []m_pTile;	
 	
	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		if (m_pTeleportLoc[i] != NULL) delete m_pTeleportLoc[i];

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pOccupyFlag[i] != NULL) delete m_pOccupyFlag[i];

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		if (m_pStrategicPointList[i] != NULL) delete m_pStrategicPointList[i];
	
	//delete m_pClearMap;
}

void CMap::SetOwner(short sOwner, char cOwnerClass, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;
	
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sOwner      = sOwner;
	pTile->m_cOwnerClass = cOwnerClass;
}

char _tmp_cMoveDirX[9] = { 0,0,1,1,1,0,-1,-1,-1 };
char _tmp_cMoveDirY[9] = { 0,-1,-1,0,1,1,1,0,-1 };
BOOL CMap::bCheckFlySpaceAvailable(short sX, short sY, char cDir, short sOwner)
{
 class CTile * pTile;
 short dX, dY;

	if ((cDir <= 0) || (cDir > 8)) return 0;
	dX = _tmp_cMoveDirX[cDir] + sX;
	dY = _tmp_cMoveDirY[cDir] + sY;
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return 0;
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_sOwner != NULL) return 0;
	pTile->m_sOwner = sOwner;
	return 1;
}

void CMap::SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;
 
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sDeadOwner      = sOwner;
	pTile->m_cDeadOwnerClass = cOwnerClass;
}


void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner      = NULL;	
		*pOwnerClass = NULL;
		return;	
	}

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	*pOwner      = pTile->m_sOwner;
	*pOwnerClass = pTile->m_cOwnerClass;
	
	if (pTile->m_sOwner == 0) *pOwnerClass = 0;
}


void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner      = NULL;	
		*pOwnerClass = NULL;
		return;	
	}

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	*pOwner      = pTile->m_sDeadOwner;
	*pOwnerClass = pTile->m_cDeadOwnerClass;
}

 								  
BOOL CMap::bGetMoveable(short dX, short dY, short * pDOtype/*, class CItem * pTopItem*/) // v2.172
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pDOtype != NULL) *pDOtype = pTile->m_sDynamicObjectType; // v1.4
//	if (pTopItem != NULL) pTopItem = pTile->m_pItem[0]; // v2.172

	if (pTile->m_sOwner != NULL) return FALSE;
	if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
	if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	
	return TRUE;
}

BOOL CMap::bGetIsMoveAllowedTile(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
	if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	
	return TRUE;
}

BOOL CMap::bGetIsTeleport(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsTeleport == FALSE) return FALSE;
	
	return TRUE;
}

void CMap::ClearOwner(int iDebugCode, short sOwnerH, char cOwnerType, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
 
	if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
		pTile->m_sOwner      = NULL;
		pTile->m_cOwnerClass = NULL;
	}

	// 
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner      = NULL;
		pTile->m_cDeadOwnerClass = NULL;	
	}
}

void CMap::ClearDeadOwner(short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;	

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sDeadOwner      = NULL;
	pTile->m_cDeadOwnerClass = NULL;
}

BOOL CMap::bSetItem(short sX, short sY, class CItem * pItem)
{
 class CTile * pTile;	
 register int i;
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL; 

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	if (pTile->m_pItem[DEF_TILE_PER_ITEMS-1] != NULL) 
		delete pTile->m_pItem[DEF_TILE_PER_ITEMS-1]; //Linea Crash3
	else pTile->m_cTotalItem++;
	
	for (i = DEF_TILE_PER_ITEMS-2; i >= 0; i--) 
		pTile->m_pItem[i+1] = pTile->m_pItem[i];

	pTile->m_pItem[0] = pItem;
	//m_pClearMap->DeleteElements();
	//m_pClearMap->AddNewElement(pItem,pTile);
	return TRUE;
}


class CItem * CMap::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor) //v1.4 color
{
 class CTile * pTile;	
 class CItem * pItem;
 register int i;
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pItem =  pTile->m_pItem[0];
	if (pTile->m_cTotalItem == 0) return NULL;

	for (i = 0; i <= DEF_TILE_PER_ITEMS-2; i++)
		pTile->m_pItem[i] = pTile->m_pItem[i+1];
	pTile->m_cTotalItem--;
	pTile->m_pItem[pTile->m_cTotalItem] = NULL;
	
	if (pTile->m_pItem[0] == NULL) {
		*pRemainItemSprite      = 0;
		*pRemainItemSpriteFrame = 0;	
		*pRemainItemColor       = 0;
	}
	else
	{
		*pRemainItemSprite      = pTile->m_pItem[0]->m_sSprite;
		*pRemainItemSpriteFrame = pTile->m_pItem[0]->m_sSpriteFrame;
		*pRemainItemColor       = pTile->m_pItem[0]->m_cItemColor;
	}

	return pItem;
}

void CMap::pGetItem2(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor, DWORD dwTime) //v1.4 color
{
 class CTile * pTile;	
 class CItem * pItem;
 register int i;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_cTotalItem == 0) return;

	// Bucle inverso para chequear desde el ultimo punto hasta el primero...
	for (i = DEF_TILE_PER_ITEMS -1; i >= 0; i--)
	{
		if (pTile->m_pItem[i] != NULL)
		{
			if ((dwTime - pTile->m_pItem[i]->m_dwTimeDrop) > DEF_TIMETODELETE && !pTile->m_pItem[i]->isbrdropitem)
			{
				if(pTile->m_pItem[i] == NULL) return;
				pItem = pTile->m_pItem[i];
				//CrashAca
				if (pItem->m_cName == "ннннннннннннннннннннн") return;
				if (!pItem) return;
				delete pItem;
				pTile->m_cTotalItem--;
				pTile->m_pItem[i] = NULL;
			}
		}
	}
}

int CMap::iCheckItem(short sX, short sY)
{
 class CTile * pTile;	
 class CItem * pItem;
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_pItem[0] != NULL)
	{
		pItem =  pTile->m_pItem[0];
		if (pTile->m_cTotalItem == 0) return NULL;

		return pItem->m_sIDnum;
	}
	else
		return NULL;
}


BOOL CMap::bIsValidLoc(short sX, short sY)
{
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return FALSE;
	return TRUE;
}

BOOL CMap::bInit(char * pName)
{
 int i;
	ZeroMemory(m_cName, sizeof(m_cName));
	strcpy(m_cName, pName);

	ZeroMemory(m_cLocationName, sizeof(m_cLocationName));

	if (_bDecodeMapDataFileContents() == FALSE) 
		return FALSE;	

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = NULL;
    
	return TRUE;
}


BOOL CMap::_bDecodeMapDataFileContents()
{
 HANDLE hFile;
 char  cMapFileName[256], cHeader[260], cTemp[100];
 DWORD dwFileSize, nRead;
 register int i, ix, iy;
 char * token, cReadMode;
 char seps[] = "= \t\n";
 class CStrTok * pStrTok = NULL;
 class CTile * pTile;
 short * sp;

	ZeroMemory(cMapFileName, sizeof(cMapFileName));
	strcat(cMapFileName, "mapdata\\");
	strcat(cMapFileName, m_cName);
	strcat(cMapFileName, ".amd");
	
	hFile = CreateFile(cMapFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	dwFileSize = GetFileSize(hFile, NULL);
	
	ZeroMemory(cHeader, sizeof(cHeader));
	ReadFile(hFile, (char *)cHeader, 256, &nRead, NULL);

	for (i = 0; i < 256; i++) 
		if (cHeader[i] == NULL) cHeader[i] = ' ';

	cReadMode = 0;

	pStrTok = new class CStrTok(cHeader, seps);
	token = pStrTok->pGet();
	//token = strtok( cHeader, seps );   
	while( token != NULL )   {
			
		if (cReadMode != 0) {
			switch (cReadMode) {
			case 1:
				m_sSizeX = atoi(token);
				cReadMode = 0;
				break;
			case 2:
				m_sSizeY = atoi(token);
				cReadMode = 0;
				break;
			case 3:
				m_sTileDataSize = atoi(token);
				cReadMode = 0;
				break;
			}
		}	
		else {
			if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
			if (memcmp(token, "TILESIZE",8) == 0) cReadMode = 3; 
		}
			
		//token = strtok( NULL, seps );
		token = pStrTok->pGet();
	}

	m_pTile = (class CTile *)new class CTile[m_sSizeX * m_sSizeY];

	for (iy = 0; iy < m_sSizeY; iy++)
	for (ix = 0; ix < m_sSizeX; ix++) {
		ReadFile(hFile, (char *)cTemp, m_sTileDataSize, &nRead, NULL);	
		pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
		if ((cTemp[8] & 0x80) != 0) {
			 pTile->m_bIsMoveAllowed = FALSE;
		}
		else pTile->m_bIsMoveAllowed = TRUE;

		if ((cTemp[8] & 0x40) != 0) {
			 pTile->m_bIsTeleport = TRUE;
		}
		else pTile->m_bIsTeleport = FALSE;
		
		if ((cTemp[8] & 0x20) != 0) {
			pTile->m_bIsFarmingAllowed = TRUE;
		}
		else pTile->m_bIsFarmingAllowed = FALSE;

		sp = (short *)&cTemp[0];
		if (*sp == 19) {
			 pTile->m_bIsWater = TRUE;
		}
		else pTile->m_bIsWater = FALSE;
	}

	CloseHandle(hFile);

	if (pStrTok != NULL) delete pStrTok;
	return TRUE;
}


/*

BOOL CMap::_bDecodeMapDataFileContents()
{
	HANDLE hFile;
	char  cMapFileName[256], cHeader[260], cTemp[100];
	DWORD dwFileSize, nRead;
	register int i, ix, iy;
	char* token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok* pStrTok = NULL;
	class CTile* pTile;
	short* sp;

	// Variable para el log
	FILE* logFile = NULL;
	bool isWaterTilesMap = false;

	ZeroMemory(cMapFileName, sizeof(cMapFileName));
	strcat(cMapFileName, "mapdata\\");
	strcat(cMapFileName, m_cName);
	strcat(cMapFileName, ".amd");

	// Verificar si es el mapa watertiles
	if (strcmp(m_cName, "watertiles") == 0) {
		isWaterTilesMap = true;
		logFile = fopen("watertiles_blocked_tiles.log", "w");
		if (logFile) {
			fprintf(logFile, "Blocked tiles coordinates for watertiles map:\n");
			fprintf(logFile, "Format: X Y\n");
			fprintf(logFile, "----------------\n");
		}
	}

	hFile = CreateFile(cMapFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	dwFileSize = GetFileSize(hFile, NULL);

	ZeroMemory(cHeader, sizeof(cHeader));
	ReadFile(hFile, (char*)cHeader, 256, &nRead, NULL);

	for (i = 0; i < 256; i++)
		if (cHeader[i] == NULL) cHeader[i] = ' ';

	cReadMode = 0;

	pStrTok = new class CStrTok(cHeader, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		if (cReadMode != 0) {
			switch (cReadMode) {
			case 1:
				m_sSizeX = atoi(token);
				cReadMode = 0;
				break;
			case 2:
				m_sSizeY = atoi(token);
				cReadMode = 0;
				break;
			case 3:
				m_sTileDataSize = atoi(token);
				cReadMode = 0;
				break;
			}
		}
		else {
			if (memcmp(token, "MAPSIZEX", 8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY", 8) == 0) cReadMode = 2;
			if (memcmp(token, "TILESIZE", 8) == 0) cReadMode = 3;
		}
		token = pStrTok->pGet();
	}

	m_pTile = (class CTile*)new class CTile[m_sSizeX * m_sSizeY];

	for (iy = 0; iy < m_sSizeY; iy++) {
		for (ix = 0; ix < m_sSizeX; ix++) {
			ReadFile(hFile, (char*)cTemp, m_sTileDataSize, &nRead, NULL);
			pTile = (class CTile*)(m_pTile + ix + iy * m_sSizeY);

			if ((cTemp[8] & 0x80) != 0) {
				pTile->m_bIsMoveAllowed = FALSE;
				// Registrar coordenadas si es watertiles
				if (isWaterTilesMap && logFile) {
					fprintf(logFile, "%d %d\n", ix, iy);
				}
			}
			else {
				pTile->m_bIsMoveAllowed = TRUE;
			}

			if ((cTemp[8] & 0x40) != 0) {
				pTile->m_bIsTeleport = TRUE;
			}
			else pTile->m_bIsTeleport = FALSE;

			if ((cTemp[8] & 0x20) != 0) {
				pTile->m_bIsFarmingAllowed = TRUE;
			}
			else pTile->m_bIsFarmingAllowed = FALSE;

			sp = (short*)&cTemp[0];
			if (*sp == 19) {
				pTile->m_bIsWater = TRUE;
			}
			else pTile->m_bIsWater = FALSE;
		}
	}

	CloseHandle(hFile);

	// Cerrar el archivo de log si estс abierto
	if (logFile) {
		fclose(logFile);
	}

	if (pStrTok != NULL) delete pStrTok;
	return TRUE;
}*/
BOOL CMap::bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir)
{
 register int i;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
	if ((m_pTeleportLoc[i] != NULL) && (m_pTeleportLoc[i]->m_sSrcX == sX) && (m_pTeleportLoc[i]->m_sSrcY == sY)) {
		memcpy(pMapName, m_pTeleportLoc[i]->m_cDestMapName, 10);
		*pDx  = m_pTeleportLoc[i]->m_sDestX;
		*pDy  = m_pTeleportLoc[i]->m_sDestY;
		*pDir = m_pTeleportLoc[i]->m_cDir;
		return TRUE;
	}

	return FALSE;
}

void CMap::SetDynamicObject(WORD wID, short sType, short sX, short sY, DWORD dwRegisterTime)
{
 class CTile * pTile;	

	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return; 

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	pTile->m_wDynamicObjectID   = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

BOOL CMap::bGetDynamicObject(short sX, short sY, short *pType, DWORD *pRegisterTime, int * pIndex)
{
 class CTile * pTile;	

	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return FALSE; 

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != NULL) *pIndex = pTile->m_wDynamicObjectID;

	return TRUE;
}

int CMap::iGetEmptyNamingValue()
{
 register int i;

	for (i = 0; i < 1000; i++)
	if (m_bNamingValueUsingStatus[i] == FALSE) {

		m_bNamingValueUsingStatus[i] = TRUE;
		return i;
	}

	return -1;
}

void CMap::SetNamingValueEmpty(int iValue)
{
	m_bNamingValueUsingStatus[iValue] = FALSE;
}

BOOL CMap::bGetIsWater(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsWater == FALSE) return FALSE;
	
	return TRUE;
}


BOOL CMap::bGetIsFarm(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsFarmingAllowed == FALSE) return FALSE;
	
	return TRUE;
}


int CMap::iAnalyze(char cType, int * pX, int * pY, int * pV1, int * pV2, int * pV3)
{
	switch (cType) {
	case 1:		
		break;
	}
	return 0;
}

void CMap::SetTempMoveAllowedFlag(int dX, int dY, BOOL bFlag)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

int CMap::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI)
{
 int i;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;
	
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++) 
	if (m_pOccupyFlag[i] == NULL) {
		m_pOccupyFlag[i] = new class COccupyFlag(dX, dY, iSide, iEKNum, iDOI);
		if (m_pOccupyFlag == NULL) return -1;
		else return i;
	}
	
	return -1;
}


void CMap::BorrarFlag()
{
	int d;
	for(d=0;d<DEF_MAXOCCUPYFLAG;d++)
	{
		delete m_pOccupyFlag[d];
		m_pOccupyFlag[d]=NULL;
	}
}

void CMap::ClearSectorInfo()
{
 int ix, iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
}

void CMap::ClearTempSectorInfo()
{
 int ix, iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stTempSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stTempSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
}

void CMap::_SetupNoAttackArea()
{
 int i, ix, iy;
 class CTile * pTile;	

	for (i = 0; i < DEF_MAXNMR; i++) {
		if ((m_rcNoAttackRect[i].top > 0)) {
			// No-Magic-Area
			for (ix = m_rcNoAttackRect[i].left; ix <= m_rcNoAttackRect[i].right; ix++)
			for (iy = m_rcNoAttackRect[i].top; iy <= m_rcNoAttackRect[i].bottom; iy++) {
				pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
				pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;
			}
		}
		else if (m_rcNoAttackRect[i].top == -10) {
			for (ix = 0; ix < m_sSizeX; ix++)
			for (iy = 0; iy < m_sSizeY; iy++) {
				pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
				pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;
			}
		}
	}
}

int CMap::iGetAttribute(int dX, int dY, int iBitMask)
{
 class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	return (pTile->m_iAttribute & iBitMask);
}

BOOL CMap::bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide)
{
 register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
	if (m_stCrusadeStructureInfo[i].cType == NULL) {
		m_stCrusadeStructureInfo[i].cType = cType;
		m_stCrusadeStructureInfo[i].cSide = cSide;
		m_stCrusadeStructureInfo[i].sX = sX;
		m_stCrusadeStructureInfo[i].sY = sY;

		m_iTotalCrusadeStructures++;
		return TRUE;
	}

	return FALSE;
}

BOOL CMap::bRemoveCrusadeStructureInfo(short sX, short sY)
{
 register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
	if ((m_stCrusadeStructureInfo[i].sX == sX) && (m_stCrusadeStructureInfo[i].sY == sY)) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
		goto RCSI_REARRANGE;
	}

	return FALSE;

RCSI_REARRANGE:;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES-1; i++)
	if ((m_stCrusadeStructureInfo[i].cType == NULL) && (m_stCrusadeStructureInfo[i+1].cType != NULL)) {
		m_stCrusadeStructureInfo[i].cType = m_stCrusadeStructureInfo[i+1].cType;
		m_stCrusadeStructureInfo[i].cSide = m_stCrusadeStructureInfo[i+1].cSide;
		m_stCrusadeStructureInfo[i].sX = m_stCrusadeStructureInfo[i+1].sX;
		m_stCrusadeStructureInfo[i].sY = m_stCrusadeStructureInfo[i+1].sY;

		m_stCrusadeStructureInfo[i+1].cType = NULL;
		m_stCrusadeStructureInfo[i+1].cSide = NULL;
		m_stCrusadeStructureInfo[i+1].sX = NULL;
		m_stCrusadeStructureInfo[i+1].sY = NULL;
	}

	m_iTotalCrusadeStructures--;
	return TRUE;
}


BOOL CMap::bAddCropsTotalSum()
{
	if(m_iTotalAgriculture < DEF_MAXAGRICULTURE)
	{
		m_iTotalAgriculture++;
		return TRUE;
	}
	return FALSE;
}

BOOL CMap::bRemoveCropsTotalSum()
{
	if(m_iTotalAgriculture < DEF_MAXAGRICULTURE)
	{
		m_iTotalAgriculture--;
		if(m_iTotalAgriculture < 0)
		{
			m_iTotalAgriculture = 0;
		}
		return TRUE;
	}
	return FALSE;
}

void CMap::RestoreStrikePoints()
{
 int i;

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].iHP = m_stStrikePoint[i].iInitHP;
	}
}

BOOL CMap::bApocalypseGate()
{
 class CTile * pTile;
 register int i, LR, TB;
 short sLeft, sTop, sRight, sBottom;

	for (i = 0; i < m_iTotalDynamicGate; i++) {
		sLeft	= m_sDynamicGateX1;
		sTop	= m_sDynamicGateY1;  
		sRight	= m_sDynamicGateX2;
		sBottom = m_sDynamicGateY2;
		for (LR = sLeft; LR <= sRight; LR++) {
			for (TB = sTop; TB <= sBottom; TB++) {
				pTile = (class CTile *)(m_pTile + TB + LR*m_sSizeY);
				pTile->m_bIsApocalypseGate = TRUE;
				pTile->m_cApocalypseGateMap = i;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CMap::bApocalypseGateTeleporter(short dX, short dY, char * cMapName, short * tX, short * tY)
{
 class CTile * pTile;	
	
	if (m_bIsGateAvailable == 0) return FALSE;
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	if (pTile->m_bIsApocalypseGate == TRUE) {
		if (cMapName != NULL) {
			strlen(m_cDynamicGateDestMap);
			memcpy(cMapName, m_cDynamicGateDestMap, pTile->m_cApocalypseGateMap); 
		}
		if (tX != NULL) {
			tX = (short *)m_sDynamicGateTgtX;
		}
		if (tY != NULL) {
			tY = (short *)m_sDynamicGateTgtY;
		}
		return TRUE;
	}
	return FALSE;
}
//verlalo
char _tmp_cEmptyAreaX[] = { 0, 1, 1, 0, -1, -1, -1, 0 ,1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2 };
char _tmp_cEmptyAreaY[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2 };

void CMap::SetBigOwner(short sOwner, char cOwnerClass, short pX, short pY, char cArea)
{
 short sX, sY, sAreaSquared;
 class CTile * pTile;
 register int i;

	sX = 0;
	sY = 0;
	sAreaSquared = cArea*cArea;
	if ((cArea > 5) || (cArea < 2)) return;
	if ((pX < 0) || (pX >= m_sSizeX) || (pY < 0) || (pY >= m_sSizeY)) return; 
	for (i = 0; i < sAreaSquared; i++) {
		sX = pX + _tmp_cEmptyAreaX[i];
		sY = pY + _tmp_cEmptyAreaY[i];	
		pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
		pTile->m_sOwner = sOwner;
		pTile->m_cOwnerClass = cOwnerClass;
	}
	return;
}

void CMap::ClearBigOwner(short sOwnerH, char cOwnerType, short pX, short pY, char cArea)
{
 short sX, sY, sAreaSquared;
 class CTile * pTile;
 register int i;

	sX = 0;
	sY = 0;
	sAreaSquared = cArea*cArea;
	if ((pX < 0) || (pX >= m_sSizeX) || (pY < 0) || (pY >= m_sSizeY)) return; 
	pTile = (class CTile *)(m_pTile + pX + pY*m_sSizeY);
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner      = NULL;
		pTile->m_cDeadOwnerClass = NULL;
	}
	for (i = 0; i < sAreaSquared; i++) {
		sX = pX + _tmp_cEmptyAreaX[i];
		sY = pY + _tmp_cEmptyAreaY[i];	
		pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
		if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
			pTile->m_sOwner      = NULL;
			pTile->m_cOwnerClass = NULL;
		}
	}
}

BOOL CMap::bGetMoveableArea(short sOwnerH, short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return FALSE;
	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if ((pTile->m_sOwner == NULL) && (pTile->m_sOwner == sOwnerH)) return FALSE;
	if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
	if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	return TRUE;
}

BOOL CMap::bGetBigMoveable(short sOwnerH,short pX, short pY, char cArea) // v2.172
{
	class CTile * pTile;	
	short sX, sY, sAreaSquared;
	register int i;
	
	sX = 0;
	sY = 0;
	sAreaSquared = cArea*cArea;
	
	if ((pX < 20) || (pX >= m_sSizeX - 20) || (pY < 20) || (pY >= m_sSizeY - 20)) return FALSE;
	for (i = 0; i < sAreaSquared; i++) {
		sX = pX + _tmp_cEmptyAreaX[i];
		sY = pY + _tmp_cEmptyAreaY[i];	
		pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
		if ((pTile->m_sOwner != NULL)&&(pTile->m_sOwner != sOwnerH)) return FALSE;
		if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
		if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	}
	return TRUE;
}
/*
#include "Header\\Game.h"
extern char G_cTxt[512];

int ITEMSPREAD_FIEXD_COORD[25][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 }, { 2, 0 },
{ 2, 1 }, { 2, 2 }, { 1, 2 }, { 0, 2 }, { -1, 2 }, { -2, 2 }, { -2, 1 }, { -2, 0 }, { -2, -1 }, { -2, -2 }, { -1, -2 }, { 0, -2 }, { 1, -2 }, { 2, -2 } };

std::map<std::string, std::vector<std::string>> npcMaterials = {
	{ "Orc", {"OrcMeat", "OrcLeather", "OrcTeeth"} },
	{ "Skeleton", {"SkeletonBones"} },
	{ "Unicorn", {"UnicornHeart", "UnicornHorn", "UnicornMeat", "UnicornLeather"} },
	{ "Troll", {"TrollHeart", "TrollMeat", "TrollLeather", "TrollClaw"} },
	{ "Scorpion", {"ScorpionPincers", "ScorpionMeat", "ScorpionSting", "ScorpionSkin"} },
	{ "Cyclops", {"CyclopsEye", "CyclopsHandEdge", "CyclopsHeart", "CyclopsMeat", "CyclopsLeather"} },
	{ "Orge", {"OgreHair", "OgreHeart", "OgreMeat", "OgreLeather", "OgreTeeth", "OgreClaw"} },
	{ "Stone-Golem", {"StoneGolemPiece"} },
	{ "Clay-Golem", {"LumpofClay"} },
	{ "WereWolf", {"WerewolfHeart", "WerewolfNail", "WerewolfMeat", "WerewolfTail", "WerewolfTeeth", "WerewolfLeather", "WerewolfClaw"} },
	{ "Hellbound", {"HelboundHeart", "HelboundLeather", "HelboundTail", "HelboundTeeth", "HelboundClaw", "HelboundTongue"} },
	{ "Demon", {"DemonHeart", "DemonMeat", "DemonLeather", "DemonEye"} }
};

#define CONTRIB_SMALL 99901
#define MAJ_SMALL 99902
#define REP_SMALL 99903

#define CONTRIB_MEDIUM 99904
#define MAJ_MEDIUM 99905
#define REP_MEDIUM 99906

#define COIN_SMALL 99907
#define COIN_MEDIUM 99908
#define COIN_BIG 99909
#define ICESEGMENTS 99910
#define FIRESEGMENTS 99911
#define LIGHTNINGSEGMENTS 99912

//HASTA ACA
void cCoreServer::DeleteNpc(int iNpcH, BOOL bHeld, BOOL Drop)
{
#ifdef DEF_DEBUG
	try {
#endif
		int  i, iNamingValue, iItemID, iNumItem, iItemIDs[MAX_NPCITEMDROP];
		char cTmp[21], cItemName[21], cTemp[256];
		class CItem * pItem;
		DWORD dwCount, dwTime;
		POINT	ItemPositions[MAX_NPCITEMDROP];
		SYSTEMTIME SysTime;
		char cTxt[100];
		int k;
		short sAttackerH;


		if (m_pNpcList[iNpcH] == NULL) return;

		iItemID = 0;
		iNumItem = 0;

		dwTime = timeGetTime();

		SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_COMMAND_LOG, DEF_MSGTYPE_REJECT, NULL, NULL, NULL);
		if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
		}
		else {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
		}
		ZeroMemory(cTmp, sizeof(cTmp));
		strcpy_secure(cTmp, (char *)(m_pNpcList[iNpcH]->m_cName + 2));
		iNamingValue = atoi(cTmp);

		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalActiveObject--;
		if (Drop == FALSE) m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject--;

		// Spot-mob-generator���� 
		if (m_pNpcList[iNpcH]->m_iSpotMobIndex != NULL)
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stSpotMobGenerator[m_pNpcList[iNpcH]->m_iSpotMobIndex].iCurMobs--;

		RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);
		
		if (m_pNpcList[iNpcH]->m_sType != 66 && m_pNpcList[iNpcH]->m_sType != 73 && m_pNpcList[iNpcH]->m_sType != 81)
		{
			bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, NULL);

			if (string(m_pNpcList[iNpcH]->m_cMapName) == "maze")
			{
				mobs_maze--;
				SendMazeMobCount();
				if (mobs_maze == 0)
				{
					ChangeRoaEventState();
				}
			}

			delete m_pNpcList[iNpcH];
			m_pNpcList[iNpcH] = NULL;
			return;
		}

		switch (m_pNpcList[iNpcH]->m_sType) {
		case 36:
		case 37:
		case 38:
		case 39:
		case 42:
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCrusadeStructureInfo(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

			for (i = 0; i < DEF_MAXGUILDS; i++) {
				if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID) {
					m_pGuildTeleportLoc[i].m_dwTime = dwTime;
					m_pGuildTeleportLoc[i].m_iV2--;
					if (m_pGuildTeleportLoc[i].m_iV2 < 0) m_pGuildTeleportLoc[i].m_iV2 = 0;
					break;
				}
			}
			break;

		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 51:
			for (i = 0; i < DEF_MAXGUILDS; i++) {
				if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID) {
					m_pGuildTeleportLoc[i].m_iNumSummonNpc--;
					if (m_pGuildTeleportLoc[i].m_iNumSummonNpc < 0) m_pGuildTeleportLoc[i].m_iNumSummonNpc = 0;
					break;
				}
			}
			break;

		case 64:	//v2.19 2002-12-16 ���� ��ų ����
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum();
			break;
		}

		// DelayEvent�� ���� 
		bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, NULL);
		int iItemprobability = 65;

		if ((m_iMiddlelandMapIndex == m_pNpcList[iNpcH]->m_cMapIndex) && (m_iMiddlelandMapIndex != -1)) iItemprobability = 55;
		if (m_iWhouseMapIndex == m_pNpcList[iNpcH]->m_cMapIndex) iItemprobability = 15;
		if (m_pNpcList[iNpcH]->m_bIsSummoned == FALSE) {
			pItem = new class CItem;
			ZeroMemory(cItemName, sizeof(cItemName));

			//V10 Reapertura low drops
			switch (m_pNpcList[iNpcH]->m_sType) {
			case 1: // Mercenary
			case 2: // Mercenary
			case 3: // Mercenary
			case 4: // Mercenary
			case 5: // Mercenary
			case 6: // Mercenary
			case 36: // Sade Structure
			case 37: // Sade Structure
			case 38: // Sade Structure
			case 39: // Sade Structure
			case 40: // Sade Structure
			case 41: // Sade Structure
			case 42: // Sade Structure
			case 87: // Tower Denfense Structure
			case 89: // Tower Denfense Structure
			case 92: // Reliquia
			case 55: // Rabbit
			case 56: // Cat
				// No drop
				break;
			case 10: //Slime //New Rare Drop Helbreath Argentina v4
				switch (iDice(1, 2)) {
				case 1: if (iDice(1, 10) >= 5) iItemID = 220; break;	// "SlimeJelly"
				case 2: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"/ *
				case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"* /
				default: break;
				}
				break;
			case 11: //Skeleton
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 20) == 1) iItemID = 219; break;	// "SkeletonBones"
				case 2: if (iDice(1, 100) == 5) iItemID = 650; break;	// "RubyRing"
				case 3: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"/ *
				/ *case 4: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 5: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"* / * /
				case 4: if (iDice(1, 5000) == 500) iItemID = 650; break;	// "EmeraldRing"
				default: break;
				}
				break;
			case 12: //StoneGolem
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 221; break;		// "StoneGolemPiece"
				case 2: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				/ *case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"* /
				case 3: if (iDice(1, 2500) == 50) iItemID = 647; break;		// "NecklaceOfStoneGolem"
				case 4: if (iDice(1, 4500) == 100) iItemID = 858; break;		// "NecklaceOfMerien"
				default: break;
				}
				break;
			case 13://Cyclops
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 194; break;	// "CyclopsEye"
				case 2: if (iDice(1, 40) == 1) iItemID = 195; break;	// "CyclopsHandEdge"
				case 3: if (iDice(1, 30) == 1) iItemID = 196; break;	// "CyclopsHeart"
				case 4: if (iDice(1, 20) == 1) iItemID = 197; break;	// "CyclopsMeat"
				case 5: if (iDice(1, 40) == 1) iItemID = 198; break;	// "CyclopsLeather"
				case 6: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;	// Rep Small ball
				case 7: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"
				/ *case 8: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 9: if (iDice(1, 1000) == 150) iItemID = 657; break;	// "StoneOfMerien"* /
				case 8: if (iDice(1, 500) == 15) iItemID = 308; break;	// "Neck(MS10)"
				case 9: if (iDice(1, 500) == 15) iItemID = 300; break;	// "Neck(RM10)"
				case 10: if (iDice(1, 500) == 15) iItemID = 311; break;	// "Neck(DF10)"
				default: break;
				}
				break;
			case 14: //orco
				switch (iDice(1, 12)) {
				case 1: if (iDice(1, 11) == 1) iItemID = 206; break;		// "OrcMeat"
				case 2: if (iDice(1, 20) == 1) iItemID = 207; break;		// "OrcLeather"
				case 3: if (iDice(1, 21) == 1) iItemID = 208; break;		// "OrcTeeth"
				case 4: if (iDice(1, 150) == 15) iItemID = 336; break;		// "SaphireRing"
				case 5: if (iDice(1, 150) == 25) iItemID = 333; break;		// "PlatiniumRing"
				case 6: if (iDice(1, 150) == 35) iItemID = 334; break;		// "LuckyGoldRing"
				case 7: if (iDice(1, 150) == 45) iItemID = 337; break;		// "RubyRing"
				case 8: if (iDice(1, 1000) == 15) iItemID = 335; break;		// "EmeraldRing"
				case 9: if (iDice(1, 100) == 45) iItemID = 634; break;		// "RingOfWizard"
				case 10: if (iDice(1, 200) == 25) iItemID = 635; break;		// "RingOfMage"
				case 11: if (iDice(1, 1000) == 35) iItemID = 650; break;		// "ZemstoneofSacrifice"
				/ *case 12: if (iDice(1, 1000) == 45) iItemID = 656; break;		// "StoneOfXelima"
				case 13: if (iDice(1, 1000) == 25) iItemID = 657; break;		// "StoneOfMerien"* /
				case 12: if (iDice(1, 5000) == 500) iItemID = 638; break;		// "NecklaceOfFireProtection"
				default: break;
				}
				break;
			case 16: //Ant
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 9) == 1)  iItemID = 192; break;	// "AntLeg"
				case 2: if (iDice(1, 10) == 1) iItemID = 193; break;	// "AntFeeler"
				case 3: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"/ *
				case 4: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 5: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"* /
				default: break;
				}
				break;
			case 17: //Scorpion
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 40) == 1) iItemID = 215; break;		// "ScorpionPincers"
				case 2: if (iDice(1, 20) == 1) iItemID = 216; break;		// "ScorpionMeat"
				case 3: if (iDice(1, 40) == 1) iItemID = 217; break;		// "ScorpionSting"
				case 4: if (iDice(1, 40) == 1) iItemID = 218; break;		// "ScorpionSkin"
				case 5: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 6: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
/ *
				case 7: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 8: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"* /
				case 7: if (iDice(1, 500) == 5) iItemID = 634; break;		// "RingOfWizard"
				case 8: if (iDice(1, 500) == 5) iItemID = 336; break;		// "SaphireRing"
				case 9: if (iDice(1, 500) == 145) iItemID = 333; break;	// "PlatiniumRing"
				default: break;
				}
				break;
			case 18: // Zombie
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 650; break;			// "ZemstoneofSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;			// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;			// "StoneOfMerien"
				case 4: if (iDice(1, 800) == 3) iItemID = REP_SMALL; break;			// Rep Small ball
				default: break;
				}
				break;
			case 22: //Snake
				switch (iDice(1, 8)) {
				case 1: if (iDice(1, 15) == 1) iItemID = 188; break;	// "SnakeMeat"
				case 2: if (iDice(1, 16) == 1) iItemID = 189; break;	// "SnakeSkin"
				case 3: if (iDice(1, 16) == 1) iItemID = 190; break;	// "SnakeTeeth"
				case 4: if (iDice(1, 17) == 1) iItemID = 191; break;	// "SnakeTounge"
				case 5: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"
				case 6: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 7: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				case 8: if (iDice(1, 5000) == 50) iItemID = 640; break;	// "NecklaceOfSufferent"
				default: break;
				}
				break;
			case 23: //ClayGolem
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 205; break;		// "LumpOfClay"
				case 2: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"						
				default: break;
				}
				break;
			case 27: //Hellbound
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 40) == 1) iItemID = 199; break;	// "HelhoundHeart"
				case 2: if (iDice(1, 30) == 1) iItemID = 200; break;	// "HelhoundLeather"
				case 3: if (iDice(1, 30) == 1) iItemID = 201; break;	// "HelhoundTail"
				case 4: if (iDice(1, 30) == 1) iItemID = 202; break;	// "HelhoundTeeth"
				case 5: if (iDice(1, 30) == 1) iItemID = 203; break;	// "HelhoundClaw"
				case 6: if (iDice(1, 50) == 1) iItemID = 204; break;	// "HelhoundTounge"
				case 7: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;// Rep Small ball
				case 8: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"
				case 9: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 10: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				default: break;
				}
				break;
			case 28://Troll
				switch (iDice(1, 12)) {
				case 1: if (iDice(1, 35) == 1) iItemID = 222; break;	// "TrollHeart"
				case 2: if (iDice(1, 23) == 1) iItemID = 223; break;	// "TrollMeat"
				case 3: if (iDice(1, 25) == 1) iItemID = 224; break;	// "TrollLeather"
				case 4: if (iDice(1, 27) == 1) iItemID = 225; break;	// "TrollClaw"
				case 5: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;	// Rep Small ball
				case 6: if (iDice(1, 100) == 5) iItemID = 334; break;	// "LuckyGoldRing"
				case 7: if (iDice(1, 100) == 5) iItemID = 337; break;	// "RubyRing"
				case 8: if (iDice(1, 100) == 5) iItemID = 336; break;	// "SaphireRing"
				case 9: if (iDice(1, 100) == 5) iItemID = 333; break;	// "PlatinumRing"
				case 10: if (iDice(1, 1000) == 50) iItemID = 650; break;	// "ZemstoneofSacrifice"
				case 11: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 12: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				default: break;
				}
				break;
			case 29://Ogre
				switch (iDice(1, 13)) {
				case 1: if (iDice(1, 20) == 1) iItemID = 209; break;		// "OgreHair"
				case 2: if (iDice(1, 22) == 1) iItemID = 210; break;		// "OgreHeart"
				case 3: if (iDice(1, 25) == 1) iItemID = 211; break;		// "OgreMeat"
				case 4: if (iDice(1, 25) == 1) iItemID = 212; break;		// "OgreLeather"
				case 5: if (iDice(1, 25) == 1) iItemID = 213; break;		// "OgreTeeth"
				case 6: if (iDice(1, 25) == 1) iItemID = 214; break;		// "OgreClaw"
				case 7: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 8: if (iDice(1, 100) == 15) iItemID = 300; break;		// "Neck(RM10)"
				case 9: if (iDice(1, 100) == 15) iItemID = 311; break;		// "Neck(DF10)"
				case 10: if (iDice(1, 500) == 20) iItemID = 632; break;		// "RingofOgrePower"
				case 11: if (iDice(1, 1000) == 60) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 12: if (iDice(1, 1000) == 60) iItemID = 656; break;		// "StoneOfXelima"
				case 13: if (iDice(1, 1000) == 60) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 30://Liche
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 1000) == 60) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 60) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 60) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 1500) == 50) iItemID = 259; break;		// "MagicWand(MSHIELD)"
				case 5: if (iDice(1, 500) == 3) iItemID = 636; break;		// "RingOfGrandMage"
				case 6:	if (iDice(1, 4000) == 3) iItemID = 734; break;		// "RingOfArcMage"
				case 7: if (iDice(1, 5000) == 82) iItemID = 648; break;		// "NecklaceOfLiche"
				case 8: if (iDice(1, 6000) == 250) iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
				case 9:	if (iDice(1, 8000) == 300) iItemID = 381; break;		// "MassFireStrikeManual"
				case 10: if (iDice(1, 10000) == 100) iItemID = 382; break;		// "BloodyShoWaveManual"
				default: break;
				}
				break;
			case 31://Demons
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 15) == 3) iItemID = 541; break;		// "DemonHeart"
				case 2:	if (iDice(1, 5) == 3) iItemID = 542; break;			// "DemonMeat"
				case 3:	if (iDice(1, 10) == 3) iItemID = 543; break;		// "DemonLeather"
				case 4:	if (iDice(1, 15) == 3) iItemID = 540; break;		// "DemonEye"
				case 5: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 6: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 7: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 8: if (iDice(1, 500) == 10) iItemID = 633; break;		// "RingofDemonpower"
				case 9: if (iDice(1, 5000) == 100) iItemID = 382; break;		// "BloodyShoWaveManual"
				default: break;
				}
				break;
			case 32: //Unicorn
				switch (iDice(1, 8)) {
				case 1: if (iDice(1, 40) == 3) iItemID = 544; break;		// "UnicornHeart"
				case 2: if (iDice(1, 25) == 3) iItemID = 545; break;		// "UnicornHorn"	
				case 3: if (iDice(1, 10) == 3) iItemID = 546; break;		// "UnicornMeat"	
				case 4: if (iDice(1, 15) == 3) iItemID = 547; break;		// "UnicornLeather"					
				case 5: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 6: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 7: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 8: if (iDice(1, 20000) == 2520) iItemID = 851; break;		// "KlonessEsterk"
				default: break;
				}
				break;
			case 33://WereWolf
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 30) == 3) iItemID = 551; break;		// "WerewolfTail"
				case 2: if (iDice(1, 25) == 3) iItemID = 548; break;		// "WerewolfHeart"
				case 3: if (iDice(1, 25) == 3) iItemID = 550; break;		// "WerewolfMeat"
				case 4: if (iDice(1, 35) == 3) iItemID = 553; break;		// "WerewolfLeather"
				case 5: if (iDice(1, 25) == 3) iItemID = 552; break;		// "WerewolfTeeth"
				case 6: if (iDice(1, 25) == 3) iItemID = 554; break;		// "WerewolfClaw"
				case 7: if (iDice(1, 35) == 3) iItemID = 549; break;		// "WerewolfNail"
				case 8: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 9: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 10: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				default: break;
				}
				break;
			case 48: // Stalker
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 10) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 20) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = REP_SMALL; break;	// Rep Small ball
				case 4: if (iDice(1, 1000) == 30) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 5: if (iDice(1, 1500) == 30) iItemID = 292; break;		// "GoldenAxeLLF"
				default: break;
				}
				break;
			case 52: // Gagoyle:
				switch (iDice(1, 11)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 500) == 10) iItemID = 630; break;		// "ringofxelima"
				case 5: if (iDice(1, 10000) == 1000) iItemID = 381; break;		// "FireStrikeManual"
				case 6: if (iDice(1, 10000) == 1000) iItemID = 382; break;		// "BloodyShockWaveManual"
				case 7: if (iDice(1, 5000) == 50) iItemID = 20; break;		// "Excalibur"
				case 8: if (iDice(1, 5000) == 100) iItemID = 610; break;		// "xelimablade"
				case 9: if (iDice(1, 90000) == 15800) iItemID = 860; break;		// "NecklaceOfXelima"
				case 10: if (iDice(1, 5000) == 400) iItemID = 612; break;		// "xelimarapier"
				case 11: if (iDice(1, 5000) == 150) iItemID = 611; break;		// "xelimaaxe"
				default: break;
				}
				break;
			case 54: // Dark-Elf
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 500) == 5) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 500) == 5) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 500) == 5) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 60000) == 25810) iItemID = 618; break;	// "DarkelfBow"
				default: break;
				}
				break;
			case 58: // Mountain Giant
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 10) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 20) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 500) == 50) iItemID = REP_SMALL; break;	// Rep Small ball
				case 4: if (iDice(1, 1000) == 30) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 5: if (iDice(1, 500) == 3) iItemID = 337; break;		// "RubyRing"
				default: break;
				}
				break;
			case 59: // Ettin
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = 650; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 500) == 30) iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, REP_SMALL); break;	//rare item
				default: break;
				}
				break;
			case 71: // Centaurus
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 650; break;		// "ZemstoneOfSacrifice"	
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"	
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 61: // Rudolph
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 650; break;		// "ZemstoneOfSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 57: // Giant-Frog
			case 62: //Direboard
			case 70: // Barlog
			case 75: // Giant-Lizard
			case 77: // MasterMage-Orc
			case 122: //Goblin
			case 123: //GmGoblin
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 650; break;		// "ZemstoneOfSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
				//Nuevos NPC 2Drop LaloRamos
			case 93:	//IceWorm
			case 94:	//FireWorm
			case 100:	//Snoob
			case 101:	//FireSnoob
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 250) iItemID = 650; break; //"ZemstoneofSacrifice"
				case 2: if (iDice(1, 1000) == 250) iItemID = MAJ_SMALL; break; // Majestics Small ball
				case 3: if (iDice(1, 1000) == 250) iItemID = CONTRIB_SMALL; break;//ContribBall Small Ball
				case 4: if (iDice(1, 1000) == 250) iItemID = REP_SMALL; break; // Rep Small ball
				case 5: if (iDice(1, 1000) == 250) iItemID = 942; break; //"RighStone"
				default: break;
				}
				break;
			case 97:	//Good-Golem
			case 98:	//Ice-Lizard
			case 99:	//Butcher
				switch (iDice(1, 6)) {
				case 1: if (iDice(1, 1000) == 350) iItemID = 941; break; //"PoisonStone"
				case 2: if (iDice(1, 1000) == 350) iItemID = 944; break; //"AgileStone"
				case 3: if (iDice(1, 1000) == 350) iItemID = 945; break; //"LightStone"
				case 4: if (iDice(1, 1000) == 350) iItemID = 946; break; //"SharpStone"
				case 5: if (iDice(1, 1000) == 350) iItemID = 948; break; //"AncStone"
				case 6: if (iDice(1, 1000) == 350) iItemID = 951; break; //"CritStone"
				default: break;
				}
				break;
			case 103:	//GiantSpider
			case 104:	//Diablo
			case 105:	//Phamton
			case 106:	//Yeti
				
				break;
			case 95:	//Blue Demon
			case 96:	//Red Demon
			case 102:	//Satan
			case 107:	//GrandEttin
				
				
				break;
			case 124: //Danny
				
				break;
			case 79:	// Nizie
			case 63:	// Frost
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 800) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 4: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				
				default: break;
				}
				break;
			case 53: // Beholder
				switch (iDice(1, 2)) {
				case 1: if (iDice(1, 1000) == 20) iItemID = REP_SMALL; break;		// Rep Small ball
				case 2: if (iDice(1, 1000) == 50) iItemID = REP_SMALL; break;		// Rep Small ball
					//	case 3: if (iDice(1, 100000) == 100) iItemID = 857; break;	// "I.M.CManual"
				default: break;
				}
				break;
			case 112://Newmobs
			case 113:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 942; break; //"RighStone"
				case 2: if (iDice(1, 1000) == 50) iItemID = 3733; break; //"ArmorDye(2) - Naranja"
				case 3: if (iDice(1, 1000) == 50) iItemID = 3734; break; //"ArmorDye(3) - Blanco"
				case 4: if (iDice(1, 1000) == 50) iItemID = 3737; break; //"ArmorDye(6) - Celeste"
				case 5: if (iDice(1, 1000) == 50) iItemID = 3738; break; //"ArmorDye(7) - Negro"
				case 6: if (iDice(1, 1000) == 50) iItemID = 3739; break; //"ArmorDye(8) - Amarillo"
				case 7: if (iDice(1, 1000) == 50) iItemID = 3740; break; //"ArmorDye(9) - Rosa"
				case 8: if (iDice(1, 1000) == 50) iItemID = 3741; break; //"ArmorDye(10) - Violeta"
				case 9: if (iDice(1, 1000) == 50) iItemID = 3742; break; //"ArmorDye(11) - Marron"
				default: break;
				}
				break;
			case 49: // Hellclaw
				if (Drop)
				{
					switch (iDice(1, 50)) {

					case 1: iItemID = 642; break;	// "KnecklaceOfIcePro"
					default:
						switch (iDice(1, 12))
						{
						case 1: iItemID = 259; break;	// "MagicWand(M.Shield)"
						case 2: iItemID = 311;	break;	// "MagicNecklace(DF+10)"
						case 3: iItemID = 308;	break;	// "MagicNecklace(MS10)"
						case 4: iItemID = 305;	break;	// "MagicNecklace(DM+1)"
						case 5: iItemID = 337;	break;	// "RubyRing"
						case 6: iItemID = 634; break;	// "RingofWizard"
						case 7: iItemID = 335; break;	// "EmeraldRing"
						case 8: iItemID = 3011;	break;	// "RepPotionRestore"
						case 9: iItemID = REP_SMALL; break;// Rep Small ball
						case 10: iItemID = 635; break;	// "RingOfMage" 
						case 11: iItemID = 636; break;	// "RingOfGrandMage" 
						case 12: iItemID = 642;	break;	// "KnecklaceOfIcePro"
						}
						break;
					}
				}
				break;
			case 50: // TigerWorm
				if (Drop)
				{
					switch (iDice(1, 50)) {
					case 1: iItemID = 610; break; // "XelimaBlade"
					case 2: iItemID = 612; break; // "XelimaRapier"							
					case 3: iItemID = 611; break; // "XelimaAxe"
					case 4: iItemID = 642; break; // "KnecklaceOfIcePro"
					case 5: iItemID = 631; break; // "RingoftheAbaddon"
					case 6: iItemID = 638; break; // "NeckofFireProtec"
					case 7: iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, CONTRIB_SMALL); break;	//ContribBall Small Ball o item muy raro en este caso hay 2 probabilidades, la del metodo getrareitem y esta
					case 11: iItemID = 857; break;// "I.M.CManual"
					default:
						switch (iDice(1, 11))
						{
						case 1: iItemID = 300;	break; // "MagicNecklace(RM10)"
						case 2: iItemID = 259;	break; // "MagicWand(M.Shield)"
						case 3: iItemID = 311;	break; // "MagicNecklace(DF+10)"
						case 4: iItemID = 308;	break; // "MagicNecklace(MS10)"
						case 5: iItemID = 305;	break; // "MagicNecklace(DM+1)"
						case 6: iItemID = 633; break; // "RingofDemonpower"
						case 7: iItemID = 630;	break; // "RingOfXelima"
						case 8: iItemID = 614;	break; // "SwordOfIceElemental"
						case 9: iItemID = 492; break; // "BloodRapier"
						case 10: iItemID = 20; break; // "Excaliber"
						case 11: iItemID = 491; break;// "BloodAxe"/ *
						//case 12: iItemID = 290; break;// "Flamberge+3LLF "* /
						}
						break;
					}
				}
				break;
			case 108: // HellDeath
				if (Drop)
				{
					switch (iDice(1, 10)) {
					case 1: iItemID = 4964; break;		//	"RingofPowerWar"
					case 2: iItemID = 4965; break;		//	"RingofPowerMage"
					case 3: iItemID = 4966; break;		//	"NeckPower(MS.30)"
					case 4: iItemID = 4968; break;		//	"NeckPowerXelima"
					case 5: iItemID = 4967; break;		//	"NeckPowerMerien"
					case 6: iItemID = CONTRIB_SMALL; break;		//	"Coins(+100)"
					default:
						switch (iDice(1, 6))
						{
						case 1: iItemID = 4964; break;		//	"RingofPowerWar"
						case 2: iItemID = 4965; break;		//	"RingofPowerMage"
						case 3: iItemID = 4966; break;		//	"NeckPower(MS.30)"
						case 4: iItemID = 4968; break;		//	"NeckPowerXelima"
						case 5: iItemID = 4967; break;		//	"NeckPowerMerien"
						case 6: iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, CONTRIB_SMALL); break;	//ContribBall Small Ball
						}
						break;
					}
				}
				break;
			case 109: // LightWorm
				if (Drop)
				{
					switch (iDice(1, 20)) {
						/ *	case 1: iItemID = 4905; break;		//	"BlackHammer"
						case 2: iItemID = 861; break;		//	"BerserkWand(MS.30)
						case 3: iItemID = 850; break;		//	"KlonessAxe"
						case 4: iItemID = 849; break;		//	"KlonessBlade"
						case 5: iItemID = 850; break;		//	"KlonessAxe"
						case 6: iItemID = 848; break;		//	"LightingBlade"* /
						//		case 7: iItemID = 616; break;		//	"DemonSlayer"
						//		case 8: iItemID = 863; break;		//  "KlonessWand(MS.30)"
					case 1: iItemID = 4964; break;		//	RingofPowerWar
					case 2: iItemID = 4965; break;		//	RingofPowerMage
					case 3: iItemID = 611; break;		// "XelimaAxe"
					case 4: iItemID = 643; break;		// "knecklaceOfIceEle
					case 5: iItemID = CONTRIB_SMALL; break;	//ContribBall Small Ball
					default:
						switch (iDice(1, 10))
						{
						case 1: iItemID = 4964; break;		//	RingofPowerWar
						case 2: iItemID = 4965; break;		//	RingofPowerMage
						case 3: iItemID = 4966; break;		//	NeckPower(MS.30)
						case 4: iItemID = 4967; break;		//	NeckPowerMerien
						case 5: iItemID = 4968; break;		//	NeckPowerXelima
						case 6: iItemID = CONTRIB_SMALL; break;		// "Coins(+500)"
						case 7: iItemID = 610; break;		// "XelimaBlade"
						case 8: iItemID = 611; break;		// "XelimaAxe"
						case 9: iItemID = 643; break;		// "knecklaceOfIceEle
						case 10: iItemID = CONTRIB_SMALL; break;	//ContribBall Small Ball
						}
						break;
					}
				}
				break;
			case 121: // GhostAba
				if (Drop)
				{
					switch (iDice(1, 20)) {
					case 1: iItemID = 4964; break;		//	RingofPowerWar
					case 2: iItemID = 4965; break;		//	RingofPowerMage	
					case 3: iItemID = 4967; break;		//	NeckPowerMerien
					case 4: iItemID = 4968; break;		//	NeckPowerXelima
					case 5: iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
					case 6: iItemID = 847; break;		// "DarkExecutor"
					case 7: iItemID = 860; break;		// "NecklaceOfXelima"
					case 8: iItemID = 858; break;		// "NecklaceOfMerien"
					default:
						switch (iDice(1, 8))
						{
						case 1: iItemID = 4964; break;		//	RingofPowerWar
						case 2: iItemID = 4965; break;		//	RingofPowerMage	
						case 3: iItemID = 4967; break;		//	NeckPowerMerien
						case 4: iItemID = 4968; break;		//	NeckPowerXelima
						case 5: iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
						case 6: iItemID = 847; break;		// "DarkExecutor"
						case 7: iItemID = 860; break;		// "NecklaceOfXelima"
						case 8: iItemID = 858; break;		// "NecklaceOfMerien"
						}
						break;
					}
				}
				break;

			case 66: // Wyvern 
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						3, 6, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			case 73: // Fire-Wyvern
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						3, 6, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			case 81: //Abaddon
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						6, 12, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 65,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			/ *case 128: // BlackWyvern
			case 114: // DeathWyvern 
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						3, 9, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			case 120: // HellAbaddon 
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						6, 12, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 65,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;* /
			}

			dwCount = 1;

			if (iItemID == 0) {
				switch (m_pNpcList[iNpcH]->m_sType) {
				case 34:
					iItemID = 90;
					dwCount = (DWORD)(iDice(1, m_pNpcList[iNpcH]->m_iExp * 4) + m_pNpcList[iNpcH]->m_iExp);
					dwCount = dwCount - (dwCount / 3);
					break;
				}
			}

			if (iNumItem > 0) {
				GetLocalTime(&SysTime);
				wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
				for (int j = 0; j < iNumItem; j++) {
					if (pItem == NULL) pItem = new class CItem;


					//pItem = new class CItem;
					if (iItemIDs[j] == ICESEGMENTS)
					{
						iItemIDs[j] = 2795;
					}
					else if (iItemIDs[j] == FIRESEGMENTS)
					{
						iItemIDs[j] = 2797;
					}
					else if (iItemIDs[j] == LIGHTNINGSEGMENTS)
					{
						iItemIDs[j] = 2796;
					}
					
					if (_bInitItemAttr(pItem, iItemIDs[j]) != FALSE &&
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bGetIsMoveAllowedTile((short)ItemPositions[j].x, (short)ItemPositions[j].y) != FALSE)
					{					
						if (iItemIDs[j] == 2795) //icesegments
						{
							pItem->m_dwCount = RollDice(2000, 4000);
						}
						else if (iItemIDs[j] == 2796) //LightningSegments
						{
							pItem->m_dwCount = RollDice(2000, 4000);
						}
						else if (iItemIDs[j] == 2797) //FireSegments
						{
							pItem->m_dwCount = RollDice(2000, 4000);
						}
						else
						{
							if (iItemIDs[j] == 90) pItem->m_dwCount = iDice(5000, 20000);
							else pItem->m_dwCount = dwCount;
						}							

						pItem->m_sTouchEffectType = DEF_ITET_ID;
						pItem->m_sTouchEffectValue1 = iDice(1, 100000);

						// Anti Dup System
						SYSTEMTIME SysTime;
						char cTemp[256];
						GetLocalTime(&SysTime);
						ZeroMemory(cTemp, sizeof(cTemp));
						pItem->m_sTouchEffectValue2 = iDice(1, 100000);
						wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
						pItem->m_sTouchEffectValue2 = atoi(cTemp);

						ZeroMemory(cTemp, sizeof(cTemp));
						wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
						pItem->m_sTouchEffectValue3 = atoi(cTemp);

						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem((short)ItemPositions[j].x, (short)ItemPositions[j].y, pItem);
						pItem->m_dwTimeDrop = timeGetTime();
						SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
							(short)ItemPositions[j].x, (short)ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
						_bItemLog(DEF_ITEMLOG_NEWGENDROP, NULL, m_pNpcList[iNpcH]->m_cNpcName, pItem);
						pItem = NULL;

					}
					else {
						delete pItem;
						pItem = NULL;

					}
				}
			}
			else {

				//pItem = new class CItem;
				if (iItemID == CONTRIB_SMALL)
				{
					iItemID = 3052;
				}
				else if (iItemID == MAJ_SMALL)
				{
					iItemID = 3055;					
				}
				else if (iItemID == REP_SMALL)
				{
					iItemID = 3054;					
				}
				
				if (_bInitItemAttr(pItem, iItemID) != FALSE) {

					if (iItemID == 3052) //contrib
					{
						pItem->m_dwCount = RollDice(500, 750);
					}
					else if (iItemID == 3055) //maj
					{
						pItem->m_dwCount = RollDice(5, 20);
					}
					else if (iItemID == 3054) //rep
					{
						pItem->m_dwCount = RollDice(1, 3);
					}
					else
					{
						pItem->m_dwCount = dwCount;
					}					

					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 10000);

					// Anti Dup System
					SYSTEMTIME SysTime;
					char cTemp[256];
					GetLocalTime(&SysTime);
					ZeroMemory(cTemp, sizeof(cTemp));
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
					pItem->m_sTouchEffectValue2 = atoi(cTemp);

					ZeroMemory(cTemp, sizeof(cTemp));
					wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
					pItem->m_sTouchEffectValue3 = atoi(cTemp);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem);
					pItem->m_dwTimeDrop = timeGetTime();

					SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
						m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY,
						pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

					_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
				}
				else {
					delete pItem;
					pItem = NULL;
				}
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: DeleteNpc");
	}
#endif
	if (string(m_pNpcList[iNpcH]->m_cMapName) == "maze")
	{		
		mobs_maze--;
		SendMazeMobCount();
		if (mobs_maze == 0)
		{
			ChangeRoaEventState();
		}
	}

	if (string(m_pNpcList[iNpcH]->m_cMapName) == "abymap")
	{
		if (m_pNpcList[iNpcH]->m_sType == 81)
		{
			handleEventWhenAbaddonDeath();
		}
	}

	delete m_pNpcList[iNpcH];
	m_pNpcList[iNpcH] = NULL;
}

BOOL cCoreServer::bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
	int iItemSpreadType, int iSpreadRange, int *iItemIDs, POINT *BasePos, int *iNumItem)
{
#ifdef DEF_DEBUG
	try {
#endif
		int		iProb = 100;
		float	fProb, fProbA, fProbB, fProbC;
		int		iItemID;
		int		iNum = 0;

		for (int i = 0; i < iMax; i++) {
			if (i > iMin) iProb = iProbability;

			fProb = (float)((100 - iProb) / 10.0);
			if (fProb < 1.0) fProb = 1.0;

			fProbA = fProbB = fProbC = fProb;

			fProbA = fProbA * 8.0f;
			fProbB = fProbB * 4.0f;
			fProbC = fProbC;

			iItemID = 0;

			switch (sNpcType) {
			case 66: // Wyvern

				switch (iDice(1, 2500)) {
				case 500: iItemID = 3172; break;   //HP_Neck 

				default:

					switch (iDice(1, 1500)) {
					case 1: iItemID = 2780; break;    //RedDevastator
					case 2: iItemID = 2783; break;    //RedZerkWand
					case 3: iItemID = 2780; break;    //RedDevastator
					case 4: iItemID = 845; break;    //Sb			

					case 5: iItemID = 4986; break;    //HSWManual				
					case 6: iItemID = 2794; break;    //FOTManual					
					case 7: iItemID = 3406; break;    //MBManual							
					case 8: iItemID = 2790; break; //BlazingSword
					case 9: iItemID = 2791; break; // HammerOfThor
					case 10: iItemID = 2792; break; // RapierofEmbers

					case 11: iItemID = 2793; break; // LightningSword
					case 12: iItemID = 2798; break; // AsteroidHammer
					case 13: iItemID = 2799; break; // ThunderRapier

					case 14: iItemID = 3400; break; // IceExecutor
					case 15: iItemID = 3401; break; // FrozenHammer
					case 16: iItemID = 3402; break; // EsterkOfFrost

					case 17: iItemID = 3403; break; // IceWizardStaff
					case 18: iItemID = 3404; break; // LightningWizStaff
					case 19: iItemID = 3405; break; // FireWizardStaff

					case 20: iItemID = 2786; break; // StripHammer

						

						default:
						// Primero chequeamos los 3 items nuevos ultra-raros (1 en 1000)
						switch (iDice(1, 1000)) {
						case 1: iItemID = 3271; break;   // WizardRapier (nuevo, ultra-raro 0.1%)
						case 2: iItemID = 1748; break;    // RedDragonEgg (nuevo, ultra-raro 0.1%)
						case 3: iItemID = 1749; break;    // BlueDragonEgg (nuevo, ultra-raro 0.1%)
						case 4: iItemID = 1750; break;    // SkyDragonEgg (nuevo, ultra-raro 0.1%)

						default:
							// Luego los drops normales (probabilidades originales)
							switch (iDice(1, 200)) {
							case 1: iItemID = 382; break;    // BlueFireManual (0.5%)
							case 2: iItemID = 614; break;    // SwordofIceElemental (0.5%)
							case 3: iItemID = 638; break;    // KnecklaceOfIcePro (0.5%)
							case 4: iItemID = 381; break;    // IceStormManual (0.5%)
							case 5: iItemID = 337; break;    // RubyRing (0.5%)
							case 6: iItemID = 335; break;    // EmeraldRing (0.5%)
							case 7: iItemID = 334; break;    // LuckyGoldRing (0.5%)

							case 8: iItemID = 3089; break;    // Tinta Azul
							case 9: iItemID = 3090; break;    // Tinta Roja
							case 10: iItemID = 3746; break;   // Tinta Verde

							case 11: iItemID = 3471; break;   // Tinta Gold ArmorDye
							case 12: iItemID = 3472; break;   // Tinta Gold Dye

							case 13: iItemID = 3473; break;   // Tinta Turq
							case 14: iItemID = 3474; break;   // Tinta Turq Dye

							case 15: iItemID = 3475; break;   // Tinta Pink ArmorDye
							case 16: iItemID = 3476; break;   // Tinta Pink Dye
								
							case 17: iItemID = 3477; break;   // Tinta Violet ArmorDye
							case 18: iItemID = 3478; break;   // Tinta Violet Dye

							case 19:
							case 20:
								iItemID = FIRESEGMENTS;
								break;

							case 21:
							case 22:
								iItemID = ICESEGMENTS;
								break;

							case 23:
							case 24:
								iItemID = LIGHTNINGSEGMENTS;
								break;

							default:
								switch (iDice(1, 10)) {
								case 1: iItemID = 273; break;    // InviPotion (10%)
								case 2: iItemID = 840; break;    // PowerRedPotion (10%)
								case 3: iItemID = 841; break;    // PowerBluePotion (10%)
								case 4: iItemID = 300; break;    // MagicNecklace(RM10) (10%)
								case 5: iItemID = 311; break;    // MagicNecklace(DF+10) (10%)
								case 6: iItemID = 308; break;    // MagicNecklace(MS10) (10%)
								case 7: iItemID = 305; break;    // MagicNecklace(DM+1) (10%)
								case 8: iItemID = 259; break;    // MagicWand(M.Shield) (10%)
								case 9: iItemID = 636; break;    // RingofGrandMage (10%)
								case 10: iItemID = 492; break;   // BloodRapier (10%)
								}
								break;
							}
							break;
						}
						break;
					}
					break;
				}
				break;

			case 73: // Fire-Wyvern

				switch (iDice(1, 2500)) {
				case 500: iItemID = 3172; break;   //HP_Neck 

				default:

					switch (iDice(1, 1500)) {
					case 1: iItemID = 2780; break;    //RedDevastator
					case 2: iItemID = 2783; break;    //RedZerkWand
					case 3: iItemID = 613; break;    //SwordofMedusa
					case 4: iItemID = 845; break;    //Sb		
					case 5: iItemID = 4986; break;    //HSWManual				
					case 6: iItemID = 2794; break;    //FOTManual		
					case 7: iItemID = 3406; break;    //MBManual							
					case 8: iItemID = 2790; break; //BlazingSword
					case 9: iItemID = 2791; break; // HammerOfThor
					case 10: iItemID = 2792; break; // RapierofEmbers

					case 11: iItemID = 2793; break; // LightningSword
					case 12: iItemID = 2798; break; // AsteroidHammer
					case 13: iItemID = 2799; break; // ThunderRapier

					case 14: iItemID = 3400; break; // IceExecutor
					case 15: iItemID = 3401; break; // FrozenHammer
					case 16: iItemID = 3402; break; // EsterkOfFrost

					case 17: iItemID = 3403; break; // IceWizardStaff
					case 18: iItemID = 3404; break; // LightningWizStaff
					case 19: iItemID = 3405; break; // FireWizardStaff

					case 20: iItemID = 2786; break; // StripHammer

					default:
						// Primero chequeamos los 3 items nuevos ultra-raros (1 en 1000)
						switch (iDice(1, 1000)) {
						case 1: iItemID = 3271; break;   // WizardRapier (nuevo, ultra-raro 0.1%)
						case 2: iItemID = 1748; break;    // RedDragonEgg (nuevo, ultra-raro 0.1%)
						case 3: iItemID = 1749; break;    // BlueDragonEgg (nuevo, ultra-raro 0.1%)
						case 4: iItemID = 1750; break;    // SkyDragonEgg (nuevo, ultra-raro 0.1%)
						default:

							// Luego los drops normales (probabilidades originales)
							switch (iDice(1, 200)) {
							case 1: iItemID = 382; break;    // HealSpotManual (0.5%)
							case 2: iItemID = 381; break;    // IceStormManual (0.5%)
							case 3: iItemID = 273; break;    // InviPotion (0.5%)
							case 4: iItemID = 300; break;    // MagicNecklace(RM10) (0.5%)
							case 5: iItemID = 20; break;     // Excaliber (0.5%)
							case 6: iItemID = 381; break;    // MassFireStrikeManual (0.5%)
							case 7: iItemID = 382; break;    // BloodyShoWaveManual (0.5%)
							case 8: iItemID = 311; break;    // MagicNecklace(DF+10) (0.5%)
							case 9: iItemID = 337; break;    // RubyRing (0.5%)
							case 10: iItemID = 335; break;   // EmeraldRing (0.5%)
							case 11: iItemID = 334; break;   // LuckyGoldRing (0.5%)

							case 12: iItemID = 3089; break;    // Tinta Azul
							case 13: iItemID = 3090; break;    // Tinta Roja
							case 14: iItemID = 3746; break;   // Tinta Verde

							case 15: iItemID = 3471; break;   // Tinta Gold ArmorDye
							case 16: iItemID = 3472; break;   // Tinta Gold Dye

							case 17: iItemID = 3473; break;   // Tinta Turq
							case 18: iItemID = 3474; break;   // Tinta Turq Dye

							case 19: iItemID = 3475; break;   // Tinta Pink ArmorDye
							case 20: iItemID = 3476; break;   // Tinta Pink Dye

							case 21: iItemID = 3477; break;   // Tinta Violet ArmorDye
							case 22: iItemID = 3478; break;   // Tinta Violet Dye

							case 23:
							case 24:
								iItemID = FIRESEGMENTS;
								break;

							case 25:
							case 26:
								iItemID = ICESEGMENTS;
								break;

							case 27:
							case 28:
								iItemID = LIGHTNINGSEGMENTS;
								break;

							default:
								switch (iDice(1, 10)) {
								case 1: iItemID = 492; break;    // BloodRapier (10%)
								case 2: iItemID = 490; break;    // BloodSword (10%)
								case 3: iItemID = 491; break;    // BloodAxe (10%)
								case 4: iItemID = 308; break;    // MagicNecklace(MS10) (10%)
								case 5: iItemID = 305; break;    // MagicNecklace(DM+1) (10%)
								case 6: iItemID = 636; break;    // RingofGrandMage (10%)
								case 7: iItemID = 735; break;    // RingofDragonpower (10%)
								case 8: iItemID = 638; break;    // KnecklaceOfFirePro (10%)
								case 9: iItemID = 645; break;    // KnecklaceOfEfreet (10%)
								case 10: iItemID = 259; break;   // MagicWand(M.Shield) (10%)
								}
								break;
							}
							break;
						}
						break;
					}
					break;
				}
				break;
		
			case 81: // Abaddon
				switch (DropRollDice(1, 25)) {
				case 1: iItemID = 3450; break;	//IceBladeOfAby
				case 2: iItemID = 3451; break;	//FireBladeOfAby
				case 3: iItemID = 3452; break;	// LightBladeOfAby
				case 4: iItemID = 3453; break;	// IceStaffOfAby
				case 5: iItemID = 3454; break;	// FireStaffOfAby
				case 6: iItemID = 3455; break;	// LightStaffOfAby
				case 7: iItemID = 3456; break;	// IceHammerOfAby
				case 8: iItemID = 3457; break;	// FireHammerOfAby
				case 9: iItemID = 3458; break;	// LightHammerOfAby

				case 10: iItemID = 2782; break;	// GoldDevastator
				case 11: iItemID = 2785; break;	// GoldZerkWand
				case 12: iItemID = 3172; break;	// HPNECK

				
				default:
					switch (DropRollDice(1, 18)) {

					case 1: iItemID = 2781; break;	// darkdevas
					case 2: iItemID = 2784; break;	// dakzw

					case 3: iItemID = 2786; break;// StripHammer
					case 4: iItemID = 2790; break;// BlazingSword
					case 5: iItemID = 2791; break;// HammerOfThor
					case 6: iItemID = 2792; break;// RapierofEmbers
					case 7: iItemID = 2793; break;// LightningSword
					case 8: iItemID = 2798; break;// AsteroidHammer 
					case 9: iItemID = 2799; break;// ThunderRapier 
					case 10: iItemID = 3400; break;// IceExecutor
					case 11: iItemID = 3401; break;// FrozenHammer 
					case 12: iItemID = 3402; break;// EsterkOfFrost 
					case 13: iItemID = 3403; break;// IceWizardStaff 
					case 14: iItemID = 3404; break;// LightningWizStaff 
					case 15: iItemID = 3405; break;// FireWizardStaff 

					case 16: iItemID = 3406; break;// MBManual 
					case 17: iItemID = 4986; break;// HSWManual 
					case 18: iItemID = 2794; break;// FOTManual		
					}
					break;
				}
				break;
			}
			// Gold 
			if (iItemID == 0) iItemID = 90; // Gold
			if (iItemID != 0) {
				// item id
				iItemIDs[iNum] = iItemID;

				// item position
				switch (iItemSpreadType) {
				case DEF_ITEMSPREAD_RANDOM:
					BasePos[iNum].x = sBaseX + iSpreadRange - DropRollDice(1, iSpreadRange * 2);
					BasePos[iNum].y = sBaseY + iSpreadRange - DropRollDice(1, iSpreadRange * 2);
					break;

				case DEF_ITEMSPREAD_FIXED:
					BasePos[iNum].x = sBaseX + ITEMSPREAD_FIEXD_COORD[iNum][0];
					BasePos[iNum].y = sBaseY + ITEMSPREAD_FIEXD_COORD[iNum][1];
					break;
				}
				iNum++;
			}
		} // for

		*iNumItem = iNum;
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: bGetMultipleItemNamesWhenDeleteNpc");
	}
#endif
	return TRUE;
}

int cCoreServer::getRareItem(short npctype, short lastType)
{
	if (!m_bRareDrops) return lastType;

	switch (npctype)
	{
	case 59: //ettin
	{
		if (g_ev.Is(EventID::DropBooster))
		{
			int iDado = RollDice(1, 50000);
			if (iDado == 25000)
			{
				PutLogList("GiantBattleHammer Dropeado.");
				return 762; //GiantBattleHammer
			}
			else return lastType;
		}
		else
		{
			int iDado = RollDice(1, 100000);
			if (iDado == 50000)
			{
				PutLogList("GiantBattleHammer Dropeado.");
				return 762; //GiantBattleHammer
			}
			else return lastType;
		}		

		break;
	}

	case 50: //Tiger worm
	{
		int iDado = RollDice(1, 80000);
		if (iDado == 40000)
		{
			PutLogList("MerienShield Dropeado.");
			return lastType; //MerienShield 620
		}
		else return lastType;

		break;
	}

	case 108://HellDeath
	{
		int iDado = RollDice(1, 2);
		switch (iDado)
		{
		case 1:
		{
			int iDado2 = RollDice(1, 120000);
			if (iDado2 == 60000)
			{
				PutLogList("Medu Neck Dropeado.");
				return lastType; //NecklaceOfMedusa 641
			}
			else return lastType;
			break;
		}
		case 2:
		{
			int iDado2 = RollDice(1, 120000);
			if (iDado2 == 60000)
			{
				PutLogList("Medu Sword Dropeada.");
				return lastType; //SwordOfMedusa 613
			}
			else return lastType;
			break;
		}
		default:
			break;
		}
		break;
	}
	
	default:
		return lastType;
		break;
	}

	return lastType;
}

int cCoreServer::DropRollDice(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

bool cCoreServer::isBossNpc(char * npcname)
{
	return strcmp(npcname, "Hellclaw") == 0 ||
		strcmp(npcname, "Tigerworm") == 0;		
}

void cCoreServer::handleDropRareItem(int client, char* npcname) {
	auto p = m_pClientList[client];
	if (!p) return;

	if (isBossNpc(npcname))
	{
		rollBossDrop(client, npcname);
	}
	else if (g_ev.Is(EventID::DropMadness))
	{
		rollDropInDropMadness(client, npcname);
	}
	else if (g_ev.Is(EventID::DropBooster))
	{
		rollDropInBooster(client, npcname);
	}
	else
	{
		rollNormalDrop(client, npcname);
	}
}

void cCoreServer::rollKnightSetDrop(int client, char * npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	dado = DropRollDice(1, 20000);
	
	if (dado <= 12) {
		DropRareItem(client, npcname, drop_legendary);
	}
	else if (dado <= 35) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 120) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 340) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 650) {
		DropRareItem(client, npcname, drop_common);
	}	
}

void cCoreServer::rollBossDrop(int client, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{
		dado = DropRollDice(1, 1500);
	}
	else
	{
		dado = DropRollDice(1, 1500);
	}

	if (dado <= 500) {
		DropRareItem(client, npcname, drop_legendary);
	}	
}


void cCoreServer::CreateCommonDropItem(int client, char* itemname, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
		ShowClientMsg(client, "You don't have enough space in your drop inventory.");
		return;
	}

	char cmsg[200];
	const char* dropType = getDropTypeString(drop_lesscommon);
	wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, 1, itemname);

	SendDropLogMsg(client, cmsg, drop_lesscommon);
	PutLogList(cmsg);

	add_rare_drop(client, (char*)itemname, 1, 0);	
}

std::string cCoreServer::getCommonItem()
{
	std::vector<std::string> items = {
		"SapphireRing",
		"PlatinumRing",
		"LuckyGoldRing",
		"RubyRing",
		"EmeraldRing",
		"RingofWizard",
		"RingofMage"
	};

	// Crear un generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, items.size() - 1);

	// Devolver un elemento aleatorio
	return items[dis(gen)];
}

void cCoreServer::rollDropInBooster(int client, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;

	if (p->m_bLuck)
	{
		dado = DropRollDice(1, DropRateBooster - 15000);
	}
	else
	{
		dado = DropRollDice(1, DropRateBooster);
	}

	if (dado <= 7) {
		int dado2 = DropRollDice(1, 2);
		if (dado2 == 1)
		{
			DropRareItem(client, npcname, drop_legendary);
		}
	}
	else if (dado <= 30) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 120) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 380) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 1000) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 400);
		if (dadito == 200)
		{
			if (string(npcname) != "Dummy" &&  string(npcname) != "Attack-Dummy")
			CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}

void cCoreServer::rollDropInDropMadness(int client, char * npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{	
		dado = DropRollDice(1, DropRateMadness - 15000);			
	}
	else
	{		
		dado = DropRollDice(1, DropRateMadness);		
	}

	if (dado <= 8) {
		DropRareItem(client, npcname, drop_legendary);		
	}
	else if (dado <= 30) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 120) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 380) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 1000) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 160);
		if (dadito == 80)
		{
			CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}

void cCoreServer::rollNormalDrop(int client, char * npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{		
		dado = DropRollDice(1, DropRateNormal - 15000);
	}
	else
	{		
		dado = DropRollDice(1, DropRateNormal);		
	}

	if (dado <= 7) {
		int dado2 = DropRollDice(1, 2);
		if (dado2 == 1)
		{
			DropRareItem(client, npcname, drop_legendary);
		}
	}
	else if (dado <= 18) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 70) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 300) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 800) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 160);
		if (dadito == 80)
		{
			CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}
void cCoreServer::DropRareItem(int client, char* npcname, short dropmode) {
	auto p = m_pClientList[client];
	if (!p) return;

	NPCDropInfo npcInfo = getNPCDropInfo(npcname);
	if (npcInfo.npcName == nullptr) return;

	int multiplier = getMultiplierForDropMode(dropmode);

	if (dropmode == drop_legendary) {
		handleLegendaryDrop(client, npcname, npcInfo);
		return;
	}

	int diceRange = 0;

	// Solo se consideran ContribBall, MajBall y fragmentos (materiales eliminados)
	if (npcInfo.baseContrib > 0) diceRange++;     // ContribBall
	if (npcInfo.baseMajs > 0) diceRange++;        // MajBall
	if (npcInfo.baseFragments > 0) diceRange += 11; // 11 tipos de fragmentos

	if (diceRange <= 0) return; // Nada para dropear

	int dado = DropRollDice(1, diceRange);
	DropItem dropItem = getDropItem(dado, npcInfo, multiplier, dropmode);

	if (dropItem.count > 0)
		CreateDropItem(client, dropItem.name, dropItem.count, dropItem.attribute, dropmode, npcname);
}

NPCDropInfo cCoreServer::getNPCDropInfo(const char* npcname) {
	if (strcmp(npcname, "Orc") == 0) {
		return{ "Orc", 10, 3, 0, {
			{ "ContribBall", 8000, 0 },
			{ "MajBall", 3000, 0 },
			{ "KnecklaceOfFirePro", 1, 0 }
		
		} };
	}
	else if (strcmp(npcname, "Skeleton") == 0) {
		return{ "Skeleton", 10, 3, 0, {
			{ "ContribBall", 8000, 0 },
			{ "MajBall", 3000, 0 }
		} };
	}
	else if (strcmp(npcname, "Scorpion") == 0) {
		return{ "Scorpion", 10, 3, 0, {
			{ "ContribBall", 8000, 0 },
			{ "MajBall", 3000, 0 }
		} };
	}
	else if (strcmp(npcname, "Stone-Golem") == 0) {
		return{ "Stone-Golem", 40, 5, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "NecklaceOfStoneGol", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Clay-Golem") == 0) {
		return{ "Clay-Golem", 40, 5, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "NecklaceOfStoneGol", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Zombie") == 0) {
		return{ "Zombie", 80, 10, 3, {
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "ContribBall", 8000, 0 },
			{ "MajBall", 3000, 0 }
		} };
	}
	else if (strcmp(npcname, "Giant-Frog") == 0) {
		return{ "Giant-Frog", 80, 10, 3, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}
	else if (strcmp(npcname, "Giant-Ant") == 0) {
		return{ "Giant-Ant", 80, 10, 2, {
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "ContribBall", 10000, 0 },
			{ "MajBall", 4000, 0 }
		} };
	}
	else if (strcmp(npcname, "Unicorn") == 0) {
		return{ "Unicorn", 80, 10, 2, {
			{ "KnecklaceOfLightPro", 1, 0 },
			{ "MerienPlateMailM", 1, 0 },
			{ "MerienPlateMailW", 1, 0 },
			{ "MerienChainMailM", 1, 0 },
			{ "MerienChainMailW", 1, 0 },
			{ "KnecklaceOfAirEle", 1, 0 }
		} };
	}
	
	/ *

	{ 3, "city", "CursedCity", "MasterHeroHelm(M)", 75 },
	{ 4, "city", "CursedCity", "MasterHeroHelm(W)", 75 },
	{ 5, "city", "CursedCity", "MasterHeroCap(M)", 75 },
	{ 6, "city", "CursedCity", "MasterHeroCap(W)", 75 },
	{ 7, "city", "CursedCity", "MasterHeroArmor(M)", 75 },
	{ 8, "city", "CursedCity", "MasterHeroArmor(W)", 75 },
	{ 9, "city", "CursedCity", "MasterHeroRobe(M)", 75 },
	{ 10, "city", "CursedCity", "MasterHeroRobe(W)", 75 },
	{ 11, "city", "CursedCity", "MasterHeroHauberk(M)", 75 },
	{ 12, "city", "CursedCity", "MasterHeroHauberk(W)", 75 },
	{ 13, "city", "CursedCity", "MasterHeroLeggs(M)", 75 },
	{ 14, "city", "CursedCity", "MasterHeroLeggs(W)", 75 },

	* /
	///aca
	else if (strcmp(npcname, "Troll") == 0) {
		return{ "Troll", 20, 4, 1, {
			{ "MasterHeroHelm(M)", 1, 0 },
			{ "MasterHeroCap(M)", 1, 0 },
			{ "MasterHeroArmor(M)", 1, 0 },
			{ "MasterHeroRobe(M)", 1, 0 },
			{ "MasterHeroHauberk(M)", 1, 0 },
			{ "MasterHeroLeggs(M)", 1, 0 },
			{ "Tokens", 15, 0 }
		} };
	}

	else if (strcmp(npcname, "Hellbound") == 0) {
		return{ "Hellbound", 20, 4, 1, {
			{ "MasterHeroHelm(W)", 1, 0 },
			{ "MasterHeroCap(W)", 1, 0 },
			{ "MasterHeroArmor(W)", 1, 0 },
			{ "MasterHeroRobe(W)", 1, 0 },
			{ "MasterHeroHauberk(W)", 1, 0 },
			{ "MasterHeroLeggs(W)", 1, 0 },
			{ "Tokens", 15, 0 }
		} };
	}
	
	else if (strcmp(npcname, "Ettin") == 0) {
		return{ "Troll", 50, 8, 5, {
			{ "RingofDemonpower", 1, 0 },
			{ "BloodSword", 1, 0 },
			{ "BlooddAxe", 1, 0 },			
			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },			
			{ "KlonessHammer", 1, 0 },
			{ "RingofMasterwar", 1, 0 },
			{ "NeckofMasterwar", 1, 0 },
			{ "CursedPiece(3)", 1, 0 },
			{ "CursedPiece(6)", 1, 0 },
			{ "CursedPiece(8)", 1, 0 },
			{ "CursedStone(2)", 1, 0 },
			{ "CursedStone(5)", 1, 0 },

			{ "HammerOfThor", 1, 0 },
			{ "AsteroidHammer", 1, 0 },
			{ "FrozenHammer", 1, 0 },

			{ "HPRecFragments", 30000, 0 },
			{ "LightShards", 30000, 0 },
			{ "StrongShards", 30000, 0 },

			{ "KnecklaceOfEfreet", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Cyclops") == 0) {
		return{ "Cyclops", 50, 8, 5, {
			{ "KnecklaceOfFirePro", 1, 0 },
			{ "KnecklaceOfLightPro", 1, 0 },
			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },
			{ "CursedPiece(7)", 1, 0 },
			{ "CursedPiece(9)", 1, 0 },
			{ "CursedStone(1)", 1, 0 },
			{ "CursedStone(2)", 1, 0 },
			{ "CursedStone(3)", 1, 0 },
			{ "CursedStone(4)", 1, 0 },
			{ "CursedStone(5)", 1, 0 },			
			{ "HunterWand(MS.50)", 1, 0 },			
			{ "RingofMastermage", 1, 0 },
			
			{ "FOTManual", 1, 0 },
			{ "ThunderRapier", 1, 0 },
			{ "LightningSword", 1, 0 },
			{ "LightningWizStaff", 1, 0 },
			{ "LightningSegments", 1000, 0 },

			{ "HPRecFragments", 30000, 0 },
			{ "LightShards", 30000, 0 },
			{ "StrongShards", 30000, 0 },

			{ "RingofMastermage", 1, 0 }

		} };
	}
	else if (strcmp(npcname, "Orge") == 0) {
		return{ "Orge", 50, 8, 5, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },

			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			
			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			
			{ "Tokens", 5, 0 }
		} };
	}

	
	else if (strcmp(npcname, "Demon") == 0) {
		return{ "Demon", 80, 10, 5, {
			{ "ContribBall", 20000, 0 },
			{ "MajBall", 6000, 0 },
			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			{ "Tokens", 5, 0 },

			{ "MagicNecklace(RM20)", 1, 0 },
			{ "MagicNecklace(RM25)", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "BloodyShoWaveManual", 1, 0 },
			{ "MerienChainMailM", 1, 0 },
			{ "MerienChainMailW", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Liche") == 0) {
		return{ "Liche", 80, 10, 5, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },

			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },

			{ "Tokens", 5, 0 },
			{ "MassFireStrikeManual", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 },
		} };
	}
	else if (strcmp(npcname, "Rudolph") == 0) {
		return{ "Rudolph", 80, 10, 5, {			
			{ "MetalArmor(M)", 1, 0 },
			{ "MetalChain(M)", 1, 0 },
			{ "MetalHauberk(M)", 1, 0 },
			{ "MetalCap(M)", 1, 0 },
			{ "MetalLeggings(M)", 1, 0 },
			{ "MetalHelm(M)", 1, 0 },
			{ "eShadowKnightCape", 1, 0 },
			{ "aShadowKnightCape", 1, 0 }

		} };			
	}
	else if (strcmp(npcname, "Ice-Golem") == 0) {
		return{ "Ice-Golem", 80, 10, 4, {				
			{ "MetalArmor(W)", 1, 0 },
			{ "MetalChain(W)", 1, 0 },
			{ "MetalHauberk(W)", 1, 0 },
			{ "MetalCap(W)", 1, 0 },
			{ "MetalLeggings(W)", 1, 0 },
			{ "MetalHelm(W)", 1, 0 },
			{ "eShadowKnightCape", 1, 0 },
			{ "aShadowKnightCape", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Gagoyle") == 0) {
		return{ "Gagoyle", 80, 10, 5, {
			{ "BloodyShockWaveManual", 1, 0 },
			{ "Excalibur", 1, 0 },
			{ "XelimaRapier", 1, 0 },
			{ "PandoraBox(Big)", 1, 0 },

			{ "MagicNecklace(RM20)", 1, 0 },
			{ "MagicNecklace(RM25)", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },
			{ "RingofPowerwar", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },

			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			{ "XelimaBlade", 1, 0 },
			{ "Tokens", 5, 0 },
			{ "XelimaAxe", 1, 0 }
		} };
	}	
	else if (strcmp(npcname, "Dark-Elf") == 0) {
		return{ "Dark-Elf", 80, 10, 5, {
			{ "Tokens", 5, 0 },
			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			
			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			
			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },
			{ "RingofPowerwar", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },

			{ "DarkElfBow", 1, 0 },

			{ "PandoraBox(Small)", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}
	else if (strcmp(npcname, "DireBoar") == 0) {
		return{ "DireBoar", 80, 10, 5, {
			{ "ContribBall", 15000, 0 },
			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },			
			{ "MerienPlateMailM", 1, 0 },
			{ "MerienPlateMailW", 1, 0 },
			{ "MerienChainMailW", 1, 0 },
			{ "MerienChainMailM", 1, 0 },
			{ "Auralith", 1, 0 },
			{ "Noxcrystal", 1, 0 },
			{ "MajBall", 5000, 0 },
			{ "Tokens", 5, 0 }
		} };
	}
	else if (strcmp(npcname, "WereWolf") == 0) {
		return{ "WereWolf", 50, 8, 5, {
			{ "MerienShield", 1, 0 },

			{ "RingofMasterwar", 1, 0 },
			{ "NeckofMasterwar", 1, 0 },
			
			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },

			{ "KlonessBlade", 1, 0 },
			{ "KlonessAxe", 1, 0 },
			{ "RedDevastator", 1, 0 },


			{ "HSWManual", 1, 0 },
			{ "BlazingSword", 1, 0 },
			{ "RapierofEmbers", 1, 0 },
			{ "FireWizardStaff", 1, 0 },	
			{ "FireSegments", 1000, 0 },

			{ "CursedPiece(1)", 1, 0 },
			{ "CursedPiece(5)", 1, 0 },
			{ "CursedPiece(11)", 1, 0 },

			{ "CursedStone(2)", 1, 0 },
			{ "CursedStone(4)", 1, 0 },
			
			{ "HPRecFragments", 30000, 0 },
			{ "LightShards", 30000, 0 },
			{ "StrongShards", 30000, 0 },

			{ "DarkElfBow", 1, 0 },
			{ "HunterSword", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Frost") == 0) {
		return{ "Frost", 50, 8, 5, {
			{ "RingofMastermage", 1, 0 },
			{ "NeckofMastermage", 1, 0 },	
			{ "WizardRapier", 1, 0 },		
			{ "HasteManual", 1, 0 },

			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },

			{ "StormOfDeathManual", 1, 0 },

			{ "KlonessWand(MS.30)", 1, 0 },
			{ "RedZerkWand", 1, 0 },

			{ "CursedPiece(2)", 1, 0 },
			{ "CursedPiece(4)", 1, 0 },
			{ "CursedPiece(10)", 1, 0 },

			{ "CursedStone(1)", 1, 0 },
			{ "CursedStone(3)", 1, 0 },			
			
			{ "MBManual", 1, 0 },
			{ "IceSegments", 1000, 0 },
			{ "IceExecutor", 1, 0 },
			{ "EsterkOfFrost", 1, 0 },
			{ "IceWizardStaff", 1, 0 },			

			{ "HPRecFragments", 30000, 0 },
			{ "LightShards", 30000, 0 },
			{ "StrongShards", 30000, 0 },

			{ "KnecklaceOfIceEle", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Cannibal-Plant") == 0) {
		return{ "Cannibal-Plant", 50, 8, 5, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "PandoraBox(Big)", 1, 0 },

			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },

			{ "MagicNecklace(RM20)", 1, 0 },
			{ "MagicNecklace(RM25)", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			{ "Tokens", 5, 0 }
		} };
	}
	else if (strcmp(npcname, "Stalker") == 0) {
		return{ "Stalker", 50, 8, 5, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },

			{ "MagicNecklace(RM20)", 1, 0 },
			{ "MagicNecklace(RM25)", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },
			{ "RingofPowerwar", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },

			{ "MassFireStrikeManual", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 },

			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			{ "Tokens", 5, 0 }
		} };
	}
	else if (strcmp(npcname, "Nizie") == 0) {
		return{ "Nizie", 80, 10, 5, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "KnecklaceOfIcePro", 1, 0 },

			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },

			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },
			{ "PandoraBox(Small)", 1, 0 },
			{ "IceStormManual", 1, 0 },
			{ "Tokens", 5, 0 },
			{ "RingofGrandMage", 1, 0 },
			{ "SwordofIceElemental", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Beholder") == 0) {
			return{ "Beholder", 80, 10, 5, {
				{ "ContribBall", 15000, 0 },
				{ "PandoraBox(Big)", 1, 0 },

				{ "MagicNecklace(RM20)", 1, 0 },
				{ "MagicNecklace(RM25)", 1, 0 },
				{ "MagicNecklace(RM30)", 1, 0 },

				{ "RingofPowermage", 1, 0 },
				{ "NeckofPowermage", 1, 0 },
				{ "RingofPowerwar", 1, 0 },
				{ "NeckofPowerwar", 1, 0 },

				{ "MassFireStrikeManual", 1, 0 },
				{ "BloodyShoWaveManual", 1, 0 },

				{ "PandoraBox(Medium)", 1, 0 },
				{ "PandoraBox(Small)", 1, 0 },
				{ "Tokens", 5, 0 },
				{ "MajBall", 5000, 0 }
		} };
	}

	
	else if (strcmp(npcname, "Giant-Plant") == 0) {
		return{ "Giant-Plant", 80, 10, 5, {
			{ "Tokens", 5, 0 },
			{ "PandoraBox(Big)", 1, 0 },
			{ "PandoraBox(Medium)", 1, 0 },

			{ "MagicNecklace(DF+20)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },
			{ "MagicNecklace(DF+25)", 1, 0 },

			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },
			{ "RingofPowerwar", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },

			{ "PandoraBox(Small)", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Hellclaw") == 0) {
		return{ "Hellclaw", 120, 0, 6, {

			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },
			{ "KlonessWand(MS.30)", 1, 0 },
			{ "KlonessBlade", 1, 0 },

			{ "MagicWand(M.Shield)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "RingOfMage", 1, 0 },
			{ "MerienShield", 1, 0 },
			{ "KlonessWand(MS.30)", 1, 0 },
			{ "SwordOfDragon", 1, 0 },
			{ "GiantBattleHammer", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "RingofMage", 1, 0 },
			{ "MagicNecklace(RM10)", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "RingofDemonpower", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "RingofWizard", 1, 0 },
			{ "Tokens", 15, 0 },
			{ "EmeraldRing", 1, 0 },
			
			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },

			{ "RingofGrandMage", 1, 0 },
			{ "ResurWand(MS.10)", 1, 0 },
			{ "SwordofIceElemental", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "RubyRing", 1, 0 },
			{ "KnecklaceOfIceEle", 1, 0 },
			{ "RingoftheAbaddon", 1, 0 },
			{ "RingofArcmage", 1, 0 },
			{ "KnecklaceOfIcePro", 1, 0 },
			{ "ArmorDye(Indigo)", 1, 0 },
			{ "ArmorDye(CrimsonRed)", 1, 0 },
			{ "ArmorDye(Gold)", 1, 0 },
			{ "ArmorDye(Aqua)", 1, 0 },
			{ "ArmorDye(Pink)", 1, 0 },
			{ "ArmorDye(Violet)", 1, 0 },
			{ "ArmorDye(Blue)", 1, 0 },
			{ "ArmorDye(Khaki)", 1, 0 },
			{ "ArmorDye(Yellow)", 1, 0 },
			{ "ArmorDye(Red)", 1, 0 },
			{ "ArmorDye(Black)", 1, 0 },
			{ "ArmorDye(Tan)", 1, 0 },
			{ "ArmorDye(Lime)", 1, 0 },
			{ "ArmorDye(Gray)", 1, 0 },
			{ "ArmorDye(Green)", 1, 0 },
			{ "ArmorDye(Decolor)", 1, 0 },
			{ "ArmorDye(White)", 1, 0 },
			{ "ArmorDye(Brown)", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Tigerworm") == 0) {
		return{ "Tigerworm", 120, 0, 6, {

			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },

			{ "KlonessWand(MS.30)", 1, 0 },
			{ "KlonessBlade", 1, 0 },

			{ "XelimaBlade", 1, 0 },
			{ "XelimaRapier", 1, 0 },
			{ "XelimaAxe", 1, 0 },
			{ "KnecklaceOfIcePro", 1, 0 },

			{ "MerienShield", 1, 0 },
			{ "KlonessWand(MS.30)", 1, 0 },
			{ "SwordOfDragon", 1, 0 },
			{ "GiantBattleHammer", 1, 0 },
			
			{ "NeckofFireProtec", 1, 0 },
			{ "MagicNecklace(RM10)", 1, 0 },
			{ "MagicWand(M.Shield)", 1, 0 },
			{ "RingOfMage", 1, 0 },
			{ "RingofWizard", 1, 0 },
			{ "Tokens", 15, 0 },
			{ "RubyRing", 1, 0 },
			{ "RingofGrandMage", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "EmeraldRing", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "KnecklaceOfIceEle", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },

			{ "AncientTreasure", 1, 0 },
			{ "EpicTreasure", 1, 0 },

			{ "RingofDemonpower", 1, 0 },
			{ "RingoftheXelima", 1, 0 },
			{ "IceStormManual", 1, 0 },
			{ "SwordofIceElemental", 1, 0 },
			{ "ResurWand(MS.30)", 1, 0 },
			{ "BloodRapier", 1, 0 },
			{ "Excaliber", 1, 0 },
			{ "RingoftheAbaddon", 1, 0 },
			{ "RingofArcmage", 1, 0 },
			{ "BloodAxe", 1, 0 },
			{ "ArmorDye(Indigo)", 1, 0 },
			{ "ArmorDye(CrimsonRed)", 1, 0 },
			{ "ArmorDye(Gold)", 1, 0 },
			{ "ArmorDye(Aqua)", 1, 0 },
			{ "ArmorDye(Pink)", 1, 0 },
			{ "ArmorDye(Violet)", 1, 0 },
			{ "ArmorDye(Blue)", 1, 0 },
			{ "ArmorDye(Khaki)", 1, 0 },
			{ "ArmorDye(Yellow)", 1, 0 },
			{ "ArmorDye(Red)", 1, 0 },
			{ "ArmorDye(Black)", 1, 0 },
			{ "ArmorDye(Tan)", 1, 0 },
			{ "ArmorDye(Lime)", 1, 0 },
			{ "ArmorDye(Gray)", 1, 0 },
			{ "ArmorDye(Green)", 1, 0 },
			{ "ArmorDye(Decolor)", 1, 0 },
			{ "ArmorDye(White)", 1, 0 },
			{ "ArmorDye(Brown)", 1, 0 }
		} };
	}


	return{ nullptr, 0, 0, 0, {} };
}

int cCoreServer::getMultiplierForDropMode(short dropmode) {
	switch (dropmode) {
	case drop_common: return 1;
	case drop_lesscommon: return 2;
	case drop_rare: return 3;
	case drop_epic: return 20;
	default: return 1;
	}
}

DropItem cCoreServer::getDropItem(int dado, const NPCDropInfo& npcInfo, int multiplier, short dropmode) {
	std::vector<const char*> combinedItems;
	std::vector<int> combinedCounts;

	// 1. Agregar ContribBall si corresponde
	if (npcInfo.baseContrib > 0) {
		combinedItems.push_back("ContribBall");
		combinedCounts.push_back(npcInfo.baseContrib * multiplier);
	}

	// 2. Agregar MajBall si corresponde
	if (npcInfo.baseMajs > 0) {
		combinedItems.push_back("MajBall");
		combinedCounts.push_back(npcInfo.baseMajs * multiplier);
	}

	// 3. Agregar fragmentos si corresponde
	if (npcInfo.baseFragments > 0) {
		const char* fragments[] = {
			"LightShards", "StrongShards", "HitProbFragments", "DRFragments",
			"HPRecFragments", "SPRecFragments", "MPRecFragments", "MRFragments",
			"PaFragments", "MaFragments", "ExpFragments"
		};
		for (const char* frag : fragments) {
			combinedItems.push_back(frag);
			combinedCounts.push_back(npcInfo.baseFragments * multiplier);
		}
	}

	// Seguridad: prevenir acceso fuera de rango
	if (dado < 1 || dado > static_cast<int>(combinedItems.size())) {
		return{ "Nothing", 0, 0 }; // Fallback
	}

	return{ combinedItems[dado - 1], combinedCounts[dado - 1], 0 };
}

void cCoreServer::handleLegendaryDrop(int client, const char* npcname, const NPCDropInfo& npcInfo) {
	int dado = RollDice(1, npcInfo.legendaryDrops.size());
	DropItem dropItem = npcInfo.legendaryDrops[dado - 1];
	CreateDropItem(client, dropItem.name, dropItem.count, dropItem.attribute, drop_legendary, npcname);
}

void cCoreServer::CreateDropItem(int client, const char* itemname, DWORD itemcount, DWORD attribute, short dropmode, const char* npcname) {
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
		ShowClientMsg(client, "You don't have enough space in your drop inventory.");
		return;
	}

	const char* dropType = getDropTypeString(dropmode);

	char cmsg2[250];
	wsprintf(cmsg2, "Character %s get %s drop %d %s from %s",p->m_cCharName, dropType, itemcount, itemname, npcname);
	PutLogList(cmsg2);

	char cmsg[200];
	wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, itemcount, itemname);

	if (dropmode == drop_legendary) {
		broadcastDropMessage(client, npcname, itemname, itemcount, dropType, dropmode);
	}

	SendDropLogMsg(client, cmsg, dropmode);


	add_rare_drop(client, (char*)itemname, itemcount, attribute);
}

const char* cCoreServer::getDropTypeString(short dropmode) {
	switch (dropmode) {
	case drop_common: return "Common";
	case drop_lesscommon: return "Less Common";
	case drop_rare: return "Rare";
	case drop_epic: return "Epic";
	case drop_legendary: return "Legendary";
	default: return "";
	}
}

void cCoreServer::broadcastDropMessage(int client, const char* npcname, const char* itemname, DWORD itemcount, const char* dropType, short dropmode) {
	auto p = m_pClientList[client];
	if (!p) return;

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", p->m_cCharName, dropType, itemcount, itemname, npcname);

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL && i != client) {
			SendDropLogMsg(i, cmsg, dropmode);
		}
	}
}

void cCoreServer::FakeBroadCastMessageHammer() 
{
	
	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalHammer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::FakeBroadCastMessageSB()
{

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalStormBringer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::FakeDropPlayer(char * charname, char itemname, char npcname)
{

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalHammer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::CreateFakeDropItem(char* charname, char* itemname, char* npcname) 
{
	int charIndex = -1;
	for (int i = 1; i < DEF_MAXCLIENTS; i++) 
	{
		if (m_pClientList[i] != NULL) 
		{
			if (string(m_pClientList[i]->m_cCharName) == charname)
			{
				charIndex = i;
				break;
			}
		}
	}

	if (charIndex != -1 && m_pClientList[charIndex])
	{
		auto p = m_pClientList[charIndex];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(charIndex, "You don't have enough space in your drop inventory.");
			return;
		}

		char cmsg[200];
		const char* dropType = getDropTypeString(drop_legendary);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, 1, itemname);

		broadcastDropMessage(charIndex, npcname, itemname, 1, dropType, drop_legendary);

		SendDropLogMsg(charIndex, cmsg, drop_legendary);
		PutLogList(cmsg);

		add_rare_drop(charIndex, (char*)itemname, 1, 0);
	}	
}


bool cCoreServer::IsCurePotion(string itname)
{
	return string(itname) == "FirePotion" ||
		string(itname) == "LightningPotion" ||
		string(itname) == "IcePotion";
}

bool cCoreServer::IsSegment(string itname)
{
	return string(itname) == "IceSegments" ||
		string(itname) == "LightningSegments" ||
		string(itname) == "FireSegments";
}

void cCoreServer::handleDropByLevel(int client, short mode)
{
	auto p = m_pClientList[client];
	if (!p) return;

	string itemname = getMineDropName(mode);
	int count = getMineDropCount(mode);

	if (string(itemname) != "#" && count != -1) CreateMineralDrop(client, (char*)itemname.c_str(), mode, count);
}

char * cCoreServer::getMineDropName(short dropmode)
{
	switch (dropmode)
	{
		case drop_common:
		{
			int dado = RollDice(1, 4);
			switch (dado)
			{
			case 1: return "Iron"; break;
			case 2: return "Copper"; break;
			case 3: return "Tin"; break;
			case 4: return "Bronze"; break;
			default:
				return "#";
				break;
			}
			break;
		}
		case drop_rare:
		{
			int dado = RollDice(1, 4);
			switch (dado)
			{
			case 1: return "Steel"; break;
			case 2: return "Platinum"; break;
			case 3: return "Obsidian"; break;
			case 4: return "Cobalt"; break;
			default:
				return "#";
				break;
			}
			break;
		}
		case drop_epic:
		{
			int dado = RollDice(1, 3);
			switch (dado)
			{
			case 1: return "HPRecFragments"; break;
			case 2: return "LightShards"; break;
			case 3: return "StrongShards"; break;
			/ *case 4: return "Voltrium"; break;* /
			default:
				return "#";
				break;
			}
			break;
		}
		case drop_legendary:
		{
			int dado = RollDice(1, 5);
			switch (dado)
			{
			case 1: return "Solvaris"; break;
			case 2: return "Cryovale"; break;
			case 3: return "Lumivor"; break;
			case 4: return "Emberveil"; break;
			case 5: return "Pyrelith"; break;

			default:
				return "#";
				break;
			}
			break;
		}
		default: return "#"; break;
	}

	return "#";
}


int cCoreServer::getMineDropCount(short dropmode)
{
	switch (dropmode)
	{
		case drop_common:
		{
			int dado = RollDice(2, 5);
			return dado;
			break;
		}
		case drop_rare:
		{
			int dado = RollDice(2, 5);
			return dado;
			break;
		}
		case drop_epic:
		{
			int dado = RollDice(300, 500);
			return dado;
			break;
		}
		case drop_legendary:
		{
			int dado = RollDice(1, 2);
			return dado;
			break;
		}
		default: return -1; break;
	}

	return -1;
}

void cCoreServer::CreateMineralDrop(int client, char* itemname, short mode, int count)
{
	if (m_pClientList[client])
	{
		auto p = m_pClientList[client];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(client, "You don't have enough space in your drop inventory.");
			return;
		}
				
		char cmsg[200];
		const char* dropType = getDropTypeString(mode);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, count, itemname);
		SendDropLogMsg(client, cmsg, mode);

		if (mode == drop_legendary)
		{
			broadcastDropMessage(client, "Mine", itemname, count, dropType, mode);
		}

		add_rare_drop(client, (char*)itemname, count, 0);
	}
}


void cCoreServer::CreateFishingDrop(int client, char* itemname, short mode, int count)
{
	if (m_pClientList[client])
	{
		auto p = m_pClientList[client];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(client, "You don't have enough space in your drop inventory.");
			m_pClientList[client]->m_bFishing = false;
			ShowClientMsg(client, "Fishing cancelled.");
			send_objects_data();
			return;
		}

		char cmsg[200];
		const char* dropType = getDropTypeString(mode);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, count, itemname);
		SendDropLogMsg(client, cmsg, mode);

		if (mode == drop_legendary)
		{
			broadcastDropMessage(client, "Fishing", itemname, count, dropType, mode);
		}

		add_rare_drop(client, (char*)itemname, count, 0);
	}
}

void cCoreServer::NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType)
{
#ifdef DEF_DEBUG
	try {
#endif
		// Validaciones iniciales
		if (m_pNpcList[iNpcH] == NULL) return;
		if ((cAttackerType != DEF_OWNERTYPE_PLAYER) || (m_pNpcList[iNpcH]->m_bIsSummoned == TRUE)) return;

		// Configuración de probabilidades
		const int iItemprobability = 1500;
		const int iGoldProbMax = 25;
		const int iGoldProb2 = 2500;
		const int iSecondProb = 2500;

		// Lista de NPCs que no dropean items
		const short noDropTypes[] = { 1, 2, 3, 4, 5, 6, 21, 40, 64, 87, 89, 91, 125, 126, 127 };
		for (short type : noDropTypes) {
			if (m_pNpcList[iNpcH]->m_sType == type) return;
		}

		// Determinar nivel de generación
		int iGenLevel = GetNpcGenLevel(m_pNpcList[iNpcH]->m_sType);
		if (iGenLevel == 0) return;

		// Proceso principal de generación de drops
		if (iDice(1, 11000) >= iItemprobability) {
			BOOL SaveDrop = FALSE;
			CItem* pItem = nullptr;

			// 35% de chance de drop (35/100)
			if (iDice(1, 11000) <= iGoldProb2) {
				// 60% de que sea oro
				pItem = GenerateGoldDrop(m_pNpcList[iNpcH], m_pClientList[sAttackerH]);
			}
			else {
				// 40% de que sea un item
				if (iDice(1, 10000) <= iSecondProb) {
					// 90% de que sea un item estándar (pociones, etc.)
					pItem = GenerateStandardDrop(iNpcH);
				}
				else {
					// 10% de que sea un item especial (armas/armaduras)
					pItem = GenerateSpecialDrop(iGenLevel);
				}
			}

			// Si se generó un item, procesarlo
			if (pItem != nullptr) {
				ProcessGeneratedItem(pItem, iGenLevel, SaveDrop, m_pNpcList[iNpcH]);
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcDeadItemGenerator");
	}
#endif
}

int cCoreServer::GetNpcGenLevel(short npcType) {
	switch (npcType) {
	case 10: case 16: case 22: case 55: case 56: return 1;
	case 11: case 14: case 17: case 18: return 2;
	case 12: case 23: return 3;
	case 27: case 61: case 74: return 4;
	case 13: case 28: case 53: case 60: case 62: return 5;
	case 29: case 33: case 48: case 54: case 65: case 70: case 72: case 78: return 6;
	case 30: case 63: case 79: return 7;
	case 31: case 32: case 49: case 50: case 52: case 71: return 8;
	case 58: return 9;
	case 59: case 77: case 75: return 10;
	case 93: case 94: case 100: case 101: return 11;
	case 95: case 96: return 12;
	case 97: case 105: case 99: case 122: case 123: case 124: return 13;
	case 104: case 102: case 103: return 14;
	case 98: case 106: return 15;
	case 107: return 16;
	case 108: case 109: case 121: return 17;
	case 112: case 113: return 18;
	case 128: case 114: return 19;
	case 115: case 116: return 20;
	case 117: return 21;
	case 118: case 119: return 22;
	default: return 0;
	}
}

CItem* cCoreServer::GenerateGoldDrop(CNpc* pNpc, CClient* pAttacker) {
	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, "Gold") == FALSE) {
		delete pItem;
		return nullptr;
	}

	DWORD goldAmount = (DWORD)(iDice(1, (pNpc->m_iGoldDiceMax - pNpc->m_iGoldDiceMin)) + pNpc->m_iGoldDiceMin);

	if ((pAttacker != nullptr) && (pAttacker->m_iAddGold != NULL)) {
		double bonus = (double)pAttacker->m_iAddGold;
		goldAmount += (DWORD)((bonus / 100.0f) * (double)goldAmount);
	}

	// Reducir a 1/3 y asegurar mínimo 1
	goldAmount = max(1, goldAmount / 4);
	pItem->m_dwCount = goldAmount;

	return pItem;
}

CItem* cCoreServer::GenerateSpecialDrop(int iGenLevel) {
	int iItemID = 0;

	if (iDice(1, 10000) <= 4000) {
		if (iDice(1, 10000) <= 7000) {
			iItemID = GenerateMeleeWeapon(iGenLevel);
		}
		else {
			iItemID = GenerateMagicWand(iGenLevel);
		}
	}
	else {
		iItemID = GenerateArmor(iGenLevel);
	}

	if (iItemID == 0) return nullptr;

	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
		return nullptr;
	}

	return pItem;
}
void cCoreServer::ProcessGeneratedItem(CItem* pItem, int iGenLevel, BOOL& SaveDrop, CNpc* pNpc) {
	DWORD dwType = 0, dwValue = 0;
	char cColor = 0;
	int iResult = 0;

	// ========== ATAQUE ==========
	if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
		iResult = iDice(1, 80);
		if (iResult <= 9) { dwType = 6; cColor = 2; }
		else if (iResult <= 19) { dwType = 8; cColor = 3; }
		else if (iResult <= 29) { dwType = 1; cColor = 5; }
		else if (iResult <= 39) { dwType = 5; cColor = 1; }
		else if (iResult <= 49) { dwType = 3; cColor = 7; }
		else if (iResult <= 59) { dwType = 2; cColor = 4; }
		else if (iResult <= 69) { dwType = 7; cColor = 6; }
		else { dwType = 9; cColor = 8; }

		pItem->m_cItemColor = cColor;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 3;

		if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		// Secundario (30% chance)
		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 1200);
			if (iResult <= 299) dwType = 2;
			else if (iResult <= 599) dwType = 10;
			else if (iResult <= 899) dwType = 12;
			else dwType = 11;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 3;

			if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========== VARITAS ==========
	else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
		dwType = 10;
		cColor = 5;
		pItem->m_cItemColor = cColor;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 3;

		if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 1200);
			if (iResult <= 299) dwType = 2;
			else if (iResult <= 599) dwType = 10;
			else if (iResult <= 899) dwType = 12;
			else dwType = 11;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 3;

			if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========== DEFENSA ==========
	else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
		iResult = iDice(1, 1200);
		if (iResult <= 299) dwType = 8;
		else if (iResult <= 599) dwType = 6;
		else if (iResult <= 899) dwType = 11;
		else dwType = 12;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 3;

		if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;
		if ((pItem->m_sIDnum == 402) && (dwValue > 1)) dwValue = 1;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 2200);
			if (iResult <= 299) dwType = 3;
			else if (iResult <= 599) dwType = 1;
			else if (iResult <= 899) dwType = 5;
			else if (iResult <= 1199) dwType = 4;
			else if (iResult <= 1499) dwType = 6;
			else if (iResult <= 1799) dwType = 7;
			else if (iResult <= 1999) dwType = 8;
			else dwType = 9;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 3;

			if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========= EXTRAS =========
	_AdjustRareItemValue(pItem);

	pItem->m_sTouchEffectType = DEF_ITET_ID;
	pItem->m_sTouchEffectValue1 = iDice(1, 100000);

	SYSTEMTIME SysTime;
	char cTemp[256];
	GetLocalTime(&SysTime);

	pItem->m_sTouchEffectValue2 = iDice(1, 100000);
	wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
	pItem->m_sTouchEffectValue2 = atoi(cTemp);

	wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
	pItem->m_sTouchEffectValue3 = atoi(cTemp);

	m_pMapList[pNpc->m_cMapIndex]->bSetItem(pNpc->m_sX, pNpc->m_sY, pItem);
	pItem->m_dwTimeDrop = timeGetTime();

	SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, pNpc->m_cMapIndex, pNpc->m_sX, pNpc->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);

	_bItemLog(DEF_ITEMLOG_NEWGENDROP, NULL, NULL, pItem, SaveDrop);
}


int cCoreServer::GenerateMeleeWeapon(int iGenLevel) {
	switch (iGenLevel) {
	case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
		switch (iDice(1, 3)) {
		case 1: return 1;   // Dagger
		case 2: return 8;   // ShortSword
		case 3: return 59;  // LightAxe
		}
		break;
	case 2: // Skeleton, Orc, Orc-Mage, Scorpion, Zombie
		switch (iDice(1, 6)) {
		case 1: return 12;  // MainGauche
		case 2: return 15;  // Gradius
		case 3: return 65;  // SexonAxe
		case 4: return 62;  // Tomahoc
		case 5: return 23;  // Sabre
		case 6: return 31;  // Esterk
		}
		break;
	case 3: // Stone-Golem, Clay-Golem
		switch (iDice(1, 4)) {
		case 1: return 17;  // LongSword
		case 2: return 68;  // DoubleAxe
		case 3: return 23;  // Sabre
		case 4: return 31;  // Esterk
		}
		break;
	case 4: // Hellbound, Rudolph
		switch (iDice(1, 5)) {
		case 1: return 23;  // Sabre
		case 2: return 25;  // Scimitar
		case 3: return 28;  // Falchion
		case 4: return 31;  // Esterk
		case 5: return 34;  // Rapier
		}
		break;
	case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
		switch (iDice(1, 3)) {
		case 1: return 31;  // Esterk
		case 2: return 34;  // Rapier
		case 3: return 71;  // WarAxe
		}
		break;
	case 6: case 11: case 13: // Orge, WereWolf, Stalker, Dark-Elf, Ice-Golem / IceWorm, FireWorm / Wood Golem, Phamton, Butcher
		switch (iDice(1, 5)) {
		case 1: return 50;  // GreatSword
		case 2: return 46;  // Claymore
		case 3: return 31;  // Esterk
		case 4: return 34;  // Rapier
		case 5: return 54;  // Flamberge
		}
		break;
	case 7: case 15: // Liche, Frost / Yeti, IceLizard
		switch (iDice(1, 4)) {
		case 1: return 50;  // GreatSword
		case 2: return 54;  // Flameberge
		case 3: return 31;  // Esterk
		case 4: return 34;  // Rapier
		}
		break;
	case 8: case 12: case 14: case 18: // Demon, Unicorn / Red Demon, Blue Demon / Diablo, Satan / Hellhound, Helliche
		switch (iDice(1, 6)) {
		case 1: return 54;   // Flamberge
		case 2: return 560; // BattleAxe
		case 3: return 615; // GiantSword
		case 4: return 402; // Cape
		case 5: return 615; // GiantSword
		case 6: return 560; // BattleAxe
		}
		break;
	case 9: // MountainGiant
		switch (iDice(1, 6)) {
		case 1: return 23;   // Sabre
		case 2: return 25;   // Scimitar
		case 3: return 760;  // Hammer
		case 4: return 31;   // Esterk
		case 5: return 28;   // Falchion
		case 6: return 760;  // Hammer
		}
		break;
	case 10: // Ettin
		return (iDice(1, 2) == 1 ? 761 : 761); // BattleHammer
	case 20: // Hellbutcher, HellTroll
		switch (iDice(1, 6)) {
		case 1: return 54;   // Flamberge
		case 2: return 560;  // BattleAxe
		case 3: return 615;  // GiantSword
		case 4: return 402;  // Cape
		case 5: return 615;  // GiantSword
		case 6: return 560;  // BattleAxe
		}
		break;
	}
	return 0;
}

int cCoreServer::GenerateMagicWand(int iGenLevel) {
	switch (iGenLevel) {
	case 2: case 3: return 258; // MagicWand(MS0)
	case 4: case 5: case 6: return 257; // MagicWand(MS10)
	case 7: case 8: return 256; // MagicWand(MS20)
	default: return 0;
	}
}

int cCoreServer::GenerateArmor(int iGenLevel) {
	switch (iGenLevel) {
	case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
		switch (iDice(1, 2)) { // Reducido a 2 opciones (sin botas)
		case 1: return 79;  // WoodShield
		case 2: return 80;  // LeatherShield
		}
		break;
	case 2: case 11: // Skeleton, Orc / IceWorm, FireWorm
		switch (iDice(1, 2)) { // Reducido a 2 opciones (sin botas)
		case 1: return 80;  // LeatherShield
		case 2: return 81;  // TargeShield
		}
		break;
	case 3: // Stone-Golem, Clay-Golem
		return 81; // TargeShield
	case 4: // Hellbound, Rudolph
		switch (iDice(1, 8)) { // Reducido a 8 opciones (sin BlondeShield)
		case 1: case 2: return 83;   // Hauberk(M)
		case 3: case 4: return 472;  // Hauberk(W)
		case 5: case 6: return 461;  // ChainHose(M)
		case 7: case 8: return 482;  // ChainHose(W)
		}
		break;
	case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
		switch (iDice(1, 3)) { // Reducido a 3 opciones (sin IronShield si falla el dado)
		case 1: return (iDice(1, 4) <= 2 ? 455 : 475); // LeatherArmor(M/W)
		case 2: return (iDice(1, 4) <= 2 ? 454 : 472); // Hauberk(M/W)
		case 3: return (iDice(1, 4) <= 2 ? 461 : 482); // ChainHose(M/W)
		}
		break;
	case 6: case 13: case 18: case 20: // Orge, WereWolf / Wood Golem / Hellhound / Hellbutcher
		switch (iDice(1, 5)) { // Reducido a 5 opciones (sin ChainHose/Hauberk en caso 6)
		case 1: return (iDice(1, 4) <= 2 ? 476 : 456); // ChainMail(W/M)
		case 2: return (iDice(1, 4) <= 2 ? 458 : 478); // PlateMail(M/W)
		case 3: return 85; // LagiShield
		case 4:
			switch (iDice(1, 8)) {
			case 1: return 750; // Horned-Helm(M)
			case 2: return 751; // Wings-Helm(M)
			case 3: return 754; // Horned-Helm(W)
			case 4: return 755; // Wings-Helm(W)
			case 5: return 752; // Wizard-Cap(M)
			case 6: return 753; // Wizard-Hat(M)
			case 7: return 756; // Wizard-Cap(W)
			case 8: return 757; // Wizard-Hat(W)
			}
		case 5: return (iDice(1, 2) == 1 ? 458 : 478); // PlateMail(M/W) como opción extra
		}
		break;
	case 7: // Liche, Frost
		switch (iDice(1, 3)) { // Reducido a 3 opciones (sin Full-Helm)
		case 1: return (iDice(1, 2) == 1 ? 458 : 478); // PlateMail(M/W)
		case 2: return 86; // KnightShield
		case 3: return 87; // TowerShield
		}
		break;
	case 8: case 12: case 14: case 16: // Demon, Unicorn / Red Demon / Diablo / GrandEttin
	case 17: case 19: case 21: case 22: // HellDeath / DeathWyvern / MountDeath / Hellgoyle
		return 87; // Solo TowerShield (eliminadas capas y botas)
		break;
	}
	return 0;
}

CItem* cCoreServer::GenerateStandardDrop(int iNpcH) {
	int iItemID = 0;
	int iResult = iDice(1, 13000);

	if ((iResult >= 1) && (iResult <= 3000))          iItemID = 95;    // Green Potion
	else if ((iResult >= 3001) && (iResult <= 4000))  iItemID = 91;    // Red Potion
	else if ((iResult >= 4001) && (iResult <= 5500))  iItemID = 93;    // Blue Potion
	else if ((iResult >= 5501) && (iResult <= 6700))  iItemID = 390;   // Power Green Potion
	else if ((iResult >= 6701) && (iResult <= 8500))  iItemID = 92;    // Big Red Potion
	else if ((iResult >= 8501) && (iResult <= 8999))  iItemID = 94;    // Big Blue Potion
	else if ((iResult >= 9000) && (iResult <= 10500)) iItemID = (iDice(1, 2) == 1 ? 390 : 390); // Power Green Potion
	else if ((iResult >= 10501) && (iResult <= 11200)) {
		switch (iDice(1, 4)) {
		case 1: iItemID = 780; break; // Red Candy
		case 2: iItemID = 781; break; // Blue Candy
		case 3:
			switch (iDice(1, 3)) {
			case 1: iItemID = 780; break; // Red Candy
			case 2: iItemID = 781; break; // Blue Candy
			case 3: iItemID = 782; break; // Green Candy
			}
			break;
		/ *case 4:
			switch (iDice(1, 2)) {
			case 1: iItemID = 656; break; // Xelima Stone
			case 2: iItemID = 657; break; // Merien Stone
			}
			break;* /
		case 4: iItemID = 650; break; // Zemstone of Sacrifice
		}
	}
	else if ((iResult >= 11201) && (iResult <= 13000)) {
		SYSTEMTIME SysTime;
		GetLocalTime(&SysTime);
		if (m_pNpcList[iNpcH]) {
			if (((short)SysTime.wMonth == 12) && (m_pNpcList[iNpcH]->m_sType == 61 || 55)) {
				switch (iDice(1, 3)) {
				case 1: iItemID = 780; break; // Red Candy
				case 2: iItemID = 781; break; // Blue Candy
				case 3: iItemID = 782; break; // Green Candy
				}
			}
		}
	}

	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
		return nullptr;
	}

	return pItem;
}*/

#include "Header\\Game.h"
extern char G_cTxt[512];

int ITEMSPREAD_FIEXD_COORD[25][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 }, { 2, 0 },
{ 2, 1 }, { 2, 2 }, { 1, 2 }, { 0, 2 }, { -1, 2 }, { -2, 2 }, { -2, 1 }, { -2, 0 }, { -2, -1 }, { -2, -2 }, { -1, -2 }, { 0, -2 }, { 1, -2 }, { 2, -2 } };


std::map<std::string, std::vector<std::string>> npcMaterials = {
	{ "Orc", { "OrcMeat", "OrcLeather", "OrcTeeth" } },
	{ "Skeleton", { "SkeletonBones" } },
	{ "Unicorn", { "UnicornHeart", "UnicornHorn", "UnicornMeat", "UnicornLeather" } },
	{ "Troll", { "TrollHeart", "TrollMeat", "TrollLeather", "TrollClaw" } },
	{ "Scorpion", { "ScorpionPincers", "ScorpionMeat", "ScorpionSting", "ScorpionSkin" } },
	{ "Cyclops", { "CyclopsEye", "CyclopsHandEdge", "CyclopsHeart", "CyclopsMeat", "CyclopsLeather" } },
	{ "Orge", { "OgreHair", "OgreHeart", "OgreMeat", "OgreLeather", "OgreTeeth", "OgreClaw" } },
	{ "Stone-Golem", { "StoneGolemPiece" } },
	{ "Clay-Golem", { "LumpofClay" } },
	{ "WereWolf", { "WerewolfHeart", "WerewolfNail", "WerewolfMeat", "WerewolfTail", "WerewolfTeeth", "WerewolfLeather", "WerewolfClaw" } },
	{ "Hellbound", { "HelboundHeart", "HelboundLeather", "HelboundTail", "HelboundTeeth", "HelboundClaw", "HelboundTongue" } },
	{ "Demon", { "DemonHeart", "DemonMeat", "DemonLeather", "DemonEye" } }
};

#define CONTRIB_SMALL 99901
#define MAJ_SMALL 99902
#define REP_SMALL 99903

#define CONTRIB_MEDIUM 99904
#define MAJ_MEDIUM 99905
#define REP_MEDIUM 99906

#define COIN_SMALL 99907
#define COIN_MEDIUM 99908
#define COIN_BIG 99909

//HASTA ACA
void cCoreServer::DeleteNpc(int iNpcH, BOOL bHeld, BOOL Drop)
{
#ifdef DEF_DEBUG
	try {
#endif
		int  i, iNamingValue, iItemID, iNumItem, iItemIDs[MAX_NPCITEMDROP];
		char cTmp[21], cItemName[21], cTemp[256];
		class CItem * pItem;
		DWORD dwCount, dwTime;
		POINT	ItemPositions[MAX_NPCITEMDROP];
		SYSTEMTIME SysTime;
		char cTxt[100];
		int k;
		short sAttackerH;


		if (m_pNpcList[iNpcH] == NULL) return;

		iItemID = 0;
		iNumItem = 0;

		dwTime = timeGetTime();

		SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, CLIENT_COMMON_COMMAND_LOG, DEF_MSGTYPE_REJECT, NULL, NULL, NULL);
		if (m_pNpcList[iNpcH]->m_sAreaSize == 0) {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
		}
		else {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearBigOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, m_pNpcList[iNpcH]->m_sAreaSize);
		}
		ZeroMemory(cTmp, sizeof(cTmp));
		strcpy_secure(cTmp, (char *)(m_pNpcList[iNpcH]->m_cName + 2));
		iNamingValue = atoi(cTmp);

		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
		m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalActiveObject--;
		if (Drop == FALSE) m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject--;

		// Spot-mob-generator���� 
		if (m_pNpcList[iNpcH]->m_iSpotMobIndex != NULL)
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stSpotMobGenerator[m_pNpcList[iNpcH]->m_iSpotMobIndex].iCurMobs--;

		RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);

		if (m_pNpcList[iNpcH]->m_sType != 66 && m_pNpcList[iNpcH]->m_sType != 73 && m_pNpcList[iNpcH]->m_sType != 81)
		{
			bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, NULL);

			delete m_pNpcList[iNpcH];
			m_pNpcList[iNpcH] = NULL;
			return;
		}

		switch (m_pNpcList[iNpcH]->m_sType) {
		case 36:
		case 37:
		case 38:
		case 39:
		case 42:
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCrusadeStructureInfo(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

			for (i = 0; i < DEF_MAXGUILDS; i++) {
				if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID) {
					m_pGuildTeleportLoc[i].m_dwTime = dwTime;
					m_pGuildTeleportLoc[i].m_iV2--;
					if (m_pGuildTeleportLoc[i].m_iV2 < 0) m_pGuildTeleportLoc[i].m_iV2 = 0;
					break;
				}
			}
			break;

		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 51:
			for (i = 0; i < DEF_MAXGUILDS; i++) {
				if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID) {
					m_pGuildTeleportLoc[i].m_iNumSummonNpc--;
					if (m_pGuildTeleportLoc[i].m_iNumSummonNpc < 0) m_pGuildTeleportLoc[i].m_iNumSummonNpc = 0;
					break;
				}
			}
			break;

		case 64:	//v2.19 2002-12-16 ���� ��ų ����
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum();
			break;
		}

		// DelayEvent�� ���� 
		bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, NULL);
		int iItemprobability = 65;

		if ((m_iMiddlelandMapIndex == m_pNpcList[iNpcH]->m_cMapIndex) && (m_iMiddlelandMapIndex != -1)) iItemprobability = 55;
		if (m_iWhouseMapIndex == m_pNpcList[iNpcH]->m_cMapIndex) iItemprobability = 15;
		if (m_pNpcList[iNpcH]->m_bIsSummoned == FALSE) {
			pItem = new class CItem;
			ZeroMemory(cItemName, sizeof(cItemName));

			//V10 Reapertura low drops
			switch (m_pNpcList[iNpcH]->m_sType) {
			case 1: // Mercenary
			case 2: // Mercenary
			case 3: // Mercenary
			case 4: // Mercenary
			case 5: // Mercenary
			case 6: // Mercenary
			case 36: // Sade Structure
			case 37: // Sade Structure
			case 38: // Sade Structure
			case 39: // Sade Structure
			case 40: // Sade Structure
			case 41: // Sade Structure
			case 42: // Sade Structure
			case 87: // Tower Denfense Structure
			case 89: // Tower Denfense Structure
			case 92: // Reliquia
			case 55: // Rabbit
			case 56: // Cat
				// No drop
				break;
			case 10: //Slime //New Rare Drop Helbreath Argentina v4
				switch (iDice(1, 2)) {
				case 1: if (iDice(1, 10) >= 5) iItemID = 220; break;	// "SlimeJelly"
				case 2: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"/*
				case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"*/
				default: break;
				}
				break;
			case 11: //Skeleton
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 20) == 1) iItemID = 219; break;	// "SkeletonBones"
				case 2: if (iDice(1, 100) == 5) iItemID = 390; break;	// "RubyRing"
				case 3: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"/*
					/*case 4: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
					case 5: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"* /*/
				case 4: if (iDice(1, 5000) == 500) iItemID = 390; break;	// "EmeraldRing"
				default: break;
				}
				break;
			case 12: //StoneGolem
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 221; break;		// "StoneGolemPiece"
				case 2: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
					/*case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
					case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"*/
				case 3: if (iDice(1, 2500) == 50) iItemID = 647; break;		// "NecklaceOfStoneGolem"
				case 4: if (iDice(1, 4500) == 100) iItemID = 858; break;		// "NecklaceOfMerien"
				default: break;
				}
				break;
			case 13://Cyclops
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 194; break;	// "CyclopsEye"
				case 2: if (iDice(1, 40) == 1) iItemID = 195; break;	// "CyclopsHandEdge"
				case 3: if (iDice(1, 30) == 1) iItemID = 196; break;	// "CyclopsHeart"
				case 4: if (iDice(1, 20) == 1) iItemID = 197; break;	// "CyclopsMeat"
				case 5: if (iDice(1, 40) == 1) iItemID = 198; break;	// "CyclopsLeather"
				case 6: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;	// Rep Small ball
				case 7: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"
					/*case 8: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
					case 9: if (iDice(1, 1000) == 150) iItemID = 657; break;	// "StoneOfMerien"*/
				case 8: if (iDice(1, 500) == 15) iItemID = 308; break;	// "Neck(MS10)"
				case 9: if (iDice(1, 500) == 15) iItemID = 300; break;	// "Neck(RM10)"
				case 10: if (iDice(1, 500) == 15) iItemID = 311; break;	// "Neck(DF10)"
				default: break;
				}
				break;
			case 14: //orco
				switch (iDice(1, 12)) {
				case 1: if (iDice(1, 11) == 1) iItemID = 206; break;		// "OrcMeat"
				case 2: if (iDice(1, 20) == 1) iItemID = 207; break;		// "OrcLeather"
				case 3: if (iDice(1, 21) == 1) iItemID = 208; break;		// "OrcTeeth"
				case 4: if (iDice(1, 150) == 15) iItemID = 336; break;		// "SaphireRing"
				case 5: if (iDice(1, 150) == 25) iItemID = 333; break;		// "PlatiniumRing"
				case 6: if (iDice(1, 150) == 35) iItemID = 334; break;		// "LuckyGoldRing"
				case 7: if (iDice(1, 150) == 45) iItemID = 337; break;		// "RubyRing"
				case 8: if (iDice(1, 1000) == 15) iItemID = 335; break;		// "EmeraldRing"
				case 9: if (iDice(1, 100) == 45) iItemID = 634; break;		// "RingOfWizard"
				case 10: if (iDice(1, 200) == 25) iItemID = 635; break;		// "RingOfMage"
				case 11: if (iDice(1, 1000) == 35) iItemID = 390; break;		// "ZemstoneofSacrifice"
					/*case 12: if (iDice(1, 1000) == 45) iItemID = 656; break;		// "StoneOfXelima"
					case 13: if (iDice(1, 1000) == 25) iItemID = 657; break;		// "StoneOfMerien"*/
				case 12: if (iDice(1, 5000) == 500) iItemID = 638; break;		// "NecklaceOfFireProtection"
				default: break;
				}
				break;
			case 16: //Ant
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 9) == 1)  iItemID = 192; break;	// "AntLeg"
				case 2: if (iDice(1, 10) == 1) iItemID = 193; break;	// "AntFeeler"
				case 3: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"/*
				case 4: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 5: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"*/
				default: break;
				}
				break;
			case 17: //Scorpion
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 40) == 1) iItemID = 215; break;		// "ScorpionPincers"
				case 2: if (iDice(1, 20) == 1) iItemID = 216; break;		// "ScorpionMeat"
				case 3: if (iDice(1, 40) == 1) iItemID = 217; break;		// "ScorpionSting"
				case 4: if (iDice(1, 40) == 1) iItemID = 218; break;		// "ScorpionSkin"
				case 5: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 6: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
					/*
					case 7: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
					case 8: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"*/
				case 7: if (iDice(1, 500) == 5) iItemID = 634; break;		// "RingOfWizard"
				case 8: if (iDice(1, 500) == 5) iItemID = 336; break;		// "SaphireRing"
				case 9: if (iDice(1, 500) == 145) iItemID = 333; break;	// "PlatiniumRing"
				default: break;
				}
				break;
			case 18: // Zombie
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 390; break;			// "ZemstoneofSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;			// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;			// "StoneOfMerien"
				case 4: if (iDice(1, 800) == 3) iItemID = REP_SMALL; break;			// Rep Small ball
				default: break;
				}
				break;
			case 22: //Snake
				switch (iDice(1, 8)) {
				case 1: if (iDice(1, 15) == 1) iItemID = 188; break;	// "SnakeMeat"
				case 2: if (iDice(1, 16) == 1) iItemID = 189; break;	// "SnakeSkin"
				case 3: if (iDice(1, 16) == 1) iItemID = 190; break;	// "SnakeTeeth"
				case 4: if (iDice(1, 17) == 1) iItemID = 191; break;	// "SnakeTounge"
				case 5: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"
				case 6: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 7: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				case 8: if (iDice(1, 5000) == 50) iItemID = 640; break;	// "NecklaceOfSufferent"
				default: break;
				}
				break;
			case 23: //ClayGolem
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 30) == 1) iItemID = 205; break;		// "LumpOfClay"
				case 2: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 3: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 4: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"						
				default: break;
				}
				break;
			case 27: //Hellbound
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 40) == 1) iItemID = 199; break;	// "HelhoundHeart"
				case 2: if (iDice(1, 30) == 1) iItemID = 200; break;	// "HelhoundLeather"
				case 3: if (iDice(1, 30) == 1) iItemID = 201; break;	// "HelhoundTail"
				case 4: if (iDice(1, 30) == 1) iItemID = 202; break;	// "HelhoundTeeth"
				case 5: if (iDice(1, 30) == 1) iItemID = 203; break;	// "HelhoundClaw"
				case 6: if (iDice(1, 50) == 1) iItemID = 204; break;	// "HelhoundTounge"
				case 7: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;// Rep Small ball
				case 8: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"
				case 9: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 10: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				default: break;
				}
				break;
			case 28://Troll
				switch (iDice(1, 12)) {
				case 1: if (iDice(1, 35) == 1) iItemID = 222; break;	// "TrollHeart"
				case 2: if (iDice(1, 23) == 1) iItemID = 223; break;	// "TrollMeat"
				case 3: if (iDice(1, 25) == 1) iItemID = 224; break;	// "TrollLeather"
				case 4: if (iDice(1, 27) == 1) iItemID = 225; break;	// "TrollClaw"
				case 5: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;	// Rep Small ball
				case 6: if (iDice(1, 100) == 5) iItemID = 334; break;	// "LuckyGoldRing"
				case 7: if (iDice(1, 100) == 5) iItemID = 337; break;	// "RubyRing"
				case 8: if (iDice(1, 100) == 5) iItemID = 336; break;	// "SaphireRing"
				case 9: if (iDice(1, 100) == 5) iItemID = 333; break;	// "PlatinumRing"
				case 10: if (iDice(1, 1000) == 50) iItemID = 390; break;	// "ZemstoneofSacrifice"
				case 11: if (iDice(1, 1000) == 50) iItemID = 656; break;	// "StoneOfXelima"
				case 12: if (iDice(1, 1000) == 50) iItemID = 657; break;	// "StoneOfMerien"
				default: break;
				}
				break;
			case 29://Ogre
				switch (iDice(1, 13)) {
				case 1: if (iDice(1, 20) == 1) iItemID = 209; break;		// "OgreHair"
				case 2: if (iDice(1, 22) == 1) iItemID = 210; break;		// "OgreHeart"
				case 3: if (iDice(1, 25) == 1) iItemID = 211; break;		// "OgreMeat"
				case 4: if (iDice(1, 25) == 1) iItemID = 212; break;		// "OgreLeather"
				case 5: if (iDice(1, 25) == 1) iItemID = 213; break;		// "OgreTeeth"
				case 6: if (iDice(1, 25) == 1) iItemID = 214; break;		// "OgreClaw"
				case 7: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 8: if (iDice(1, 100) == 15) iItemID = 300; break;		// "Neck(RM10)"
				case 9: if (iDice(1, 100) == 15) iItemID = 311; break;		// "Neck(DF10)"
				case 10: if (iDice(1, 500) == 20) iItemID = 632; break;		// "RingofOgrePower"
				case 11: if (iDice(1, 1000) == 60) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 12: if (iDice(1, 1000) == 60) iItemID = 656; break;		// "StoneOfXelima"
				case 13: if (iDice(1, 1000) == 60) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 30://Liche
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 1000) == 60) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 60) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 60) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 1500) == 50) iItemID = 259; break;		// "MagicWand(MSHIELD)"
				case 5: if (iDice(1, 500) == 3) iItemID = 636; break;		// "RingOfGrandMage"
				case 6:	if (iDice(1, 4000) == 3) iItemID = 734; break;		// "RingOfArcMage"
				case 7: if (iDice(1, 5000) == 82) iItemID = 648; break;		// "NecklaceOfLiche"
				case 8: if (iDice(1, 6000) == 250) iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
				case 9:	if (iDice(1, 8000) == 300) iItemID = 381; break;		// "MassFireStrikeManual"
				case 10: if (iDice(1, 10000) == 100) iItemID = 382; break;		// "BloodyShoWaveManual"
				default: break;
				}
				break;
			case 31://Demons
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 15) == 3) iItemID = 541; break;		// "DemonHeart"
				case 2:	if (iDice(1, 5) == 3) iItemID = 542; break;			// "DemonMeat"
				case 3:	if (iDice(1, 10) == 3) iItemID = 543; break;		// "DemonLeather"
				case 4:	if (iDice(1, 15) == 3) iItemID = 540; break;		// "DemonEye"
				case 5: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 6: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 7: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 8: if (iDice(1, 500) == 10) iItemID = 633; break;		// "RingofDemonpower"
				case 9: if (iDice(1, 5000) == 100) iItemID = 382; break;		// "BloodyShoWaveManual"
				default: break;
				}
				break;
			case 32: //Unicorn
				switch (iDice(1, 8)) {
				case 1: if (iDice(1, 40) == 3) iItemID = 544; break;		// "UnicornHeart"
				case 2: if (iDice(1, 25) == 3) iItemID = 545; break;		// "UnicornHorn"	
				case 3: if (iDice(1, 10) == 3) iItemID = 546; break;		// "UnicornMeat"	
				case 4: if (iDice(1, 15) == 3) iItemID = 547; break;		// "UnicornLeather"					
				case 5: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 6: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 7: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 8: if (iDice(1, 20000) == 2520) iItemID = 851; break;		// "KlonessEsterk"
				default: break;
				}
				break;
			case 33://WereWolf
				switch (iDice(1, 10)) {
				case 1: if (iDice(1, 30) == 3) iItemID = 551; break;		// "WerewolfTail"
				case 2: if (iDice(1, 25) == 3) iItemID = 548; break;		// "WerewolfHeart"
				case 3: if (iDice(1, 25) == 3) iItemID = 550; break;		// "WerewolfMeat"
				case 4: if (iDice(1, 35) == 3) iItemID = 553; break;		// "WerewolfLeather"
				case 5: if (iDice(1, 25) == 3) iItemID = 552; break;		// "WerewolfTeeth"
				case 6: if (iDice(1, 25) == 3) iItemID = 554; break;		// "WerewolfClaw"
				case 7: if (iDice(1, 35) == 3) iItemID = 549; break;		// "WerewolfNail"
				case 8: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 9: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 10: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				default: break;
				}
				break;
			case 48: // Stalker
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 10) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 20) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = REP_SMALL; break;	// Rep Small ball
				case 4: if (iDice(1, 1000) == 30) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 5: if (iDice(1, 1500) == 30) iItemID = 292; break;		// "GoldenAxeLLF"
				default: break;
				}
				break;
			case 52: // Gagoyle:
				switch (iDice(1, 11)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 500) == 10) iItemID = 630; break;		// "ringofxelima"
				case 5: if (iDice(1, 10000) == 1000) iItemID = 381; break;		// "FireStrikeManual"
				case 6: if (iDice(1, 10000) == 1000) iItemID = 382; break;		// "BloodyShockWaveManual"
				case 7: if (iDice(1, 5000) == 50) iItemID = 20; break;		// "Excalibur"
				case 8: if (iDice(1, 5000) == 100) iItemID = 610; break;		// "xelimablade"
				case 9: if (iDice(1, 90000) == 15800) iItemID = 860; break;		// "NecklaceOfXelima"
				case 10: if (iDice(1, 5000) == 400) iItemID = 612; break;		// "xelimarapier"
				case 11: if (iDice(1, 5000) == 150) iItemID = 611; break;		// "xelimaaxe"
				default: break;
				}
				break;
			case 54: // Dark-Elf
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 500) == 5) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 500) == 5) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 500) == 5) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 60000) == 25810) iItemID = 618; break;	// "DarkelfBow"
				default: break;
				}
				break;
			case 58: // Mountain Giant
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 10) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 20) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 500) == 50) iItemID = REP_SMALL; break;	// Rep Small ball
				case 4: if (iDice(1, 1000) == 30) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 5: if (iDice(1, 500) == 3) iItemID = 337; break;		// "RubyRing"
				default: break;
				}
				break;
			case 59: // Ettin
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 1000) == 50) iItemID = 390; break;		// "ZemstoneofSacrifice"
				case 4: if (iDice(1, 500) == 30) iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, REP_SMALL); break;	//rare item
				default: break;
				}
				break;
			case 71: // Centaurus
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 390; break;		// "ZemstoneOfSacrifice"	
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"	
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 61: // Rudolph
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 390; break;		// "ZemstoneOfSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
			case 57: // Giant-Frog
			case 62: //Direboard
			case 70: // Barlog
			case 75: // Giant-Lizard
			case 77: // MasterMage-Orc
			case 122: //Goblin
			case 123: //GmGoblin
				switch (iDice(1, 3)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 390; break;		// "ZemstoneOfSacrifice"
				case 2: if (iDice(1, 1000) == 30) iItemID = 656; break;		// "StoneOfXelima"
				case 3: if (iDice(1, 1000) == 30) iItemID = 657; break;		// "StoneOfMerien"
				default: break;
				}
				break;
				//Nuevos NPC 2Drop LaloRamos
			case 93:	//IceWorm
			case 94:	//FireWorm
			case 100:	//Snoob
			case 101:	//FireSnoob
				switch (iDice(1, 5)) {
				case 1: if (iDice(1, 1000) == 250) iItemID = 390; break; //"ZemstoneofSacrifice"
				case 2: if (iDice(1, 1000) == 250) iItemID = MAJ_SMALL; break; // Majestics Small ball
				case 3: if (iDice(1, 1000) == 250) iItemID = CONTRIB_SMALL; break;//ContribBall Small Ball
				case 4: if (iDice(1, 1000) == 250) iItemID = REP_SMALL; break; // Rep Small ball
				case 5: if (iDice(1, 1000) == 250) iItemID = 942; break; //"RighStone"
				default: break;
				}
				break;
			case 97:	//Good-Golem
			case 98:	//Ice-Lizard
			case 99:	//Butcher
				switch (iDice(1, 6)) {
				case 1: if (iDice(1, 1000) == 350) iItemID = 941; break; //"PoisonStone"
				case 2: if (iDice(1, 1000) == 350) iItemID = 944; break; //"AgileStone"
				case 3: if (iDice(1, 1000) == 350) iItemID = 945; break; //"LightStone"
				case 4: if (iDice(1, 1000) == 350) iItemID = 946; break; //"SharpStone"
				case 5: if (iDice(1, 1000) == 350) iItemID = 948; break; //"AncStone"
				case 6: if (iDice(1, 1000) == 350) iItemID = 951; break; //"CritStone"
				default: break;
				}
				break;
			case 103:	//GiantSpider
			case 104:	//Diablo
			case 105:	//Phamton
			case 106:	//Yeti

				break;
			case 95:	//Blue Demon
			case 96:	//Red Demon
			case 102:	//Satan
			case 107:	//GrandEttin


				break;
			case 124: //Danny

				break;
			case 79:	// Nizie
			case 63:	// Frost
				switch (iDice(1, 4)) {
				case 1: if (iDice(1, 1000) == 50) iItemID = 656; break;		// "StoneOfXelima"
				case 2: if (iDice(1, 1000) == 50) iItemID = 657; break;		// "StoneOfMerien"
				case 3: if (iDice(1, 800) == 3) iItemID = REP_SMALL; break;		// Rep Small ball
				case 4: if (iDice(1, 500) == 3) iItemID = REP_SMALL; break;		// Rep Small ball

				default: break;
				}
				break;
			case 53: // Beholder
				switch (iDice(1, 2)) {
				case 1: if (iDice(1, 1000) == 20) iItemID = REP_SMALL; break;		// Rep Small ball
				case 2: if (iDice(1, 1000) == 50) iItemID = REP_SMALL; break;		// Rep Small ball
					//	case 3: if (iDice(1, 100000) == 100) iItemID = 857; break;	// "I.M.CManual"
				default: break;
				}
				break;
			case 112://Newmobs
			case 113:
			case 115:
			case 116:
			case 117:
			case 118:
			case 119:
				switch (iDice(1, 9)) {
				case 1: if (iDice(1, 1000) == 30) iItemID = 942; break; //"RighStone"
				case 2: if (iDice(1, 1000) == 50) iItemID = 3733; break; //"ArmorDye(2) - Naranja"
				case 3: if (iDice(1, 1000) == 50) iItemID = 3734; break; //"ArmorDye(3) - Blanco"
				case 4: if (iDice(1, 1000) == 50) iItemID = 3737; break; //"ArmorDye(6) - Celeste"
				case 5: if (iDice(1, 1000) == 50) iItemID = 3738; break; //"ArmorDye(7) - Negro"
				case 6: if (iDice(1, 1000) == 50) iItemID = 3739; break; //"ArmorDye(8) - Amarillo"
				case 7: if (iDice(1, 1000) == 50) iItemID = 3740; break; //"ArmorDye(9) - Rosa"
				case 8: if (iDice(1, 1000) == 50) iItemID = 3741; break; //"ArmorDye(10) - Violeta"
				case 9: if (iDice(1, 1000) == 50) iItemID = 3742; break; //"ArmorDye(11) - Marron"
				default: break;
				}
				break;
			case 49: // Hellclaw
				if (Drop)
				{
					switch (iDice(1, 12)) {

					case 1: iItemID = 642; break;	// "KnecklaceOfIcePro"
					default:
						switch (iDice(1, 12))
						{
						case 1: iItemID = 259; break;	// "MagicWand(M.Shield)"
						case 2: iItemID = 311;	break;	// "MagicNecklace(DF+10)"
						case 3: iItemID = 308;	break;	// "MagicNecklace(MS10)"
						case 4: iItemID = 305;	break;	// "MagicNecklace(DM+1)"
						case 5: iItemID = 337;	break;	// "RubyRing"
						case 6: iItemID = 634; break;	// "RingofWizard"
						case 7: iItemID = 335; break;	// "EmeraldRing"
						case 8: iItemID = 3011;	break;	// "RepPotionRestore"
						case 9: iItemID = REP_SMALL; break;// Rep Small ball
						case 10: iItemID = 635; break;	// "RingOfMage" 
						case 11: iItemID = 636; break;	// "RingOfGrandMage" 
						case 12: iItemID = 642;	break;	// "KnecklaceOfIcePro"
						}
						break;
					}
				}
				break;
			case 50: // TigerWorm
				if (Drop)
				{
					switch (iDice(1, 50)) {
					case 1: iItemID = 610; break; // "XelimaBlade"
					case 2: iItemID = 612; break; // "XelimaRapier"							
					case 3: iItemID = 611; break; // "XelimaAxe"
					case 4: iItemID = 642; break; // "KnecklaceOfIcePro"
					case 5: iItemID = 631; break; // "RingoftheAbaddon"
					case 6: iItemID = 638; break; // "NeckofFireProtec"
					case 7: iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, CONTRIB_SMALL); break;	//ContribBall Small Ball o item muy raro en este caso hay 2 probabilidades, la del metodo getrareitem y esta
					case 11: iItemID = 857; break;// "I.M.CManual"
					default:
						switch (iDice(1, 11))
						{
						case 1: iItemID = 300;	break; // "MagicNecklace(RM10)"
						case 2: iItemID = 259;	break; // "MagicWand(M.Shield)"
						case 3: iItemID = 311;	break; // "MagicNecklace(DF+10)"
						case 4: iItemID = 308;	break; // "MagicNecklace(MS10)"
						case 5: iItemID = 305;	break; // "MagicNecklace(DM+1)"
						case 6: iItemID = 633; break; // "RingofDemonpower"
						case 7: iItemID = 630;	break; // "RingOfXelima"
						case 8: iItemID = 614;	break; // "SwordOfIceElemental"
						case 9: iItemID = 492; break; // "BloodRapier"
						case 10: iItemID = 20; break; // "Excaliber"
						case 11: iItemID = 491; break;// "BloodAxe"/*
							//case 12: iItemID = 290; break;// "Flamberge+3LLF "*/
						}
						break;
					}
				}
				break;
			case 108: // HellDeath
				if (Drop)
				{
					switch (iDice(1, 10)) {
					case 1: iItemID = 4964; break;		//	"RingofPowerWar"
					case 2: iItemID = 4965; break;		//	"RingofPowerMage"
					case 3: iItemID = 4966; break;		//	"NeckPower(MS.30)"
					case 4: iItemID = 4968; break;		//	"NeckPowerXelima"
					case 5: iItemID = 4967; break;		//	"NeckPowerMerien"
					case 6: iItemID = CONTRIB_SMALL; break;		//	"Coins(+100)"
					default:
						switch (iDice(1, 6))
						{
						case 1: iItemID = 4964; break;		//	"RingofPowerWar"
						case 2: iItemID = 4965; break;		//	"RingofPowerMage"
						case 3: iItemID = 4966; break;		//	"NeckPower(MS.30)"
						case 4: iItemID = 4968; break;		//	"NeckPowerXelima"
						case 5: iItemID = 4967; break;		//	"NeckPowerMerien"
						case 6: iItemID = getRareItem(m_pNpcList[iNpcH]->m_sType, CONTRIB_SMALL); break;	//ContribBall Small Ball
						}
						break;
					}
				}
				break;
			case 109: // LightWorm
				if (Drop)
				{
					switch (iDice(1, 20)) {
						/*	case 1: iItemID = 4905; break;		//	"BlackHammer"
						case 2: iItemID = 861; break;		//	"BerserkWand(MS.30)
						case 3: iItemID = 850; break;		//	"KlonessAxe"
						case 4: iItemID = 849; break;		//	"KlonessBlade"
						case 5: iItemID = 850; break;		//	"KlonessAxe"
						case 6: iItemID = 848; break;		//	"LightingBlade"*/
						//		case 7: iItemID = 616; break;		//	"DemonSlayer"
						//		case 8: iItemID = 863; break;		//  "KlonessWand(MS.30)"
					case 1: iItemID = 4964; break;		//	RingofPowerWar
					case 2: iItemID = 4965; break;		//	RingofPowerMage
					case 3: iItemID = 611; break;		// "XelimaAxe"
					case 4: iItemID = 643; break;		// "knecklaceOfIceEle
					case 5: iItemID = CONTRIB_SMALL; break;	//ContribBall Small Ball
					default:
						switch (iDice(1, 10))
						{
						case 1: iItemID = 4964; break;		//	RingofPowerWar
						case 2: iItemID = 4965; break;		//	RingofPowerMage
						case 3: iItemID = 4966; break;		//	NeckPower(MS.30)
						case 4: iItemID = 4967; break;		//	NeckPowerMerien
						case 5: iItemID = 4968; break;		//	NeckPowerXelima
						case 6: iItemID = CONTRIB_SMALL; break;		// "Coins(+500)"
						case 7: iItemID = 610; break;		// "XelimaBlade"
						case 8: iItemID = 611; break;		// "XelimaAxe"
						case 9: iItemID = 643; break;		// "knecklaceOfIceEle
						case 10: iItemID = CONTRIB_SMALL; break;	//ContribBall Small Ball
						}
						break;
					}
				}
				break;
			case 121: // GhostAba
				if (Drop)
				{
					switch (iDice(1, 20)) {
					case 1: iItemID = 4964; break;		//	RingofPowerWar
					case 2: iItemID = 4965; break;		//	RingofPowerMage	
					case 3: iItemID = 4967; break;		//	NeckPowerMerien
					case 4: iItemID = 4968; break;		//	NeckPowerXelima
					case 5: iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
					case 6: iItemID = 847; break;		// "DarkExecutor"
					case 7: iItemID = 860; break;		// "NecklaceOfXelima"
					case 8: iItemID = 858; break;		// "NecklaceOfMerien"
					default:
						switch (iDice(1, 8))
						{
						case 1: iItemID = 4964; break;		//	RingofPowerWar
						case 2: iItemID = 4965; break;		//	RingofPowerMage	
						case 3: iItemID = 4967; break;		//	NeckPowerMerien
						case 4: iItemID = 4968; break;		//	NeckPowerXelima
						case 5: iItemID = CONTRIB_SMALL; break;		//ContribBall Small Ball
						case 6: iItemID = 847; break;		// "DarkExecutor"
						case 7: iItemID = 860; break;		// "NecklaceOfXelima"
						case 8: iItemID = 858; break;		// "NecklaceOfMerien"
						}
						break;
					}
				}
				break;

			case 66: // Wyvern 
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						3, 6, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			case 73: // Fire-Wyvern
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						3, 6, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
			case 81: //Abaddon
				if (Drop == TRUE) {
					bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
						6, 12, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 65,
						iItemIDs, ItemPositions, &iNumItem);
				}
				break;
				/*case 128: // BlackWyvern
				case 114: // DeathWyvern
				if (Drop == TRUE) {
				bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
				3, 9, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 4,
				iItemIDs, ItemPositions, &iNumItem);
				}
				break;
				case 120: // HellAbaddon
				if (Drop == TRUE) {
				bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType, iItemprobability,
				6, 12, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, DEF_ITEMSPREAD_FIXED, 65,
				iItemIDs, ItemPositions, &iNumItem);
				}
				break;*/
			}

			dwCount = 1;

			if (iItemID == 0) {
				switch (m_pNpcList[iNpcH]->m_sType) {
				case 34:
					iItemID = 90;
					dwCount = (DWORD)(iDice(1, m_pNpcList[iNpcH]->m_iExp * 4) + m_pNpcList[iNpcH]->m_iExp);
					dwCount = dwCount - (dwCount / 3);
					break;
				}
			}

			if (iNumItem > 0) {
				GetLocalTime(&SysTime);
				wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
				for (int j = 0; j < iNumItem; j++) {
					if (pItem == NULL) pItem = new class CItem;


					//pItem = new class CItem;
					if (iItemIDs[j] == CONTRIB_MEDIUM)
					{
						iItemIDs[j] = 3052;
					}
					else if (iItemIDs[j] == COIN_MEDIUM)
					{
						iItemIDs[j] = 3053;
					}
					else if (iItemIDs[j] == REP_MEDIUM)
					{
						iItemIDs[j] = 3054;
					}
					else if (iItemIDs[j] == MAJ_MEDIUM)
					{
						iItemIDs[j] = 3055;
					}



					if (_bInitItemAttr(pItem, iItemIDs[j]) != FALSE &&
						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bGetIsMoveAllowedTile((short)ItemPositions[j].x, (short)ItemPositions[j].y) != FALSE)
					{

						if (g_ev.Is(EventID::DropBooster))
						{
							if (iItemIDs[j] == 3052) //contrib
							{
								pItem->m_dwCount = RollDice(8000, 15000);
							}
							else if (iItemIDs[j] == 3055) //maj
							{
								pItem->m_dwCount = RollDice(600, 1000);
							}
							else if (iItemIDs[j] == 3054) //rep
							{
								pItem->m_dwCount = RollDice(10, 30);
							}
							else if (iItemIDs[j] == 3053) //coin
							{
								pItem->m_dwCount = RollDice(10, 15);
							}
							else
							{
								if (iItemIDs[j] == 90) pItem->m_dwCount = iDice(5000, 20000);
								else pItem->m_dwCount = dwCount;
							}
						}
						else
						{
							if (iItemIDs[j] == 3052) //contrib
							{
								pItem->m_dwCount = RollDice(3000, 10000);
							}
							else if (iItemIDs[j] == 3055) //maj
							{
								pItem->m_dwCount = RollDice(300, 500);
							}
							else if (iItemIDs[j] == 3054) //rep
							{
								pItem->m_dwCount = RollDice(5, 20);
							}
							else if (iItemIDs[j] == 3053) //coin
							{
								pItem->m_dwCount = RollDice(2, 7);
							}
							else
							{
								if (iItemIDs[j] == 90) pItem->m_dwCount = iDice(10, 15000);
								else pItem->m_dwCount = dwCount;
							}
						}



						pItem->m_sTouchEffectType = DEF_ITET_ID;
						pItem->m_sTouchEffectValue1 = iDice(1, 100000);

						// Anti Dup System
						SYSTEMTIME SysTime;
						char cTemp[256];
						GetLocalTime(&SysTime);
						ZeroMemory(cTemp, sizeof(cTemp));
						pItem->m_sTouchEffectValue2 = iDice(1, 100000);
						wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
						pItem->m_sTouchEffectValue2 = atoi(cTemp);

						ZeroMemory(cTemp, sizeof(cTemp));
						wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
						pItem->m_sTouchEffectValue3 = atoi(cTemp);

						m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem((short)ItemPositions[j].x, (short)ItemPositions[j].y, pItem);
						pItem->m_dwTimeDrop = timeGetTime();
						SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
							(short)ItemPositions[j].x, (short)ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
						_bItemLog(DEF_ITEMLOG_NEWGENDROP, NULL, m_pNpcList[iNpcH]->m_cNpcName, pItem);
						pItem = NULL;

					}
					else {
						delete pItem;
						pItem = NULL;

					}
				}
			}
			else {

				//pItem = new class CItem;
				if (iItemID == CONTRIB_SMALL)
				{
					iItemID = 3052;
				}
				else if (iItemID == MAJ_SMALL)
				{
					iItemID = 3055;
				}
				else if (iItemID == REP_SMALL)
				{
					iItemID = 3054;
				}

				if (_bInitItemAttr(pItem, iItemID) != FALSE) {

					if (iItemID == 3052) //contrib
					{
						pItem->m_dwCount = RollDice(500, 750);
					}
					else if (iItemID == 3055) //maj
					{
						pItem->m_dwCount = RollDice(5, 20);
					}
					else if (iItemID == 3054) //rep
					{
						pItem->m_dwCount = RollDice(1, 3);
					}
					else
					{
						pItem->m_dwCount = dwCount;
					}

					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 10000);

					// Anti Dup System
					SYSTEMTIME SysTime;
					char cTemp[256];
					GetLocalTime(&SysTime);
					ZeroMemory(cTemp, sizeof(cTemp));
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
					pItem->m_sTouchEffectValue2 = atoi(cTemp);

					ZeroMemory(cTemp, sizeof(cTemp));
					wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
					pItem->m_sTouchEffectValue3 = atoi(cTemp);

					m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem);
					pItem->m_dwTimeDrop = timeGetTime();

					SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
						m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY,
						pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

					_bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
				}
				else {
					delete pItem;
					pItem = NULL;
				}
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: DeleteNpc");
	}
#endif

	delete m_pNpcList[iNpcH];
	m_pNpcList[iNpcH] = NULL;
}

BOOL cCoreServer::bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
	int iItemSpreadType, int iSpreadRange, int *iItemIDs, POINT *BasePos, int *iNumItem)
{
#ifdef DEF_DEBUG
	try {
#endif
		int		iProb = 100;
		float	fProb, fProbA, fProbB, fProbC;
		int		iItemID;
		int		iNum = 0;

		for (int i = 0; i < iMax; i++) {
			if (i > iMin) iProb = iProbability;

			fProb = (float)((100 - iProb) / 10.0);
			if (fProb < 1.0) fProb = 1.0;

			fProbA = fProbB = fProbC = fProb;

			fProbA = fProbA * 8.0f;
			fProbB = fProbB * 4.0f;
			fProbC = fProbC;

			iItemID = 0;

			switch (sNpcType) {
			case 66: // Wyvern

				switch (iDice(1, 3)) {
				case 1: iItemID = 762; break;	// GiantBattleHammer
				case 2: iItemID = 616; break;	// DemonSlayer
				case 3: iItemID = 1780; break;	// DeathWand(MS.30)

				default:


					switch (iDice(1, 2500)) {
					case 1: iItemID = 3852; break;	// HasteManual
					case 2: iItemID = 618; break;	// DarkElfBow

					case 3: iItemID = 1340; break;	// RingofPowermage
					case 4: iItemID = 1341; break;	// RingofPowerwar
					case 5: iItemID = 1342; break;	// NeckofPowerwar
					case 6: iItemID = 1343; break;	// NeckofPowermage

					default:

						switch (iDice(1, 500)) {
						case 1: iItemID = 847; break;	// DarkExecutor
						case 2: iItemID = 291; break;	// MagicWand(MS30-LLF)
						case 3: iItemID = 860; break;	// NecklaceOfXelima
						case 4: iItemID = 631; break;	// RingoftheAbaddon
						case 5: iItemID = 734; break;	// RingofArcmage

						case 6: iItemID = 650; break;	// ZemstoneofSacrifice
						case 7: iItemID = 907; break;	// MagicNecklace(RM30)
						case 8: iItemID = 903; break;	// MagicNecklace(DF+30)

						case 9: iItemID = 382; break;	// BlueFireManual
						case 10: iItemID = 614; break;	// SwordofIceElemental
						case 11: iItemID = 638; break;	// KnecklaceOfIcePro
						case 12: iItemID = 381; break;	// IceStormManual (más raro)
						case 13: iItemID = 337; break;	//RubyRing
						case 14: iItemID = 335; break;	// EmeraldRing
						case 15: iItemID = 334; break;	// LuckyGoldRing

						default:

							switch (iDice(1, 10)) {
							case 1: iItemID = 273; break;	// InviPotion
							case 2: iItemID = 840; break;	// PowerRedPotion
							case 3: iItemID = 841; break;	// PowerBluePotion
							case 4: iItemID = 300; break;	// MagicNecklace(RM10)
							case 5: iItemID = 311; break;	// MagicNecklace(DF+10)
							case 6: iItemID = 308; break;	// MagicNecklace(MS10)
							case 7: iItemID = 305; break;	// MagicNecklace(DM+1)
							case 8: iItemID = 259; break;	// MagicWand(M.Shield)
							case 9: iItemID = 636; break;	// RingofGrandMage
							case 10: iItemID = 492; break;	// BloodRapier
							}
							break;
						}
						break;
					}
					break;
				}
				break;
			case 73: // Fire-Wyvern
				switch (iDice(1, 3500)) {
				case 1: iItemID = 762; break;	// GiantBattleHammer
				case 2: iItemID = 616; break;	// DemonSlayer
				case 3: iItemID = 1780; break;	// DeathWand(MS.30)

				default:

					switch (iDice(1, 2500)) {
					case 1: iItemID = 3852; break;	// HasteManual
					case 2: iItemID = 618; break;	// DarkElfBow

					case 3: iItemID = 1340; break;	// RingofPowermage
					case 4: iItemID = 1341; break;	// RingofPowerwar
					case 5: iItemID = 1342; break;	// NeckofPowerwar
					case 6: iItemID = 1343; break;	// NeckofPowermage

					default:

						switch (iDice(1, 500)) {
						case 1: iItemID = 847; break;	// DarkExecutor
						case 2: iItemID = 291; break;	// MagicWand(MS30-LLF)
						case 3: iItemID = 860; break;	// NecklaceOfXelima
						case 4: iItemID = 631; break;	// RingoftheAbaddon
						case 5: iItemID = 734; break;	// RingofArcmage

						case 6: iItemID = 650; break;	// ZemstoneofSacrifice
						case 7: iItemID = 907; break;	// MagicNecklace(RM30)
						case 8: iItemID = 903; break;	// MagicNecklace(DF+30)

						case 9: iItemID = 382; break;	// HealSpotManual (más raro)
						case 10: iItemID = 381; break;	// IceStormManual (más raro)
						case 11: iItemID = 273; break;	// InviPotion
						case 12: iItemID = 300; break;	// MagicNecklace(RM10)
						case 13: iItemID = 20; break;	// Excaliber
						case 14: iItemID = 381; break;	// MassFireStrikeManual
						case 15: iItemID = 382; break;	// BloodyShoWaveManual
						case 16: iItemID = 311; break;	// MagicNecklace(DF+10)
						case 17: iItemID = 337; break;	//RubyRing
						case 18: iItemID = 335; break;	// EmeraldRing
						case 19: iItemID = 334; break;	// LuckyGoldRing

						default:
							switch (iDice(1, 10)) {
							case 1: iItemID = 492; break;	// BloodRapier
							case 2: iItemID = 490; break;	// BloodSword
							case 3: iItemID = 491; break;	// BloodAxe
							case 4: iItemID = 308; break;	// MagicNecklace(MS10)
							case 5: iItemID = 305; break;	// MagicNecklace(DM+1)
							case 6: iItemID = 636; break;	// RingofGrandMage
							case 7: iItemID = 735; break;	// RingofDragonpower
							case 8: iItemID = 638; break;	// KnecklaceOfFirePro
							case 9: iItemID = 645; break;	// KnecklaceOfEfreet
							case 10: iItemID = 259; break;	// MagicWand(M.Shield)
							}
							break;
						}
						break;
					}
					break;
				}
				break;

			case 81: // Abaddon
				switch (iDice(1, 200)) {
				case 1: iItemID = 382; break;	// HealSpotManual (más raro)
				case 2: iItemID = 381; break;	// IceStormManual (más raro)
				case 3: iItemID = 20; break;	// Excaliber
				default:
					switch (iDice(1, 10)) {
					case 1: iItemID = 492; break;	// BloodRapier
					case 2: iItemID = 490; break;	// BloodSword
					case 3: iItemID = 491; break;	// BloodAxe
					case 4: iItemID = 308; break;	// MagicNecklace(MS10)
					case 5: iItemID = 305; break;	// MagicNecklace(DM+1)
					case 6: iItemID = 636; break;	// RingofGrandMage
					case 7: iItemID = 735; break;	// RingofDragonpower
					case 8: iItemID = 638; break;	// KnecklaceOfFirePro
					case 9: iItemID = 645; break;	// KnecklaceOfEfreet
					case 10: iItemID = 259; break;	// MagicWand(M.Shield)
					}
					break;
				}
				break;
			}
			// Gold 
			if (iItemID == 0) iItemID = 90; // Gold
			if (iItemID != 0) {
				// item id
				iItemIDs[iNum] = iItemID;

				// item position
				switch (iItemSpreadType) {
				case DEF_ITEMSPREAD_RANDOM:
					BasePos[iNum].x = sBaseX + iSpreadRange - iDice(1, iSpreadRange * 2);
					BasePos[iNum].y = sBaseY + iSpreadRange - iDice(1, iSpreadRange * 2);
					break;

				case DEF_ITEMSPREAD_FIXED:
					BasePos[iNum].x = sBaseX + ITEMSPREAD_FIEXD_COORD[iNum][0];
					BasePos[iNum].y = sBaseY + ITEMSPREAD_FIEXD_COORD[iNum][1];
					break;
				}
				iNum++;
			}
		} // for

		*iNumItem = iNum;
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: bGetMultipleItemNamesWhenDeleteNpc");
	}
#endif
	return TRUE;
}

int cCoreServer::getRareItem(short npctype, short lastType)
{
	return lastType;

	switch (npctype)
	{
	case 59: //ettin
	{
		if (g_ev.Is(EventID::DropBooster))
		{
			int iDado = RollDice(1, 50000);
			if (iDado == 25000)
			{
				PutLogList("GiantBattleHammer Dropeado.");
				return 762; //GiantBattleHammer
			}
			else return lastType;
		}
		else
		{
			int iDado = RollDice(1, 100000);
			if (iDado == 50000)
			{
				PutLogList("GiantBattleHammer Dropeado.");
				return 762; //GiantBattleHammer
			}
			else return lastType;
		}

		break;
	}

	case 50: //Tiger worm
	{
		int iDado = RollDice(1, 80000);
		if (iDado == 40000)
		{
			PutLogList("MerienShield Dropeado.");
			return lastType; //MerienShield 620
		}
		else return lastType;

		break;
	}

	case 108://HellDeath
	{
		int iDado = RollDice(1, 2);
		switch (iDado)
		{
		case 1:
		{
			int iDado2 = RollDice(1, 120000);
			if (iDado2 == 60000)
			{
				PutLogList("Medu Neck Dropeado.");
				return lastType; //NecklaceOfMedusa 641
			}
			else return lastType;
			break;
		}
		case 2:
		{
			int iDado2 = RollDice(1, 120000);
			if (iDado2 == 60000)
			{
				PutLogList("Medu Sword Dropeada.");
				return lastType; //SwordOfMedusa 613
			}
			else return lastType;
			break;
		}
		default:
			break;
		}
		break;
	}

	default:
		return lastType;
		break;
	}

	return lastType;
}

int cCoreServer::DropRollDice(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

bool cCoreServer::isBossNpc(char * npcname)
{
	return strcmp(npcname, "Hellclaw") == 0 ||
		strcmp(npcname, "Tigerworm") == 0;
}

void cCoreServer::handleDropRareItem(int client, char* npcname) {
	auto p = m_pClientList[client];
	if (!p) return;

	if (isBossNpc(npcname))
	{
		rollBossDrop(client, npcname);
	}
	else if (g_ev.Is(EventID::DropMadness))
	{
		rollDropInDropMadness(client, npcname);
	}
	else if (g_ev.Is(EventID::DropBooster))
	{
		rollDropInBooster(client, npcname);
	}
	else
	{
		rollNormalDrop(client, npcname);
	}
}

void cCoreServer::rollBossDrop(int client, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{
		dado = DropRollDice(1, 1000);
	}
	else
	{
		dado = DropRollDice(1, 1000);
	}

	if (dado <= 500) {
		DropRareItem(client, npcname, drop_legendary);
	}

}


void cCoreServer::CreateCommonDropItem(int client, char* itemname, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
		ShowClientMsg(client, "You don't have enough space in your drop inventory.");
		return;
	}

	char cmsg[200];
	const char* dropType = getDropTypeString(drop_lesscommon);
	wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, 1, itemname);

	SendDropLogMsg(client, cmsg, drop_lesscommon);
	PutLogList(cmsg);

	add_rare_drop(client, (char*)itemname, 1, 0);
}

std::string cCoreServer::getCommonItem()
{
	std::vector<std::string> items = {
		"SapphireRing",
		"PlatinumRing",
		"LuckyGoldRing",
		"RubyRing",
		"EmeraldRing",
		"RingofWizard",
		"RingofMage"
	};

	// Crear un generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, items.size() - 1);

	// Devolver un elemento aleatorio
	return items[dis(gen)];
}

void cCoreServer::rollDropInBooster(int client, char* npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;

	if (p->m_bLuck)
	{
		dado = DropRollDice(1, DropRateBooster - 20000);
	}
	else
	{
		dado = DropRollDice(1, DropRateBooster);
	}

	if (dado <= 7) {
		int dado2 = DropRollDice(1, 2);
		if (dado2 == 1)
		{
			DropRareItem(client, npcname, drop_legendary);
		}
	}
	else if (dado <= 30) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 120) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 380) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 1000) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 600);
		if (dadito == 200)
		{
			if (string(npcname) != "Dummy" &&  string(npcname) != "Attack-Dummy")
				CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}

void cCoreServer::rollDropInDropMadness(int client, char * npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{
		dado = DropRollDice(1, DropRateMadness - 20000);
	}
	else
	{
		dado = DropRollDice(1, DropRateMadness);
	}

	if (dado <= 8) {
		DropRareItem(client, npcname, drop_legendary);
	}
	else if (dado <= 30) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 120) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 380) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 1000) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 600);
		if (dadito == 200)
		{
			if (string(npcname) != "Dummy" &&  string(npcname) != "Attack-Dummy")
				CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}

void cCoreServer::rollNormalDrop(int client, char * npcname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = 0;
	if (p->m_bLuck)
	{
		dado = DropRollDice(1, DropRateNormal - 20000);
	}
	else
	{
		dado = DropRollDice(1, DropRateNormal);
	}

	if (dado <= 7) {
		int dado2 = DropRollDice(1, 2);
		if (dado2 == 1)
		{
			DropRareItem(client, npcname, drop_legendary);
		}
	}
	else if (dado <= 18) {
		DropRareItem(client, npcname, drop_epic);
	}
	else if (dado <= 70) {
		DropRareItem(client, npcname, drop_rare);
	}
	else if (dado <= 300) {
		DropRareItem(client, npcname, drop_lesscommon);
	}
	else if (dado <= 800) {
		DropRareItem(client, npcname, drop_common);
	}
	else
	{
		int dadito = DropRollDice(1, 600);
		if (dadito == 200)
		{
			if (string(npcname) != "Dummy" &&  string(npcname) != "Attack-Dummy")
				CreateCommonDropItem(client, (char*)getCommonItem().c_str(), npcname);
		}
	}
}

void cCoreServer::DropRareItem(int client, char* npcname, short dropmode) {
	auto p = m_pClientList[client];
	if (!p) return;

	NPCDropInfo npcInfo = getNPCDropInfo(npcname);
	if (npcInfo.npcName == nullptr) return;

	int multiplier = getMultiplierForDropMode(dropmode);

	if (dropmode == drop_legendary) {
		handleLegendaryDrop(client, npcname, npcInfo);
		return;
	}

	int diceRange = 0;

	// Agregar materiales si existen y baseMaterials > 0
	auto it = npcMaterials.find(npcInfo.npcName);
	if (it != npcMaterials.end() && npcInfo.baseMaterials > 0) {
		diceRange += static_cast<int>(it->second.size());
	}

	// Agregar ítems normales según los valores base
	if (npcInfo.baseContrib > 0) diceRange++; // ContribBall
	if (npcInfo.baseMajs > 0) diceRange++;    // MajBall
	if (npcInfo.baseFragments > 0) diceRange += 11; // Fragmentos restantes

	if (diceRange <= 0) return; // Nada para dropear

	int dado = DropRollDice(1, diceRange);
	DropItem dropItem = getDropItem(dado, npcInfo, multiplier, dropmode);

	if (dropItem.count > 0)
		CreateDropItem(client, dropItem.name, dropItem.count, dropItem.attribute, dropmode, npcname);
}

NPCDropInfo cCoreServer::getNPCDropInfo(const char* npcname) {
	if (strcmp(npcname, "Orc") == 0) {
		return{ "Orc", 10, 3, 0, 2, {
			{ "ContribBall", 3000, 0 },
			{ "MajBall", 1000, 0 },
			{ "KnecklaceOfFirePro", 1, 0 }

		} };
	}
	else if (strcmp(npcname, "Skeleton") == 0) {
		return{ "Skeleton", 10, 3, 0, 2, {
			{ "ContribBall", 3000, 0 },
			{ "MajBall", 1000, 0 }
		} };
	}
	else if (strcmp(npcname, "Scorpion") == 0) {
		return{ "Scorpion", 10, 3, 0, 2, {
			{ "ContribBall", 3000, 0 },
			{ "MajBall", 1000, 0 }
		} };
	}
	else if (strcmp(npcname, "Unicorn") == 0) {
		return{ "Unicorn", 40, 5, 0, 2, {
			{ "EtherealEmerald", 1, 0 },
			{ "KnecklaceOfLightPro", 1, 0 },
			{ "MerienPlateMailM", 1, 0 },
			{ "MerienPlateMailW", 1, 0 },
			{ "MerienChainMailM", 1, 0 },
			{ "MerienChainMailW", 1, 0 },
			{ "KnecklaceOfAirEle", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Stone-Golem") == 0) {
		return{ "Stone-Golem", 40, 5, 0, 2, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "NecklaceOfStoneGol", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Clay-Golem") == 0) {
		return{ "Clay-Golem", 40, 5, 0, 2, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "NecklaceOfStoneGol", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Troll") == 0) {
		return{ "Troll", 50, 8, 3, 2, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "RingofOgrepower", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Frost") == 0) {
		return{ "Frost", 50, 8, 3, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },

			{ "StrongShards", 40000, 0 },
			{ "HitProbFragments", 20000, 0 },
			{ "PaFragments", 20000, 0 },
			{ "MaFragments", 20000, 0 },
			{ "MRFragments", 20000, 0 },
			{ "DRFragments", 20000, 0 },

			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },

			{ "DeathWand(MS.30)", 1, 0 },

			{ "LightingWand", 1, 0 },

			{ "RingofArcmage", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "KnecklaceOfIcePro", 1, 0 },
			//{ "KnecklaceOfIceEle", 1, 0 },
			{ "RingOfGrandMage", 1, 0 },
			{ "IceStormManual", 1, 0 },
			{ "MassFireStrikeManual", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Ettin") == 0) {
		return{ "Ettin", 50, 8, 3, 0, {
			{ "RingofDemonpower", 1, 0 },

			{ "StrongShards", 40000, 0 },
			{ "HitProbFragments", 20000, 0 },
			{ "PaFragments", 20000, 0 },
			{ "MaFragments", 20000, 0 },
			{ "MRFragments", 20000, 0 },
			{ "DRFragments", 20000, 0 },

			{ "GiantBattleHammer", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },
			{ "RingofPowerwar", 1, 0 },
			
			{ "RingoftheAbaddon", 1, 0 },
			{ "NecklaceOfXelima", 1, 0 },
			{ "MagicNecklace(DF+30)", 1, 0 },

			{ "BloodSword", 1, 0 },
			{ "BloodSwordAxe", 1, 0 },
			{ "BloodSwordRapier", 1, 0 },
			{ "KnecklaceOfEfreet", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Cyclops") == 0) {
		return{ "Cyclops", 50, 8, 3, 2, {
			{ "KnecklaceOfFirePro", 1, 0 },
			{ "KnecklaceOfLightPro", 1, 0 },

			{ "StrongShards", 40000, 0 },
			{ "HitProbFragments", 20000, 0 },
			{ "PaFragments", 20000, 0 },
			{ "MaFragments", 20000, 0 },
			{ "MRFragments", 20000, 0 },
			{ "DRFragments", 20000, 0 },

			{ "DeathWand(MS.30)", 1, 0 },

			{ "MagicWand(MS30-LLF)", 1, 0 },
			{ "RingofArcmage", 1, 0 },
			{ "MagicNecklace(RM30)", 1, 0 },

			{ "RingofPowermage", 1, 0 },
			{ "NeckofPowermage", 1, 0 },

			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "IceStormManual", 1, 0 },
			{ "MassFireStrikeManual", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Orge") == 0) {
		return{ "Orge", 50, 8, 3, 2, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "RingofOgrepower", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "WereWolf") == 0) {
		return{ "WereWolf", 50, 8, 3, 2, {
			{ "MerienPlateMailM", 1, 0 },
			{ "MerienPlateMailW", 1, 0 },

			{ "DemonSlayer", 1, 0 },

			{ "StrongShards", 40000, 0 },
			{ "HitProbFragments", 20000, 0 },
			{ "PaFragments", 20000, 0 },
			{ "MaFragments", 20000, 0 },
			{ "MRFragments", 20000, 0 },
			{ "DRFragments", 20000, 0 },
			
			{ "LightingBlade", 1, 0 },
			{ "NeckofPowerwar", 1, 0 },
			{ "RingofPowerwar", 1, 0 },

			{ "RingoftheAbaddon", 1, 0 },
			{ "NecklaceOfXelima", 1, 0 },
			{ "MagicNecklace(DF+30)", 1, 0 },
			
			{ "MerienChainMailM", 1, 0 },
			{ "MerienChainMailW", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Hellbound") == 0) {
		return{ "Hellbound", 50, 8, 3, 2, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}
	else if (strcmp(npcname, "Demon") == 0) {
		return{ "Demon", 80, 10, 3, 2, {
			{ "RingofDemonpower", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Liche") == 0) {
		return{ "Liche", 80, 10, 3, 0, {
			{ "NecklaceOfLiche", 1, 0 },
			{ "RingOfGrandMage", 1, 0 },
			{ "MassFireStrikeManual", 1, 0 },
			{ "BloodyShoWaveManual", 1, 0 }
		} };
	}

	//news

	else if (strcmp(npcname, "Zombie") == 0) {
		return{ "Zombie", 80, 10, 2, 0, {
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "ContribBall", 3000, 0 },
			{ "MajBall", 1000, 0 }
		} };
	}
	else if (strcmp(npcname, "Dark-Elf") == 0) {
		return{ "Dark-Elf", 80, 10, 3, 0, {
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "MagicNecklace(RM+10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}
	else if (strcmp(npcname, "Rudolph") == 0) {
		return{ "Rudolph", 80, 10, 3, 0, {
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "MagicNecklace(RM+10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "NecklaceofFireProtection", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "DireBoar") == 0) {
		return{ "DireBoar", 80, 10, 3, 0, {
			{ "NecklaceOfLiche", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "MagicNecklace(RM+10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "RingOfGrandMage", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Ice-Golem") == 0) {
		return{ "Ice-Golem", 80, 10, 2, 0, {
			{ "MagicWand(M.Shield)", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "MagicNecklace(RM+10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "NecklaceofIceProtection", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Gagoyle") == 0) {
		return{ "Gagoyle", 80, 10, 3, 0, {
			{ "BloodyShockWaveManual", 1, 0 },
			{ "RingoftheXelima", 1, 0 },
			{ "Excalibur", 1, 0 },
			{ "XelimaRapier", 1, 0 },
			{ "XelimaBlade", 1, 0 },
			{ "XelimaAxe", 1, 0 },
			{ "RingofDemonpower", 1, 0 },
			{ "RingofDragonPower", 1, 0 }
		} };
	}

	else if (strcmp(npcname, "Giant-Frog") == 0) {
		return{ "Giant-Frog", 80, 10, 2, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Cannibal-Plant") == 0) {
		return{ "Cannibal-Plant", 80, 10, 3, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Stalker") == 0) {
		return{ "Stalker", 80, 10, 3, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "RingofDemonpower", 1, 0 },
			{ "RingofDragonPower", 1, 0 }
		} };
	}

	else if (strcmp(npcname, "Beholder") == 0) {
		return{ "Beholder", 80, 10, 3, 0, {
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 }
		} };
	}

	else if (strcmp(npcname, "Giant-Ant") == 0) {
		return{ "Giant-Ant", 80, 10, 1, 0, {
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "ContribBall", 10000, 0 },
			{ "MajBall", 4000, 0 }
		} };
	}
	else if (strcmp(npcname, "Giant-Plant") == 0) {
		return{ "Giant-Plant", 80, 10, 3, 0, {
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "ContribBall", 15000, 0 },
			{ "MajBall", 5000, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 }
		} };
	}

	//new ends
	//
	else if (strcmp(npcname, "Hellclaw") == 0) {
		return{ "Hellclaw", 120, 0, 5, 0, {
			{ "MagicWand(M.Shield)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "RingOfMage", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			{ "RingofMage", 1, 0 },
			{ "MagicNecklace(RM10)", 1, 0 },
			{ "RingofOgrepower", 1, 0 },
			{ "RingofDemonpower", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "RingofWizard", 1, 0 },
			{ "EmeraldRing", 1, 0 },
			{ "RingofGrandMage", 1, 0 },
			{ "ResurWand(MS.10)", 1, 0 },
			{ "SwordofIceElemental", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "RubyRing", 1, 0 },
			//{ "KnecklaceOfIceEle", 1, 0 },
			{ "KnecklaceOfIcePro", 1, 0 }
		} };
	}
	else if (strcmp(npcname, "Tigerworm") == 0) {
		return{ "Tigerworm", 120, 0, 5, 0, {
			{ "XelimaBlade", 1, 0 },
			{ "XelimaRapier", 1, 0 },
			{ "XelimaAxe", 1, 0 },
			{ "KnecklaceOfIcePro", 1, 0 },
			{ "NeckofFireProtec", 1, 0 },
			{ "MagicNecklace(RM10)", 1, 0 },
			{ "MagicWand(M.Shield)", 1, 0 },
			{ "RingOfMage", 1, 0 },
			{ "RingofWizard", 1, 0 },
			{ "RubyRing", 1, 0 },
			{ "RingofGrandMage", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "EmeraldRing", 1, 0 },
			{ "MagicNecklace(DF+10)", 1, 0 },
			///{ "KnecklaceOfIceEle", 1, 0 },
			{ "LuckyGoldRing", 1, 0 },
			{ "MagicNecklace(MS10)", 1, 0 },
			{ "MagicNecklace(DM+1)", 1, 0 },
			{ "RingofDemonpower", 1, 0 },
			{ "RingoftheXelima", 1, 0 },
			{ "IceStormManual", 1, 0 },
			{ "SwordofIceElemental", 1, 0 },
			{ "ResurWand(MS.30)", 1, 0 },
			{ "BloodRapier", 1, 0 },
			{ "Excaliber", 1, 0 },
			{ "BloodAxe", 1, 0 }
		} };
	}

	return{ nullptr, 0, 0, 0, 0, {} };
}

int cCoreServer::getMultiplierForDropMode(short dropmode) {
	switch (dropmode) {
	case drop_common: return 1;
	case drop_lesscommon: return 2;
	case drop_rare: return 3;
	case drop_epic: return 20;
	default: return 1;
	}
}
DropItem cCoreServer::getDropItem(int dado, const NPCDropInfo& npcInfo, int multiplier, short dropmode) {
	auto it = npcMaterials.find(npcInfo.npcName);
	bool hasMaterials = (it != npcMaterials.end());

	std::vector<const char*> combinedItems;
	std::vector<int> combinedCounts;

	// Agregar materiales si existen
	if (hasMaterials) {
		const std::vector<std::string>& materials = it->second;
		for (const auto& mat : materials) {
			combinedItems.push_back(mat.c_str());
			combinedCounts.push_back(npcInfo.baseMaterials * multiplier);
		}
	}

	// Agregar ContribBall si corresponde
	if (npcInfo.baseContrib > 0) {
		combinedItems.push_back("ContribBall");
		combinedCounts.push_back(npcInfo.baseContrib * multiplier);
	}

	// Agregar MajBall si corresponde
	if (npcInfo.baseMajs > 0) {
		combinedItems.push_back("MajBall");
		combinedCounts.push_back(npcInfo.baseMajs * multiplier);
	}

	// Agregar fragmentos si corresponde
	if (npcInfo.baseFragments > 0) {
		const char* fragments[] = {
			"LightShards", "StrongShards", "HitProbFragments", "DRFragments",
			"HPRecFragments", "SPRecFragments", "MPRecFragments", "MRFragments",
			"PaFragments", "MaFragments", "ExpFragments"
		};
		for (const char* frag : fragments) {
			combinedItems.push_back(frag);
			combinedCounts.push_back(npcInfo.baseFragments * multiplier);
		}
	}

	// Seguridad: prevenir acceso fuera de rango
	if (dado < 1 || dado > static_cast<int>(combinedItems.size())) {
		return{ "Nothing", 0, 0 }; // Fallback de emergencia
	}

	return{ combinedItems[dado - 1], combinedCounts[dado - 1], 0 };
}


void cCoreServer::handleLegendaryDrop(int client, const char* npcname, const NPCDropInfo& npcInfo) {
	int dado = RollDice(1, npcInfo.legendaryDrops.size());
	DropItem dropItem = npcInfo.legendaryDrops[dado - 1];
	CreateDropItem(client, dropItem.name, dropItem.count, dropItem.attribute, drop_legendary, npcname);
}

void cCoreServer::CreateDropItem(int client, const char* itemname, DWORD itemcount, DWORD attribute, short dropmode, const char* npcname) {
	auto p = m_pClientList[client];
	if (!p) return;

	if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
		ShowClientMsg(client, "You don't have enough space in your drop inventory.");
		return;
	}

	const char* dropType = getDropTypeString(dropmode);

	char cmsg2[250];
	wsprintf(cmsg2, "Character %s get %s drop %d %s from %s", p->m_cCharName, dropType, itemcount, itemname, npcname);
	PutLogList(cmsg2);

	char cmsg[200];
	wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, itemcount, itemname);

	if (/*dropmode == drop_epic || */dropmode == drop_legendary) {
		broadcastDropMessage(client, npcname, itemname, itemcount, dropType, dropmode);
	}

	SendDropLogMsg(client, cmsg, dropmode);


	add_rare_drop(client, (char*)itemname, itemcount, attribute);
}

const char* cCoreServer::getDropTypeString(short dropmode) {
	switch (dropmode) {
	case drop_common: return "Common";
	case drop_lesscommon: return "Less Common";
	case drop_rare: return "Rare";
	case drop_epic: return "Epic";
	case drop_legendary: return "Legendary";
	default: return "";
	}
}

void cCoreServer::broadcastDropMessage(int client, const char* npcname, const char* itemname, DWORD itemcount, const char* dropType, short dropmode) {
	auto p = m_pClientList[client];
	if (!p) return;

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", p->m_cCharName, dropType, itemcount, itemname, npcname);

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL && i != client) {
			SendDropLogMsg(i, cmsg, dropmode);
		}
	}
}

void cCoreServer::FakeBroadCastMessageHammer()
{

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalHammer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::FakeBroadCastMessageSB()
{

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalStormBringer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::FakeDropPlayer(char * charname, char itemname, char npcname)
{

	char cmsg[200];
	wsprintf(cmsg, "%s get %s drop %d %s from mob %s", "Sugar", "Legendary", 1, "EternalHammer", "Ettin");

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		if (m_pClientList[i] != NULL) {
			SendDropLogMsg(i, cmsg, drop_legendary);
		}
	}
}

void cCoreServer::CreateFakeDropItem(char* charname, char* itemname, char* npcname)
{
	int charIndex = -1;
	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != NULL)
		{
			if (string(m_pClientList[i]->m_cCharName) == charname)
			{
				charIndex = i;
				break;
			}
		}
	}

	if (charIndex != -1 && m_pClientList[charIndex])
	{
		auto p = m_pClientList[charIndex];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(charIndex, "You don't have enough space in your drop inventory.");
			return;
		}

		char cmsg[200];
		const char* dropType = getDropTypeString(drop_legendary);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, 1, itemname);

		broadcastDropMessage(charIndex, npcname, itemname, 1, dropType, drop_legendary);

		SendDropLogMsg(charIndex, cmsg, drop_legendary);
		PutLogList(cmsg);

		add_rare_drop(charIndex, (char*)itemname, 1, 0);
	}
}

void cCoreServer::handleDropByLevel(int client, short mode)
{
	auto p = m_pClientList[client];
	if (!p) return;

	string itemname = getMineDropName(mode);
	int count = getMineDropCount(mode);

	if (string(itemname) != "#" && count != -1) CreateMineralDrop(client, (char*)itemname.c_str(), mode, count);
}

char * cCoreServer::getMineDropName(short dropmode)
{
	switch (dropmode)
	{
	case drop_common:
	{
		int dado = RollDice(1, 6);
		switch (dado)
		{
		case 1: return "Iron"; break;
		case 2: return "Copper"; break;
		case 3: return "Tin"; break;
		case 4: return "Bronze"; break;
		case 5: return "Silver"; break;
		default:
			return "#";
			break;
		}
		break;
	}
	case drop_rare:
	{
		int dado = RollDice(1, 5);
		switch (dado)
		{
		case 1: return "Steel"; break;
		case 2: return "Platinum"; break;
		case 3: return "Obsidian"; break;
		case 4: return "Cobalt"; break;
		default:
			return "#";
			break;
		}
		break;
	}
	case drop_epic:
	{
		int dado = RollDice(1, 8);
		switch (dado)
		{
		case 1: return "Mithril"; break;
		case 2: return "Adamantite"; break;
		case 3: return "Orichalcum"; break;
		case 4: return "Titanium"; break;
		case 5: return "Aetherium"; break;
		case 6: return "Dragonite"; break;
		case 7: return "Darksteel"; break;
		case 8: return "Palladium"; break;
		default:
			return "#";
			break;
		}
		break;
	}
	case drop_legendary:
	{
		int dado = RollDice(1, 4);
		switch (dado)
		{
		case 1: return "Eclipsium"; break;
		case 2: return "Voidsteel"; break;
		case 3: return "Astronyx"; break;
		case 4: return "Netherite"; break;
		default:
			return "#";
			break;
		}
		break;
	}
	default: return "#"; break;
	}

	return "#";
}

int cCoreServer::getMineDropCount(short dropmode)
{
	switch (dropmode)
	{
	case drop_common:
	{
		int dado = RollDice(2, 5);
		return dado;
		break;
	}
	case drop_rare:
	{
		int dado = RollDice(2, 5);
		return dado;
		break;
	}
	case drop_epic:
	{
		int dado = RollDice(2, 5);
		return dado;
		break;
	}
	case drop_legendary:
	{
		int dado = RollDice(1, 2);
		return dado;
		break;
	}
	default: return -1; break;
	}

	return -1;
}

void cCoreServer::CreateMineralDrop(int client, char* itemname, short mode, int count)
{
	if (m_pClientList[client])
	{
		auto p = m_pClientList[client];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(client, "You don't have enough space in your drop inventory.");
			return;
		}

		char cmsg[200];
		const char* dropType = getDropTypeString(mode);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, count, itemname);
		SendDropLogMsg(client, cmsg, mode);

		if (mode == drop_legendary)
		{
			broadcastDropMessage(client, "Mine", itemname, count, dropType, mode);
		}

		add_rare_drop(client, (char*)itemname, count, 0);
	}
}


void cCoreServer::CreateFishingDrop(int client, char* itemname, short mode, int count)
{
	if (m_pClientList[client])
	{
		auto p = m_pClientList[client];
		if (!p) return;

		if (p->m_vDropItemList.size() == DEF_MAXDROPITEMS) {
			ShowClientMsg(client, "You don't have enough space in your drop inventory.");
			m_pClientList[client]->m_bFishing = false;
			ShowClientMsg(client, "Fishing cancelled.");
			send_objects_data();
			return;
		}

		char cmsg[200];
		const char* dropType = getDropTypeString(mode);
		wsprintf(cmsg, "%s Drop: You have received %d %s in Drops inventory.", dropType, count, itemname);
		SendDropLogMsg(client, cmsg, mode);

		if (mode == drop_legendary)
		{
			broadcastDropMessage(client, "Fishing", itemname, count, dropType, mode);
		}

		add_rare_drop(client, (char*)itemname, count, 0);
	}
}

void cCoreServer::NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType)
{
#ifdef DEF_DEBUG
	try {
#endif
		// Validaciones iniciales
		if (m_pNpcList[iNpcH] == NULL) return;
		if ((cAttackerType != DEF_OWNERTYPE_PLAYER) || (m_pNpcList[iNpcH]->m_bIsSummoned == TRUE)) return;

		// Configuración de probabilidades
		const int iItemprobability = 1500;
		const int iGoldProbMax = 25;
		const int iGoldProb2 = 2500;
		const int iSecondProb = 2500;

		// Lista de NPCs que no dropean items
		const short noDropTypes[] = { 1, 2, 3, 4, 5, 6, 21, 40, 64, 87, 89, 91, 125, 126, 127 };
		for (short type : noDropTypes) {
			if (m_pNpcList[iNpcH]->m_sType == type) return;
		}

		// Determinar nivel de generación
		int iGenLevel = GetNpcGenLevel(m_pNpcList[iNpcH]->m_sType);
		if (iGenLevel == 0) return;

		// Proceso principal de generación de drops
		if (iDice(1, 11000) >= iItemprobability) {
			BOOL SaveDrop = FALSE;
			CItem* pItem = nullptr;

			// 35% de chance de drop (35/100)
			if (iDice(1, 11000) <= iGoldProb2) {
				// 60% de que sea oro
				pItem = GenerateGoldDrop(m_pNpcList[iNpcH], m_pClientList[sAttackerH]);
			}
			else {
				// 40% de que sea un item
				if (iDice(1, 10000) <= iSecondProb) {
					// 90% de que sea un item estándar (pociones, etc.)
					pItem = GenerateStandardDrop(iNpcH);
				}
				else {
					// 10% de que sea un item especial (armas/armaduras)
					pItem = GenerateSpecialDrop(iGenLevel);
				}
			}

			// Si se generó un item, procesarlo
			if (pItem != nullptr) {
				ProcessGeneratedItem(pItem, iGenLevel, SaveDrop, m_pNpcList[iNpcH]);
			}
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: NpcDeadItemGenerator");
	}
#endif
}

int cCoreServer::GetNpcGenLevel(short npcType) {
	switch (npcType) {
	case 10: case 16: case 22: case 55: case 56: return 1;
	case 11: case 14: case 17: case 18: return 2;
	case 12: case 23: return 3;
	case 27: case 61: case 74: return 4;
	case 13: case 28: case 53: case 60: case 62: return 5;
	case 29: case 33: case 48: case 54: case 65: case 70: case 72: case 78: return 6;
	case 30: case 63: case 79: return 7;
	case 31: case 32: case 49: case 50: case 52: case 71: return 8;
	case 58: return 9;
	case 59: case 77: case 75: return 10;
	case 93: case 94: case 100: case 101: return 11;
	case 95: case 96: return 12;
	case 97: case 105: case 99: case 122: case 123: case 124: return 13;
	case 104: case 102: case 103: return 14;
	case 98: case 106: return 15;
	case 107: return 16;
	case 108: case 109: case 121: return 17;
	case 112: case 113: return 18;
	case 128: case 114: return 19;
	case 115: case 116: return 20;
	case 117: return 21;
	case 118: case 119: return 22;
	default: return 0;
	}
}

CItem* cCoreServer::GenerateGoldDrop(CNpc* pNpc, CClient* pAttacker) {
	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, "Gold") == FALSE) {
		delete pItem;
		return nullptr;
	}

	DWORD goldAmount = (DWORD)(iDice(1, (pNpc->m_iGoldDiceMax - pNpc->m_iGoldDiceMin)) + pNpc->m_iGoldDiceMin);

	if ((pAttacker != nullptr) && (pAttacker->m_iAddGold != NULL)) {
		double bonus = (double)pAttacker->m_iAddGold;
		goldAmount += (DWORD)((bonus / 100.0f) * (double)goldAmount);
	}

	// Reducir a 1/3 y asegurar mínimo 1
	goldAmount = max(1, goldAmount / 4);
	pItem->m_dwCount = goldAmount;

	return pItem;
}

CItem* cCoreServer::GenerateSpecialDrop(int iGenLevel) {
	int iItemID = 0;

	if (iDice(1, 10000) <= 4000) {
		if (iDice(1, 10000) <= 7000) {
			iItemID = GenerateMeleeWeapon(iGenLevel);
		}
		else {
			iItemID = GenerateMagicWand(iGenLevel);
		}
	}
	else {
		iItemID = GenerateArmor(iGenLevel);
	}

	if (iItemID == 0) return nullptr;

	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
		return nullptr;
	}

	return pItem;
}
void cCoreServer::ProcessGeneratedItem(CItem* pItem, int iGenLevel, BOOL& SaveDrop, CNpc* pNpc) {
	DWORD dwType = 0, dwValue = 0;
	char cColor = 0;
	int iResult = 0;

	// ========== ATAQUE ==========
	if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
		iResult = iDice(1, 80);
		if (iResult <= 9) { dwType = 6; cColor = 2; }
		else if (iResult <= 19) { dwType = 8; cColor = 3; }
		else if (iResult <= 29) { dwType = 1; cColor = 5; }
		else if (iResult <= 39) { dwType = 5; cColor = 1; }
		else if (iResult <= 49) { dwType = 3; cColor = 7; }
		else if (iResult <= 59) { dwType = 2; cColor = 4; }
		else if (iResult <= 69) { dwType = 7; cColor = 6; }
		else { dwType = 9; cColor = 8; }

		pItem->m_cItemColor = cColor;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 2;

		//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		// Secundario (30% chance)
		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 1200);
			if (iResult <= 299) dwType = 2;
			else if (iResult <= 599) dwType = 10;
			else if (iResult <= 899) dwType = 12;
			else dwType = 11;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 2;

			//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========== VARITAS ==========
	else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE) {
		dwType = 10;
		cColor = 5;
		pItem->m_cItemColor = cColor;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 2;

		//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 1200);
			if (iResult <= 299) dwType = 2;
			else if (iResult <= 599) dwType = 10;
			else if (iResult <= 899) dwType = 12;
			else dwType = 11;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 2;

			//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========== DEFENSA ==========
	else if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
		iResult = iDice(1, 1200);
		if (iResult <= 299) dwType = 8;
		else if (iResult <= 599) dwType = 6;
		else if (iResult <= 899) dwType = 11;
		else dwType = 12;

		iResult = iDice(1, 1900);
		if (iResult < 1150) dwValue = 1;
		else if (iResult < 1650) dwValue = 2;
		else dwValue = 2;

		//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;
		if ((pItem->m_sIDnum == 402) && (dwValue > 1)) dwValue = 1;

		pItem->m_dwAttribute = 0;
		pItem->m_dwAttribute |= (dwType << 20) | (dwValue << 16);

		if (iDice(1, 10000) >= 7000) {
			iResult = iDice(1, 2200);
			if (iResult <= 299) dwType = 3;
			else if (iResult <= 599) dwType = 1;
			else if (iResult <= 899) dwType = 5;
			else if (iResult <= 1199) dwType = 4;
			else if (iResult <= 1499) dwType = 6;
			else if (iResult <= 1799) dwType = 7;
			else if (iResult <= 1999) dwType = 8;
			else dwType = 9;

			iResult = iDice(1, 1900);
			if (iResult < 1150) dwValue = 1;
			else if (iResult < 1650) dwValue = 2;
			else dwValue = 2;

			//if (iGenLevel <= 2 && dwValue > 3) dwValue = 3;

			pItem->m_dwAttribute |= (dwType << 12) | (dwValue << 8);
		}
	}

	// ========= EXTRAS =========
	_AdjustRareItemValue(pItem);

	pItem->m_sTouchEffectType = DEF_ITET_ID;
	pItem->m_sTouchEffectValue1 = iDice(1, 100000);

	SYSTEMTIME SysTime;
	char cTemp[256];
	GetLocalTime(&SysTime);

	pItem->m_sTouchEffectValue2 = iDice(1, 100000);
	wsprintf(cTemp, "%02d%02d", (short)SysTime.wMonth, (short)SysTime.wDay);
	pItem->m_sTouchEffectValue2 = atoi(cTemp);

	wsprintf(cTemp, "%02d%02d%02d%d", (short)SysTime.wHour, (short)SysTime.wMinute, (short)SysTime.wSecond, (short)SysTime.wMilliseconds);
	pItem->m_sTouchEffectValue3 = atoi(cTemp);

	m_pMapList[pNpc->m_cMapIndex]->bSetItem(pNpc->m_sX, pNpc->m_sY, pItem);
	pItem->m_dwTimeDrop = timeGetTime();

	SendEventToNearClient_TypeB(CLIENT_COMMON_EVENT, CLIENT_COMMON_ITEMDROP, pNpc->m_cMapIndex, pNpc->m_sX, pNpc->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);

	_bItemLog(DEF_ITEMLOG_NEWGENDROP, NULL, NULL, pItem, SaveDrop);
}


int cCoreServer::GenerateMeleeWeapon(int iGenLevel) {
	switch (iGenLevel) {
	case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
		switch (iDice(1, 3)) {
		case 1: return 1;   // Dagger
		case 2: return 8;   // ShortSword
		case 3: return 59;  // LightAxe
		}
		break;
	case 2: // Skeleton, Orc, Orc-Mage, Scorpion, Zombie
		switch (iDice(1, 6)) {
		case 1: return 12;  // MainGauche
		case 2: return 15;  // Gradius
		case 3: return 65;  // SexonAxe
		case 4: return 62;  // Tomahoc
		case 5: return 23;  // Sabre
		case 6: return 31;  // Esterk
		}
		break;
	case 3: // Stone-Golem, Clay-Golem
		switch (iDice(1, 4)) {
		case 1: return 17;  // LongSword
		case 2: return 68;  // DoubleAxe
		case 3: return 23;  // Sabre
		case 4: return 31;  // Esterk
		}
		break;
	case 4: // Hellbound, Rudolph
		switch (iDice(1, 5)) {
		case 1: return 23;  // Sabre
		case 2: return 25;  // Scimitar
		case 3: return 28;  // Falchion
		case 4: return 31;  // Esterk
		case 5: return 34;  // Rapier
		}
		break;
	case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
		switch (iDice(1, 3)) {
		case 1: return 31;  // Esterk
		case 2: return 34;  // Rapier
		case 3: return 71;  // WarAxe
		}
		break;
	case 6: case 11: case 13: // Orge, WereWolf, Stalker, Dark-Elf, Ice-Golem / IceWorm, FireWorm / Wood Golem, Phamton, Butcher
		switch (iDice(1, 5)) {
		case 1: return 50;  // GreatSword
		case 2: return 46;  // Claymore
		case 3: return 31;  // Esterk
		case 4: return 34;  // Rapier
		case 5: return 54;  // Flamberge
		}
		break;
	case 7: case 15: // Liche, Frost / Yeti, IceLizard
		switch (iDice(1, 4)) {
		case 1: return 50;  // GreatSword
		case 2: return 54;  // Flameberge
		case 3: return 31;  // Esterk
		case 4: return 34;  // Rapier
		}
		break;
	case 8: case 12: case 14: case 18: // Demon, Unicorn / Red Demon, Blue Demon / Diablo, Satan / Hellhound, Helliche
		switch (iDice(1, 6)) {
		case 1: return 54;   // Flamberge
		case 2: return 560; // BattleAxe
		case 3: return 615; // GiantSword
		case 4: return 402; // Cape
		case 5: return 615; // GiantSword
		case 6: return 560; // BattleAxe
		}
		break;
	case 9: // MountainGiant
		switch (iDice(1, 6)) {
		case 1: return 23;   // Sabre
		case 2: return 25;   // Scimitar
		case 3: return 760;  // Hammer
		case 4: return 31;   // Esterk
		case 5: return 28;   // Falchion
		case 6: return 760;  // Hammer
		}
		break;
	case 10: // Ettin
		return (iDice(1, 2) == 1 ? 761 : 761); // BattleHammer
	case 20: // Hellbutcher, HellTroll
		switch (iDice(1, 6)) {
		case 1: return 54;   // Flamberge
		case 2: return 560;  // BattleAxe
		case 3: return 615;  // GiantSword
		case 4: return 402;  // Cape
		case 5: return 615;  // GiantSword
		case 6: return 560;  // BattleAxe
		}
		break;
	}
	return 0;
}

int cCoreServer::GenerateMagicWand(int iGenLevel) {
	switch (iGenLevel) {
	case 2: case 3: return 258; // MagicWand(MS0)
	case 4: case 5: case 6: return 257; // MagicWand(MS10)
	case 7: case 8: return 256; // MagicWand(MS20)
	default: return 0;
	}
}

int cCoreServer::GenerateArmor(int iGenLevel) {
	switch (iGenLevel) {
	case 1: // Slime, Giant-Ant, Amphis, Rabbit, Cat
		switch (iDice(1, 2)) { // Reducido a 2 opciones (sin botas)
		case 1: return 79;  // WoodShield
		case 2: return 80;  // LeatherShield
		}
		break;
	case 2: case 11: // Skeleton, Orc / IceWorm, FireWorm
		switch (iDice(1, 2)) { // Reducido a 2 opciones (sin botas)
		case 1: return 80;  // LeatherShield
		case 2: return 81;  // TargeShield
		}
		break;
	case 3: // Stone-Golem, Clay-Golem
		return 81; // TargeShield
	case 4: // Hellbound, Rudolph
		switch (iDice(1, 8)) { // Reducido a 8 opciones (sin BlondeShield)
		case 1: case 2: return 83;   // Hauberk(M)
		case 3: case 4: return 472;  // Hauberk(W)
		case 5: case 6: return 461;  // ChainHose(M)
		case 7: case 8: return 482;  // ChainHose(W)
		}
		break;
	case 5: // Cyclops, Troll, Beholder, Cannibal-Plant, DireBoar
		switch (iDice(1, 3)) { // Reducido a 3 opciones (sin IronShield si falla el dado)
		case 1: return (iDice(1, 4) <= 2 ? 455 : 475); // LeatherArmor(M/W)
		case 2: return (iDice(1, 4) <= 2 ? 454 : 472); // Hauberk(M/W)
		case 3: return (iDice(1, 4) <= 2 ? 461 : 482); // ChainHose(M/W)
		}
		break;
	case 6: case 13: case 18: case 20: // Orge, WereWolf / Wood Golem / Hellhound / Hellbutcher
		switch (iDice(1, 5)) { // Reducido a 5 opciones (sin ChainHose/Hauberk en caso 6)
		case 1: return (iDice(1, 4) <= 2 ? 476 : 456); // ChainMail(W/M)
		case 2: return (iDice(1, 4) <= 2 ? 458 : 478); // PlateMail(M/W)
		case 3: return 85; // LagiShield
		case 4:
			switch (iDice(1, 8)) {
			case 1: return 750; // Horned-Helm(M)
			case 2: return 751; // Wings-Helm(M)
			case 3: return 754; // Horned-Helm(W)
			case 4: return 755; // Wings-Helm(W)
			case 5: return 752; // Wizard-Cap(M)
			case 6: return 753; // Wizard-Hat(M)
			case 7: return 756; // Wizard-Cap(W)
			case 8: return 757; // Wizard-Hat(W)
			}
		case 5: return (iDice(1, 2) == 1 ? 458 : 478); // PlateMail(M/W) como opción extra
		}
		break;
	case 7: // Liche, Frost
		switch (iDice(1, 3)) { // Reducido a 3 opciones (sin Full-Helm)
		case 1: return (iDice(1, 2) == 1 ? 458 : 478); // PlateMail(M/W)
		case 2: return 86; // KnightShield
		case 3: return 87; // TowerShield
		}
		break;
	case 8: case 12: case 14: case 16: // Demon, Unicorn / Red Demon / Diablo / GrandEttin
	case 17: case 19: case 21: case 22: // HellDeath / DeathWyvern / MountDeath / Hellgoyle
		return 87; // Solo TowerShield (eliminadas capas y botas)
		break;
	}
	return 0;
}

CItem* cCoreServer::GenerateStandardDrop(int iNpcH) {
	int iItemID = 0;
	int iResult = iDice(1, 13000);

	if ((iResult >= 1) && (iResult <= 3000))          iItemID = 95;    // Green Potion
	else if ((iResult >= 3001) && (iResult <= 4000))  iItemID = 91;    // Red Potion
	else if ((iResult >= 4001) && (iResult <= 5500))  iItemID = 93;    // Blue Potion
	else if ((iResult >= 5501) && (iResult <= 6700))  iItemID = 390;   // Power Green Potion
	else if ((iResult >= 6701) && (iResult <= 8500))  iItemID = 92;    // Big Red Potion
	else if ((iResult >= 8501) && (iResult <= 8999))  iItemID = 94;    // Big Blue Potion
	else if ((iResult >= 9000) && (iResult <= 10500)) iItemID = (iDice(1, 2) == 1 ? 390 : 390); // Power Green Potion
	else if ((iResult >= 10501) && (iResult <= 11200)) {
		switch (iDice(1, 4)) {
		case 1: iItemID = 780; break; // Red Candy
		case 2: iItemID = 781; break; // Blue Candy
		case 3:
			switch (iDice(1, 3)) {
			case 1: iItemID = 780; break; // Red Candy
			case 2: iItemID = 781; break; // Blue Candy
			case 3: iItemID = 782; break; // Green Candy
			}
			break;
			/*case 4:
			switch (iDice(1, 2)) {
			case 1: iItemID = 656; break; // Xelima Stone
			case 2: iItemID = 657; break; // Merien Stone
			}
			break;*/
		case 4: iItemID = 390; break; // Zemstone of Sacrifice
		}
	}
	else if ((iResult >= 11201) && (iResult <= 13000)) {
		SYSTEMTIME SysTime;
		GetLocalTime(&SysTime);
		if (m_pNpcList[iNpcH]) {
			if (((short)SysTime.wMonth == 12) && (m_pNpcList[iNpcH]->m_sType == 61 || 55)) {
				switch (iDice(1, 3)) {
				case 1: iItemID = 780; break; // Red Candy
				case 2: iItemID = 781; break; // Blue Candy
				case 3: iItemID = 782; break; // Green Candy
				}
			}
		}
	}

	CItem* pItem = new CItem();
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
		return nullptr;
	}

	return pItem;
}
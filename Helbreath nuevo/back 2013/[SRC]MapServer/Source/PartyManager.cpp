// PartyManager.cpp: implementation of the PartyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../Header/PartyManager.h"

extern char G_cTxt[120];
extern void PutLogList(char * cMsg);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PartyManager::PartyManager()
{
	ClearParty();
}

PartyManager::~PartyManager()
{

}

void PartyManager::iCreateNewParty(int iClientH)
{
	m_stPartyInfo[0].iClientH = iClientH;
	iTotalMembers = 0;
}

BOOL PartyManager::bAddMember(int NewMemberID)
{
	if (iTotalMembers >= DEF_MAXPARTYMEMBERS) return FALSE;
	
	for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		if (m_stPartyInfo[i].iClientH == NULL) {
			m_stPartyInfo[i].iClientH = NewMemberID;

			iTotalMembers++;
			return TRUE;
		}
		else if (m_stPartyInfo[i].iClientH == NewMemberID) return FALSE;
	}
	return FALSE;
}

int PartyManager::iGetMemberID(int iClientH)
{
	if (iClientH == NULL) return -1;
	short MemberID = -1;

	for (int i = 0; i <= iTotalMembers; i++) {
		if (m_stPartyInfo[i].iClientH == iClientH) 
			MemberID = i;
	}
	return MemberID;
}

BOOL PartyManager::bRemoveMember(int iClientH)
{
	int i, j;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		if (m_stPartyInfo[i].iClientH == iClientH) {
			m_stPartyInfo[i].iClientH = NULL;
			iTotalMembers--;

			for (j = 0; j < DEF_MAXPARTYMEMBERS - 1; j++) {
				if ((m_stPartyInfo[j].iClientH == NULL) && (m_stPartyInfo[j + 1].iClientH != NULL)) {
					m_stPartyInfo[j].iClientH = m_stPartyInfo[j + 1].iClientH;
					m_stPartyInfo[j + 1].iClientH = NULL;
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}

void PartyManager::ClearParty()
{
	for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++)
		m_stPartyInfo[i].iClientH = NULL;
	
	iTotalMembers = 0;
}


BOOL PartyManager::bCheckPartyMember(int iClientH, int iPartyID, char *pName)
{
 int i, iRet;
 char * cp, cData[120];
 DWORD * dwp;
 WORD * wp;

	for (i = 1; i < DEF_MAXPARTY; i++)
	if ((m_stMemberNameList[i].m_iPartyID == iPartyID) && (strcmp(m_stMemberNameList[i].m_cName, pName) == 0)) {
		m_stMemberNameList[i].m_dwServerChangeTime = NULL;
		return TRUE;
	}

	return FALSE;
}

BOOL PartyManager::bGetPartyInfo(int iClientH, char * pName, int iPartyID)
{
 int i, iRet, iTotal;
 char * cp, cData[1024];
 DWORD * dwp;
 WORD * wp, * wpTotal;

	return bCheckPartyMember(iClientH, iPartyID, pName);

	/*iTotal = 0;
	for (i = 1; i < DEF_MAXPARTY; i++)
	if ((m_stMemberNameList[i].m_iPartyID == iPartyID) && (m_stMemberNameList[i].m_iPartyID != NULL)) {
		memcpy(cp, m_stMemberNameList[i].m_cName, 10);
		cp += 11;
		iTotal++;
	}
	
	*wpTotal = iTotal;
//	iRet = m_pGateCore->m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 20 + iTotal*11 +1);

	return TRUE;*/
}

void PartyManager::GameServerDown(int iClientH)
{
 int i;

	for (i = 0; i < DEF_MAXPARTY; i++)
	if (m_stMemberNameList[i].m_iIndex == iClientH) {
		//testcode
		//wsprintf(G_cTxt, "Removing Party member(%s) by Server down", m_stMemberNameList[i].m_cName);
		//PutLogList(G_cTxt);

		m_iMemberNumList[m_stMemberNameList[i].m_iPartyID]--;
		m_stMemberNameList[i].m_iPartyID  = NULL;
		m_stMemberNameList[i].m_iIndex    = NULL;
		m_stMemberNameList[i].m_dwServerChangeTime = NULL;
		ZeroMemory(m_stMemberNameList[i].m_cName, sizeof(m_stMemberNameList[i].m_cName));
	}
}

void PartyManager::SetServerChangeStatus(char *pName, int iPartyID)
{
 int i;

	for (i = 1; i < DEF_MAXPARTY; i++)
	if ((m_stMemberNameList[i].m_iPartyID == iPartyID) && (strcmp(m_stMemberNameList[i].m_cName, pName) == 0)) {
		m_stMemberNameList[i].m_dwServerChangeTime = timeGetTime();
		return;
	}
}

void PartyManager::CheckMemberActivity()
{
 int i;
 DWORD * dwp, dwTime = timeGetTime();
 char * cp, cData[120];
 WORD * wp;

	if ((dwTime - m_dwCheckMemberActTime) > 1000*2) {
		m_dwCheckMemberActTime = dwTime;
	} else return;

	/*for (i = 1; i < DEF_MAXPARTY; i++)
	if ((m_stMemberNameList[i].m_dwServerChangeTime != NULL) && ((dwTime - m_stMemberNameList[i].m_dwServerChangeTime) > 1000*20)) {
		bGetPartyInfo(iClientH, m_stMemberNameList[i].m_cName, m_stMemberNameList[i].m_iPartyID);
	}*/
}

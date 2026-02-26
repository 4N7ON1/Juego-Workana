// PartyManager.h: interface for the PartyManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMANAGER_H__ED26235F_8BA2_493A_AF52_397C25D4CF5C__INCLUDED_)
#define AFX_PARTYMANAGER_H__ED26235F_8BA2_493A_AF52_397C25D4CF5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define DEF_MAXPARTY			5000
#define DEF_MAXPARTYMEMBERS		15
#define DEF_MAXPARTYMEMBER		15


class PartyManager  
{
public:
	PartyManager();
	virtual ~PartyManager();

	void CheckMemberActivity();
	void SetServerChangeStatus(char * pName, int iPartyID);
	void GameServerDown(int iClientH);
	BOOL bGetPartyInfo(int iClientH, char * pName, int iPartyID);
	BOOL bCheckPartyMember(int iClientH, int iPartyID, char * pName);
	BOOL bRemoveMember(int iClientH);
	BOOL bAddMember(int NewMemberID);
	BOOL bDeleteParty(int iPartyID);
	void iCreateNewParty(int iClientH);

	int m_iMemberNumList[DEF_MAXPARTY];
	int iGetMemberID(int iClientH);

	void ClearParty();

	struct {
		int  m_iPartyID, m_iIndex;
		char m_cName[12];
		DWORD m_dwServerChangeTime;
	} m_stMemberNameList[DEF_MAXPARTY];

	struct {
		int iClientH;
		int MemberID;
	} m_stPartyInfo[DEF_MAXPARTYMEMBERS];

	int iTotalMembers;
	DWORD m_dwCheckMemberActTime;
};

#endif // !defined(AFX_PARTYMANAGER_H__ED26235F_8BA2_493A_AF52_397C25D4CF5C__INCLUDED_)

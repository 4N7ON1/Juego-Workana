// Msg.cpp: implementation of the CMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "../Header/Msg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsg::CMsg()
{
	m_pData  = NULL;
	m_dwSize = NULL;
}

CMsg::~CMsg()						   
{
	if (m_pData != NULL) delete []m_pData;
}

#define DEF_MSGBUFFERSIZE	200000//1

BOOL CMsg::bPut(char cFrom, char * pData, DWORD dwSize, int iIndex, char cKey)
{
	if (pData == NULL || dwSize < 1 || dwSize > DEF_MSGBUFFERSIZE)
		return FALSE;

	m_pData = new char [dwSize + 1];
	ZeroMemory(m_pData, dwSize + 1);
	memcpy(m_pData, pData, dwSize);

	m_dwSize = dwSize;
	m_cFrom  = cFrom;
	m_iIndex = iIndex;
	m_cKey   = cKey;

	return TRUE;
}

void CMsg::Get(char * pFrom, char * pData, DWORD * pSize, int * pIndex, char * pKey)
{
	*pFrom  = m_cFrom;
	memcpy(pData, m_pData, m_dwSize);
	*pSize  = m_dwSize;
	*pIndex = m_iIndex;
	*pKey   = m_cKey;
}

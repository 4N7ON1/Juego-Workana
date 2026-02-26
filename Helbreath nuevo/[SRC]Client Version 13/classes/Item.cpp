// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "../Headers/Item.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	ZeroMemory(m_cName, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
	m_dwAttribute = NULL;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;
}

CItem::~CItem()
{
	
}

bool CItem::IsShield()
{
	return strcmp(m_cName, "WoodShield") == 0 ||
		strcmp(m_cName, "LeatherShield") == 0 ||
		strcmp(m_cName, "TargeShield") == 0 ||
		strcmp(m_cName, "LagiShield") == 0 ||
		strcmp(m_cName, "MerienShield") == 0 ||
		strcmp(m_cName, "TowerShield") == 0;
}

bool CItem::IsAngelic()
{
	return strcmp(m_cName, "AngelicPandent(STR)") == 0 ||
		strcmp(m_cName, "AngelicPandent(DEX)") == 0 ||
		strcmp(m_cName, "AngelicPandent(INT)") == 0 ||
		strcmp(m_cName, "AngelicPandent(MAG)") == 0;
}

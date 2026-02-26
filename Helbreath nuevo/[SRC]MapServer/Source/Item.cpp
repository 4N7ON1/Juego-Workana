// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "../Header/Item.h"
#include <string>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	ZeroMemory(m_cName, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
											  
	m_sItemEffectValue1 = 0;
	m_sItemEffectValue2 = 0;
	m_sItemEffectValue3 = 0; 

	m_sItemEffectValue4 = 0;
	m_sItemEffectValue5 = 0;
	m_sItemEffectValue6 = 0; 

	m_dwCount = 1;
	m_sTouchEffectType   = 0;
	m_sTouchEffectValue1 = 0;
	m_sTouchEffectValue2 = 0;
	m_sTouchEffectValue3 = 0;
	
	m_cItemColor = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;

	m_sSpecialEffectValue1 = 0;
	m_sSpecialEffectValue2 = 0;
	
	m_wCurLifeSpan = 0;
	m_dwAttribute   = 0;

	m_cCategory = NULL;
	m_sIDnum    = 0;

	m_bIsForSale = FALSE;

	isggweap = false;
	isgungameitem = false;
	isggequip = false;
	isggcandy = false;

	m_bagPositionX = 40;
    m_bagPositionY = 30;
	m_bEquipped = FALSE;
	sIndex = -1;
	m_dwTimeDrop = -1;
	isbrfixitemequip = false;
	isbrweap = false;
	isbritem = false;
	isbrequip = false;
	isbrcandy = false;
	isbrdropitem = false;

	m_iTokenVal = 0;
}

CItem::~CItem()
{

}

void CItem::SetBagPosition(short positionX, short positionY)
{
    m_bagPositionX = positionX;
    m_bagPositionY = positionY;
}

short CItem::GetBagPositionX()
{
	return m_bagPositionX;
}
short CItem::GetBagPositionY()
{
	return m_bagPositionY;
}

// Bank Page's
void CItem::SetBankPosition(short BankPosition)
{
	m_sBankPosition = BankPosition;
}

short CItem::GetBankPosition()
{
	return m_sBankPosition;
}

//heaton 2024
bool CItem::IsArmor()
{
	return strcmp(m_cName, "Hauberk(M)") == 0 ||
		strcmp(m_cName, "Hauberk(W)") == 0 ||
		strcmp(m_cName, "Helm(M)") == 0 ||
		strcmp(m_cName, "Helm(W)") == 0 ||
		strcmp(m_cName, "Robe(M)") == 0 ||
		strcmp(m_cName, "Robe(W)") == 0 ||
		strcmp(m_cName, "ChainHose(M)") == 0 ||
		strcmp(m_cName, "ChainHose(W)") == 0 ||
		strcmp(m_cName, "FullHelm(M)") == 0 ||
		strcmp(m_cName, "FullHelm(W)") == 0 ||
		strcmp(m_cName, "ChainMail(M)") == 0 ||
		strcmp(m_cName, "ChainMail(W)") == 0 ||
		strcmp(m_cName, "PlateMail(M)") == 0 ||
		strcmp(m_cName, "PlateMail(W)") == 0 ||
		strcmp(m_cName, "Horned-Helm(M)") == 0 ||
		strcmp(m_cName, "Horned-Helm(W)") == 0 ||
		strcmp(m_cName, "Wings-Helm(M)") == 0 ||
		strcmp(m_cName, "Wings-Helm(W)") == 0 ||
		strcmp(m_cName, "WizardHauberk(M)") == 0 ||
		strcmp(m_cName, "WizardHauberk(W)") == 0 ||
		strcmp(m_cName, "Wizard-Cap(M)") == 0 ||
		strcmp(m_cName, "Wizard-Cap(W)") == 0 ||
		strcmp(m_cName, "Wizard-Hat(M)") == 0 ||
		strcmp(m_cName, "Wizard-Hat(W)") == 0 ||
		strcmp(m_cName, "Cape") == 0 ||
		strcmp(m_cName, "Cape+1") == 0 ||
		strcmp(m_cName, "LongBoots") == 0 ||
		strcmp(m_cName, "Shoes") == 0 ||

		string(m_cName) == "ShadowKnightHelm(M)" ||
		string(m_cName) == "ShadowKnightHelm(W)" ||
		string(m_cName) == "ShadowKnightLegg(M)" ||
		string(m_cName) == "ShadowKnightLegg(W)" ||
		string(m_cName) == "ShadowKnightBerk(M)" ||
		string(m_cName) == "ShadowKnightBerk(W)" ||

		string(m_cName) == "ShadowKnightArmor(M)" ||
		string(m_cName) == "ShadowKnightArmor(W)" ||
		string(m_cName) == "ShadowKnightRobe(M)" ||
		string(m_cName) == "ShadowKnightRobe(W)" ||
		string(m_cName) == "ShadowKnightHat(M)" ||
		string(m_cName) == "ShadowKnightHat(W)" ||

		string(m_cName) == "eShadowKnightCape" ||
		string(m_cName) == "aShadowKnightCape";
}

bool CItem::IsAngelic()
{
	return strcmp(m_cName, "AngelicPandent(STR)") == 0 ||
		strcmp(m_cName, "AngelicPandent(DEX)") == 0 ||
		strcmp(m_cName, "AngelicPandent(INT)") == 0 ||
		strcmp(m_cName, "AngelicPandent(MAG)") == 0;
}

bool CItem::IsMysticHeroSet()
{
	return false;/*
	return strcmp(m_cName, "MysticHeroHelm(M)") == 0 ||
		strcmp(m_cName, "MysticHeroHelm(W)") == 0 ||
		strcmp(m_cName, "MysticHeroCap(M)") == 0 ||
		strcmp(m_cName, "MysticHeroCap(W)") == 0 ||
		strcmp(m_cName, "MysticHeroArmor(M)") == 0 ||
		strcmp(m_cName, "MysticHeroArmor(W)") == 0 ||
		strcmp(m_cName, "MysticHeroRobe(M)") == 0 ||
		strcmp(m_cName, "MysticHeroRobe(W)") == 0 ||
		strcmp(m_cName, "MysticHeroHauberk(M)") == 0 ||
		strcmp(m_cName, "MysticHeroHauberk(W)") == 0 ||
		strcmp(m_cName, "MysticHeroLeggs(M)") == 0 ||
		strcmp(m_cName, "MysticHeroLeggs(W)") == 0 ||
		strcmp(m_cName, "MysticHeroCape") == 0;*/
}

bool CItem::PermitedInExp()
{
	return strcmp(m_cName, "Cape") == 0 ||
		strcmp(m_cName, "Shoes") == 0 ||
		strcmp(m_cName, "LongBoots") == 0 ||
		strcmp(m_cName, "Cape+1") == 0;
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

bool CItem::IsWeapon()
{	//swords
	return strcmp(m_cName, "DemonSlayer") == 0 ||
		strcmp(m_cName, "DarkExecutor") == 0 ||
		strcmp(m_cName, "LightingBlade") == 0 ||
		strcmp(m_cName, "LightingWand") == 0 ||
		strcmp(m_cName, "KlonessBlade") == 0 ||
		strcmp(m_cName, "KlonessHammer") == 0 ||
		strcmp(m_cName, "DeathWand(MS.30)") == 0 ||
		strcmp(m_cName, "KlonessAxe") == 0 ||
		strcmp(m_cName, "GiantBattleHammer") == 0 ||
		strcmp(m_cName, "The_Devastator") == 0 ||
		strcmp(m_cName, "StormBringer") == 0 ||
		strcmp(m_cName, "PowerBarbarian") == 0 ||
		strcmp(m_cName, "PowerRapier") == 0 ||
		strcmp(m_cName, "PkssBlade") == 0 ||
		strcmp(m_cName, "StripIceHammer") == 0 ||
		strcmp(m_cName, "PkssBarbarian") == 0 ||
		strcmp(m_cName, "PkssAxe") == 0 ||
		strcmp(m_cName, "PkssEsterk") == 0 ||
		strcmp(m_cName, "BattleHammer") == 0 ||
		strcmp(m_cName, "BarbarianHammer") == 0 ||
		strcmp(m_cName, "PowerHeroSword") == 0 ||
		strcmp(m_cName, "BlackHammer") == 0 ||
		strcmp(m_cName, "SwordOfDragon") == 0 ||
		strcmp(m_cName, "GiantSword") == 0 ||

		strcmp(m_cName, "RedZerkWand") == 0 ||
		strcmp(m_cName, "DarkZerkWand") == 0 ||
		strcmp(m_cName, "GoldZerkWand") == 0 ||

		strcmp(m_cName, "RedDevastator") == 0 ||
		strcmp(m_cName, "DarkDevastator") == 0 ||
		strcmp(m_cName, "GoldDevastator") == 0 ||

		//wands
		strcmp(m_cName, "MagicWand(MS30-LLF)") == 0 ||
		strcmp(m_cName, "BerserkWand(MS.30)") == 0 ||
		strcmp(m_cName, "BerserkWand(MS.10)") == 0 ||
		strcmp(m_cName, "ResurWand(MS.30)") == 0 ||
		strcmp(m_cName, "ResurWand(MS.10)") == 0 ||
		strcmp(m_cName, "KlonessWand(MS.30)") == 0 ||
		strcmp(m_cName, "KlonessWand(MS.10)") == 0 ||
		strcmp(m_cName, "WhiteMWand(MS.60)") == 0 ||
		strcmp(m_cName, "PkssWand(MS.20)") == 0 ||
		strcmp(m_cName, "PkssWand(MS.40)") == 0 ||
		strcmp(m_cName, "PowerHeroWand") == 0 ||

		strcmp(m_cName, "MysticHammer") == 0 ||
		strcmp(m_cName, "MysticAxe") == 0 ||
		strcmp(m_cName, "MysticBlade") == 0 ||
		strcmp(m_cName, "MysticRapier") == 0 ||
		strcmp(m_cName, "MysticWand(MS.30)") == 0 ||
		strcmp(m_cName, "EternalHammer") == 0 ||
		strcmp(m_cName, "EternalRapier") == 0 ||
		strcmp(m_cName, "EternalBlade") == 0 ||
		strcmp(m_cName, "EternalWand(MS.40)") == 0 ||
		strcmp(m_cName, "EternalWand(MS.40)") == 0 ||
		strcmp(m_cName, "HunterWand(MS.50)") == 0 ||
		strcmp(m_cName, "HunterSword") == 0 ||
		strcmp(m_cName, "EternalStormBringer") == 0 ||

		strcmp(m_cName, "DarkDemonSlayer") == 0 ||
		strcmp(m_cName, "RedLightingBlade") == 0 ||
		strcmp(m_cName, "GoldKlonessBlade") == 0 ||
		strcmp(m_cName, "DarkKlonessWand") == 0 ||
		strcmp(m_cName, "RedKlonessAxe") == 0 ||
		strcmp(m_cName, "GoldDevastator") == 0 ||
		strcmp(m_cName, "GoldBerserkWand") == 0 ||
		strcmp(m_cName, "DarkBattleHammer") == 0 ||
		strcmp(m_cName, "DarkEternalBlade") == 0 ||
		strcmp(m_cName, "GoldEternalHammer") == 0 ||
		strcmp(m_cName, "RedEternalWand") == 0 ||
		strcmp(m_cName, "GoldMysticWand") == 0 ||
		strcmp(m_cName, "RedMysticBlade") == 0 ||
		strcmp(m_cName, "DarkMysticHammer") == 0 ||
		
		strcmp(m_cName, "BlazingSword") == 0 ||
		strcmp(m_cName, "HammerOfThor") == 0 ||
		strcmp(m_cName, "RapierofEmbers") == 0 ||
		strcmp(m_cName, "LightningSword") == 0 ||
		strcmp(m_cName, "AsteroidHammer") == 0 ||
		strcmp(m_cName, "ThunderRapier") == 0 ||
		strcmp(m_cName, "IceExecutor") == 0 ||
		strcmp(m_cName, "FrozenHammer") == 0 ||
		strcmp(m_cName, "EsterkOfFrost") == 0 ||
		strcmp(m_cName, "IceWizardStaff") == 0 ||
		strcmp(m_cName, "LightningWizStaff") == 0 ||
		strcmp(m_cName, "FireWizardStaff") == 0 ||

		strcmp(m_cName, "IceBladeOfAby") == 0 ||
		strcmp(m_cName, "FireBladeOfAby") == 0 ||
		strcmp(m_cName, "LightBladeOfAby") == 0 ||
		strcmp(m_cName, "IceStaffOfAby") == 0 ||
		strcmp(m_cName, "FireStaffOfAby") == 0 ||
		strcmp(m_cName, "LightStaffOfAby") == 0 ||
		strcmp(m_cName, "IceHammerOfAby") == 0 ||
		strcmp(m_cName, "FireHammerOfAby") == 0 ||
		strcmp(m_cName, "LightHammerOfAby") == 0 ||

		strcmp(m_cName, "GoldMysticAxe") == 0;

}
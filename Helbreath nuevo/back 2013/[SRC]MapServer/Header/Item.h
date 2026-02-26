// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>
#include <winbase.h>
#include "..\\..\\Shared\\netmsg.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
										   
#define DEF_MAXITEMEQUIPPOS		22
#define DEF_EQUIPPOS_NONE		0		// 장착이 불가능하다.
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_LEGGINGS	5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13	
#define DEF_EQUIPPOS_NEWRING	14			//헤드 바디 암 신발 팬츠 망토
#define DEF_ITEMTYPE_NOTUSED	-1			// v1.4 기존에 사용되었으나 현재로서는 사용되지 않는 아이템: 포상금으로 대체된다.
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1		// 공격치: value1 D value2 + value3
#define DEF_ITEMEFFECTTYPE_DEFENSE			2		// 방어능력 
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3		// 화살을 사용하는 공격무기. 무기 자체는 공격력이 없다.		
#define DEF_ITEMEFFECTTYPE_HP   		    4		// HP관련 효과 
#define DEF_ITEMEFFECTTYPE_MP   		    5		// MP관련 효과 
#define DEF_ITEMEFFECTTYPE_SP   		    6		// SP관련 효과 
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7		// HP관련 효과. 즉각적으로 반응이 나타나지는 않는다. 
#define DEF_ITEMEFFECTTYPE_GET			    8		// 얻는다. 물고기나 광물등 
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9		// 기술을 배우는 아이템이다.
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10		// 위치를 표시하는 아이템. 
#define DEF_ITEMEFFECTTYPE_MAGIC			11		// 사용하면 마법 효과를 얻는 아이템 
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12		// 플레이어의 속성을 변환시킨다. 성별, 머리모양, 피부색, 헤어스타일 
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13		// 공격효과와 더불어 마나의 절약 효과가 있다.(마법사용)
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14		// 추가 효과
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15		// 마법으로 인한 공격의 대미지를 절감한다. 반지나 목걸이류 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16		// 점령 깃발 
#define DEF_ITEMEFFECTTYPE_DYE				17		// 염색약. 
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18		// 마법을 배우는 아이템이다.
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19		// 공격 효과와 함께 최대  HP 및 HP 회복량이 떨어지는 아이템 
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20		// 공격 효과와 함께 갑옷과 같이 물리 방어구 효과가 있는 아이템 
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21		// 무기 제작 재료 속성 
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINAR		22		// 스태미너 고정 아이템 
#define DEF_ITEMEFFECTTYPE_LOTTERY			23		// 복권 아이템

#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24	// 공격 아이템 특수 효과 
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25	// 방어구 아이템 특수 효과 
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26	// 다른 아이템 떨어질 것 대신 이 아이템이 떨어진다.

#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27	// 건축 키트
#define DEF_ITEMEFFECTTYPE_WARM					28	// 얼린 몸을 녹인다. v2.172 2002-7-5
#define DEF_ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29	// 방어능력 v2.172 2002-7-5
#define DEF_ITEMEFFECTTYPE_ITEMFARM				30  //v2.19 2002-12-16 농사 스킬 관련
#define DEF_ITEMEFFECTTYPE_SLATES				31
#define DEF_ITEMEFFECTTYPE_ARMORDYE				32

#define DEF_ITEMEFFECTTYPE_ADDREPUTATION	35
#define DEF_ITEMEFFECTTYPE_SWEAPONATTRIBUTE_STONE	36  //by ogeid Atributos secundarios
#define DEF_ITEMEFFECTTYPE_PWEAPONATTRIBUTE_STONE	37 //by ogeid Atributos primarios
#define DEF_ITEMEFFECTTYPE_ADDCOINS			38
#define DEF_ITEMEFFECTTYPE_SUMMONBALL		39

#define DEF_ITEMEFFECTTYPE_REMOVEITEMID		40 // ZeroEoyPnk - Remove Item ID
#define DEF_ITEMEFFECTTYPE_MasterItemID		41 // ZeroEoyPnk - Remove Item ID
#define DEF_ITEMEFFECTTYPE_RAIDERTICKET		42
#define DEF_ITEMEFFECTTYPE_RAIDERTICKET2	43
#define DEF_ITEMEFFECTTYPE_HUNTERTICKET		44

//#define DEF_ITEMEFFECTTYPE_SEXITEMCHANGE	80 // ZeroEoyPnk - Remove Item ID

#define DEF_ITEMEFFECTTYPE_CONTRIBUTION		45

#define ITEMEFFECTTYPE_BALL 62

#define DEF_ITEMEFFECTTYPE_COIN_BALL		47
#define DEF_ITEMEFFECTTYPE_REP_BALL		48
#define DEF_ITEMEFFECTTYPE_MAJ_BALL		49

#define DEF_ITEMEFFECTTYPE_ADDCRITICALS		55
#define DEF_ITEMEFFECTTYPE_DEDUCTREPUTATION	60
#define DEF_ITEMEFFECTTYPE_RESTOREREP		64

#define DEF_ITEMEFFECTTYPE_CANDYINSERT	65
#define DEF_ITEMEFFECTTYPE_CANDYSUPRIMIR	66

#define DEF_ITEMEFFECTTYPE_SacoItemID	67
#define ITEMEFFECTTYPE_CURATIONPOTION 68

#define DEF_ITET_UNIQUE_OWNER				1		// ItemTouchEffect: 주인을 갖는 아이템 
#define DEF_ITET_ID							2		// 그냥 아이템 아이디
#define DEF_ITET_DATE						3		// 사용가능한 날짜가 지정된 아이템 


class CItem  
{
public:

	CItem();
	virtual ~CItem();

	char  m_cName[21];
	
	short m_sIDnum;					// 아이템의 고유 번호 
	char  m_cItemType;
	char  m_cEquipPos;
	short m_sItemEffectType;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3;
	short m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6;

	//Fix Endu 	
//	WORD  m_wMaxLifeSpan;
	int   m_wMaxLifeSpan;
	short m_sSpecialEffect;
	
	//short m_sSM_HitRatio, m_sL_HitRatio;
	//v1.432 명중률 가감 사용 안한다. 대신 특수 능력 수치가 들어간다.
	short m_sSpecialEffectValue1, m_sSpecialEffectValue2;
	bool m_bEquipped;
	short m_sSprite;
	short m_sSpriteFrame;

	char  m_cApprValue;
	char  m_cSpeed;

	DWORD m_wPrice;
	WORD  m_wWeight;
	short m_sLevelLimit;
	char  m_cGenderLimit;

	short m_sRelatedSkill;
	
	char  m_cCategory;
	BOOL  m_bIsForSale;
	// 

	DWORD m_dwCount;
	short m_sTouchEffectType;
	short m_sTouchEffectValue1, m_sTouchEffectValue2;
	int m_sTouchEffectValue3;
	char  m_cItemColor; // v1.4 에서 아이템 색으로 변경되었다. 
//	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	//Fix Endurance
	int m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	int  m_wCurLifeSpan;
	DWORD m_dwAttribute;				// aaaa bbbb cccc dddd eeee ffff xxxx xxx1 
										// 1: Custom-Made Item flag 
										// a: 추가 대미지
										// b: RESERVED
										// c: 특수 아이템 성질 Flag 
										// d: 특수 아이템 성질 정도 
	// e: 특수 아이템 추가 성질 Flag
	short m_bagPositionX;
	short m_bagPositionY;
	bool isggweap;
	bool isgungameitem;
	bool isggequip;
	bool isggcandy;
	int m_iTokenVal;
	DWORD m_dwTimeDrop;
	short sIndex;
	
	bool isbrweap;
	bool isbritem;
	bool isbrequip;
	bool isbrcandy;
	bool isbrfixitemequip;
	
	bool isbrdropitem;

	bool isdkweap;
	bool isdkitem;
	bool isdkequip;
	bool isdkcandy;

private:
	short m_sBankPosition;

public:

    short GetBagPositionX();
    short GetBagPositionY();
	short GetBankPosition();

	//heaton 2024
	bool IsArmor();
	bool IsAngelic();
	bool IsMysticHeroSet();
	bool PermitedInExp();
	bool IsWeapon();
	bool IsShield();

    void SetBagPosition(short positionX, short positionY);
	void SetBankPosition(short BankPosition);

	DWORD GetBallValue()
	{
		return m_dwCount;
	}

	bool isBall()
	{
		return m_sIDnum == (short)BItemID::ContributionBall || m_sIDnum == (short)BItemID::MajesticBall || m_sIDnum == (short)BItemID::CoinBall || m_sIDnum == (short)BItemID::RepBall;
	}
};



#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)

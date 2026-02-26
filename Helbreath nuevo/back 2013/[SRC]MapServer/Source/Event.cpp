// dynamic events
#include "..\\Header\\Event.h"
#include "..\\Header\\Game.h"
#include "..\\Header\\battleroyale.h"
#include "..\\Header\\dkevent.h"


//HeatoN events
extern class CBattleRoyale * c_br;
extern class CDkEvent * c_dkevent;
extern cCoreServer * c_map;

Event::Event()
{

}

Event::~Event()
{

}

void Event::Handler::Activate(EventID id)
{
	CMap * map = 0;

	int i;

	if (Is(id))	return;

	auto & p = ptr[(int)id];

	std::ofstream outfile;//

	switch (id)
	{
	case EventID::Deathmatch:
	{
		p = new Event;
		c_map->EnableDeathMatch();
		break;
	}

	case EventID::Crusada:
	{
		p = new Event;
		c_map->DeleteNpcs();
		break;
	}

	case EventID::Abaddon:
	{
		p = new Event;
		c_map->InitAbyEvent();
		break;
	}

	case EventID::RiseOfAbaddon:
	{
		p = new Event;
		c_map->InitAbyEvent();
		break;
	}

	case EventID::CVC:
	{
		p = new Event;
		c_map->EnableCVC();
		break;
	}

	case EventID::ThePurge:
	{
		p = new Event;
		c_map->purge_newround = 0;
		break;
	}


	case EventID::BattleRoyale:
	{	
		c_map->ClearMapBattleRoyale("battle");
		c_map->vec_brbackmembers.clear();
		c_map->vec_brfighters.clear();
		c_map->brwinners.clear();
		c_map->enableBattleRoyaleEvent();
		p = new Event;
		break;
	}

	case EventID::DkEvent:
	{
		p = new Event;
		c_map->vec_dkbackmembers.clear();
		c_map->EnableDkEvent();

		break;
	}

	case EventID::GunGame:
	{
		p = new Event;
		c_map->vec_backmembers.clear();
		c_map->EnableGunGame();
		string leader = c_map->findGGLeader();
		c_map->NotifyGunGameLeader(leader);
		/*const int result = remove("Csv\\dmplayers.csv");
		ofstream outputFile("Csv\\dmplayers.csv");
		outputFile.close();
		c_dm->UpdateDmFile();
		p = new Event;
		c_dm->EnableEvent();*/
		break;
	}
	case EventID::CTR:
	{
		p = new Event;

		c_map->DeleteGuards();
		c_map->DeleteNpcs();
		c_map->m_sSummonStones = 0;
		c_map->m_bSummonStones = true;

		c_map->m_sRelicStatus = RELIC_MIDDLE;

		c_map->m_sAresdenRelics = 0;
		c_map->m_sElvineRelics = 0;

		c_map->m_sFloorRelicX = 0;
		c_map->m_sFloorRelicY = 0;
		
		c_map->NotifyRelicStatus();

		for (int i = 1; i < DEF_MAXCLIENTS; i++){
			if (c_map->m_pClientList[i] != NULL){
				if (!c_map->m_pClientList[i]->m_bIsInitComplete) continue;
				c_map->m_pClientList[i]->m_bRelicHolder = false;

				if (c_map->m_pClientList[i]->m_bIsKilled) continue;

				if (c_map->m_pClientList[i]->IsInMap("city") || c_map->m_pClientList[i]->IsInMap("2ndmiddle"))
				{
					if (c_map->m_pClientList[i]->IsLocation("elvine"))
						c_map->RequestTeleportHandler(i, "2   ", "elvine", -1, -1);
					else
						c_map->RequestTeleportHandler(i, "2   ", "aresden", -1, -1);
				}				
			}
		}

		c_map->send_objects_data();
		break;
	}
	default:
		p = new Event;
		break;
	}

	NotifyStatus(id);
}

void Event::Handler::NotifyStatus(EventID id)
{
	for (auto p : c_map->m_pClientList)
	{
		if (!p)
			continue;

		p->Notify(NOTIFY_EVENT_STATUS, (DWORD)id, Is(id));
	}
}


void Event::Handler::LoginNotify(int client)
{
	auto g = c_map;
	auto p = g->m_pClientList[client];
	if (!p) return;
	for (int i = 0; i < (int)EventID::Max; i++)
	{
		auto is = g_ev.Is((EventID)i);
		p->Notify(NOTIFY_EVENT_STATUS2, (DWORD)i, is);
	}
}

void Event::Handler::Deactivate(EventID id)
{
	std::ofstream outfile1;
	std::ofstream outfile;

	if (!Is(id)) return;

	if (ptr[(int)id])
	{
		delete ptr[(int)id];
		ptr[(int)id] = nullptr;
	}

	NotifyStatus(id);

	switch (id)
	{

		case EventID::Deathmatch:
		{
			c_map->DisableDeathmatch();
			break;
		}

		case EventID::BattleRoyale:
		{
			c_map->ClearMapBattleRoyale("battle");
			c_map->FinishBattleRoyaleEvent();
			break;
		}

		case EventID::DkEvent:
		{
			c_map->DisableDkEvent();
			break;
		}
		
		case EventID::GunGame:
		{
			
			c_map->DisableGunGame();
			break;
		}

		case EventID::ThePurge:
		{
			c_map->DisableBloorRite();
			break;
		}

		case EventID::Abaddon:
		{
			c_map->DeleteApocaNpcs();
			break;
		}

		case EventID::RiseOfAbaddon:
		{
			c_map->DeleteApocaNpcs();
			c_map->restoreBotToSafe();
			break;
		}

		case EventID::CTR:
		{
			c_map->m_sRelicStatus = RELIC_MIDDLE;
			c_map->NotifyRelicStatus();
			c_map->DeleteStones();
			c_map->m_sSummonStones = 0;
			c_map->m_bSummonStones = false;
			for (int i = 1; i < DEF_MAXCLIENTS; i++){
				if (c_map->m_pClientList[i] != NULL){
					if (!c_map->m_pClientList[i]->m_bIsInitComplete) continue;
					c_map->m_pClientList[i]->m_bRelicHolder = false;
				}
			}

			c_map->send_objects_data();
			break;
		}
	}	
}

void Event::Handler::Toggle(EventID id)
{
	if (Is(id))
		Activate(id);
	else Deactivate(id);
}

void Event::Handler::Teleport(EventID id, CClient * p)
{
	auto g = c_map;

	if (!Is(id))
	{
		g->SendAlertMsg(p->client, "Can't teleport, event is disabled!");
		return;
	}

	if (!p->IsInsideCh())
	{
		g->SendAlertMsg(p->client, "Can't teleport on this map!");
		return;
	}

	/*p->iPartyNum = -1;
	c_cmd->SendCommand(p->client, "/clearparty", 0, 0, 0, 0);*/	

	switch (id)
	{
	case EventID::Deathmatch: /*c_dm->Join(p->handle);*/ break;
	case EventID::GunGame: /*c_ggv2->JoinEvent(p->handle);*/ break;
	}
}
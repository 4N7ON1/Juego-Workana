#include "..\\Headers\\Event.h"
#include "..\\Headers\\Game.h"

extern class CGame * G_pGame;

Event::Event()
{
}


Event::~Event()
{
}

void Event::NotifyEventStatus(char *& cp, int mode)
{
	EventID id;
	Pop(cp, id);
	
	bool pre_status = Is(id);

	Pop(cp, status[(int)id]);

	if (pre_status != status[(int)id])
	{
		string s = "Event " + string(getEventName(id)) + " is ";
		if (Is(id))
			s.append("Enabled");
		else s.append("Disabled");

		if (mode == 1)
		G_pGame->SetTopMsg((char*)s.c_str(), 10);
	}

	auto is = Is(id);

	switch (id)
	{	
	case EventID::Deathmatch:
		break;
	case EventID::Max:
		break;
	default:
		break;
	}
}

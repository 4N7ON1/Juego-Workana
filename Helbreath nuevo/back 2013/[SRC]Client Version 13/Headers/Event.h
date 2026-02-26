#pragma once

#include "..\..\shared\shared.h"

class Event
{
public:
	Event();
	~Event();

	typedef EventMsg M;

	void Init()
	{
		for (auto & b : status)
			b = false;
	}

	void Read(char *& cp)
	{
		for (auto & b : status)
			Pop(cp, b);
	}

	void NotifyEventStatus(char *& cp, int mode);

	bool Is(EventID id){ return status[(int)id]; }
	bool status[(int)EventID::Max];
};


#pragma once
#include "..\..\shared\shared.h"

class CClient;
class CMap;

class Event {
public:
	Event();
	virtual ~Event();
	virtual void JoinEvent(int client) {}
	typedef EventMsg M;

	class Handler
	{
	public:
		Handler()
		{
			for (auto & p : ptr)
				p = nullptr;
		}
		~Handler()
		{
		
		}

		void Teleport(EventID id, CClient * p);

		Event * Get(EventID id) { return ptr[(int)id]; }
		bool Is(EventID id) { return Get(id) != nullptr; }

		void Write(char *& cp)
		{
			for (int i = 0; i < (int)EventID::Max; i++)
			{
				Push(cp, Is((EventID)i));
			}
		}
		void Activate(EventID id);
		void NotifyStatus(EventID id);
		void Toggle(EventID id);
		void Deactivate(EventID id);
		void Tick(){}
		void RemoveFile(char * name);
		void LoginNotify(int client);
		string ReadEventWinner(char * filename);
		void GetWinners(int client);
		Event * ptr[(int)EventID::Max];
	};
};



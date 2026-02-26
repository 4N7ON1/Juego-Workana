#pragma once
#include <windows.h>
#include <string.h>

#define SHARED_MEM_NAME "HB_RELAY_SHARED"
#define MAX_PACKETS 128

// ======================================
// Estructura de cada mensaje
// ======================================
struct RelayPacket {
	unsigned int length;
	unsigned int mapID;
	unsigned int clientID;
	char data[256];
};

// ======================================
// Estructura principal compartida
// ======================================
struct SharedRelayBuffer {
	volatile long writePos;
	volatile long readPos;
	RelayPacket packets[MAX_PACKETS];
};

// ======================================
// Variables globales
// ======================================
extern HANDLE g_hRelayMap;
extern SharedRelayBuffer* g_pSharedRelay;

// ======================================
// Funciones
// ======================================
bool InitRelaySharedMemory();
void PushRelayPacket(unsigned int map, unsigned int client, const char* msg);
void ShutdownRelaySharedMemory();

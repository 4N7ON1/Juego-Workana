#include "../Header/RelayShared.h"
#include <iostream>

HANDLE g_hRelayMap = NULL;
SharedRelayBuffer* g_pSharedRelay = NULL;

// ======================================
// Inicializar memoria compartida
// ======================================
bool InitRelaySharedMemory()
{
	g_hRelayMap = CreateFileMapping(
		INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
		0, sizeof(SharedRelayBuffer), SHARED_MEM_NAME);

	if (g_hRelayMap == NULL) {
		std::cout << "ERROR: CreateFileMapping falló. Código: " << GetLastError() << std::endl;
		return false;
	}

	g_pSharedRelay = (SharedRelayBuffer*)MapViewOfFile(
		g_hRelayMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedRelayBuffer));

	if (g_pSharedRelay == NULL) {
		std::cout << "ERROR: MapViewOfFile falló. Código: " << GetLastError() << std::endl;
		CloseHandle(g_hRelayMap);
		g_hRelayMap = NULL;
		return false;
	}

	// Si el bloque es nuevo, inicializarlo
	if (GetLastError() != ERROR_ALREADY_EXISTS) {
		g_pSharedRelay->writePos = 0;
		g_pSharedRelay->readPos = 0;
		std::cout << "[RelayShared] Memoria compartida creada nueva." << std::endl;
	}
	else {
		std::cout << "[RelayShared] Memoria compartida existente (puede haber Relay activo)." << std::endl;
	}

	return true;
}

// ======================================
// Enviar un paquete al Relay
// ======================================
void PushRelayPacket(unsigned int map, unsigned int client, const char* msg)
{
	if (g_pSharedRelay == NULL) return;

	HWND hwnd = FindWindowA(NULL, "Helbreath Cursed - Relay Server");
	if (hwnd == NULL)
	{
		// relay cerrado → no enviar
		return;
	}

	unsigned int w = g_pSharedRelay->writePos;
	unsigned int next = (w + 1) % MAX_PACKETS;

	// Si se llena, sobrescribe el más viejo
	if (next == (unsigned int)g_pSharedRelay->readPos)
		g_pSharedRelay->readPos = (g_pSharedRelay->readPos + 1) % MAX_PACKETS;

	RelayPacket& pkt = g_pSharedRelay->packets[w];
	pkt.mapID = map;
	pkt.clientID = client;
	pkt.length = (unsigned int)strlen(msg) + 1;
	strncpy(pkt.data, msg, sizeof(pkt.data) - 1);
	pkt.data[sizeof(pkt.data) - 1] = 0;

	g_pSharedRelay->writePos = next;
}

// ======================================
// Liberar memoria compartida
// ======================================
void ShutdownRelaySharedMemory()
{
	if (g_pSharedRelay) {
		UnmapViewOfFile(g_pSharedRelay);
		g_pSharedRelay = NULL;
	}
	if (g_hRelayMap) {
		CloseHandle(g_hRelayMap);
		g_hRelayMap = NULL;
	}
}

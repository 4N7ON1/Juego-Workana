#include "..\\Headers\\XSocketUDP.h"
#include <iostream>

XSocketUDP::XSocketUDP() : m_socket(INVALID_SOCKET) {
	// Inicialización de Winsock (si no lo has hecho en otra parte)
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed.\n";
	}
}

XSocketUDP::~XSocketUDP() {
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
	}
	WSACleanup();
}

bool XSocketUDP::Init() {
	// Crear el socket UDP
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
		return false;
	}

	std::cout << "Socket initialized for client usage.\n";
	return true;
}

bool XSocketUDP::Send(const Message& msg, const std::string& targetAddress, unsigned short targetPort) {
	sockaddr_in targetAddr = {};
	targetAddr.sin_family = AF_INET;
	targetAddr.sin_port = htons(targetPort);
	targetAddr.sin_addr.s_addr = inet_addr(targetAddress.c_str());

	int sendResult = sendto(m_socket, (char*)&msg, sizeof(msg), 0, (sockaddr*)&targetAddr, sizeof(targetAddr));
	return sendResult != SOCKET_ERROR;
}

bool XSocketUDP::Receive(Message& msg) {
	sockaddr_in senderAddr;
	int senderAddrSize = sizeof(senderAddr);

	int recvResult = recvfrom(m_socket, (char*)&msg, sizeof(msg), 0, (sockaddr*)&senderAddr, &senderAddrSize);
	return recvResult != SOCKET_ERROR;
}

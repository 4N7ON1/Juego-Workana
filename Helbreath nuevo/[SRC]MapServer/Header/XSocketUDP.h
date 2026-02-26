#ifndef XSOCKETUDP_H
#define XSOCKETUDP_H

#include <string>
#include <winsock2.h> // O cualquier librería de sockets compatible con tu proyecto

// Estructura de mensaje
struct Message {
	int type;
	char data[256]; // Mensaje de ejemplo
};

class XSocketUDP {
public:
	XSocketUDP();
	~XSocketUDP();

	bool Init(const std::string& address, unsigned short port);
	bool Send(const Message& msg, const std::string& targetAddress, unsigned short targetPort);
	bool Receive(Message& msg);

private:
	SOCKET m_socket;
	sockaddr_in m_serverAddr;
};

#endif // XSOCKETUDP_H

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <map>
#include <mutex>
#include <signal.h>
#include <atomic>
#include <fcntl.h>

// ===========================================================
// Buffers sincronizados con Helbreath
// ===========================================================
#define BUFFER_SIZE 200000                // Igual que DEF_MSGBUFFERSIZE del cliente (200 KB)
const int MAX_BUFFER = 262144;            // 256 KB → suficiente para cliente (200 KB) y server (300 KB)

// ===========================================================
// Config
// ===========================================================
struct RelayPort {
    int listen_port;
    std::string dst_ip;
    int dst_port;
};

struct UDPRelayConfig {
    int listen_port;
    std::string dst_ip;
    int dst_port;
};

struct ClientInfo {
    sockaddr_in client_addr;
    sockaddr_in remote_addr;
    time_t last_activity;
};

std::map<std::string, ClientInfo> udp_clients;
std::mutex clients_mutex;

std::string getClientKey(const sockaddr_in& addr) {
    return std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(ntohs(addr.sin_port));
}

void safeCloseSocket(int s) {
    if (s >= 0) close(s);
}

// ===========================================================
// TCP Relay optimizado (no bloqueante, sin Connection Lost)
// ===========================================================
void forwardData(int src, int dst, std::atomic<bool>& stopFlag) {
    char buffer[BUFFER_SIZE];
    while (!stopFlag.load()) {
        ssize_t bytesRead = recv(src, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            ssize_t totalSent = 0;
            while (totalSent < bytesRead && !stopFlag.load()) {
                ssize_t sent = send(dst, buffer + totalSent, bytesRead - totalSent, MSG_NOSIGNAL);
                if (sent > 0) {
                    totalSent += sent;
                } 
                else if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    // Buffer del kernel lleno → breve pausa (0.2 ms) para liberar presión
                    usleep(200);
                    continue;
                } 
                else {
                    stopFlag.store(true);
                    break;
                }
            }
        }
        else if (bytesRead == 0) {
            // Peer cerró la conexión
            stopFlag.store(true);
            break;
        }
        else if (bytesRead < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            usleep(200);
            continue;
        }
        else {
            stopFlag.store(true);
            break;
        }
    }
}

void handleClient(int clientSock, sockaddr_in /*clientAddr*/, RelayPort portCfg) {
    int flag = 1;

    // Configurar socket del cliente (cliente Helbreath)
    setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    setsockopt(clientSock, SOL_SOCKET, SO_RCVBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));
    setsockopt(clientSock, SOL_SOCKET, SO_SNDBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));

    // KeepAlive agresivo (detectar cortes en ~10 s)
    int enable = 1, idle = 10, interval = 3, count = 3;
    setsockopt(clientSock, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
    setsockopt(clientSock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    setsockopt(clientSock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    setsockopt(clientSock, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));

    // Crear socket remoto (relay → servidor Helbreath)
    int remoteSock = socket(AF_INET, SOCK_STREAM, 0);
    if (remoteSock < 0) {
        safeCloseSocket(clientSock);
        return;
    }

    setsockopt(remoteSock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    setsockopt(remoteSock, SOL_SOCKET, SO_RCVBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));
    setsockopt(remoteSock, SOL_SOCKET, SO_SNDBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));
    setsockopt(remoteSock, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
    setsockopt(remoteSock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    setsockopt(remoteSock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    setsockopt(remoteSock, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));

    // Modo no bloqueante
    fcntl(clientSock, F_SETFL, O_NONBLOCK);
    fcntl(remoteSock, F_SETFL, O_NONBLOCK);

    sockaddr_in remoteAddr{};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(portCfg.dst_port);
    inet_pton(AF_INET, portCfg.dst_ip.c_str(), &remoteAddr.sin_addr);

    if (connect(remoteSock, (sockaddr*)&remoteAddr, sizeof(remoteAddr)) < 0 && errno != EINPROGRESS) {
        perror("connect remoto");
        safeCloseSocket(clientSock);
        safeCloseSocket(remoteSock);
        return;
    }

    std::atomic<bool> stopFlag(false);

    // Dos hilos: cliente→servidor y servidor→cliente
    std::thread t1(forwardData, clientSock, remoteSock, std::ref(stopFlag));
    std::thread t2(forwardData, remoteSock, clientSock, std::ref(stopFlag));

    t1.join();
    t2.join();

    safeCloseSocket(clientSock);
    safeCloseSocket(remoteSock);
}

void startListener(RelayPort portCfg) {
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) { perror("socket TCP"); return; }

    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(listenSock, SOL_SOCKET, SO_RCVBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));
    setsockopt(listenSock, SOL_SOCKET, SO_SNDBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));

    sockaddr_in listenAddr{};
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(portCfg.listen_port);
    listenAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&listenAddr, sizeof(listenAddr)) < 0) {
        perror("bind TCP");
        safeCloseSocket(listenSock);
        return;
    }

    if (listen(listenSock, 512) < 0) {
        perror("listen TCP");
        safeCloseSocket(listenSock);
        return;
    }

    std::cout << "[OK] TCP " << portCfg.listen_port
              << " → " << portCfg.dst_ip << ":" << portCfg.dst_port << std::endl;

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientSock = accept(listenSock, (sockaddr*)&clientAddr, &len);
        if (clientSock >= 0)
            std::thread(handleClient, clientSock, clientAddr, portCfg).detach();
    }
}

// ===========================================================
// UDP Relay (sincronizado, no bloqueante implícito)
// ===========================================================
void handleClientToRemote(int udpSock, UDPRelayConfig udpCfg) {
    char buffer[BUFFER_SIZE];
    sockaddr_in remoteAddr{};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(udpCfg.dst_port);
    inet_pton(AF_INET, udpCfg.dst_ip.c_str(), &remoteAddr.sin_addr);

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        ssize_t bytes = recvfrom(udpSock, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &len);
        if (bytes <= 0) continue;

        std::string key = getClientKey(clientAddr);
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            udp_clients[key] = {clientAddr, remoteAddr, time(nullptr)};
        }

        sendto(udpSock, buffer, bytes, MSG_NOSIGNAL, (sockaddr*)&remoteAddr, sizeof(remoteAddr));
    }
}

void handleRemoteToClient(int udpSock) {
    char buffer[BUFFER_SIZE];
    while (true) {
        sockaddr_in from{};
        socklen_t len = sizeof(from);
        ssize_t bytes = recvfrom(udpSock, buffer, sizeof(buffer), 0, (sockaddr*)&from, &len);
        if (bytes <= 0) continue;

        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& [key, info] : udp_clients) {
            if (info.remote_addr.sin_addr.s_addr == from.sin_addr.s_addr &&
                info.remote_addr.sin_port == from.sin_port) {
                sendto(udpSock, buffer, bytes, MSG_NOSIGNAL, (sockaddr*)&info.client_addr, sizeof(info.client_addr));
                break;
            }
        }
    }
}

void cleanInactiveClients() {
    while (true) {
        sleep(60);
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto now = time(nullptr);
        for (auto it = udp_clients.begin(); it != udp_clients.end();) {
            if (now - it->second.last_activity > 300)
                it = udp_clients.erase(it);
            else ++it;
        }
    }
}

void startUDPListener(UDPRelayConfig udpCfg) {
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0) { perror("socket UDP"); return; }

    int opt = 1;
    setsockopt(udpSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(udpSock, SOL_SOCKET, SO_RCVBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));
    setsockopt(udpSock, SOL_SOCKET, SO_SNDBUF, &MAX_BUFFER, sizeof(MAX_BUFFER));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(udpCfg.listen_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udpSock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind UDP");
        safeCloseSocket(udpSock);
        return;
    }

    std::cout << "[OK] UDP " << udpCfg.listen_port
              << " → " << udpCfg.dst_ip << ":" << udpCfg.dst_port << std::endl;

    std::thread(handleClientToRemote, udpSock, udpCfg).detach();
    std::thread(handleRemoteToClient, udpSock).detach();
    std::thread(cleanInactiveClients).detach();
}

// ===========================================================
// MAIN
// ===========================================================
int main() {
    signal(SIGPIPE, SIG_IGN);

    std::vector<std::thread> threads;
    RelayPort login{9735, "35.235.114.103", 9735};
    RelayPort game{8531, "35.235.114.103", 8531};
    RelayPort secure{3392, "35.235.114.103", 3392};
    UDPRelayConfig udp{8888, "35.235.114.103", 8888};

    std::cout << "[*] Relay inicializando..." << std::endl;
    threads.emplace_back(startListener, login);
    threads.emplace_back(startListener, game);
    threads.emplace_back(startListener, secure);
    threads.emplace_back(startUDPListener, udp);

    for (auto& t : threads) t.join();
    return 0;
}

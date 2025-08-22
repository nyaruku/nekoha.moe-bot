#include <string>
#include <stdexcept>
#include <chrono>

#ifdef _WIN32
    #define NOMINMAX
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #include <icmpapi.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/ip_icmp.h>
    #include <sys/socket.h>
    #include <cstring>
#endif

namespace network {
    namespace ping {
    #ifdef _WIN32
        inline long latency(const std::string& host) {
            static bool wsaInitialized = false;
            if (!wsaInitialized) {
                WSADATA wsaData;
                if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                    throw std::runtime_error("WSAStartup failed");
                }
                wsaInitialized = true;
            }

            addrinfo hints{}, *res = nullptr;
            hints.ai_family = AF_INET;
            if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) {
                throw std::runtime_error("DNS resolution failed");
            }

            IPAddr ip = ((sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
            freeaddrinfo(res);

            HANDLE hIcmp = IcmpCreateFile();
            if (hIcmp == INVALID_HANDLE_VALUE) {
                throw std::runtime_error("IcmpCreateFile failed");
            }

            char sendData[] = "icmp ping";
            char replyBuffer[sizeof(ICMP_ECHO_REPLY) + sizeof(sendData)];
            DWORD result = IcmpSendEcho(
                hIcmp,
                ip,
                sendData,
                sizeof(sendData),
                nullptr,
                replyBuffer,
                sizeof(replyBuffer),
                1000
            );

            IcmpCloseHandle(hIcmp);

            if (result != 0) {
                ICMP_ECHO_REPLY* reply = (ICMP_ECHO_REPLY*)replyBuffer;
                return reply->RoundTripTime; // ms
            } else {
                throw std::runtime_error("Ping failed, error=" + std::to_string(GetLastError()));
            }
        }

    #else
        inline unsigned short checksum(void* buffer, int length) {
            unsigned short* buf = (unsigned short*)buffer;
            unsigned int sum = 0;
            for (; length > 1; length -= 2) sum += *buf++;
            if (length == 1) sum += *(unsigned char*)buf;
            sum = (sum >> 16) + (sum & 0xFFFF);
            sum += (sum >> 16);
            return (unsigned short)(~sum);
        }

        inline std::string latency(const std::string& host) {
            // Resolve hostname
            addrinfo hints{}, *res = nullptr;
            hints.ai_family = AF_INET;
            if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) {
                return "DNS resolution failed";
            }

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr = ((sockaddr_in*)res->ai_addr)->sin_addr;
            freeaddrinfo(res);

            int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
            if (sock < 0) {
                return "Failed to create raw socket (need root?)";
            }

            char packet[64];
            memset(packet, 0, sizeof(packet));
            icmphdr* icmp = (icmphdr*)packet;
            icmp->type = ICMP_ECHO;
            icmp->code = 0;
            icmp->un.echo.id = getpid();
            icmp->un.echo.sequence = 1;
            icmp->checksum = checksum(packet, sizeof(packet));

            auto start = std::chrono::steady_clock::now();
            if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&addr, sizeof(addr)) <= 0) {
                close(sock);
                return "sendto failed";
            }

            char buffer[1024];
            socklen_t addr_len = sizeof(addr);
            if (recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&addr, &addr_len) <= 0) {
                close(sock);
                return "recvfrom failed";
            }
            auto end = std::chrono::steady_clock::now();
            close(sock);

            return "Latency to " + host + ": " + 
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
        }
    #endif
    }
}
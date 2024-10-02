#include "Connection.hpp"
#include "Exception.hpp"

#include <ifaddrs.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

Connection::Connection(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol)
    : sourceIp(sourceIp), destIp(destIp), sourcePort(sourcePort), destPort(destPort), protocol(protocol) {
    last_update = chrono::steady_clock::now();
}

void Connection::updateTraffic(int packetSize, bool is_rx) {
    if (is_rx) {
        rx_bytes += packetSize;
        rx_packets++;

    } else {
        tx_bytes += packetSize;
        tx_packets++;
        
    }

    last_update = chrono::steady_clock::now();
}

string Connection::getInterfaceIp(const string& interface, int family) {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        throw Exception(3, "getifaddrs() failed");
        exit(3);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        if (interface == ifa->ifa_name && ifa->ifa_addr->sa_family == family) {
            if (family == AF_INET) {
                struct sockaddr_in *addr = (struct sockaddr_in *) ifa->ifa_addr;
                inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);

            } else if (family == AF_INET6) {
                struct sockaddr_in6 *addr = (struct sockaddr_in6 *) ifa->ifa_addr;
                inet_ntop(AF_INET6, &(addr->sin6_addr), ip, INET6_ADDRSTRLEN);

            }

            freeifaddrs(ifaddr);
            return string(ip);  // Return the IP address
        }
    }

    freeifaddrs(ifaddr);
    return "";  // Return empty string if no match
}
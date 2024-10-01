#include "Connection.hpp"
#include <ifaddrs.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

Connection::Connection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol)
    : src_ip(src_ip), dst_ip(dst_ip), src_port(src_port), dst_port(dst_port), protocol(protocol) {
    last_update = chrono::steady_clock::now();
}

void Connection::updateTraffic(int packet_size, bool is_rx) {
    if (is_rx) {
        rx_bytes += packet_size;
        rx_packets++;

    } else {
        tx_bytes += packet_size;
        tx_packets++;
        
    }

    last_update = chrono::steady_clock::now();
}

string Connection::getInterfaceIp(const string& interface, int family) {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return "";
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
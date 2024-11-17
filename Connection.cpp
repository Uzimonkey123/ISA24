/**
 * @file Connection.cpp
 * @brief Implementation of the Connection class
 * @author Norman Babiak
 * login: xbabia01
 */

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
    lastUpdate = chrono::steady_clock::now();
}

void Connection::updateTraffic(int packetSize, bool is_rx) {
    if (is_rx) {
        rxBytes += packetSize; // Update the received bytes
        rxPackets++; // Update the received packets

    } else {
        txBytes += packetSize; // Update the transmitted bytes
        txPackets++; // Update the transmitted packets
        
    }

    lastUpdate = chrono::steady_clock::now();
}

// Basics of the code taken from: https://stackoverflow.com/questions/4139405/how-can-i-get-to-know-the-ip-address-for-interfaces-in-c
string Connection::getInterfaceIp(const string& interface, int family) {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET6_ADDRSTRLEN];

    // Get the interface IP address
    if (getifaddrs(&ifaddr) == -1) {
        throw Exception(3, "getifaddrs() failed");
        exit(3);
    }

    // Iterate through the interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // Check if the interface matches the specified interface
        if (interface == ifa->ifa_name && ifa->ifa_addr->sa_family == family) {
            if (family == AF_INET) {
                // Get the IPv4 address
                struct sockaddr_in *addr = (struct sockaddr_in *) ifa->ifa_addr;
                inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);

            } else if (family == AF_INET6) {
                // Get the IPv6 address
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
/**
 * @file Packet.cpp
 * @brief Implementation of the Packet class
 * @author Norman Babiak
 * login: xbabia01
 */

#include "Packet.hpp"
#include "ConnectionManager.hpp"

#include <unordered_set>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netdb.h>

void Packet::handlePacket(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    Packet instance;
    instance.handlePacketNonStatic(userData, pkthdr, packet);
}

// Fundamentals of the function are taken from https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
void Packet::handlePacketNonStatic(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    ConnectionManager* connectionManager = reinterpret_cast<ConnectionManager*>(userData); // Get ConnectionManager instance
    const struct ether_header* ethernetHeader = (struct ether_header*) packet;

    switch (ntohs(ethernetHeader->ether_type)) {
        case ETHERTYPE_IP: {  // IPv4 packet
            const struct ip* ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

            // Convert source and destination IP addresses to string
            char sourceIp[INET_ADDRSTRLEN];
            char destIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

            int sourcePort = 0;
            int destPort = 0;
            string protocol = getProtocolNameFromNumber(ipHeader->ip_p);

            // Determine if it's TCP or UDP to get port numbers
            if (ipHeader->ip_p == IPPROTO_TCP) {
                const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);
                sourcePort = ntohs(tcpHeader->source);
                destPort = ntohs(tcpHeader->dest);
                protocol = "tcp";

            } else if (ipHeader->ip_p == IPPROTO_UDP) {
                const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);
                sourcePort = ntohs(udpHeader->source);
                destPort = ntohs(udpHeader->dest);
                protocol = "udp";

            }

            // Store the connection for processing and display
            connectionManager->storeConnection(sourceIp, sourcePort, destIp, destPort, protocol, pkthdr->len, AF_INET);
            break;
        }

        case ETHERTYPE_IPV6: {  // IPv6 packet
            const struct ip6_hdr* ipv6Header = (struct ip6_hdr*)(packet + sizeof(struct ether_header));

            // Convert source and destination IP addresses to string
            char sourceIp[INET6_ADDRSTRLEN];
            char destIp[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &ipv6Header->ip6_src, sourceIp, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &ipv6Header->ip6_dst, destIp, INET6_ADDRSTRLEN);

            int sourcePort = 0;
            int destPort = 0;
            string protocol = getProtocolNameFromNumber(ipv6Header->ip6_nxt);

            // Determine if it's TCP or UDP to get port numbers
            if (ipv6Header->ip6_nxt == IPPROTO_TCP) {
                const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
                sourcePort = ntohs(tcpHeader->source);
                destPort = ntohs(tcpHeader->dest);
                protocol = "tcp";

            } else if (ipv6Header->ip6_nxt == IPPROTO_UDP) {
                const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
                sourcePort = ntohs(udpHeader->source);
                destPort = ntohs(udpHeader->dest);
                protocol = "udp";

            }

            // Store the connection for processing and display
            connectionManager->storeConnection(sourceIp, sourcePort, destIp, destPort, protocol, pkthdr->len, AF_INET6);
            break;
        }

        default:
            // Should not get here due to preset, but in case of unknown Ethernet type
            cerr << "Unsupported Ethernet type: " << ntohs(ethernetHeader->ether_type) << endl;
            break;
    }
}


string Packet::getProtocolNameFromNumber(int protocol_number) {
    struct protoent *proto = getprotobynumber(protocol_number); // Get protocol entry from number

    if (proto) {
        return string(proto->p_name);
    }

    return "unknown";
}
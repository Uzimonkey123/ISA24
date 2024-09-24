#include "Packet.hpp"
#include "ConnectionManager.hpp"

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
            char sourceIp[INET_ADDRSTRLEN];
            char destIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

            std::string protocol;
            int src_port = 0;
            int dst_port = 0;

            if (ipHeader->ip_p == IPPROTO_TCP) {
                const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);
                src_port = ntohs(tcpHeader->th_sport);
                dst_port = ntohs(tcpHeader->th_dport);
                protocol = "tcp";
            } else if (ipHeader->ip_p == IPPROTO_UDP) {
                const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);
                src_port = ntohs(udpHeader->uh_sport);
                dst_port = ntohs(udpHeader->uh_dport);
                protocol = "udp";
            } else if (ipHeader->ip_p == IPPROTO_ICMP) {
                protocol = "icmp";
            }

            // Update connection stats
            connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, true);
            break;
        }

        case ETHERTYPE_IPV6: {  // IPv6 packet
            const struct ip6_hdr* ipv6Header = (struct ip6_hdr*)(packet + sizeof(struct ether_header));
            char sourceIp[INET6_ADDRSTRLEN];
            char destIp[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &ipv6Header->ip6_src, sourceIp, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET6, &ipv6Header->ip6_dst, destIp, INET6_ADDRSTRLEN);

            std::string protocol;
            int src_port = 0;
            int dst_port = 0;

            if (ipv6Header->ip6_nxt == IPPROTO_TCP) {
                const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
                src_port = ntohs(tcpHeader->th_sport);
                dst_port = ntohs(tcpHeader->th_dport);
                protocol = "tcp";
            } else if (ipv6Header->ip6_nxt == IPPROTO_UDP) {
                const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
                src_port = ntohs(udpHeader->uh_sport);
                dst_port = ntohs(udpHeader->uh_dport);
                protocol = "udp";
            } else if (ipv6Header->ip6_nxt == IPPROTO_ICMPV6) {
                protocol = "icmpv6";
            }

            // Update connection stats
            connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, true);
            break;
        }

        default:
            break;
    }
}
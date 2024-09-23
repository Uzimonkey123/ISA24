#include "Packet.hpp"

void Packet::handlePacket(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    Packet instance;
    instance.handlePacketNonStatic(userData, pkthdr, packet);
}

void printConnection(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol) {
    std::string formatted_src_ip = src_ip;
    std::string formatted_dst_ip = dst_ip;

    // If the IP is IPv6, wrap it in square brackets
    if (src_ip.find(":") != std::string::npos) {
        formatted_src_ip = "[" + src_ip + "]";
    }
    if (dst_ip.find(":") != std::string::npos) {
        formatted_dst_ip = "[" + dst_ip + "]";
    }

    // Print IP addresses, ports, and protocol
    std::cout << std::left << std::setw(40) << (formatted_src_ip + ":" + std::to_string(src_port))
              << std::setw(40) << (formatted_dst_ip + ":" + std::to_string(dst_port))
              << std::setw(10) << protocol;

    std::cout << std::setw(8) << "0B/s"  // Rx b/s
              << std::setw(8) << "0p/s"  // Rx p/s
              << std::setw(8) << "0B/s"  // Tx b/s
              << std::setw(8) << "0p/s";  // Tx p/s
}


// Fundamentals of the function are taken from https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
void Packet::handlePacketNonStatic(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    const struct ether_header* ethernetHeader = (struct ether_header*) packet;
    (void)userData;
    (void)pkthdr;
    
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

            printConnection(sourceIp, src_port, destIp, dst_port, protocol);
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

            printConnection(sourceIp, src_port, destIp, dst_port, protocol);
            break;
        }

        default:
            //std::cout << "Unsupported ethernet type: " << std::hex << ntohs(ethernetHeader->ether_type) << std::endl;
            break;
    }
    std::cout << std::endl;
}
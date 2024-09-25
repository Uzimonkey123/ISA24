#include "Packet.hpp"
#include "ConnectionManager.hpp"
#include <unordered_set>

void Packet::handlePacket(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    Packet instance;
    instance.handlePacketNonStatic(userData, pkthdr, packet);
}

std::unordered_set<std::string> getLocalIPAddresses() {
    std::unordered_set<std::string> local_ips;
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *temp_addr = nullptr;

    // Retrieve the current interfaces
    if (getifaddrs(&interfaces) == 0) {
        temp_addr = interfaces;

        while (temp_addr != nullptr) {
            if (temp_addr->ifa_addr->sa_family == AF_INET) {  // Check for IPv4
                char address_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr, address_buffer, INET_ADDRSTRLEN);
                local_ips.insert(std::string(address_buffer));

            } else if (temp_addr->ifa_addr->sa_family == AF_INET6) {  // Check for IPv6
                char address_buffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &((struct sockaddr_in6 *)temp_addr->ifa_addr)->sin6_addr, address_buffer, INET6_ADDRSTRLEN);
                local_ips.insert(std::string(address_buffer));

            }

            temp_addr = temp_addr->ifa_next;
        }
    }
    
    freeifaddrs(interfaces);  // Free the interface list memory
    return local_ips;
}

// Fundamentals of the function are taken from https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
void Packet::handlePacketNonStatic(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    ConnectionManager* connectionManager = reinterpret_cast<ConnectionManager*>(userData);
    const struct ether_header* ethernetHeader = (struct ether_header*) packet;

    static std::unordered_set<std::string> local_ips = getLocalIPAddresses();
    
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

            bool is_rx = local_ips.find(destIp) != local_ips.end();
            bool is_tx = local_ips.find(sourceIp) != local_ips.end();

            if (is_rx) {
                connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, true);
            } else if (is_tx) {
                connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, false);
            }
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

            bool is_rx = local_ips.find(destIp) != local_ips.end();
            bool is_tx = local_ips.find(sourceIp) != local_ips.end();

            if (is_rx) {
                connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, true);
            } else if (is_tx) {
                connectionManager->updateConnection(sourceIp, src_port, destIp, dst_port, protocol, pkthdr->len, false);
            }
            break;
        }

        default:
            break;
    }
}
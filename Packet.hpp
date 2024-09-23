#ifndef PACKET_HPP
#define PACKET_HPP

#include <pcap.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include <cctype>
#include <ctime>
#include <sstream>
#include <string>

class Packet {
    public:
        /**
         * @brief Handle the received packet, function for the pcap loop callback
         * @param userData User data (not used)
         * @param pkthdr Packet header
         * @param packet Packet data
        */
        static void handlePacket(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);

    private:
        /**
         * @brief Handle the received packet, main function for packet handling
         * @param userData User data (not used)
         * @param pkthdr Packet header
         * @param packet Packet data
        */
        void handlePacketNonStatic(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);
};

#endif
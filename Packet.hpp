#ifndef PACKET_HPP
#define PACKET_HPP

#include <pcap.h>
#include <netinet/ether.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

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

        /**
         * @brief Get the protocol name from the protocol number
         * @param protocol_number Protocol number
         * @return Protocol name, unknown if not found
        */
        string getProtocolNameFromNumber(int protocol_number);
};

#endif
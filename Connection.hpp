/**
 * @file Connection.hpp
 * @brief Header file for the Connection class
 * @author Norman Babiak
 * login: xbabia01
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Bandwidth.hpp"
#include <string>
#include <chrono>

using namespace std;

class Connection {
    public:
        string sourceIp;
        string destIp;
        int sourcePort;
        int destPort;
        string protocol;
        uint64_t rxBytes = 0;
        uint64_t txBytes = 0;
        uint64_t rxPackets = 0;
        uint64_t txPackets = 0;
        chrono::steady_clock::time_point lastUpdate; // Last time the connection was updated
        mutable Bandwidth bw; // Bandwidth object to calculate bandwidth and packet rate

        // Default constructor
        Connection() = default;

        /**
         * @brief Construct a new Connection object, used to store connection information
         */
        Connection(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol);
        
        /**
         * @brief Update the traffic statistics for the connection
         * @param packetSize Size of the packet
         * @param is_rx True if the packet is received, false if transmitted
         */
        void updateTraffic(int packetSize, bool is_rx = false);

        /**
         * @brief Get the IP address of the specified interface
         * @param interface Interface name
         * @param family Address family (AF_INET or AF_INET6)
         */
        static string getInterfaceIp(const string& interface, int family);
};

#endif
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
        uint64_t rx_bytes = 0;
        uint64_t tx_bytes = 0;
        uint64_t rx_packets = 0;
        uint64_t tx_packets = 0;
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
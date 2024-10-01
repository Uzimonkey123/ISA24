#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Bandwidth.hpp"
#include <string>
#include <chrono>

using namespace std;

class Connection {
    public:
        string src_ip;
        string dst_ip;
        int src_port;
        int dst_port;
        string protocol;
        uint64_t rx_bytes = 0;
        uint64_t tx_bytes = 0;
        uint64_t rx_packets = 0;
        uint64_t tx_packets = 0;
        chrono::steady_clock::time_point last_update; // Last time the connection was updated
        mutable Bandwidth bw; // Bandwidth object to calculate bandwidth and packet rate

        // Default constructor
        Connection() = default;

        /**
         * @brief Construct a new Connection object, used to store connection information
         */
        Connection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol);
        
        /**
         * @brief Update the traffic statistics for the connection
         * @param packet_size Size of the packet
         * @param is_rx True if the packet is received, false if transmitted
         */
        void updateTraffic(int packet_size, bool is_rx = false);

        /**
         * @brief Get the IP address of the specified interface
         * @param interface Interface name
         * @param family Address family (AF_INET or AF_INET6)
         */
        static string getInterfaceIp(const string& interface, int family);
};

#endif
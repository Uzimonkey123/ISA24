#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Bandwidth.hpp"
#include <string>
#include <chrono>

class Connection {
    public:
        std::string src_ip;
        std::string dst_ip;
        int src_port;
        int dst_port;
        std::string protocol;
        uint64_t rx_bytes = 0;
        uint64_t tx_bytes = 0;
        uint64_t rx_packets = 0;
        uint64_t tx_packets = 0;
        std::chrono::steady_clock::time_point last_update;
        mutable Bandwidth bw;

        Connection() = default;

        Connection(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol);
        
        void updateTraffic(int packet_size, bool is_rx);
};

#endif
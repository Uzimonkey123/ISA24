#include "Connection.hpp"

Connection::Connection(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol)
    : src_ip(src_ip), dst_ip(dst_ip), src_port(src_port), dst_port(dst_port), protocol(protocol) {
    last_update = std::chrono::steady_clock::now();
}

void Connection::updateTraffic(int packet_size, bool is_rx) {
    if (is_rx) {
        rx_bytes += packet_size;
        rx_packets++;

    } else {
        tx_bytes += packet_size;
        tx_packets++;
        
    }

    last_update = std::chrono::steady_clock::now();
}
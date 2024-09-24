#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <string>
#include <map>
#include <ncurses.h>
#include <chrono>

// Structure to store connection statistics
struct Connection {
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
};

class ConnectionManager {
private:
    std::map<std::string, Connection> connections;
    std::string generateConnectionKey(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol);

public:
    ConnectionManager();

    void updateConnection(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol, int packet_size, bool is_rx);

    void displayConnections();
};

#endif
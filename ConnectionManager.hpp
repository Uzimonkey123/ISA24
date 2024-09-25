#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "Bandwidth.hpp"

#include <string>
#include <map>
#include <ncurses.h>
#include <chrono>
#include <mutex>
#include <vector>

using namespace std;

// Structure to store connection statistics
struct Connection {
    string src_ip;
    string dst_ip;
    int src_port;
    int dst_port;
    string protocol;
    uint64_t rx_bytes = 0;
    uint64_t tx_bytes = 0;
    uint64_t rx_packets = 0;
    uint64_t tx_packets = 0;
    chrono::steady_clock::time_point last_update;
    mutable Bandwidth bw = Bandwidth();
};

class ConnectionManager {
private:
    struct SavedConnection {
        Connection conn;
        double rx_bps;
        double tx_bps;
        double rx_pps;
        double tx_pps;
    };

    mutex conn_mutex;
    map<string, Connection> connections;

public:
    ConnectionManager();

    void updateConnection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol, int packet_size, bool is_rx);

    void updateConnectionDetails(Connection& conn, const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol);

    void updateTraffic(Connection& conn, int packet_size, bool is_rx);

    vector<SavedConnection> getActiveConnections(bool sort_by_bytes);

    void displayConnections(bool sort_by_bytes);
};

#endif
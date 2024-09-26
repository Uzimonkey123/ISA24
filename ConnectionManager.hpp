#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "Bandwidth.hpp"
#include "Connection.hpp"

#include <string>
#include <map>
#include <ncurses.h>
#include <chrono>
#include <mutex>
#include <vector>

using namespace std;

class ConnectionManager {
    private:
        struct SavedConnection {
            Connection conn;
            double rx_bps = 0;
            double tx_bps = 0;
            double rx_pps = 0;
            double tx_pps = 0;

            SavedConnection(const Connection& c) : conn(c) {}
        };

        mutex conn_mutex;
        std::map<std::string, Connection> connections;

    public:
        ConnectionManager();

        void updateConnection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol, int packet_size, bool is_rx);

        vector<SavedConnection> getActiveConnections(bool sort_by_bytes);

        void displayConnections(bool sort_by_bytes);
};

#endif
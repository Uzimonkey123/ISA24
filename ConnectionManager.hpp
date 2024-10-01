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

/**
 * @brief Set the global interface to process interface IP
 * @param interface Interface name
 */
void setGlobalInterface(const string& interface);

class ConnectionManager {
    private:
        mutex conn_mutex; // Mutex to protect the connections map
        map<string, Connection> connections; // Map to store connections

    public:
        /**
         * @brief Store a connection in the connection manager
         */
        struct SavedConnection {
            Connection conn;
            double rx_bps = 0;
            double tx_bps = 0;
            double rx_pps = 0;
            double tx_pps = 0;

            /**
             * @brief Construct a new Saved Connection object
             * @param c Connection object
             */
            SavedConnection(const Connection& c) : conn(c) {}
        };

        /**
         * @brief Store a connection in the connection manager class for further reaching
         * @param src_ip Source IP address
         * @param src_port Source port
         * @param dst_ip Destination IP address
         * @param dst_port Destination port
         * @param protocol Protocol name
         * @param packet_size Size of the packet
         * @param family Address family (AF_INET or AF_INET6)
         */
        void storeConnection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol, int packet_size, int family);

        /**
         * @brief Get the active connections ready to display
         * @param sort_by_bytes True to sort by bytes, false to sort by packets
         * @return vector<SavedConnection> List of active connections
         */
        vector<SavedConnection> getActiveConnections(bool sort_by_bytes);
};

#endif
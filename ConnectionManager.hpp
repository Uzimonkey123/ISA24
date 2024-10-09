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

        /**
         * @brief Classify the traffic as Rx, Tx or undecided
         * @param connection Connection object
         * @param sourceIp Source IP address
         * @param destIp Destination IP address
         * @param packetSize Size of the packet
         * @param interfaceIp Interface IP address
         */
        void trafficRoute(Connection& connection, const string& sourceIp, const string& destIp, int packetSize, const string& interfaceIp);

        /**
         * @brief Manage a connection by creating a new one or returning an existing one
         * @param forwardKey Key for the forward direction
         * @param reverseKey Key for the reverse direction
         * @param sourceIp Source IP address
         * @param sourcePort Source port
         * @param destIp Destination IP address
         * @param destPort Destination port
         * @param protocol Protocol name
         * @return Connection& Reference to the connection object
         */
        Connection& manageConnection(const string& forwardKey, const string& reverseKey, const string& sourceIp, int sourcePort, const string& destIp, 
                                                            int destPort, const string& protocol);

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
         * @param sourceIp Source IP address
         * @param sourcePort Source port
         * @param destIp Destination IP address
         * @param destPort Destination port
         * @param protocol Protocol name
         * @param packetSize Size of the packet
         * @param family Address family (AF_INET or AF_INET6)
         */
        void storeConnection(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol, int packetSize, int family);

        /**
         * @brief Get the active connections ready to display
         * @param sort_by_bytes True to sort by bytes, false to sort by packets
         * @return vector<SavedConnection> List of active connections
         */
        vector<SavedConnection> getActiveConnections(bool sort_by_bytes);

        /**
         * @brief Sort the connections by bytes or packets
         * @param connections List of connections
         * @param sortByBytes True to sort by bytes, false to sort by packets
         */
        void sortConnections(vector<SavedConnection>& connections, bool sortByBytes) const;
};

#endif
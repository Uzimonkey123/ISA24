#include "ConnectionManager.hpp"
#include "KeyGenerator.hpp"

#include <algorithm>
#include <mutex>
#include <iostream>

string globalInterface = ""; // Global interface to process interface IP

void setGlobalInterface(const string& interface) {
    globalInterface = interface;
}

Connection& ConnectionManager::manageConnection(const string& forwardKey, const string& reverseKey, const string& sourceIp, int sourcePort, const string& destIp, 
                                                int destPort, const string& protocol) {
    // Attempt to find the connection using forward or reverse keys
    auto it = connections.find(forwardKey);
    if (it != connections.end()) {
        return it->second;
    }

    it = connections.find(reverseKey);
    if (it != connections.end()) {
        return it->second;
    }

    // Create a new connection if not found and insert it into the map
    auto [newIt, inserted] = connections.emplace(forwardKey, Connection(sourceIp, sourcePort, destIp, destPort, protocol));

    // Return the connection
    return newIt->second;
}

void ConnectionManager::storeConnection(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol, int packetSize, int family) {
    lock_guard<mutex> lock(conn_mutex); // Lock the connections map

    // Generate keys for both directions
    KeyGenerator keyGen;
    string forwardKey = keyGen.generateKey(sourceIp, sourcePort, destIp, destPort, protocol);
    string reverseKey = keyGen.generateKey(destIp, destPort, sourceIp, sourcePort, protocol);

    // Retrieve or create the connection
    Connection& connection = manageConnection(forwardKey, reverseKey, sourceIp, sourcePort, destIp, destPort, protocol);

    // Get the interface IP
    string interfaceIp = Connection::getInterfaceIp(globalInterface, family);

    // Update the traffic route for the connection
    trafficRoute(connection, sourceIp, destIp, packetSize, interfaceIp);
}

void ConnectionManager::trafficRoute(Connection& connection, const string& sourceIp, const string& destIp, int packetSize, const string& interfaceIp) {
    if (sourceIp == interfaceIp) {
        connection.updateTraffic(packetSize, false); // Tx

    } else if (destIp == interfaceIp) {
        connection.updateTraffic(packetSize, true); // Rx

    } else {
        connection.updateTraffic(packetSize); // Undecided (tx)
        
    }
}

vector<ConnectionManager::SavedConnection> ConnectionManager::getActiveConnections(bool sort_by_bytes) {
    vector<SavedConnection> connList;

    for (auto& it : connections) {
        Connection& conn = it.second;
        SavedConnection sc(conn);
        sc.conn = conn;

        // Calculate bandwidth and packet rate
        conn.bw.calculateBandwidth(conn.rx_bytes, conn.tx_bytes, conn.rx_packets, conn.tx_packets,
                                   sc.rx_bps, sc.tx_bps, sc.rx_pps, sc.tx_pps);

        // Skip if both rx and tx rates are zero
        if (sc.rx_bps == 0 && sc.tx_bps == 0 && sc.rx_pps == 0 && sc.tx_pps == 0) {
            continue;
        }
        
        connList.push_back(sc);
    }

    // Sort based on bytes or packets
    sortConnections(connList, sort_by_bytes);

    return connList;
}

void ConnectionManager::sortConnections(vector<SavedConnection>& connections, bool sortByBytes) const {
    auto comparator = sortByBytes
        ? [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_bps + a.tx_bps) > (b.rx_bps + b.tx_bps);
            }

        : [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_pps + a.tx_pps) > (b.rx_pps + b.tx_pps);
            };
    sort(connections.begin(), connections.end(), comparator);
}
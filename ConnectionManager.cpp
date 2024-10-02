#include "ConnectionManager.hpp"
#include "KeyGenerator.hpp"

#include <algorithm>
#include <mutex>
#include <iostream>

string globalInterface = ""; // Global interface to process interface IP

void setGlobalInterface(const string& interface) {
    globalInterface = interface;
}

void ConnectionManager::storeConnection(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol, int packetSize, int family) {
    lock_guard<mutex> lock(conn_mutex); // Lock the connections map

    // Generate the key, both ways for bi-directional traffic
    KeyGenerator keyGen;
    string key = keyGen.generateKey(sourceIp, sourcePort, destIp, destPort, protocol);
    string reverseKey = keyGen.generateKey(destIp, destPort, sourceIp, sourcePort, protocol);

    // Get the interface IP
    string interfaceIp = Connection::getInterfaceIp(globalInterface, family);
    
    // Check if the connection already exists
    auto it = connections.find(key);
    if (it != connections.end()) {
        trafficRoute(it->second, sourceIp, destIp, packetSize, interfaceIp);
        return;
    }

    // Same check, but in the other direction
    it = connections.find(reverseKey);
    if (it != connections.end()) {
        trafficRoute(it->second, sourceIp, destIp, packetSize, interfaceIp);
        return;
    }

    // Create a new connection if it doesn't exist
    connections[key] = Connection(sourceIp, sourcePort, destIp, destPort, protocol);
    trafficRoute(connections[key], sourceIp, destIp, packetSize, interfaceIp);
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
    auto custom_sort = sort_by_bytes
        ? [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_bps + a.tx_bps) > (b.rx_bps + b.tx_bps); // Sort by bytes
        }
        
        : [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_pps + a.tx_pps) > (b.rx_pps + b.tx_pps); // Sort by packets
        };

    sort(connList.begin(), connList.end(), custom_sort);

    return connList;
}
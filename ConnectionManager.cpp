#include "ConnectionManager.hpp"
#include "KeyGenerator.hpp"

#include <algorithm>
#include <mutex>
#include <iostream>

string globalInterface = ""; // Global interface to process interface IP

void setGlobalInterface(const string& interface) {
    globalInterface = interface;
}

void ConnectionManager::storeConnection(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol, int packet_size, int family) {
    lock_guard<mutex> lock(conn_mutex);

    KeyGenerator keyGen;
    string key = keyGen.generateKey(src_ip, src_port, dst_ip, dst_port, protocol);
    string reverseKey = keyGen.generateKey(dst_ip, dst_port, src_ip, src_port, protocol);

    // Get the interface IP to check if the packet matches the interface
    string interfaceIp = Connection::getInterfaceIp(globalInterface, family);

    // TODO: Implement the connection store logic (check it with interface IP too)
}

vector<ConnectionManager::SavedConnection> ConnectionManager::getActiveConnections(bool sort_by_bytes) {
    vector<SavedConnection> conn_list;

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
        
        conn_list.push_back(sc);
    }

    // Sort based on bytes or packets
    auto custom_sort = sort_by_bytes
        ? [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_bps + a.tx_bps) > (b.rx_bps + b.tx_bps); // Sort by bytes
        }
        
        : [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_pps + a.tx_pps) > (b.rx_pps + b.tx_pps); // Sort by packets
        };

    sort(conn_list.begin(), conn_list.end(), custom_sort);

    return conn_list;
}
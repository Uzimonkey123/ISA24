#include "ConnectionManager.hpp"
#include "KeyGenerator.hpp"

#include <algorithm>
#include <mutex>
#include <iostream>

ConnectionManager::ConnectionManager() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    
    curs_set(0);
    mousemask(0, NULL);
}

void ConnectionManager::updateConnection(const std::string& src_ip, int src_port, const std::string& dst_ip, int dst_port, const std::string& protocol, int packet_size, bool is_rx) {
    std::lock_guard<std::mutex> lock(conn_mutex);

    KeyGenerator keyGen;
    std::string key = keyGen.generateKey(src_ip, src_port, dst_ip, dst_port, protocol);

    // Check if connection exists or create a new one
    if (connections.find(key) == connections.end()) {
        connections[key] = Connection(src_ip, src_port, dst_ip, dst_port, protocol);
    }

    connections[key].updateTraffic(packet_size, is_rx);
}

void ConnectionManager::updateConnectionDetails(Connection& conn, const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol) {
    conn.src_ip = src_ip;
    conn.dst_ip = dst_ip;
    conn.src_port = src_port;
    conn.dst_port = dst_port;
    conn.protocol = protocol;
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
    if (sort_by_bytes) {
        sort(conn_list.begin(), conn_list.end(), [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_bps + a.tx_bps) > (b.rx_bps + b.tx_bps);
        });

    } else {
        sort(conn_list.begin(), conn_list.end(), [](const SavedConnection& a, const SavedConnection& b) {
            return (a.rx_pps + a.tx_pps) > (b.rx_pps + b.tx_pps);
        });

    }

    return conn_list;
}

void ConnectionManager::displayConnections(bool sort_by_bytes) {
    lock_guard<mutex> lock(conn_mutex);

    clear();
    mvprintw(0, 0, "ISA-TOP");
    mvprintw(1, 0, "%-21s %-21s %-9s %-10s %-10s %-8s %-8s", 
             "Src IP:Port", "Dst IP:Port", "Protocol", "Rx Rate", "Tx Rate", "Rx P/s", "Tx P/s");

    int row = 2;
    auto sc_list = getActiveConnections(sort_by_bytes);

    // Display the connections
    for (size_t i = 0; i < min(sc_list.size(), size_t(10)); ++i) {
        const SavedConnection& sc = sc_list[i];
        const Connection& conn = sc.conn;

        string rx_bytes_rate = conn.bw.formatBandwidth(sc.rx_bps * 8);  // Convert to bits per second
        string tx_bytes_rate = conn.bw.formatBandwidth(sc.tx_bps * 8);
        string rx_packet_rate = conn.bw.formatPacketRate(sc.rx_pps);
        string tx_packet_rate = conn.bw.formatPacketRate(sc.tx_pps);

        // Fixed width columns for proper alignment
        mvprintw(row, 0, "%-21s %-21s %-9s %-10s %-10s %-8s %-8s",
                 (conn.src_ip + ":" + to_string(conn.src_port)).c_str(),
                 (conn.dst_ip + ":" + to_string(conn.dst_port)).c_str(),
                 conn.protocol.c_str(),
                 rx_bytes_rate.c_str(), tx_bytes_rate.c_str(),
                 rx_packet_rate.c_str(), tx_packet_rate.c_str());

        row++;
    }

    refresh();
}
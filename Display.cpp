#include "Display.hpp"
#include <ncurses.h>

Display::Display() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    
    curs_set(0);
    mousemask(0, NULL);
}

void Display::displayConnections(const std::vector<ConnectionManager::SavedConnection>& connections) {
    clear();
    mvprintw(0, 0, "ISA-TOP");
    mvprintw(1, 0, "%-45s %-45s %-9s %-10s %-10s %-8s %-8s", 
             "Src IP:Port", "Dst IP:Port", "Protocol", "Rx Rate", "Tx Rate", "Rx P/s", "Tx P/s");

    int row = 2;

    // Display the connections
    for (size_t i = 0; i < std::min(connections.size(), size_t(10)); ++i) {
        const ConnectionManager::SavedConnection& sc = connections[i];
        const Connection& conn = sc.conn;

        std::string rx_bytes_rate = conn.bw.formatBandwidth(sc.rx_bps * 8);  // Convert to bits per second
        std::string tx_bytes_rate = conn.bw.formatBandwidth(sc.tx_bps * 8);
        std::string rx_packet_rate = conn.bw.formatPacketRate(sc.rx_pps);
        std::string tx_packet_rate = conn.bw.formatPacketRate(sc.tx_pps);

        // Fixed width columns for proper alignment
        mvprintw(row, 0, "%-45s %-45s %-9s %-10s %-10s %-8s %-8s",
                 (conn.sourceIp + ":" + std::to_string(conn.sourcePort)).c_str(),
                 (conn.destIp + ":" + std::to_string(conn.destPort)).c_str(),
                 conn.protocol.c_str(),
                 rx_bytes_rate.c_str(), tx_bytes_rate.c_str(),
                 rx_packet_rate.c_str(), tx_packet_rate.c_str());

        row++;
    }

    refresh();
}
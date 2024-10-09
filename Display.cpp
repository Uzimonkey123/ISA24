#include "Display.hpp"
#include <ncurses.h>

using namespace std;

Display::Display() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    
    curs_set(0); // Hide the cursor
    mousemask(0, NULL); // Disable mouse events
}

void Display::displayConnections(const vector<ConnectionManager::SavedConnection>& connections) {
    clear();
    mvprintw(0, 0, "ISA-TOP");
    mvprintw(1, 0, "%-45s %-45s %-9s %-10s %-10s %-8s %-8s", 
             "Src IP:Port", "Dst IP:Port", "Protocol", "Rx Rate", "Tx Rate", "Rx P/s", "Tx P/s");

    int row = 2; // Start at row 3

    // Display the connections
    for (size_t i = 0; i < min(connections.size(), size_t(10)); ++i) {
        // Get the connection and its bandwidth statistics
        const ConnectionManager::SavedConnection& sc = connections[i];
        const Connection& conn = sc.conn;

        string rxBytesRate = conn.bw.formatBandwidth(sc.rx_bps * 8);  // Convert to bits per second
        string txBytesRate = conn.bw.formatBandwidth(sc.tx_bps * 8);
        string rxPacketRate = conn.bw.formatPacketRate(sc.rx_pps);
        string txPacketRate = conn.bw.formatPacketRate(sc.tx_pps);

        // Fixed width columns for proper alignment
        mvprintw(row, 0, "%-45s %-45s %-9s %-10s %-10s %-8s %-8s",
                 (conn.sourceIp + ":" + to_string(conn.sourcePort)).c_str(),
                 (conn.destIp + ":" + to_string(conn.destPort)).c_str(),
                 conn.protocol.c_str(),
                 rxBytesRate.c_str(), txBytesRate.c_str(),
                 rxPacketRate.c_str(), txPacketRate.c_str());

        row++;
    }

    refresh();
}
#include "parser.hpp"
#include "Exception.hpp"
#include "Packet.hpp"
#include "ConnectionManager.hpp"

#include <pcap.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <signal.h>
#include <unistd.h>

pcap_t* global_pcap_handle = nullptr;
ConnectionManager* connManagerPtr = nullptr;
int interval = 1;
bool sort_by_bytes = true;

// Signal handler to cleanly exit on Ctrl+C
void signalHandler(int signum) {
    (void) signum;

    if (global_pcap_handle != nullptr) {
        pcap_breakloop(global_pcap_handle);
        pcap_close(global_pcap_handle);
        global_pcap_handle = nullptr;
    }

    endwin();
    exit(0);
}

// Display refresh function called periodically by alarm
void refreshDisplay(int signum) {
    (void) signum;

    if (connManagerPtr != nullptr) {
        connManagerPtr->displayConnections(sort_by_bytes);  // Refresh the screen with ncurses
    }

    alarm(interval);  // Set the next alarm to refresh the display
}

void packetCapture(ConnectionManager* connManager, const std::string& interface) {
    char errbuf[PCAP_ERRBUF_SIZE];

    global_pcap_handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (global_pcap_handle == nullptr) {
        throw Exception(2, "pcap_open_live() failed: " + std::string(errbuf));
    }


    pcap_loop(global_pcap_handle, 0, Packet::handlePacket, reinterpret_cast<u_char*>(connManager));
    if (global_pcap_handle != nullptr) {
        pcap_close(global_pcap_handle);
        global_pcap_handle = nullptr;
    }
}

int main(int argc, char* argv[]) {
    Parser ArgParse = Parser();

    try {
        ArgParse.parse(argc, argv);
    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
        return e.getCode();
    }

    interval = ArgParse.getInterval();
    sort_by_bytes = ArgParse.getOrderBy() == 'b';

    ConnectionManager connManager = ConnectionManager();
    connManagerPtr = &connManager;

    signal(SIGINT, signalHandler);  // Handle Ctrl+C
    signal(SIGALRM, refreshDisplay);  // Handle screen refresh

    // Alarm for first refresh screen
    alarm(1);

    // Start packet capture in a separate thread
    std::thread captureThread(packetCapture, &connManager, ArgParse.getInterface());

    captureThread.join();
    return 0;
}
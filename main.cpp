#include "parser.hpp"
#include "Exception.hpp"
#include "Packet.hpp"
#include "ConnectionManager.hpp"
#include "Display.hpp"

#include <pcap.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <signal.h>
#include <unistd.h>

using namespace std;

pcap_t* global_pcap_handle = nullptr;
Display* displayPtr = nullptr;
ConnectionManager* connManagerPtr = nullptr;
int interval = 1;
bool sort_by_bytes = true;

// Signal handler to cleanly exit on Ctrl+C
void signalHandler(int signum) {
    (void) signum;

    if (global_pcap_handle != nullptr) { // Close the pcap handle
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

    if (displayPtr != nullptr && connManagerPtr != nullptr) {
        auto activeConnections = connManagerPtr->getActiveConnections(sort_by_bytes);
        displayPtr->displayConnections(activeConnections);  // Refresh the screen with ncurses
    }

    alarm(interval);  // Set the next alarm to refresh the display
}

void packetCapture(ConnectionManager* connManager, const string& interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program filter;

    // Open the device for capturing
    global_pcap_handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (global_pcap_handle == nullptr) {
        throw Exception(2, "pcap_open_live() failed: " + string(errbuf));
    }

    if (pcap_compile(global_pcap_handle, &filter, "ip or ip6", 0, PCAP_NETMASK_UNKNOWN) == -1) {
        throw Exception(2, "pcap_compile() failed: " + string(pcap_geterr(global_pcap_handle)));
    }

    // Set the compiled filter
    if (pcap_setfilter(global_pcap_handle, &filter) == -1) {
        throw Exception(2, "pcap_setfilter() failed: " + string(pcap_geterr(global_pcap_handle)));
    }

    // Start capturing packets
    pcap_loop(global_pcap_handle, 0, Packet::handlePacket, reinterpret_cast<u_char*>(connManager));

    // Close the capture handle after capturing
    if (global_pcap_handle != nullptr) {
        pcap_close(global_pcap_handle);
        global_pcap_handle = nullptr;
    }
}

int main(int argc, char* argv[]) {
    Parser ArgParse;

    try {
        ArgParse.parse(argc, argv);
    } catch (Exception &e) {
        cerr << e.what() << endl;
        return e.getCode();
    }

    interval = ArgParse.getInterval();
    sort_by_bytes = ArgParse.getOrderBy() == 'b';
    setGlobalInterface(ArgParse.getInterface());

    ConnectionManager connManager;
    connManagerPtr = &connManager;
    
    Display display;
    displayPtr = &display;

    signal(SIGINT, signalHandler);   // Handle Ctrl+C for graceful exit
    signal(SIGALRM, refreshDisplay); // Handle screen refresh

    // Alarm for first refresh of the screen
    alarm(interval);

    // Start packet capture in a separate thread
    thread captureThread(packetCapture, &connManager, ArgParse.getInterface());
    captureThread.join();

    return 0;
}
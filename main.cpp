/**
 * @file main.cpp
 * @brief Main file for the ISA-TOP program
 * @author Norman Babiak
 * login: xbabia01
 */

#include "parser.hpp"
#include "Exception.hpp"
#include "Packet.hpp"
#include "ConnectionManager.hpp"
#include "Display.hpp"

// Libraries for packet capture
#include <pcap.h>
#include <iostream>
#include <signal.h> // Signal handling
#include <unistd.h>

// Libraries for thread synchronization
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

// Global variables
pcap_t* globalPcapHandle = nullptr;
Display* displayPtr = nullptr; // To pass into the signal handler
ConnectionManager* connManagerPtr = nullptr; // To pass into the packet capture thread
int interval = 1; // To pass into the signal handler
bool sort_by_bytes = true; // To pass into the signal handler
atomic<bool> threadStop(false);
mutex globalMutex;

// Signal handler to cleanly exit on Ctrl+C
void signalHandler(int signum) {
    (void)signum;

    threadStop.store(true); // Signal the main thread to stop
    {
        lock_guard<mutex> lock(globalMutex);

        if (globalPcapHandle != nullptr) { // Stop the packet capture
            pcap_breakloop(globalPcapHandle);
        }
    }
}

// Display refresh function called periodically by alarm
void refreshDisplay(int signum) {
    (void)signum;

    if (displayPtr != nullptr && connManagerPtr != nullptr) {
        auto activeConnections = connManagerPtr->getActiveConnections(sort_by_bytes);
        displayPtr->displayConnections(activeConnections);  // Refresh the screen with ncurses
    }

    alarm(interval);  // Set the next alarm to refresh the display
}

void packetCapture(ConnectionManager* connManager, const string& interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program filter;

    { // Lock the global variables
        lock_guard<mutex> lock(globalMutex);

        // Open the device for capturing
        globalPcapHandle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
        if (globalPcapHandle == nullptr) {
            throw Exception(2, "pcap_open_live() failed: " + string(errbuf));
        }

        if (pcap_compile(globalPcapHandle, &filter, "ip or ip6", 0, PCAP_NETMASK_UNKNOWN) == -1) {
            throw Exception(2, "pcap_compile() failed: " + string(pcap_geterr(globalPcapHandle)));
        }

        // Set the compiled filter
        if (pcap_setfilter(globalPcapHandle, &filter) == -1) {
            throw Exception(2, "pcap_setfilter() failed: " + string(pcap_geterr(globalPcapHandle)));
        }
    }

    // Start capturing packets
    pcap_loop(globalPcapHandle, 0, Packet::handlePacket, reinterpret_cast<u_char*>(connManager));

    {
        lock_guard<mutex> lock(globalMutex);

        // Close the capture handle after capturing
        if (globalPcapHandle != nullptr) { 
            pcap_close(globalPcapHandle);
            globalPcapHandle = nullptr;
        }
    }
}

int main(int argc, char* argv[]) {
    Parser ArgParse;

    try {
        ArgParse.parse(argc, argv);
    } catch (Exception& e) {
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

    signal(SIGINT, signalHandler);   // Handle Ctrl+C
    signal(SIGALRM, refreshDisplay); // Refresh the display periodically
    
    // Alarm for first refresh of the screen
    alarm(interval);

    // Start packet capture in a separate thread
    thread captureThread(packetCapture, &connManager, ArgParse.getInterface());

    // Part to avoid the main thread from exiting while the capture thread is running (valgrind errors)
    while (!threadStop.load()) {
        this_thread::sleep_for(chrono::milliseconds(100)); // Sleep to reduce CPU usage
    }

    // Wait for the capture thread to finish
    if (captureThread.joinable()) {
        captureThread.join();
    }

    // Cleanup ncurses display
    endwin();

    return 0;
}

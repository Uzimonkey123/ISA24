#include "parser.hpp"
#include "Exception.hpp"
#include "Packet.hpp"
#include "ConnectionManager.hpp"

#include <pcap.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <ctime>
#include <vector>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <signal.h>
#include <unistd.h>

pcap_t* global_pcap_handle = nullptr;
ConnectionManager* connManagerPtr = nullptr;
int interval = 1;

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

void refreshDisplay(int signum) {
    (void) signum;

    if (connManagerPtr != nullptr) {
        connManagerPtr->displayConnections();
    }

    alarm(interval);
}

int main(int argc, char* argv[]) {
    Parser ArgParse = Parser();
    char errbuf[PCAP_ERRBUF_SIZE];

    try {
        ArgParse.parse(argc, argv);
    } catch (Exception &e) {
        std::cerr << e.what() << std::endl;
        return e.getCode();
    }

    // Debug output
    std::cout << "Interface: " << ArgParse.getInterface() << std::endl;
    std::cout << "Order by: " << ArgParse.getOrderBy() << std::endl;
    std::cout << "Interval: " << ArgParse.getInterval() << std::endl;

    interval = ArgParse.getInterval();

    ConnectionManager connManager = ConnectionManager();
    connManagerPtr = &connManager;

    signal(SIGINT, signalHandler);  // Handle Ctrl+C
    signal(SIGALRM, refreshDisplay);  // Handle screen refresh

    // Open the network interface for packet capture
    global_pcap_handle = pcap_open_live(ArgParse.getInterface().c_str(), BUFSIZ, 1, 1000, errbuf);
    if (global_pcap_handle == nullptr) {
        throw Exception(2, "pcap_open_live() failed: " + std::string(errbuf));
    }

    // Alarm for first refresh screen
    alarm(1);

    pcap_loop(global_pcap_handle, 0, Packet::handlePacket, reinterpret_cast<u_char*>(&connManager));

    pcap_close(global_pcap_handle);
    return 0;
}
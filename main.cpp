#include "parser.hpp"
#include "Exception.hpp"
#include "Packet.hpp"

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

pcap_t* global_pcap_handle = nullptr;

void signalHandler(int signum) {
    (void)signum;

    if (global_pcap_handle != nullptr) {
        pcap_breakloop(global_pcap_handle);
        pcap_close(global_pcap_handle);
        global_pcap_handle = nullptr;
    }

    // Exit program
    exit(0);
}

int main(int argc, char* argv[]) {
    Parser ArgParse = Parser();

    try {
        ArgParse.parse(argc, argv);
    } catch(Exception &e) {
        std::cerr << e.what() << std::endl;
        return e.getCode();
    }

    std::cout << "Interface: " << ArgParse.getInterface() << std::endl;
    std::cout << "Order by: " << ArgParse.getOrderBy() << std::endl;
    std::cout << "Interval: " << ArgParse.getInterval() << std::endl;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(ArgParse.getInterface().c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Failed to open device " << ArgParse.getInterface() << ": " << errbuf << std::endl;
        return 2;
    }

    // Set the global handle
    global_pcap_handle = handle;

    // Register the signal handler for SIGINT
    signal(SIGINT, signalHandler);
    
    // Start capturing packets
    pcap_loop(handle, 5, Packet::handlePacket, nullptr);

    // Cleanup on normal exit
    pcap_close(handle);
    return 0;
}
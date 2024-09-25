#ifndef BANDWIDTH_HPP
#define BANDWIDTH_HPP

#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

class Bandwidth {
    private:
        mutable uint64_t rx_bytes;
        mutable uint64_t tx_bytes;
        mutable uint64_t rx_packets;
        mutable uint64_t tx_packets;
        mutable chrono::steady_clock::time_point update_time;

        double calculateRate(uint64_t current, uint64_t previous, double elapsed_time);

    public:
        Bandwidth() : rx_bytes(0), tx_bytes(0), rx_packets(0), tx_packets(0) {
            update_time = chrono::steady_clock::now();
        }

        void calculateBandwidth(uint64_t current_rx_bytes, uint64_t current_tx_bytes, uint64_t current_rx_packets, uint64_t current_tx_packets, 
                                double& rx_bps, double& tx_bps, double& rx_pps, double& tx_pps);

        string formatBandwidth(double bps);

        string formatPacketRate(double pps);
};

#endif
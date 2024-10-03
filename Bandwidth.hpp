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
        // Bandwidth counters
        mutable uint64_t rx_bytes;
        mutable uint64_t tx_bytes;
        mutable uint64_t rx_packets;
        mutable uint64_t tx_packets;
        mutable chrono::steady_clock::time_point updateTime;

        /**
         * @brief Calculate the rate of change between two values
         * @param current The current value
         * @param previous The previous value
         * @param elapsedTime The time elapsed between the two values
         * @return The rate of change
         */
        double calculateRate(uint64_t current, uint64_t previous, double elapsedTime);

    public:
        /**
         * @brief Construct a new Bandwidth object
         */
        Bandwidth() : rx_bytes(0), tx_bytes(0), rx_packets(0), tx_packets(0) {
            updateTime = chrono::steady_clock::now();
        }

        /**
         * @brief Calculate the bandwidth and packet rate
         * @param current_rx_bytes The current number of received bytes
         * @param current_tx_bytes The current number of transmitted bytes
         * @param current_rx_packets The current number of received packets
         * @param current_tx_packets The current number of transmitted packets
         * @param rx_bps The received bandwidth in bits per second
         * @param tx_bps The transmitted bandwidth in bits per second
         * @param rx_pps The received packet rate in packets per second
         * @param tx_pps The transmitted packet rate in packets per second
         */
        void calculateBandwidth(uint64_t current_rx_bytes, uint64_t current_tx_bytes, uint64_t current_rx_packets, uint64_t current_tx_packets, 
                                double& rx_bps, double& tx_bps, double& rx_pps, double& tx_pps);

        /**
         * @brief Format the bandwidth into a human-readable string
         * @param bps The bandwidth in bits per second
         */
        string formatBandwidth(double bps);

        /**
         * @brief Format the packet rate into a human-readable string
         * @param pps The packet rate in packets per second
         */
        string formatPacketRate(double pps);
};

#endif
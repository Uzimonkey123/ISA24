#include "Bandwidth.hpp"

double Bandwidth::calculateRate(uint64_t current, uint64_t previous, double elapsedTime) {
    return static_cast<double>(current - previous) / elapsedTime;
}

void Bandwidth::calculateBandwidth(uint64_t current_rx_bytes, uint64_t current_tx_bytes, uint64_t current_rx_packets, uint64_t current_tx_packets, 
                                    double& rx_bps, double& tx_bps, double& rx_pps, double& tx_pps) {
    auto now = chrono::steady_clock::now();
    double elapsedTime = chrono::duration<double>(now - updateTime).count();

    if (elapsedTime > 0) {
        rx_bps = calculateRate(current_rx_bytes, rx_bytes, elapsedTime);
        tx_bps = calculateRate(current_tx_bytes, tx_bytes, elapsedTime);

        rx_pps = calculateRate(current_rx_packets, rx_packets, elapsedTime);
        tx_pps = calculateRate(current_tx_packets, tx_packets, elapsedTime);
    }

    rx_bytes = current_rx_bytes;
    tx_bytes = current_tx_bytes;
    rx_packets = current_rx_packets;
    tx_packets = current_tx_packets;
    updateTime = now;
}

string Bandwidth::formatBandwidth(double bps) {
    ostringstream oss;

    if (bps >= 1e9) {
        oss << fixed << setprecision(1) << (bps / 1e9) << " Gbps";

    } else if (bps >= 1e6) {
        oss << fixed << setprecision(1) << (bps / 1e6) << " Mbps";

    } else if (bps >= 1e3) {
        oss << fixed << setprecision(1) << (bps / 1e3) << " Kbps";

    } else {
        oss << fixed << setprecision(1) << bps << " bps";

    }

    return oss.str();
}

string Bandwidth::formatPacketRate(double pps) {
    ostringstream oss;

    if (pps >= 1e9) {
        oss << fixed << setprecision(1) << (pps / 1e9) << " Gpps";

    } else if (pps >= 1e6) {
        oss << fixed << setprecision(1) << (pps / 1e6) << " Mpps";

    } else if (pps >= 1e3) {
        oss << fixed << setprecision(1) << (pps / 1e3) << " Kpps";

    } else {
        oss << fixed << setprecision(1) << pps << " pps";

    }

    return oss.str();
}
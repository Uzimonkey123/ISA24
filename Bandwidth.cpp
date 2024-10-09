#include "Bandwidth.hpp"

double Bandwidth::calculateRate(uint64_t current, uint64_t previous, double elapsedTime) {
    return static_cast<double>(current - previous) / elapsedTime;
}

void Bandwidth::calculateBandwidth(uint64_t currentRxBytes, uint64_t currentTxBytes, uint64_t currentRxPackets, uint64_t currentTxPackets, 
                                    double& rx_bps, double& tx_bps, double& rx_pps, double& tx_pps) {
    auto now = chrono::steady_clock::now();
    double elapsedTime = chrono::duration<double>(now - updateTime).count();

    if (elapsedTime > 0) {
        rx_bps = calculateRate(currentRxBytes, rxBytes, elapsedTime);
        tx_bps = calculateRate(currentTxBytes, txBytes, elapsedTime);

        rx_pps = calculateRate(currentRxPackets, rxPackets, elapsedTime);
        tx_pps = calculateRate(currentTxPackets, txPackets, elapsedTime);
    }

    rxBytes = currentRxBytes;
    txBytes = currentTxBytes;
    rxPackets = currentRxPackets;
    txPackets = currentTxPackets;
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
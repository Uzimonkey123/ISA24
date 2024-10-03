#include "KeyGenerator.hpp"

string KeyGenerator::combineIpPort(const string& ip, int port) {
    return ip + ":" + to_string(port); // Combine IP and port
}

string KeyGenerator::generateKey(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol) {
    string combinedSrc = combineIpPort(sourceIp, sourcePort); // Combine source IP and port
    string combinedDst = combineIpPort(destIp, destPort); // Combine destination IP and port

    return combinedSrc < combinedDst ? combinedSrc + "-" + combinedDst + "-" + protocol 
                                     : combinedDst + "-" + combinedSrc + "-" + protocol;
}
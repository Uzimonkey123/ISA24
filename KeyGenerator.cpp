#include "KeyGenerator.hpp"

string KeyGenerator::combineIpPort(const string& ip, int port) {
    return ip + ":" + to_string(port);
}

string KeyGenerator::generateKey(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol) {
    string combinedSrc = combineIpPort(sourceIp, sourcePort);
    string combinedDst = combineIpPort(destIp, destPort);

    return combinedSrc < combinedDst ? combinedSrc + "-" + combinedDst + "-" + protocol 
                                     : combinedDst + "-" + combinedSrc + "-" + protocol;
}
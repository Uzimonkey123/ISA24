#include "KeyGenerator.hpp"

string KeyGenerator::combineIpPort(const string& ip, int port) {
    return ip + ":" + to_string(port);
}

string KeyGenerator::generateKey(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol) {
    string combinedSrc = combineIpPort(src_ip, src_port);
    string combinedDst = combineIpPort(dst_ip, dst_port);

    return combinedSrc < combinedDst ? combinedSrc + "-" + combinedDst + "-" + protocol 
                                     : combinedDst + "-" + combinedSrc + "-" + protocol;
}
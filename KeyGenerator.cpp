#include "KeyGenerator.hpp"

string KeyGenerator::generateKey(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol) {
    auto buildKey = [](const string& ip_a, int port_a, const string& ip_b, int port_b, const string& protocol) {
        return ip_a + ":" + to_string(port_a) + "-" + ip_b + ":" + to_string(port_b) + "-" + protocol;
    };

    if (src_ip < dst_ip || (src_ip == dst_ip && src_port < dst_port)) {
        return buildKey(src_ip, src_port, dst_ip, dst_port, protocol);

    } else {
        return buildKey(dst_ip, dst_port, src_ip, src_port, protocol);

    }
}
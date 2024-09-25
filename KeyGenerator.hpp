#ifndef KEYGENERATOR_HPP
#define KEYGENERATOR_HPP

#include <string>

using namespace std;

class KeyGenerator {
    public:
        string generateKey(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol);
};

#endif
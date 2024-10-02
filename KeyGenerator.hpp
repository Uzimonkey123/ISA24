#ifndef KEYGENERATOR_HPP
#define KEYGENERATOR_HPP

#include <string>

using namespace std;

class KeyGenerator {
    private:
        /**
         * @brief Combines IP and port into a single string
         * @param ip IP address
         * @param port Port number
         * @return Combined string of IP and port
         */
        string combineIpPort(const string& ip, int port);

    public:
        /**
         * @brief Generates a unique key for a connection
         * @param sourceIp Source IP address
         * @param sourcePort Source port number
         * @param destIp Destination IP address
         * @param destPort Destination port number
         * @param protocol Protocol used
         * @return Unique key for the connection
         */
        string generateKey(const string& sourceIp, int sourcePort, const string& destIp, int destPort, const string& protocol);
};

#endif
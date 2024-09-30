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
         * @param src_ip Source IP address
         * @param src_port Source port number
         * @param dst_ip Destination IP address
         * @param dst_port Destination port number
         * @param protocol Protocol used
         * @return Unique key for the connection
         */
        string generateKey(const string& src_ip, int src_port, const string& dst_ip, int dst_port, const string& protocol);
};

#endif
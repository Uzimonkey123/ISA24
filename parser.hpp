#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>

using namespace std;

class Parser {
    private:
        string interface;
        char orderBy;
        int interval;

    public:
        /**
         * @brief Construct a new Parser object
         */
        Parser();

        /**
         * @brief Parse the command line arguments
         * @param argc Number of arguments
         * @param argv Array of arguments
         */
        void parse(int argc, char* argv[]);

        /**
         * @brief Get the interface name
         * @return string Interface name
         */
        string getInterface();

        /**
         * @brief Get the order by parameter
         * @return char Order by parameter
         */
        char getOrderBy();

        /**
         * @brief Get the interval parameter
         * @return int Interval parameter
         */
        int getInterval();
        
};

#endif
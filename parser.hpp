#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>

class Parser {
    private:
        std::string interface;
        char orderBy;
        int interval;

    public:
        Parser();

        void parse(int argc, char* argv[]);

        std::string getInterface();
        char getOrderBy();
        int getInterval();
        
};

#endif
#include "parser.hpp"
#include "Exception.hpp"

#include <iostream>
#include <getopt.h>
#include <cstring>

Parser::Parser() {
    interface = "";
    orderBy = 'b'; // Default order by bytes
    interval = 1; // Default interval is 1 second
}

void Parser::parse(int argc, char* argv[]) {
    const struct option long_options[] = {
        {"i", required_argument, 0, 'i'},
        {"s", required_argument, 0, 's'},
        {"t", required_argument, 0, 't'},
        {nullptr, 0, nullptr, 0}
    };

    int option_index = 0;
    int option;

    while((option = getopt_long(argc, argv, "i:s:t:", long_options, &option_index)) != -1) {
        switch(option) {
            case 'i':
                if(optarg == nullptr || optarg[0] == '-') {
                    throw Exception(1, "Invalid argument for -i");
                }

                interface = optarg;
                break;

            case 's':
                if(strcmp(optarg, "b") == 0) {
                    orderBy = 'b';
                } else if(strcmp(optarg, "p") == 0) {
                    orderBy = 'p';
                } else {
                    throw Exception(1, "Invalid argument for -s");
                }
                break;

            case 't':
                interval = std::atoi(optarg);
                break;

            default:
                throw Exception(1, "Invalid argument");
        }
    }

    if(interface == "") {
        throw Exception(1, "Interface is required");
    }
}

std::string Parser::getInterface() {
    return interface;
}

char Parser::getOrderBy() {
    return orderBy;
}

int Parser::getInterval() {
    return interval;
}
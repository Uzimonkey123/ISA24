#include "parser.hpp"

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
        {"interface", required_argument, 0, 'i'},
        {"order-by", required_argument, 0, 's'},
        {"time", required_argument, 0, 't'},
        {nullptr, 0, nullptr, 0}
    };

    int option_index = 0;
    int option;

    while((option = getopt_long(argc, argv, "i:s:t:", long_options, &option_index)) != -1) {
        switch(option) {
            case 'i':
                if(optarg == nullptr || optarg[0] == '-') {
                    std::cerr << "Invalid argument for -i" << std::endl;
                    exit(1);
                }

                interface = optarg;
                break;

            case 's':
                if(strcmp(optarg, "b") == 0) {
                    orderBy = 'b';
                } else if(strcmp(optarg, "p") == 0) {
                    orderBy = 'p';
                } else {
                    std::cerr << "Invalid argument for -s" << std::endl;
                    exit(1);
                }
                break;

            case 't':
                interval = std::atoi(optarg);
                break;

            default:
                std::cerr << "Invalid argument" << std::endl;
                exit(1);
        }
    }

    if(interface == "") {
        std::cerr << "Interface is required" << std::endl;
        exit(1);
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
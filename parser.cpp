#include "parser.hpp"
#include "Exception.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

Parser::Parser() {
    interface = "";
    orderBy = 'b'; // Default order by bytes
    interval = 1; // Default interval is 1 second
}

void Parser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        // Ensure it's a valid option
        if (arg[0] == '-') {
            char option = arg[1]; // Get the option character after '-'

            switch (option) {
                case 'i':
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        interface = argv[++i];
                    } else {
                        throw Exception(1, "Invalid argument for -i");
                    }
                    break;

                case 's':
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        string order = argv[++i];

                        if (order == "b") {
                            orderBy = 'b';

                        } else if (order == "p") {
                            orderBy = 'p';

                        } else {
                            throw Exception(1, "Invalid argument for -s");

                        }
                    } else {
                        throw Exception(1, "Invalid argument for -s");

                    }
                    break;

                case 't':
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        try {
                            interval = stoi(argv[++i]);

                        } catch (const invalid_argument&) {
                            throw Exception(1, "Invalid argument for -t");

                        }
                    } else {
                        throw Exception(1, "Invalid argument for -t");
                        
                    }
                    break;

                default:
                    throw Exception(1, "Invalid argument");
            }
        } else {
            throw Exception(1, "Invalid argument format");
        }
    }

    if (interface.empty()) {
        throw Exception(1, "Interface is required");
    }
}

string Parser::getInterface() {
    return interface;
}

char Parser::getOrderBy() {
    return orderBy;
}

int Parser::getInterval() {
    return interval;
}

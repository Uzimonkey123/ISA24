#include "parser.hpp"

int main(int argc, char* argv[]) {
    Parser ArgParse = Parser();
    ArgParse.parse(argc, argv);

    std::cout << "Interface: " << ArgParse.getInterface() << std::endl;
    std::cout << "Order by: " << ArgParse.getOrderBy() << std::endl;
    std::cout << "Interval: " << ArgParse.getInterval() << std::endl;

    return 0;
}
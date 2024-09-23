#include "parser.hpp"
#include "Exception.hpp"

int main(int argc, char* argv[]) {
    Parser ArgParse = Parser();

    try {
        ArgParse.parse(argc, argv);
    } catch(Exception &e) {
        std::cerr << e.what() << std::endl;
        return e.getCode();
    }

    std::cout << "Interface: " << ArgParse.getInterface() << std::endl;
    std::cout << "Order by: " << ArgParse.getOrderBy() << std::endl;
    std::cout << "Interval: " << ArgParse.getInterval() << std::endl;

    return 0;
}
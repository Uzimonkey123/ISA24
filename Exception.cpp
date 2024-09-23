#include "Exception.hpp"
#include <sstream>

Exception::Exception(int code, const std::string& message) 
    : errorCode(code), errorMessage(message) {}

const char* Exception::what() const throw() {
    static std::string output;
    std::stringstream ss;
    ss << "Error code " << errorCode << ": " << errorMessage;
    output = ss.str();
    return output.c_str();
}

int Exception::getCode() const {
    return errorCode;
}
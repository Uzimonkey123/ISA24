/**
 * @file Exception.cpp
 * @brief Implementation of the Exception class
 * @author Norman Babiak
 * login: xbabia01
 */

#include "Exception.hpp"
#include <sstream>

Exception::Exception(int code, const string& message) 
    : errorCode(code), errorMessage(message) {}

const char* Exception::what() const throw() {
    static string output;
    stringstream ss;

    ss << "Error code " << errorCode << ": " << errorMessage;
    output = ss.str();
    
    return output.c_str();
}

int Exception::getCode() const {
    return errorCode;
}
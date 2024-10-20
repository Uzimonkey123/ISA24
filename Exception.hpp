#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

using namespace std;

class Exception : public exception {
    private:
        int errorCode;
        string errorMessage;

    public:
        /**
         * @brief Constructor
         * @param code Error code
         * @param message Error message
         */
        Exception(int code, const string& message);

        /**
         * @brief Get the error message
         * @return Error message
         */
        virtual const char* what() const throw();

        /**
         * @brief Get the error code
         * @return Error code
         */
        int getCode() const;
};

#endif
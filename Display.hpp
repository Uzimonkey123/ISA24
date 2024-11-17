/**
 * @file Display.hpp
 * @brief Declaration of the Display class
 * @author Norman Babiak
 * login: xbabia01
 */

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>
#include "ConnectionManager.hpp"

class Display{
    public:
        /**
         * @brief Construct a new Display object
         */
        Display();

        /**
         * @brief Display the connections
         * @param connections Vector of connections
         */
        void displayConnections(const std::vector<ConnectionManager::SavedConnection>& connections);
};

#endif
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>
#include "ConnectionManager.hpp"

class Display{
    public:
        Display();

        void displayConnections(const std::vector<ConnectionManager::SavedConnection>& connections);
};

#endif
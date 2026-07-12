#pragma once

#include "Server.hpp"

struct Message
{
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
};

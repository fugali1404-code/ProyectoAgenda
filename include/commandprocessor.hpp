#pragma once

#include <string>
#include <vector>

#include "sessionmaneger.hpp"

class CommandProcessor
{
public:

    static std::string procesar(
        const std::vector<std::string>& datos,
        SessionManager& session
    );
};
#pragma once

#include <string>
#include <vector>

class Protocol
{
public:

    static std::vector<std::string> dividir(
        const std::string& mensaje,
        char separador = '|'
    );

};
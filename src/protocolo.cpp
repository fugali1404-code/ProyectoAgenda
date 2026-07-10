#include "protocolo.hpp"

#include <sstream>

std::vector<std::string> Protocol::dividir(
    const std::string& mensaje,
    char separador
)
{
    std::vector<std::string> partes;

    std::stringstream flujo(mensaje);

    std::string palabra;

    while(std::getline(
        flujo,
        palabra,
        separador))
    {
        partes.push_back(palabra);
    }

    return partes;
}
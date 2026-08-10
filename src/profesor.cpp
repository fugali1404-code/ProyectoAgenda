#include "profesor.hpp"

Profesor::Profesor()
{
}

Profesor::Profesor(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador)
    : Usuario(
        id,
        nombre,
        correo,
        password,
        identificador)
{
}

std::string Profesor::getRol() const
{
    return "Profesor";
}
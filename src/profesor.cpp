#include "profesor.hpp"

Profesor::Profesor()
{
}

Profesor::Profesor(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password)
    : Usuario(
        id,
        nombre,
        correo,
        password)
{
}

std::string Profesor::getRol() const
{
    return "Profesor";
}
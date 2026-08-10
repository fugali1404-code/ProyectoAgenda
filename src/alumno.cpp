#include "alumno.hpp"

Alumno::Alumno()
{
}

Alumno::Alumno(
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

std::string Alumno::getRol() const
{
    return "Alumno";
}
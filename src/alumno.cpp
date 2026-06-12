#include "alumno.hpp"

Alumno::Alumno()
{
}

Alumno::Alumno(
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

std::string Alumno::getRol() const
{
    return "Alumno";
}
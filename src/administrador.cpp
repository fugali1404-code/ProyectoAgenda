#include "administrador.hpp"

Administrador::Administrador()
{
}

Administrador::Administrador(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador)
:
Usuario(
    id,
    nombre,
    correo,
    password,
    identificador
)

{
}

std::string Administrador::getRol() const
{
    return "Administrador";
}
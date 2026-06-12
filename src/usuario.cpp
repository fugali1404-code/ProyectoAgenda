#include "usuario.hpp"
#include <string>

Usuario::Usuario()
    : id(0)
{
}

Usuario::Usuario(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password)
    : id(id),
      nombre(nombre),
      correo(correo),
      password(password)
{
}

Usuario::~Usuario()
{
}

int Usuario::getId() const
{
    return id;
}

std::string Usuario::getNombre() const
{
    return nombre;
}

std::string Usuario::getCorreo() const
{
    return correo;
}

void Usuario::setNombre(
    const std::string& nombre)
{
    this->nombre = nombre;
}

void Usuario::setCorreo(
    const std::string& correo)
{
    this->correo = correo;
}

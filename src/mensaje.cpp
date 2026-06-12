#include "mensaje.hpp"

Mensaje::Mensaje()
    : tipo(TipoMensaje::ERROR)
{
}

Mensaje::Mensaje(
    TipoMensaje tipo,
    const std::string& contenido)
    : tipo(tipo),
      contenido(contenido)
{
}

TipoMensaje
Mensaje::getTipo() const
{
    return tipo;
}

std::string
Mensaje::getContenido() const
{
    return contenido;
}
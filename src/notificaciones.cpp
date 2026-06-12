#include "notificaciones.hpp"

Notificacion::Notificacion()
    : id(0),
      tipo(TipoNotificacion::SISTEMA),
      leida(false)
{
}

Notificacion::Notificacion(
    int id,
    TipoNotificacion tipo,
    const std::string& titulo,
    const std::string& mensaje,
    const std::string& fecha)
    : id(id),
      tipo(tipo),
      titulo(titulo),
      mensaje(mensaje),
      fecha(fecha),
      leida(false)
{
}

int Notificacion::getId() const
{
    return id;
}

TipoNotificacion
Notificacion::getTipo() const
{
    return tipo;
}

std::string
Notificacion::getTitulo() const
{
    return titulo;
}

std::string
Notificacion::getMensaje() const
{
    return mensaje;
}

std::string
Notificacion::getFecha() const
{
    return fecha;
}

bool
Notificacion::estaLeida() const
{
    return leida;
}

void
Notificacion::marcarComoLeida()
{
    leida = true;
}
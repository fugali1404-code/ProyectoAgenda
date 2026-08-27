#include "notificaciones.hpp"

// ============================================================
// CONSTRUCTOR POR DEFECTO
// ============================================================

Notificacion::Notificacion()

    : id(0),
      idUsuario(0),
      tipo(TipoNotificacion::SISTEMA),
      idReferencia(0),
      tipoReferencia(TipoReferenciaNotificacion::NINGUNA),
      titulo(""),
      mensaje(""),
      fecha(""),
      leida(false)

{
}

// ============================================================
// CONSTRUCTOR
// ============================================================

Notificacion::Notificacion(

    int id,
    int idUsuario,
    TipoNotificacion tipo,
    int idReferencia,
    TipoReferenciaNotificacion tipoReferencia,
    const std::string& titulo,
    const std::string& mensaje,
    const std::string& fecha

)

    : id(id),
      idUsuario(idUsuario),
      tipo(tipo),
      idReferencia(idReferencia),
      tipoReferencia(tipoReferencia),
      titulo(titulo),
      mensaje(mensaje),
      fecha(fecha),
      leida(false)

{
}

// ============================================================
// GET ID
// ============================================================

int Notificacion::getId() const
{
    return id;
}

// ============================================================
// GET ID USUARIO
// ============================================================

int Notificacion::getUsuarioId() const
{
    return idUsuario;
}

// ============================================================
// GET TIPO
// ============================================================

TipoNotificacion Notificacion::getTipo() const
{
    return tipo;
}

// ============================================================
// GET ID REFERENCIA
// ============================================================

int Notificacion::getIdReferencia() const
{
    return idReferencia;
}

// ============================================================
// GET TIPO REFERENCIA
// ============================================================

TipoReferenciaNotificacion
Notificacion::getTipoReferencia() const
{
    return tipoReferencia;
}

// ============================================================
// GET TITULO
// ============================================================

std::string Notificacion::getTitulo() const
{
    return titulo;
}

// ============================================================
// GET MENSAJE
// ============================================================

std::string Notificacion::getMensaje() const
{
    return mensaje;
}

// ============================================================
// GET FECHA
// ============================================================

std::string Notificacion::getFecha() const
{
    return fecha;
}

// ============================================================
// VERIFICAR SI ESTÁ LEÍDA
// ============================================================

bool Notificacion::estaLeida() const
{
    return leida;
}

// ============================================================
// MARCAR COMO LEÍDA
// ============================================================

void Notificacion::marcarComoLeida()
{
    leida = true;
}

// ============================================================
// MARCAR COMO NO LEÍDA
// ============================================================

void Notificacion::marcarComoNoLeida()
{
    leida = false;
}


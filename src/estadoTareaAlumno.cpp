#include "estadoTareaAlumno.hpp"

// ============================================================
// CONSTRUCTOR POR DEFECTO
// ============================================================

EstadoTareaAlumno::EstadoTareaAlumno()
    : idTarea(0),
      idAlumno(0),
      estado(EstadoTarea::NO_COMPLETADO)
{
}

// ============================================================
// CONSTRUCTOR
// ============================================================

EstadoTareaAlumno::EstadoTareaAlumno(
    int idTarea,
    int idAlumno,
    EstadoTarea estado
)
    : idTarea(idTarea),
      idAlumno(idAlumno),
      estado(estado)
{
}

// ============================================================
// GETTERS
// ============================================================

int EstadoTareaAlumno::getTareaId() const
{
    return idTarea;
}

// ============================================================

int EstadoTareaAlumno::getAlumnoId() const
{
    return idAlumno;
}

// ============================================================

EstadoTarea EstadoTareaAlumno::getEstado() const
{
    return estado;
}

// ============================================================
// SETTER
// ============================================================

void EstadoTareaAlumno::setEstado(
    EstadoTarea estado
)
{
    this->estado = estado;
}
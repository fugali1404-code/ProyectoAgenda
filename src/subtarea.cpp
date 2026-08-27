#include "subtarea.hpp"

////////////////////////////////////////////////////////////
// Constructor por defecto
////////////////////////////////////////////////////////////

Subtarea::Subtarea()
    : id(0),
      tareaId(0),
      alumnoId(0),
      descripcion(""),
      estado(EstadoSubtarea::PENDIENTE)
{
}

////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////

Subtarea::Subtarea(
    int id,
    int tareaId,
    int alumnoId,
    const std::string& descripcion
)
    : id(id),
      tareaId(tareaId),
      alumnoId(alumnoId),
      descripcion(descripcion),
      estado(EstadoSubtarea::PENDIENTE)
{
}


////////////////////////////////////////////////////////////
// GETTERS
////////////////////////////////////////////////////////////

int Subtarea::getId() const
{
    return id;
}

int Subtarea::getTareaId() const
{
    return tareaId;
}

int Subtarea::getAlumnoId() const
{
    return alumnoId;
}

std::string Subtarea::getDescripcion() const
{
    return descripcion;
}



EstadoSubtarea Subtarea::getEstado() const
{
    return estado;
}

////////////////////////////////////////////////////////////
// SETTERS
////////////////////////////////////////////////////////////

void Subtarea::setDescripcion(
    const std::string& descripcion
)
{
    this->descripcion = descripcion;
}

void Subtarea::setEstado(
    EstadoSubtarea estado
)
{
    this->estado = estado;
}
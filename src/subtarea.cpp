#include "subtarea.hpp"

Subtarea::Subtarea()
    : id(0),
      completada(false)
{
}

Subtarea::Subtarea(
    int id,
    const std::string& descripcion)
    : id(id),
      descripcion(descripcion),
      completada(false)
{
}

void Subtarea::completar()
{
    completada = true;
}

bool Subtarea::estaCompletada() const
{
    return completada;
}

std::string Subtarea::getDescripcion() const
{
    return descripcion;
}
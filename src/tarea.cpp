#include "tarea.hpp"

Tarea::Tarea()
    : id(0),
      completada(false),
      prioridad(Prioridad::MEDIA)
{
}

Tarea::Tarea(
    int id,
    const std::string& titulo,
    const std::string& descripcion,
    const std::string& fechaEntrega,
    Prioridad prioridad)
    : id(id),
      titulo(titulo),
      descripcion(descripcion),
      fechaEntrega(fechaEntrega),
      completada(false),
      prioridad(prioridad)
{
}

void Tarea::agregarSubtarea(
    const Subtarea& subtarea)
{
    subtareas.push_back(subtarea);
}

void Tarea::completar()
{
    completada = true;
}

bool Tarea::estaCompletada() const
{
    return completada;
}

int Tarea::getId() const
{
    return id;
}

std::string Tarea::getTitulo() const
{
    return titulo;
}

std::string Tarea::getFechaEntrega() const
{
    return fechaEntrega;
}

Prioridad Tarea::getPrioridad() const
{
    return prioridad;
}

std::vector<Subtarea>&
Tarea::getSubtareas()
{
    return subtareas;
}
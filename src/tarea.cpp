#include "tarea.hpp"

Tarea::Tarea()
{
    id = 0;
    materiaId = 0;
    completada = false;
    prioridad = Prioridad::MEDIA;
}

Tarea::Tarea(
    int id,
    int materiaId,
    const std::string& titulo,
    const std::string& descripcion,
    const std::string& fechaEntrega,
    Prioridad prioridad)
{
    this->id = id;
    this->materiaId = materiaId;
    this->titulo = titulo;
    this->descripcion = descripcion;
    this->fechaEntrega = fechaEntrega;
    this->prioridad = prioridad;
    this->completada = false;
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

//=====================
// GETTERS
//=====================

bool Tarea::estaCompletada() const
{
    return completada;
}

bool Tarea::getCompletada() const
{
    return completada;
}

int Tarea::getId() const
{
    return id;
}

int Tarea::getMateriaId() const
{
    return materiaId;
}

std::string Tarea::getTitulo() const
{
    return titulo;
}

std::string Tarea::getDescripcion() const
{
    return descripcion;
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

const std::vector<Subtarea>&
Tarea::getSubtareas() const
{
    return subtareas;
}

//=====================
// SETTERS
//=====================

void Tarea::setId(
    int id)
{
    this->id = id;
}

void Tarea::setMateriaId(
    int materiaId)
{
    this->materiaId = materiaId;
}

void Tarea::setTitulo(
    const std::string& titulo)
{
    this->titulo = titulo;
}

void Tarea::setDescripcion(
    const std::string& descripcion)
{
    this->descripcion = descripcion;
}

void Tarea::setFechaEntrega(
    const std::string& fechaEntrega)
{
    this->fechaEntrega = fechaEntrega;
}

void Tarea::setPrioridad(
    Prioridad prioridad)
{
    this->prioridad = prioridad;
}

void Tarea::setCompletada(
    bool completada)
{
    this->completada = completada;
}
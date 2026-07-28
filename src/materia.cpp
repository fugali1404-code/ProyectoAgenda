#include "materia.hpp"

Materia::Materia()
{
    id = 0;
    profesorId = 0;
}

Materia::Materia(
    int id,
    const std::string& nombre,
    int profesorId)
{
    this->id = id;
    this->nombre = nombre;
    this->profesorId = profesorId;
}

void Materia::agregarAlumno(
    int alumnoId)
{
    alumnos.push_back(alumnoId);
}

void Materia::agregarTarea(
    const Tarea& tarea)
{
    tareas.push_back(tarea);
}

//=====================
// GETTERS
//=====================

int Materia::getId() const
{
    return id;
}

std::string Materia::getNombre() const
{
    return nombre;
}

int Materia::getProfesorId() const
{
    return profesorId;
}

const std::vector<int>&
Materia::getAlumnos() const
{
    return alumnos;
}

std::vector<Tarea>&
Materia::getTareas()
{
    return tareas;
}

const std::vector<Tarea>&
Materia::getTareas() const
{
    return tareas;
}

//=====================
// SETTERS
//=====================

void Materia::setNombre(
    const std::string& nombre)
{
    this->nombre = nombre;
}

void Materia::setId(
    int id)
{
    this->id = id;
}

void Materia::setProfesorId(
    int profesorId)
{
    this->profesorId = profesorId;
}
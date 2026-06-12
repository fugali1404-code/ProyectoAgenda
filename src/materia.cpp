#include "materia.hpp"

Materia::Materia()
    : id(0),
      profesorId(0)
{
}

Materia::Materia(
    int id,
    const std::string& nombre,
    int profesorId)
    : id(id),
      nombre(nombre),
      profesorId(profesorId)
{
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

std::string Materia::getNombre() const
{
    return nombre;
}

std::vector<Tarea>&
Materia::getTareas()
{
    return tareas;
}
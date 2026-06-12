// main.cpp

#include <iostream>

#include "alumno.hpp"
#include "profesor.hpp"
#include "materia.hpp"
#include "tarea.hpp"
#include "subtarea.hpp"

int main()
{
    Profesor prof(
        1,
        "Juan Perez",
        "juan@escuela.com",
        "1234");

    Alumno alumno(
        1,
        "Ana Lopez",
        "ana@escuela.com",
        "1234");

    Materia programacion(
        1,
        "Programacion",
        prof.getId());

    Tarea tarea(
        1,
        "Proyecto Final",
        "Sistema Academico",
        "15/07/2026",
        Prioridad::ALTA);

    tarea.agregarSubtarea(
        Subtarea(
            1,
            "Disenar UML"));

    tarea.agregarSubtarea(
        Subtarea(
            2,
            "Implementar clases"));

    programacion.agregarAlumno(
        alumno.getId());

    programacion.agregarTarea(
        tarea);

    std::cout
        << "Materia: "
        << programacion.getNombre()
        << std::endl;

    std::cout
        << "Tarea: "
        << tarea.getTitulo()
        << std::endl;

    return 0;
}
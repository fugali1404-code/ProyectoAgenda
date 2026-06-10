#ifndef MATERIA_HPP
#define MATERIA_HPP

#include <string>
#include <vector>

#include "Tarea.hpp"

class Materia
{
private:

    int id;

    std::string nombre;

    int profesorId;

    std::vector<int> alumnos;

    std::vector<Tarea> tareas;

public:

    Materia();

    Materia(
        int id,
        const std::string& nombre,
        int profesorId
    );

    void agregarAlumno(
        int alumnoId
    );

    void agregarTarea(
        const Tarea& tarea
    );

    std::string getNombre() const;

    std::vector<Tarea>&
    getTareas();
};

#endif
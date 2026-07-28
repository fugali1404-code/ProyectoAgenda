#ifndef MATERIA_HPP
#define MATERIA_HPP

#include <string>
#include <vector>

#include "tarea.hpp"

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

    //=====================
    // GETTERS
    //=====================

    int getId() const;

    std::string getNombre() const;

    int getProfesorId() const;

    const std::vector<int>&
    getAlumnos() const;

    std::vector<Tarea>&
    getTareas();

    const std::vector<Tarea>&
    getTareas() const;

    //=====================
    // SETTERS
    //=====================

    void setNombre(
        const std::string& nombre
    );

    void setId(
        int id
    );

    void setProfesorId(
        int profesorId
    );
};

#endif
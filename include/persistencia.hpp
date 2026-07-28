#ifndef PERSISTENCIA_HPP
#define PERSISTENCIA_HPP

#include <string>
#include <vector>

#include "materia.hpp"
#include "tarea.hpp"
#include "usuario.hpp"

class Persistencia
{
public:

    //==============================
    // TAREAS
    //==============================

    static bool guardarTareas(
        const std::vector<Tarea>& tareas,
        const std::string& archivo
    );

    static bool cargarTareas(
        std::vector<Tarea>& tareas,
        const std::string& archivo
    );

    //==============================
    // MATERIAS
    //==============================

    static bool guardarMaterias(
        const std::vector<Materia>& materias,
        const std::string& archivo
    );

    static bool cargarMaterias(
        std::vector<Materia>& materias,
        const std::string& archivo
    );

    //==============================
    // USUARIOS
    //==============================

    static bool guardarUsuarios(
        const std::vector<Usuario*>& usuarios,
        const std::string& archivo
    );

    static bool cargarUsuarios(
        std::vector<Usuario*>& usuarios,
        const std::string& archivo
    );

private:

    //==============================
    // Conversión de prioridad
    //==============================

    static std::string prioridadAString(
        Prioridad prioridad
    );

    static Prioridad stringAPrioridad(
        const std::string& texto
    );

};

#endif
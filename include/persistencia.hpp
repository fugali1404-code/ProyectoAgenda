#ifndef PERSISTENCIA_HPP
#define PERSISTENCIA_HPP

#include <string>
#include <vector>

#include "Usuario.hpp"
#include "Materia.hpp"
#include "Tarea.hpp"

class Persistencia
{
public:

    Persistencia();

    // Usuarios

    static bool guardarUsuarios(
        const std::vector<Usuario*>& usuarios,
        const std::string& archivo
    );

    static bool cargarUsuarios(
        std::vector<Usuario*>& usuarios,
        const std::string& archivo
    );

    // Materias

    static bool guardarMaterias(
        const std::vector<Materia>& materias,
        const std::string& archivo
    );

    static bool cargarMaterias(
        std::vector<Materia>& materias,
        const std::string& archivo
    );

    // Tareas

    static bool guardarTareas(
        const std::vector<Tarea>& tareas,
        const std::string& archivo
    );

    static bool cargarTareas(
        std::vector<Tarea>& tareas,
        const std::string& archivo
    );
};

#endif
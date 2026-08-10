#ifndef PERSISTENCIA_HPP
#define PERSISTENCIA_HPP

#include <string>
#include <vector>

#include "materia.hpp"
#include "tarea.hpp"
#include "usuario.hpp"
#include "administrador.hpp"

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

    ///////////////////////////////////////////////////////////
    // MATERIAS
    ///////////////////////////////////////////////////////////

    static bool guardarMaterias(
        int profesorId,
        const std::vector<Materia>& materias,
        const std::string& archivo = "materias.txt"
    );

    static bool cargarMaterias(
        std::vector<Materia>& materias,
        const std::string& archivo = "materias.txt"
    );

    static int generarIdMateria(
        const std::string& archivo
    );

    ///////////////////////////////////////////////////////////
    //Usuarios
    ///////////////////////////////////////////////////////////

    static bool guardarUsuarios(
        const std::vector<Usuario*>& usuarios,
        const std::string& archivo = "usuarios.txt"
    );

    static bool cargarUsuarios(
        std::vector<Usuario*>& usuarios,
        const std::string& archivo = "usuarios.txt"
    );


    static Usuario* autenticarUsuario(
    const std::string& correo,
    const std::string& password,
    const std::string& archivo = "usuarios.txt"
    );


    static bool agregarUsuario(
        const Usuario& usuario,
        const std::string& archivo = "usuarios.txt"
    );

    static bool actualizarUsuario(
        const Usuario& usuario,
        const std::string& archivo = "usuarios.txt");

    static bool eliminarUsuario(
        int idUsuario,
        const std::string& archivo = "usuarios.txt");

    static int generarIdUsuario(
        const std::string& archivo = "usuarios.txt");


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
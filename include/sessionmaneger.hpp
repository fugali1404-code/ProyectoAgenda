#pragma once

#include <string>
#include <vector>

#include "materia.hpp"
#include "sessiondata.hpp"
#include "persistencia.hpp"



class SessionManager
{
private:

    SessionData datos;
    
    
public:

    SessionManager();

    bool login(
        const std::string& usuario,
        const std::string& password
    );

    void logout();

    bool estaAutenticado() const;

    std::string obtenerInformacionUsuario() const;

    std::string obtenerUsuario() const;

    std::string obtenerRol() const;

    std::string obtenerNombreCompleto() const;

    std::string obtenerIdentificador() const;

    int obtenerUsuarioId() const;

    const std::vector<Materia>& obtenerVectorMaterias() const;

    PlannerSemana& obtenerPlanner();



    ///////////////////////////////////////////////////////////
    // CRUD Usuarios
    ///////////////////////////////////////////////////////////

    bool agregarUsuario(
        const std::string& rol,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    std::string obtenerUsuarios() const;

    bool actualizarUsuario(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    bool eliminarUsuario(
        int id
    );
////////////////////////////////////////////////////////////
//CRUD Materias
////////////////////////////////////////////////////////////

    bool agregarMateria(
        const std::string& nombre
    );

    std::string obtenerMaterias() const;

    bool eliminarMateria(
        int id
    );

    bool editarMateria(
        int id,
        const std::string& nombre
    );

};
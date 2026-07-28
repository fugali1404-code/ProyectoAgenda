#pragma once

#include <string>
#include <vector>

#include "materia.hpp"
#include "sessiondata.hpp"


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

    std::string obtenerUsuario() const;

    bool agregarMateria(
        const std::string& nombre
    );

    std::string obtenerMaterias() const;

    PlannerSemana& obtenerPlanner();

    bool eliminarMateria(
    int id
    );

    bool editarMateria(
        int id,
        const std::string& nombre
    );
};
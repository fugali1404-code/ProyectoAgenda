#pragma once

#include <string>
#include <vector>

#include "materia.hpp"
#include "plannerSemana.hpp"

class SessionData
{
private:

    bool autenticado;
    std::string usuario;
    std::vector<Materia> materias;
    PlannerSemana planner;
    std::string nombreCompleto;
    std::string identificador;
    int usuarioId;
    std::string rol;

public:

    SessionData();

    bool estaAutenticado() const;

    void setAutenticado(
        bool estado
    );

    std::string obtenerUsuario() const;

    void setUsuario(
        const std::string& usuario
    );


    void limpiar();

    bool agregarMateria(
        const Materia& materia
    );

    void setNombreCompleto(
        const std::string& nombre
    );

    void setIdentificador(
        const std::string& identificador
    );

    void setUsuarioId(
        int id
    );

    void setRol(
        const std::string& rol
    );

    std::string obtenerRol() const;

    int obtenerUsuarioId() const;

    std::string obtenerMaterias() const;
    
    const std::vector<Materia>& obtenerVectorMaterias() const;

    std::string obtenerNombreCompleto() const;

    std::string obtenerIdentificador() const;

    PlannerSemana& obtenerPlanner();

    std::vector<Materia>& obtenerVectorMaterias();

    void guardarDatos();

    void setMaterias(
        const std::vector<Materia>& materias
    );
    


    
};
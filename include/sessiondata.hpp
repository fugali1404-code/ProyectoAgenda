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
        const std::string& nombre
    );

    std::string obtenerMaterias() const;

    PlannerSemana&
    obtenerPlanner();

    std::vector<Materia>&
    obtenerVectorMaterias();

    void cargarDatos();

    void guardarDatos();


    
};
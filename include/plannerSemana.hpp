#ifndef PLANNER_SEMANA_HPP
#define PLANNER_SEMANA_HPP

#include <array>
#include <string>

#include "PlannerDia.hpp"

class PlannerSemana
{
private:

    // Los siete días de la semana
    std::array<
        PlannerDia,
        7
    > dias;

    // Fecha del primer día de la semana.
    // Formato: YYYY-MM-DD
    std::string fechaInicio;

public:

    //========================
    // CONSTRUCTOR
    //========================

    PlannerSemana();

    //========================
    // FECHA DE INICIO
    //========================

    void setFechaInicio(
        const std::string& fecha
    );

    const std::string&
    getFechaInicio() const;

    //========================
    // DIAS
    //========================

    PlannerDia&
    getDia(
        int indice
    );

    const PlannerDia&
    getDia(
        int indice
    ) const;
};

#endif
#ifndef PLANNER_SEMANA_HPP
#define PLANNER_SEMANA_HPP

#include <array>

#include "PlannerDia.hpp"

class PlannerSemana
{
private:

    std::array<
        PlannerDia,
        7
    > dias;

public:

    PlannerDia&
    getDia(int indice);
};

#endif
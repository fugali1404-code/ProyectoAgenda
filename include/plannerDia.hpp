#ifndef PLANNER_DIA_HPP
#define PLANNER_DIA_HPP

#include <vector>

#include "Tarea.hpp"

class PlannerDia
{
private:

    std::vector<Tarea> tareas;

public:

    void agregarTarea(
        const Tarea& tarea
    );

    std::vector<Tarea>&
    getTareas();
};

#endif
#include "plannerDia.hpp"

void PlannerDia::agregarTarea(
    const Tarea& tarea)
{
    tareas.push_back(tarea);
}

std::vector<Tarea>&
PlannerDia::getTareas()
{
    return tareas;
}
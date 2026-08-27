#include "plannerDia.hpp"

#include <algorithm>

// ============================================================
// CONSTRUCTOR POR DEFECTO
// ============================================================

PlannerDia::PlannerDia()
    : fecha("")
{
}

// ============================================================
// CONSTRUCTOR CON FECHA
// ============================================================

PlannerDia::PlannerDia(
    const std::string& fecha
)
    : fecha(fecha)
{
}

// ============================================================
// FECHA
// ============================================================

void PlannerDia::setFecha(
    const std::string& fecha
)
{
    this->fecha = fecha;
}

const std::string&
PlannerDia::getFecha() const
{
    return fecha;
}

// ============================================================
// TAREAS
// ============================================================

void PlannerDia::agregarTarea(
    int idTarea,
    PrioridadPlanner prioridad
)
{
    // Evitar que una tarea aparezca dos veces
    // dentro del mismo día.

    for (const TareaPlanner& tarea : tareas)
    {
        if (tarea.idTarea == idTarea)
        {
            return;
        }
    }

    TareaPlanner nuevaTarea;

    nuevaTarea.idTarea = idTarea;
    nuevaTarea.prioridad = prioridad;

    tareas.push_back(nuevaTarea);
}

// ============================================================

void PlannerDia::cambiarPrioridadTarea(
    int idTarea,
    PrioridadPlanner prioridad
)
{
    for (TareaPlanner& tarea : tareas)
    {
        if (tarea.idTarea == idTarea)
        {
            tarea.prioridad = prioridad;
            return;
        }
    }
}

// ============================================================

void PlannerDia::eliminarTarea(
    int idTarea
)
{
    tareas.erase(
        std::remove_if(
            tareas.begin(),
            tareas.end(),
            [idTarea](const TareaPlanner& tarea)
            {
                return tarea.idTarea == idTarea;
            }
        ),
        tareas.end()
    );
}

// ============================================================

const std::vector<TareaPlanner>&
PlannerDia::getTareas() const
{
    return tareas;
}

// ============================================================
// SUBTAREAS
// ============================================================

void PlannerDia::agregarSubtarea(
    int idSubtarea
)
{
    // Evitar duplicar una subtarea
    // en el mismo día.

    for (int id : idsSubtareas)
    {
        if (id == idSubtarea)
        {
            return;
        }
    }

    idsSubtareas.push_back(idSubtarea);
}

// ============================================================

void PlannerDia::eliminarSubtarea(
    int idSubtarea
)
{
    idsSubtareas.erase(
        std::remove(
            idsSubtareas.begin(),
            idsSubtareas.end(),
            idSubtarea
        ),
        idsSubtareas.end()
    );
}

// ============================================================

const std::vector<int>&
PlannerDia::getSubtareas() const
{
    return idsSubtareas;
}
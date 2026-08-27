#ifndef PLANNER_DIA_HPP
#define PLANNER_DIA_HPP

#include <string>
#include <vector>

// ============================================================
// PRIORIDAD DE UNA TAREA DENTRO DEL PLANNER
// ============================================================

enum class PrioridadPlanner
{
    BAJA,
    MEDIA,
    ALTA
};

// ============================================================
// INFORMACION DE UNA TAREA EN EL PLANNER
// ============================================================

struct TareaPlanner
{
    int idTarea;
    PrioridadPlanner prioridad;
};

// ============================================================
// PLANNER DIA
// ============================================================

class PlannerDia
{
private:

    // Fecha del día: YYYY-MM-DD
    std::string fecha;

    // Tareas cuya fecha de entrega corresponde
    // a este día.
    std::vector<TareaPlanner> tareas;

    // Subtareas que el alumno decidió realizar
    // en este día.
    std::vector<int> idsSubtareas;

public:

    //========================
    // CONSTRUCTORES
    //========================

    PlannerDia();

    explicit PlannerDia(
        const std::string& fecha
    );

    //========================
    // FECHA
    //========================

    void setFecha(
        const std::string& fecha
    );

    const std::string&
    getFecha() const;

    //========================
    // TAREAS
    //========================

    void agregarTarea(
        int idTarea,
        PrioridadPlanner prioridad
    );

    void cambiarPrioridadTarea(
        int idTarea,
        PrioridadPlanner prioridad
    );

    void eliminarTarea(
        int idTarea
    );

    const std::vector<TareaPlanner>&
    getTareas() const;

    //========================
    // SUBTAREAS
    //========================

    void agregarSubtarea(
        int idSubtarea
    );

    void eliminarSubtarea(
        int idSubtarea
    );

    const std::vector<int>&
    getSubtareas() const;
};

#endif

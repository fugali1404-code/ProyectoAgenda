#ifndef PLANNER_CONTROLLER_HPP
#define PLANNER_CONTROLLER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

#include "plannerSemana.hpp"
#include "plannerDia.hpp"
#include "subtarea.hpp"
#include "Tarea.hpp"
#include "estadoTareaAlumno.hpp"

using namespace std;

class PlannerController
{

    struct EstadoTareaPlanner
    {
        int idTarea;
        EstadoTarea estado;
    };


private:

    //---------------------------------
    // Referencias
    //---------------------------------

    NetworkManager& network;

    SessionClient& session;

    //---------------------------------
    // Datos del Planner
    //---------------------------------

    PlannerSemana planner;

    //---------------------------------
    // Subtareas del alumno
    //---------------------------------

    vector<Subtarea> subtareas;

    vector<EstadoTareaPlanner> estadosTareas;

    //---------------------------------
    // Auxiliares para interpretar
    // respuestas del servidor
    //---------------------------------

    bool convertirPlanner(
        const string& respuesta,
        PlannerSemana& resultado
    ) const;

    bool convertirSubtareas(
        const string& respuesta,
        vector<Subtarea>& resultado
    ) const;

public:

    //---------------------------------
    // Constructor
    //---------------------------------

    PlannerController(
        NetworkManager& network,
        SessionClient& session
    );

    //---------------------------------
    // PLANNER
    //---------------------------------

    bool cargarPlanner();

    const PlannerSemana&
    obtenerPlanner() const;

    //--------------------------------
    //Estados tareas 
    //--------------------------------

    bool cargarEstadosTareas();

    const vector<EstadoTareaPlanner>& obtenerEstadosTareas() const;
    

    //---------------------------------
    // SUBTAREAS
    //---------------------------------

    bool cargarSubtareas();

    const vector<Subtarea>&
    obtenerSubtareas() const;

    Subtarea* obtenerSubtarea(
        int idSubtarea
    );

    const Subtarea* obtenerSubtarea(
        int idSubtarea
    ) const;

    //---------------------------------
    // CRUD SUBTAREAS
    //---------------------------------

    bool agregarSubtarea(
        int idTarea,
        const string& descripcion
    );

    bool editarSubtarea(
        int idSubtarea,
        const string& descripcion
    );

    bool eliminarSubtarea(
        int idSubtarea
    );

    //---------------------------------
    // ESTADO DE SUBTAREA
    //---------------------------------

    bool cambiarEstadoSubtarea(
        int idSubtarea,
        EstadoSubtarea estado
    );

    //---------------------------------
    // PRIORIDAD DE TAREA EN PLANNER
    //---------------------------------

    bool cambiarPrioridadTarea(
        int idTarea,
        PrioridadPlanner prioridad
    );

    //---------------------------------
    // AGREGAR SUBTAREA AL PLANNER
    //---------------------------------

    bool agregarSubtareaPlanner(
        int idSubtarea,
        const string& fecha
    );

    //---------------------------------
    // ELIMINAR SUBTAREA DEL PLANNER
    //---------------------------------

    bool eliminarSubtareaPlanner(
        int idSubtarea,
        const string& fecha
    );

    //---------------------------------
    // ACTUALIZAR DATOS
    //---------------------------------

    bool recargar();

    //---------------------------------
    // CONVERSIONES
    //---------------------------------

    static string prioridadPlannerAString(
        PrioridadPlanner prioridad
    );

    static PrioridadPlanner stringAPrioridadPlanner(
        const string& prioridad
    );

    static string estadoSubtareaAString(
        EstadoSubtarea estado
    );

    static EstadoSubtarea stringAEstadoSubtarea(
        const string& estado
    );

    static EstadoTarea stringAEstadoTarea(
        const string& estado
    );

};

#endif
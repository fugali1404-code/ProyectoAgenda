#ifndef PERSISTENCIA_HPP
#define PERSISTENCIA_HPP

#include <string>
#include <vector>
#include <iostream>

#include "materia.hpp"
#include "tarea.hpp"
#include "usuario.hpp"
#include "alumno.hpp"
#include "profesor.hpp"
#include "administrador.hpp"
#include "plannerSemana.hpp"
#include "plannerDia.hpp"
#include "estadoTareaAlumno.hpp"
#include "notificaciones.hpp"
#include "ponderacion.hpp"
#include "calificaciones.hpp"

using namespace std;

class Persistencia
{
public:

    ////////////////////////////////////////
    // TAREAS
    ///////////////////////////////////////

    static bool guardarTareas(
        const vector<Tarea>& tareas,
        const string& archivo = "tareas.txt"
    );

    static bool cargarTareas(
        vector<Tarea>& tareas,
        const string& archivo = "tareas.txt"
    );

    static int generarIdTarea(
        const string& archivo = "tareas.txt"
    );

    static string tipoTareaAString(
        TipoTarea tipo      
    );


    ///////////////////////////////////////////////////////////
    // PONDERACIONES
    ///////////////////////////////////////////////////////////

    static bool guardarPonderaciones(
        const vector<Ponderacion>& ponderaciones,
        const string& archivo
    );

    static bool cargarPonderaciones(
        vector<Ponderacion>& ponderaciones,
        const string& archivo
    );


    ///////////////////////////////////////////////////////////
    // MATERIAS
    ///////////////////////////////////////////////////////////

    static bool guardarMaterias(
        int profesorId,
        const vector<Materia>& materias,
        const string& archivo = "materias.txt"
    );

    static bool cargarMaterias(
        vector<Materia>& materias,
        const string& archivo = "materias.txt"
    );

    static int generarIdMateria(
        const string& archivo
    );

    ///////////////////////////////////////////////////////////
    // CALIFICACIONES
    ///////////////////////////////////////////////////////////

    static bool guardarCalificaciones(
        const vector<Calificacion>& calificaciones,
        const string& archivo = "calificaciones.txt"
    );

    static bool cargarCalificaciones(
        vector<Calificacion>& calificaciones,
        const string& archivo = "calificaciones.txt"
    );

    ///////////////////////////////////////////////////////////
    //Usuarios
    ///////////////////////////////////////////////////////////

    static bool guardarUsuarios(
        const vector<Usuario*>& usuarios,
        const string& archivo = "usuarios.txt"
    );

    static bool cargarUsuarios(
        vector<Usuario*>& usuarios,
        const string& archivo = "usuarios.txt"
    );


    static Usuario* autenticarUsuario(
        const string& correo,
        const string& password,
        const string& archivo = "usuarios.txt"
    );


    static bool agregarUsuario(
        const Usuario& usuario,
        const string& archivo = "usuarios.txt"
    );

    static bool actualizarUsuario(
        const Usuario& usuario,
        const string& archivo = "usuarios.txt");

    static bool eliminarUsuario(
        int idUsuario,
        const string& archivo = "usuarios.txt");

    static int generarIdUsuario(
        const string& archivo = "usuarios.txt");

    
    ///////////////////////////////////////////////////////////
    // SUBTAREAS
    ///////////////////////////////////////////////////////////

    static bool guardarSubtareas(
        const vector<Subtarea>& subtareas,
        const string& archivo = "subtareas.txt"
    );

    static bool cargarSubtareas(
        vector<Subtarea>& subtareas,
        const string& archivo = "subtareas.txt"
    );

    static int generarIdSubtarea(
        const string& archivo = "subtareas.txt"
    );
        
    ///////////////////////////////////////////////////////////
    // PLANNER
    ///////////////////////////////////////////////////////////

    static bool guardarPlanner(
        int alumnoId,
        const PlannerSemana& planner,
        const string& archivo = "planner.txt"
    );

    static bool cargarPlanner(
        int alumnoId,
        PlannerSemana& planner,
        const string& archivo = "planner.txt"
    );

    static string prioridadPlannerAString(
        PrioridadPlanner prioridad
    );

    ///////////////////////////////////////////////////////////
    // ESTADOS DE TAREAS POR ALUMNO
    ///////////////////////////////////////////////////////////

    static bool guardarEstadosTareas(
        const vector<EstadoTareaAlumno>& estados,
        const string& archivo = "estadosTareas.txt"
    );

    static bool cargarEstadosTareas(
        vector<EstadoTareaAlumno>& estados,
        const string& archivo = "estadosTareas.txt"
    );

    static string estadoTareaAString(
        EstadoTarea estado
    );

    
    ///////////////////////////////////////////////////////////
    // NOTIFICACIONES
    ///////////////////////////////////////////////////////////

    static bool guardarNotificaciones(
        const vector<Notificacion>& notificaciones,
        const string& archivo
    );

    static bool cargarNotificaciones(
        vector<Notificacion>& notificaciones,
        const string& archivo
    );

    static int generarIdNotificacion(
        const string& archivo
    );

    static string tipoNotificacionAString(
        TipoNotificacion tipo
    );

    static string tipoReferenciaNotificacionAString(
        TipoReferenciaNotificacion tipo
    );




    private:

    //==============================
    // Conversión de prioridad
    //==============================

    static TipoTarea stringATipoTarea(
        const string& texto
    );

    //=================================
    // Conversión de estado de subtarea
    //==================================

    static string estadoSubtareaAString(
        EstadoSubtarea estado
    );

    static EstadoSubtarea stringAEstadoSubtarea(
        const string& texto
    );

    //=================================
    // Conversión de prioridad Planner
    //=================================

    static PrioridadPlanner stringAPrioridadPlanner(
        const string& texto
    );

    //====================================
    //  Conversión de pioridad EstadoTarea
    //====================================

    static EstadoTarea stringAEstadoTarea(
        const string& estado
    );
    
    //==================================
    // Conversión notificaciones
    //==================================ç

    static TipoNotificacion stringATipoNotificacion(
        const string& texto
    );

    static TipoReferenciaNotificacion stringATipoReferenciaNotificacion(
        const string& texto
    );
    

};

#endif
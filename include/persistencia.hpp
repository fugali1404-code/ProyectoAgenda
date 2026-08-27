#ifndef PERSISTENCIA_HPP
#define PERSISTENCIA_HPP

#include <string>
#include <vector>

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

class Persistencia
{
public:

    ////////////////////////////////////////
    // TAREAS
    ///////////////////////////////////////

    static bool guardarTareas(
        const std::vector<Tarea>& tareas,
        const std::string& archivo = "tareas.txt"
    );

    static bool cargarTareas(
        std::vector<Tarea>& tareas,
        const std::string& archivo = "tareas.txt"
    );

    static int generarIdTarea(
        const std::string& archivo = "tareas.txt"
    );

    static std::string tipoTareaAString(
        TipoTarea tipo      
    );

    ///////////////////////////////////////////////////////////
    // MATERIAS
    ///////////////////////////////////////////////////////////

    static bool guardarMaterias(
        int profesorId,
        const std::vector<Materia>& materias,
        const std::string& archivo = "materias.txt"
    );

    static bool cargarMaterias(
        std::vector<Materia>& materias,
        const std::string& archivo = "materias.txt"
    );

    static int generarIdMateria(
        const std::string& archivo
    );

    ///////////////////////////////////////////////////////////
    //Usuarios
    ///////////////////////////////////////////////////////////

    static bool guardarUsuarios(
        const std::vector<Usuario*>& usuarios,
        const std::string& archivo = "usuarios.txt"
    );

    static bool cargarUsuarios(
        std::vector<Usuario*>& usuarios,
        const std::string& archivo = "usuarios.txt"
    );


    static Usuario* autenticarUsuario(
    const std::string& correo,
    const std::string& password,
    const std::string& archivo = "usuarios.txt"
    );


    static bool agregarUsuario(
        const Usuario& usuario,
        const std::string& archivo = "usuarios.txt"
    );

    static bool actualizarUsuario(
        const Usuario& usuario,
        const std::string& archivo = "usuarios.txt");

    static bool eliminarUsuario(
        int idUsuario,
        const std::string& archivo = "usuarios.txt");

    static int generarIdUsuario(
        const std::string& archivo = "usuarios.txt");

    
    ///////////////////////////////////////////////////////////
    // SUBTAREAS
    ///////////////////////////////////////////////////////////

    static bool guardarSubtareas(
        const std::vector<Subtarea>& subtareas,
        const std::string& archivo = "subtareas.txt"
    );

    static bool cargarSubtareas(
        std::vector<Subtarea>& subtareas,
        const std::string& archivo = "subtareas.txt"
    );

    static int generarIdSubtarea(
        const std::string& archivo = "subtareas.txt"
    );
        
    ///////////////////////////////////////////////////////////
    // PLANNER
    ///////////////////////////////////////////////////////////

    static bool guardarPlanner(
        int alumnoId,
        const PlannerSemana& planner,
        const std::string& archivo = "planner.txt"
    );

    static bool cargarPlanner(
        int alumnoId,
        PlannerSemana& planner,
        const std::string& archivo = "planner.txt"
    );

    static std::string prioridadPlannerAString(
        PrioridadPlanner prioridad
    );

    ///////////////////////////////////////////////////////////
    // ESTADOS DE TAREAS POR ALUMNO
    ///////////////////////////////////////////////////////////

    static bool guardarEstadosTareas(
        const std::vector<EstadoTareaAlumno>& estados,
        const std::string& archivo = "estadosTareas.txt"
    );

    static bool cargarEstadosTareas(
        std::vector<EstadoTareaAlumno>& estados,
        const std::string& archivo = "estadosTareas.txt"
    );

    static std::string estadoTareaAString(
        EstadoTarea estado
    );

    
    ///////////////////////////////////////////////////////////
    // NOTIFICACIONES
    ///////////////////////////////////////////////////////////

    static bool guardarNotificaciones(
        const std::vector<Notificacion>& notificaciones,
        const std::string& archivo
    );

    static bool cargarNotificaciones(
        std::vector<Notificacion>& notificaciones,
        const std::string& archivo
    );

    static int generarIdNotificacion(
        const std::string& archivo
    );

    static std::string tipoNotificacionAString(
        TipoNotificacion tipo
    );

    static std::string tipoReferenciaNotificacionAString(
        TipoReferenciaNotificacion tipo
    );




    private:

    //==============================
    // Conversión de prioridad
    //==============================

    static TipoTarea stringATipoTarea(
        const std::string& texto
    );

    //=================================
    // Conversión de estado de subtarea
    //==================================

    static std::string estadoSubtareaAString(
        EstadoSubtarea estado
    );

    static EstadoSubtarea stringAEstadoSubtarea(
        const std::string& texto
    );

    //=================================
    // Conversión de prioridad Planner
    //=================================

    static PrioridadPlanner stringAPrioridadPlanner(
        const std::string& texto
    );

    //====================================
    //  Conversión de pioridad EstadoTarea
    //====================================

    static EstadoTarea stringAEstadoTarea(
        const std::string& estado
    );
    
    //==================================
    // Conversión notificaciones
    //==================================ç

    static TipoNotificacion stringATipoNotificacion(
        const std::string& texto
    );

    static TipoReferenciaNotificacion stringATipoReferenciaNotificacion(
        const std::string& texto
    );
    

};

#endif
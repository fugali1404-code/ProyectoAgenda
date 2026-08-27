#pragma once

#include <string>
#include <vector>

#include "administrador.hpp"
#include "alumno.hpp"
#include "profesor.hpp"
#include "persistencia.hpp"
#include "plannerSemana.hpp"
#include "plannerDia.hpp"
#include "sessiondata.hpp"
#include "persistencia.hpp"
#include "tarea.hpp"
#include "materia.hpp"
#include "inscripciones.hpp"
#include "notificaciones.hpp"

class SessionManager
{
private:

    SessionData datos;
    
    //--------------------------------
    //conversion dias
    //--------------------------------
    std::string obtenerLunesSemana(
        const std::string& fecha
    ) const;

    std::string sumarDias(
        const std::string& fecha,
        int dias
    ) const;


public:

    SessionManager();

    bool login(
        const std::string& usuario,
        const std::string& password
    );

    void logout();

    bool estaAutenticado() const;

    std::string obtenerInformacionUsuario() const;

    std::string obtenerUsuario() const;

    std::string obtenerRol() const;

    std::string obtenerNombreCompleto() const;

    std::string obtenerIdentificador() const;

    int obtenerUsuarioId() const;

    const std::vector<Materia>& obtenerVectorMaterias() const;

    

    ///////////////////////////////////////////////////////////
    // CRUD Usuarios
    ///////////////////////////////////////////////////////////

    bool agregarUsuario(
        const std::string& rol,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    std::string obtenerUsuarios() const;

    bool actualizarUsuario(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    bool eliminarUsuario(
        int id
    );
    
    ////////////////////////////////////////////////////////////
    //CRUD Materias
    ////////////////////////////////////////////////////////////

    bool agregarMateria(
        const std::string& nombre
    );

    std::string obtenerMaterias() const;

    bool eliminarMateria(
        int id
    );

    bool editarMateria(
        int id,
        const std::string& nombre
    );


    ///////////////////////////////////////////////////////////
    // INSCRIPCIONES
    ///////////////////////////////////////////////////////////

    bool inscribirAlumno(
        int idMateria,
        int idAlumno
    );

    bool desinscribirAlumno(
        int idMateria,
        int idAlumno
    );

    std::string obtenerAlumnosMateria(
        int idMateria
    ) const;

    std::string obtenerMateriasAlumno() const;


    //////////////////////////////////////////////////////////
    // CRUD Tareas
    //////////////////////////////////////////////////////////
    
    bool agregarTarea(
        int idMateria,
        const std::string& titulo,
        const std::string& fechaEntrega,
        const std::string& descripcion,
        TipoTarea tipo
    );

    std::vector<Tarea> obtenerTareas() const;

    bool editarTarea(
        int idTarea,
        const std::string& titulo,
        const std::string& fechaEntrega,
        const std::string& descripcion,
        TipoTarea tipo
    );

    bool eliminarTarea( int idTarea);

    //////////////////////////////////////////////////////////
    // SUBTAREAS
    //////////////////////////////////////////////////////////

    bool agregarSubtarea(
        int idTarea,
        const std::string& descripcion
    );

    std::vector<Subtarea> obtenerSubtareas() const;

    bool editarSubtarea(
        int idSubtarea,
        const std::string& descripcion
    );

    bool eliminarSubtarea(
        int idSubtarea
    );

    bool cambiarEstadoSubtarea(
        int idSubtarea,
        EstadoSubtarea estado
    );

    ///////////////////////////////////////////////////////////
    // PLANNER
    ///////////////////////////////////////////////////////////

    PlannerSemana& obtenerPlanner();
    
    bool cambiarPrioridadPlanner(
        int idTarea,
        PrioridadPlanner prioridad
    );


    bool agregarSubtareaFecha(
        int idSubtarea,
        const std::string& fecha
    );

    bool eliminarSubtareaPlanner(
        int idSubtarea,
        const std::string& fecha
    );

    //////////////////////////////////////////////////////////
    // ESTADO DE TAREAS POR ALUMNO
    //////////////////////////////////////////////////////////

    bool cambiarEstadoTarea(
        int idTarea,
        EstadoTarea estado
    );

    EstadoTarea obtenerEstadoTarea(
        int idTarea
    ) const;

    std::vector<EstadoTareaAlumno> obtenerEstadosTarea(
        int idTarea
    ) const;


    std::vector<EstadoTareaAlumno>obtenerEstadosAlumno(
        EstadoTarea estado
    ) const;

    ////////////////////////////////////////////////////////////
    // NOTIFICACIONES
    ////////////////////////////////////////////////////////////

    bool agregarNotificacion(
        int idUsuario,
        TipoNotificacion tipo,
        int idReferencia,
        TipoReferenciaNotificacion tipoReferencia,
        const std::string& titulo,
        const std::string& mensaje,
        const std::string& fecha
    );

    std::vector<Notificacion> obtenerNotificaciones() const;

    bool marcarNotificacionLeida(
        int idNotificacion
    );

    bool eliminarNotificacion(
        int idNotificacion
    );

};

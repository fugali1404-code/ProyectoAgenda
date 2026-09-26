#ifndef TAREASCONTROLLER_HPP
#define TAREASCONTROLLER_HPP

#include <string>
#include <vector>

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

#include "Tarea.hpp"
#include "estadoTareaAlumno.hpp"
#include "calificaciones.hpp"

class TareasController
{
private:

    //---------------------------------
    // Referencias
    //---------------------------------

    NetworkManager& network;

    SessionClient& session;

    //---------------------------------
    // Datos
    //---------------------------------

    std::vector<Tarea> tareas;

    std::vector<EstadoTareaAlumno> estadosTarea;

    std::vector<Calificacion> calificaciones;

    //---------------------------------
    // Auxiliar para interpretar
    // respuestas de calificaciones
    //---------------------------------

    bool convertirCalificaciones(
        const std::string& respuesta,
        std::vector<Calificacion>& resultado
    ) const;

public:

    //---------------------------------
    // Constructor
    //---------------------------------

    TareasController(
        NetworkManager& network,
        SessionClient& session
    );

    //---------------------------------
    // TAREAS
    //---------------------------------

    bool cargarTareas();

    const std::vector<Tarea>&
    obtenerTareas() const;

    std::vector<Tarea>
    obtenerTareasMateria(
        int idMateria
    ) const;

    //---------------------------------
    // CRUD TAREAS
    //---------------------------------

    bool agregarTarea(
        int idMateria,
        const std::string& titulo,
        const std::string& fechaEntrega,
        const std::string& descripcion,
        TipoTarea tipo,
        int parcial
    );

    bool editarTarea(
        int idTarea,
        const std::string& titulo,
        const std::string& fechaEntrega,
        const std::string& descripcion,
        TipoTarea tipo,
        int parcial
    );

    bool eliminarTarea(
        int idTarea
    );

    //---------------------------------
    // ESTADO DE TAREA DEL ALUMNO
    //---------------------------------

    bool cambiarEstadoTarea(
        int idTarea,
        EstadoTarea estado
    );

    //---------------------------------
    // ESTADOS DE UNA TAREA
    // Profesor
    //---------------------------------

    bool cargarEstadosTarea(
        int idTarea
    );

    const std::vector<EstadoTareaAlumno>&
    obtenerEstadosTarea() const;

    //---------------------------------
    // ESTADOS DEL ALUMNO
    //---------------------------------

    bool cargarEstadosAlumno(
        EstadoTarea estado
    );

    //---------------------------------
    // CALIFICACIONES
    //---------------------------------

    /*
     * Carga las calificaciones de un alumno.
     *
     * El idAlumno se recibe como parámetro
     * porque SessionClient actualmente no
     * almacena el ID numérico.
     */
    bool cargarCalificacionesAlumno(
        int idAlumno
    );

    //---------------------------------
    // Obtener todas las calificaciones
    // actualmente cargadas
    //---------------------------------

    const std::vector<Calificacion>&
    obtenerCalificaciones() const;

    //---------------------------------
    // Obtener calificaciones de una tarea
    //---------------------------------

    std::vector<Calificacion>
    obtenerCalificacionesTarea(
        int idTarea
    ) const;

    //---------------------------------
    // Cargar calificaciones de varios
    // alumnos para una tarea
    //---------------------------------

    bool cargarCalificacionesTarea(
        int idTarea,
        const std::vector<int>& idsAlumnos
    );

    bool obtenerIdAlumnoActual(
        int& idAlumno
    );

    //---------------------------------
    // CRUD CALIFICACIONES
    //---------------------------------

    bool agregarCalificacion(
        int idAlumno,
        int idTarea,
        double calificacion
    );

    bool editarCalificacion(
        int idAlumno,
        int idTarea,
        double calificacion
    );

    bool eliminarCalificacion(
        int idAlumno,
        int idTarea
    );

    //---------------------------------
    // CONVERSIONES
    //---------------------------------

    static std::string tipoTareaAString(
        TipoTarea tipo
    );

    static TipoTarea stringATipoTarea(
        const std::string& tipo
    );

    static std::string estadoTareaAString(
        EstadoTarea estado
    );

    static EstadoTarea stringAEstadoTarea(
        const std::string& estado
    );
};

#endif
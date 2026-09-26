#include "controllers/tareasController.hpp"

#include "protocolo.hpp"

#include <algorithm>
#include <cstdlib>

//=================================================
// Constructor
//=================================================

TareasController::TareasController(
    NetworkManager& network,
    SessionClient& session
)
    : network(network),
      session(session)
{
}

//=================================================
// CARGAR TAREAS
//=================================================

bool TareasController::cargarTareas()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    std::string respuesta =
        network.enviarComando(
            "GET_TAREAS"
        );

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta
        );

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "SIN_TAREAS")
    {
        tareas.clear();

        return true;
    }

    if(datos[0] != "TAREAS")
    {
        return false;
    }

    tareas.clear();

    for(size_t i = 1; i < datos.size(); ++i)
    {
        std::vector<std::string> campos =
            Protocol::dividir(
                datos[i],
                ';'
            );

        if(campos.size() < 7)
        {
            continue;
        }

        try
        {
            int id =
                std::stoi(campos[0]);

            int materiaId =
                std::stoi(campos[1]);

            std::string titulo =
                campos[2];

            std::string fechaEntrega =
                campos[3];

            std::string descripcion =
                campos[4];

            int parcial =
                std::stoi(campos[5]);

            TipoTarea tipo =
                stringATipoTarea(
                    campos[6]
                );

            Tarea tarea(
                id,
                materiaId,
                titulo,
                descripcion,
                fechaEntrega,
                tipo,
                parcial
            );

            tareas.push_back(
                tarea
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return true;
}

//=================================================
// OBTENER TAREAS
//=================================================

const std::vector<Tarea>&
TareasController::obtenerTareas() const
{
    return tareas;
}

//=================================================
// OBTENER TAREAS DE UNA MATERIA
//=================================================

std::vector<Tarea>
TareasController::obtenerTareasMateria(
    int idMateria
) const
{
    std::vector<Tarea> resultado;

    for(const Tarea& tarea : tareas)
    {
        if(tarea.getMateriaId() == idMateria)
        {
            resultado.push_back(
                tarea
            );
        }
    }

    return resultado;
}

//=================================================
// AGREGAR TAREA
//=================================================

bool TareasController::agregarTarea(
    int idMateria,
    const std::string& titulo,
    const std::string& fechaEntrega,
    const std::string& descripcion,
    TipoTarea tipo,
    int parcial
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idMateria <= 0)
    {
        return false;
    }

    if(titulo.empty())
    {
        return false;
    }

    if(fechaEntrega.empty())
    {
        return false;
    }

    if(parcial < 1)
    {
        return false;
    }

    std::string comando =
        "ADD_TAREA|" +
        std::to_string(idMateria) +
        "|" +
        titulo +
        "|" +
        fechaEntrega +
        "|" +
        descripcion +
        "|" +
        tipoTareaAString(tipo) +
        "|" +
        std::to_string(parcial);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "TAREA_CREADA")
    {
        return false;
    }

    return cargarTareas();
}

//=================================================
// EDITAR TAREA
//=================================================

bool TareasController::editarTarea(
    int idTarea,
    const std::string& titulo,
    const std::string& fechaEntrega,
    const std::string& descripcion,
    TipoTarea tipo,
    int parcial
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idTarea <= 0)
    {
        return false;
    }

    if(titulo.empty())
    {
        return false;
    }

    if(fechaEntrega.empty())
    {
        return false;
    }

    if(parcial < 1)
    {
        return false;
    }

    std::string comando =
        "UPDATE_TAREA|" +
        std::to_string(idTarea) +
        "|" +
        titulo +
        "|" +
        fechaEntrega +
        "|" +
        descripcion +
        "|" +
        tipoTareaAString(tipo) +
        "|" +
        std::to_string(parcial);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "TAREA_ACTUALIZADA")
    {
        return false;
    }

    return cargarTareas();
}

//=================================================
// ELIMINAR TAREA
//=================================================

bool TareasController::eliminarTarea(
    int idTarea
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idTarea <= 0)
    {
        return false;
    }

    std::string comando =
        "DELETE_TAREA|" +
        std::to_string(idTarea);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "TAREA_ELIMINADA")
    {
        return false;
    }

    return cargarTareas();
}

//=================================================
// CAMBIAR ESTADO DE TAREA
//=================================================

bool TareasController::cambiarEstadoTarea(
    int idTarea,
    EstadoTarea estado
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    if(idTarea <= 0)
    {
        return false;
    }

    std::string comando =
        "UPDATE_ESTADO_TAREA|" +
        std::to_string(idTarea) +
        "|" +
        estadoTareaAString(estado);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "ESTADO_TAREA_ACTUALIZADO")
    {
        return false;
    }

    return true;
}

//=================================================
// CARGAR ESTADOS DE UNA TAREA
//=================================================

bool TareasController::cargarEstadosTarea(
    int idTarea
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(idTarea <= 0)
    {
        return false;
    }

    std::string comando =
        "GET_ESTADOS_TAREA|" +
        std::to_string(idTarea);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta
        );

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "SIN_ESTADOS_TAREA")
    {
        estadosTarea.clear();

        return true;
    }

    if(datos[0] != "ESTADOS_TAREA")
    {
        return false;
    }

    estadosTarea.clear();

    for(size_t i = 1; i < datos.size(); ++i)
    {
        std::vector<std::string> campos =
            Protocol::dividir(
                datos[i],
                ','
            );

        if(campos.size() < 3)
        {
            continue;
        }

        try
        {
            int idTareaRespuesta =
                std::stoi(campos[0]);

            int idAlumno =
                std::stoi(campos[1]);

            EstadoTarea estado =
                stringAEstadoTarea(
                    campos[2]
                );

            EstadoTareaAlumno estadoAlumno(
                idTareaRespuesta,
                idAlumno,
                estado
            );

            estadosTarea.push_back(
                estadoAlumno
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return true;
}

//=================================================
// OBTENER ESTADOS DE UNA TAREA
//=================================================

const std::vector<EstadoTareaAlumno>&
TareasController::obtenerEstadosTarea() const
{
    return estadosTarea;
}

//=================================================
// CARGAR ESTADOS DEL ALUMNO
//=================================================

bool TareasController::cargarEstadosAlumno(
    EstadoTarea estado
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    std::string comando =
        "GET_ESTADOS_ALUMNO|" +
        estadoTareaAString(estado);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta
        );

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "SIN_ESTADOS_ALUMNO")
    {
        estadosTarea.clear();

        return true;
    }

    if(datos[0] != "ESTADOS_ALUMNO")
    {
        return false;
    }

    estadosTarea.clear();

    for(size_t i = 1; i < datos.size(); ++i)
    {
        try
        {
            int idTarea =
                std::stoi(datos[i]);

            EstadoTareaAlumno estadoAlumno(
                idTarea,
                0,
                estado
            );

            estadosTarea.push_back(
                estadoAlumno
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return true;
}

//=================================================
// CONVERTIR RESPUESTA DE CALIFICACIONES
//=================================================

bool TareasController::convertirCalificaciones(
    const std::string& respuesta,
    std::vector<Calificacion>& resultado
) const
{
    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta
        );

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "SIN_CALIFICACIONES")
    {
        resultado.clear();

        return true;
    }

    if(datos[0] != "CALIFICACIONES")
    {
        return false;
    }

    resultado.clear();

    for(size_t i = 1; i < datos.size(); ++i)
    {
        std::vector<std::string> campos =
            Protocol::dividir(
                datos[i],
                ';'
            );

        if(campos.size() < 3)
        {
            continue;
        }

        try
        {
            int idAlumno =
                std::stoi(campos[0]);

            int idTarea =
                std::stoi(campos[1]);

            double calificacion =
                std::stod(campos[2]);

            if(idAlumno <= 0)
            {
                continue;
            }

            if(idTarea <= 0)
            {
                continue;
            }

            if(calificacion < 0.0 ||
               calificacion > 10.0)
            {
                continue;
            }

            resultado.emplace_back(
                idAlumno,
                idTarea,
                calificacion
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return true;
}

//=================================================
// CARGAR CALIFICACIONES DE UN ALUMNO
//=================================================

bool TareasController::cargarCalificacionesAlumno(
    int idAlumno
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(idAlumno <= 0)
    {
        return false;
    }

    std::string comando =
        "GET_CALIFICACIONES|" +
        std::to_string(idAlumno);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    std::vector<Calificacion> resultado;

    if(!convertirCalificaciones(
        respuesta,
        resultado
    ))
    {
        return false;
    }

    calificaciones =
        resultado;

    return true;
}



bool TareasController::obtenerIdAlumnoActual(
    int& idAlumno
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    std::string respuesta =
        network.enviarComando(
            "GET_MI_ID"
        );

    std::vector<std::string> datos =
        Protocol::dividir(respuesta);

    if(datos.size() < 2)
    {
        return false;
    }

    if(datos[0] != "MI_ID")
    {
        return false;
    }

    try
    {
        idAlumno =
            std::stoi(datos[1]);
    }
    catch(...)
    {
        return false;
    }

    if(idAlumno <= 0)
    {
        return false;
    }

    return true;
}


//=================================================
// OBTENER CALIFICACIONES
//=================================================

const std::vector<Calificacion>&
TareasController::obtenerCalificaciones() const
{
    return calificaciones;
}

//=================================================
// OBTENER CALIFICACIONES DE UNA TAREA
//=================================================

std::vector<Calificacion>
TareasController::obtenerCalificacionesTarea(
    int idTarea
) const
{
    std::vector<Calificacion> resultado;

    for(const Calificacion& calificacion :
        calificaciones)
    {
        if(calificacion.getIdTarea() == idTarea)
        {
            resultado.push_back(
                calificacion
            );
        }
    }

    return resultado;
}

//=================================================
// CARGAR CALIFICACIONES DE UNA TAREA
//=================================================

bool TareasController::cargarCalificacionesTarea(
    int idTarea,
    const std::vector<int>& idsAlumnos
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idTarea <= 0)
    {
        return false;
    }

    calificaciones.clear();

    //---------------------------------------------
    // Evitar consultar dos veces al mismo alumno
    //---------------------------------------------

    std::vector<int> alumnosConsultados;

    for(int idAlumno : idsAlumnos)
    {
        if(idAlumno <= 0)
        {
            continue;
        }

        if(std::find(
            alumnosConsultados.begin(),
            alumnosConsultados.end(),
            idAlumno
        ) != alumnosConsultados.end())
        {
            continue;
        }

        alumnosConsultados.push_back(
            idAlumno
        );

        //-----------------------------------------
        // Consultar calificaciones del alumno
        //-----------------------------------------

        std::string comando =
            "GET_CALIFICACIONES|" +
            std::to_string(idAlumno);

        std::string respuesta =
            network.enviarComando(
                comando
            );

        std::vector<Calificacion> resultado;

        if(!convertirCalificaciones(
            respuesta,
            resultado
        ))
        {
            return false;
        }

        //-----------------------------------------
        // Guardar solamente las de esta tarea
        //-----------------------------------------

        for(const Calificacion& calificacion :
            resultado)
        {
            if(calificacion.getIdTarea() == idTarea)
            {
                calificaciones.push_back(
                    calificacion
                );
            }
        }
    }

    return true;
}

//=================================================
// AGREGAR CALIFICACION
//=================================================

bool TareasController::agregarCalificacion(
    int idAlumno,
    int idTarea,
    double calificacion
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idAlumno <= 0 ||
       idTarea <= 0)
    {
        return false;
    }

    if(calificacion < 0.0 ||
       calificacion > 10.0)
    {
        return false;
    }

    std::string comando =
        "ADD_CALIFICACION|" +
        std::to_string(idAlumno) +
        "|" +
        std::to_string(idTarea) +
        "|" +
        std::to_string(calificacion);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "CALIFICACION_AGREGADA")
    {
        return false;
    }

    //---------------------------------------------
    // Actualizar cache local
    //---------------------------------------------

    calificaciones.emplace_back(
        idAlumno,
        idTarea,
        calificacion
    );

    return true;
}

//=================================================
// EDITAR CALIFICACION
//=================================================

bool TareasController::editarCalificacion(
    int idAlumno,
    int idTarea,
    double calificacion
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idAlumno <= 0 ||
       idTarea <= 0)
    {
        return false;
    }

    if(calificacion < 0.0 ||
       calificacion > 10.0)
    {
        return false;
    }

    std::string comando =
        "UPDATE_CALIFICACION|" +
        std::to_string(idAlumno) +
        "|" +
        std::to_string(idTarea) +
        "|" +
        std::to_string(calificacion);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "CALIFICACION_ACTUALIZADA")
    {
        return false;
    }

    //---------------------------------------------
    // Actualizar cache local
    //---------------------------------------------

    for(Calificacion& item :
        calificaciones)
    {
        if(item.getIdAlumno() == idAlumno &&
           item.getIdTarea() == idTarea)
        {
            item.setCalificacion(
                calificacion
            );

            break;
        }
    }

    return true;
}

//=================================================
// ELIMINAR CALIFICACION
//=================================================

bool TareasController::eliminarCalificacion(
    int idAlumno,
    int idTarea
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idAlumno <= 0 ||
       idTarea <= 0)
    {
        return false;
    }

    std::string comando =
        "DELETE_CALIFICACION|" +
        std::to_string(idAlumno) +
        "|" +
        std::to_string(idTarea);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "CALIFICACION_ELIMINADA")
    {
        return false;
    }

    //---------------------------------------------
    // Eliminar del cache local
    //---------------------------------------------

    calificaciones.erase(
        std::remove_if(
            calificaciones.begin(),
            calificaciones.end(),
            [idAlumno, idTarea](
                const Calificacion& item
            )
            {
                return
                    item.getIdAlumno() == idAlumno &&
                    item.getIdTarea() == idTarea;
            }
        ),
        calificaciones.end()
    );

    return true;
}

//=================================================
// TIPO TAREA -> STRING
//=================================================

std::string TareasController::tipoTareaAString(
    TipoTarea tipo
)
{
    switch(tipo)
    {
        case TipoTarea::TAREA:
            return "TAREA";

        case TipoTarea::EXAMEN:
            return "EXAMEN";

        case TipoTarea::PRACTICA:
            return "PRACTICA";

        case TipoTarea::PROYECTO:
            return "PROYECTO";

        case TipoTarea::TRABAJO:
            return "TRABAJO";

        case TipoTarea::OTRO:
            return "OTRO";
    }

    return "OTRO";
}

//=================================================
// STRING -> TIPO TAREA
//=================================================

TipoTarea TareasController::stringATipoTarea(
    const std::string& tipo
)
{
    if(tipo == "TAREA")
    {
        return TipoTarea::TAREA;
    }

    if(tipo == "EXAMEN")
    {
        return TipoTarea::EXAMEN;
    }

    if(tipo == "PRACTICA")
    {
        return TipoTarea::PRACTICA;
    }

    if(tipo == "PROYECTO")
    {
        return TipoTarea::PROYECTO;
    }

    if(tipo == "TRABAJO")
    {
        return TipoTarea::TRABAJO;
    }

    return TipoTarea::OTRO;
}

//=================================================
// ESTADO TAREA -> STRING
//=================================================

std::string TareasController::estadoTareaAString(
    EstadoTarea estado
)
{
    switch(estado)
    {
        case EstadoTarea::COMPLETADO:
            return "COMPLETADO";

        case EstadoTarea::NO_COMPLETADO:
            return "NO_COMPLETADO";
    }

    return "NO_COMPLETADO";
}

//=================================================
// STRING -> ESTADO TAREA
//=================================================

EstadoTarea TareasController::stringAEstadoTarea(
    const std::string& estado
)
{
    if(estado == "COMPLETADO")
    {
        return EstadoTarea::COMPLETADO;
    }

    return EstadoTarea::NO_COMPLETADO;
}
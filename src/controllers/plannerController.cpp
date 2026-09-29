#include "controllers/plannerController.hpp"

#include "protocolo.hpp"

#include <algorithm>

//=================================================
// Constructor
//=================================================

PlannerController::PlannerController(
    NetworkManager& network,
    SessionClient& session
)
    : network(network),
      session(session)
{
}

//=================================================
// CARGAR PLANNER
//=================================================

bool PlannerController::cargarPlanner()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    string respuesta = network.enviarComando("GET_PLANNER");
    PlannerSemana resultado;

    if(!convertirPlanner(respuesta,resultado))
    {
        return false;
    }

    planner = resultado;

    return true;
}

//=================================================
// OBTENER PLANNER
//=================================================

const PlannerSemana&
PlannerController::obtenerPlanner() const
{
    return planner;
}

//=================================================
// CONVERTIR RESPUESTA DEL PLANNER
//=================================================

bool PlannerController::convertirPlanner(
    const string& respuesta,
    PlannerSemana& resultado
) const
{
    vector<string> datos = Protocol::dividir(respuesta);

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] != "PLANNER")
    {
        return false;
    }

    PlannerSemana nuevoPlanner;

    //---------------------------------------------
    // Recorrer los 7 días
    //---------------------------------------------

    for(size_t i = 1; i < datos.size(); ++i)
    {
        vector<string> elementos = Protocol::dividir(datos[i],',');

        if(elementos.size() < 2)
        {
            continue;
        }

        if(elementos[0] != "DIA")
        {
            continue;
        }

        string fecha = elementos[1];

        if(fecha.empty())
        {
            continue;
        }

        //-----------------------------------------
        // Buscar el día correspondiente
        //-----------------------------------------

        int indiceDia = -1;

        for(int j = 0; j < 7; ++j)
        {
            if(nuevoPlanner.getDia(j).getFecha() == fecha)
            {
                indiceDia = j;
                break;
            }
        }

        
        // Si todavía no tiene fecha,
        // asignarla al siguiente día disponible
        if(indiceDia == -1)
        {
            for(int j = 0; j < 7; ++j)
            {
                if(nuevoPlanner.getDia(j).getFecha().empty())
                {
                    nuevoPlanner.getDia(j).setFecha(fecha);

                    indiceDia = j;

                    break;
                }
            }
        }

        if(indiceDia == -1)
        {
            continue;
        }

        PlannerDia& dia = nuevoPlanner.getDia(indiceDia);


        // Procesar elementos del día
        size_t posicion = 2;

        while(posicion < elementos.size())
        {
            
            // TAREA
            if(elementos[posicion] == "TAREA")
            {
                if(posicion + 2 >= elementos.size())
                {
                    break;
                }

                try
                {
                    int idTarea = stoi(elementos[posicion + 1]);

                    PrioridadPlanner prioridad = stringAPrioridadPlanner(
                            elementos[posicion + 2]);

                    if(idTarea > 0)
                    {
                        dia.agregarTarea(
                            idTarea,
                            prioridad
                        );
                    }
                }
                catch(...)
                {
                    // Ignorar elemento inválido
                }

                posicion += 3;
            }

            //-------------------------------------
            // SUBTAREA
            //-------------------------------------

            else if(elementos[posicion] == "SUBTAREA")
            {
                if(posicion + 1 >= elementos.size())
                {
                    break;
                }

                try
                {
                    int idSubtarea = stoi(elementos[posicion + 1]);

                    if(idSubtarea > 0)
                    {
                        dia.agregarSubtarea(
                            idSubtarea
                        );
                    }
                }
                catch(...)
                {
                    // Ignorar elemento inválido
                }

                posicion += 2;
            }

            // Elemento desconocido
            else
            {
                ++posicion;
            }
        }
    }

    // Guardar resultado
    resultado = nuevoPlanner;

    return true;
}


//=================================================
// CARGAR ESTADOS DE TAREAS
//=================================================

bool PlannerController::cargarEstadosTareas()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    estadosTareas.clear();

    //---------------------------------------------
    // COMPLETADAS
    //---------------------------------------------

    string respuestaCompletadas =
        network.enviarComando(
            "GET_ESTADOS_ALUMNO|COMPLETADO"
        );

    vector<string> datosCompletadas =
        Protocol::dividir(respuestaCompletadas);

    if(
        datosCompletadas.empty() ||
        datosCompletadas[0] != "ESTADOS_ALUMNO"
    )
    {
        return false;
    }

    for(size_t i = 1; i < datosCompletadas.size(); ++i)
    {
        try
        {
            int idTarea = stoi(datosCompletadas[i]);

            if(idTarea > 0)
            {
                estadosTareas.push_back(
                    {
                        idTarea,
                        EstadoTarea::COMPLETADO
                    }
                );
            }
        }
        catch(...)
        {
            // Ignorar ID inválido
        }
    }

    //---------------------------------------------
    // NO COMPLETADAS
    //---------------------------------------------

    string respuestaNoCompletadas =
        network.enviarComando(
            "GET_ESTADOS_ALUMNO|NO_COMPLETADO"
        );

    vector<string> datosNoCompletadas =
        Protocol::dividir(respuestaNoCompletadas);

    if(
        datosNoCompletadas.empty() ||
        datosNoCompletadas[0] != "ESTADOS_ALUMNO"
    )
    {
        return false;
    }

    for(size_t i = 1; i < datosNoCompletadas.size(); ++i)
    {
        try
        {
            int idTarea = stoi(datosNoCompletadas[i]);

            if(idTarea > 0)
            {
                estadosTareas.push_back(
                    {
                        idTarea,
                        EstadoTarea::NO_COMPLETADO
                    }
                );
            }
        }
        catch(...)
        {
            // Ignorar ID inválido
        }
    }

    return true;
}

//=================================================
// OBTENER ESTADOS DE TAREAS
//=================================================

const vector<PlannerController::EstadoTareaPlanner>&
PlannerController::obtenerEstadosTareas() const
{
    return estadosTareas;
}


//=================================================
// CARGAR SUBTAREAS
//=================================================

bool PlannerController::cargarSubtareas()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    string respuesta = network.enviarComando("GET_SUBTAREAS");

    vector<Subtarea> resultado;

    if(!convertirSubtareas(respuesta,resultado))
    {
        return false;
    }

    subtareas = resultado;

    return true;
}

//=================================================
// CONVERTIR SUBTAREAS
//=================================================

bool PlannerController::convertirSubtareas(
    const string& respuesta,
    vector<Subtarea>& resultado
) const
{
    vector<std::string> datos = Protocol::dividir(respuesta);

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "SIN_SUBTAREAS")
    {
        resultado.clear();

        return true;
    }

    if(datos[0] != "SUBTAREAS")
    {
        return false;
    }

    resultado.clear();

    //---------------------------------------------
    // Formato:
    //
    // SUBTAREAS|
    // idSubtarea|
    // idTarea|
    // descripcion|
    // estado|
    // idSubtarea|
    // idTarea|
    // descripcion|
    // estado
    //---------------------------------------------

    size_t posicion = 1;

    while(posicion + 3 < datos.size())
    {
        try
        {
            int idSubtarea = stoi(datos[posicion]);
            int idTarea = stoi(datos[posicion + 1]);
            string descripcion = datos[posicion + 2];
            EstadoSubtarea estado = stringAEstadoSubtarea(datos[posicion + 3]);

            if(idSubtarea <= 0 || idTarea <= 0)
            {
                posicion += 4;

                continue;
            }

            Subtarea subtarea(
                idSubtarea,
                idTarea,
                0,
                descripcion
            );

            subtarea.setEstado(
                estado
            );

            resultado.push_back(
                subtarea
            );
        }
        catch(...)
        {
            // Ignorar subtarea inválida
        }

        posicion += 4;
    }

    return true;
}

//=================================================
// OBTENER SUBTAREAS
//=================================================

const vector<Subtarea>&
PlannerController::obtenerSubtareas() const
{
    return subtareas;
}

//=================================================
// OBTENER SUBTAREA
//=================================================

Subtarea* PlannerController::obtenerSubtarea(
    int idSubtarea
)
{
    for(Subtarea& subtarea : subtareas)
    {
        if(subtarea.getId() == idSubtarea)
        {
            return &subtarea;
        }
    }

    return nullptr;
}

//=================================================
// OBTENER SUBTAREA CONST
//=================================================

const Subtarea* PlannerController::obtenerSubtarea(
    int idSubtarea
) const
{
    for(const Subtarea& subtarea : subtareas)
    {
        if(subtarea.getId() == idSubtarea)
        {
            return &subtarea;
        }
    }

    return nullptr;
}

//=================================================
// AGREGAR SUBTAREA
//=================================================

bool PlannerController::agregarSubtarea(
    int idTarea,
    const string& descripcion
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

    if(descripcion.empty())
    {
        return false;
    }

    string comando = "ADD_SUBTAREA|" + to_string(idTarea) + "|" + descripcion;
    string respuesta = network.enviarComando(comando);

    if(respuesta != "SUBTAREA_CREADA")
    {
        return false;
    }

    //---------------------------------------------
    // Actualizar subtareas
    //---------------------------------------------

    return cargarSubtareas();
}

//=================================================
// EDITAR SUBTAREA
//=================================================

bool PlannerController::editarSubtarea(
    int idSubtarea,
    const string& descripcion
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

    if(idSubtarea <= 0)
    {
        return false;
    }

    if(descripcion.empty())
    {
        return false;
    }

    string comando = "UPDATE_SUBTAREA|" + to_string(idSubtarea) + "|" + descripcion;
    string respuesta = network.enviarComando(comando);

    if(respuesta != "SUBTAREA_ACTUALIZADA")
    {
        return false;
    }

    return cargarSubtareas();
}

//=================================================
// ELIMINAR SUBTAREA
//=================================================

bool PlannerController::eliminarSubtarea(
    int idSubtarea
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

    if(idSubtarea <= 0)
    {
        return false;
    }

    string comando = "DELETE_SUBTAREA|" + to_string(idSubtarea);
    string respuesta = network.enviarComando(comando);

    if(respuesta != "SUBTAREA_ELIMINADA")
    {
        return false;
    }

    //---------------------------------------------
    // Actualizar cache de subtareas
    //---------------------------------------------

    return cargarSubtareas();
}

//=================================================
// CAMBIAR ESTADO DE SUBTAREA
//=================================================

bool PlannerController::cambiarEstadoSubtarea(
    int idSubtarea,
    EstadoSubtarea estado
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

    if(idSubtarea <= 0)
    {
        return false;
    }

    string comando = "UPDATE_ESTADO_SUBTAREA|" + to_string(idSubtarea) + "|" 
        + estadoSubtareaAString(estado);

    string respuesta = network.enviarComando(comando);

    if(respuesta != "ESTADO_SUBTAREA_ACTUALIZADO")
    {
        return false;
    }

    //---------------------------------------------
    // Actualizar cache local
    //---------------------------------------------

    Subtarea* subtarea = obtenerSubtarea(idSubtarea);

    if(subtarea != nullptr)
    {
        subtarea->setEstado(estado);
    }

    return true;
}

//=================================================
// CAMBIAR PRIORIDAD DE TAREA
//=================================================

bool PlannerController::cambiarPrioridadTarea(
    int idTarea,
    PrioridadPlanner prioridad
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

    string comando = "SET_PRIORIDAD_PLANNER|" + to_string(idTarea) + "|" +
        prioridadPlannerAString(prioridad);

    string respuesta = network.enviarComando(comando);

    if(respuesta != "PRIORIDAD_PLANNER_ACTUALIZADA")
    {
        return false;
    }


    // Actualizar cache del Planner
    for(int i = 0; i < 7; ++i)
    {
        planner.getDia(i).cambiarPrioridadTarea(idTarea,prioridad);
    }

    return true;
}

//=================================================
// AGREGAR SUBTAREA AL PLANNER
//=================================================

bool PlannerController::agregarSubtareaPlanner(
    int idSubtarea,
    const string& fecha
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

    if(idSubtarea <= 0)
    {
        return false;
    }

    if(fecha.empty())
    {
        return false;
    }

    string comando = "ADD_SUBTAREA_PLANNER|" + to_string(idSubtarea) + "|" + fecha;
    string respuesta = network.enviarComando(comando);

    if(respuesta != "SUBTAREA_AGREGADA_PLANNER")
    {
        return false;
    }

    // Actualizar Planner
    return cargarPlanner();
}

//=================================================
// ELIMINAR SUBTAREA DEL PLANNER
//=================================================

bool PlannerController::eliminarSubtareaPlanner(
    int idSubtarea,
    const string& fecha
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

    if(idSubtarea <= 0)
    {
        return false;
    }

    if(fecha.empty())
    {
        return false;
    }

    string comando = "DELETE_SUBTAREA_PLANNER|" + to_string(idSubtarea) + "|" + fecha;
    string respuesta = network.enviarComando(comando);

    if(respuesta != "SUBTAREA_ELIMINADA_PLANNER")
    {
        return false;
    }

    // Actualizar Planner
    return cargarPlanner();
}

//=================================================
// RECARGAR PLANNER COMPLETO
//=================================================

bool PlannerController::recargar()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Alumno")
    {
        return false;
    }

    if(!cargarSubtareas())
    {
        return false;
    }

    if(!cargarEstadosTareas())
    {
        return false;
    }

    if(!cargarPlanner())
    {
        return false;
    }

    return true;
}

//=================================================
// PRIORIDAD PLANNER -> STRING
//=================================================

string PlannerController::prioridadPlannerAString(PrioridadPlanner prioridad)
{
    switch(prioridad)
    {
        case PrioridadPlanner::BAJA:
            return "BAJA";

        case PrioridadPlanner::MEDIA:
            return "MEDIA";

        case PrioridadPlanner::ALTA:
            return "ALTA";
    }

    return "MEDIA";
}

//=================================================
// STRING -> PRIORIDAD PLANNER
//=================================================

PrioridadPlanner PlannerController::stringAPrioridadPlanner(
    const string& prioridad
)
{
    if(prioridad == "BAJA")
    {
        return PrioridadPlanner::BAJA;
    }

    if(prioridad == "ALTA")
    {
        return PrioridadPlanner::ALTA;
    }

    return PrioridadPlanner::MEDIA;
}

//=================================================
// ESTADO SUBTAREA -> STRING
//=================================================

string PlannerController::estadoSubtareaAString(
    EstadoSubtarea estado
)
{
    switch(estado)
    {
        case EstadoSubtarea::PENDIENTE:
            return "PENDIENTE";

        case EstadoSubtarea::EN_PROGRESO:
            return "EN_PROGRESO";

        case EstadoSubtarea::COMPLETADA:
            return "COMPLETADA";
    }

    return "PENDIENTE";
}

//=================================================
// STRING -> ESTADO SUBTAREA
//=================================================

EstadoSubtarea PlannerController::stringAEstadoSubtarea(
    const string& estado
)
{
    if(estado == "EN_PROGRESO")
    {
        return EstadoSubtarea::EN_PROGRESO;
    }

    if(estado == "COMPLETADA")
    {
        return EstadoSubtarea::COMPLETADA;
    }

    return EstadoSubtarea::PENDIENTE;
}
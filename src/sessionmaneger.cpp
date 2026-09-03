#include "sessionmaneger.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <map>



std::mutex SessionManager::mutexDatos;


// -----------------FUNCIONES AUXILIARES DEL PLANNER---------------------------
namespace
{
    ///////////////////////////////////////////////////////////
    // Actualizar la fecha de una tarea dentro de planner.txt
    ///////////////////////////////////////////////////////////

    bool actualizarTareaEnPlannerArchivo(
        int idTarea,
        const std::string& nuevaFecha,
        const std::string& archivo
    )
    {
        std::ifstream entrada(archivo);

        if(!entrada.is_open())
        {
            return true;
        }

        std::vector<std::string> registros;
        std::string linea;

        // Saltar encabezado
        std::getline(entrada, linea);

        bool modificada = false;
        bool tareaYaAgregada = false;

        while(std::getline(entrada, linea))
        {
            if(linea.empty())
            {
                continue;
            }

            std::stringstream ss(linea);

            std::string alumnoTexto;
            std::string fecha;
            std::string tipo;
            std::string idElementoTexto;
            std::string prioridad;

            std::getline(ss, alumnoTexto, '|');
            std::getline(ss, fecha, '|');
            std::getline(ss, tipo, '|');
            std::getline(ss, idElementoTexto, '|');
            std::getline(ss, prioridad, '|');

            // Solo nos interesan registros TAREA
            if(tipo == "TAREA")
            {
                int idElemento;

                try
                {
                    idElemento = std::stoi(idElementoTexto);
                }
                catch(...)
                {
                    registros.push_back(linea);
                    continue;
                }

                if(idElemento == idTarea)
                {
                    // Evitar duplicar la misma tarea
                    if(tareaYaAgregada)
                    {
                        modificada = true;
                        continue;
                    }

                    tareaYaAgregada = true;

                    std::string nuevoRegistro =
                        alumnoTexto + "|" +
                        nuevaFecha + "|" +
                        "TAREA|" +
                        idElementoTexto + "|" +
                        prioridad;

                    registros.push_back(nuevoRegistro);

                    modificada = true;
                    continue;
                }
            }

            // Conservar cualquier otro registro
            registros.push_back(linea);
        }

        entrada.close();

        if(!modificada)
        {
            return true;
        }

        std::ofstream salida(archivo);

        if(!salida.is_open())
        {
            return false;
        }

        salida
            << "idAlumno|"
            << "fecha|"
            << "tipo|"
            << "idElemento|"
            << "prioridad\n";

        for(const std::string& registro : registros)
        {
            salida << registro << "\n";
        }

        salida.close();

        return true;
    }


    ///////////////////////////////////////////////////////////
    // Eliminar registros de una tarea y sus subtareas
    // del planner.txt
    ///////////////////////////////////////////////////////////

    bool eliminarElementosTareaDelPlanner(
        int idTarea,
        const std::vector<int>& idsSubtareas,
        const std::string& archivo
    )
    {
        std::ifstream entrada(archivo);

        if(!entrada.is_open())
        {
            return true;
        }

        std::vector<std::string> registros;
        std::string linea;

        // Saltar encabezado
        std::getline(entrada, linea);

        while(std::getline(entrada, linea))
        {
            if(linea.empty())
            {
                continue;
            }

            std::stringstream ss(linea);

            std::string alumnoTexto;
            std::string fecha;
            std::string tipo;
            std::string idElementoTexto;
            std::string prioridad;

            std::getline(ss, alumnoTexto, '|');
            std::getline(ss, fecha, '|');
            std::getline(ss, tipo, '|');
            std::getline(ss, idElementoTexto, '|');
            std::getline(ss, prioridad, '|');

            bool eliminar = false;

            int idElemento = 0;

            try
            {
                idElemento = std::stoi(idElementoTexto);
            }
            catch(...)
            {
                registros.push_back(linea);
                continue;
            }

            // =========================================
            // Eliminar la tarea
            // =========================================

            if(tipo == "TAREA" && idElemento == idTarea)
            {
                eliminar = true;
            }

            // =========================================
            // Eliminar subtareas pertenecientes
            // =========================================

            if(tipo == "SUBTAREA")
            {
                for(int idSubtarea : idsSubtareas)
                {
                    if(idElemento == idSubtarea)
                    {
                        eliminar = true;
                        break;
                    }
                }
            }

            // =========================================
            // Conservar lo demás
            // =========================================

            if(!eliminar)
            {
                registros.push_back(linea);
            }
        }

        entrada.close();

        std::ofstream salida(archivo);

        if(!salida.is_open())
        {
            return false;
        }

        salida
            << "idAlumno|"
            << "fecha|"
            << "tipo|"
            << "idElemento|"
            << "prioridad\n";

        for(const std::string& registro : registros)
        {
            salida << registro << "\n";
        }

        salida.close();

        return true;
    }
}


namespace
{
    // Buscar el índice del día dentro de la semana
    int obtenerIndiceDia(
        const PlannerSemana& planner,
        const std::string& fecha
    )
    {
        for(int i = 0; i < 7; ++i)
        {
            if(planner.getDia(i).getFecha() == fecha)
            {
                return i;
            }
        }

        return -1;
    }
}
//--------------------------------------

std::string SessionManager::sumarDias(
    const std::string& fecha,
    int dias
) const
{
    if(fecha.size() != 10)
    {
        return "";
    }

    int anio;
    int mes;
    int dia;

    try
    {
        anio = std::stoi(fecha.substr(0, 4));
        mes  = std::stoi(fecha.substr(5, 2));
        dia  = std::stoi(fecha.substr(8, 2));
    }
    catch(...)
    {
        return "";
    }

    std::tm fechaTm = {};

    fechaTm.tm_year = anio - 1900;
    fechaTm.tm_mon  = mes - 1;
    fechaTm.tm_mday = dia;
    fechaTm.tm_hour = 12;

    std::time_t tiempo = std::mktime(&fechaTm);

    if(tiempo == -1)
    {
        return "";
    }

    tiempo += static_cast<std::time_t>(dias) * 24 * 60 * 60;

    std::tm* resultado = std::localtime(&tiempo);

    if(resultado == nullptr)
    {
        return "";
    }

    char buffer[11];

    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d",
        resultado
    );

    return std::string(buffer);
}



SessionManager::SessionManager()
{
    
}

bool SessionManager::login(
    const std::string& correo,
    const std::string& password
)
{
    Usuario* usuario = Persistencia::autenticarUsuario(correo,password,"usuarios.txt");

    if(usuario == nullptr)
    {
        datos.limpiar();
        return false;
    }

    datos.setUsuarioId(usuario->getId());
    datos.setUsuario(usuario->getCorreo());
    datos.setNombreCompleto(usuario->getNombre());
    datos.setRol(usuario->getRol());


    // Identificador
    datos.setIdentificador(usuario->getIdentificador());

    // Materias
    std::vector<Materia> todasLasMaterias;

    Persistencia::cargarMaterias(todasLasMaterias,"materias.txt");

    std::vector<Materia> materiasProfesor;

    for(const auto& materia : todasLasMaterias)
    {
        if(materia.getProfesorId() == usuario->getId())
        {
            materiasProfesor.push_back(materia);
        }
    
    }

    datos.setMaterias(materiasProfesor);

    // Sesión autenticada
    datos.setAutenticado(true);

    delete usuario;

    return true;
}

//-------------------------------------------------------------

void SessionManager::logout()
{
    datos.limpiar();
}

bool SessionManager::estaAutenticado() const
{
    return datos.estaAutenticado();
}

std::string SessionManager::obtenerUsuario() const
{
    return datos.obtenerUsuario();
}

std::string SessionManager::obtenerNombreCompleto() const
{
    return datos.obtenerNombreCompleto();
}

std::string SessionManager::obtenerIdentificador() const
{
    return datos.obtenerIdentificador();
}

//-------------------------------Fechas--------------------------------------------

///////////////////////////////////////////////////////////
// Obtener fecha actual
///////////////////////////////////////////////////////////

static std::string obtenerFechaActual()
{
    std::time_t tiempo = std::time(nullptr);
    std::tm* fecha = std::localtime(&tiempo);
    std::ostringstream salida;

    salida
        << std::setfill('0')
        << std::setw(4)
        << fecha->tm_year + 1900
        << "-"
        << std::setw(2)
        << fecha->tm_mon + 1
        << "-"
        << std::setw(2)
        << fecha->tm_mday;

    return salida.str();
}

////////////////////////////////////////////////////////////
// Obtener lunes de la semana
////////////////////////////////////////////////////////////

std::string SessionManager::obtenerLunesSemana(
    const std::string& fecha
) const
{
    if(fecha.empty())
    {
        return "";
    }

    std::tm tmFecha = {};
    
    int anio;
    int mes;
    int dia;

    // Esperamos formato YYYY-MM-DD
    if(
        std::sscanf(
            fecha.c_str(),
            "%d-%d-%d",
            &anio,
            &mes,
            &dia
        ) != 3
    )
    {
        return "";
    }

    tmFecha.tm_year = anio - 1900;
    tmFecha.tm_mon  = mes - 1;
    tmFecha.tm_mday = dia;
    tmFecha.tm_hour = 12;

    // Convertir a fecha válida
    if(std::mktime(&tmFecha) == -1)
    {
        return "";
    }

    // tm_wday:
    // 0 = domingo
    // 1 = lunes
    // 2 = martes
    // ...
    // 6 = sábado

    int diaSemana = tmFecha.tm_wday;

    int diasDesdeLunes;

    if(diaSemana == 0)
    {
        // Domingo
        diasDesdeLunes = 6;
    }
    else
    {
        diasDesdeLunes = diaSemana - 1;
    }

    tmFecha.tm_mday -= diasDesdeLunes;

    if(std::mktime(&tmFecha) == -1)
    {
        return "";
    }

    char buffer[11];

    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d",
        &tmFecha
    );

    return std::string(buffer);
}

///////////////////////////////////////////////////////////
// Convertir YYYY-MM-DD a tm
///////////////////////////////////////////////////////////

static bool convertirFecha(
    const std::string& texto,
    std::tm& fecha
)
{
    if(texto.size() != 10)
    {
        return false;
    }

    if(
        texto[4] != '-' ||
        texto[7] != '-'
    )
    {
        return false;
    }

    try
    {
        int anio = std::stoi(texto.substr(0, 4));
        int mes =std::stoi(texto.substr(5, 2));
        int dia = std::stoi(texto.substr(8, 2));

        fecha = {};

        fecha.tm_year = anio - 1900;
        fecha.tm_mon = mes - 1;
        fecha.tm_mday = dia;

        fecha.tm_hour = 12;

        return true;
    }
    catch(...)
    {
        return false;
    }
}


///////////////////////////////////////////////////////////
// Obtener el lunes de la semana
///////////////////////////////////////////////////////////

static std::string obtenerLunesSemana(
    const std::string& fecha
)
{
    int anio;
    int mes;
    int dia;

    char separador1;
    char separador2;

    std::stringstream ss(fecha);

    ss >> anio
       >> separador1
       >> mes
       >> separador2
       >> dia;

    if(
        ss.fail() ||
        separador1 != '-' ||
        separador2 != '-'
    )
    {
        return "";
    }

    std::tm fechaTm = {};

    fechaTm.tm_year = anio - 1900;
    fechaTm.tm_mon = mes - 1;
    fechaTm.tm_mday = dia;

    // Convertir la fecha a tiempo
    std::mktime(&fechaTm);

    // tm_wday:
    // 0 = domingo
    // 1 = lunes
    // 2 = martes
    // ...
    // 6 = sábado

    int diaSemana = fechaTm.tm_wday;

    int diasDesdeLunes;

    if(diaSemana == 0)
    {
        // Domingo
        diasDesdeLunes = 6;
    }
    else
    {
        diasDesdeLunes = diaSemana - 1;
    }

    fechaTm.tm_mday -= diasDesdeLunes;

    // Normalizar nuevamente la fecha
    std::mktime(&fechaTm);

    std::ostringstream resultado;

    resultado
        << std::setfill('0')
        << std::setw(4)
        << (fechaTm.tm_year + 1900)
        << "-"
        << std::setw(2)
        << (fechaTm.tm_mon + 1)
        << "-"
        << std::setw(2)
        << fechaTm.tm_mday;

    return resultado.str();
}


////////////////////////////////////////////////////////////////////
//Agregar Materia
////////////////////////////////////////////////////////////////////

bool SessionManager::agregarMateria(
    const std::string& nombre
)
{
    if(!datos.estaAutenticado())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    Materia nueva;

    nueva.setId(Persistencia::generarIdMateria("materias.txt"));
    nueva.setNombre(nombre);

    // El profesor autenticado es el propietario de la materia.
    nueva.setProfesorId(datos.obtenerUsuarioId());

    if(!datos.agregarMateria(nueva))
    {
        return false;
    }

    return Persistencia::guardarMaterias(datos.obtenerUsuarioId(),
        datos.obtenerVectorMaterias(),"materias.txt");
}

//////////////////////////////////////////////////////////////
// Obtener Materias
//////////////////////////////////////////////////////////////

std::string SessionManager::obtenerMaterias() const
{
    //==================================================
    // AUTENTICACIÓN
    //==================================================

    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    //==================================================
    // BLOQUEAR ACCESO
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // CARGAR MATERIAS ACTUALES
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return "MATERIAS|";
    }

    //==================================================
    // CONSTRUIR RESPUESTA
    //==================================================

    std::string lista;

    for(const auto& materia : materias)
    {
        lista += std::to_string(
            materia.getId()
        );

        lista += "|";

        lista += materia.getNombre();

        lista += "|";

        lista += std::to_string(
            materia.getProfesorId()
        );

        lista += ";";
    }

    return "MATERIAS|" + lista;
}

/////////////////////////////////////////////////////////////
// Eliminar Materia
/////////////////////////////////////////////////////////////

bool SessionManager::eliminarMateria(
    int id
)
{

    // AUTENTICACIÓN
    if(!datos.estaAutenticado())
    {
        return false;
    }

  
    // SOLO PROFESORES
     if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    
    // VALIDAR ID
    if(id <= 0)
    {
        return false;
    }

   
    // BLOQUEAR DATOS COMPARTIDOS
    std::lock_guard<std::mutex> lock(mutexDatos);

   
    // CARGAR MATERIAS
    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias( materias,"materias.txt"))
    {
        return false;
    }


    // BUSCAR MATERIA
    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == id)
        {
            materiaEncontrada = true;

            // Verificar propietario
            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            break;
        }
    }

    if(!materiaEncontrada)
    {
        return false;
    }

    //==================================================
    // OBTENER ALUMNOS INSCRITOS
    //==================================================

    std::vector<int> alumnos = Inscripciones::
    obtenerAlumnosMateria( id,"inscripciones.txt");

        
    // CARGAR TAREAS
    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas,"tareas.txt"))
    {
        return false;
    }


    // OBTENER IDS DE LAS TAREAS
    std::vector<int> idsTareas;

    for(const auto& tarea : tareas)
    {
        if(tarea.getMateriaId() == id)
        {
            idsTareas.push_back(tarea.getId());
        }
    }


    // ELIMINAR CALIFICACIONES DE LAS TAREAS DE LA MATERIA
    std::vector<Calificacion> calificaciones;

    if(Persistencia::cargarCalificaciones(calificaciones,"calificaciones.txt"))
    {
        calificaciones.erase(
            std::remove_if(
                calificaciones.begin(),
                calificaciones.end(),
                [&idsTareas](const Calificacion& calificacion)
                {
                    return std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        calificacion.getIdTarea()
                    ) != idsTareas.end();
                }
            ),
            calificaciones.end()
        );

        if(!Persistencia::guardarCalificaciones(calificaciones,"calificaciones.txt"))
        {
            return false;
        }
    }

    // ELIMINAR PONDERACIONES DE LA MATERIA
    std::vector<Ponderacion> ponderaciones;

    if(Persistencia::cargarPonderaciones(ponderaciones,"ponderaciones.txt"))
    {
        ponderaciones.erase(
            std::remove_if(
                ponderaciones.begin(),
                ponderaciones.end(),
                [id](const Ponderacion& ponderacion)
                {
                    return ponderacion.getIdMateria() == id;
                }
            ),
            ponderaciones.end()
        );

        if(!Persistencia::guardarPonderaciones(ponderaciones,"ponderaciones.txt"))
        {
            return false;
        }
    }

    //==================================================
    // CARGAR SUBTAREAS
    //==================================================

    std::vector<Subtarea> subtareas;

    bool haySubtareas = Persistencia::cargarSubtareas(subtareas, "subtareas.txt");

    //==================================================
    // OBTENER IDS DE SUBTAREAS
    //==================================================

    std::vector<int> idsSubtareas;

    if(haySubtareas)
    {
        for(const auto& subtarea : subtareas)
        {
            if(std::find(idsTareas.begin(),idsTareas.end(),subtarea.getTareaId()) 
                != idsTareas.end())
            {
                idsSubtareas.push_back(subtarea.getId());
            }
        }
    }


    // ELIMINAR TAREAS
    tareas.erase(
        std::remove_if(
            tareas.begin(),
            tareas.end(),
            [id](const Tarea& tarea)
            {
                return tarea.getMateriaId() == id;
            }
        ),
        tareas.end()
    );

    if(!Persistencia::guardarTareas(tareas,"tareas.txt"))
    {
        return false;
    }

    // ELIMINAR ESTADOS DE LAS TAREAS
    std::vector<EstadoTareaAlumno> estados;

    if(Persistencia::cargarEstadosTareas(estados,"estadosTareas.txt"))
    {
        estados.erase(
            std::remove_if(
                estados.begin(),
                estados.end(),
                [&idsTareas](const EstadoTareaAlumno& estado)
                {
                    return std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        estado.getTareaId()
                    ) != idsTareas.end();
                }
            ),
            estados.end()
        );

        if(!Persistencia::guardarEstadosTareas(estados,"estadosTareas.txt"))
        {
            return false;
        }
    }


    // ELIMINAR SUBTAREAS
    if(haySubtareas)
    {
        subtareas.erase(
            std::remove_if(
                subtareas.begin(),
                subtareas.end(),
                [&idsTareas](const Subtarea& subtarea)
                {
                    return std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        subtarea.getTareaId()
                    ) != idsTareas.end();
                }
            ),
            subtareas.end()
        );

        if(!Persistencia::guardarSubtareas(subtareas,"subtareas.txt"))
        {
            return false;
        }
    }

    // ELIMINAR TAREAS Y SUBTAREAS DEL PLANNER
    for(int idAlumno : alumnos)
    {
        PlannerSemana planner;

        if(!Persistencia::cargarPlanner(idAlumno, planner,"planner.txt"))
        {
            continue;
        }

        
        // ELIMINAR TAREAS
        for(int idTarea : idsTareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarTarea(idTarea);
            }
        }

        // ELIMINAR SUBTAREAS
        for(int idSubtarea : idsSubtareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarSubtarea(idSubtarea);
            }
        }

        // GUARDAR PLANNER
        if(!Persistencia::guardarPlanner(idAlumno,planner,"planner.txt"))
        {
            return false;
        }
    }


    // ELIMINAR NOTIFICACIONES DE LAS TAREAS
    std::vector<Notificacion> notificaciones;

    if(Persistencia::cargarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        notificaciones.erase(
            std::remove_if(
                notificaciones.begin(),
                notificaciones.end(),
                [&idsTareas](
                    const Notificacion& notificacion
                )
                {
                    return
                        notificacion.getTipoReferencia() ==
                            TipoReferenciaNotificacion::TAREA
                        &&
                        std::find(
                            idsTareas.begin(),
                            idsTareas.end(),
                            notificacion.getIdReferencia()
                        ) != idsTareas.end();
                }
            ),
            notificaciones.end()
        );

        if(!Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt"))
        {
            return false;
        }
    }


    // ELIMINAR INSCRIPCIONES
    for(int idAlumno : alumnos)
    {
        if(!Inscripciones::desinscribirAlumno(
            id,idAlumno,"inscripciones.txt"))
        {
            return false;
        }
    }

    
    // ELIMINAR MATERIA
    materias.erase(
        std::remove_if(
            materias.begin(),
            materias.end(),
            [id](
                const Materia& materia
            )
            {
                return materia.getId() == id;
            }
        ),
        materias.end()
    );


    // GUARDAR MATERIAS
    if(!Persistencia::guardarMaterias(datos.obtenerUsuarioId(),materias,"materias.txt"))
    {
        return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////
// Editar Materia
/////////////////////////////////////////////////////////////

bool SessionManager::editarMateria(
    int id,
    const std::string& nombre
)
{
    // Debe estar autenticado
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Solo profesores pueden editar materias
    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // Buscar la materia
    std::vector<Materia>& materias = datos.obtenerVectorMaterias();

    for(auto& materia : materias)
    {
        if(materia.getId() == id)
        {
            // La materia pertenece al profesor actual
            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            // Evitar nombre vacío
            if(nombre.empty())
            {
                return false;
            }

            // Evitar nombres repetidos
            for(const auto& otra : materias)
            {
                if(otra.getId() != id && otra.getNombre() == nombre)
                {
                    return false;
                }
            }

            materia.setNombre(nombre);

            return Persistencia::guardarMaterias(datos.obtenerUsuarioId(),
                materias,"materias.txt");
        }
    }

    return false;
}


///////////////////////////////////////////////////////////
// Registrar Calificacion
///////////////////////////////////////////////////////////

bool SessionManager::registrarCalificacion(
    int idAlumno,
    int idTarea,
    double calificacion
)
{
    if(!estaAutenticado())
    {
        return false;
    }

    //--------------------------------------------------
    // Solo profesores
    //--------------------------------------------------

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //--------------------------------------------------
    // Validar parametros
    //--------------------------------------------------

    if(idAlumno <= 0 || idTarea <= 0)
    {
        return false;
    }

    if(
        calificacion < 0.0 ||
        calificacion > 10.0
    )
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //--------------------------------------------------
    // Cargar tareas
    //--------------------------------------------------

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Buscar tarea
    //--------------------------------------------------

    Tarea* tareaEncontrada = nullptr;

    for(auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            tareaEncontrada = &tarea;
            break;
        }
    }

    if(tareaEncontrada == nullptr)
    {
        return false;
    }

    //--------------------------------------------------
    // Cargar materias
    //--------------------------------------------------

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Verificar que la materia pertenezca al profesor
    //--------------------------------------------------

    int idProfesor = datos.obtenerUsuarioId();

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() ==
                tareaEncontrada->getMateriaId()
            &&
            materia.getProfesorId() ==
                idProfesor
        )
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    //--------------------------------------------------
    // Verificar que el alumno esté inscrito
    // en la materia de la tarea
    //--------------------------------------------------

    std::vector<int> alumnosInscritos =
        Inscripciones::obtenerAlumnosMateria(
            tareaEncontrada->getMateriaId()
        );

    bool alumnoInscrito = false;

    for(int alumnoId : alumnosInscritos)
    {
        if(alumnoId == idAlumno)
        {
            alumnoInscrito = true;
            break;
        }
    }

    if(!alumnoInscrito)
    {
        return false;
    }

    //--------------------------------------------------
    // Cargar calificaciones
    //--------------------------------------------------

    std::vector<Calificacion> calificaciones;

    if(!Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        calificaciones.clear();
    }

    //--------------------------------------------------
    // Verificar que no exista
    //--------------------------------------------------

    for(const auto& existente : calificaciones)
    {
        if(
            existente.getIdAlumno() == idAlumno &&
            existente.getIdTarea() == idTarea
        )
        {
            return false;
        }
    }

    //--------------------------------------------------
    // Crear calificacion
    //--------------------------------------------------

    Calificacion nueva(
        idAlumno,
        idTarea,
        calificacion
    );

    calificaciones.push_back(nueva);

    //--------------------------------------------------
    // Guardar
    //--------------------------------------------------

    if(!Persistencia::guardarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    return true;
}


///////////////////////////////////////////////////////////
// Obtener Calificaciones de Alumno
///////////////////////////////////////////////////////////

bool SessionManager::obtenerCalificacionesAlumno(
    int idAlumno,
    std::vector<Calificacion>& calificaciones
)
{
    if(!estaAutenticado())
    {
        return false;
    }

    if(idAlumno <= 0)
    {
        return false;
    }

    //--------------------------------------------------
    // Un alumno solamente puede consultar sus
    // propias calificaciones.
    //--------------------------------------------------

    if(datos.obtenerRol() == "Alumno")
    {
        if(datos.obtenerUsuarioId() != idAlumno)
        {
            return false;
        }
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //--------------------------------------------------
    // Cargar todas las calificaciones
    //--------------------------------------------------

    std::vector<Calificacion> todas;

    if(!Persistencia::cargarCalificaciones(
        todas,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Filtrar por alumno
    //--------------------------------------------------

    calificaciones.clear();

    for(const auto& calificacion : todas)
    {
        if(
            calificacion.getIdAlumno() ==
            idAlumno
        )
        {
            calificaciones.push_back(
                calificacion
            );
        }
    }

    return true;
}


///////////////////////////////////////////////////////////
// Editar Calificacion
///////////////////////////////////////////////////////////

bool SessionManager::editarCalificacion(
    int idAlumno,
    int idTarea,
    double calificacion
)
{
    if(!estaAutenticado())
    {
        return false;
    }

    //--------------------------------------------------
    // Solo profesores
    //--------------------------------------------------

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //--------------------------------------------------
    // Validar parámetros
    //--------------------------------------------------

    if(idAlumno <= 0 || idTarea <= 0)
    {
        return false;
    }

    if(
        calificacion < 0.0 ||
        calificacion > 10.0
    )
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //--------------------------------------------------
    // Cargar tareas
    //--------------------------------------------------

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Buscar tarea
    //--------------------------------------------------

    Tarea* tareaEncontrada = nullptr;

    for(auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            tareaEncontrada = &tarea;
            break;
        }
    }

    if(tareaEncontrada == nullptr)
    {
        return false;
    }

    //--------------------------------------------------
    // Verificar materia del profesor
    //--------------------------------------------------

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"
    ))
    {
        return false;
    }

    int idProfesor = datos.obtenerUsuarioId();

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() ==
                tareaEncontrada->getMateriaId()
            &&
            materia.getProfesorId() ==
                idProfesor
        )
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    //--------------------------------------------------
    // Cargar calificaciones
    //--------------------------------------------------

    std::vector<Calificacion> calificaciones;

    if(!Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Buscar calificación
    //--------------------------------------------------

    bool encontrada = false;

    for(auto& existente : calificaciones)
    {
        if(
            existente.getIdAlumno() == idAlumno &&
            existente.getIdTarea() == idTarea
        )
        {
            existente.setCalificacion(
                calificacion
            );

            encontrada = true;
            break;
        }
    }

    if(!encontrada)
    {
        return false;
    }

    //--------------------------------------------------
    // Guardar
    //--------------------------------------------------

    if(!Persistencia::guardarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    return true;
}


///////////////////////////////////////////////////////////
// Eliminar Calificacion
///////////////////////////////////////////////////////////

bool SessionManager::eliminarCalificacion(
    int idAlumno,
    int idTarea
)
{
    if(!estaAutenticado())
    {
        return false;
    }

    //--------------------------------------------------
    // Solo profesores
    //--------------------------------------------------

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //--------------------------------------------------
    // Validar parámetros
    //--------------------------------------------------

    if(idAlumno <= 0 || idTarea <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //--------------------------------------------------
    // Cargar tarea
    //--------------------------------------------------

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Buscar tarea
    //--------------------------------------------------

    Tarea* tareaEncontrada = nullptr;

    for(auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            tareaEncontrada = &tarea;
            break;
        }
    }

    if(tareaEncontrada == nullptr)
    {
        return false;
    }

    //--------------------------------------------------
    // Verificar materia del profesor
    //--------------------------------------------------

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"
    ))
    {
        return false;
    }

    int idProfesor = datos.obtenerUsuarioId();

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() ==
                tareaEncontrada->getMateriaId()
            &&
            materia.getProfesorId() ==
                idProfesor
        )
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    //--------------------------------------------------
    // Cargar calificaciones
    //--------------------------------------------------

    std::vector<Calificacion> calificaciones;

    if(!Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    //--------------------------------------------------
    // Buscar y eliminar
    //--------------------------------------------------

    auto it = std::find_if(
        calificaciones.begin(),
        calificaciones.end(),
        [idAlumno, idTarea](
            const Calificacion& calificacion
        )
        {
            return
                calificacion.getIdAlumno() ==
                    idAlumno
                &&
                calificacion.getIdTarea() ==
                    idTarea;
        }
    );

    if(it == calificaciones.end())
    {
        return false;
    }

    calificaciones.erase(it);

    //--------------------------------------------------
    // Guardar
    //--------------------------------------------------

    if(!Persistencia::guardarCalificaciones(
        calificaciones,
        "calificaciones.txt"
    ))
    {
        return false;
    }

    return true;
}


///////////////////////////////////////////////////////////
// OBTENER CALIFICACION FINAL
///////////////////////////////////////////////////////////

bool SessionManager::obtenerCalificacionFinal(
    int idAlumno,
    int idMateria,
    double& calificacionFinal
)
{
    if(!datos.estaAutenticado())
    {
        return false;
    }

    if(idAlumno <= 0 || idMateria <= 0)
    {
        return false;
    }

    //=======================================================
    // UN SOLO LOCK PARA TODA LA OPERACION
    //=======================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //=======================================================
    // CARGAR TAREAS
    //=======================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //=======================================================
    // CARGAR CALIFICACIONES
    //=======================================================

    std::vector<Calificacion> calificaciones;

    if(!Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"))
    {
        return false;
    }

    //=======================================================
    // CARGAR PONDERACIONES
    //=======================================================

    std::vector<Ponderacion> ponderaciones;

    if(!Persistencia::cargarPonderaciones(
        ponderaciones,
        "ponderaciones.txt"))
    {
        return false;
    }

    //=======================================================
    // BUSCAR TAREAS DE LA MATERIA
    //=======================================================

    std::vector<Tarea> tareasMateria;

    for(const auto& tarea : tareas)
    {
        if(tarea.getMateriaId() == idMateria)
        {
            tareasMateria.push_back(tarea);
        }
    }

    if(tareasMateria.empty())
    {
        return false;
    }

    //=======================================================
    // DETERMINAR SI LA MATERIA ES ACUMULATIVA
    //=======================================================

    bool acumulativa = false;

    for(const auto& ponderacion : ponderaciones)
    {
        if(ponderacion.getIdMateria() == idMateria &&
           ponderacion.getParcial() == 0)
        {
            acumulativa = true;
            break;
        }
    }

    //=======================================================
    // CALCULAR UN PARCIAL
    //=======================================================

    auto calcularParcial = [&](
        int numeroParcial
    )
    {
        const Ponderacion* ponderacionSeleccionada = nullptr;

        //===================================================
        // BUSCAR PONDERACION DEL PARCIAL
        //===================================================

        if(numeroParcial > 0)
        {
            for(const auto& ponderacion : ponderaciones)
            {
                if(ponderacion.getIdMateria() == idMateria &&
                   ponderacion.getParcial() == numeroParcial)
                {
                    ponderacionSeleccionada = &ponderacion;
                    break;
                }
            }
        }

        //===================================================
        // BUSCAR PONDERACION GENERAL
        //===================================================

        if(ponderacionSeleccionada == nullptr)
        {
            for(const auto& ponderacion : ponderaciones)
            {
                if(ponderacion.getIdMateria() == idMateria &&
                   ponderacion.getParcial() == 0)
                {
                    ponderacionSeleccionada = &ponderacion;
                    break;
                }
            }
        }

        if(ponderacionSeleccionada == nullptr)
        {
            return -1.0;
        }

        //===================================================
        // SUMAS DE CALIFICACIONES
        //===================================================

        double sumaTareas = 0.0;
        double sumaExamenes = 0.0;
        double sumaPracticas = 0.0;
        double sumaProyectos = 0.0;
        double sumaTrabajos = 0.0;
        double sumaOtros = 0.0;

        //===================================================
        // CANTIDAD DE CALIFICACIONES
        //===================================================

        int cantidadTareas = 0;
        int cantidadExamenes = 0;
        int cantidadPracticas = 0;
        int cantidadProyectos = 0;
        int cantidadTrabajos = 0;
        int cantidadOtros = 0;

        //===================================================
        // BUSCAR CALIFICACIONES DEL ALUMNO
        //===================================================

        for(const auto& tarea : tareasMateria)
        {
            // En una materia por parciales solamente
            // tomamos las tareas del parcial correspondiente.

            if(!acumulativa && tarea.getParcial() != numeroParcial)
            {
                continue;
            }

            for(const auto& calificacion : calificaciones)
            {
                if(calificacion.getIdAlumno() != idAlumno)
                {
                    continue;
                }

                if(calificacion.getIdTarea() != tarea.getId())
                {
                    continue;
                }

                switch(tarea.getTipo())
                {
                    case TipoTarea::TAREA:

                        sumaTareas += calificacion.getCalificacion();

                        cantidadTareas++;

                        break;

                    case TipoTarea::EXAMEN:

                        sumaExamenes += calificacion.getCalificacion();

                        cantidadExamenes++;

                        break;

                    case TipoTarea::PRACTICA:

                        sumaPracticas += calificacion.getCalificacion();

                        cantidadPracticas++;

                        break;

                    case TipoTarea::PROYECTO:

                        sumaProyectos += calificacion.getCalificacion();

                        cantidadProyectos++;

                        break;

                    case TipoTarea::TRABAJO:

                        sumaTrabajos += calificacion.getCalificacion();

                        cantidadTrabajos++;

                        break;

                    case TipoTarea::OTRO:

                        sumaOtros += calificacion.getCalificacion();

                        cantidadOtros++;

                        break;
                }

                break;
            }
        }

        //===================================================
        // CALCULAR PROMEDIO DE CADA TIPO
        //===================================================

        double promedioTareas = 0.0;
        double promedioExamenes = 0.0;
        double promedioPracticas = 0.0;
        double promedioProyectos = 0.0;
        double promedioTrabajos = 0.0;
        double promedioOtros = 0.0;

        if(cantidadTareas > 0)
        {
            promedioTareas = sumaTareas / cantidadTareas;
        }

        if(cantidadExamenes > 0)
        {
            promedioExamenes = sumaExamenes / cantidadExamenes;
        }

        if(cantidadPracticas > 0)
        {
            promedioPracticas = sumaPracticas / cantidadPracticas;
        }

        if(cantidadProyectos > 0)
        {
            promedioProyectos = sumaProyectos / cantidadProyectos;
        }

        if(cantidadTrabajos > 0)
        {
            promedioTrabajos = sumaTrabajos / cantidadTrabajos;
        }

        if(cantidadOtros > 0)
        {
            promedioOtros = sumaOtros / cantidadOtros;
        }

        //===================================================
        // APLICAR PONDERACIONES
        //===================================================

        double resultado = 0.0;

        resultado += promedioTareas * (ponderacionSeleccionada->getTarea() / 100.0);

        resultado += promedioExamenes * (ponderacionSeleccionada->getExamen() / 100.0);

        resultado += promedioPracticas * (ponderacionSeleccionada->getPractica() / 100.0);

        resultado += promedioProyectos * (ponderacionSeleccionada->getProyecto() / 100.0);

        resultado += promedioTrabajos * (ponderacionSeleccionada->getTrabajo() / 100.0);

        resultado += promedioOtros * (ponderacionSeleccionada->getOtro() / 100.0);

        return resultado;
    };

    //=======================================================
    // MATERIA ACUMULATIVA
    //=======================================================

    if(acumulativa)
    {
        double resultado = calcularParcial(0);

        if(resultado < 0.0)
        {
            return false;
        }

        calificacionFinal = resultado;

        return true;
    }

    //=======================================================
    // OBTENER LOS PARCIALES EXISTENTES
    //=======================================================

    std::vector<int> parciales;

    for(const auto& tarea : tareasMateria)
    {
        int parcial = tarea.getParcial();

        if(parcial <= 0)
        {
            continue;
        }

        bool yaExiste = false;

        for(int parcialExistente : parciales)
        {
            if(parcialExistente == parcial)
            {
                yaExiste = true;
                break;
            }
        }

        if(!yaExiste)
        {
            parciales.push_back(parcial);
        }
    }

    if(parciales.empty())
    {
        return false;
    }

    //=======================================================
    // CALCULAR TODOS LOS PARCIALES
    //=======================================================

    double sumaParciales = 0.0;

    for(int parcial : parciales)
    {
        double resultado = calcularParcial(parcial);

        if(resultado < 0.0)
        {
            return false;
        }

        sumaParciales += resultado;
    }

    //=======================================================
    // TODOS LOS PARCIALES VALEN LO MISMO
    //=======================================================

    calificacionFinal = sumaParciales / parciales.size();

    return true;
}


///////////////////////////////////////////////////////////
// Agregar Usuario
///////////////////////////////////////////////////////////

bool SessionManager::agregarUsuario(
    const std::string& rol,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{
    // Solo el Administrador puede crear usuarios
    if(!datos.estaAutenticado())
    {
        return false;
    }

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // Validar campos vacíos
    if(nombre.empty() || correo.empty() || password.empty() || identificador.empty())
    {
        return false;
    }

    // Validar rol
    if(rol != "Administrador" && rol != "Profesor" && rol != "Alumno")
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // Cargar usuarios existentes
    std::vector<Usuario*> usuarios;

    Persistencia::cargarUsuarios(usuarios,"usuarios.txt");

    // Validar correo e identificador repetidos
    for(const auto* usuario : usuarios)
    {
        if(usuario == nullptr)
        {
            continue;
        }

        if(usuario->getCorreo() == correo)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }

        if(usuario->getIdentificador() == identificador)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }
    }

    // Generar ID
    int id = Persistencia::generarIdUsuario("usuarios.txt");

    // Crear usuario según el rol
    Usuario* nuevoUsuario = nullptr;

    if(rol == "Administrador")
    {
        nuevoUsuario = new Administrador(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }
    else if(rol == "Profesor")
    {
        nuevoUsuario = new Profesor(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }
    else if(rol == "Alumno")
    {
        nuevoUsuario = new Alumno(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }

    if(nuevoUsuario == nullptr)
    {
        for(auto* u : usuarios)
        {
            delete u;
        }

        return false;
    }

    // Guardar usuario
    bool resultado =Persistencia::agregarUsuario(*nuevoUsuario,"usuarios.txt");

    // Liberar memoria
    delete nuevoUsuario;

    for(auto* u : usuarios)
    {
        delete u;
    }

    return resultado;
}

///////////////////////////////////////////////////////////
// Obtener Usuarios
///////////////////////////////////////////////////////////

std::string SessionManager::obtenerUsuarios() const
{
    
    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(usuarios))
    {
        return "ERROR";
    }

    std::string respuesta = "USUARIOS|";

    for(size_t i = 0; i < usuarios.size(); i++)
    {
        respuesta += std::to_string(usuarios[i]->getId());
        respuesta += ",";
        respuesta += usuarios[i]->getRol();
        respuesta += ",";
        respuesta += usuarios[i]->getNombre();
        respuesta += ",";
        respuesta += usuarios[i]->getCorreo();

        if(i != usuarios.size()-1)
        {
            respuesta += ";";
        }
    }

    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    return respuesta;
}

///////////////////////////////////////////////////////////
// Actualizar Usuario
///////////////////////////////////////////////////////////

bool SessionManager::actualizarUsuario(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{

    // Debe estar autenticado
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // ---------------------------------------------
    // Solo el Administrador puede actualizar
    // usuarios
    // ---------------------------------------------

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // ---------------------------------------------
    // Validar campos
    // ---------------------------------------------

    if(nombre.empty() || correo.empty() || password.empty() || identificador.empty())
    {
        return false;
    }


    std::lock_guard<std::mutex> lock(mutexDatos);

    // Cargar usuarios
    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(usuarios,"usuarios.txt"))
    {
        return false;
    }

    Usuario* usuarioObjetivo = nullptr;

    // Buscar usuario
    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr && usuario->getId() == id)
        {
            usuarioObjetivo = usuario;
            break;
        }
    }

    if(usuarioObjetivo == nullptr)
    {
        for(auto* usuario : usuarios)
        {
            delete usuario;
        }

        return false;
    }

    // Validar correo e identificador duplicados
    for(auto* usuario : usuarios)
    {
        if(usuario == nullptr)
        {
            continue;
        }

        // No comparar el usuario consigo mismo
        if(usuario->getId() == id)
        {
            continue;
        }

        if(usuario->getCorreo() == correo)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }

        if(usuario->getIdentificador() == identificador)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }
    }

    // Actualizar datos
    usuarioObjetivo->setNombre(nombre);
    usuarioObjetivo->setCorreo(correo);
    usuarioObjetivo->setPassword(password);
    usuarioObjetivo->setIdentificador(identificador);

    // Guardar
    bool resultado = Persistencia::actualizarUsuario(*usuarioObjetivo,"usuarios.txt");

    // Liberar memoria
    for(auto* usuario : usuarios)
    {
        delete usuario;
    }

    return resultado;
}

/////////////////////////////////////////////////////////////
// Eliminar Usuario
/////////////////////////////////////////////////////////////

bool SessionManager::eliminarUsuario(
    int id
)
{

    // AUTENTICACIÓN
    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // SOLO ADMINISTRADORES
    //==================================================

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    //==================================================
    // VALIDAR ID
    //==================================================

    if(id <= 0)
    {
        return false;
    }

    //==================================================
    // NO PUEDE ELIMINARSE A SÍ MISMO
    //==================================================

    if(id == datos.obtenerUsuarioId())
    {
        return false;
    }

    //==================================================
    // BLOQUEAR DATOS COMPARTIDOS
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // CARGAR USUARIOS
    //==================================================

    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"))
    {
        return false;
    }

    //==================================================
    // BUSCAR USUARIO
    //==================================================

    Usuario* usuarioObjetivo = nullptr;

    for(auto* usuario : usuarios)
    {
        if(
            usuario != nullptr &&
            usuario->getId() == id
        )
        {
            usuarioObjetivo = usuario;
            break;
        }
    }

    if(usuarioObjetivo == nullptr)
    {
        for(auto* usuario : usuarios)
        {
            delete usuario;
        }

        return false;
    }

    //==================================================
    // NO ELIMINAR ÚLTIMO ADMINISTRADOR
    //==================================================

    if(usuarioObjetivo->getRol() == "Administrador")
    {
        int cantidadAdministradores = 0;

        for(auto* usuario : usuarios)
        {
            if(
                usuario != nullptr &&
                usuario->getRol() == "Administrador"
            )
            {
                cantidadAdministradores++;
            }
        }

        if(cantidadAdministradores <= 1)
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }
    }

    //==================================================
    // CASO ALUMNO
    //==================================================

    if(usuarioObjetivo->getRol() == "Alumno")
    {
        //================================================
        // OBTENER MATERIAS INSCRITAS
        //================================================

        std::vector<int> materiasAlumno =
            Inscripciones::obtenerMateriasAlumno(
                id,
                "inscripciones.txt"
            );

        //================================================
        // ELIMINAR ESTADOS
        //================================================

        std::vector<EstadoTareaAlumno> estados;

        if(Persistencia::cargarEstadosTareas(
            estados,
            "estadosTareas.txt"))
        {
            estados.erase(
                std::remove_if(
                    estados.begin(),
                    estados.end(),
                    [id](
                        const EstadoTareaAlumno& estado
                    )
                    {
                        return
                            estado.getAlumnoId() == id;
                    }
                ),
                estados.end()
            );

            if(!Persistencia::guardarEstadosTareas(
                estados,
                "estadosTareas.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR CALIFICACIONES DEL ALUMNO
        //================================================

        std::vector<Calificacion> calificaciones;

        if(Persistencia::cargarCalificaciones(
            calificaciones,
            "calificaciones.txt"))
        {
            calificaciones.erase(
                std::remove_if(
                    calificaciones.begin(),
                    calificaciones.end(),
                    [id](
                        const Calificacion& calificacion
                    )
                    {
                        return
                            calificacion.getIdAlumno() ==
                            id;
                    }
                ),
                calificaciones.end()
            );

            if(!Persistencia::guardarCalificaciones(
                calificaciones,
                "calificaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR SUBTAREAS DEL ALUMNO
        //================================================

        std::vector<Subtarea> subtareas;

        if(Persistencia::cargarSubtareas(
            subtareas,
            "subtareas.txt"))
        {
            subtareas.erase(
                std::remove_if(
                    subtareas.begin(),
                    subtareas.end(),
                    [id](
                        const Subtarea& subtarea
                    )
                    {
                        return
                            subtarea.getAlumnoId() == id;
                    }
                ),
                subtareas.end()
            );

            if(!Persistencia::guardarSubtareas(
                subtareas,
                "subtareas.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR PLANNER
        //================================================

        PlannerSemana plannerVacio;

        if(!Persistencia::guardarPlanner(
            id,
            plannerVacio,
            "planner.txt"))
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }

        //================================================
        // ELIMINAR INSCRIPCIONES
        //================================================

        for(int idMateria : materiasAlumno)
        {
            if(!Inscripciones::desinscribirAlumno(
                idMateria,
                id,
                "inscripciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR NOTIFICACIONES DEL ALUMNO
        //================================================

        std::vector<Notificacion> notificaciones;

        if(Persistencia::cargarNotificaciones(
            notificaciones,
            "notificaciones.txt"))
        {
            notificaciones.erase(
                std::remove_if(
                    notificaciones.begin(),
                    notificaciones.end(),
                    [id](
                        const Notificacion& notificacion
                    )
                    {
                        return
                            notificacion.getUsuarioId() ==
                            id;
                    }
                ),
                notificaciones.end()
            );

            if(!Persistencia::guardarNotificaciones(
                notificaciones,
                "notificaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }
    }

    //==================================================
    // CASO PROFESOR
    //==================================================

    else if(usuarioObjetivo->getRol() == "Profesor")
    {
        //================================================
        // CARGAR MATERIAS
        //================================================

        std::vector<Materia> materias;

        if(!Persistencia::cargarMaterias(
            materias,
            "materias.txt"))
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }

        //================================================
        // OBTENER IDS DE MATERIAS
        //================================================

        std::vector<int> idsMaterias;

        for(const auto& materia : materias)
        {
            if(materia.getProfesorId() == id)
            {
                idsMaterias.push_back(
                    materia.getId()
                );
            }
        }

        //================================================
        // CARGAR TAREAS
        //================================================

        std::vector<Tarea> tareas;

        if(!Persistencia::cargarTareas(
            tareas,
            "tareas.txt"))
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }

        //================================================
        // OBTENER IDS DE TAREAS
        //================================================

        std::vector<int> idsTareas;

        for(const auto& tarea : tareas)
        {
            if(
                std::find(
                    idsMaterias.begin(),
                    idsMaterias.end(),
                    tarea.getMateriaId()
                ) != idsMaterias.end()
            )
            {
                idsTareas.push_back(
                    tarea.getId()
                );
            }
        }

        //================================================
        // CARGAR SUBTAREAS
        //================================================

        std::vector<Subtarea> subtareas;

        bool haySubtareas =
            Persistencia::cargarSubtareas(
                subtareas,
                "subtareas.txt"
            );

        std::vector<int> idsSubtareas;

        if(haySubtareas)
        {
            for(const auto& subtarea : subtareas)
            {
                if(
                    std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        subtarea.getTareaId()
                    ) != idsTareas.end()
                )
                {
                    idsSubtareas.push_back(
                        subtarea.getId()
                    );
                }
            }
        }

        //================================================
        // OBTENER ALUMNOS AFECTADOS
        //================================================

        std::vector<int> alumnosAfectados;

        for(int idMateria : idsMaterias)
        {
            std::vector<int> alumnos =
                Inscripciones::obtenerAlumnosMateria(
                    idMateria,
                    "inscripciones.txt"
                );

            for(int idAlumno : alumnos)
            {
                if(
                    std::find(
                        alumnosAfectados.begin(),
                        alumnosAfectados.end(),
                        idAlumno
                    ) == alumnosAfectados.end()
                )
                {
                    alumnosAfectados.push_back(
                        idAlumno
                    );
                }
            }
        }

        //================================================
        // ELIMINAR TAREAS
        //================================================

        tareas.erase(
            std::remove_if(
                tareas.begin(),
                tareas.end(),
                [&idsMaterias](
                    const Tarea& tarea
                )
                {
                    return std::find(
                        idsMaterias.begin(),
                        idsMaterias.end(),
                        tarea.getMateriaId()
                    ) != idsMaterias.end();
                }
            ),
            tareas.end()
        );

        if(!Persistencia::guardarTareas(
            tareas,
            "tareas.txt"))
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }

        //================================================
        // ELIMINAR ESTADOS
        //================================================

        std::vector<EstadoTareaAlumno> estados;

        if(Persistencia::cargarEstadosTareas(
            estados,
            "estadosTareas.txt"))
        {
            estados.erase(
                std::remove_if(
                    estados.begin(),
                    estados.end(),
                    [&idsTareas](
                        const EstadoTareaAlumno& estado
                    )
                    {
                        return std::find(
                            idsTareas.begin(),
                            idsTareas.end(),
                            estado.getTareaId()
                        ) != idsTareas.end();
                    }
                ),
                estados.end()
            );

            if(!Persistencia::guardarEstadosTareas(
                estados,
                "estadosTareas.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR CALIFICACIONES
        // DE LAS TAREAS
        //================================================

        std::vector<Calificacion> calificaciones;

        if(Persistencia::cargarCalificaciones(
            calificaciones,
            "calificaciones.txt"))
        {
            calificaciones.erase(
                std::remove_if(
                    calificaciones.begin(),
                    calificaciones.end(),
                    [&idsTareas](
                        const Calificacion& calificacion
                    )
                    {
                        return std::find(
                            idsTareas.begin(),
                            idsTareas.end(),
                            calificacion.getIdTarea()
                        ) != idsTareas.end();
                    }
                ),
                calificaciones.end()
            );

            if(!Persistencia::guardarCalificaciones(
                calificaciones,
                "calificaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR PONDERACIONES
        // DE LAS MATERIAS
        //================================================

        std::vector<Ponderacion> ponderaciones;

        if(Persistencia::cargarPonderaciones(
            ponderaciones,
            "ponderaciones.txt"))
        {
            ponderaciones.erase(
                std::remove_if(
                    ponderaciones.begin(),
                    ponderaciones.end(),
                    [&idsMaterias](
                        const Ponderacion& ponderacion
                    )
                    {
                        return std::find(
                            idsMaterias.begin(),
                            idsMaterias.end(),
                            ponderacion.getIdMateria()
                        ) != idsMaterias.end();
                    }
                ),
                ponderaciones.end()
            );

            if(!Persistencia::guardarPonderaciones(
                ponderaciones,
                "ponderaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR SUBTAREAS
        //================================================

        if(haySubtareas)
        {
            subtareas.erase(
                std::remove_if(
                    subtareas.begin(),
                    subtareas.end(),
                    [&idsTareas](
                        const Subtarea& subtarea
                    )
                    {
                        return std::find(
                            idsTareas.begin(),
                            idsTareas.end(),
                            subtarea.getTareaId()
                        ) != idsTareas.end();
                    }
                ),
                subtareas.end()
            );

            if(!Persistencia::guardarSubtareas(
                subtareas,
                "subtareas.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR DEL PLANNER
        //================================================

        for(int idAlumno : alumnosAfectados)
        {
            PlannerSemana planner;

            if(!Persistencia::cargarPlanner(
                idAlumno,
                planner,
                "planner.txt"))
            {
                continue;
            }

            for(int idTarea : idsTareas)
            {
                for(int i = 0; i < 7; ++i)
                {
                    planner.getDia(i).eliminarTarea(
                        idTarea
                    );
                }
            }

            for(int idSubtarea : idsSubtareas)
            {
                for(int i = 0; i < 7; ++i)
                {
                    planner.getDia(i).eliminarSubtarea(
                        idSubtarea
                    );
                }
            }

            if(!Persistencia::guardarPlanner(
                idAlumno,
                planner,
                "planner.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR NOTIFICACIONES DE LAS TAREAS
        //================================================

        std::vector<Notificacion> notificaciones;

        if(Persistencia::cargarNotificaciones(
            notificaciones,
            "notificaciones.txt"))
        {
            notificaciones.erase(
                std::remove_if(
                    notificaciones.begin(),
                    notificaciones.end(),
                    [&idsTareas](
                        const Notificacion& notificacion
                    )
                    {
                        return
                            notificacion.getTipoReferencia() ==
                                TipoReferenciaNotificacion::TAREA
                            &&
                            std::find(
                                idsTareas.begin(),
                                idsTareas.end(),
                                notificacion.getIdReferencia()
                            ) != idsTareas.end();
                    }
                ),
                notificaciones.end()
            );

            if(!Persistencia::guardarNotificaciones(
                notificaciones,
                "notificaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }

        //================================================
        // ELIMINAR INSCRIPCIONES
        //================================================

        for(int idMateria : idsMaterias)
        {
            std::vector<int> alumnos =
                Inscripciones::obtenerAlumnosMateria(
                    idMateria,
                    "inscripciones.txt"
                );

            for(int idAlumno : alumnos)
            {
                if(!Inscripciones::desinscribirAlumno(
                    idMateria,
                    idAlumno,
                    "inscripciones.txt"))
                {
                    for(auto* usuario : usuarios)
                    {
                        delete usuario;
                    }

                    return false;
                }
            }
        }

        //================================================
        // ELIMINAR MATERIAS
        //================================================

        materias.erase(
            std::remove_if(
                materias.begin(),
                materias.end(),
                [id](
                    const Materia& materia
                )
                {
                    return materia.getProfesorId() == id;
                }
            ),
            materias.end()
        );

        if(!Persistencia::guardarMaterias(
            id,
            materias,
            "materias.txt"))
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

            return false;
        }

        //================================================
        // ELIMINAR NOTIFICACIONES DEL PROFESOR
        //================================================

        std::vector<Notificacion> notificacionesProfesor;

        if(Persistencia::cargarNotificaciones(
            notificacionesProfesor,
            "notificaciones.txt"))
        {
            notificacionesProfesor.erase(
                std::remove_if(
                    notificacionesProfesor.begin(),
                    notificacionesProfesor.end(),
                    [id](
                        const Notificacion& notificacion
                    )
                    {
                        return
                            notificacion.getUsuarioId() ==
                            id;
                    }
                ),
                notificacionesProfesor.end()
            );

            if(!Persistencia::guardarNotificaciones(
                notificacionesProfesor,
                "notificaciones.txt"))
            {
                for(auto* usuario : usuarios)
                {
                    delete usuario;
                }

                return false;
            }
        }
    }

    //==================================================
    // ELIMINAR USUARIO
    //==================================================

    bool resultado =
        Persistencia::eliminarUsuario(
            id,
            "usuarios.txt"
        );

    //==================================================
    // LIBERAR MEMORIA
    //==================================================

    for(auto* usuario : usuarios)
    {
        delete usuario;
    }

    return resultado;
}

///////////////////////////////////////////////////////////
// Obtener Rol
///////////////////////////////////////////////////////////

std::string SessionManager::obtenerRol() const
{
    return datos.obtenerRol();
}

////////////////////////////////////////////////////////////
// Inscribir Alumno
////////////////////////////////////////////////////////////

bool SessionManager::inscribirAlumno(
    int idMateria,
    int idAlumno
)
{
    
    // Autenticación
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Solo profesores
    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    // Validar IDs
    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // Cargar materias
    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(materias,"materias.txt"))
    {
        return false;
    }

    // Buscar materia y verificar propietario
    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            materiaEncontrada = true;

            // La materia debe pertenecer
            // al profesor que está realizando
            // la operación.

            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            break;
        }
    }

    if(!materiaEncontrada)
    {
        return false;
    }

    // Cargar usuarios
    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(usuarios,"usuarios.txt"))
    {
        return false;
    }

    // Verificar que el usuario sea alumno
    bool alumnoEncontrado = false;

    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr && usuario->getId() == idAlumno)
        {
            if(usuario->getRol() == "Alumno")
            {
                alumnoEncontrado = true;
            }

            break;
        }
    }

    // Liberar memoria
    for(auto* usuario : usuarios)
    {
        delete usuario;
    }

    if(!alumnoEncontrado)
    {
        return false;
    }

    // Verificar que no esté inscrito
    if(Inscripciones::estaInscrito(idMateria,idAlumno,"inscripciones.txt"))
    {
        return false;
    }

    // Cargar tareas existentes de la materia
   std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    // Cargar estados existentes
    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(estados,"estadosTareas.txt"))
    {
        // Si el archivo todavía no existe,
        // comenzamos con una lista vacía.

        estados.clear();
    }

    // Crear estados para las tareas existentes de esta materia
    for(const auto& tarea : tareas)
    {
        // Solo tareas de la materia
        if(tarea.getMateriaId() != idMateria)
        {
            continue;
        }

        // Verificar si ya existe el estado
        bool estadoExiste = false;

        for(const auto& estado : estados)
        {
            if(estado.getTareaId() == tarea.getId() &&
                estado.getAlumnoId() == idAlumno
            )
            {
                estadoExiste = true;
                break;
            }
        }

        // Si no existe, crear estado inicial
        if(!estadoExiste)
        {
            estados.emplace_back(
                tarea.getId(),
                idAlumno,
                EstadoTarea::NO_COMPLETADO
            );
        }
    }

    // Crear inscripción
    if(!Inscripciones::inscribirAlumno(idMateria,idAlumno,"inscripciones.txt"))
    {
        return false;
    }

    // Guardar estados de tareas
    if(!Persistencia::guardarEstadosTareas(estados,"estadosTareas.txt"))
    {
        // Si no se pudieron guardar los estados,
        // intentamos deshacer la inscripción para
        // evitar dejar datos inconsistentes.

        Inscripciones::desinscribirAlumno(
            idMateria,
            idAlumno,
            "inscripciones.txt"
        );

        return false;
    }

    //==================================================
    // IMPORTANTE:
    //
    // NO modificar planner.txt aquí.
    //
    // GET_PLANNER será quien determine qué tareas
    // aparecen en la semana actual.
    //==================================================

    return true;
}


////////////////////////////////////////////////////////////
// Desinscribir Alumno
////////////////////////////////////////////////////////////

bool SessionManager::desinscribirAlumno(
    int idMateria,
    int idAlumno
)
{
    //==================================================
    // AUTENTICACIÓN
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // SOLO PROFESORES
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //==================================================
    // VALIDAR IDs
    //==================================================

    if(
        idMateria <= 0 ||
        idAlumno <= 0
    )
    {
        return false;
    }

    //==================================================
    // BLOQUEAR DATOS COMPARTIDOS
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // CARGAR MATERIAS
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return false;
    }

    //==================================================
    // VERIFICAR QUE LA MATERIA
    // EXISTA Y PERTENEZCA AL PROFESOR
    //==================================================

    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            materiaEncontrada = true;

            if(
                materia.getProfesorId() !=
                datos.obtenerUsuarioId()
            )
            {
                return false;
            }

            break;
        }
    }

    if(!materiaEncontrada)
    {
        return false;
    }

    //==================================================
    // VERIFICAR QUE EL ALUMNO ESTÉ INSCRITO
    //==================================================

    std::vector<int> alumnosInscritos =
        Inscripciones::obtenerAlumnosMateria(
            idMateria,
            "inscripciones.txt"
        );

    bool alumnoInscrito = false;

    for(int id : alumnosInscritos)
    {
        if(id == idAlumno)
        {
            alumnoInscrito = true;

            break;
        }
    }

    if(!alumnoInscrito)
    {
        return false;
    }

    //==================================================
    // CARGAR TAREAS
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // OBTENER IDS DE LAS TAREAS
    // DE LA MATERIA
    //==================================================

    std::vector<int> idsTareas;

    for(const auto& tarea : tareas)
    {
        if(tarea.getMateriaId() == idMateria)
        {
            idsTareas.push_back(
                tarea.getId()
            );
        }
    }

    //==================================================
    // ELIMINAR ESTADOS DEL ALUMNO
    // DE LAS TAREAS DE LA MATERIA
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(Persistencia::cargarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        estados.erase(
            std::remove_if(
                estados.begin(),
                estados.end(),
                [&idsTareas, idAlumno](
                    const EstadoTareaAlumno& estado
                )
                {
                    // Verificar que sea del alumno
                    if(
                        estado.getAlumnoId() !=
                        idAlumno
                    )
                    {
                        return false;
                    }

                    // Verificar que la tarea
                    // pertenezca a esta materia
                    return std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        estado.getTareaId()
                    ) != idsTareas.end();
                }
            ),
            estados.end()
        );

        if(!Persistencia::guardarEstadosTareas(
            estados,
            "estadosTareas.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR CALIFICACIONES DEL ALUMNO
    // DE LAS TAREAS DE LA MATERIA
    //==================================================

    std::vector<Calificacion> calificaciones;

    if(Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"))
    {
        calificaciones.erase(
            std::remove_if(
                calificaciones.begin(),
                calificaciones.end(),
                [&idsTareas, idAlumno](
                    const Calificacion& calificacion
                )
                {
                    // Verificar que sea del alumno
                    if(
                        calificacion.getIdAlumno() !=
                        idAlumno
                    )
                    {
                        return false;
                    }

                    // Verificar que la tarea
                    // pertenezca a esta materia
                    return std::find(
                        idsTareas.begin(),
                        idsTareas.end(),
                        calificacion.getIdTarea()
                    ) != idsTareas.end();
                }
            ),
            calificaciones.end()
        );

        if(!Persistencia::guardarCalificaciones(
            calificaciones,
            "calificaciones.txt"))
        {
            return false;
        }
    }

    //==================================================
    // CARGAR SUBTAREAS
    //==================================================

    std::vector<Subtarea> subtareas;

    std::vector<int> idsSubtareas;

    if(Persistencia::cargarSubtareas(
        subtareas,
        "subtareas.txt"))
    {
        //================================================
        // OBTENER IDS DE SUBTAREAS
        // DEL ALUMNO Y DE LAS TAREAS
        // DE ESTA MATERIA
        //================================================

        for(const auto& subtarea : subtareas)
        {
            if(
                subtarea.getAlumnoId() !=
                idAlumno
            )
            {
                continue;
            }

            for(int idTarea : idsTareas)
            {
                if(
                    subtarea.getTareaId() ==
                    idTarea
                )
                {
                    idsSubtareas.push_back(
                        subtarea.getId()
                    );

                    break;
                }
            }
        }

        //================================================
        // ELIMINAR SUBTAREAS
        //================================================

        subtareas.erase(
            std::remove_if(
                subtareas.begin(),
                subtareas.end(),
                [&idsSubtareas](
                    const Subtarea& subtarea
                )
                {
                    return std::find(
                        idsSubtareas.begin(),
                        idsSubtareas.end(),
                        subtarea.getId()
                    ) != idsSubtareas.end();
                }
            ),
            subtareas.end()
        );

        //================================================
        // GUARDAR SUBTAREAS
        //================================================

        if(!Persistencia::guardarSubtareas(
            subtareas,
            "subtareas.txt"))
        {
            return false;
        }
    }

    //==================================================
    // LIMPIAR PLANNER DEL ALUMNO
    //==================================================

    PlannerSemana planner;

    if(Persistencia::cargarPlanner(
        idAlumno,
        planner,
        "planner.txt"))
    {
        //================================================
        // ELIMINAR TAREAS DE ESTA MATERIA
        //================================================

        for(int idTarea : idsTareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarTarea(
                    idTarea
                );
            }
        }

        //================================================
        // ELIMINAR SUBTAREAS DE ESTA MATERIA
        //================================================

        for(int idSubtarea : idsSubtareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarSubtarea(
                    idSubtarea
                );
            }
        }

        //================================================
        // GUARDAR PLANNER
        //================================================

        if(!Persistencia::guardarPlanner(
            idAlumno,
            planner,
            "planner.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR INSCRIPCIÓN
    //==================================================

    if(!Inscripciones::desinscribirAlumno(
        idMateria,
        idAlumno,
        "inscripciones.txt"))
    {
        return false;
    }

    return true;
}


//////////////////////////////////////////////////////////////
// Obtener Alumnos de una Materia
//////////////////////////////////////////////////////////////

std::string SessionManager::obtenerAlumnosMateria(
    int idMateria
) const
{
    std::lock_guard<std::mutex> lock(mutexDatos);



    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    if(datos.obtenerRol() != "Profesor")
    {
        return "ERROR|Solo los profesores pueden consultar alumnos";
    }

    if(idMateria <= 0)
    {
        return "ERROR|ID de materia invalido";
    }

    //==================================================
    // Cargar materias
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return "ERROR|No se pudieron cargar las materias";
    }

    //==================================================
    // Buscar materia
    //==================================================

    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            materiaEncontrada = true;

            if(materia.getProfesorId() !=
               datos.obtenerUsuarioId())
            {
                return "ERROR|La materia no pertenece al profesor";
            }

            break;
        }
    }

    if(!materiaEncontrada)
    {
        return "ERROR|Materia no encontrada";
    }

    //==================================================
    // Obtener alumnos
    //==================================================

    std::vector<int> alumnos =
        Inscripciones::obtenerAlumnosMateria(
            idMateria,
            "inscripciones.txt"
        );

    if(alumnos.empty())
    {
        return "ALUMNOS|";
    }

    std::string resultado = "ALUMNOS|";

    for(std::size_t i = 0;
        i < alumnos.size();
        ++i)
    {
        resultado += std::to_string(
            alumnos[i]
        );

        if(i + 1 < alumnos.size())
        {
            resultado += ";";
        }
    }

    return resultado;
}

////////////////////////////////////////////////////////////////
// Obtener Materias de un Alumno
////////////////////////////////////////////////////////////////

std::string SessionManager::obtenerMateriasAlumno() const
{

    std::lock_guard<std::mutex> lock(mutexDatos);

    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    if(datos.obtenerRol() != "Alumno")
    {
        return "ERROR|Solo los alumnos pueden consultar sus materias";
    }

    std::vector<int> materias =
        Inscripciones::obtenerMateriasAlumno(
            datos.obtenerUsuarioId(),
            "inscripciones.txt"
        );

    if(materias.empty())
    {
        return "MATERIAS_ALUMNO|";
    }

    std::string resultado =
        "MATERIAS_ALUMNO|";

    for(std::size_t i = 0;
        i < materias.size();
        ++i)
    {
        resultado += std::to_string(
            materias[i]
        );

        if(i + 1 < materias.size())
        {
            resultado += ";";
        }
    }

    return resultado;
}



////////////////////////////////////////////////////////////
// Agregar Tarea
////////////////////////////////////////////////////////////

bool SessionManager::agregarTarea(
    int idMateria,
    const std::string& titulo,
    const std::string& fechaEntrega,
    const std::string& descripcion,
    TipoTarea tipo,
    int parcial
)
{
    //==================================================
    // AUTENTICACIÓN
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // SOLO PROFESORES
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //==================================================
    // VALIDAR DATOS
    //==================================================

    if(
        idMateria <= 0 ||
        titulo.empty() ||
        fechaEntrega.empty() ||
        descripcion.empty()
    )
    {
        return false;
    }

    //==================================================
    // VALIDAR PARCIAL
    //
    // 0 = general
    // 1, 2, 3... = parcial
    //==================================================

    if(parcial < 0)
    {
        return false;
    }

    //==================================================
    // BLOQUEAR DATOS
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // CARGAR MATERIAS
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(materias,"materias.txt"))
    {
        return false;
    }

    //==================================================
    // VERIFICAR QUE LA MATERIA EXISTE
    // Y PERTENECE AL PROFESOR
    //==================================================

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    //==================================================
    // CARGAR TAREAS EXISTENTES
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas,"tareas.txt"))
    {
        return false;
    }

    
    // GENERAR ID DE TAREA
    int idTarea = Persistencia::generarIdTarea("tareas.txt");

    
    // CREAR TAREA
    Tarea nuevaTarea(
        idTarea,
        idMateria,
        titulo,
        descripcion,
        fechaEntrega,
        tipo,
        parcial
    );

    
    // AGREGAR TAREA AL VECTOR
    tareas.push_back(nuevaTarea);

    
    // GUARDAR TAREAS
    if(!Persistencia::guardarTareas(tareas,"tareas.txt"))
    {
        return false;
    }

    
    // OBTENER ALUMNOS INSCRITOS
    std::vector<int> alumnos = Inscripciones::obtenerAlumnosMateria(
        idMateria,"inscripciones.txt");

    
    // CARGAR ESTADOS DE TAREAS
    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(estados,"estadosTareas.txt"))
    {
        estados.clear();
    }

   
    // CREAR ESTADO PARA CADA ALUMNO
    for(int idAlumno : alumnos)
    {
        bool existe = false;

        for(const auto& estado : estados)
        {
            if(estado.getTareaId() == idTarea && estado.getAlumnoId() == idAlumno)
            {
                existe = true;
                break;
            }
        }

        if(!existe)
        {
            estados.emplace_back(
                idTarea,
                idAlumno,
                EstadoTarea::NO_COMPLETADO
            );
        }
    }

  
    // GUARDAR ESTADOS
    if(!Persistencia::guardarEstadosTareas(estados,"estadosTareas.txt"))
    {
        return false;
    }

   
    // CREAR NOTIFICACIONES
    std::string fechaNotificacion = obtenerFechaActual();

    
    // CARGAR NOTIFICACIONES
    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        notificaciones.clear();
    }

    
    // GENERAR ID DE NOTIFICACIÓN
    int idNotificacion = Persistencia::generarIdNotificacion("notificaciones.txt");

   
    // CREAR NOTIFICACIÓN PARA CADA ALUMNO
    for(int idAlumno : alumnos)
    {
        Notificacion nuevaNotificacion(
            idNotificacion,
            idAlumno,
            TipoNotificacion::NUEVA_TAREA,
            idTarea,
            TipoReferenciaNotificacion::TAREA,
            "Nueva tarea",
            "Se ha agregado una nueva tarea a una de tus materias.",
            fechaNotificacion
        );

        notificaciones.push_back(nuevaNotificacion);

        idNotificacion++;
    }

    
    // GUARDAR NOTIFICACIONES
    if(!Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////
// Obtener Tareas
/////////////////////////////////////////////////////////////

std::vector<Tarea> SessionManager::obtenerTareas() const
{

    std::lock_guard<std::mutex> lock(mutexDatos);

    std::vector<Tarea> tareasPermitidas;

    
    // Verificar autenticación
    if(!datos.estaAutenticado())
    {
        return tareasPermitidas;
    }

    //==================================================
    // Cargar todas las tareas
    //==================================================

    std::vector<Tarea> todasLasTareas;

    if(!Persistencia::cargarTareas(todasLasTareas,"tareas.txt"))
    {
        return tareasPermitidas;
    }

    //==================================================
    // PROFESOR
    //==================================================

    if(datos.obtenerRol() == "Profesor")
    {
        std::vector<Materia> materias;

        if(!Persistencia::cargarMaterias(materias,"materias.txt"))
        {
            return tareasPermitidas;
        }

        // Buscar las materias que pertenecen
        // al profesor conectado

        for(const auto& tarea : todasLasTareas)
        {
            for(const auto& materia : materias)
            {
                // Primero verificamos que la materia
                // pertenezca al profesor

                if(materia.getProfesorId() == datos.obtenerUsuarioId())
                {
                    // Después verificamos que la tarea
                    // pertenezca a esa materia

                    if(tarea.getMateriaId() == materia.getId())
                    {
                        tareasPermitidas.push_back(tarea);

                        break;
                    }
                }
            }
        }

        return tareasPermitidas;
    }

    //==================================================
    // ALUMNO
    //==================================================

    if(datos.obtenerRol() == "Alumno")
    {
        std::vector<int> materiasInscritas = Inscripciones::obtenerMateriasAlumno(
            datos.obtenerUsuarioId(),"inscripciones.txt");

        // Buscar tareas de las materias
        // en las que está inscrito

        for(const auto& tarea : todasLasTareas)
        {
            for(int idMateria : materiasInscritas)
            {
                if(tarea.getMateriaId() == idMateria)
                {
                    tareasPermitidas.push_back(tarea);

                    break;
                }
            }
        }

        return tareasPermitidas;
    }

    
    // Cualquier otro rol
    return tareasPermitidas;

}



////////////////////////////////////////////////////////////
// Editar Tarea
////////////////////////////////////////////////////////////

bool SessionManager::editarTarea(
    int idTarea,
    const std::string& titulo,
    const std::string& fechaEntrega,
    const std::string& descripcion,
    TipoTarea tipo,
    int parcial
)
{
    
    // AUTENTICACIÓN
    if(!datos.estaAutenticado())
    {
        return false;
    }

    
    // SOLO PROFESORES
    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }


    // VALIDAR DATOS
    if(idTarea <= 0 || titulo.empty() ||fechaEntrega.empty() || descripcion.empty())
    {
        return false;
    }

    
    // VALIDAR PARCIAL
    if(parcial < 0)
    {
        return false;
    }


    // BLOQUEAR DATOS
    std::lock_guard<std::mutex> lock(mutexDatos);

    // CARGAR MATERIAS
    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(materias,"materias.txt"))
    {
        return false;
    }

    
    // CARGAR TAREAS
    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas,"tareas.txt"))
    {
        return false;
    }

    
    // BUSCAR TAREA
    Tarea* tareaEncontrada = nullptr;

    for(auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            tareaEncontrada = &tarea;
            break;
        }
    }

    if(tareaEncontrada == nullptr)
    {
        return false;
    }

    // OBTENER MATERIA
    int idMateria = tareaEncontrada->getMateriaId();

   
    // VERIFICAR QUE LA MATERIA PERTENEZCA AL PROFESOR
    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria &&materia.getProfesorId() ==
           datos.obtenerUsuarioId())
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    // GUARDAR FECHA ANTERIOR
    std::string fechaAnterior = tareaEncontrada->getFechaEntrega();

    
    // ACTUALIZAR TAREA
    tareaEncontrada->setTitulo(titulo);
    tareaEncontrada->setFechaEntrega(fechaEntrega);
    tareaEncontrada->setDescripcion(descripcion);
    tareaEncontrada->setTipo(tipo);
    tareaEncontrada->setParcial(parcial);


    // GUARDAR TAREAS
    if(!Persistencia::guardarTareas(tareas,"tareas.txt"))
    {
        return false;
    }


    // SI NO CAMBIÓ LA FECHA
    if(fechaAnterior == fechaEntrega)
    {
        return true;
    }

    
    // ACTUALIZAR PLANNER
    if(!actualizarTareaEnPlannerArchivo(idTarea,fechaEntrega,"planner.txt"))
    {
        return false;
    }

    
    // OBTENER ALUMNOS INSCRITOS
    std::vector<int> alumnos = Inscripciones::obtenerAlumnosMateria(
            idMateria,"inscripciones.txt");


    // FECHA DE NOTIFICACIÓN
    std::string fechaNotificacion = obtenerFechaActual();

    
    // CARGAR NOTIFICACIONES
    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        notificaciones.clear();
    }


    // GENERAR ID
    int idNotificacion = Persistencia::generarIdNotificacion("notificaciones.txt");

    
    // CREAR NOTIFICACIONES
    for(int idAlumno : alumnos)
    {
        Notificacion nuevaNotificacion(
            idNotificacion,
            idAlumno,
            TipoNotificacion::CAMBIO_FECHA,
            idTarea,
            TipoReferenciaNotificacion::TAREA,
            "Cambio de fecha",
            "La fecha de entrega de una tarea ha cambiado.",
            fechaNotificacion
        );

        notificaciones.push_back(nuevaNotificacion);

        idNotificacion++;
    }

    //==================================================
    // GUARDAR NOTIFICACIONES
    //==================================================

    if(!Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        return false;
    }

    return true;
}


//////////////////////////////////////////////////////////////
// Eliminar Tarea
///////////////////////////////////////////////////////////////

bool SessionManager::eliminarTarea(
    int idTarea
)
{
    //==================================================
    // AUTENTICACIÓN
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // SOLO PROFESORES
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //==================================================
    // VALIDAR ID
    //==================================================

    if(idTarea <= 0)
    {
        return false;
    }

    //==================================================
    // BLOQUEAR DATOS COMPARTIDOS
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // CARGAR TAREAS
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // BUSCAR TAREA
    //==================================================

    int idMateria = 0;

    bool tareaEncontrada = false;

    for(const auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            idMateria =
                tarea.getMateriaId();

            tareaEncontrada = true;

            break;
        }
    }

    if(!tareaEncontrada)
    {
        return false;
    }

    //==================================================
    // CARGAR MATERIAS
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return false;
    }

    //==================================================
    // VERIFICAR QUE LA MATERIA
    // PERTENEZCA AL PROFESOR
    //==================================================

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() == idMateria &&
            materia.getProfesorId() ==
                datos.obtenerUsuarioId()
        )
        {
            materiaValida = true;

            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    //==================================================
    // OBTENER IDS DE SUBTAREAS
    //
    // Se necesitan antes de eliminarlas
    // para poder quitarlas del Planner.
    //==================================================

    std::vector<int> idsSubtareas;

    std::vector<Subtarea> subtareas;

    bool haySubtareas =
        Persistencia::cargarSubtareas(
            subtareas,
            "subtareas.txt"
        );

    if(haySubtareas)
    {
        for(const auto& subtarea : subtareas)
        {
            if(
                subtarea.getTareaId() ==
                idTarea
            )
            {
                idsSubtareas.push_back(
                    subtarea.getId()
                );
            }
        }
    }

    //==================================================
    // OBTENER ALUMNOS INSCRITOS
    //
    // Se obtienen antes de modificar inscripciones
    // para saber a qué Planners afectar.
    //==================================================

    std::vector<int> alumnos =
        Inscripciones::obtenerAlumnosMateria(
            idMateria,
            "inscripciones.txt"
        );

    //==================================================
    // ELIMINAR TAREA
    //==================================================

    tareas.erase(
        std::remove_if(
            tareas.begin(),
            tareas.end(),
            [idTarea](
                const Tarea& tarea
            )
            {
                return
                    tarea.getId() ==
                    idTarea;
            }
        ),
        tareas.end()
    );

    //==================================================
    // GUARDAR TAREAS
    //==================================================

    if(!Persistencia::guardarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // ELIMINAR ESTADOS DE LA TAREA
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(Persistencia::cargarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        estados.erase(
            std::remove_if(
                estados.begin(),
                estados.end(),
                [idTarea](
                    const EstadoTareaAlumno& estado
                )
                {
                    return
                        estado.getTareaId() ==
                        idTarea;
                }
            ),
            estados.end()
        );

        if(!Persistencia::guardarEstadosTareas(
            estados,
            "estadosTareas.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR CALIFICACIONES DE LA TAREA
    //==================================================

    std::vector<Calificacion> calificaciones;

    if(Persistencia::cargarCalificaciones(
        calificaciones,
        "calificaciones.txt"))
    {
        calificaciones.erase(
            std::remove_if(
                calificaciones.begin(),
                calificaciones.end(),
                [idTarea](
                    const Calificacion& calificacion
                )
                {
                    return
                        calificacion.getIdTarea() ==
                        idTarea;
                }
            ),
            calificaciones.end()
        );

        if(!Persistencia::guardarCalificaciones(
            calificaciones,
            "calificaciones.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR SUBTAREAS
    //==================================================

    if(haySubtareas)
    {
        subtareas.erase(
            std::remove_if(
                subtareas.begin(),
                subtareas.end(),
                [idTarea](
                    const Subtarea& subtarea
                )
                {
                    return
                        subtarea.getTareaId() ==
                        idTarea;
                }
            ),
            subtareas.end()
        );

        if(!Persistencia::guardarSubtareas(
            subtareas,
            "subtareas.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR DEL PLANNER
    //
    // Se eliminan:
    //   - la tarea
    //   - sus subtareas
    //==================================================

    if(!eliminarElementosTareaDelPlanner(
        idTarea,
        idsSubtareas,
        "planner.txt"))
    {
        return false;
    }

    //==================================================
    // ELIMINAR NOTIFICACIONES
    //
    // Solo se eliminan las notificaciones que
    // hacen referencia a ESTA tarea.
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        notificaciones.erase(
            std::remove_if(
                notificaciones.begin(),
                notificaciones.end(),
                [idTarea](
                    const Notificacion& notificacion
                )
                {
                    return
                        notificacion.getIdReferencia() ==
                            idTarea
                        &&
                        notificacion.getTipoReferencia() ==
                            TipoReferenciaNotificacion::TAREA;
                }
            ),
            notificaciones.end()
        );

        if(!Persistencia::guardarNotificaciones(
            notificaciones,
            "notificaciones.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR TAREA
    // DE INSCRIPCIONES
    //
    // NO se elimina la inscripción porque
    // solamente estamos eliminando una tarea.
    // La inscripción pertenece a la materia.
    //==================================================

    return true;
}


////////////////////////////////////////////////////////////
// Configurar Ponderacion
////////////////////////////////////////////////////////////

bool SessionManager::configurarPonderacion(
    int idMateria,
    int parcial,
    double tarea,
    double examen,
    double practica,
    double proyecto,
    double trabajo,
    double otro
)
{
    //==================================================
    // AUTENTICACION
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }


    //==================================================
    // SOLO PROFESORES
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }


    //==================================================
    // VALIDAR ID DE MATERIA
    //==================================================

    if(idMateria <= 0)
    {
        return false;
    }


    //==================================================
    // VALIDAR PARCIAL
    //
    // parcial = 0 significa configuración general.
    //
    // parcial >= 1 representa un parcial específico.
    //==================================================

    if(parcial < 0)
    {
        return false;
    }


    //==================================================
    // VALIDAR PORCENTAJES
    //==================================================

    if(
        tarea < 0 ||
        examen < 0 ||
        practica < 0 ||
        proyecto < 0 ||
        trabajo < 0 ||
        otro < 0
    )
    {
        return false;
    }


    //==================================================
    // VALIDAR SUMA DE PONDERACIONES
    //==================================================

    double total =
        tarea +
        examen +
        practica +
        proyecto +
        trabajo +
        otro;


    // Usamos un pequeño margen por trabajar con double.
    if(total < 99.99 || total > 100.01)
    {
        return false;
    }


    // BLOQUEAR DATOS
    std::lock_guard<std::mutex> lock(mutexDatos);


    // CARGAR MATERIAS
    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(materias, "materias.txt"))
    {
        return false;
    }


    // VERIFICAR QUE LA MATERIA EXISTA Y PERTENEZCA AL PROFESOR
    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            materiaValida = true;

            break;
        }
    }


    if(!materiaValida)
    {
        return false;
    }


    // CARGAR PONDERACIONES EXISTENTES
    std::vector<Ponderacion> ponderaciones;

    if(!Persistencia::cargarPonderaciones(ponderaciones,"ponderaciones.txt"))
    {
        //==================================================
        // Si el archivo todavía no existe,
        // comenzamos con un vector vacío.
        //==================================================

        ponderaciones.clear();
    }


    //==================================================
    // BUSCAR PONDERACION
    //
    // Una ponderación es única por:
    //
    // idMateria + parcial
    //==================================================

    Ponderacion* ponderacionEncontrada = nullptr;

    for(auto& ponderacion : ponderaciones)
    {
        if(ponderacion.getIdMateria() == idMateria &&
            ponderacion.getParcial() == parcial)
        {
            ponderacionEncontrada = &ponderacion;

            break;
        }
    }


    // ACTUALIZAR PONDERACION EXISTENTE
    if(ponderacionEncontrada != nullptr)
    {
        ponderacionEncontrada->setTarea(tarea);
        ponderacionEncontrada->setExamen(examen);
        ponderacionEncontrada->setPractica(practica);
        ponderacionEncontrada->setProyecto(proyecto);
        ponderacionEncontrada->setTrabajo(trabajo);
        ponderacionEncontrada->setOtro(otro);
    }


    
    // CREAR NUEVA PONDERACION
    else
    {
        Ponderacion nuevaPonderacion(
            idMateria,
            parcial,
            tarea,
            examen,
            practica,
            proyecto,
            trabajo,
            otro
        );

        ponderaciones.push_back(nuevaPonderacion);

    }


    // GUARDAR PONDERACIONES
    if(!Persistencia::guardarPonderaciones(ponderaciones,"ponderaciones.txt"))
    {
        return false;
    }


    return true;
}


////////////////////////////////////////////////////////////
// Obtener Ponderaciones de Materia
////////////////////////////////////////////////////////////

bool SessionManager::obtenerPonderacionesMateria(
    int idMateria,
    std::vector<Ponderacion>& ponderacionesMateria
)
{

    // AUTENTICACION
     if(!datos.estaAutenticado())
    {
        return false;
    }


    // VALIDAR ID
    if(idMateria <= 0)
    {
        return false;
    }


    // BLOQUEAR DATOS
    std::lock_guard<std::mutex> lock(mutexDatos);


    // CARGAR TODAS LAS PONDERACIONES
    std::vector<Ponderacion> todas;

    if(!Persistencia::cargarPonderaciones(todas,"ponderaciones.txt"))
    {
        return false;
    }


    // LIMPIAR VECTOR DE RESULTADO
    ponderacionesMateria.clear();


    // FILTRAR POR MATERIA
    for(const auto& ponderacion : todas)
    {
        if(
            ponderacion.getIdMateria() == idMateria
        )
        {
            ponderacionesMateria.push_back(ponderacion);
        }
    }


    return true;
}


bool SessionManager::eliminarPonderacion(
    int idMateria,
    int parcial
)
{
    if(!estaAutenticado())
    {
        return false;
    }

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    if(idMateria <= 0 || parcial < 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    ///////////////////////////////////////////////////////////
    // Cargar materias
    ///////////////////////////////////////////////////////////

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"
    ))
    {
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Verificar que la materia pertenezca al profesor
    ///////////////////////////////////////////////////////////

    int idProfesor = datos.obtenerUsuarioId();

    bool materiaValida = false;

    for(const Materia& materia : materias)
    {
        if(
            materia.getId() == idMateria &&
            materia.getProfesorId() == idProfesor
        )
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Cargar ponderaciones
    ///////////////////////////////////////////////////////////

    std::vector<Ponderacion> ponderaciones;

    if(!Persistencia::cargarPonderaciones(
        ponderaciones,
        "ponderaciones.txt"
    ))
    {
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Buscar ponderación
    ///////////////////////////////////////////////////////////

    auto it = std::find_if(
        ponderaciones.begin(),
        ponderaciones.end(),
        [idMateria, parcial](const Ponderacion& ponderacion)
        {
            return
                ponderacion.getIdMateria() == idMateria &&
                ponderacion.getParcial() == parcial;
        }
    );

    if(it == ponderaciones.end())
    {
        return false;
    }

    ///////////////////////////////////////////////////////////
    // Eliminar ponderación
    ///////////////////////////////////////////////////////////

    ponderaciones.erase(it);

    ///////////////////////////////////////////////////////////
    // Guardar cambios
    ///////////////////////////////////////////////////////////

    if(!Persistencia::guardarPonderaciones(
        ponderaciones,
        "ponderaciones.txt"
    ))
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////
// Agregar Subtarea
////////////////////////////////////////////////////////////

bool SessionManager::agregarSubtarea(
    int idTarea,
    const std::string& descripcion
)
{
    //==================================================
    // Debe estar autenticado
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    //==================================================
    // Validar datos
    //==================================================

    if(idTarea <= 0)
    {
        return false;
    }

    if(descripcion.empty())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar tareas
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas, "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // Buscar tarea
    //==================================================

    int idMateria = 0;

    bool tareaEncontrada = false;

    for(const auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            idMateria = tarea.getMateriaId();

            tareaEncontrada = true;

            break;
        }
    }

    if(!tareaEncontrada)
    {
        return false;
    }

    //==================================================
    // Verificar que el alumno esté inscrito
    // en la materia de la tarea
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    if(!Inscripciones::estaInscrito(idMateria,idAlumno,"inscripciones.txt"))
    {
        return false;
    }

    //==================================================
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(subtareas,"subtareas.txt"))
    {
        return false;
    }

    //==================================================
    // Generar ID
    //==================================================

    int idSubtarea = Persistencia::generarIdSubtarea("subtareas.txt");

    //==================================================
    // Crear subtarea
    //==================================================

    Subtarea nuevaSubtarea(idSubtarea,idTarea,idAlumno,descripcion);

    //==================================================
    // Estado inicial
    //==================================================

    nuevaSubtarea.setEstado(EstadoSubtarea::PENDIENTE);
    subtareas.push_back(nuevaSubtarea);

    //==================================================
    // Guardar
    //==================================================

    return Persistencia::guardarSubtareas(subtareas,"subtareas.txt");
}

////////////////////////////////////////////////////////////
// Obtener Subtareas
////////////////////////////////////////////////////////////

std::vector<Subtarea> SessionManager::obtenerSubtareas() const
{
    std::vector<Subtarea> resultado;

    //==================================================
    // Debe estar autenticado
    //==================================================

    if(!datos.estaAutenticado())
    {
        return resultado;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return resultado;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(subtareas, "subtareas.txt"))
    {
        return resultado;
    }

    //==================================================
    // ID del alumno actual
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    //==================================================
    // Filtrar subtareas propias
    //==================================================

    for(const auto& subtarea : subtareas)
    {
        if(subtarea.getAlumnoId() == idAlumno)
        {
            resultado.push_back(subtarea);
        }
    }

    return resultado;
}

////////////////////////////////////////////////////////////
// Editar Subtarea
////////////////////////////////////////////////////////////

bool SessionManager::editarSubtarea(
    int idSubtarea,
    const std::string& descripcion
)
{
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    //==================================================
    // Validar
    //==================================================

    if(idSubtarea <= 0 || descripcion.empty())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(subtareas,"subtareas.txt"))
    {
        return false;
    }

    //==================================================
    // Buscar subtarea
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    bool encontrada = false;

    for(auto& subtarea : subtareas)
    {
        if(
            subtarea.getId() == idSubtarea &&
            subtarea.getAlumnoId() == idAlumno
        )
        {
            subtarea.setDescripcion(descripcion);

            encontrada = true;

            break;
        }
    }

    if(!encontrada)
    {
        return false;
    }

    //==================================================
    // Guardar
    //==================================================

    return Persistencia::guardarSubtareas(subtareas, "subtareas.txt");

}

////////////////////////////////////////////////////////////
// Eliminar Subtarea
////////////////////////////////////////////////////////////

bool SessionManager::eliminarSubtarea(
    int idSubtarea
)
{
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    //==================================================
    // Validar ID
    //==================================================

    if(idSubtarea <= 0)
    {
        return false;
    }

    //==================================================
    // ID del alumno actual
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(
        subtareas,
        "subtareas.txt"))
    {
        return false;
    }

    //==================================================
    // Buscar y eliminar subtarea
    //==================================================

    bool encontrada = false;

    for(auto it = subtareas.begin();
        it != subtareas.end();
        ++it)
    {
        if(
            it->getId() == idSubtarea &&
            it->getAlumnoId() == idAlumno
        )
        {
            subtareas.erase(it);

            encontrada = true;

            break;
        }
    }

    //==================================================
    // La subtarea no pertenece al alumno
    //==================================================

    if(!encontrada)
    {
        return false;
    }

    //==================================================
    // Guardar subtareas
    //==================================================

    if(!Persistencia::guardarSubtareas(
        subtareas,
        "subtareas.txt"))
    {
        return false;
    }

    //==================================================
    // Eliminar la subtarea del Planner
    //==================================================

    std::ifstream entrada("planner.txt");

    if(!entrada.is_open())
    {
        // No hay Planner que limpiar
        return true;
    }

    std::vector<std::string> registros;

    std::string linea;

    //==================================================
    // Saltar encabezado
    //==================================================

    std::getline(entrada, linea);

    //==================================================
    // Leer registros
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string alumnoTexto;
        std::string fecha;
        std::string tipo;
        std::string idElementoTexto;
        std::string prioridad;

        std::getline(
            ss,
            alumnoTexto,
            '|'
        );

        std::getline(
            ss,
            fecha,
            '|'
        );

        std::getline(
            ss,
            tipo,
            '|'
        );

        std::getline(
            ss,
            idElementoTexto,
            '|'
        );

        std::getline(
            ss,
            prioridad,
            '|'
        );

        //==================================================
        // Intentar obtener alumno
        //==================================================

        int idAlumnoArchivo;

        try
        {
            idAlumnoArchivo =
                std::stoi(alumnoTexto);
        }
        catch(...)
        {
            // Registro inválido:
            // se conserva para no perder información
            registros.push_back(linea);
            continue;
        }

        //==================================================
        // Intentar obtener elemento
        //==================================================

        int idElemento;

        try
        {
            idElemento =
                std::stoi(idElementoTexto);
        }
        catch(...)
        {
            registros.push_back(linea);
            continue;
        }

        //==================================================
        // Determinar si se debe eliminar
        //==================================================

        bool eliminar = false;

        if(
            idAlumnoArchivo == idAlumno &&
            tipo == "SUBTAREA" &&
            idElemento == idSubtarea
        )
        {
            eliminar = true;
        }

        //==================================================
        // Conservar todos los demás registros
        //==================================================

        if(!eliminar)
        {
            registros.push_back(linea);
        }
    }

    entrada.close();

    //==================================================
    // Reescribir Planner
    //==================================================

    std::ofstream salida("planner.txt");

    if(!salida.is_open())
    {
        return false;
    }

    //==================================================
    // Encabezado
    //==================================================

    salida
        << "idAlumno|"
        << "fecha|"
        << "tipo|"
        << "idElemento|"
        << "prioridad"
        << "\n";

    //==================================================
    // Restaurar registros
    //==================================================

    for(const std::string& registro : registros)
    {
        salida << registro << "\n";
    }

    salida.close();

    return true;
}

////////////////////////////////////////////////////////////
// Cambiar Estado de Subtarea
////////////////////////////////////////////////////////////

bool SessionManager::cambiarEstadoSubtarea(
    int idSubtarea,
    EstadoSubtarea estado
)
{
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    //==================================================
    // Validar ID
    //==================================================

    if(idSubtarea <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(subtareas,"subtareas.txt"))
    {
        return false;
    }

    //==================================================
    // Alumno actual
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    //==================================================
    // Buscar subtarea
    //==================================================

    bool encontrada = false;

    for(auto& subtarea : subtareas)
    {
        if(subtarea.getId() == idSubtarea && subtarea.getAlumnoId() == idAlumno)
        {
            subtarea.setEstado(estado);

            encontrada = true;

            break;
        }
    }

    if(!encontrada)
    {
        return false;
    }

    //==================================================
    // Guardar
    //==================================================

    return Persistencia::guardarSubtareas(subtareas,"subtareas.txt");

}



///////////////////////////////////////////////////////////
// Obtener Planner
///////////////////////////////////////////////////////////

PlannerSemana& SessionManager::obtenerPlanner()
{

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Si no está autenticado
    //==================================================

    if(!datos.estaAutenticado())
    {
        return datos.obtenerPlanner();
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return datos.obtenerPlanner();
    }

    //==================================================
    // Crear un Planner NUEVO
    //
    // Esto evita conservar información de un GET_PLANNER
    // anterior.
    //==================================================

    PlannerSemana planner;

    //==================================================
    // Obtener fecha actual
    //==================================================

    std::string hoy = obtenerFechaActual();

    std::string lunes =
        obtenerLunesSemana(hoy);

    if(lunes.empty())
    {
        return datos.obtenerPlanner();
    }

    //==================================================
    // Configurar semana
    //==================================================

    planner.setFechaInicio(lunes);

    std::tm fechaLunes{};

    if(!convertirFecha(lunes, fechaLunes))
    {
        return datos.obtenerPlanner();
    }

    std::mktime(&fechaLunes);

    //==================================================
    // Crear los 7 días
    //==================================================

    for(int i = 0; i < 7; ++i)
    {
        std::tm fechaDia = fechaLunes;

        fechaDia.tm_mday += i;

        std::mktime(&fechaDia);

        std::ostringstream salida;

        salida
            << std::setfill('0')
            << std::setw(4)
            << fechaDia.tm_year + 1900
            << "-"
            << std::setw(2)
            << fechaDia.tm_mon + 1
            << "-"
            << std::setw(2)
            << fechaDia.tm_mday;

        planner
            .getDia(i)
            .setFecha(salida.str());
    }

    //==================================================
    // Cargar decisiones personales
    //
    // Esto carga:
    // - prioridades de tareas
    // - ubicación de subtareas
    //
    // NO carga tareas automáticamente.
    //==================================================

    Persistencia::cargarPlanner(
        datos.obtenerUsuarioId(),
        planner,
        "planner.txt"
    );

    //==================================================
    // Obtener tareas disponibles para el alumno
    //==================================================
    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas,"tareas.txt"))
    {
        return datos.obtenerPlanner();
    }

    //==================================================
    // Agregar tareas automáticamente según
    // fecha de entrega
    //==================================================

    for(const Tarea& tarea : tareas)
    {
        const std::string& fechaEntrega =
            tarea.getFechaEntrega();

        // Buscar el día correspondiente
        for(int i = 0; i < 7; ++i)
        {
            PlannerDia& dia =
                planner.getDia(i);

            if(dia.getFecha() != fechaEntrega)
            {
                continue;
            }

            //================================================
            // Verificar si ya está cargada desde planner.txt
            //================================================

            bool existe = false;

            for(const TareaPlanner& tareaPlanner :
                dia.getTareas())
            {
                if(tareaPlanner.idTarea ==
                   tarea.getId())
                {
                    existe = true;
                    break;
                }
            }

            //================================================
            // Si no tiene decisión personal,
            // usar MEDIA como prioridad por defecto.
            //================================================

            if(!existe)
            {
                dia.agregarTarea(
                    tarea.getId(),
                    PrioridadPlanner::MEDIA
                );
            }

            break;
        }
    }

    //==================================================
    // Guardar el Planner reconstruido en SessionData
    //==================================================

    datos.obtenerPlanner() = planner;

    return datos.obtenerPlanner();
}



////////////////////////////////////////////////////////////
// Cambiar Prioridad de Tarea en Planner
////////////////////////////////////////////////////////////

bool SessionManager::cambiarPrioridadPlanner(
    int idTarea,
    PrioridadPlanner prioridad
)
{
    // =============================================
    // Debe estar autenticado
    // =============================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    // =============================================
    // Solo alumnos
    // =============================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    // =============================================
    // Validar ID
    // =============================================

    if(idTarea <= 0)
    {
        return false;
    }

    // =============================================
    // Obtener ID del alumno
    // =============================================

    int idAlumno =
        datos.obtenerUsuarioId();

    // =============================================
    // Bloquear datos
    // =============================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    // =============================================
    // Convertir prioridad a texto
    // =============================================

    std::string prioridadTexto;

    switch(prioridad)
    {
        case PrioridadPlanner::ALTA:
            prioridadTexto = "ALTA";
            break;

        case PrioridadPlanner::MEDIA:
            prioridadTexto = "MEDIA";
            break;

        case PrioridadPlanner::BAJA:
            prioridadTexto = "BAJA";
            break;

        default:
            return false;
    }

    // =============================================
    // Cargar tareas
    // =============================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    // =============================================
    // Buscar tarea
    // =============================================

    Tarea* tareaEncontrada = nullptr;

    for(auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            tareaEncontrada = &tarea;
            break;
        }
    }

    if(tareaEncontrada == nullptr)
    {
        return false;
    }

    // =============================================
    // Verificar que el alumno esté inscrito
    // en la materia de la tarea
    // =============================================

    std::vector<int> materiasInscritas =
        Inscripciones::obtenerMateriasAlumno(
            idAlumno,
            "inscripciones.txt"
        );

    bool materiaInscrita = false;

    for(int idMateria : materiasInscritas)
    {
        if(
            idMateria ==
            tareaEncontrada->getMateriaId()
        )
        {
            materiaInscrita = true;
            break;
        }
    }

    if(!materiaInscrita)
    {
        return false;
    }

    // =============================================
    // Cargar planner.txt
    // =============================================

    std::ifstream entrada(
        "planner.txt"
    );

    std::vector<std::string> registros;

    if(entrada.is_open())
    {
        std::string linea;

        // =========================================
        // Saltar encabezado
        // =========================================

        std::getline(
            entrada,
            linea
        );

        // =========================================
        // Leer registros
        // =========================================

        while(std::getline(
            entrada,
            linea))
        {
            if(!linea.empty())
            {
                registros.push_back(
                    linea
                );
            }
        }

        entrada.close();
    }

    // =============================================
    // Buscar si la tarea ya está en Planner
    // =============================================

    bool tareaEnPlanner = false;

    for(std::string& registro : registros)
    {
        std::stringstream ss(
            registro
        );

        std::string alumnoTexto;
        std::string fecha;
        std::string tipo;
        std::string idElementoTexto;
        std::string prioridadAnterior;

        std::getline(
            ss,
            alumnoTexto,
            '|'
        );

        std::getline(
            ss,
            fecha,
            '|'
        );

        std::getline(
            ss,
            tipo,
            '|'
        );

        std::getline(
            ss,
            idElementoTexto,
            '|'
        );

        std::getline(
            ss,
            prioridadAnterior,
            '|'
        );

        int idAlumnoRegistro;
        int idElemento;

        try
        {
            idAlumnoRegistro =
                std::stoi(alumnoTexto);

            idElemento =
                std::stoi(idElementoTexto);
        }
        catch(...)
        {
            continue;
        }

        // =========================================
        // La tarea ya existe en el Planner
        // =========================================

        if(
            idAlumnoRegistro == idAlumno &&
            tipo == "TAREA" &&
            idElemento == idTarea
        )
        {
            registro =
                alumnoTexto +
                "|" +
                fecha +
                "|" +
                tipo +
                "|" +
                idElementoTexto +
                "|" +
                prioridadTexto;

            tareaEnPlanner = true;

            break;
        }
    }

    // =============================================
    // Si la tarea NO estaba en Planner,
    // agregarla usando su fecha de entrega
    // =============================================

    if(!tareaEnPlanner)
    {
        registros.push_back(
            std::to_string(idAlumno) +
            "|" +
            tareaEncontrada->getFechaEntrega() +
            "|TAREA|" +
            std::to_string(idTarea) +
            "|" +
            prioridadTexto
        );
    }

    // =============================================
    // Guardar planner.txt
    // =============================================

    std::ofstream salida(
        "planner.txt"
    );

    if(!salida.is_open())
    {
        return false;
    }

    // =============================================
    // Encabezado
    // =============================================

    salida
        << "idAlumno|"
        << "fecha|"
        << "tipo|"
        << "idElemento|"
        << "prioridad"
        << "\n";

    // =============================================
    // Guardar registros
    // =============================================

    for(const std::string& registro :
        registros)
    {
        salida
            << registro
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// Agregar subtarea al Planner
///////////////////////////////////////////////////////////

bool SessionManager::agregarSubtareaFecha(
    int idSubtarea,
    const std::string& fecha
)
{
    // =============================================
    // Debe estar autenticado
    // =============================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    // =============================================
    // Solo alumnos
    // =============================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    // =============================================
    // Validar datos
    // =============================================

    if(idSubtarea <= 0)
    {
        return false;
    }

    if(fecha.empty())
    {
        return false;
    }

    // =============================================
    // Obtener ID del alumno
    // =============================================

    int idAlumno =
        datos.obtenerUsuarioId();

    // =============================================
    // Bloquear datos
    // =============================================

    std::lock_guard<std::mutex> lock(mutexDatos);

    // =============================================
    // Cargar subtareas
    // =============================================

    std::vector<Subtarea> subtareas;

    if(!Persistencia::cargarSubtareas(
        subtareas,
        "subtareas.txt"))
    {
        return false;
    }

    // =============================================
    // Verificar que la subtarea pertenece
    // al alumno actual
    // =============================================

    bool subtareaEncontrada = false;

    for(const auto& subtarea : subtareas)
    {
        if(
            subtarea.getId() == idSubtarea &&
            subtarea.getAlumnoId() == idAlumno
        )
        {
            subtareaEncontrada = true;
            break;
        }
    }

    if(!subtareaEncontrada)
    {
        return false;
    }

    // =============================================
    // Cargar Planner
    //
    // NO usamos obtenerPlanner()
    // porque esa función también bloquea mutexDatos.
    // =============================================

    std::ifstream entrada(
        "planner.txt"
    );

    // =============================================
    // Si el archivo no existe,
    // crear uno nuevo
    // =============================================

    std::vector<std::string> registros;

    if(entrada.is_open())
    {
        std::string linea;

        // =========================================
        // Saltar encabezado
        // =========================================

        std::getline(
            entrada,
            linea
        );

        // =========================================
        // Leer registros
        // =========================================

        while(std::getline(
            entrada,
            linea))
        {
            if(!linea.empty())
            {
                registros.push_back(
                    linea
                );
            }
        }

        entrada.close();
    }

    // =============================================
    // Verificar si la subtarea ya está
    // asignada al alumno
    // =============================================

    for(const std::string& registro :
        registros)
    {
        std::stringstream ss(
            registro
        );

        std::string alumnoTexto;
        std::string fechaRegistro;
        std::string tipo;
        std::string idElementoTexto;
        std::string prioridad;

        std::getline(
            ss,
            alumnoTexto,
            '|'
        );

        std::getline(
            ss,
            fechaRegistro,
            '|'
        );

        std::getline(
            ss,
            tipo,
            '|'
        );

        std::getline(
            ss,
            idElementoTexto,
            '|'
        );

        std::getline(
            ss,
            prioridad,
            '|'
        );

        int idAlumnoRegistro;
        int idElemento;

        try
        {
            idAlumnoRegistro =
                std::stoi(alumnoTexto);

            idElemento =
                std::stoi(idElementoTexto);
        }
        catch(...)
        {
            continue;
        }

        // =========================================
        // La subtarea ya está en esa misma fecha
        // =========================================

        if(
            idAlumnoRegistro == idAlumno &&
            fechaRegistro == fecha &&
            tipo == "SUBTAREA" &&
            idElemento == idSubtarea
        )
        {
            return true;
        }
    }

    // =============================================
    // Eliminar la subtarea de cualquier
    // otra fecha del alumno
    // =============================================

    std::vector<std::string> nuevosRegistros;

    for(const std::string& registro :
        registros)
    {
        std::stringstream ss(
            registro
        );

        std::string alumnoTexto;
        std::string fechaRegistro;
        std::string tipo;
        std::string idElementoTexto;
        std::string prioridad;

        std::getline(
            ss,
            alumnoTexto,
            '|'
        );

        std::getline(
            ss,
            fechaRegistro,
            '|'
        );

        std::getline(
            ss,
            tipo,
            '|'
        );

        std::getline(
            ss,
            idElementoTexto,
            '|'
        );

        std::getline(
            ss,
            prioridad,
            '|'
        );

        int idAlumnoRegistro;
        int idElemento;

        try
        {
            idAlumnoRegistro =
                std::stoi(alumnoTexto);

            idElemento =
                std::stoi(idElementoTexto);
        }
        catch(...)
        {
            // Registro inválido:
            // conservarlo
            nuevosRegistros.push_back(
                registro
            );

            continue;
        }

        // =========================================
        // Si es la misma subtarea del alumno,
        // se elimina de su fecha anterior.
        // =========================================

        if(
            idAlumnoRegistro == idAlumno &&
            tipo == "SUBTAREA" &&
            idElemento == idSubtarea
        )
        {
            continue;
        }

        nuevosRegistros.push_back(
            registro
        );
    }

    // =============================================
    // Agregar subtarea en la nueva fecha
    //
    // Prioridad por defecto:
    // MEDIA
    // =============================================

    nuevosRegistros.push_back(
        std::to_string(idAlumno) +
        "|" +
        fecha +
        "|SUBTAREA|" +
        std::to_string(idSubtarea) +
        "|MEDIA"
    );

    // =============================================
    // Guardar Planner
    // =============================================

    std::ofstream salida(
        "planner.txt"
    );

    if(!salida.is_open())
    {
        return false;
    }

    // =============================================
    // Encabezado
    // =============================================

    salida
        << "idAlumno|"
        << "fecha|"
        << "tipo|"
        << "idElemento|"
        << "prioridad"
        << "\n";

    // =============================================
    // Guardar registros
    // =============================================

    for(const std::string& registro :
        nuevosRegistros)
    {
        salida
            << registro
            << "\n";
    }

    salida.close();

    return true;
}

////////////////////////////////////////////////////////////
// Eliminar Subtarea del Planner
////////////////////////////////////////////////////////////


bool SessionManager::eliminarSubtareaPlanner(
    int idSubtarea,
    const std::string& fecha
)
{
    // =============================================
    // Debe estar autenticado
    // =============================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    // =============================================
    // Solo alumnos
    // =============================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    // =============================================
    // Validar
    // =============================================

    if(idSubtarea <= 0)
    {
        return false;
    }

    if(fecha.empty())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // =============================================
    // Obtener Planner
    // =============================================

    PlannerSemana& planner =
        datos.obtenerPlanner();

    // =============================================
    // Buscar día
    // =============================================

    int indiceDia =
        obtenerIndiceDia(
            planner,
            fecha
        );

    if(indiceDia < 0)
    {
        return false;
    }

    PlannerDia& dia = planner.getDia(indiceDia);

    // =============================================
    // Comprobar que existe
    // =============================================

    bool encontrada = false;

    for(int id : dia.getSubtareas())
    {
        if(id == idSubtarea)
        {
            encontrada = true;
            break;
        }
    }

    if(!encontrada)
    {
        return false;
    }

    // =============================================
    // Eliminar del día
    // =============================================

    dia.eliminarSubtarea(idSubtarea);

    // =============================================
    // Guardar Planner
    // =============================================

    return Persistencia::guardarPlanner(
        datos.obtenerUsuarioId(),
        planner,
        "planner.txt"
    );
}


////////////////////////////////////////////////////////////
// Cambiar Estado de Tarea
////////////////////////////////////////////////////////////

bool SessionManager::cambiarEstadoTarea(
    int idTarea,
    EstadoTarea estado
)
{
    //==================================================
    // Debe estar autenticado
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return false;
    }

    //==================================================
    // Validar ID
    //==================================================

    if(idTarea <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar estados
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(estados,"estadosTareas.txt"))
    {
        return false;
    }

    //==================================================
    // Alumno actual
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    //==================================================
    // Buscar estado del alumno
    //==================================================

    for(auto& registro : estados)
    {
        if(
            registro.getTareaId() == idTarea &&
            registro.getAlumnoId() == idAlumno
        )
        {
            registro.setEstado(
                estado
            );

            return Persistencia::guardarEstadosTareas(
                estados,
                "estadosTareas.txt"
            );
        }
    }

    //==================================================
    // No existe relación tarea-alumno
    //==================================================

    return false;
}

////////////////////////////////////////////////////////////
// Obtener Estados de una Tarea
////////////////////////////////////////////////////////////

std::vector<EstadoTareaAlumno>
SessionManager::obtenerEstadosTarea(
    int idTarea
) const
{
    std::vector<EstadoTareaAlumno> resultado;

    if(!datos.estaAutenticado())
    {
        return resultado;
    }

    //==================================================
    // Solo profesores
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return resultado;
    }

    if(idTarea <= 0)
    {
        return resultado;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // Cargar tarea
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(tareas,"tareas.txt"))
    {
        return resultado;
    }

    int idMateria = 0;

    bool tareaEncontrada = false;

    for(const auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            idMateria = tarea.getMateriaId();

            tareaEncontrada = true;
            break;
        }
    }

    if(!tareaEncontrada)
    {
        return resultado;
    }

    //==================================================
    // Verificar que la materia pertenece al profesor
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(materias,"materias.txt"))
    {
        return resultado;
    }

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() == idMateria &&
            materia.getProfesorId() == datos.obtenerUsuarioId()
        )
        {
            materiaValida = true;
            break;
        }
    }

    if(!materiaValida)
    {
        return resultado;
    }

    //==================================================
    // Cargar estados
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(estados,"estadosTareas.txt"))
    {
        return resultado;
    }

    //==================================================
    // Filtrar estados de esta tarea
    //==================================================

    for(const auto& estado : estados)
    {
        if(estado.getTareaId() == idTarea)
        {
            resultado.push_back(estado);
        }
    }

    return resultado;
}

////////////////////////////////////////////////////////////
// Obtener Estados de Tareas del Alumno
////////////////////////////////////////////////////////////

std::vector<EstadoTareaAlumno>
SessionManager::obtenerEstadosAlumno(
    EstadoTarea estado
) const
{
    std::vector<EstadoTareaAlumno> resultado;

    //==================================================
    // Verificar autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return resultado;
    }

    //==================================================
    // Solo alumnos
    //==================================================

    if(datos.obtenerRol() != "Alumno")
    {
        return resultado;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    //==================================================
    // ID del alumno actual
    //==================================================

    int idAlumno = datos.obtenerUsuarioId();

    //==================================================
    // Cargar estados
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(estados, "estadosTareas.txt"))
    {
        return resultado;
    }

    //==================================================
    // Filtrar estados del alumno actual
    // y por el estado solicitado
    //==================================================

    for(const auto& estadoTarea : estados)
    {
        if(estadoTarea.getAlumnoId() == idAlumno && estadoTarea.getEstado() == estado)
        {
            resultado.push_back(estadoTarea);
        }
    }

    return resultado;
}


////////////////////////////////////////////////////////////
// Agregar Notificación
////////////////////////////////////////////////////////////

bool SessionManager::agregarNotificacion(
    int idUsuario,
    TipoNotificacion tipo,
    int idReferencia,
    TipoReferenciaNotificacion tipoReferencia,
    const std::string& titulo,
    const std::string& mensaje,
    const std::string& fecha
)
{
    
    // Autenticación
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Validar usuario
    if(idUsuario <= 0)
    {
        return false;
    }

    // Validar referencia    
    if(
        idReferencia < 0
    )
    {
        return false;
    }

    
    // Validar datos
    if(titulo.empty() || mensaje.empty() || fecha.empty())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);


    // Cargar notificaciones
    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        notificaciones.clear();
    }

    // Generar ID
    int idNotificacion = Persistencia::generarIdNotificacion("notificaciones.txt");

    
    // Crear notificación
    Notificacion nuevaNotificacion(
        idNotificacion,
        idUsuario,
        tipo,
        idReferencia,
        tipoReferencia,
        titulo,
        mensaje,
        fecha
    );

        
    // Agregar
    notificaciones.push_back(nuevaNotificacion);

    // Guardar
    return Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt");
}

////////////////////////////////////////////////////////////
// Obtener Notificaciones
////////////////////////////////////////////////////////////

std::vector<Notificacion> SessionManager::obtenerNotificaciones() const
{
    std::lock_guard<std::mutex> lock(mutexDatos);

    std::vector<Notificacion> resultado;

    //==================================================
    // Verificar autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return resultado;
    }

    //==================================================
    // Cargar todas
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        return resultado;
    }

    //==================================================
    // Usuario actual
    //==================================================

    int idUsuario =
        datos.obtenerUsuarioId();

    //==================================================
    // Filtrar únicamente las propias
    //==================================================

    for(const auto& notificacion : notificaciones)
    {
        if(
            notificacion.getUsuarioId() ==
            idUsuario
        )
        {
            resultado.push_back(
                notificacion
            );
        }
    }

    return resultado;
}


////////////////////////////////////////////////////////////
// Marcar Notificación como Leída
////////////////////////////////////////////////////////////

bool SessionManager::marcarNotificacionLeida(
    int idNotificacion
)
{

    // Autenticación
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Validar ID
    if(idNotificacion <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // Cargar notificaciones
    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        return false;
    }

    // Usuario conectado
    int idUsuario = datos.obtenerUsuarioId();

    // Buscar únicamente una propia
    bool encontrada = false;

    for(auto& notificacion : notificaciones)
    {
        if(notificacion.getId() == idNotificacion && 
           notificacion.getUsuarioId() == idUsuario)
        {
            notificacion.marcarComoLeida();

            encontrada = true;

            break;
        }
    }

    // No encontrada o no pertenece
    if(!encontrada)
    {
        return false;
    }

    // Guardar cambios
    return Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt");
}

////////////////////////////////////////////////////////////
// Eliminar Notificación
////////////////////////////////////////////////////////////

bool SessionManager::eliminarNotificacion(
    int idNotificacion
)
{

    // Autenticación
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Validar ID
    if(idNotificacion <= 0)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mutexDatos);

    // Cargar notificaciones
    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(notificaciones,"notificaciones.txt"))
    {
        return false;
    }

    // Usuario actual
    int idUsuario = datos.obtenerUsuarioId();

    // Buscar y eliminar
    bool eliminada = false;

    for(auto it = notificaciones.begin(); it != notificaciones.end(); ++it)
    {
        if(it->getId() == idNotificacion && it->getUsuarioId() == idUsuario)
        {
            notificaciones.erase(it);

            eliminada = true;

            break;
        }
    }

    // No encontrada
    if(!eliminada)
    {
        return false;
    }

    // Guardar
    return Persistencia::guardarNotificaciones(notificaciones,"notificaciones.txt");
}

////////////////////////////////////////////////////////////
// Generar recordatorios
////////////////////////////////////////////////////////////

void SessionManager::generarRecordatorios()
{
    //==================================================
    // BLOQUEAR DATOS
    //
    // Este método es ejecutado por el hilo de
    // notificaciones mientras los clientes pueden
    // modificar los archivos.
    //==================================================

    std::lock_guard<std::mutex> lock(mutexDatos);


    //==================================================
    // OBTENER FECHA ACTUAL
    //==================================================

    std::string hoy =
        obtenerFechaActual();

    if(hoy.empty())
    {
        return;
    }


    //==================================================
    // OBTENER FECHA DE MAÑANA
    //==================================================

    std::string manana =
        sumarDias(hoy, 1);

    if(manana.empty())
    {
        return;
    }


    //==================================================
    // CARGAR NOTIFICACIONES EXISTENTES
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        // Si el archivo todavía no existe,
        // comenzamos con un vector vacío.

        notificaciones.clear();
    }


    //==================================================
    // GENERAR ID PARA NUEVAS NOTIFICACIONES
    //==================================================

    int siguienteId =
        Persistencia::generarIdNotificacion(
            "notificaciones.txt"
        );


    //==================================================
    // CARGAR TAREAS
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return;
    }


    //==================================================
    // RECORDATORIOS DE TAREAS
    //
    // Solo se generan para tareas cuya fecha de
    // entrega sea mañana.
    //==================================================

    for(const auto& tarea : tareas)
    {
        //==================================================
        // Verificar fecha de entrega
        //==================================================

        if(tarea.getFechaEntrega() != manana)
        {
            continue;
        }


        //==================================================
        // Obtener alumnos inscritos
        //==================================================

        std::vector<int> alumnos =
            Inscripciones::obtenerAlumnosMateria(
                tarea.getMateriaId(),
                "inscripciones.txt"
            );


        //==================================================
        // Crear recordatorio para cada alumno
        //==================================================

        for(int idAlumno : alumnos)
        {
            //==================================================
            // Verificar si ya existe el recordatorio
            //
            // Se considera duplicado si:
            //
            // - Es para el mismo alumno
            // - Es RECORDATORIO
            // - Hace referencia a la misma tarea
            // - Es una referencia TAREA
            // - Fue generado hoy
            //==================================================

            bool existe = false;

            for(const auto& notificacion :
                notificaciones)
            {
                if(
                    notificacion.getUsuarioId() ==
                        idAlumno
                    &&
                    notificacion.getTipo() ==
                        TipoNotificacion::RECORDATORIO
                    &&
                    notificacion.getIdReferencia() ==
                        tarea.getId()
                    &&
                    notificacion.getTipoReferencia() ==
                        TipoReferenciaNotificacion::TAREA
                    &&
                    notificacion.getFecha() ==
                        hoy
                )
                {
                    existe = true;
                    break;
                }
            }


            //==================================================
            // Si ya existe, no crear otro
            //==================================================

            if(existe)
            {
                continue;
            }


            //==================================================
            // Crear notificación
            //==================================================

            Notificacion nuevaNotificacion(
                siguienteId,
                idAlumno,
                TipoNotificacion::RECORDATORIO,
                tarea.getId(),
                TipoReferenciaNotificacion::TAREA,
                "Recordatorio",
                "Tienes una tarea proxima a vencer.",
                hoy
            );


            //==================================================
            // Agregar al vector
            //==================================================

            notificaciones.push_back(
                nuevaNotificacion
            );


            siguienteId++;
        }
    }


    //==================================================
    // RECORDATORIOS DE SUBTAREAS
    //
    // Las subtareas no tienen fecha propia.
    //
    // Su fecha se obtiene de planner.txt:
    //
    // idAlumno|fecha|tipo|idElemento|prioridad
    //
    //==================================================

    std::ifstream archivoPlanner(
        "planner.txt"
    );


    if(archivoPlanner.is_open())
    {
        std::string linea;


        //==================================================
        // Leer planner.txt
        //==================================================

        while(std::getline(
            archivoPlanner,
            linea))
        {
            if(linea.empty())
            {
                continue;
            }


            //==================================================
            // Separar campos
            //==================================================

            std::stringstream ss(linea);

            std::string idAlumnoTexto;
            std::string fecha;
            std::string tipo;
            std::string idElementoTexto;
            std::string prioridad;


            std::getline(
                ss,
                idAlumnoTexto,
                '|'
            );

            std::getline(
                ss,
                fecha,
                '|'
            );

            std::getline(
                ss,
                tipo,
                '|'
            );

            std::getline(
                ss,
                idElementoTexto,
                '|'
            );

            std::getline(
                ss,
                prioridad,
                '|'
            );


            //==================================================
            // Solo elementos programados para mañana
            //==================================================

            if(fecha != manana)
            {
                continue;
            }


            //==================================================
            // Solo subtareas
            //==================================================

            if(tipo != "SUBTAREA")
            {
                continue;
            }


            //==================================================
            // Convertir IDs
            //==================================================

            int idAlumno;
            int idSubtarea;

            try
            {
                idAlumno =
                    std::stoi(idAlumnoTexto);

                idSubtarea =
                    std::stoi(idElementoTexto);
            }
            catch(...)
            {
                continue;
            }


            //==================================================
            // Validar IDs
            //==================================================

            if(
                idAlumno <= 0 ||
                idSubtarea <= 0
            )
            {
                continue;
            }


            //==================================================
            // Verificar si ya existe el recordatorio
            //==================================================

            bool existe = false;

            for(const auto& notificacion :
                notificaciones)
            {
                if(
                    notificacion.getUsuarioId() ==
                        idAlumno
                    &&
                    notificacion.getTipo() ==
                        TipoNotificacion::RECORDATORIO
                    &&
                    notificacion.getIdReferencia() ==
                        idSubtarea
                    &&
                    notificacion.getTipoReferencia() ==
                        TipoReferenciaNotificacion::SUBTAREA
                    &&
                    notificacion.getFecha() ==
                        hoy
                )
                {
                    existe = true;
                    break;
                }
            }


            //==================================================
            // Si ya existe, no crear otro
            //==================================================

            if(existe)
            {
                continue;
            }


            //==================================================
            // Crear recordatorio de subtarea
            //==================================================

            Notificacion nuevaNotificacion(
                siguienteId,
                idAlumno,
                TipoNotificacion::RECORDATORIO,
                idSubtarea,
                TipoReferenciaNotificacion::SUBTAREA,
                "Recordatorio",
                "Tienes una subtarea programada para manana.",
                hoy
            );


            //==================================================
            // Agregar al vector
            //==================================================

            notificaciones.push_back(
                nuevaNotificacion
            );


            siguienteId++;
        }


        archivoPlanner.close();
    }


    //==================================================
    // GUARDAR NOTIFICACIONES
    //==================================================
    //
    // Se guarda el vector completo.
    //
    // Esto conserva las notificaciones anteriores
    // y agrega únicamente las nuevas.
    //==================================================

    if(!Persistencia::guardarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        return;
    }
}
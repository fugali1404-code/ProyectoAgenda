#include "sessionmaneger.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>

////////////////////////////////////////////////////////////
// FUNCIONES AUXILIARES DEL PLANNER
////////////////////////////////////////////////////////////


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

            if(
                tipo == "TAREA" &&
                idElemento == idTarea
            )
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

    tiempo += static_cast<std::time_t>(dias) *
              24 * 60 * 60;

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

    //---------------------------------
    // Identificador
    //---------------------------------

    datos.setIdentificador(usuario->getIdentificador());

    //---------------------------------
    // Materias
    //---------------------------------

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

    //---------------------------------
    // Sesión autenticada
    //---------------------------------

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


///////////////////////////////////////////////////////////////////
//Fechas
//////////////////////////////////////////////////////////////////

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

    Materia nueva;

    nueva.setId(
        Persistencia::generarIdMateria(
            "materias.txt"
        )
    );

    nueva.setNombre(nombre);

    // El profesor autenticado es el propietario
    // de la materia.
    nueva.setProfesorId(
        datos.obtenerUsuarioId()
    );

    if(!datos.agregarMateria(nueva))
    {
        return false;
    }

    return Persistencia::guardarMaterias(
        datos.obtenerUsuarioId(),
        datos.obtenerVectorMaterias(),
        "materias.txt"
    );
}

//////////////////////////////////////////////////////////////
// Obtener Materias
//////////////////////////////////////////////////////////////

std::string SessionManager::obtenerMaterias() const
{
    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    std::string lista = datos.obtenerMaterias();

    if(lista == "SIN_MATERIAS")
    {
        return "MATERIAS|";
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

    if(id <= 0)
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
    // BUSCAR MATERIA
    //==================================================

    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == id)
        {
            materiaEncontrada = true;

            // Verificar propietario
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
    // OBTENER ALUMNOS INSCRITOS
    //==================================================

    std::vector<int> alumnos =
        Inscripciones::obtenerAlumnosMateria(
            id,
            "inscripciones.txt"
        );

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
    //==================================================

    std::vector<int> idsTareas;

    for(const auto& tarea : tareas)
    {
        if(tarea.getMateriaId() == id)
        {
            idsTareas.push_back(
                tarea.getId()
            );
        }
    }

    //==================================================
    // CARGAR SUBTAREAS
    //==================================================

    std::vector<Subtarea> subtareas;

    bool haySubtareas =
        Persistencia::cargarSubtareas(
            subtareas,
            "subtareas.txt"
        );

    //==================================================
    // OBTENER IDS DE SUBTAREAS
    //==================================================

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

    //==================================================
    // ELIMINAR TAREAS
    //==================================================

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

    if(!Persistencia::guardarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // ELIMINAR ESTADOS DE LAS TAREAS
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
            return false;
        }
    }

    //==================================================
    // ELIMINAR TAREAS Y SUBTAREAS DEL PLANNER
    //==================================================

    for(int idAlumno : alumnos)
    {
        PlannerSemana planner;

        if(!Persistencia::cargarPlanner(
            idAlumno,
            planner,
            "planner.txt"))
        {
            continue;
        }

        // Eliminar tareas
        for(int idTarea : idsTareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarTarea(
                    idTarea
                );
            }
        }

        // Eliminar subtareas
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
            return false;
        }
    }

    //==================================================
    // ELIMINAR NOTIFICACIONES DE LAS TAREAS
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
            return false;
        }
    }

    //==================================================
    // ELIMINAR INSCRIPCIONES
    //==================================================

    for(int idAlumno : alumnos)
    {
        if(!Inscripciones::desinscribirAlumno(
            id,
            idAlumno,
            "inscripciones.txt"))
        {
            return false;
        }
    }

    //==================================================
    // ELIMINAR MATERIA
    //==================================================

    materias.erase(
        std::remove_if(
            materias.begin(),
            materias.end(),
            [id](const Materia& materia)
            {
                return materia.getId() == id;
            }
        ),
        materias.end()
    );

    //==================================================
    // GUARDAR MATERIAS
    //==================================================

    if(!Persistencia::guardarMaterias(
        datos.obtenerUsuarioId(),
        materias,
        "materias.txt"))
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

    // Buscar la materia
    std::vector<Materia>& materias =
        datos.obtenerVectorMaterias();

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
                if(otra.getId() != id &&
                   otra.getNombre() == nombre)
                {
                    return false;
                }
            }

            materia.setNombre(nombre);

            return Persistencia::guardarMaterias(
                datos.obtenerUsuarioId(),
                materias,
                "materias.txt"
            );
        }
    }

    return false;
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
    // ---------------------------------------------
    // Solo el Administrador puede crear usuarios
    // ---------------------------------------------

    if(!datos.estaAutenticado())
    {
        return false;
    }

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // ---------------------------------------------
    // Validar campos vacíos
    // ---------------------------------------------

    if(nombre.empty() ||
       correo.empty() ||
       password.empty() ||
       identificador.empty())
    {
        return false;
    }

    // ---------------------------------------------
    // Validar rol
    // ---------------------------------------------

    if(rol != "Administrador" &&
       rol != "Profesor" &&
       rol != "Alumno")
    {
        return false;
    }

    // ---------------------------------------------
    // Cargar usuarios existentes
    // ---------------------------------------------

    std::vector<Usuario*> usuarios;

    Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"
    );

    // ---------------------------------------------
    // Validar correo e identificador repetidos
    // ---------------------------------------------

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

    // ---------------------------------------------
    // Generar ID
    // ---------------------------------------------

    int id = Persistencia::generarIdUsuario(
        "usuarios.txt"
    );

    // ---------------------------------------------
    // Crear usuario según el rol
    // ---------------------------------------------

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

    // ---------------------------------------------
    // Guardar usuario
    // ---------------------------------------------

    bool resultado =
        Persistencia::agregarUsuario(
            *nuevoUsuario,
            "usuarios.txt"
        );

    // ---------------------------------------------
    // Liberar memoria
    // ---------------------------------------------

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
        respuesta +=
            std::to_string(
                usuarios[i]->getId()
            );

        respuesta += ",";

        respuesta +=
            usuarios[i]->getRol();

        respuesta += ",";

        respuesta +=
            usuarios[i]->getNombre();

        respuesta += ",";

        respuesta +=
            usuarios[i]->getCorreo();

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
    // ---------------------------------------------
    // Debe estar autenticado
    // ---------------------------------------------

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

    if(nombre.empty() ||
       correo.empty() ||
       password.empty() ||
       identificador.empty())
    {
        return false;
    }

    // ---------------------------------------------
    // Cargar usuarios
    // ---------------------------------------------

    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"))
    {
        return false;
    }

    Usuario* usuarioObjetivo = nullptr;

    // ---------------------------------------------
    // Buscar usuario
    // ---------------------------------------------

    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr &&
           usuario->getId() == id)
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

    // ---------------------------------------------
    // Validar correo e identificador duplicados
    // ---------------------------------------------

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

    // ---------------------------------------------
    // Actualizar datos
    // ---------------------------------------------

    usuarioObjetivo->setNombre(nombre);
    usuarioObjetivo->setCorreo(correo);
    usuarioObjetivo->setPassword(password);
    usuarioObjetivo->setIdentificador(identificador);

    // ---------------------------------------------
    // Guardar
    // ---------------------------------------------

    bool resultado =
        Persistencia::actualizarUsuario(
            *usuarioObjetivo,
            "usuarios.txt"
        );

    // ---------------------------------------------
    // Liberar memoria
    // ---------------------------------------------

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
    //==================================================
    // AUTENTICACIÓN
    //==================================================

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
                        return estado.getAlumnoId() == id;
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
                        return subtarea.getAlumnoId() == id;
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
        //
        // Por si existen notificaciones recibidas
        // directamente por el profesor.
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
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo profesores
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //==================================================
    // Validar IDs
    //==================================================

    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar materias
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return false;
    }

    //==================================================
    // Buscar materia y verificar propietario
    //==================================================

    bool materiaEncontrada = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            materiaEncontrada = true;

            // La materia debe pertenecer
            // al profesor que está realizando
            // la operación.

            if(materia.getProfesorId() !=
               datos.obtenerUsuarioId())
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
    // Cargar usuarios
    //==================================================

    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"))
    {
        return false;
    }

    //==================================================
    // Verificar que el usuario sea alumno
    //==================================================

    bool alumnoEncontrado = false;

    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr &&
           usuario->getId() == idAlumno)
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

    //==================================================
    // Verificar que no esté inscrito
    //==================================================

    if(Inscripciones::estaInscrito(
        idMateria,
        idAlumno,
        "inscripciones.txt"))
    {
        return false;
    }

    //==================================================
    // Cargar tareas existentes de la materia
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // Cargar estados existentes
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        // Si el archivo todavía no existe,
        // comenzamos con una lista vacía.

        estados.clear();
    }

    //==================================================
    // Crear estados para las tareas existentes
    // de esta materia
    //==================================================

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
            if(
                estado.getTareaId() == tarea.getId() &&
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

    //==================================================
    // Crear inscripción
    //==================================================

    if(!Inscripciones::inscribirAlumno(
        idMateria,
        idAlumno,
        "inscripciones.txt"))
    {
        return false;
    }

    //==================================================
    // Guardar estados de tareas
    //==================================================

    if(!Persistencia::guardarEstadosTareas(
        estados,
        "estadosTareas.txt"))
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
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Solo profesores
    //==================================================

    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    //==================================================
    // Validar IDs
    //==================================================

    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar materias
    //==================================================

    std::vector<Materia> materias;

    if(!Persistencia::cargarMaterias(
        materias,
        "materias.txt"))
    {
        return false;
    }

    //==================================================
    // Verificar que la materia exista
    // y pertenezca al profesor
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
    // Verificar que el alumno esté inscrito
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
    // Cargar tareas de la materia
    //==================================================

    std::vector<Tarea> tareas;

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // Obtener IDs de las tareas de esta materia
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
    // Eliminar estados del alumno
    // para las tareas de la materia
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
                    for(int idTarea : idsTareas)
                    {
                        if(
                            estado.getTareaId() ==
                            idTarea
                        )
                        {
                            return true;
                        }
                    }

                    return false;
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
    // Cargar subtareas
    //==================================================

    std::vector<Subtarea> subtareas;

    std::vector<int> idsSubtareas;

    if(Persistencia::cargarSubtareas(
        subtareas,
        "subtareas.txt"))
    {
        //================================================
        // Obtener IDs de subtareas pertenecientes
        // al alumno y a tareas de esta materia
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
        // Eliminar subtareas
        //================================================

        subtareas.erase(
            std::remove_if(
                subtareas.begin(),
                subtareas.end(),
                [&idsSubtareas](
                    const Subtarea& subtarea
                )
                {
                    for(int idSubtarea :
                        idsSubtareas)
                    {
                        if(
                            subtarea.getId() ==
                            idSubtarea
                        )
                        {
                            return true;
                        }
                    }

                    return false;
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
    // Limpiar Planner del alumno
    //==================================================

    PlannerSemana planner;

    if(Persistencia::cargarPlanner(
        idAlumno,
        planner,
        "planner.txt"))
    {
        //================================================
        // Eliminar tareas de esta materia
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
        // Eliminar subtareas de esta materia
        //================================================

        for(int idSubtarea :
            idsSubtareas)
        {
            for(int i = 0; i < 7; ++i)
            {
                planner.getDia(i).eliminarSubtarea(
                    idSubtarea
                );
            }
        }

        //================================================
        // Guardar Planner
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
    // Finalmente eliminar inscripción
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
    TipoTarea tipo
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
    // VERIFICAR QUE LA MATERIA EXISTE
    // Y PERTENECE AL PROFESOR
    //==================================================

    bool materiaValida = false;

    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            if(
                materia.getProfesorId() !=
                datos.obtenerUsuarioId()
            )
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

    if(!Persistencia::cargarTareas(
        tareas,
        "tareas.txt"))
    {
        return false;
    }

    //==================================================
    // GENERAR ID DE TAREA
    //==================================================

    int idTarea =
        Persistencia::generarIdTarea(
            "tareas.txt"
        );

    //==================================================
    // CREAR TAREA
    //==================================================

    Tarea nuevaTarea(
        idTarea,
        idMateria,
        titulo,
        descripcion,
        fechaEntrega,
        tipo
    );

    //==================================================
    // AGREGAR TAREA AL VECTOR
    //==================================================

    tareas.push_back(
        nuevaTarea
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
    // OBTENER ALUMNOS INSCRITOS
    //==================================================

    std::vector<int> alumnos =
        Inscripciones::obtenerAlumnosMateria(
            idMateria,
            "inscripciones.txt"
        );

    //==================================================
    // CARGAR ESTADOS DE TAREAS
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        // Si todavía no existe el archivo,
        // se comienza con un vector vacío.
        estados.clear();
    }

    //==================================================
    // CREAR ESTADO PARA CADA ALUMNO
    //==================================================

    for(int idAlumno : alumnos)
    {
        bool existe = false;

        for(const auto& estado : estados)
        {
            if(
                estado.getTareaId() == idTarea &&
                estado.getAlumnoId() == idAlumno
            )
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

    //==================================================
    // GUARDAR ESTADOS
    //==================================================

    if(!Persistencia::guardarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        return false;
    }

    //==================================================
    // NOTIFICACIONES
    //
    // Se genera una notificación individual
    // para cada alumno inscrito.
    //
    // idReferencia = idTarea
    // tipoReferencia = TAREA
    //==================================================

    std::string fechaNotificacion =
        fechaEntrega;

    for(int idAlumno : alumnos)
    {
        if(!agregarNotificacion(
            idAlumno,
            TipoNotificacion::NUEVA_TAREA,
            idTarea,
            TipoReferenciaNotificacion::TAREA,
            "Nueva tarea",
            "Se ha agregado una nueva tarea a una de tus materias.",
            fechaNotificacion
        ))
        {
            return false;
        }
    }

    //==================================================
    // IMPORTANTE
    //
    // No se modifica planner.txt aquí.
    //
    // El Planner obtiene la tarea a partir
    // de la fecha de entrega.
    //==================================================

    return true;
}


/////////////////////////////////////////////////////////////
// Obtener Tareas
/////////////////////////////////////////////////////////////

std::vector<Tarea> SessionManager::obtenerTareas() const
{
    std::vector<Tarea> tareasPermitidas;

    //==================================================
    // Verificar autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return tareasPermitidas;
    }

    //==================================================
    // Cargar todas las tareas
    //==================================================

    std::vector<Tarea> todasLasTareas;

    if(!Persistencia::cargarTareas(
        todasLasTareas,
        "tareas.txt"))
    {
        return tareasPermitidas;
    }

    //==================================================
    // PROFESOR
    //==================================================

    if(datos.obtenerRol() == "Profesor")
    {
        std::vector<Materia> materias;

        if(!Persistencia::cargarMaterias(
            materias,
            "materias.txt"))
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

                if(materia.getProfesorId() ==
                       datos.obtenerUsuarioId())
                {
                    // Después verificamos que la tarea
                    // pertenezca a esa materia

                    if(tarea.getMateriaId() ==
                       materia.getId())
                    {
                        tareasPermitidas.push_back(
                            tarea
                        );

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
        std::vector<int> materiasInscritas =
            Inscripciones::obtenerMateriasAlumno(
                datos.obtenerUsuarioId(),
                "inscripciones.txt"
            );

        // Buscar tareas de las materias
        // en las que está inscrito

        for(const auto& tarea : todasLasTareas)
        {
            for(int idMateria : materiasInscritas)
            {
                if(tarea.getMateriaId() ==
                   idMateria)
                {
                    tareasPermitidas.push_back(
                        tarea
                    );

                    break;
                }
            }
        }

        return tareasPermitidas;
    }

    //==================================================
    // Cualquier otro rol
    //==================================================

    return tareasPermitidas;
}



//////////////////////////////////////////////////////////////
// Editar Tarea
//////////////////////////////////////////////////////////////

bool SessionManager::editarTarea(
    int idTarea,
    const std::string& titulo,
    const std::string& fechaEntrega,
    const std::string& descripcion,
    TipoTarea tipo
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

    if(idTarea <= 0)
    {
        return false;
    }

    if(
        titulo.empty() ||
        fechaEntrega.empty() ||
        descripcion.empty()
    )
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

    //==================================================
    // OBTENER MATERIA
    //==================================================

    int idMateria =
        tareaEncontrada->getMateriaId();

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
    // GUARDAR FECHA ANTERIOR
    //==================================================

    std::string fechaAnterior =
        tareaEncontrada->getFechaEntrega();

    //==================================================
    // ACTUALIZAR TAREA
    //
    // IMPORTANTE:
    // NO se modifica idMateria.
    //==================================================

    tareaEncontrada->setTitulo(
        titulo
    );

    tareaEncontrada->setFechaEntrega(
        fechaEntrega
    );

    tareaEncontrada->setDescripcion(
        descripcion
    );

    tareaEncontrada->setTipo(
        tipo
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
    // SI NO CAMBIÓ LA FECHA
    //
    // No se genera notificación.
    // No se modifica planner.
    //==================================================

    if(fechaAnterior == fechaEntrega)
    {
        return true;
    }

    //==================================================
    // ACTUALIZAR PLANNER
    //
    // La tarea conserva su ID.
    // Solamente cambia su fecha.
    //==================================================

    if(!actualizarTareaEnPlannerArchivo(
        idTarea,
        fechaEntrega,
        "planner.txt"))
    {
        return false;
    }

    //==================================================
    // OBTENER ALUMNOS INSCRITOS
    //==================================================

    std::vector<int> alumnos =
        Inscripciones::obtenerAlumnosMateria(
            idMateria,
            "inscripciones.txt"
        );

    //==================================================
    // FECHA DE LA NOTIFICACIÓN
    //==================================================

    std::string fechaNotificacion =
        obtenerFechaActual();

    //==================================================
    // CREAR NOTIFICACIÓN PARA CADA ALUMNO
    //
    // idReferencia = idTarea
    // tipoReferencia = TAREA
    //==================================================

    for(int idAlumno : alumnos)
    {
        if(!agregarNotificacion(
            idAlumno,
            TipoNotificacion::CAMBIO_FECHA,
            idTarea,
            TipoReferenciaNotificacion::TAREA,
            "Cambio de fecha",
            "La fecha de entrega de una tarea ha cambiado.",
            fechaNotificacion
        ))
        {
            return false;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////
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
            [idTarea](const Tarea& tarea)
            {
                return tarea.getId() ==
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
                    return estado.getTareaId() ==
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
                    return subtarea.getTareaId() ==
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
    // IMPORTANTE:
    //
    // Solo se eliminan las notificaciones que
    // hacen referencia a ESTA tarea.
    //
    // No se eliminan las demás notificaciones
    // del alumno.
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
    // NOTA:
    // Aquí NO se elimina la inscripción porque
    // solamente estamos eliminando una tarea.
    // La inscripción pertenece a la materia.
    //==================================================

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

    std::vector<Tarea> tareas =
        obtenerTareas();

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
    // Obtener Planner del alumno
    // =============================================

    PlannerSemana& planner =
        datos.obtenerPlanner();

    // =============================================
    // Buscar la tarea en los 7 días
    // =============================================

    for(int i = 0; i < 7; ++i)
    {
        PlannerDia& dia =
            planner.getDia(i);

        for(const TareaPlanner& tarea :
            dia.getTareas())
        {
            if(tarea.idTarea == idTarea)
            {
                dia.cambiarPrioridadTarea(
                    idTarea,
                    prioridad
                );

                // =================================
                // Guardar cambios
                // =================================

                return Persistencia::guardarPlanner(
                    datos.obtenerUsuarioId(),
                    planner,
                    "planner.txt"
                );
            }
        }
    }

    // =============================================
    // La tarea no está en el Planner
    // =============================================

    return false;
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
    // Obtener Planner correctamente inicializado
    // =============================================

    PlannerSemana& planner =
        obtenerPlanner();

    // =============================================
    // Buscar si la fecha pertenece a la semana
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

    // =============================================
    // Verificar si la subtarea ya está
    // en algún día
    // =============================================

    for(int i = 0; i < 7; ++i)
    {
        const PlannerDia& dia =
            planner.getDia(i);

        for(int id : dia.getSubtareas())
        {
            if(id == idSubtarea)
            {
                // =================================
                // Si ya está exactamente en esa
                // fecha, no hay nada que hacer.
                // =================================

                if(i == indiceDia)
                {
                    return true;
                }
            }
        }
    }

    // =============================================
    // Eliminar la subtarea de cualquier otro día
    // =============================================

    for(int i = 0; i < 7; ++i)
    {
        if(i != indiceDia)
        {
            planner.getDia(i).eliminarSubtarea(
                idSubtarea
            );
        }
    }

    // =============================================
    // Agregar al nuevo día
    // =============================================

    planner.getDia(indiceDia).agregarSubtarea(
        idSubtarea
    );

    // =============================================
    // Guardar Planner
    // =============================================

    return Persistencia::guardarPlanner(
        idAlumno,
        planner,
        "planner.txt"
    );
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

    PlannerDia& dia =
        planner.getDia(indiceDia);

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

    dia.eliminarSubtarea(
        idSubtarea
    );

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
            resultado.push_back(
                estado
            );
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

    //==================================================
    // ID del alumno actual
    //==================================================

    int idAlumno =
        datos.obtenerUsuarioId();

    //==================================================
    // Cargar estados
    //==================================================

    std::vector<EstadoTareaAlumno> estados;

    if(!Persistencia::cargarEstadosTareas(
        estados,
        "estadosTareas.txt"))
    {
        return resultado;
    }

    //==================================================
    // Filtrar estados del alumno actual
    // y por el estado solicitado
    //==================================================

    for(const auto& estadoTarea : estados)
    {
        if(
            estadoTarea.getAlumnoId() == idAlumno &&
            estadoTarea.getEstado() == estado
        )
        {
            resultado.push_back(
                estadoTarea
            );
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
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Validar usuario
    //==================================================

    if(idUsuario <= 0)
    {
        return false;
    }

    //==================================================
    // Validar referencia
    //==================================================

    if(
        idReferencia < 0
    )
    {
        return false;
    }

    //==================================================
    // Validar datos
    //==================================================

    if(
        titulo.empty() ||
        mensaje.empty() ||
        fecha.empty()
    )
    {
        return false;
    }

    //==================================================
    // Cargar notificaciones
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        notificaciones.clear();
    }

    //==================================================
    // Generar ID
    //==================================================

    int idNotificacion =
        Persistencia::generarIdNotificacion(
            "notificaciones.txt"
        );

    //==================================================
    // Crear notificación
    //==================================================

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

    //==================================================
    // Agregar
    //==================================================

    notificaciones.push_back(
        nuevaNotificacion
    );

    //==================================================
    // Guardar
    //==================================================

    return Persistencia::guardarNotificaciones(
        notificaciones,
        "notificaciones.txt"
    );
}

////////////////////////////////////////////////////////////
// Obtener Notificaciones
////////////////////////////////////////////////////////////

std::vector<Notificacion>
SessionManager::obtenerNotificaciones() const
{
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
    //==================================================
    // Autenticación
    //==================================================

    if(!datos.estaAutenticado())
    {
        return false;
    }

    //==================================================
    // Validar ID
    //==================================================

    if(idNotificacion <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar notificaciones
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        return false;
    }

    //==================================================
    // Usuario conectado
    //==================================================

    int idUsuario =
        datos.obtenerUsuarioId();

    //==================================================
    // Buscar únicamente una propia
    //==================================================

    bool encontrada = false;

    for(auto& notificacion : notificaciones)
    {
        if(
            notificacion.getId() == idNotificacion &&
            notificacion.getUsuarioId() == idUsuario
        )
        {
            notificacion.marcarComoLeida();

            encontrada = true;

            break;
        }
    }

    //==================================================
    // No encontrada o no pertenece
    //==================================================

    if(!encontrada)
    {
        return false;
    }

    //==================================================
    // Guardar cambios
    //==================================================

    return Persistencia::guardarNotificaciones(
        notificaciones,
        "notificaciones.txt"
    );
}

////////////////////////////////////////////////////////////
// Eliminar Notificación
////////////////////////////////////////////////////////////

bool SessionManager::eliminarNotificacion(
    int idNotificacion
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
    // Validar ID
    //==================================================

    if(idNotificacion <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar notificaciones
    //==================================================

    std::vector<Notificacion> notificaciones;

    if(!Persistencia::cargarNotificaciones(
        notificaciones,
        "notificaciones.txt"))
    {
        return false;
    }

    //==================================================
    // Usuario actual
    //==================================================

    int idUsuario =
        datos.obtenerUsuarioId();

    //==================================================
    // Buscar y eliminar
    //==================================================

    bool eliminada = false;

    for(
        auto it = notificaciones.begin();
        it != notificaciones.end();
        ++it
    )
    {
        if(
            it->getId() == idNotificacion &&
            it->getUsuarioId() == idUsuario
        )
        {
            notificaciones.erase(it);

            eliminada = true;

            break;
        }
    }

    //==================================================
    // No encontrada
    //==================================================

    if(!eliminada)
    {
        return false;
    }

    //==================================================
    // Guardar
    //==================================================

    return Persistencia::guardarNotificaciones(
        notificaciones,
        "notificaciones.txt"
    );
}
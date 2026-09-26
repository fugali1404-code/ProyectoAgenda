#include "controllers/materiasController.hpp"

#include "protocolo.hpp"

MateriasController::MateriasController(
    NetworkManager& networkManager,
    SessionClient& sessionClient
)
    : network(networkManager),
      session(sessionClient)
{
}

///////////////////////////////////////////////////////////
// Convertir respuesta a materias
///////////////////////////////////////////////////////////

std::vector<Materia>
MateriasController::convertirMaterias(
    const std::string& respuesta
) const
{
    std::vector<Materia> materias;

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta,
            '|'
        );

    if(datos.empty())
    {
        return materias;
    }

    ///////////////////////////////////////////////////////
    // MATERIAS
    ///////////////////////////////////////////////////////

    if(datos[0] == "MATERIAS")
    {
        for(std::size_t i = 1;
            i < datos.size();
            ++i)
        {
            std::vector<std::string> datosMateria =
                Protocol::dividir(
                    datos[i],
                    ';'
                );

            if(datosMateria.size() < 3)
            {
                continue;
            }

            try
            {
                int idMateria =
                    std::stoi(
                        datosMateria[0]
                    );

                std::string nombre =
                    datosMateria[1];

                int profesorId =
                    std::stoi(
                        datosMateria[2]
                    );

                Materia materia(
                    idMateria,
                    nombre,
                    profesorId
                );

                materias.push_back(
                    materia
                );
            }
            catch(...)
            {
                continue;
            }
        }

        return materias;
    }

    ///////////////////////////////////////////////////////
    // MATERIAS ALUMNO
    ///////////////////////////////////////////////////////

    if(datos[0] == "MATERIAS_ALUMNO")
    {
        for(std::size_t i = 1;
            i < datos.size();
            ++i)
        {
            std::vector<std::string> datosMateria =
                Protocol::dividir(
                    datos[i],
                    ';'
                );

            if(datosMateria.size() < 3)
            {
                continue;
            }

            try
            {
                int idMateria =
                    std::stoi(
                        datosMateria[0]
                    );

                std::string nombre =
                    datosMateria[1];

                std::string nombreProfesor =
                    datosMateria[2];

                Materia materia(
                    idMateria,
                    nombre,
                    0
                );

                materias.push_back(
                    materia
                );

                //-------------------------------------------------
                // Guardar profesor
                //-------------------------------------------------

                const_cast<
                    MateriasController*
                >(this)->profesores.push_back(
                    nombreProfesor
                );
            }
            catch(...)
            {
                continue;
            }
        }
    }

    return materias;
}

///////////////////////////////////////////////////////////
// Obtener materias
///////////////////////////////////////////////////////////

std::vector<Materia>
MateriasController::obtenerMaterias()
{
    std::vector<Materia> materias;

    //-------------------------------------------------
    // Limpiar profesores anteriores
    //-------------------------------------------------

    profesores.clear();

    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return materias;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return materias;
    }

    //-------------------------------------------------
    // Alumno
    //-------------------------------------------------

    if(session.obtenerRol() == "Alumno")
    {
        std::string respuesta =
            network.enviarComando(
                "GET_MATERIAS_ALUMNO"
            );

        return convertirMaterias(
            respuesta
        );
    }

    //-------------------------------------------------
    // Profesor
    //-------------------------------------------------

    if(session.obtenerRol() == "Profesor")
    {
        std::string respuesta =
            network.enviarComando(
                "GET_MATERIAS"
            );

        return convertirMaterias(
            respuesta
        );
    }

    //-------------------------------------------------
    // Otro rol
    //-------------------------------------------------

    return materias;
}

///////////////////////////////////////////////////////////
// Obtener profesores
///////////////////////////////////////////////////////////

const std::vector<std::string>&
MateriasController::obtenerProfesores() const
{
    return profesores;
}

///////////////////////////////////////////////////////////
// Agregar materia
///////////////////////////////////////////////////////////

bool MateriasController::agregarMateria(
    const std::string& nombre
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar nombre
    //-------------------------------------------------

    if(nombre.empty())
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "ADD_MATERIA|" +
            nombre
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta ==
        "MATERIA_AGREGADA";
}

///////////////////////////////////////////////////////////
// Actualizar materia
///////////////////////////////////////////////////////////

bool MateriasController::actualizarMateria(
    int idMateria,
    const std::string& nombre
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar nombre
    //-------------------------------------------------

    if(nombre.empty())
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "UPDATE_MATERIA|" +
            std::to_string(idMateria) +
            "|" +
            nombre
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta ==
        "MATERIA_ACTUALIZADA";
}

///////////////////////////////////////////////////////////
// Eliminar materia
///////////////////////////////////////////////////////////

bool MateriasController::eliminarMateria(
    int idMateria
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "DELETE_MATERIA|" +
            std::to_string(idMateria)
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta == "MATERIA_ELIMINADA";
}

///////////////////////////////////////////////////////////
// Obtener alumnos de una materia
///////////////////////////////////////////////////////////

std::vector<MateriasController::AlumnoMateria>
MateriasController::obtenerAlumnosMateria(
    int idMateria
)
{
    std::vector<AlumnoMateria> alumnos;

    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Verificar ID
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "GET_ALUMNOS_MATERIA|" +
            std::to_string(idMateria)
        );

    //-------------------------------------------------
    // Dividir respuesta
    //
    // ALUMNOS|id;nombre;identificador|...
    //-------------------------------------------------

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta,
            '|'
        );

    if(datos.empty())
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    if(datos[0] != "ALUMNOS")
    {
        return alumnos;
    }

    //-------------------------------------------------
    // Convertir alumnos
    //-------------------------------------------------

    for(std::size_t i = 1;
        i < datos.size();
        ++i)
    {
        std::vector<std::string> datosAlumno =
            Protocol::dividir(
                datos[i],
                ';'
            );

        if(datosAlumno.size() < 3)
        {
            continue;
        }

        try
        {
            AlumnoMateria alumno;

            alumno.id =
                std::stoi(
                    datosAlumno[0]
                );

            alumno.nombre =
                datosAlumno[1];

            alumno.identificador =
                datosAlumno[2];

            alumnos.push_back(
                alumno
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return alumnos;
}


///////////////////////////////////////////////////////////
// Obtener ponderaciones
///////////////////////////////////////////////////////////

std::vector<MateriasController::PonderacionMateria>
MateriasController::obtenerPonderaciones(
    int idMateria
)
{
    std::vector<PonderacionMateria> ponderaciones;

    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return ponderaciones;
    }


    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return ponderaciones;
    }

    //-------------------------------------------------
    // Verificar ID
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return ponderaciones;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "GET_PONDERACIONES|" +
            std::to_string(idMateria)
        );

    //-------------------------------------------------
    // Dividir respuesta
    //-------------------------------------------------

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta,
            '|'
        );

    if(datos.empty())
    {
        return ponderaciones;
    }

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    if(datos[0] != "PONDERACIONES")
    {
        return ponderaciones;
    }

    //-------------------------------------------------
    // Cada ponderacion tiene 7 datos
    //
    // parcial
    // tarea
    // examen
    // practica
    // proyecto
    // trabajo
    // otro
    //-------------------------------------------------

    for(std::size_t i = 1;
        i + 6 < datos.size();
        i += 7)
    {
        try
        {
            PonderacionMateria ponderacion;

            ponderacion.parcial =
                std::stoi(datos[i]);

            ponderacion.tarea =
                std::stod(datos[i + 1]);

            ponderacion.examen =
                std::stod(datos[i + 2]);

            ponderacion.practica =
                std::stod(datos[i + 3]);

            ponderacion.proyecto =
                std::stod(datos[i + 4]);

            ponderacion.trabajo =
                std::stod(datos[i + 5]);

            ponderacion.otro =
                std::stod(datos[i + 6]);

            ponderaciones.push_back(
                ponderacion
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return ponderaciones;
}

///////////////////////////////////////////////////////////
// Configurar ponderacion
///////////////////////////////////////////////////////////

bool MateriasController::configurarPonderacion(
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
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar datos
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return false;
    }

    if(parcial < 0)
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "SET_PONDERACION|" +
            std::to_string(idMateria) +
            "|" +
            std::to_string(parcial) +
            "|" +
            std::to_string(tarea) +
            "|" +
            std::to_string(examen) +
            "|" +
            std::to_string(practica) +
            "|" +
            std::to_string(proyecto) +
            "|" +
            std::to_string(trabajo) +
            "|" +
            std::to_string(otro)
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta == "PONDERACION_GUARDADA";
}

///////////////////////////////////////////////////////////
// Eliminar ponderacion
///////////////////////////////////////////////////////////

bool MateriasController::eliminarPonderacion(
    int idMateria,
    int parcial
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar datos
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return false;
    }

    if(parcial < 0)
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "DELETE_PONDERACION|" +
            std::to_string(idMateria) +
            "|" +
            std::to_string(parcial)
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta == "PONDERACION_ELIMINADA";

}

///////////////////////////////////////////////////////////
// Obtener calificacion final
///////////////////////////////////////////////////////////

bool MateriasController::obtenerCalificacionFinal(
    int idAlumno,
    int idMateria,
    double& calificacionFinal
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar IDs
    //-------------------------------------------------

    if(idAlumno <= 0)
    {
        return false;
    }

    if(idMateria <= 0)
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "GET_CALIFICACION_FINAL|" +
            std::to_string(idAlumno) +
            "|" +
            std::to_string(idMateria)
        );

    //-------------------------------------------------
    // Dividir respuesta
    //-------------------------------------------------

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta,
            '|'
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    if(datos.size() < 4)
    {
        return false;
    }

    if(datos[0] != "CALIFICACION_FINAL")
    {
        return false;
    }

    //-------------------------------------------------
    // Convertir calificacion
    //-------------------------------------------------

    try
    {
        calificacionFinal =
            std::stod(datos[3]);
    }
    catch(...)
    {
        return false;
    }

    return true;
}




///////////////////////////////////////////////////////////
// Inscribir alumno
///////////////////////////////////////////////////////////

bool MateriasController::inscribirAlumno(
    int idMateria,
    const std::string& boletas
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar ID de materia
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar boletas
    //-------------------------------------------------

    if(boletas.empty())
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "INSCRIBIR_ALUMNO|" +
            std::to_string(idMateria) +
            "|" +
            boletas
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta ==
        "ALUMNO_INSCRITO";
}

///////////////////////////////////////////////////////////
// Desinscribir alumno
///////////////////////////////////////////////////////////

bool MateriasController::desinscribirAlumno(
    int idMateria,
    const std::string& boletas
)
{
    //-------------------------------------------------
    // Verificar sesión
    //-------------------------------------------------

    if(!session.estaAutenticado())
    {
        return false;
    }

    //-------------------------------------------------
    // Solo profesores
    //-------------------------------------------------

    if(session.obtenerRol() != "Profesor")
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar ID de materia
    //-------------------------------------------------

    if(idMateria <= 0)
    {
        return false;
    }

    //-------------------------------------------------
    // Verificar boletas
    //-------------------------------------------------

    if(boletas.empty())
    {
        return false;
    }

    //-------------------------------------------------
    // Enviar comando
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "DESINSCRIBIR_ALUMNO|" +
            std::to_string(idMateria) +
            "|" +
            boletas
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    return respuesta == "ALUMNO_DESINSCRITO";
}
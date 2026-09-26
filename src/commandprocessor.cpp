#include "commandprocessor.hpp"


#include <sstream>
#include <string>
#include <iostream>

std::string CommandProcessor::procesar(
    const std::vector<std::string>& datos,
    SessionManager& session
)
{
    if(datos.empty())
        return "ERROR|Mensaje vacio";

    std::string comando = datos[0];

    //---------------------------------
    // LOGIN
    //---------------------------------

    if(comando == "LOGIN")
    {
        if(datos.size() < 3)
        {
            return "LOGIN_ERROR";
        }

        if(session.login(datos[1], datos[2]))
        {
            return
                "LOGIN_OK|"
                + session.obtenerNombreCompleto()
                + "|"
                + session.obtenerRol()
                + "|"
                + session.obtenerIdentificador();
        }

        return "LOGIN_ERROR";
    }

    //---------------------------------
    // LOGOUT
    //---------------------------------

    if(comando == "LOGOUT")
    {
        session.logout();
        return "LOGOUT_OK";
    }



    ///////////////////////////////////////////////////////
    // GET_MI_ID
    ///////////////////////////////////////////////////////

    if(comando == "GET_MI_ID")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        return
            "MI_ID|" +
            std::to_string(session.obtenerUsuarioId());
    }


    ///////////////////////////////////////////////////////////
    // ADD_USUARIO
    ///////////////////////////////////////////////////////////

    
    else if(datos[0] == "ADD_USUARIO")
    {

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }
        
        if(session.obtenerRol() != "Administrador")
        {   
            return "PERMISO_DENEGADO";
        }
        
        if(datos.size() != 6)
        {
            return "ERROR";
        }

        bool ok = session.agregarUsuario(
            
            datos[1], // rol
            datos[2], // nombre
            datos[3], // correo
            datos[4],  // password
            datos[5]  // identificador
        );

        if(ok)
        {
            return "USUARIO_AGREGADO";
        }

        return "ERROR";
    }


    ///////////////////////////////////////////////////////////
    // DELETE_USUARIO
    ///////////////////////////////////////////////////////////


    if(comando == "DELETE_USUARIO")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Administrador")
        {
            return "ERROR|Solo el administrador puede gestionar usuarios";
        }

        if(datos.size() < 2)
        {
            return "ERROR|ID de usuario requerido";
        }

        int id;

        try
        {
            id = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de usuario invalido";
        }

        if(session.eliminarUsuario(id))
        {
            return "USUARIO_ELIMINADO";
        }

        return "ERROR|No se pudo eliminar el usuario";
    }

    ///////////////////////////////////////////////////////////
    // UPDATE_USUARIO
    ///////////////////////////////////////////////////////////

    if(comando == "UPDATE_USUARIO")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Administrador")
        {
            return "ERROR|Solo el administrador puede gestionar usuarios";
        }

        if(datos.size() < 6)
        {
            return "ERROR|Datos insuficientes";
        }

        int id;

        try
        {
            id = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de usuario invalido";
        }

        const std::string& nombre = datos[2];
        const std::string& correo = datos[3];
        const std::string& password = datos[4];
        const std::string& identificador = datos[5];

        if(session.actualizarUsuario(
            id,
            nombre,
            correo,
            password,
            identificador))
        {
            return "USUARIO_ACTUALIZADO";
        }

        return "ERROR|No se pudo actualizar el usuario";
    }


    ///////////////////////////////////////////////////////////
    // GET_USUARIOS
    ///////////////////////////////////////////////////////////

    else if(datos[0] == "GET_USUARIOS")
    {
        if(session.obtenerRol() != "Administrador")
        {
            return "PERMISO_DENEGADO";
        }

        return session.obtenerUsuarios();
    }

    ///////////////////////////////////////////////////////////
    // GET_ROL
    ///////////////////////////////////////////////////////////

    else if(datos[0] == "GET_ROL")
    {
        return session.obtenerRol();
    }

    ///////////////////////////////////////////////////////////
    // ADD_MATERIA
    ///////////////////////////////////////////////////////////

    if(comando == "ADD_MATERIA")
    {
        // Debe existir una sesión activa
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        // Solo los profesores pueden crear materias
        if(session.obtenerRol() != "Profesor")
        {
            return "ERROR|Solo los profesores pueden crear materias";
        }

        // Verificar que se haya enviado el nombre
        if(datos.size() < 2)
        {
            return "ERROR|Nombre de materia faltante";
        }

        // Verificar nombre vacío
        if(datos[1].empty())
        {
            return "ERROR|Nombre de materia vacio";
        }

        // Intentar agregar
        if(session.agregarMateria(datos[1]))
        {
            return "MATERIA_AGREGADA";
        }   

        // El nombre ya existe
        return "MATERIA_EXISTENTE";
    }

    ///////////////////////////////////////////////////////////
    // GET_MATERIAS
    ///////////////////////////////////////////////////////////

    if(comando == "GET_MATERIAS")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        // Solo profesores
        if(session.obtenerRol() != "Profesor")
        {
            return "ERROR|Solo los profesores pueden gestionar materias";
        }

        return session.obtenerMaterias();
    }


    /////////////////////////////////////////////////////////
    // UPDATE_MATERIA
    /////////////////////////////////////////////////////////

    if(comando == "UPDATE_MATERIA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Profesor")
        {
            return "ERROR|Solo los profesores pueden gestionar materias";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Datos insuficientes";
        }

        int id;

        try
        {
            id = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de materia invalido";
        }

        const std::string& nombre = datos[2];

        if(nombre.empty())
        {
            return "ERROR|Nombre vacio";
        }
    

        if(session.editarMateria(id, nombre))
        {
            return "MATERIA_ACTUALIZADA";
        }

        return "ERROR|No se pudo actualizar la materia";
    }

    
    ////////////////////////////////////////////////////////
    // DELETE_MATERIA
    ////////////////////////////////////////////////////////

    if(comando == "DELETE_MATERIA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Profesor")
        {
            return "ERROR|Solo los profesores pueden gestionar materias";
        }   

        if(datos.size() < 2)
        {
            return "ERROR|ID de materia requerido";
        }

        int id;

        try
        {
            id = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de materia invalido";
        }

        if(session.eliminarMateria(id))
        {
            return "MATERIA_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la materia";
    }

    ///////////////////////////////////////////////////////////
    // AGREGAR CALIFICACION
    ///////////////////////////////////////////////////////////

    if(comando == "ADD_CALIFICACION")
    {
        
        // AUTENTICACIÓN
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

   
        // VALIDAR PARÁMETROS
        if(datos.size() < 4)
        {
            return "ERROR|Datos insuficientes";
        }

   
        // CONVERTIR ID ALUMNO
        int idAlumno;

        try
        {
            idAlumno = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de alumno invalido";
        }

    
        // CONVERTIR ID TAREA
        int idTarea;

        try
        {
            idTarea = std::stoi(datos[2]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

    
        // CONVERTIR CALIFICACIÓN
        double calificacion;

        try
        {
            calificacion = std::stod(datos[3]);
        }
        catch(...)
        {
            return "ERROR|Calificacion invalida";
        }   


        // AGREGAR CALIFICACIÓN
        if(session.registrarCalificacion(idAlumno,idTarea,calificacion))
        {
            return "CALIFICACION_AGREGADA";
        }

        return "ERROR|No se pudo agregar la calificacion";
    }


    ///////////////////////////////////////////////////////////
    // GET_CALIFICACIONES
    ///////////////////////////////////////////////////////////

    if(comando == "GET_CALIFICACIONES")
    {
        //==================================================
        // AUTENTICACIÓN
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // VALIDAR PARÁMETROS
        //==================================================

        if(datos.size() < 2)
        {
            return "ERROR|ID de alumno requerido";
        }

        //==================================================
        // CONVERTIR ID
        //==================================================

        int idAlumno;

        try
        {
            idAlumno = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de alumno invalido";
        }

        //==================================================
        // OBTENER CALIFICACIONES
        //==================================================

        std::vector<Calificacion> calificaciones;

        if(!session.obtenerCalificacionesAlumno(idAlumno,calificaciones))
        {
            return "ERROR|No se pudieron obtener las calificaciones";
        }

        //==================================================
        // VERIFICAR SI HAY CALIFICACIONES
        //==================================================

        if(calificaciones.empty())
        {
            return "SIN_CALIFICACIONES";
        }

        //==================================================
        // CONSTRUIR RESPUESTA
        //==================================================

        std::stringstream respuesta;

        respuesta << "CALIFICACIONES";

        for(const auto& calificacion : calificaciones)
        {
            respuesta
                << "|"
                << calificacion.getIdAlumno()
                << ";"
                << calificacion.getIdTarea()
                << ";"
                << calificacion.getCalificacion();
        }

        return respuesta.str();
    }

    ///////////////////////////////////////////////////////////
    // ACTUALIZAR CALIFICACION
    ///////////////////////////////////////////////////////////

    if(comando == "UPDATE_CALIFICACION")
    {
        //==================================================
        // AUTENTICACIÓN
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // VALIDAR PARÁMETROS
        //==================================================

        if(datos.size() < 4)
        {
            return "ERROR|Datos insuficientes";
        }

        //==================================================
        // CONVERTIR ID ALUMNO
        //==================================================

        int idAlumno;

        try
        {
            idAlumno = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de alumno invalido";
        }

        //==================================================
        // CONVERTIR ID TAREA
        //==================================================

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[2]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        //==================================================
        // CONVERTIR CALIFICACIÓN
        //==================================================

        double calificacion;

        try
        {
            calificacion = std::stod(datos[3]);
        }
        catch(...)
        {
            return "ERROR|Calificacion invalida";
        }

        //==================================================
        // ACTUALIZAR CALIFICACIÓN
        //==================================================

        if(session.editarCalificacion(idAlumno,idTarea,calificacion))
        {
            return "CALIFICACION_ACTUALIZADA";
        }

        return "ERROR|No se pudo actualizar la calificacion";
    }


    ////////////////////////////////////////////////////////
    // ELIMINAR CALIFICACION
    ////////////////////////////////////////////////////////

    if(comando == "DELETE_CALIFICACION")
    {
        //==================================================
        // AUTENTICACIÓN
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // VALIDAR PARÁMETROS
        //==================================================

        if(datos.size() < 3)
        {
            return "ERROR|Datos insuficientes";
        }

        //==================================================
        // CONVERTIR ID ALUMNO
        //==================================================

        int idAlumno;

        try
        {
            idAlumno = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de alumno invalido";
        }

        //==================================================
        // CONVERTIR ID TAREA
        //==================================================

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[2]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        //==================================================
        // ELIMINAR CALIFICACIÓN
        //==================================================

        if(session.eliminarCalificacion(idAlumno,idTarea))
        {
            return "CALIFICACION_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la calificacion";
    }


    ///////////////////////////////////////////////////////////
    // GET_CALIFICACION_FINAL
    ///////////////////////////////////////////////////////////

    if(comando == "GET_CALIFICACION_FINAL")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Datos insuficientes";
        }

        int idAlumno;

        try
        {
            idAlumno = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de alumno invalido";
        }

        int idMateria;

        try
        {   
            idMateria = std::stoi(datos[2]);
        }
        catch(...)
        {
            return "ERROR|ID de materia invalido";
        }

        double calificacionFinal;

        if(session.obtenerCalificacionFinal(idAlumno,idMateria,calificacionFinal))
        {
            std::stringstream respuesta;

            respuesta
                << "CALIFICACION_FINAL|"
                << idAlumno
                << "|"
                << idMateria
                << "|"
                << calificacionFinal;

            return respuesta.str();
        }

        return "ERROR|No se pudo obtener la calificacion final";
    }


    ////////////////////////////////////////////////////////
    // INSCRIBIR_ALUMNO
    ////////////////////////////////////////////////////////

    if(comando == "INSCRIBIR_ALUMNO")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idMateria;

        try
        {
            idMateria = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }

        std::string boleta = datos[2];

        if(boleta.empty())
        {
            return "ERROR|La boleta no puede estar vacia";
        }

        if(session.inscribirAlumno(idMateria, boleta))
        {
            return "ALUMNO_INSCRITO";
        }

        return "ERROR|No se pudo inscribir al alumno";
    }


    ////////////////////////////////////////////////////////
    // DESINSCRIBIR_ALUMNO
    ////////////////////////////////////////////////////////

    if(comando == "DESINSCRIBIR_ALUMNO")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idMateria;

        try
        {
            idMateria = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }

        std::string boleta = datos[2];

        if(boleta.empty())
        {
            return "ERROR|La boleta no puede estar vacia";
        }

        if(session.desinscribirAlumno(idMateria,boleta))
        {
            return "ALUMNO_DESINSCRITO";
        }

        return "ERROR|No se pudo desinscribir al alumno";
    }

    ////////////////////////////////////////////////////////
    // GET_ALUMNOS_MATERIA
    ////////////////////////////////////////////////////////

    if(comando == "GET_ALUMNOS_MATERIA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de materia";
        }

        int idMateria;

        try
        {
            idMateria = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }

        return session.obtenerAlumnosMateria(idMateria);
    }


    ////////////////////////////////////////////////////////
    // GET_MATERIAS_ALUMNO
    ////////////////////////////////////////////////////////

    if(comando == "GET_MATERIAS_ALUMNO")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        return session.obtenerMateriasAlumno();
    }


    /////////////////////////////////////////////////////////
    // ADD_TAREA
    ///////////////////////////////////////////////////////

    if(comando == "ADD_TAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 7)
        {
            return "ERROR|Faltan parametros";
        }

    
        // ID DE MATERIA
        int idMateria;

        try
        {
            idMateria = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }

        // DATOS DE LA TAREA
        std::string titulo = datos[2];
        std::string fechaEntrega = datos[3];
        std::string descripcion = datos[4];
        std::string tipoTexto = datos[5];
        std::string parcialTexto = datos[6];
    
        // VALIDAR DATOS
        if(titulo.empty())
        {
            return "ERROR|Falta titulo";
        }

        if(fechaEntrega.empty())
        {
            return "ERROR|Falta fecha de entrega";
        }

        if(descripcion.empty())
        {
            return "ERROR|Falta descripcion";
        }

        if(tipoTexto.empty())
        {
            return "ERROR|Falta tipo de tarea";
        }

        if(parcialTexto.empty())
        {
            return "ERROR|Falta parcial";
        }

        // CONVERTIR PARCIAL
        int parcial;

        try
        {
            parcial = std::stoi(parcialTexto);
        }
        catch(...)
        {
            return "ERROR|El parcial debe ser un numero";
        }


    
        // VALIDAR PARCIAL
        if(parcial < 0)
        {
            return "ERROR|El parcial no puede ser negativo";
        }

        // CONVERTIR TIPO DE TAREA
        TipoTarea tipo;

        if(tipoTexto == "TAREA")
        {
            tipo = TipoTarea::TAREA;
        }
        else if(tipoTexto == "EXAMEN")
        {
            tipo = TipoTarea::EXAMEN;
        }
        else if(tipoTexto == "PRACTICA")
        {
            tipo = TipoTarea::PRACTICA;
        }
        else if(tipoTexto == "PROYECTO")
        {
            tipo = TipoTarea::PROYECTO;
        }
        else if(tipoTexto == "TRABAJO")
        {
            tipo = TipoTarea::TRABAJO;
        }
        else if(tipoTexto == "OTRO")
        {
            tipo = TipoTarea::OTRO;
        }
        else
        {
            return "ERROR|Tipo de tarea invalido";
        }

        // CREAR TAREA
        if(session.agregarTarea(
            idMateria,
            titulo,
            fechaEntrega,
            descripcion,
            tipo,
            parcial
        ))
        {
            return "TAREA_CREADA";
        }

        return "ERROR|No se pudo crear la tarea";
    }


    /////////////////////////////////////////////////////////
    // GET_TAREAS
    /////////////////////////////////////////////////////////

    if(comando == "GET_TAREAS")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        std::vector<Tarea> tareas = session.obtenerTareas();
        std::string respuesta = "TAREAS";

        for(const auto& tarea : tareas)
        {
            respuesta += "|";

            respuesta += std::to_string(tarea.getId());
            respuesta += ";";
            respuesta +=
            std::to_string(tarea.getMateriaId());
            respuesta += ";";
            respuesta += tarea.getTitulo();
            respuesta += ";";
            respuesta += tarea.getFechaEntrega();
            respuesta += ";";
            respuesta += tarea.getDescripcion();
            respuesta += ";";
            respuesta += std::to_string(tarea.getParcial());
            respuesta += ";";
            respuesta += Persistencia::tipoTareaAString(tarea.getTipo());
        }

        return respuesta;
    }


    ///////////////////////////////////////////////////////
    // UPDATE_TAREA
    ///////////////////////////////////////////////////////

    if(comando == "UPDATE_TAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 7)
        {
            return "ERROR|Faltan parametros";
        }

        // ID DE TAREA
        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de tarea debe ser un numero";
        }


        // DATOS DE LA TAREA
        std::string titulo = datos[2];
        std::string fechaEntrega = datos[3];
        std::string descripcion = datos[4];
        std::string tipoTexto = datos[5];
        std::string parcialTexto = datos[6];


    
    // VALIDAR DATOS

        if(titulo.empty())
        {
            return "ERROR|Falta titulo";
        }

        if(fechaEntrega.empty())
        {
            return "ERROR|Falta fecha de entrega";
        }

        if(descripcion.empty())
        {
            return "ERROR|Falta descripcion";
        }

        if(tipoTexto.empty())
        {
            return "ERROR|Falta tipo de tarea";
        }

        if(parcialTexto.empty())
        {
            return "ERROR|Falta parcial";
        }


    
        // CONVERTIR PARCIAL
        int parcial;

        try
        {
            parcial = std::stoi(parcialTexto);
        }
        catch(...)
        {
            return "ERROR|El parcial debe ser un numero";
        }


    
        // VALIDAR PARCIAL
        if(parcial < 0)
        {
            return "ERROR|El parcial no puede ser negativo";
        }


        // CONVERTIR TIPO DE TAREA
        TipoTarea tipo;

        if(tipoTexto == "TAREA")
        {
            tipo = TipoTarea::TAREA;
        }
        else if(tipoTexto == "EXAMEN")
        {
            tipo = TipoTarea::EXAMEN;
        }
        else if(tipoTexto == "PRACTICA")
        {
            tipo = TipoTarea::PRACTICA;
        }
        else if(tipoTexto == "PROYECTO")
        {
            tipo = TipoTarea::PROYECTO;
        }
        else if(tipoTexto == "TRABAJO")
        {
            tipo = TipoTarea::TRABAJO;
        }
        else if(tipoTexto == "OTRO")
        {
            tipo = TipoTarea::OTRO;
        }
        else
        {
            return "ERROR|Tipo de tarea invalido";
        }


    
        // ACTUALIZAR TAREA
        if(session.editarTarea(
            idTarea,
            titulo,
            fechaEntrega,
            descripcion,
            tipo,
            parcial
        ))
        {
            return "TAREA_ACTUALIZADA";
        }

        return "ERROR|No se pudo actualizar la tarea";
    }

    ////////////////////////////////////////////////////////
    //ELIMINAR TAREAS
    ////////////////////////////////////////////////////////

    if(comando == "DELETE_TAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de tarea";
        }

        // ================================================
        // ID DE TAREA
        // ================================================

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de tarea debe ser un numero";
        }

        // ================================================
        // ELIMINAR TAREA
        // ================================================

        if(session.eliminarTarea(idTarea))
        {
            return "TAREA_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la tarea";
    }

    ///////////////////////////////////////////////////////
    // CONFIGURAR PONDERACION
    ///////////////////////////////////////////////////////

    if(comando == "SET_PONDERACION")
    {
    
        // AUTENTICACION
         if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // VALIDAR CANTIDAD DE PARAMETROS
        //
        // comando
        // idMateria
        // parcial
        // tarea
        // examen
        // practica
        // proyecto
        // trabajo
        // otro
        //==================================================

        if(datos.size() < 9)
        {
            return "ERROR|Faltan parametros";
        }

        
        // CONVERTIR ID DE MATERIA
        int idMateria;

        try
        {
            idMateria = std::stoi(
                datos[1]
            );
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }


        // CONVERTIR PARCIAL
        int parcial;

        try
        {
            parcial = std::stoi(
                datos[2]
            );
        }
        catch(...)
        {
            return "ERROR|El parcial debe ser un numero";
        }

        
        // CONVERTIR PONDERACIONES
        double tarea;
        double examen;
        double practica;
        double proyecto;
        double trabajo;
        double otro;

        try
        {
            tarea = std::stod(datos[3]);
            examen = std::stod(datos[4]);
            practica = std::stod(datos[5]);
            proyecto = std::stod(datos[6]);
            trabajo = std::stod(datos[7]);
            otro = std::stod(datos[8]);
        }
        catch(...)
        {
            return "ERROR|Las ponderaciones deben ser numeros";
        }

        
        // CONFIGURAR PONDERACION
        if(session.configurarPonderacion(
            idMateria,
            parcial,
            tarea,
            examen,
            practica,
            proyecto,
            trabajo,
            otro
        ))
        {
            return "PONDERACION_GUARDADA";
        }

        return "ERROR|No se pudo guardar la ponderacion";
    }


    ///////////////////////////////////////////////////////
    // OBTENER PONDERACIONES
    ///////////////////////////////////////////////////////

    if(comando == "GET_PONDERACIONES")
    {
        //==================================================
        // AUTENTICACION
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // VALIDAR PARAMETROS
        //==================================================

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de materia";
        }

        //==================================================
        // CONVERTIR ID
        //==================================================

        int idMateria;

        try
        {
            idMateria = std::stoi(
                datos[1]
            );
        }
        catch(...)
        {
            return "ERROR|El ID de materia debe ser un numero";
        }

        //==================================================
        // OBTENER PONDERACIONES
        //==================================================

        std::vector<Ponderacion> ponderaciones;

        if(!session.obtenerPonderacionesMateria(
            idMateria,
            ponderaciones
        ))
        {
            return "ERROR|No se pudieron obtener las ponderaciones";
        }

        //==================================================
        // SI NO HAY PONDERACIONES
        //==================================================

        if(ponderaciones.empty())
        {
            return "SIN_PONDERACIONES";
        }

        //==================================================
        // CONSTRUIR RESPUESTA
        //
        // Formato:
        //
        // PONDERACIONES|
        // parcial|tarea|examen|practica|
        // proyecto|trabajo|otro
        //==================================================

        std::string respuesta =
            "PONDERACIONES";

        for(const auto& ponderacion :
            ponderaciones)
        {
            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getParcial()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getTarea()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getExamen()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getPractica()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getProyecto()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getTrabajo()
                );

            respuesta +=
                "|" +
                std::to_string(
                    ponderacion.getOtro()
                );
        }

        return respuesta;
    }

    ///////////////////////////////////////////////////////////
    // DELETE_PONDERACION
    ///////////////////////////////////////////////////////////

    if(comando == "DELETE_PONDERACION")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idMateria;
        int parcial;

        try
        {
            idMateria = std::stoi(datos[1]);
            parcial = std::stoi(datos[2]);
        }   
        catch(...)
        {
            return "ERROR|Parametros invalidos";
        }

        if(idMateria <= 0)
        {
            return "ERROR|ID de materia invalido";
        }

        if(parcial < 0)
        {
            return "ERROR|Parcial invalido";
        }

        if(session.eliminarPonderacion(idMateria,parcial))
        {
            return "PONDERACION_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la ponderacion";
    }

    ////////////////////////////////////////////////////////
    // ADD_SUBTAREA
    ////////////////////////////////////////////////////////

    if(comando == "ADD_SUBTAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden crear subtareas";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        std::string descripcion = datos[2];

        if(descripcion.empty())
        {
            return "ERROR|La descripcion no puede estar vacia";
        }

        if(session.agregarSubtarea(idTarea,descripcion))
        {
            return "SUBTAREA_CREADA";
        }

        return "ERROR|No se pudo crear la subtarea";
    }

    ////////////////////////////////////////////////////////
    // GET_SUBTAREAS
    ////////////////////////////////////////////////////////

    if(comando == "GET_SUBTAREAS")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden consultar sus subtareas";
        }

        std::vector<Subtarea> subtareas = session.obtenerSubtareas();

        std::string resultado = "SUBTAREAS";

        for(const auto& subtarea : subtareas)
        {
            resultado += "|";
            resultado += std::to_string(subtarea.getId());
            resultado += "|";
            resultado += std::to_string(subtarea.getTareaId());
            resultado += "|";
            resultado += subtarea.getDescripcion();
            resultado += "|";

            switch(subtarea.getEstado())
            {
                case EstadoSubtarea::PENDIENTE:
                    resultado += "PENDIENTE";
                    break;

                case EstadoSubtarea::EN_PROGRESO:
                    resultado += "EN_PROGRESO";
                    break;

                case EstadoSubtarea::COMPLETADA:
                    resultado += "COMPLETADA";
                    break;
            }
        }

        return resultado;
    }

    ////////////////////////////////////////////////////////
    // UPDATE_SUBTAREA
    ////////////////////////////////////////////////////////

    if(comando == "UPDATE_SUBTAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden modificar subtareas";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idSubtarea;

        try
        {
            idSubtarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de subtarea invalido";
        }

        std::string descripcion = datos[2];

        if(descripcion.empty())
        {
            return "ERROR|La descripcion no puede estar vacia";
        }

        if(session.editarSubtarea(idSubtarea, descripcion))
        {
            return "SUBTAREA_ACTUALIZADA";
        }

        return "ERROR|No se pudo actualizar la subtarea";
    }

    ////////////////////////////////////////////////////////
    // DELETE_SUBTAREA
    ////////////////////////////////////////////////////////

    if(comando == "DELETE_SUBTAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden eliminar subtareas";
        }

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de subtarea";
        }

        int idSubtarea;

        try
        {
            idSubtarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de subtarea invalido";
        }

        if(session.eliminarSubtarea(idSubtarea))
        {
            return "SUBTAREA_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la subtarea";
    }

    ////////////////////////////////////////////////////////
    // UPDATE_ESTADO_SUBTAREA
    ////////////////////////////////////////////////////////

    if(comando == "UPDATE_ESTADO_SUBTAREA")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden cambiar el estado";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idSubtarea;

        try
        {
            idSubtarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de subtarea invalido";
        }

        std::string estadoTexto = datos[2];
        EstadoSubtarea estado;

        if(estadoTexto == "PENDIENTE")
        {   
            estado = EstadoSubtarea::PENDIENTE;
        }
        else if(estadoTexto == "EN_PROGRESO")
        {
            estado = EstadoSubtarea::EN_PROGRESO;
        }
        else if(estadoTexto == "COMPLETADA")
        {
            estado = EstadoSubtarea::COMPLETADA;
        }
        else
        {
            return "ERROR|Estado de subtarea invalido";
        }

        if(session.cambiarEstadoSubtarea(idSubtarea,estado))
        {
            return "ESTADO_SUBTAREA_ACTUALIZADO";
        }

        return "ERROR|No se pudo cambiar el estado";
    }

    
    ////////////////////////////////////////////////////////
    // GET_PLANNER
    ////////////////////////////////////////////////////////

    if(comando == "GET_PLANNER")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos tienen Planner";
        }

        PlannerSemana& planner = session.obtenerPlanner();
        std::string respuesta = "PLANNER";

        //==================================================
        // Recorrer los 7 días
        //==================================================

        for(int i = 0; i < 7; ++i)
        {
            const PlannerDia& dia = planner.getDia(i);

            respuesta += "|DIA,";
            respuesta += dia.getFecha();

            //==================================================
            // TAREAS
            //==================================================

            for(const auto& tarea : dia.getTareas())
            {
                respuesta += ",TAREA,";
                respuesta += std::to_string(tarea.idTarea);

                respuesta += ",";
                respuesta +=Persistencia::prioridadPlannerAString(tarea.prioridad);
            }

            //==================================================
            // SUBTAREAS
            //==================================================

            for(int idSubtarea : dia.getSubtareas())
            {
                respuesta += ",SUBTAREA,";
                respuesta += std::to_string(idSubtarea);
            }
        }

        return respuesta;
    }

    ////////////////////////////////////////////////////////
    // SET_PRIORIDAD_PLANNER
    ////////////////////////////////////////////////////////

    if(comando == "SET_PRIORIDAD_PLANNER")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden cambiar la prioridad";
        }

        if(datos.size() < 3)
        {   
            return "ERROR|Faltan parametros";
        }

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        std::string prioridadTexto = datos[2];

        PrioridadPlanner prioridad;

        if(prioridadTexto == "BAJA")
        {
            prioridad = PrioridadPlanner::BAJA;
        }
        else if(prioridadTexto == "MEDIA")
        {
            prioridad = PrioridadPlanner::MEDIA;
        }
        else if(prioridadTexto == "ALTA")
        {
            prioridad = PrioridadPlanner::ALTA;
        }
        else
        {
            return "ERROR|Prioridad invalida";
        }

        if(session.cambiarPrioridadPlanner(idTarea,prioridad))
        {
            return "PRIORIDAD_PLANNER_ACTUALIZADA";
        }

        return "ERROR|No se pudo actualizar la prioridad";
    }


    ////////////////////////////////////////////////////////
    // ADD_SUBTAREA_PLANNER
    ////////////////////////////////////////////////////////

    if(comando == "ADD_SUBTAREA_PLANNER")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden agregar subtareas al Planner";
        }

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idSubtarea;

        try
        {
            idSubtarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de subtarea invalido";
        }

        std::string fecha = datos[2];

        if(fecha.empty())
        {
            return "ERROR|Falta fecha";
        }

        if(session.agregarSubtareaFecha(idSubtarea, fecha))
        {
            return "SUBTAREA_AGREGADA_PLANNER";
        }

        return "ERROR|No se pudo agregar la subtarea al Planner";
    }

    ////////////////////////////////////////////////////////
    // DELETE_SUBTAREA_PLANNER
    ////////////////////////////////////////////////////////

    if(comando == "DELETE_SUBTAREA_PLANNER")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden modificar el Planner";
        }

        //==================================================
        // Validar parametros
        //==================================================

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        //==================================================
        // ID DE SUBTAREA
        //==================================================

        int idSubtarea;

        try
        {
            idSubtarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de subtarea invalido";
        }

        if(idSubtarea <= 0)
        {
            return "ERROR|ID de subtarea invalido";
        }

        //==================================================
        // FECHA
        //==================================================

        std::string fecha = datos[2];

        if(fecha.empty())
        {
            return "ERROR|Falta fecha";
        }

        //==================================================
        // ELIMINAR DEL PLANNER
        //==================================================

        if(session.eliminarSubtareaPlanner(idSubtarea,fecha))
        {
            return "SUBTAREA_ELIMINADA_PLANNER";
        }

        return "ERROR|No se pudo eliminar la subtarea del Planner";
    }

    ////////////////////////////////////////////////////////
    // UPDATE_ESTADO_TAREA
    ////////////////////////////////////////////////////////

    if(comando == "UPDATE_ESTADO_TAREA")
    {
        //==================================================
        // Verificar autenticación
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // Solo alumnos
        //==================================================

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden cambiar el estado de una tarea";
        }

        //==================================================
        // Validar parametros
        //==================================================

        if(datos.size() < 3)
        {
            return "ERROR|Faltan parametros";
        }

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        if(idTarea <= 0)
        {
            return "ERROR|ID de tarea invalido";
        }

        std::string estadoTexto = datos[2];

        //==================================================
        // Convertir estado
        //==================================================

        EstadoTarea estado;

        if(estadoTexto == "COMPLETADO")
        {
            estado = EstadoTarea::COMPLETADO;
        }
        else if(estadoTexto == "NO_COMPLETADO")
        {
            estado = EstadoTarea::NO_COMPLETADO;
        }
        else
        {
            return "ERROR|Estado de tarea invalido";
        }

        //==================================================
        // Cambiar estado
        //==================================================

        if(session.cambiarEstadoTarea(idTarea, estado))
        {
            return "ESTADO_TAREA_ACTUALIZADO";
        }

        return "ERROR|No se pudo actualizar el estado de la tarea";
    }

    ////////////////////////////////////////////////////////
    // GET_ESTADOS_TAREA
    ////////////////////////////////////////P////////////////


    if(comando == "GET_ESTADOS_TAREA")
    {   
        //==================================================
        // Verificar autenticación
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }   

        //==================================================
        // Validar parametros
        //==================================================

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de tarea";
        }

        //==================================================
        // Obtener ID de tarea
        //==================================================

        int idTarea;

        try
        {
            idTarea = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|ID de tarea invalido";
        }

        if(idTarea <= 0)
        {
            return "ERROR|ID de tarea invalido";
        }

        //==================================================
        // Obtener estados
        //==================================================

        std::vector<EstadoTareaAlumno> estados = session.obtenerEstadosTarea(idTarea);

        //==================================================
        // Construir respuesta
        //==================================================

        std::string respuesta = "ESTADOS_TAREA";

        for(const auto& estado : estados)
        {
            respuesta += "|";
            respuesta += std::to_string(estado.getTareaId());
            respuesta += ",";
            respuesta += std::to_string(estado.getAlumnoId());
            respuesta += ",";
            respuesta += Persistencia::estadoTareaAString(estado.getEstado());
        }

        return respuesta;
    }

    ////////////////////////////////////////////////////////
    // GET_ESTADOS_ALUMNO
    ////////////////////////////////////////////////////////

    if(comando == "GET_ESTADOS_ALUMNO")
    {
        
        //==================================================
        // Verificar autenticación
        //==================================================

        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        //==================================================
        // Solo alumnos
        //==================================================

        if(session.obtenerRol() != "Alumno")
        {
            return "ERROR|Solo los alumnos pueden consultar sus estados";
        }

        //==================================================
        // Validar parametros
        //==================================================

        if(datos.size() < 2)
        {
            return "ERROR|Falta estado";
        }

        std::string estadoTexto = datos[1];

        //==================================================
        // Convertir estado
        //==================================================

        EstadoTarea estado;

        if(estadoTexto == "COMPLETADO")
        {
            estado = EstadoTarea::COMPLETADO;
        }
        else if(estadoTexto == "NO_COMPLETADO")
        {
            estado = EstadoTarea::NO_COMPLETADO;
        }
        else
        {
            return "ERROR|Estado de tarea invalido";
        }

        //==================================================
        // Obtener estados del alumno
        //==================================================

        std::vector<EstadoTareaAlumno> estados = session.obtenerEstadosAlumno(estado);

        //==================================================
        // Construir respuesta
        //==================================================

        std::string respuesta = "ESTADOS_ALUMNO";

        for(const auto& estadoTarea : estados)
        {
            respuesta += "|";

            respuesta += std::to_string(estadoTarea.getTareaId());
        }

        return respuesta;
    }

    ////////////////////////////////////////////////////////////
    // GET_NOTIFICACIONES
    ////////////////////////////////////////////////////////////

    if(comando == "GET_NOTIFICACIONES")
    {
        std::vector<Notificacion> notificaciones = session.obtenerNotificaciones();
        std::ostringstream respuesta;

        respuesta << "OK";

        for(const auto& notificacion : notificaciones)
        {
            respuesta << "|"
                      << notificacion.getId()
                      << ";"
                      << static_cast<int>(notificacion.getTipo())
                      << ";"
                      << notificacion.getIdReferencia()
                      << ";"
                      << static_cast<int>(notificacion.getTipoReferencia())
                      << ";"
                      << notificacion.getTitulo()
                      << ";"
                      << notificacion.getMensaje()
                      << ";"
                      << notificacion.getFecha()
                      << ";"
                      << (notificacion.estaLeida() ? "1" : "0");
        }

        return respuesta.str();
    }


    ////////////////////////////////////////////////////////////
    // MARCAR_NOTIFICACION_LEIDA
    ////////////////////////////////////////////////////////////

    if(comando == "MARCAR_NOTIFICACION_LEIDA")
    {
        if(!session.estaAutenticado())
        {   
            return "NO_LOGIN";
        }

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de notificacion";
        }

        int idNotificacion;

        try
        {
            idNotificacion = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de notificacion debe ser un numero";
        }

        if(session.marcarNotificacionLeida(idNotificacion))
        {
            return "NOTIFICACION_MARCADA";
        }

        return "ERROR|No se pudo marcar la notificacion";
    }

    ////////////////////////////////////////////////////////////
    // DELETE_NOTIFICACION
    ////////////////////////////////////////////////////////////

    if(comando == "DELETE_NOTIFICACION")
    {
        if(!session.estaAutenticado())
        {
            return "NO_LOGIN";
        }

        if(datos.size() < 2)
        {
            return "ERROR|Falta ID de notificacion";
        }

        int idNotificacion;

        try
        {
            idNotificacion = std::stoi(datos[1]);
        }
        catch(...)
        {
            return "ERROR|El ID de notificacion debe ser un numero";
        }

        if(session.eliminarNotificacion(idNotificacion))
        {
            return "NOTIFICACION_ELIMINADA";
        }

        return "ERROR|No se pudo eliminar la notificacion";
    }

    
    
    //---------------------------------
    // COMANDO DESCONOCIDO
    //---------------------------------

    return "ERROR|Comando desconocido";
}
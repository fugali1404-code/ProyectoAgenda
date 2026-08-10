#include "commandprocessor.hpp"

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
    // AGREGAR MATERIA
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
    // OBTENER MATERIAS
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
    // ACTUALIZAR MATERIA
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
    // ELIMINAR MATERIA
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

    //---------------------------------
    // COMANDO DESCONOCIDO
    //---------------------------------

    return "ERROR|Comando desconocido";
}
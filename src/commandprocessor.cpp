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
            return "LOGIN_ERROR";

        if(session.login(
                datos[1],
                datos[2]))
        {
            return "LOGIN_OK";
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

    //---------------------------------
    // GET_USUARIO
    //---------------------------------

    if(comando == "GET_USUARIO")
    {
        if(session.estaAutenticado())
        {
            return "USUARIO|" +
                   session.obtenerUsuario();
        }

        return "NO_LOGIN";
    }

    //---------------------------------
    // ADD_MATERIA
    //---------------------------------

    if(comando == "ADD_MATERIA")
    {
        if(!session.estaAutenticado())
            return "NO_LOGIN";

        if(datos.size() < 2)
            return "ERROR";

        if(session.agregarMateria(datos[1]))
            return "MATERIA_AGREGADA";

        return "MATERIA_EXISTENTE";
    }

    //---------------------------------
    // GET_MATERIAS
    //---------------------------------

    if(comando == "GET_MATERIAS")
    {
    if(!session.estaAutenticado())
        return "NO_LOGIN";

    return session.obtenerMaterias();
    }


    //---------------------------------
    // COMANDO DESCONOCIDO
    //---------------------------------

    return "ERROR|Comando desconocido";
}
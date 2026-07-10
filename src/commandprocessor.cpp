#include "commandprocessor.hpp"

std::string CommandProcessor::procesar(
    const std::vector<std::string>& datos
)
{
    if(datos.empty())
        return "ERROR|Mensaje vacio";

    std::string comando = datos[0];

    if(comando == "LOGIN")
    {
        if(datos.size() < 3)
            return "LOGIN_ERROR";

        return "LOGIN_OK";
    }

    if(comando == "ADD_MATERIA")
    {
        return "MATERIA_AGREGADA";
    }

    if(comando == "ADD_TAREA")
    {
        return "TAREA_AGREGADA";
    }

    if(comando == "GET_PLANNER")
    {
        return "PLANNER_VACIO";
    }

    return "ERROR|Comando desconocido";
}
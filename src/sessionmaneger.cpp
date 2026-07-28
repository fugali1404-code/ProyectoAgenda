#include "sessionmaneger.hpp"

SessionManager::SessionManager()
{
    
}

bool SessionManager::login(
    const std::string& usuario,
    const std::string& password
)
{
    if(usuario=="juan" && password=="1234")
    {
        datos.setUsuario(usuario);
        datos.setAutenticado(true);
        return true;
    }

    if(usuario=="liliana" && password=="abcd")
    {
        datos.setUsuario(usuario);
        datos.setAutenticado(true);
        return true;
    }

    datos.limpiar();

    return false;
}

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

bool SessionManager::agregarMateria(
    const std::string& nombre
)
{
    if(!datos.estaAutenticado())
        return false;

    return datos.agregarMateria(nombre);
}


std::string SessionManager::obtenerMaterias() const
{
    if(!datos.estaAutenticado())
        return "NO_LOGIN";

    return datos.obtenerMaterias();
}


PlannerSemana&
SessionManager::obtenerPlanner()
{
    return datos.obtenerPlanner();
}


/* 
#include "Persistencia.hpp"

#include <fstream>

//Guardar tareas 
bool Persistencia::guardarTareas(
    const std::vector<Tarea>& tareas,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return false;

    for(const auto& tarea : tareas)
    {
        out
        << tarea.getId()
        << ";"
        << tarea.getTitulo()
        << ";"
        << tarea.getFechaEntrega()
        << "\n";
    }

    out.close();

    return true;
}

//Cargar tareas
bool Persistencia::cargarTareas(
    std::vector<Tarea>& tareas,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    tareas.clear();

    return true;
}

//Guardar materias
bool Persistencia::guardarMaterias(
    const std::vector<Materia>& materias,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return false;

    for(const auto& materia : materias)
    {
        out
        << materia.getNombre()
        << "\n";
    }

    return true;
}

//Cargar materias 
bool Persistencia::cargarMaterias(
    std::vector<Materia>& materias,
    const std::string& archivo)
{
    materias.clear();

    return true;
}

//usuarios 
bool Persistencia::guardarUsuarios(
    const std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    return true;
}

bool Persistencia::cargarUsuarios(
    std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    return true;
}





*/
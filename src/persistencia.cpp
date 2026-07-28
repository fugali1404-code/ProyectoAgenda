#include "persistencia.hpp"
#include "alumno.hpp"
#include "profesor.hpp"

#include <fstream>
#include <sstream>

///////////////////////////////////////////////////////////
// Conversión de Prioridad
///////////////////////////////////////////////////////////

std::string Persistencia::prioridadAString(
    Prioridad prioridad)
{
    switch (prioridad)
    {
        case Prioridad::BAJA:
            return "BAJA";

        case Prioridad::MEDIA:
            return "MEDIA";

        case Prioridad::ALTA:
            return "ALTA";
    }

    return "MEDIA";
}

Prioridad Persistencia::stringAPrioridad(
    const std::string& texto)
{
    if(texto == "BAJA")
        return Prioridad::BAJA;

    if(texto == "ALTA")
        return Prioridad::ALTA;

    return Prioridad::MEDIA;
}

///////////////////////////////////////////////////////////
// Guardar Tareas
///////////////////////////////////////////////////////////

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
        << tarea.getId() << "|"
        << tarea.getMateriaId()<< "|"
        << tarea.getTitulo() << "|"
        << tarea.getDescripcion() << "|"
        << tarea.getFechaEntrega() << "|"
        << prioridadAString(
                tarea.getPrioridad())
        << "|"
        << tarea.getCompletada()
        << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::cargarTareas(
    std::vector<Tarea>& tareas,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    tareas.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
            continue;

        std::stringstream ss(linea);

        std::string campo;

        Tarea tarea;

        // id
        std::getline(ss, campo, '|');
        tarea.setId(std::stoi(campo));

        // materiaId
        std::getline(ss, campo, '|');
        tarea.setMateriaId(std::stoi(campo));


        // titulo
        std::getline(ss, campo, '|');
        tarea.setTitulo(campo);

        // descripcion
        std::getline(ss, campo, '|');
        tarea.setDescripcion(campo);

        // fecha
        std::getline(ss, campo, '|');
        tarea.setFechaEntrega(campo);

        // prioridad
        std::getline(ss, campo, '|');
        tarea.setPrioridad(
            stringAPrioridad(campo));

        // completada
        std::getline(ss, campo, '|');

        tarea.setCompletada(
            campo == "1"
        );

        tareas.push_back(tarea);
    }

    in.close();

    return true;
}

///////////////////////////////////////////////////////////
// Guardar Materias
///////////////////////////////////////////////////////////

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
            << materia.getId() << "|"
            << materia.getNombre() << "|"
            << materia.getProfesorId()
            << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Materias
///////////////////////////////////////////////////////////

bool Persistencia::cargarMaterias(
    std::vector<Materia>& materias,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    materias.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
            continue;

        std::stringstream ss(linea);

        std::string campo;

        Materia materia;

        // id
        std::getline(ss, campo, '|');
        materia.setId(std::stoi(campo));

        // nombre
        std::getline(ss, campo, '|');
        materia.setNombre(campo);

        // profesor
        std::getline(ss, campo, '|');
        materia.setProfesorId(std::stoi(campo));

        materias.push_back(materia);
    }

    in.close();

    return true;
}

///////////////////////////////////////////////////////////
// Guardar Usuarios
///////////////////////////////////////////////////////////

bool Persistencia::guardarUsuarios(
    const std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return false;

    for(const auto usuario : usuarios)
    {
        if(usuario == nullptr)
            continue;

        out
            << usuario->getRol() << "|"
            << usuario->getId() << "|"
            << usuario->getNombre() << "|"
            << usuario->getCorreo() << "|"
            << usuario->getPassword()
            << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Usuarios
///////////////////////////////////////////////////////////

bool Persistencia::cargarUsuarios(
    std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    // Liberar memoria anterior
    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    usuarios.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
            continue;

        std::stringstream ss(linea);

        std::string rol;
        std::string campo;

        std::getline(ss, rol, '|');

        // id
        std::getline(ss, campo, '|');
        int id = std::stoi(campo);

        // nombre
        std::string nombre;
        std::getline(ss, nombre, '|');

        // correo
        std::string correo;
        std::getline(ss, correo, '|');

        // password
        std::string password;
        std::getline(ss, password);

        Usuario* usuario = nullptr;

        if(rol == "Alumno")
        {
            usuario = new Alumno(
                id,
                nombre,
                correo,
                password
            );
        }
        else if(rol == "Profesor")
        {
            usuario = new Profesor(
                id,
                nombre,
                correo,
                password
            );
        }

        if(usuario != nullptr)
        {
            usuarios.push_back(usuario);
        }
    }

    in.close();

    return true;
}

#include "sessiondata.hpp"
#include "persistencia.hpp"

SessionData::SessionData()
{
    autenticado = false;
    cargarDatos();
}

bool SessionData::estaAutenticado() const
{
    return autenticado;
}

void SessionData::setAutenticado(
    bool estado
)
{
    autenticado = estado;
}

std::string SessionData::obtenerUsuario() const
{
    return usuario;
}

void SessionData::setUsuario(
    const std::string& usuario
)
{
    this->usuario = usuario;
}

void SessionData::limpiar()
{
    autenticado = false;

    usuario.clear();

    materias.clear();

    planner = PlannerSemana();
}

bool SessionData::agregarMateria(
    const std::string& nombre)
{
    for(const auto& materia : materias)
    {
        if(materia.getNombre() == nombre)
            return false;
    }

    Materia nueva;

    int nuevoId = 1;

    for(const auto& materia : materias)
    {
        if(materia.getId() >= nuevoId)
        {
            nuevoId =
            materia.getId() + 1;
        }
    }

    nueva.setId(nuevoId);

    nueva.setNombre(nombre);

    nueva.setProfesorId(0);

    materias.push_back(nueva);

    guardarDatos();

    return true;
}

std::string SessionData::obtenerMaterias() const
{
    if(materias.empty())
        return "SIN_MATERIAS";

    std::string lista;

    for(const auto& materia : materias)
    {
        lista += materia.getNombre();
        lista += ";";
    }

    return lista;
}

PlannerSemana&
SessionData::obtenerPlanner()
{
    return planner;
}

std::vector<Materia>&
SessionData::obtenerVectorMaterias()
{
    return materias;
}


void SessionData::cargarDatos()
{
    Persistencia::cargarMaterias(
        materias,
        "materias.txt"
    );

    std::vector<Tarea> tareas;

    Persistencia::cargarTareas(
        tareas,
        "tareas.txt"
    );

    for(const auto& tarea : tareas)
    {
        for(auto& materia : materias)
        {
            if(materia.getId() ==
               tarea.getMateriaId())
            {
                materia.agregarTarea(
                    tarea
                );

                break;
            }
        }
    }
}


void SessionData::guardarDatos()
{
    Persistencia::guardarMaterias(
        materias,
        "materias.txt"
    );

    std::vector<Tarea> todasLasTareas;

    for(const auto& materia : materias)
    {
        for(const auto& tarea :
            materia.getTareas())
        {
            todasLasTareas.push_back(
                tarea
            );
        }
    }

    Persistencia::guardarTareas(
        todasLasTareas,
        "tareas.txt"
    );
}


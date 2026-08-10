#include "sessiondata.hpp"
#include "persistencia.hpp"

SessionData::SessionData()
{
    autenticado = false;
    usuarioId = -1;
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
    usuarioId = -1;
    usuario.clear();
    nombreCompleto.clear();
    identificador.clear();
    materias.clear();
    rol.clear();
    
}

bool SessionData::agregarMateria(
    const Materia& materia
)
{
    for(const auto& m : materias)
    {
        if(m.getNombre() == materia.getNombre())
        {
            return false;
        }
    }

    materias.push_back(materia);

    return true;
}

std::string SessionData::obtenerMaterias() const
{
    if(materias.empty())
    {
        return "SIN_MATERIAS";
    }

    std::string lista;

    for(const auto& materia : materias)
    {
        lista += std::to_string(materia.getId());
        lista += "|";

        lista += materia.getNombre();
        lista += "|";

        lista += std::to_string(materia.getProfesorId());

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


void SessionData::guardarDatos()
{
    Persistencia::guardarMaterias(
        usuarioId,
        materias,
        "materias.txt"
    );
}

void SessionData::setNombreCompleto(
    const std::string& nombre
)
{
    nombreCompleto = nombre;
}

std::string SessionData::obtenerNombreCompleto() const
{
    return nombreCompleto;
}

void SessionData::setIdentificador(
    const std::string& identificador
)
{
    this->identificador = identificador;
}

std::string SessionData::obtenerIdentificador() const
{
    return identificador;
}



const std::vector<Materia>&
SessionData::obtenerVectorMaterias() const
{
    return materias;
}

void SessionData::setUsuarioId(
    int id
)
{
    usuarioId = id;
}

int SessionData::obtenerUsuarioId() const
{
    return usuarioId;
}

void SessionData::setRol(
    const std::string& rol)
{
    this->rol = rol;
}

std::string SessionData::obtenerRol() const
{
    return rol;
}

void SessionData::setMaterias(
    const std::vector<Materia>& materias
)
{
    this->materias = materias;
}


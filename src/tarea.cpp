#include "tarea.hpp"

//=====================
// CONSTRUCTOR
//=====================

Tarea::Tarea()
{
    id = 0;
    materiaId = 0;
    titulo = "";
    descripcion = "";
    fechaEntrega = "";

    tipo = TipoTarea::TAREA;

    // 0 = ponderación general
    parcial = 0;

    // Se conserva como estado inicial.
    // Más adelante el completado real se manejará
    // por alumno.
    completada = false;
}

//=====================
// CONSTRUCTOR CON DATOS
//=====================

Tarea::Tarea(
    int id,
    int materiaId,
    const std::string& titulo,
    const std::string& descripcion,
    const std::string& fechaEntrega,
    TipoTarea tipo,
    int parcial
)
{
    this->id = id;
    this->materiaId = materiaId;
    this->titulo = titulo;
    this->descripcion = descripcion;
    this->fechaEntrega = fechaEntrega;
    this->tipo = tipo;

    // Evitamos que una tarea tenga un parcial negativo.
    if(parcial < 0)
    {
        this->parcial = 0;
    }
    else
    {
        this->parcial = parcial;
    }

    completada = false;
}

//=====================
// SUBTAREAS
//=====================

void Tarea::agregarSubtarea(
    const Subtarea& subtarea
)
{
    subtareas.push_back(subtarea);
}

//=====================
// COMPLETAR
//=====================

void Tarea::completar()
{
    completada = true;
}

//=====================
// GETTERS
//=====================

int Tarea::getId() const
{
    return id;
}

int Tarea::getMateriaId() const
{
    return materiaId;
}

std::string Tarea::getTitulo() const
{
    return titulo;
}

std::string Tarea::getDescripcion() const
{
    return descripcion;
}

std::string Tarea::getFechaEntrega() const
{
    return fechaEntrega;
}

TipoTarea Tarea::getTipo() const
{
    return tipo;
}

int Tarea::getParcial() const
{
    return parcial;
}

bool Tarea::estaCompletada() const
{
    return completada;
}

bool Tarea::getCompletada() const
{
    return completada;
}

std::vector<Subtarea>&
Tarea::getSubtareas()
{
    return subtareas;
}

const std::vector<Subtarea>&
Tarea::getSubtareas() const
{
    return subtareas;
}

//=====================
// SETTERS
//=====================

void Tarea::setId(
    int id
)
{
    this->id = id;
}

void Tarea::setMateriaId(
    int materiaId
)
{
    this->materiaId = materiaId;
}

void Tarea::setTitulo(
    const std::string& titulo
)
{
    this->titulo = titulo;
}

void Tarea::setDescripcion(
    const std::string& descripcion
)
{
    this->descripcion = descripcion;
}

void Tarea::setFechaEntrega(
    const std::string& fechaEntrega
)
{
    this->fechaEntrega = fechaEntrega;
}

void Tarea::setTipo(
    TipoTarea tipo
)
{
    this->tipo = tipo;
}

void Tarea::setParcial(
    int parcial
)
{
    if(parcial < 0)
    {
        this->parcial = 0;
    }
    else
    {
        this->parcial = parcial;
    }
}

void Tarea::setCompletada(
    bool completada
)
{
    this->completada = completada;
}

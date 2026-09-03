#include "ponderacion.hpp"

//=====================
// CONSTRUCTOR
//=====================

Ponderacion::Ponderacion()
{
    idMateria = 0;
    parcial = 0;

    tarea = 0.0;
    examen = 0.0;
    practica = 0.0;
    proyecto = 0.0;
    trabajo = 0.0;
    otro = 0.0;
}

//=====================
// CONSTRUCTOR CON DATOS
//=====================

Ponderacion::Ponderacion(
    int idMateria,
    int parcial,
    double tarea,
    double examen,
    double practica,
    double proyecto,
    double trabajo,
    double otro
)
{
    this->idMateria = idMateria;
    this->parcial = parcial;

    this->tarea = tarea;
    this->examen = examen;
    this->practica = practica;
    this->proyecto = proyecto;
    this->trabajo = trabajo;
    this->otro = otro;
}

//=====================
// GETTERS
//=====================

int Ponderacion::getIdMateria() const
{
    return idMateria;
}

int Ponderacion::getParcial() const
{
    return parcial;
}

double Ponderacion::getTarea() const
{
    return tarea;
}

double Ponderacion::getExamen() const
{
    return examen;
}

double Ponderacion::getPractica() const
{
    return practica;
}

double Ponderacion::getProyecto() const
{
    return proyecto;
}

double Ponderacion::getTrabajo() const
{
    return trabajo;
}

double Ponderacion::getOtro() const
{
    return otro;
}

//=====================
// SETTERS
//=====================

void Ponderacion::setIdMateria(
    int idMateria
)
{
    this->idMateria = idMateria;
}

void Ponderacion::setParcial(
    int parcial
)
{
    this->parcial = parcial;
}

void Ponderacion::setTarea(
    double tarea
)
{
    this->tarea = tarea;
}

void Ponderacion::setExamen(
    double examen
)
{
    this->examen = examen;
}

void Ponderacion::setPractica(
    double practica
)
{
    this->practica = practica;
}

void Ponderacion::setProyecto(
    double proyecto
)
{
    this->proyecto = proyecto;
}

void Ponderacion::setTrabajo(
    double trabajo
)
{
    this->trabajo = trabajo;
}

void Ponderacion::setOtro(
    double otro
)
{
    this->otro = otro;
}
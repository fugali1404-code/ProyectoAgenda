#include "calificaciones.hpp"

///////////////////////////////////////////////////////////
// Constructor por defecto
///////////////////////////////////////////////////////////

Calificacion::Calificacion()
{
    idAlumno = 0;
    idTarea = 0;
    calificacion = 0.0;
}

///////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////

Calificacion::Calificacion(
    int idAlumno,
    int idTarea,
    double calificacion
)
{
    this->idAlumno = idAlumno;
    this->idTarea = idTarea;
    this->calificacion = calificacion;
}

///////////////////////////////////////////////////////////
// GETTERS
///////////////////////////////////////////////////////////

int Calificacion::getIdAlumno() const
{
    return idAlumno;
}

int Calificacion::getIdTarea() const
{
    return idTarea;
}

double Calificacion::getCalificacion() const
{
    return calificacion;
}

///////////////////////////////////////////////////////////
// SETTERS
///////////////////////////////////////////////////////////

void Calificacion::setIdAlumno(
    int idAlumno
)
{
    this->idAlumno = idAlumno;
}

void Calificacion::setIdTarea(
    int idTarea
)
{
    this->idTarea = idTarea;
}

void Calificacion::setCalificacion(
    double calificacion
)
{
    this->calificacion = calificacion;
}
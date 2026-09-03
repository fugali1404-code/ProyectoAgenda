#pragma once

class Calificacion
{
private:

    int idAlumno;
    int idTarea;
    double calificacion;

public:

    Calificacion();

    Calificacion(
        int idAlumno,
        int idTarea,
        double calificacion
    );

    int getIdAlumno() const;

    int getIdTarea() const;

    double getCalificacion() const;

    void setIdAlumno(
        int idAlumno
    );

    void setIdTarea(
        int idTarea
    );

    void setCalificacion(
        double calificacion
    );
};
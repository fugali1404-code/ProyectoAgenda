#ifndef PONDERACION_HPP
#define PONDERACION_HPP

class Ponderacion
{
private:

    int idMateria;

    // 0 = ponderacion general
    // 1 = parcial 1
    // 2 = parcial 2
    // 3 = parcial 3
    int parcial;

    double tarea;
    double examen;
    double practica;
    double proyecto;
    double trabajo;
    double otro;

public:

    //=====================
    // CONSTRUCTORES
    //=====================

    Ponderacion();

    Ponderacion(
        int idMateria,
        int parcial,
        double tarea,
        double examen,
        double practica,
        double proyecto,
        double trabajo,
        double otro
    );

    //=====================
    // GETTERS
    //=====================

    int getIdMateria() const;

    int getParcial() const;

    double getTarea() const;

    double getExamen() const;

    double getPractica() const;

    double getProyecto() const;

    double getTrabajo() const;

    double getOtro() const;

    //=====================
    // SETTERS
    //=====================

    void setIdMateria(
        int idMateria
    );

    void setParcial(
        int parcial
    );

    void setTarea(
        double tarea
    );

    void setExamen(
        double examen
    );

    void setPractica(
        double practica
    );

    void setProyecto(
        double proyecto
    );

    void setTrabajo(
        double trabajo
    );

    void setOtro(
        double otro
    );
};

#endif
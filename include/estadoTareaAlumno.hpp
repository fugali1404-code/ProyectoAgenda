#ifndef ESTADO_TAREA_ALUMNO_HPP
#define ESTADO_TAREA_ALUMNO_HPP

// ============================================================
// ESTADO DE UNA TAREA PARA UN ALUMNO
// ============================================================

enum class EstadoTarea
{
    NO_COMPLETADO,
    COMPLETADO
};

// ============================================================
// RELACION TAREA - ALUMNO
// ============================================================

class EstadoTareaAlumno
{
private:

    int idTarea;
    int idAlumno;
    EstadoTarea estado;

public:

    //========================
    // CONSTRUCTORES
    //========================

    EstadoTareaAlumno();

    EstadoTareaAlumno(
        int idTarea,
        int idAlumno,
        EstadoTarea estado
    );

    //========================
    // GETTERS
    //========================


    int getTareaId() const;

    int getAlumnoId() const;

    EstadoTarea getEstado() const;


    //========================
    // SETTERS
    //========================

    void setEstado(
        EstadoTarea estado
    );

    
};

#endif
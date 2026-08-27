#ifndef SUBTAREA_HPP
#define SUBTAREA_HPP

#include <string>

enum class EstadoSubtarea
{
    PENDIENTE,
    EN_PROGRESO,
    COMPLETADA
};

class Subtarea
{
private:

    int id;
    int tareaId;
    int alumnoId;

    std::string descripcion;

    EstadoSubtarea estado;

public:

    Subtarea();

    Subtarea(
        int id,
        int tareaId,
        int alumnoId,
        const std::string& descripcion
    );

    //========================
    // GETTERS
    //========================

    int getId() const;

    int getTareaId() const;

    int getAlumnoId() const;

    std::string getDescripcion() const;

    EstadoSubtarea getEstado() const;

    //========================
    // SETTERS
    //========================

    void setDescripcion(
        const std::string& descripcion
    );

    void setEstado(
        EstadoSubtarea estado
    );
};

#endif
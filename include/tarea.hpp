#ifndef TAREA_HPP
#define TAREA_HPP

#include <string>
#include <vector>

#include "Subtarea.hpp"

enum class Prioridad
{
    BAJA,
    MEDIA,
    ALTA
};

class Tarea
{
private:

    int id;

    std::string titulo;
    std::string descripcion;

    std::string fechaEntrega;

    bool completada;

    Prioridad prioridad;

    std::vector<Subtarea> subtareas;

public:

    Tarea();

    Tarea(
        int id,
        const std::string& titulo,
        const std::string& descripcion,
        const std::string& fechaEntrega,
        Prioridad prioridad
    );

    void agregarSubtarea(
        const Subtarea& subtarea
    );

    void completar();

    bool estaCompletada() const;

    int getId() const;

    std::string getTitulo() const;

    std::string getFechaEntrega() const;

    Prioridad getPrioridad() const;

    std::vector<Subtarea>&
    getSubtareas();
};

#endif
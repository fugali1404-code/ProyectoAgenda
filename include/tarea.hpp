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
    int materiaId;
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
        int materiaId,
        const std::string& titulo,
        const std::string& descripcion,
        const std::string& fechaEntrega,
        Prioridad prioridad
    );

    void agregarSubtarea(
        const Subtarea& subtarea
    );

    void completar();

    //=====================
    // GETTERS
    //=====================

    bool estaCompletada() const;
    bool getCompletada() const;
    int getId() const;
    int getMateriaId() const;
    std::string getTitulo() const;
    std::string getDescripcion() const;
    std::string getFechaEntrega() const;
    Prioridad getPrioridad() const;
    std::vector<Subtarea>&
    getSubtareas();
    const std::vector<Subtarea>&
    getSubtareas() const;


    //=====================
    // SETTERS
    //=====================

    void setId(
        int id
    );

    void setMateriaId(
    int materiaId
    );

    void setTitulo(
        const std::string& titulo
    );

    void setDescripcion(
        const std::string& descripcion
    );

    void setFechaEntrega(
        const std::string& fechaEntrega
    );

    void setPrioridad(
        Prioridad prioridad
    );

    void setCompletada(
        bool completada
    );



};

#endif
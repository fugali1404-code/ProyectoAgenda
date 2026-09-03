#ifndef TAREA_HPP
#define TAREA_HPP

#include <string>
#include <vector>

#include "Subtarea.hpp"

enum class TipoTarea
{
    TAREA,
    EXAMEN,
    PRACTICA,
    PROYECTO,
    TRABAJO,
    OTRO
};

class Tarea
{
private:

    int id;
    int materiaId;
    std::string titulo;
    std::string descripcion;
    std::string fechaEntrega;
    TipoTarea tipo;

    //==================================================
    // PARCIAL
    //
    // 0 = Ponderación general
    // 1 = Parcial 1
    // 2 = Parcial 2
    // etc.
    //==================================================

    int parcial;

    // Se conserva por compatibilidad con el modelo actual.
    // El estado de completado por alumno se manejará
    // posteriormente mediante una persistencia independiente.
    bool completada;

    std::vector<Subtarea> subtareas;

public:

    //=====================
    // CONSTRUCTORES
    //=====================

    Tarea();

    Tarea(
        int id,
        int materiaId,
        const std::string& titulo,
        const std::string& descripcion,
        const std::string& fechaEntrega,
        TipoTarea tipo,
        int parcial
    );

    //=====================
    // SUBTAREAS
    //=====================

    void agregarSubtarea(
        const Subtarea& subtarea
    );

    //=====================
    // COMPLETAR
    //=====================

    void completar();

    //=====================
    // GETTERS
    //=====================

    int getId() const;

    int getMateriaId() const;

    std::string getTitulo() const;

    std::string getDescripcion() const;

    std::string getFechaEntrega() const;

    TipoTarea getTipo() const;

    int getParcial() const;

    bool estaCompletada() const;

    bool getCompletada() const;

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

    void setTipo(
        TipoTarea tipo
    );

    void setParcial(
        int parcial
    );

    void setCompletada(
        bool completada
    );
};

#endif


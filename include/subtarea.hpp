#ifndef SUBTAREA_HPP
#define SUBTAREA_HPP

#include <string>

class Subtarea
{
private:

    int id;
    std::string descripcion;
    bool completada;

public:

    Subtarea();

    Subtarea(
        int id,
        const std::string& descripcion
    );

    void completar();

    bool estaCompletada() const;

    std::string getDescripcion() const;
};

#endif
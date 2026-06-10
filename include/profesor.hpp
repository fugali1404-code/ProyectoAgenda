#ifndef PROFESOR_HPP
#define PROFESOR_HPP

#include "Usuario.hpp"

class Profesor : public Usuario
{
public:

    Profesor();

    Profesor(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password
    );

    std::string getRol() const override;
};

#endif
#ifndef ALUMNO_HPP
#define ALUMNO_HPP

#include "Usuario.hpp"

class Alumno : public Usuario
{
public:

    Alumno();

    Alumno(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password
    );

    std::string getRol() const override;
};

#endif
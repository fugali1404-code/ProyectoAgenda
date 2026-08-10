#ifndef ADMINISTRADOR_HPP
#define ADMINISTRADOR_HPP

#include "usuario.hpp"

class Administrador : public Usuario
{
public:

    Administrador();

    Administrador(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    std::string getRol() const override;
};

#endif
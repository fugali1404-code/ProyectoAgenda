#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

class Usuario
{
protected:

    int id;
    std::string nombre;
    std::string correo;
    std::string password;

public:

    Usuario();
    Usuario(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password
    );

    virtual ~Usuario();

    int getId() const;
    std::string getNombre() const;
    std::string getCorreo() const;

    void setNombre(const std::string& nombre);
    void setCorreo(const std::string& correo);

    virtual std::string getRol() const = 0;
};

#endif
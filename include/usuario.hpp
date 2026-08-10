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
    std::string identificador;

public:

    Usuario();
    Usuario(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    virtual ~Usuario();

    int getId() const;
    std::string getNombre() const;
    std::string getCorreo() const;
    std::string getPassword() const;
    std::string getIdentificador() const;

    void setNombre(const std::string& nombre);
    void setCorreo(const std::string& correo);
    void setIdentificador(const std::string& identificador);
    void setId(int id);

    void setPassword(
        const std::string& password
    );

    

    virtual std::string getRol() const = 0;

    
    
};

#endif
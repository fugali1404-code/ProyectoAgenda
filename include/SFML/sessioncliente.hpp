#ifndef SESSIONCLIENT_HPP
#define SESSIONCLIENT_HPP

#include <string>

class SessionClient
{
private:

    //---------------------------------
    // Estado de la sesión
    //---------------------------------

    bool autenticado;

    //---------------------------------
    // Datos del usuario
    //---------------------------------

    std::string nombre;

    std::string rol;

    std::string identificador;

public:

    //---------------------------------
    // Constructor
    //---------------------------------

    SessionClient();

    //---------------------------------
    // Iniciar sesión
    //---------------------------------

    void iniciarSesion(
        const std::string& nombreUsuario,
        const std::string& rolUsuario,
        const std::string& identificadorUsuario
    );

    //---------------------------------
    // Cerrar sesión
    //---------------------------------

    void cerrarSesion();

    //---------------------------------
    // Estado
    //---------------------------------

    bool estaAutenticado() const;

    //---------------------------------
    // Getters
    //---------------------------------

    const std::string& obtenerNombre() const;

    const std::string& obtenerRol() const;

    const std::string& obtenerIdentificador() const;
};

#endif
#include "SFML/sessioncliente.hpp"

SessionClient::SessionClient()
{
    autenticado = false;

    nombre = "";

    rol = "";

    identificador = "";
}

void SessionClient::iniciarSesion(
    const std::string& nombreUsuario,
    const std::string& rolUsuario,
    const std::string& identificadorUsuario
)
{
    nombre = nombreUsuario;

    rol = rolUsuario;

    identificador = identificadorUsuario;

    autenticado = true;
}

void SessionClient::cerrarSesion()
{
    autenticado = false;

    nombre = "";

    rol = "";

    identificador = "";
}

bool SessionClient::estaAutenticado() const
{
    return autenticado;
}

const std::string&
SessionClient::obtenerNombre() const
{
    return nombre;
}

const std::string&
SessionClient::obtenerRol() const
{
    return rol;
}

const std::string&
SessionClient::obtenerIdentificador() const
{
    return identificador;
}
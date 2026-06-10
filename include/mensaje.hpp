#ifndef MENSAJE_HPP
#define MENSAJE_HPP

#include <string>

enum class TipoMensaje
{
    LOGIN,
    LOGOUT,

    CREAR_TAREA,
    ACTUALIZAR_TAREA,

    NOTIFICACION,

    CHAT,

    ERROR
};

class Mensaje
{
private:

    TipoMensaje tipo;
    std::string contenido;

public:

    Mensaje();

    Mensaje(
        TipoMensaje tipo,
        const std::string& contenido
    );

    TipoMensaje getTipo() const;

    std::string getContenido() const;
};

#endif
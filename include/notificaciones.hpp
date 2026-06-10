#ifndef NOTIFICACION_HPP
#define NOTIFICACION_HPP

#include <string>

enum class TipoNotificacion
{
    RECORDATORIO,
    NUEVA_TAREA,
    CAMBIO_FECHA,
    MENSAJE_PROFESOR,
    SISTEMA
};

class Notificacion
{
private:

    int id;

    TipoNotificacion tipo;

    std::string titulo;

    std::string mensaje;

    std::string fecha;

    bool leida;

public:

    Notificacion();

    Notificacion(
        int id,
        TipoNotificacion tipo,
        const std::string& titulo,
        const std::string& mensaje,
        const std::string& fecha
    );

    int getId() const;

    TipoNotificacion getTipo() const;

    std::string getTitulo() const;

    std::string getMensaje() const;

    std::string getFecha() const;

    bool estaLeida() const;

    void marcarComoLeida();
};

#endif
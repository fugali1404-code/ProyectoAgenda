#ifndef NOTIFICACIONES_CONTROLLER_HPP
#define NOTIFICACIONES_CONTROLLER_HPP

#include <vector>
#include <string>

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"
#include "notificaciones.hpp"

class NotificacionesController
{
private:

    NetworkManager& network;
    SessionClient& session;

    std::vector<Notificacion> notificaciones;

    bool convertirNotificaciones(
        const std::string& respuesta,
        std::vector<Notificacion>& resultado
    ) const;

public:

    NotificacionesController(
        NetworkManager& network,
        SessionClient& session
    );

    bool cargarNotificaciones();

    const std::vector<Notificacion>&
    obtenerNotificaciones() const;

    bool marcarComoLeida(
        int idNotificacion
    );

    bool eliminarNotificacion(
        int idNotificacion
    );

    bool recargar();
};

#endif
#include "controllers/notificacionesController.hpp"

#include <iostream>

#include "protocolo.hpp"

NotificacionesController::NotificacionesController(
    NetworkManager& network,
    SessionClient& session
)
    : network(network),
      session(session)
{
}

// ============================================================
// CONVERTIR NOTIFICACIONES
// ============================================================

bool NotificacionesController::convertirNotificaciones(
    const std::string& respuesta,
    std::vector<Notificacion>& resultado
) const
{
    resultado.clear();

    std::vector<std::string> datos =
        Protocol::dividir(respuesta);

    if(datos.empty())
    {
        return false;
    }

    if(datos[0] == "NO_LOGIN")
    {
        return false;
    }

    if(datos[0] != "OK")
    {
        return false;
    }

    for(size_t i = 1; i < datos.size(); ++i)
    {
        std::vector<std::string> campos =
            Protocol::dividir(datos[i], ';');

        if(campos.size() < 8)
        {
            continue;
        }

        try
        {
            int id =
                std::stoi(campos[0]);

            int tipo =
                std::stoi(campos[1]);

            int idReferencia =
                std::stoi(campos[2]);

            int tipoReferencia =
                std::stoi(campos[3]);

            std::string titulo =
                campos[4];

            std::string mensaje =
                campos[5];

            std::string fecha =
                campos[6];

            bool leida =
                campos[7] == "1";

            if(id <= 0)
            {
                continue;
            }

            Notificacion notificacion(
                id,
                0,
                static_cast<TipoNotificacion>(tipo),
                idReferencia,
                static_cast<TipoReferenciaNotificacion>(
                    tipoReferencia
                ),
                titulo,
                mensaje,
                fecha
            );

            if(leida)
            {
                notificacion.marcarComoLeida();
            }

            resultado.push_back(
                notificacion
            );
        }
        catch(...)
        {
            continue;
        }
    }

    return true;
}

// ============================================================
// CARGAR NOTIFICACIONES
// ============================================================

bool NotificacionesController::cargarNotificaciones()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    notificaciones.clear();

    std::string respuesta =
        network.enviarComando(
            "GET_NOTIFICACIONES"
        );

    return convertirNotificaciones(
        respuesta,
        notificaciones
    );
}

// ============================================================
// OBTENER NOTIFICACIONES
// ============================================================

const std::vector<Notificacion>&
NotificacionesController::obtenerNotificaciones() const
{
    return notificaciones;
}

// ============================================================
// MARCAR COMO LEÍDA
// ============================================================

bool NotificacionesController::marcarComoLeida(
    int idNotificacion
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(idNotificacion <= 0)
    {
        return false;
    }

    std::string comando =
        "MARCAR_NOTIFICACION_LEIDA|" +
        std::to_string(idNotificacion);

    std::string respuesta =
        network.enviarComando(comando);

    if(respuesta != "NOTIFICACION_MARCADA")
    {
        return false;
    }

    for(auto& notificacion : notificaciones)
    {
        if(notificacion.getId() == idNotificacion)
        {
            notificacion.marcarComoLeida();
            break;
        }
    }

    return true;
}

// ============================================================
// ELIMINAR NOTIFICACIÓN
// ============================================================

bool NotificacionesController::eliminarNotificacion(
    int idNotificacion
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(idNotificacion <= 0)
    {
        return false;
    }

    std::string comando =
        "DELETE_NOTIFICACION|" +
        std::to_string(idNotificacion);

    std::string respuesta =
        network.enviarComando(comando);

    if(respuesta != "NOTIFICACION_ELIMINADA")
    {
        return false;
    }

    for(auto it = notificaciones.begin();
        it != notificaciones.end();
        ++it)
    {
        if(it->getId() == idNotificacion)
        {
            notificaciones.erase(it);
            break;
        }
    }

    return true;
}

// ============================================================
// RECARGAR
// ============================================================

bool NotificacionesController::recargar()
{
    return cargarNotificaciones();
}
#include "networkmanager.hpp"

NetworkManager::NetworkManager()
{
    conectado = false;
}

bool NetworkManager::conectar(
    const std::string& ip,
    int puerto
)
{
    conectado = cliente.conectar(ip, puerto);

    return conectado;
}

bool NetworkManager::enviarMensaje(
    const std::string& mensaje
)
{
    if(!conectado)
        return false;

    return cliente.enviar(mensaje);
}

std::string NetworkManager::recibirMensaje()
{
    if(!conectado)
        return "";

    return cliente.recibir();
}

void NetworkManager::desconectar()
{
    if(conectado)
    {
        cliente.desconectar();
        conectado = false;
    }
}

bool NetworkManager::estaConectado() const
{
    return conectado;
}

bool NetworkManager::enviarYRecibir(
    const std::string& mensaje,
    std::string& respuesta
)
{
    if(!conectado)
        return false;

    if(!cliente.enviar(mensaje))
        return false;

    respuesta = cliente.recibir();

    return !respuesta.empty();
}

std::string NetworkManager::enviarComando(
    const std::string& comando
)
{
    if(!conectado)
    {
        return "NO_CONECTADO";
    }

    std::string respuesta;

    if(!enviarYRecibir(
            comando,
            respuesta))
    {
        return "ERROR_RED";
    }

    return respuesta;
}
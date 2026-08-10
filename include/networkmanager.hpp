#pragma once

#include "cliente.hpp"

#include <string>

class NetworkManager
{
private:

    Cliente cliente;

    bool conectado;

public:

    NetworkManager();

    bool conectar(
        const std::string& ip,
        int puerto
    );

    bool enviarMensaje(
        const std::string& mensaje
    );

    std::string recibirMensaje();

    bool enviarYRecibir(
        const std::string& mensaje,
        std::string& respuesta
    );

    std::string enviarComando(
      const std::string& comando
    );

    void desconectar();

    bool estaConectado() const;
};
#pragma once

#include "cliente.hpp"

class NetworkManager
{
private:

    Cliente cliente;

public:

    bool conectar(
        const std::string& ip,
        int puerto
    );

    void enviarMensaje(
        const std::string& msg
    );
};

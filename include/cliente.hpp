#pragma once

#include <string>

class Cliente
{
private:

    int socketCliente;

public:

    Cliente();

    bool conectar(
        const std::string& ip,
        int puerto
    );

    bool enviar(
        const std::string& mensaje
    );

    std::string recibir();

    void desconectar();
};
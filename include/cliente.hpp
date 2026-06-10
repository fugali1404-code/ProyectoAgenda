#ifndef CLIENTE_HPP
#define CLIENTE_HPP

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

    void enviar(
        const std::string& mensaje
    );

    std::string recibir();

    void desconectar();
};

#endif
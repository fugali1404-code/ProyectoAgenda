#pragma once

#include <vector>
#include <string>

class Servidor
{
private:

    int serverSocket;

public:

    Servidor();

    bool iniciar(int puerto);

    int aceptarCliente();

    bool recibirMensaje(
        int clienteSocket,
        std::string& mensaje
    );
    
    bool enviarMensaje(
    int clienteSocket,
    const std::string& mensaje
    );

    void cerrar();
};
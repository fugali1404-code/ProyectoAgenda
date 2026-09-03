#pragma once

#include <vector>
#include <string>

class Servidor
{
private:

    int serverSocket;
    bool activo;

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

    bool estaActivo() const;

    void detener();

    void cerrar();
};
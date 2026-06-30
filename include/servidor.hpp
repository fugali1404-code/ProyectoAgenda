#pragma once

#include <vector>
#include <string>

class Servidor
{
private:

    int serverSocket;

public:

    Servidor();

    bool iniciar(
        int puerto
    );

    int aceptarCliente();

    void cerrar();
};
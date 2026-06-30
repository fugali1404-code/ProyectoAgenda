#include "servidor.hpp"

#include <iostream>

int main()
{
    Servidor servidor;

    if (!servidor.iniciar(54000))
    {
        std::cerr << "Error: no se pudo iniciar el servidor.\n";
        return 1;
    }

    std::cout << "Servidor iniciado en el puerto 54000.\n";
    std::cout << "Esperando cliente...\n";

    int socketCliente = servidor.aceptarCliente();

    if (socketCliente < 0)
    {
        std::cerr << "Error: no se pudo aceptar al cliente.\n";
        servidor.cerrar();
        return 1;
    }

    std::cout << "Cliente conectado correctamente.\n";

    servidor.cerrar();

    return 0;
}
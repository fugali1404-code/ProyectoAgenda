#include "networkmanager.hpp"

#include <iostream>
#include <string>

int main()
{
    NetworkManager network;

    std::cout << "=================================\n";
    std::cout << " PRUEBA DE NETWORK MANAGER\n";
    std::cout << "=================================\n\n";

    std::cout << "Conectando al servidor...\n";

    if (!network.conectar("127.0.0.1", 54000))
    {
        std::cout << "No se pudo conectar al servidor.\n";
        return 1;
    }

    std::cout << "Conexion establecida.\n\n";

    std::string mensaje;
    std::string respuesta;

    while (true)
    {
        std::cout << "---------------------------------\n";
        std::cout << "Escribe un mensaje para enviar.\n";
        std::cout << "Escribe SALIR para terminar.\n\n";

        std::getline(std::cin, mensaje);

        if (mensaje == "SALIR")
            break;

        if (network.enviarYRecibir(mensaje, respuesta))
        {
            std::cout << "\nRespuesta del servidor:\n";
            std::cout << respuesta << "\n\n";
        }
        else
        {
            std::cout << "\nNo hubo respuesta del servidor.\n\n";
        }
    }

    network.desconectar();

    std::cout << "\nConexion cerrada.\n";

    return 0;
}
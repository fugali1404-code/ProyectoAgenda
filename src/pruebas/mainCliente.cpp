#include "cliente.hpp"

#include <iostream>

int main()
{
    std::cout
        << "Iniciando cliente...\n";

    Cliente cliente;

    std::cout
        << "Intentando conectar...\n";

    if (!cliente.conectar("127.0.0.1", 54000))
    {
        std::cout
            << "No se pudo conectar.\n";

        return 1;
    }

    std::cout
        << "Conexion exitosa.\n";

    if (cliente.enviar("Hola servidor"))
    {
        std::cout
            << "Mensaje enviado correctamente.\n";
    }
    else
    {
        std::cout
            << "Error al enviar mensaje.\n";
    }

    cliente.desconectar();

    std::cout
        << "Cliente finalizado.\n";

    return 0;
}
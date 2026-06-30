#include "cliente.hpp"

#include <iostream>

int main()
{
    std::cout << "Iniciando cliente...\n";

    Cliente cliente;

    std::cout << "Intentando conectar a 127.0.0.1:54000...\n";

    if (!cliente.conectar("127.0.0.1", 54000))
    {
        std::cerr << "Error: no se pudo conectar al servidor.\n";
        std::cin.get();
        return 1;
    }

    std::cout << "Conexion exitosa con el servidor.\n";

    cliente.desconectar();

    std::cout << "Cliente finalizado.\n";
    std::cin.get();

    return 0;
}
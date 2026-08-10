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

    std::string mensaje;   
    std::string respuesta; 

/*

    std::cout << network.enviarComando("LOGIN|admin@agenda.com|admin123") << std::endl;


    std::cout << "\n===== USUARIOS =====\n";
    std::cout << network.enviarComando("GET_USUARIOS") << std::endl;

    std::cout<< "\n===== AGREGANDO =====\n";
    std::cout<< network.enviarComando("ADD_USUARIO|Alumno|Carlos Ruiz|carlos@gmail.com|123456")
    << std::endl;

    std::cout << "\n===== USUARIOS =====\n";
    std::cout << network.enviarComando("GET_USUARIOS")<< std::endl;

    std::cout<< network.enviarComando("UPDATE_USUARIO|3|Carlos Alberto|carlos@gmail.com")
    << std::endl;   

    std::cout << network.enviarComando( "DELETE_USUARIO|3") << std::endl;

    */

    std::cout << "Conexion establecida.\n\n";

    

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
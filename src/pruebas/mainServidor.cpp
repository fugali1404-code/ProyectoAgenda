#include "servidor.hpp"
#include "protocolo.hpp"
#include "commandprocessor.hpp"

#include <iostream>

int main()
{
    Servidor servidor;

    if(!servidor.iniciar(54000))
    {
        std::cout
            << "Error al iniciar servidor.\n";

        return 1;
    }

    std::cout
        << "Servidor iniciado.\n";

    std::cout
        << "Esperando cliente...\n";

    int cliente =
        servidor.aceptarCliente();

    if(cliente < 0)
    {
        servidor.cerrar();
        return 1;
    }

    std::cout
        << "Cliente conectado.\n";

    std::string mensaje;

    if(
        servidor.recibirMensaje(
            cliente,
            mensaje
        )
    )
    {
        std::cout
            << "\nMensaje recibido:\n"
            << mensaje
            << "\n";

        auto datos =
            Protocol::dividir(
                mensaje
            );

        std::string respuesta =
            CommandProcessor::procesar(
                datos
            );

        std::cout
            << "\nRespuesta:\n"
            << respuesta
            << "\n";

        servidor.enviarMensaje(
            cliente,
            respuesta
        );
    }

    servidor.cerrar();

    return 0;
}
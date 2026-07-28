#include "servidor.hpp"
#include "protocolo.hpp"
#include "commandprocessor.hpp"
#include "sessionmaneger.hpp"

#include <iostream>
#include <string>

int main()
{
    Servidor servidor;

    if (!servidor.iniciar(54000))
    {
        std::cout << "Error al iniciar servidor.\n";
        return 1;
    }

    std::cout << "Servidor iniciado.\n";
    std::cout << "Esperando cliente...\n";

    int cliente = servidor.aceptarCliente();

    if (cliente < 0)
    {
        servidor.cerrar();
        return 1;
    }

    std::cout << "Cliente conectado.\n";

    SessionManager session;

    while (true)
    {
        std::string mensaje;

        if (!servidor.recibirMensaje(cliente, mensaje))
        {
            std::cout << "\nCliente desconectado.\n";
            break;
        }

        std::cout << "\n=============================\n";
        std::cout << "Mensaje recibido:\n";
        std::cout << mensaje << std::endl;

        auto datos = Protocol::dividir(mensaje);

        std::string respuesta =
            CommandProcessor::procesar(
                datos,
                session
            );

        std::cout << "\nRespuesta:\n";
        std::cout << respuesta << std::endl;

        if (!servidor.enviarMensaje(cliente, respuesta))
        {
            std::cout << "Error al enviar respuesta.\n";
            break;
        }
    }

    servidor.cerrar();

    std::cout << "\nServidor finalizado.\n";

    return 0;
}
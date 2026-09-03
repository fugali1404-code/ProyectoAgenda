#include "servidor.hpp"
#include "protocolo.hpp"
#include "commandprocessor.hpp"
#include "sessionmaneger.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <mutex>


////////////////////////////////////////////////////////////
// Atender un cliente
////////////////////////////////////////////////////////////

void atenderCliente(
    Servidor& servidor,
    int cliente
)
{
    SessionManager session;

    while(true)
    {
        std::string mensaje;

        ////////////////////////////////////////////////////////////
        // Recibir mensaje
        ////////////////////////////////////////////////////////////

        if(!servidor.recibirMensaje(
            cliente,
            mensaje
        ))
        {
            std::cout
                << "\nCliente desconectado."
                << std::endl;

            break;
        }


        ////////////////////////////////////////////////////////////
        // Mostrar mensaje recibido
        ////////////////////////////////////////////////////////////

        std::cout
            << "\n============================="
            << std::endl;

        std::cout
            << "Mensaje recibido:"
            << std::endl;

        std::cout
            << mensaje
            << std::endl;


        ////////////////////////////////////////////////////////////
        // Dividir comando
        ////////////////////////////////////////////////////////////

        auto datos = Protocol::dividir(mensaje);


        ////////////////////////////////////////////////////////////
        // Procesar comando
        ////////////////////////////////////////////////////////////

        std::string respuesta =
            CommandProcessor::procesar(
                datos,
                session
            );


        ////////////////////////////////////////////////////////////
        // Mostrar respuesta
        ////////////////////////////////////////////////////////////

        std::cout
            << "\nRespuesta:"
            << std::endl;

        std::cout
            << respuesta
            << std::endl;


        ////////////////////////////////////////////////////////////
        // Enviar respuesta
        ////////////////////////////////////////////////////////////

        if(!servidor.enviarMensaje(
            cliente,
            respuesta
        ))
        {
            std::cout
                << "Error al enviar respuesta."
                << std::endl;

            break;
        }
    }


    ////////////////////////////////////////////////////////////
    // Cliente terminó
    ////////////////////////////////////////////////////////////

    std::cout
        << "Hilo del cliente terminado."
        << std::endl;
}


////////////////////////////////////////////////////////////
// Hilo de consola
////////////////////////////////////////////////////////////

void controlarServidor(
    Servidor& servidor,
    std::atomic<bool>& servidorActivo,
    std::condition_variable& condicion
)
{
    std::string comando;

    while(servidor.estaActivo())
    {
        std::getline(
            std::cin,
            comando
        );

        if(!servidor.estaActivo())
        {
            break;
        }


        ////////////////////////////////////////////////////////////
        // APAGAR SERVIDOR
        ////////////////////////////////////////////////////////////

        if(comando == "APAGAR")
        {
            std::cout
                << "\nSolicitud de apagado recibida."
                << std::endl;


            ////////////////////////////////////////////////////////
            // Indicar que los hilos deben terminar
            ////////////////////////////////////////////////////////

            servidorActivo = false;


            ////////////////////////////////////////////////////////
            // Despertar hilo de notificaciones
            ////////////////////////////////////////////////////////

            condicion.notify_all();


            ////////////////////////////////////////////////////////
            // Detener servidor
            ////////////////////////////////////////////////////////

            servidor.detener();

            break;
        }
    }
}


////////////////////////////////////////////////////////////
// Hilo de recordatorios
////////////////////////////////////////////////////////////

void revisarNotificaciones(
    std::atomic<bool>& servidorActivo,
    std::condition_variable& condicion,
    std::mutex& mutexCondicion
)
{
    SessionManager session;


    ////////////////////////////////////////////////////////////
    // PRIMERA EJECUCIÓN INMEDIATA
    ////////////////////////////////////////////////////////////

    session.generarRecordatorios();

    std::cout
        << "\n[NOTIFICACIONES] Se revisaron los recordatorios."
        << std::endl;


    ////////////////////////////////////////////////////////////
    // REVISIONES CADA HORA
    ////////////////////////////////////////////////////////////

    while(servidorActivo)
    {
        std::unique_lock<std::mutex> lock(
            mutexCondicion
        );


        ////////////////////////////////////////////////////////
        // Esperar una hora
        //
        // Si el servidor se apaga antes,
        // el hilo despierta inmediatamente.
        ////////////////////////////////////////////////////////

        bool apagar =
            condicion.wait_for(
                lock,
                std::chrono::hours(1),
                [&servidorActivo]()
                {
                    return !servidorActivo.load();
                }
            );


        ////////////////////////////////////////////////////////
        // Servidor apagado
        ////////////////////////////////////////////////////////

        if(apagar || !servidorActivo)
        {
            break;
        }


        ////////////////////////////////////////////////////////
        // Liberar mutex antes de revisar
        ////////////////////////////////////////////////////////

        lock.unlock();


        ////////////////////////////////////////////////////////
        // Ejecutar revisión
        ////////////////////////////////////////////////////////

        session.generarRecordatorios();

        std::cout
            << "\n[NOTIFICACIONES] Se revisaron los recordatorios."
            << std::endl;
    }


    ////////////////////////////////////////////////////////////
    // Hilo terminado
    ////////////////////////////////////////////////////////////

    std::cout
        << "\n[NOTIFICACIONES] Hilo de recordatorios terminado."
        << std::endl;
}


////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////

int main()
{
    ////////////////////////////////////////////////////////////
    // Crear servidor
    ////////////////////////////////////////////////////////////

    Servidor servidor;


    ////////////////////////////////////////////////////////////
    // CONTROL DEL HILO DE NOTIFICACIONES
    ////////////////////////////////////////////////////////////

    std::atomic<bool> servidorActivo(true);

    std::condition_variable condicionNotificaciones;

    std::mutex mutexNotificaciones;


    ////////////////////////////////////////////////////////////
    // Iniciar servidor
    ////////////////////////////////////////////////////////////

    if(!servidor.iniciar(54000))
    {
        std::cout
            << "Error al iniciar servidor."
            << std::endl;

        return 1;
    }


    ////////////////////////////////////////////////////////////
    // Mensajes iniciales
    ////////////////////////////////////////////////////////////

    std::cout
        << "================================="
        << std::endl;

    std::cout
        << "       SERVIDOR AGENDA"
        << std::endl;

    std::cout
        << "================================="
        << std::endl;

    std::cout
        << "Servidor iniciado."
        << std::endl;

    std::cout
        << "Esperando clientes..."
        << std::endl;

    std::cout
        << "Escribe APAGAR para detener el servidor."
        << std::endl;


    ////////////////////////////////////////////////////////////
    // Hilo de notificaciones
    ////////////////////////////////////////////////////////////

    std::thread hiloNotificaciones(
        revisarNotificaciones,
        std::ref(servidorActivo),
        std::ref(condicionNotificaciones),
        std::ref(mutexNotificaciones)
    );


    ////////////////////////////////////////////////////////////
    // Vector de hilos de clientes
    ////////////////////////////////////////////////////////////

    std::vector<std::thread> hilos;


    ////////////////////////////////////////////////////////////
    // Hilo para controlar la consola
    ////////////////////////////////////////////////////////////

    std::thread hiloConsola(
        controlarServidor,
        std::ref(servidor),
        std::ref(servidorActivo),
        std::ref(condicionNotificaciones)
    );


    ////////////////////////////////////////////////////////////
    // Aceptar clientes
    ////////////////////////////////////////////////////////////

    while(servidor.estaActivo())
    {
        int cliente =
            servidor.aceptarCliente();


        ////////////////////////////////////////////////////////
        // Si el servidor fue detenido
        ////////////////////////////////////////////////////////

        if(cliente < 0)
        {
            if(!servidor.estaActivo())
            {
                break;
            }

            std::cout
                << "Error al aceptar cliente."
                << std::endl;

            continue;
        }


        ////////////////////////////////////////////////////////
        // Cliente conectado
        ////////////////////////////////////////////////////////

        std::cout
            << "\nCliente conectado."
            << std::endl;


        ////////////////////////////////////////////////////////
        // Crear hilo para cliente
        ////////////////////////////////////////////////////////

        hilos.emplace_back(
            atenderCliente,
            std::ref(servidor),
            cliente
        );


        ////////////////////////////////////////////////////////
        // Mostrar cantidad de hilos
        ////////////////////////////////////////////////////////

        std::cout
            << "Hilos de clientes activos: "
            << hilos.size()
            << std::endl;
    }


    ////////////////////////////////////////////////////////////
    // El servidor dejó de aceptar clientes
    ////////////////////////////////////////////////////////////

    std::cout
        << "\nDejando de aceptar nuevos clientes..."
        << std::endl;


    ////////////////////////////////////////////////////////////
    // DETENER HILO DE NOTIFICACIONES
    ////////////////////////////////////////////////////////////

    servidorActivo = false;

    condicionNotificaciones.notify_all();


    ////////////////////////////////////////////////////////////
    // Esperar hilo de notificaciones
    ////////////////////////////////////////////////////////////

    if(hiloNotificaciones.joinable())
    {
        hiloNotificaciones.join();
    }


    ////////////////////////////////////////////////////////////
    // Esperar hilo de consola
    ////////////////////////////////////////////////////////////

    if(hiloConsola.joinable())
    {
        hiloConsola.join();
    }


    ////////////////////////////////////////////////////////////
    // Esperar hilos de clientes
    ////////////////////////////////////////////////////////////

    std::cout
        << "Esperando a que terminen los hilos..."
        << std::endl;

    for(auto& hilo : hilos)
    {
        if(hilo.joinable())
        {
            hilo.join();
        }
    }


    ////////////////////////////////////////////////////////////
    // Cerrar servidor
    ////////////////////////////////////////////////////////////

    servidor.cerrar();


    ////////////////////////////////////////////////////////////
    // Servidor terminado
    ////////////////////////////////////////////////////////////

    std::cout
        << "\n================================="
        << std::endl;

    std::cout
        << "       SERVIDOR FINALIZADO"
        << std::endl;

    std::cout
        << "================================="
        << std::endl;


    return 0;
}
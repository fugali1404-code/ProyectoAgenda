#include "servidor.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include <iostream>
#include <string>

Servidor::Servidor()
{
    serverSocket = -1;

#ifdef _WIN32
    WSADATA wsaData;

    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (resultado != 0)
    {
        std::cerr << "Error en WSAStartup: " << resultado << std::endl;
    }
#endif
}

bool Servidor::iniciar(int puerto)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
    if (serverSocket == INVALID_SOCKET)
#else
    if (serverSocket < 0)
#endif
    {
        std::cerr << "Error al crear el socket." << std::endl;
        return false;
    }

    sockaddr_in direccion{};
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    int opcion = 1;

    setsockopt(
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        reinterpret_cast<const char*>(&opcion),
        sizeof(opcion)
    );

    if (bind(
            serverSocket,
            reinterpret_cast<sockaddr*>(&direccion),
            sizeof(direccion)
        ) < 0)
    {
        std::cerr << "Error en bind()." << std::endl;
        cerrar();
        return false;
    }

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Error en listen()." << std::endl;
        cerrar();
        return false;
    }

    return true;
}

int Servidor::aceptarCliente()
{
    sockaddr_in cliente{};

#ifdef _WIN32
    int tamCliente = sizeof(cliente);
#else
    socklen_t tamCliente = sizeof(cliente);
#endif

    int socketCliente = accept(
        serverSocket,
        reinterpret_cast<sockaddr*>(&cliente),
        &tamCliente
    );

#ifdef _WIN32
    if (socketCliente == INVALID_SOCKET)
#else
    if (socketCliente < 0)
#endif
    {
        std::cerr << "Error al aceptar cliente." << std::endl;
        return -1;
    }

    return socketCliente;
}

bool Servidor::recibirMensaje(
    int clienteSocket,
    std::string& mensaje
)
{
    char buffer[1024];

    int bytesRecibidos = recv(
        clienteSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesRecibidos <= 0)
    {
        return false;
    }

    buffer[bytesRecibidos] = '\0';

    mensaje = buffer;

    return true;
}

void Servidor::cerrar()
{
#ifdef _WIN32

    if (serverSocket != INVALID_SOCKET &&
        serverSocket != -1)
    {
        closesocket(serverSocket);
        serverSocket = -1;
    }

    WSACleanup();

#else

    if (serverSocket >= 0)
    {
        close(serverSocket);
        serverSocket = -1;
    }

#endif
}

bool Servidor::enviarMensaje(
    int clienteSocket,
    const std::string& mensaje
)
{
    int enviados = send(
        clienteSocket,
        mensaje.c_str(),
        static_cast<int>(mensaje.size()),
        0
    );

    return enviados > 0;
}
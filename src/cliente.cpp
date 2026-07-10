#include "cliente.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include <iostream>

Cliente::Cliente()
{
    socketCliente = -1;

#ifdef _WIN32
    WSADATA wsaData;

    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (resultado != 0)
    {
        std::cerr << "Error en WSAStartup: "
                  << resultado
                  << std::endl;
    }
#endif
}

bool Cliente::conectar(const std::string& ip, int puerto)
{
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
    if (socketCliente == INVALID_SOCKET)
#else
    if (socketCliente < 0)
#endif
    {
        std::cerr << "Error al crear el socket." << std::endl;
        return false;
    }

    sockaddr_in servidor{};

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);

    servidor.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(
            socketCliente,
            reinterpret_cast<sockaddr*>(&servidor),
            sizeof(servidor)
        ) < 0)
    {
        std::cerr << "Error al conectar." << std::endl;
        return false;
    }

    return true;
}

bool Cliente::enviar(const std::string& mensaje)
{
    int enviados = send(
        socketCliente,
        mensaje.c_str(),
        static_cast<int>(mensaje.size()),
        0
    );

    return enviados > 0;
}

std::string Cliente::recibir()
{
    char buffer[1024];

    int bytes = recv(
        socketCliente,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytes <= 0)
    {
        return "";
    }

    buffer[bytes] = '\0';

    return std::string(buffer);
}

void Cliente::desconectar()
{
#ifdef _WIN32

    if (socketCliente != INVALID_SOCKET &&
        socketCliente != -1)
    {
        closesocket(socketCliente);
        socketCliente = -1;
    }

    WSACleanup();

#else

    if (socketCliente >= 0)
    {
        close(socketCliente);
        socketCliente = -1;
    }

#endif
}
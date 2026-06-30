#include "cliente.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

Cliente::Cliente()
{
    socketCliente = -1;

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

bool Cliente::conectar(const std::string& ip, int puerto)
{
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
    if (socketCliente == INVALID_SOCKET)
        return false;
#else
    if (socketCliente < 0)
        return false;
#endif

    sockaddr_in servidor{};
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);

    servidor.sin_addr.s_addr = inet_addr(ip.c_str());

    if (servidor.sin_addr.s_addr == INADDR_NONE)
        return false;

    return connect(
        socketCliente,
        reinterpret_cast<sockaddr*>(&servidor),
        sizeof(servidor)
    ) == 0;
}

bool Cliente::enviar(const std::string& mensaje)
{
    if (socketCliente == -1)
        return false;

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

    int recibidos = recv(
        socketCliente,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (recibidos <= 0)
        return "";

    buffer[recibidos] = '\0';

    return std::string(buffer);
}

void Cliente::desconectar()
{
#ifdef _WIN32
    if (socketCliente != INVALID_SOCKET && socketCliente != -1)
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
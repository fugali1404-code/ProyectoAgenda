#include "servidor.hpp"

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#else

#include <arpa/inet.h>
#include <unistd.h>

#endif

#include <iostream>

Servidor::Servidor()
{
    serverSocket = -1;

#ifdef _WIN32

    WSADATA wsaData;

    WSAStartup(
        MAKEWORD(2,2),
        &wsaData
    );

#endif
}

bool Servidor::iniciar(int puerto)
{
    serverSocket =
        socket(AF_INET,SOCK_STREAM,0);

    if(serverSocket < 0)
        return false;

    sockaddr_in direccion;

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr =
        INADDR_ANY;

    direccion.sin_port =
        htons(puerto);

    if(
        bind(
            serverSocket,
            (sockaddr*)&direccion,
            sizeof(direccion)
        ) < 0
    )
    {
        return false;
    }

    if(
        listen(
            serverSocket,
            5
        ) < 0
    )
    {
        return false;
    }

    return true;
}

int Servidor::aceptarCliente()
{
    sockaddr_in cliente;

#ifdef _WIN32

    int tam =
        sizeof(cliente);

#else

    socklen_t tam =
        sizeof(cliente);

#endif

    int clienteSocket =
        accept(
            serverSocket,
            (sockaddr*)&cliente,
            &tam
        );

    return clienteSocket;
}

void Servidor::cerrar()
{
#ifdef _WIN32

    closesocket(serverSocket);
    WSACleanup();

#else

    close(serverSocket);

#endif
}
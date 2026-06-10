#ifndef SERVIDOR_HPP
#define SERVIDOR_HPP

#include <vector>
#include <mutex>

class Servidor
{
private:

    int serverSocket;

    std::vector<int> clientes;

    std::mutex clientesMutex;

public:

    Servidor();

    bool iniciar(
        int puerto
    );

    void aceptarClientes();

    void enviarATodos(
        const std::string& mensaje
    );

    void cerrar();
};

#endif
#ifndef USUARIOS_CONTROLLER_HPP
#define USUARIOS_CONTROLLER_HPP

#include <vector>
#include <string>

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

// ============================================================
// DATOS DE USUARIO PARA LA VISTA
// ============================================================

struct UsuarioDatos
{
    int id;

    std::string rol;
    std::string nombre;
    std::string correo;
    std::string identificador;

    // Se utiliza para agregar/actualizar.
    // GET_USUARIOS no lo devuelve.
    std::string password;
};

// ============================================================
// CONTROLLER DE USUARIOS
// ============================================================

class UsuariosController
{
private:

    NetworkManager& network;
    SessionClient& session;

    std::vector<UsuarioDatos> usuarios;

    // --------------------------------------------------------
    // Convertir respuesta GET_USUARIOS
    // --------------------------------------------------------

    bool convertirUsuarios(
        const std::string& respuesta,
        std::vector<UsuarioDatos>& resultado
    ) const;

public:

    UsuariosController(
        NetworkManager& network,
        SessionClient& session
    );

    // --------------------------------------------------------
    // Cargar usuarios
    // --------------------------------------------------------

    bool cargarUsuarios();

    // --------------------------------------------------------
    // Obtener usuarios cargados
    // --------------------------------------------------------

    const std::vector<UsuarioDatos>&
    obtenerUsuarios() const;

    // --------------------------------------------------------
    // Agregar usuario
    // --------------------------------------------------------

    bool agregarUsuario(
        const std::string& rol,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    // --------------------------------------------------------
    // Actualizar usuario
    // --------------------------------------------------------

    bool actualizarUsuario(
        int id,
        const std::string& nombre,
        const std::string& correo,
        const std::string& password,
        const std::string& identificador
    );

    // --------------------------------------------------------
    // Eliminar usuario
    // --------------------------------------------------------

    bool eliminarUsuario(
        int id
    );

    // --------------------------------------------------------
    // Recargar usuarios
    // --------------------------------------------------------

    bool recargar();
};

#endif
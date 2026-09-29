#include "controllers/usuariosController.hpp"

#include "protocolo.hpp"


// ============================================================
// CONSTRUCTOR
// ============================================================

UsuariosController::UsuariosController(
    NetworkManager& network,
    SessionClient& session
)
    : network(network),
      session(session)
{
}


// ============================================================
// CONVERTIR USUARIOS
// ============================================================

bool UsuariosController::convertirUsuarios(
    const std::string& respuesta,
    std::vector<UsuarioDatos>& resultado
) const
{
    resultado.clear();

    // --------------------------------------------------------
    // Separar la respuesta por |
    // --------------------------------------------------------

    std::vector<std::string> datos =
        Protocol::dividir(respuesta);

    if(datos.empty())
    {
        return false;
    }

    // --------------------------------------------------------
    // Validar respuesta
    // --------------------------------------------------------

    if(datos[0] == "NO_LOGIN")
    {
        return false;
    }

    if(datos[0] == "PERMISO_DENEGADO")
    {
        return false;
    }

    if(datos[0] == "ERROR")
    {
        return false;
    }

    if(datos[0] != "USUARIOS")
    {
        return false;
    }

    // --------------------------------------------------------
    // Los usuarios vienen dentro de datos[1]
    //
    // Formato:
    //
    // USUARIOS|
    // id,rol,nombre,correo,identificador;
    // id,rol,nombre,correo,identificador;
    // ...
    // --------------------------------------------------------

    for(size_t i = 1; i < datos.size(); ++i)
    {
        if(datos[i].empty())
        {
            continue;
        }

        // ----------------------------------------------------
        // Separar usuarios por ;
        // ----------------------------------------------------

        std::vector<std::string> usuariosRecibidos =
            Protocol::dividir(
                datos[i],
                ';'
            );

        for(const std::string& usuarioTexto : usuariosRecibidos)
        {
            if(usuarioTexto.empty())
            {
                continue;
            }

            // ------------------------------------------------
            // Separar campos del usuario por ,
            // ------------------------------------------------

            std::vector<std::string> campos =
                Protocol::dividir(
                    usuarioTexto,
                    ','
                );

            if(campos.size() < 5)
            {
                continue;
            }

            try
            {
                UsuarioDatos usuario;

                usuario.id =
                    std::stoi(campos[0]);

                usuario.rol =
                    campos[1];

                usuario.nombre =
                    campos[2];

                usuario.correo =
                    campos[3];

                usuario.identificador =
                    campos[4];

                // GET_USUARIOS no devuelve contraseña
                usuario.password = "";

                if(usuario.id <= 0)
                {
                    continue;
                }

                resultado.push_back(
                    usuario
                );
            }
            catch(...)
            {
                continue;
            }
        }
    }

    return true;
}

// ============================================================
// CARGAR USUARIOS
// ============================================================

bool UsuariosController::cargarUsuarios()
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Administrador")
    {
        return false;
    }

    std::string respuesta =
        network.enviarComando(
            "GET_USUARIOS"
        );

    std::vector<UsuarioDatos> resultado;

    if(!convertirUsuarios(
        respuesta,
        resultado
    ))
    {
        return false;
    }

    usuarios =
        resultado;

    return true;
}


// ============================================================
// OBTENER USUARIOS
// ============================================================

const std::vector<UsuarioDatos>&
UsuariosController::obtenerUsuarios() const
{
    return usuarios;
}


// ============================================================
// AGREGAR USUARIO
// ============================================================

bool UsuariosController::agregarUsuario(
    const std::string& rol,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Administrador")
    {
        return false;
    }

    if(
        rol.empty() ||
        nombre.empty() ||
        correo.empty() ||
        password.empty() ||
        identificador.empty()
    )
    {
        return false;
    }

    std::string comando =
        "ADD_USUARIO|" +
        rol + "|" +
        nombre + "|" +
        correo + "|" +
        password + "|" +
        identificador;

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "USUARIO_AGREGADO")
    {
        return false;
    }

    return cargarUsuarios();
}


// ============================================================
// ACTUALIZAR USUARIO
// ============================================================

bool UsuariosController::actualizarUsuario(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Administrador")
    {
        return false;
    }

    if(id <= 0)
    {
        return false;
    }

    if(
        nombre.empty() ||
        correo.empty() ||
        password.empty() ||
        identificador.empty()
    )
    {
        return false;
    }

    std::string comando =
        "UPDATE_USUARIO|" +
        std::to_string(id) + "|" +
        nombre + "|" +
        correo + "|" +
        password + "|" +
        identificador;

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "USUARIO_ACTUALIZADO")
    {
        return false;
    }

    return cargarUsuarios();
}


// ============================================================
// ELIMINAR USUARIO
// ============================================================

bool UsuariosController::eliminarUsuario(
    int id
)
{
    if(!session.estaAutenticado())
    {
        return false;
    }

    if(session.obtenerRol() != "Administrador")
    {
        return false;
    }

    if(id <= 0)
    {
        return false;
    }

    std::string comando =
        "DELETE_USUARIO|" +
        std::to_string(id);

    std::string respuesta =
        network.enviarComando(
            comando
        );

    if(respuesta != "USUARIO_ELIMINADO")
    {
        return false;
    }

    return cargarUsuarios();
}


// ============================================================
// RECARGAR
// ============================================================

bool UsuariosController::recargar()
{
    return cargarUsuarios();
}
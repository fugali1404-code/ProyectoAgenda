#include "controllers/loginController.hpp"

#include "protocolo.hpp"

LoginController::LoginController(
    LoginView& vistaLogin,
    NetworkManager& networkManager,
    SessionClient& sessionClient
)
    : vista(vistaLogin),
      network(networkManager),
      session(sessionClient)
{
}

bool LoginController::procesarLogin(
    const std::string& correo,
    const std::string& password
)
{
    //-------------------------------------------------
    // Verificar conexión
    //-------------------------------------------------

    if(!network.estaConectado())
    {
        vista.setMensaje(
            "No hay conexion con el servidor"
        );

        return false;
    }

    //-------------------------------------------------
    // Enviar Login
    //-------------------------------------------------

    std::string respuesta =
        network.enviarComando(
            "LOGIN|" +
            correo +
            "|" +
            password
        );

    //-------------------------------------------------
    // Dividir respuesta
    //-------------------------------------------------

    std::vector<std::string> datos =
        Protocol::dividir(
            respuesta
        );

    //-------------------------------------------------
    // Verificar respuesta
    //-------------------------------------------------

    if(datos.empty())
    {
        vista.setMensaje(
            "Error de comunicacion"
        );

        return false;
    }

    //-------------------------------------------------
    // Login correcto
    //-------------------------------------------------

    if(datos[0] == "LOGIN_OK")
    {
        if(datos.size() < 4)
        {
            vista.setMensaje(
                "Respuesta invalida del servidor"
            );

            return false;
        }

        //-------------------------------------------------
        // Obtener datos
        //-------------------------------------------------

        std::string nombre =
            datos[1];

        std::string rol =
            datos[2];

        std::string identificador =
            datos[3];

        //-------------------------------------------------
        // Guardar sesión
        //-------------------------------------------------

        session.iniciarSesion(
            nombre,
            rol,
            identificador
        );

        //-------------------------------------------------
        // Mensaje
        //-------------------------------------------------

        vista.setMensaje(
            "Login correcto"
        );

        return true;
    }

    //-------------------------------------------------
    // Login incorrecto
    //-------------------------------------------------

    vista.setMensaje(
        "Correo o contraseña incorrectos"
    );

    return false;
}

void LoginController::manejarEvento(
    const sf::Event& event,
    const sf::RenderWindow& window
)
{
    vista.manejarEvento(
        event,
        window
    );
}

bool LoginController::loginPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
) const
{
    return vista.loginPresionado(
        window,
        event
    );
}

VistaActual LoginController::obtenerVistaSiguiente() const
{
    if(session.estaAutenticado())
    {
        return VistaActual::DASHBOARD;
    }

    return VistaActual::LOGIN;
}
#ifndef LOGINCONTROLLER_HPP
#define LOGINCONTROLLER_HPP

#include <string>

#include "../SFML/login.hpp"
#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"
#include "SFML/vistaActual.hpp"

class LoginController
{
private:

    //-------------------------------------------------
    // Vista
    //-------------------------------------------------

    LoginView& vista;

    //-------------------------------------------------
    // Red
    //-------------------------------------------------

    NetworkManager& network;

    //-------------------------------------------------
    // Sesión
    //-------------------------------------------------

    SessionClient& session;

public:

    //-------------------------------------------------
    // Constructor
    //-------------------------------------------------

    LoginController(
        LoginView& vistaLogin,
        NetworkManager& networkManager,
        SessionClient& sessionClient
    );

    //-------------------------------------------------
    // Procesar Login
    //-------------------------------------------------

    bool procesarLogin(
        const std::string& correo,
        const std::string& password
    );

    //-------------------------------------------------
    // Eventos
    //-------------------------------------------------

    void manejarEvento(
        const sf::Event& event,
        const sf::RenderWindow& window
    );

    //-------------------------------------------------
    // Login
    //-------------------------------------------------

    bool loginPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    ) const;

    //-------------------------------------------------
    // Vista siguiente
    //-------------------------------------------------

    VistaActual obtenerVistaSiguiente() const;
};

#endif
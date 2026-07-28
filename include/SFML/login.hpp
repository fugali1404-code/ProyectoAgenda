#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "textbox.hpp"

class LoginView
{
private:

    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    sf::Font font;

    //-------------------------------------------------
    // TextBox
    //-------------------------------------------------

    TextBox usuarioBox;
    TextBox passwordBox;

    //-------------------------------------------------
    // Botón Login
    //-------------------------------------------------

    sf::RectangleShape botonLogin;

    //-------------------------------------------------
    // Estado
    //-------------------------------------------------

    std::string mensajeEstado;

public:

    //-------------------------------------------------
    // Constructor
    //-------------------------------------------------

    LoginView();

    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    bool cargarFuente(
        const std::string& ruta
    );

    //-------------------------------------------------
    // Dibujar
    //-------------------------------------------------

    void draw(
        sf::RenderWindow& window
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
    // Obtener datos
    //-------------------------------------------------

    std::string obtenerUsuario() const;

    std::string obtenerPassword() const;

    //-------------------------------------------------
    // Utilidades
    //-------------------------------------------------

    void limpiar();

    void setMensaje(
        const std::string& mensaje
    );

    std::string getMensaje() const;

    //-------------------------------------------------
    // Acceso a la fuente
    //-------------------------------------------------

    const sf::Font&
    getFont() const;
};

#endif
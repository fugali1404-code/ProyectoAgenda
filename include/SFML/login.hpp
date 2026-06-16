#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <SFML/Graphics.hpp>
#include <string>

class LoginView
{
private:

    sf::Font font;

    sf::RectangleShape botonLogin;

public:

    LoginView();

    bool cargarFuente(
        const std::string& ruta
    );

    void draw(
        sf::RenderWindow& window
    );
};

#endif
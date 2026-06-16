#include "SFML/login.hpp"

LoginView::LoginView()
{
    botonLogin.setSize(
        {200.f,50.f}
    );

    botonLogin.setPosition(
        {300.f,350.f}
    );
}

bool LoginView::cargarFuente(
    const std::string& ruta)
{
    return font.openFromFile(ruta);
}

void LoginView::draw(
    sf::RenderWindow& window)
{
    sf::Text titulo(font);

    titulo.setString(
        "Agenda Academica"
    );

    titulo.setCharacterSize(40);

    titulo.setPosition(
        {250.f,50.f}
    );

    sf::Text usuario(font);

    usuario.setString(
        "Usuario: _______"
    );

    usuario.setCharacterSize(25);

    usuario.setPosition(
        {200.f,180.f}
    );

    sf::Text password(font);

    password.setString(
        "Password: _______"
    );

    password.setCharacterSize(25);

    password.setPosition(
        {200.f,250.f}
    );

    window.draw(titulo);
    window.draw(usuario);
    window.draw(password);
    window.draw(botonLogin);
}
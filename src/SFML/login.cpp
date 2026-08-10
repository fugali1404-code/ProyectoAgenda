#include "SFML/login.hpp"

LoginView::LoginView()
    : usuarioBox(false),
      passwordBox(true)
{
    usuarioBox.setPosition(250.f, 170.f);
    usuarioBox.setSize(300.f, 40.f);

    passwordBox.setPosition(250.f, 270.f);
    passwordBox.setSize(300.f, 40.f);

    botonLogin.setSize({180.f, 50.f});
    botonLogin.setPosition({310.f, 360.f});

    botonLogin.setFillColor(
        sf::Color(70,130,180)
    );

    mensajeEstado = "Esperando...";
}

bool LoginView::cargarFuente(
    const std::string& ruta)
{
    return font.openFromFile(ruta);
}

void LoginView::draw(
    sf::RenderWindow& window)
{
    //---------------------------------
    // Título
    //---------------------------------

    sf::Text titulo(font);

    titulo.setString(
        "Agenda Academica"
    );

    titulo.setCharacterSize(36);

    titulo.setFillColor(
        sf::Color::White
    );

    titulo.setPosition(
        {220.f,50.f}
    );

    //---------------------------------
    // Usuario
    //---------------------------------

    sf::Text usuario(font);

    usuario.setString(
        "Correo"
    );

    usuario.setCharacterSize(22);

    usuario.setFillColor(
        sf::Color::White
    );

    usuario.setPosition(
        {250.f,135.f}
    );

    //---------------------------------
    // Password
    //---------------------------------

    sf::Text password(font);

    password.setString(
        "Password"
    );

    password.setCharacterSize(22);

    password.setFillColor(
        sf::Color::White
    );

    password.setPosition(
        {250.f,235.f}
    );

    //---------------------------------
    // Texto botón
    //---------------------------------

    sf::Text textoBoton(font);

    textoBoton.setString(
        "Iniciar Sesion"
    );

    textoBoton.setCharacterSize(22);

    textoBoton.setFillColor(
        sf::Color::White
    );

    textoBoton.setPosition(
        {332.f,372.f}
    );

    //---------------------------------
    // Estado
    //---------------------------------

    sf::Text estado(font);

    estado.setString(
        mensajeEstado
    );

    estado.setCharacterSize(18);

    estado.setFillColor(
        sf::Color::Yellow
    );

    estado.setPosition(
        {250.f,440.f}
    );

    //---------------------------------
    // Dibujar
    //---------------------------------

    window.draw(titulo);

    window.draw(usuario);

    usuarioBox.draw(
        window,
        font
    );

    window.draw(password);

    passwordBox.draw(
        window,
        font
    );

    window.draw(botonLogin);

    window.draw(textoBoton);

    window.draw(estado);
}

void LoginView::manejarEvento(
    const sf::Event& event,
    const sf::RenderWindow& window)
{
    usuarioBox.handleEvent(
        event,
        window
    );

    passwordBox.handleEvent(
        event,
        window
    );
}

bool LoginView::loginPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
) const
{
    if(const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f posicion(
            static_cast<float>(
                mouse->position.x),
            static_cast<float>(
                mouse->position.y)
        );

        return botonLogin
            .getGlobalBounds()
            .contains(posicion);
    }

    return false;
}

std::string LoginView::obtenerUsuario() const
{
    return usuarioBox.getText();
}

std::string LoginView::obtenerPassword() const
{
    return passwordBox.getText();
}

void LoginView::limpiar()
{
    usuarioBox.clear();
    passwordBox.clear();
}

void LoginView::setMensaje(
    const std::string& mensaje)
{
    mensajeEstado = mensaje;
}

std::string LoginView::getMensaje() const
{
    return mensajeEstado;
}

const sf::Font&
LoginView::getFont() const
{
    return font;
}
#include "SFML/login.hpp"

LoginView::LoginView()
    : usuarioBox(false),
      passwordBox(true)
{
    //---------------------------------
    // Usuario
    //---------------------------------

    usuarioBox.setPosition(
        490.f,
        270.f
    );

    usuarioBox.setSize(
        300.f,
        40.f
    );

    //---------------------------------
    // Password
    //---------------------------------

    passwordBox.setPosition(
        490.f,
        370.f
    );

    passwordBox.setSize(
        300.f,
        40.f
    );

    //---------------------------------
    // Botón Login
    //---------------------------------

    botonLogin.setSize(
        {180.f,50.f}
    );

    botonLogin.setPosition(
        {550.f,450.f}
    );

    botonLogin.setFillColor(
        sf::Color(60,150,80)
    );

    //---------------------------------
    // Estado
    //---------------------------------

    mensajeEstado = "";
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
    // COLORES
    //---------------------------------

    sf::Color fondo(
        240,
        240,
        240
    );

    sf::Color tarjetaColor(
        255,
        255,
        255
    );

    sf::Color textoNegro(
        40,
        40,
        40
    );

    sf::Color bordeTarjeta(
        220,
        220,
        220
    );

    //---------------------------------
    // FONDO
    //---------------------------------

    window.clear(
        fondo
    );

    //---------------------------------
    // TITULO
    //---------------------------------

    sf::Text titulo(font);

    titulo.setString(
        "AGENDA\nESCOLAR"
    );

    titulo.setCharacterSize(
        38
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {40.f,30.f}
    );

    window.draw(
        titulo
    );

    //---------------------------------
    // TARJETA LOGIN
    //---------------------------------

    sf::RectangleShape tarjeta;

    tarjeta.setSize(
        {500.f,500.f}
    );

    tarjeta.setPosition(
        {390.f,100.f}
    );

    tarjeta.setFillColor(
        tarjetaColor
    );

    tarjeta.setOutlineThickness(
        2
    );

    tarjeta.setOutlineColor(
        bordeTarjeta
    );

    window.draw(
        tarjeta
    );

    //---------------------------------
    // TITULO LOGIN
    //---------------------------------

    sf::Text tituloLogin(font);

    tituloLogin.setString(
        "Iniciar Sesion"
    );

    tituloLogin.setCharacterSize(
        30
    );

    tituloLogin.setFillColor(
        textoNegro
    );

    tituloLogin.setPosition(
        {505.f,150.f}
    );

    window.draw(
        tituloLogin
    );

    //---------------------------------
    // CORREO
    //---------------------------------

    sf::Text usuario(font);

    usuario.setString(
        "Correo"
    );

    usuario.setCharacterSize(
        20
    );

    usuario.setFillColor(
        textoNegro
    );

    usuario.setPosition(
        {490.f,235.f}
    );

    window.draw(
        usuario
    );

    //---------------------------------
    // PASSWORD
    //---------------------------------

    sf::Text password(font);

    password.setString(
        "Password"
    );

    password.setCharacterSize(
        20
    );

    password.setFillColor(
        textoNegro
    );

    password.setPosition(
        {490.f,335.f}
    );

    window.draw(
        password
    );

    //---------------------------------
    // TEXTBOX USUARIO
    //---------------------------------

    usuarioBox.draw(
        window,
        font
    );

    //---------------------------------
    // TEXTBOX PASSWORD
    //---------------------------------

    passwordBox.draw(
        window,
        font
    );

    //---------------------------------
    // BOTON
    //---------------------------------

    window.draw(
        botonLogin
    );

    //---------------------------------
    // TEXTO BOTON
    //---------------------------------

    sf::Text textoBoton(font);

    textoBoton.setString(
        "Iniciar Sesion"
    );

    textoBoton.setCharacterSize(
        18
    );

    textoBoton.setFillColor(
        sf::Color::White
    );

    textoBoton.setPosition(
        {570.f,465.f}
    );

    window.draw(
        textoBoton
    );

    //---------------------------------
    // MENSAJE
    //---------------------------------

    if(!mensajeEstado.empty())
    {
        sf::Text estado(font);

        estado.setString(
            mensajeEstado
        );

        estado.setCharacterSize(
            18
        );

        estado.setFillColor(
            sf::Color::Red
        );

        estado.setPosition(
            {490.f,525.f}
        );

        window.draw(
            estado
        );
    }
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
                mouse->position.x
            ),
            static_cast<float>(
                mouse->position.y
            )
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
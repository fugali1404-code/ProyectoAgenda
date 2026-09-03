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
#pragma once

#include "cliente.hpp"

#include <string>

class NetworkManager
{
private:

    Cliente cliente;

    bool conectado;

public:

    NetworkManager();

    bool conectar(
        const std::string& ip,
        int puerto
    );

    bool enviarMensaje(
        const std::string& mensaje
    );

    std::string recibirMensaje();

    bool enviarYRecibir(
        const std::string& mensaje,
        std::string& respuesta
    );

    std::string enviarComando(
      const std::string& comando
    );

    void desconectar();

    bool estaConectado() const;
};
#include "networkmanager.hpp"

NetworkManager::NetworkManager()
{
    conectado = false;
}

bool NetworkManager::conectar(
    const std::string& ip,
    int puerto
)
{
    conectado = cliente.conectar(ip, puerto);

    return conectado;
}

bool NetworkManager::enviarMensaje(
    const std::string& mensaje
)
{
    if(!conectado)
        return false;

    return cliente.enviar(mensaje);
}

std::string NetworkManager::recibirMensaje()
{
    if(!conectado)
        return "";

    return cliente.recibir();
}

void NetworkManager::desconectar()
{
    if(conectado)
    {
        cliente.desconectar();
        conectado = false;
    }
}

bool NetworkManager::estaConectado() const
{
    return conectado;
}

bool NetworkManager::enviarYRecibir(
    const std::string& mensaje,
    std::string& respuesta
)
{
    if(!conectado)
        return false;

    if(!cliente.enviar(mensaje))
        return false;

    respuesta = cliente.recibir();

    return !respuesta.empty();
}

std::string NetworkManager::enviarComando(
    const std::string& comando
)
{
    if(!conectado)
    {
        return "NO_CONECTADO";
    }

    std::string respuesta;

    if(!enviarYRecibir(
            comando,
            respuesta))
    {
        return "ERROR_RED";
    }

    return respuesta;
}
bool SessionManager::login(
    const std::string& correo,
    const std::string& password
)
{
    Usuario* usuario = Persistencia::autenticarUsuario(correo,password,"usuarios.txt");

    if(usuario == nullptr)
    {
        datos.limpiar();
        return false;
    }

    datos.setUsuarioId(usuario->getId());
    datos.setUsuario(usuario->getCorreo());
    datos.setNombreCompleto(usuario->getNombre());
    datos.setRol(usuario->getRol());


    // Identificador
    datos.setIdentificador(usuario->getIdentificador());

    // Materias
    std::vector<Materia> todasLasMaterias;

    Persistencia::cargarMaterias(todasLasMaterias,"materias.txt");

    std::vector<Materia> materiasProfesor;

    for(const auto& materia : todasLasMaterias)
    {
        if(materia.getProfesorId() == usuario->getId())
        {
            materiasProfesor.push_back(materia);
        }
    
    }

    datos.setMaterias(materiasProfesor);

    // Sesión autenticada
    datos.setAutenticado(true);

    delete usuario;

    return true;
}

//-------------------------------------------------------------

void SessionManager::logout()
{
    datos.limpiar();
}

/---------------------------------
    // LOGIN
    //---------------------------------

    if(comando == "LOGIN")
    {
        if(datos.size() < 3)
        {
            return "LOGIN_ERROR";
        }

        if(session.login(datos[1], datos[2]))
        {
            return
                "LOGIN_OK|"
                + session.obtenerNombreCompleto()
                + "|"
                + session.obtenerRol()
                + "|"
                + session.obtenerIdentificador();
        }

        return "LOGIN_ERROR";
    }

    //---------------------------------
    // LOGOUT
    //---------------------------------

    if(comando == "LOGOUT")
    {
        session.logout();
        return "LOGOUT_OK";
    }


#ifndef USUARIOSVIEW_HPP
#define USUARIOSVIEW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "controllers/usuariosController.hpp"

class UsuariosView
{
private:

    sf::Font font;

    std::vector<UsuarioDatos> usuarios;

    float inicioLista;
    float altoTarjeta;
    float espacioTarjetas;
    float desplazamientoUsuarios;

    int usuarioSeleccionado;

    sf::RectangleShape botonAgregar;

    std::vector<sf::RectangleShape> botonesEditar;
    std::vector<sf::RectangleShape> botonesEliminar;

    // Formulario
    bool mostrandoFormulario;
    bool modoEditar;

    bool aceptarFormulario;
    bool cancelarFormulario;

    // Confirmación de eliminación
    bool mostrandoConfirmacion;
    bool aceptarEliminacion;
    bool cancelarEliminacion;

    int usuarioEliminar;

    UsuarioDatos usuarioFormulario;

    int campoActivo;

    sf::RectangleShape ventanaFormulario;

    sf::RectangleShape botonAceptar;
    sf::RectangleShape botonCancelar;

    sf::RectangleShape campoRol;
    sf::RectangleShape campoNombre;
    sf::RectangleShape campoCorreo;
    sf::RectangleShape campoPassword;
    sf::RectangleShape campoIdentificador;

    // Botones de confirmación
    sf::RectangleShape botonConfirmarEliminacion;
    sf::RectangleShape botonCancelarEliminacion;

    std::string mensajeEstado;

    sf::Clock relojDobleClic;
    int usuarioPendienteDobleClic;

    void actualizarGeometria();

    void dibujarTexto(
        sf::RenderWindow& ventana,
        const std::string& texto,
        float x,
        float y,
        unsigned int tamano
    );

    void dibujarUsuarios(
        sf::RenderWindow& ventana
    );

    void dibujarFormulario(
        sf::RenderWindow& ventana
    );

    void dibujarConfirmacion(
        sf::RenderWindow& ventana
    );

    void dibujarBotones(
        sf::RenderWindow& ventana
    );

    sf::Vector2f obtenerPosicionMouse(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool puntoDentro(
        const sf::RectangleShape& rectangulo,
        const sf::Vector2f& posicion
    ) const;

    void activarCampo(int campo);

    void agregarCaracter(char caracter);

    void borrarCaracter();

public:

    UsuariosView();

    bool cargarFuente(
        const std::string& ruta
    );

    void setUsuarios(
        const std::vector<UsuarioDatos>& usuarios
    );

    void manejarEvento(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void draw(
        sf::RenderWindow& ventana
    );

    bool botonAgregarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    );

    bool botonEditarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento,
        int& indice
    );

    bool botonEliminarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento,
        int& indice
    );

    bool formularioAceptado();

    bool formularioCancelado();

    bool formularioActivo() const;

    bool formularioEsEdicion() const;

    const UsuarioDatos& obtenerDatosFormulario() const;

    void cerrarFormulario();

    void mostrarMensaje(
        const std::string& mensaje
    );

    // Confirmación de eliminación
    bool confirmacionActiva() const;

    bool confirmacionAceptada();

    bool confirmacionCancelada();

    int obtenerUsuarioEliminar() const;
};

#endif
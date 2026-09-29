#include "SFML/usuariosView.hpp"

#include <algorithm>
#include <cctype>


//////////////////////////////////////////////////////////////
// COLORES DEL PROYECTO
//////////////////////////////////////////////////////////////

static const sf::Color encabezadoColor(41, 53, 65);
static const sf::Color tarjetaColor(255, 255, 255);
static const sf::Color textoBlanco(255, 255, 255);
static const sf::Color textoNegro(40, 40, 40);
static const sf::Color bordeTarjeta(220, 220, 220);
static const sf::Color lineaSeparadora(90, 105, 120);
static const sf::Color fondoScroll(225, 225, 225);
static const sf::Color barraScroll(120, 120, 120);
static const sf::Color botonRojo(220, 70, 70);
static const sf::Color botonGris(110, 110, 110);


//////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////

UsuariosView::UsuariosView()
{
    inicioLista = 115.0f;
    altoTarjeta = 145.0f;
    espacioTarjetas = 15.0f;

    desplazamientoUsuarios = 0.0f;

    usuarioSeleccionado = -1;

    mostrandoFormulario = false;
    modoEditar = false;

    aceptarFormulario = false;
    cancelarFormulario = false;

    mostrandoConfirmacion = false;
    aceptarEliminacion = false;
    cancelarEliminacion = false;

    usuarioEliminar = -1;

    campoActivo = -1;

    usuarioPendienteDobleClic = -1;

    usuarioFormulario.id = 0;
    usuarioFormulario.rol = "";
    usuarioFormulario.nombre = "";
    usuarioFormulario.correo = "";
    usuarioFormulario.identificador = "";
    usuarioFormulario.password = "";


    //////////////////////////////////////////////////////////
    // BOTON AGREGAR
    //////////////////////////////////////////////////////////

    botonAgregar.setSize(
        sf::Vector2f(
            150.0f,
            45.0f
        )
    );

    botonAgregar.setPosition(
        sf::Vector2f(
            1000.0f,
            45.0f
        )
    );


    //////////////////////////////////////////////////////////
    // VENTANA FORMULARIO
    //////////////////////////////////////////////////////////

    ventanaFormulario.setSize(
        sf::Vector2f(
            650.0f,
            560.0f
        )
    );

    ventanaFormulario.setPosition(
        sf::Vector2f(
            315.0f,
            80.0f
        )
    );


    //////////////////////////////////////////////////////////
    // BOTON ACEPTAR
    //////////////////////////////////////////////////////////

    botonAceptar.setSize(
        sf::Vector2f(
            130.0f,
            45.0f
        )
    );

    botonAceptar.setPosition(
        sf::Vector2f(
            490.0f,
            560.0f
        )
    );


    //////////////////////////////////////////////////////////
    // BOTON CANCELAR
    //////////////////////////////////////////////////////////

    botonCancelar.setSize(
        sf::Vector2f(
            130.0f,
            45.0f
        )
    );

    botonCancelar.setPosition(
        sf::Vector2f(
            660.0f,
            560.0f
        )
    );


    //////////////////////////////////////////////////////////
    // CAMPOS
    //////////////////////////////////////////////////////////

    campoRol.setSize(
        sf::Vector2f(
            500.0f,
            45.0f
        )
    );

    campoRol.setPosition(
        sf::Vector2f(
            390.0f,
            165.0f
        )
    );


    campoNombre.setSize(
        sf::Vector2f(
            500.0f,
            45.0f
        )
    );

    campoNombre.setPosition(
        sf::Vector2f(
            390.0f,
            235.0f
        )
    );


    campoCorreo.setSize(
        sf::Vector2f(
            500.0f,
            45.0f
        )
    );

    campoCorreo.setPosition(
        sf::Vector2f(
            390.0f,
            305.0f
        )
    );


    campoPassword.setSize(
        sf::Vector2f(
            500.0f,
            45.0f
        )
    );

    campoPassword.setPosition(
        sf::Vector2f(
            390.0f,
            375.0f
        )
    );


    campoIdentificador.setSize(
        sf::Vector2f(
            500.0f,
            45.0f
        )
    );

    campoIdentificador.setPosition(
        sf::Vector2f(
            390.0f,
            445.0f
        )
    );


    //////////////////////////////////////////////////////////
    // BOTONES DE CONFIRMACION
    //////////////////////////////////////////////////////////

    botonCancelarEliminacion.setSize(
        sf::Vector2f(
            130.0f,
            45.0f
        )
    );

    botonCancelarEliminacion.setPosition(
        sf::Vector2f(
            460.0f,
            395.0f
        )
    );


    botonConfirmarEliminacion.setSize(
        sf::Vector2f(
            130.0f,
            45.0f
        )
    );

    botonConfirmarEliminacion.setPosition(
        sf::Vector2f(
            620.0f,
            395.0f
        )
    );


    actualizarGeometria();
}


//////////////////////////////////////////////////////////////
// CARGAR FUENTE
//////////////////////////////////////////////////////////////

bool UsuariosView::cargarFuente(
    const std::string& ruta
)
{
    return font.openFromFile(ruta);
}


//////////////////////////////////////////////////////////////
// ACTUALIZAR GEOMETRIA
//////////////////////////////////////////////////////////////

void UsuariosView::actualizarGeometria()
{
    botonAgregar.setPosition(
        sf::Vector2f(
            1000.0f,
            45.0f
        )
    );
}


//////////////////////////////////////////////////////////////
// DIBUJAR TEXTO
//////////////////////////////////////////////////////////////

void UsuariosView::dibujarTexto(
    sf::RenderWindow& ventana,
    const std::string& texto,
    float x,
    float y,
    unsigned int tamano
)
{
    sf::Text textoDibujar(
        font,
        texto,
        tamano
    );

    textoDibujar.setFillColor(
        textoNegro
    );

    textoDibujar.setPosition(
        sf::Vector2f(
            x,
            y
        )
    );

    ventana.draw(
        textoDibujar
    );
}


//////////////////////////////////////////////////////////////
// OBTENER POSICION DEL MOUSE
//////////////////////////////////////////////////////////////

sf::Vector2f UsuariosView::obtenerPosicionMouse(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return sf::Vector2f(
            0.0f,
            0.0f
        );
    }

    return ventana.mapPixelToCoords(
        mouse->position
    );
}


//////////////////////////////////////////////////////////////
// PUNTO DENTRO
//////////////////////////////////////////////////////////////

bool UsuariosView::puntoDentro(
    const sf::RectangleShape& rectangulo,
    const sf::Vector2f& posicion
) const
{
    return rectangulo
        .getGlobalBounds()
        .contains(posicion);
}


//////////////////////////////////////////////////////////////
// SET USUARIOS
//////////////////////////////////////////////////////////////

void UsuariosView::setUsuarios(
    const std::vector<UsuarioDatos>& nuevosUsuarios
)
{
    usuarios = nuevosUsuarios;

    usuarioSeleccionado = -1;

    desplazamientoUsuarios = 0.0f;

    botonesEditar.clear();
    botonesEliminar.clear();

    usuarioEliminar = -1;

    mostrandoConfirmacion = false;

    aceptarEliminacion = false;
    cancelarEliminacion = false;
}


//////////////////////////////////////////////////////////////
// DIBUJAR BOTONES SUPERIORES
//////////////////////////////////////////////////////////////

void UsuariosView::dibujarBotones(
    sf::RenderWindow& ventana
)
{
    botonAgregar.setFillColor(
        botonGris
    );

    ventana.draw(
        botonAgregar
    );

    sf::Text textoAgregar(
        font,
        "+ Agregar",
        20
    );

    textoAgregar.setFillColor(
        textoBlanco
    );

    textoAgregar.setPosition(
        sf::Vector2f(
            1025.0f,
            55.0f
        )
    );

    ventana.draw(
        textoAgregar
    );
}


//////////////////////////////////////////////////////////////
// DIBUJAR USUARIOS
//////////////////////////////////////////////////////////////

void UsuariosView::dibujarUsuarios(
    sf::RenderWindow& ventana
)
{
    botonesEditar.clear();
    botonesEliminar.clear();


    for(size_t i = 0; i < usuarios.size(); i++)
    {
        float y =
            inicioLista
            + static_cast<float>(i)
            * (altoTarjeta + espacioTarjetas)
            + desplazamientoUsuarios;


        ////////////////////////////////////////////////////////
        // BOTON EDITAR
        ////////////////////////////////////////////////////////

        sf::RectangleShape editar;

        editar.setSize(
            sf::Vector2f(
                90.0f,
                38.0f
            )
        );

        editar.setPosition(
            sf::Vector2f(
                1010.0f,
                y + 20.0f
            )
        );


        ////////////////////////////////////////////////////////
        // BOTON ELIMINAR
        ////////////////////////////////////////////////////////

        sf::RectangleShape eliminar;

        eliminar.setSize(
            sf::Vector2f(
                90.0f,
                38.0f
            )
        );

        eliminar.setPosition(
            sf::Vector2f(
                1010.0f,
                y + 75.0f
            )
        );


        ////////////////////////////////////////////////////////
        // GUARDAR PARA TODOS LOS USUARIOS
        ////////////////////////////////////////////////////////

        botonesEditar.push_back(
            editar
        );

        botonesEliminar.push_back(
            eliminar
        );


        ////////////////////////////////////////////////////////
        // FUERA DE PANTALLA
        ////////////////////////////////////////////////////////

        if(y + altoTarjeta < 95.0f)
        {
            continue;
        }

        if(y > 720.0f)
        {
            continue;
        }


        ////////////////////////////////////////////////////////
        // TARJETA
        ////////////////////////////////////////////////////////

        sf::RectangleShape tarjeta;

        tarjeta.setSize(
            sf::Vector2f(
                1160.0f,
                altoTarjeta
            )
        );

        tarjeta.setPosition(
            sf::Vector2f(
                60.0f,
                y
            )
        );

        tarjeta.setFillColor(
            tarjetaColor
        );

        tarjeta.setOutlineColor(
            bordeTarjeta
        );

        tarjeta.setOutlineThickness(
            1.0f
        );

        ventana.draw(
            tarjeta
        );


        ////////////////////////////////////////////////////////
        // INFORMACION
        ////////////////////////////////////////////////////////

        std::string textoId =
            "ID: " +
            std::to_string(
                usuarios[i].id
            );

        std::string textoRol =
            "Rol: " +
            usuarios[i].rol;

        std::string textoNombre =
            "Nombre: " +
            usuarios[i].nombre;

        std::string textoCorreo =
            "Correo: " +
            usuarios[i].correo;

        std::string textoIdentificador =
            "Identificador: " +
            usuarios[i].identificador;


        dibujarTexto(
            ventana,
            textoId,
            80.0f,
            y + 15.0f,
            18
        );

        dibujarTexto(
            ventana,
            textoRol,
            200.0f,
            y + 15.0f,
            18
        );

        dibujarTexto(
            ventana,
            textoNombre,
            80.0f,
            y + 50.0f,
            18
        );

        dibujarTexto(
            ventana,
            textoCorreo,
            80.0f,
            y + 85.0f,
            17
        );

        dibujarTexto(
            ventana,
            textoIdentificador,
            650.0f,
            y + 50.0f,
            17
        );


        ////////////////////////////////////////////////////////
        // BOTON EDITAR
        ////////////////////////////////////////////////////////

        editar.setFillColor(
            botonGris
        );

        ventana.draw(
            editar
        );

        sf::Text textoEditar(
            font,
            "Editar",
            17
        );

        textoEditar.setFillColor(
            textoBlanco
        );

        textoEditar.setPosition(
            sf::Vector2f(
                1028.0f,
                y + 28.0f
            )
        );

        ventana.draw(
            textoEditar
        );


        ////////////////////////////////////////////////////////
        // BOTON ELIMINAR
        ////////////////////////////////////////////////////////

        eliminar.setFillColor(
            botonRojo
        );

        ventana.draw(
            eliminar
        );

        sf::Text textoEliminar(
            font,
            "Eliminar",
            16
        );

        textoEliminar.setFillColor(
            textoBlanco
        );

        textoEliminar.setPosition(
            sf::Vector2f(
                1018.0f,
                y + 83.0f
            )
        );

        ventana.draw(
            textoEliminar
        );
    }
}


//////////////////////////////////////////////////////////////
// ACTIVAR CAMPO
//////////////////////////////////////////////////////////////

void UsuariosView::activarCampo(
    int campo
)
{
    campoActivo = campo;
}


//////////////////////////////////////////////////////////////
// AGREGAR CARACTER
//////////////////////////////////////////////////////////////

void UsuariosView::agregarCaracter(
    char caracter
)
{
    switch(campoActivo)
    {
        case 0:
            usuarioFormulario.rol += caracter;
            break;

        case 1:
            usuarioFormulario.nombre += caracter;
            break;

        case 2:
            usuarioFormulario.correo += caracter;
            break;

        case 3:
            usuarioFormulario.password += caracter;
            break;

        case 4:
            usuarioFormulario.identificador += caracter;
            break;

        default:
            break;
    }
}


//////////////////////////////////////////////////////////////
// BORRAR CARACTER
//////////////////////////////////////////////////////////////

void UsuariosView::borrarCaracter()
{
    std::string* campo = nullptr;

    switch(campoActivo)
    {
        case 0:
            campo =
                &usuarioFormulario.rol;
            break;

        case 1:
            campo =
                &usuarioFormulario.nombre;
            break;

        case 2:
            campo =
                &usuarioFormulario.correo;
            break;

        case 3:
            campo =
                &usuarioFormulario.password;
            break;

        case 4:
            campo =
                &usuarioFormulario.identificador;
            break;

        default:
            return;
    }

    if(!campo->empty())
    {
        campo->pop_back();
    }
}


//////////////////////////////////////////////////////////////
// DIBUJAR FORMULARIO
//////////////////////////////////////////////////////////////

void UsuariosView::dibujarFormulario(
    sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // FONDO OSCURO
    //////////////////////////////////////////////////////////

    sf::RectangleShape fondo;

    fondo.setSize(
        sf::Vector2f(
            1280.0f,
            720.0f
        )
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            100
        )
    );

    ventana.draw(
        fondo
    );


    //////////////////////////////////////////////////////////
    // VENTANA
    //////////////////////////////////////////////////////////

    ventanaFormulario.setFillColor(
        tarjetaColor
    );

    ventanaFormulario.setOutlineColor(
        bordeTarjeta
    );

    ventanaFormulario.setOutlineThickness(
        1.0f
    );

    ventana.draw(
        ventanaFormulario
    );


    //////////////////////////////////////////////////////////
    // TITULO
    //////////////////////////////////////////////////////////

    sf::Text titulo(
        font,
        modoEditar
            ? "Editar usuario"
            : "Agregar usuario",
        27
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        sf::Vector2f(
            390.0f,
            105.0f
        )
    );

    ventana.draw(
        titulo
    );


    //////////////////////////////////////////////////////////
    // ETIQUETAS
    //////////////////////////////////////////////////////////

    dibujarTexto(
        ventana,
        "Rol",
        390.0f,
        140.0f,
        17
    );

    dibujarTexto(
        ventana,
        "Nombre",
        390.0f,
        210.0f,
        17
    );

    dibujarTexto(
        ventana,
        "Correo",
        390.0f,
        280.0f,
        17
    );

    dibujarTexto(
        ventana,
        "Contraseña",
        390.0f,
        350.0f,
        17
    );

    dibujarTexto(
        ventana,
        "Identificador",
        390.0f,
        420.0f,
        17
    );


    //////////////////////////////////////////////////////////
    // COLOR DE CAMPOS
    //////////////////////////////////////////////////////////

    campoRol.setFillColor(
        campoActivo == 0
            ? sf::Color(220, 235, 255)
            : tarjetaColor
    );

    campoNombre.setFillColor(
        campoActivo == 1
            ? sf::Color(220, 235, 255)
            : tarjetaColor
    );

    campoCorreo.setFillColor(
        campoActivo == 2
            ? sf::Color(220, 235, 255)
            : tarjetaColor
    );

    campoPassword.setFillColor(
        campoActivo == 3
            ? sf::Color(220, 235, 255)
            : tarjetaColor
    );

    campoIdentificador.setFillColor(
        campoActivo == 4
            ? sf::Color(220, 235, 255)
            : tarjetaColor
    );


    //////////////////////////////////////////////////////////
    // BORDES
    //////////////////////////////////////////////////////////

    campoRol.setOutlineColor(
        bordeTarjeta
    );

    campoNombre.setOutlineColor(
        bordeTarjeta
    );

    campoCorreo.setOutlineColor(
        bordeTarjeta
    );

    campoPassword.setOutlineColor(
        bordeTarjeta
    );

    campoIdentificador.setOutlineColor(
        bordeTarjeta
    );


    campoRol.setOutlineThickness(1.0f);
    campoNombre.setOutlineThickness(1.0f);
    campoCorreo.setOutlineThickness(1.0f);
    campoPassword.setOutlineThickness(1.0f);
    campoIdentificador.setOutlineThickness(1.0f);


    //////////////////////////////////////////////////////////
    // DIBUJAR CAMPOS
    //////////////////////////////////////////////////////////

    ventana.draw(campoRol);
    ventana.draw(campoNombre);
    ventana.draw(campoCorreo);
    ventana.draw(campoPassword);
    ventana.draw(campoIdentificador);


    //////////////////////////////////////////////////////////
    // CONTRASEÑA OCULTA
    //////////////////////////////////////////////////////////

    std::string passwordVisible =
        usuarioFormulario.password;

    if(!passwordVisible.empty())
    {
        passwordVisible =
            std::string(
                passwordVisible.size(),
                '*'
            );
    }


    //////////////////////////////////////////////////////////
    // TEXTO DE LOS CAMPOS
    //////////////////////////////////////////////////////////

    dibujarTexto(
        ventana,
        usuarioFormulario.rol,
        405.0f,
        177.0f,
        18
    );

    dibujarTexto(
        ventana,
        usuarioFormulario.nombre,
        405.0f,
        247.0f,
        18
    );

    dibujarTexto(
        ventana,
        usuarioFormulario.correo,
        405.0f,
        317.0f,
        18
    );

    dibujarTexto(
        ventana,
        passwordVisible,
        405.0f,
        387.0f,
        18
    );

    dibujarTexto(
        ventana,
        usuarioFormulario.identificador,
        405.0f,
        457.0f,
        18
    );


    //////////////////////////////////////////////////////////
    // BOTONES
    //////////////////////////////////////////////////////////

    botonAceptar.setFillColor(
        botonGris
    );

    botonCancelar.setFillColor(
        botonRojo
    );

    ventana.draw(
        botonAceptar
    );

    ventana.draw(
        botonCancelar
    );


    //////////////////////////////////////////////////////////
    // TEXTO ACEPTAR
    //////////////////////////////////////////////////////////

    sf::Text textoAceptar(
        font,
        "Aceptar",
        18
    );

    textoAceptar.setFillColor(
        textoBlanco
    );

    textoAceptar.setPosition(
        sf::Vector2f(
            515.0f,
            570.0f
        )
    );

    ventana.draw(
        textoAceptar
    );


    //////////////////////////////////////////////////////////
    // TEXTO CANCELAR
    //////////////////////////////////////////////////////////

    sf::Text textoCancelar(
        font,
        "Cancelar",
        18
    );

    textoCancelar.setFillColor(
        textoBlanco
    );

    textoCancelar.setPosition(
        sf::Vector2f(
            680.0f,
            570.0f
        )
    );

    ventana.draw(
        textoCancelar
    );
}


//////////////////////////////////////////////////////////////
// MANEJAR EVENTO
//////////////////////////////////////////////////////////////

void UsuariosView::manejarEvento(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // CONFIRMACION DE ELIMINACION
    //////////////////////////////////////////////////////////

    if(mostrandoConfirmacion)
    {
        if(const auto* mouse =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >())
        {
            sf::Vector2f posicion =
                ventana.mapPixelToCoords(
                    mouse->position
                );


            //////////////////////////////////////////////////////
            // CANCELAR
            //////////////////////////////////////////////////////

            if(puntoDentro(
                botonCancelarEliminacion,
                posicion))
            {
                cancelarEliminacion = true;

                mostrandoConfirmacion = false;

                usuarioEliminar = -1;

                return;
            }


            //////////////////////////////////////////////////////
            // CONFIRMAR ELIMINACION
            //////////////////////////////////////////////////////

            if(puntoDentro(
                botonConfirmarEliminacion,
                posicion))
            {
                aceptarEliminacion = true;

                mostrandoConfirmacion = false;

                return;
            }
        }

        return;
    }


    //////////////////////////////////////////////////////////
    // FORMULARIO
    //////////////////////////////////////////////////////////

    if(mostrandoFormulario)
    {
        if(const auto* mouse =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >())
        {
            sf::Vector2f posicion =
                ventana.mapPixelToCoords(
                    mouse->position
                );


            //////////////////////////////////////////////////////
            // ACEPTAR
            //////////////////////////////////////////////////////

            if(puntoDentro(
                botonAceptar,
                posicion))
            {
                aceptarFormulario = true;

                return;
            }


            //////////////////////////////////////////////////////
            // CANCELAR
            //////////////////////////////////////////////////////

            if(puntoDentro(
                botonCancelar,
                posicion))
            {
                cancelarFormulario = true;

                return;
            }


            //////////////////////////////////////////////////////
            // CAMPOS
            //////////////////////////////////////////////////////

            if(puntoDentro(
                campoRol,
                posicion))
            {
                activarCampo(0);
            }
            else if(puntoDentro(
                campoNombre,
                posicion))
            {
                activarCampo(1);
            }
            else if(puntoDentro(
                campoCorreo,
                posicion))
            {
                activarCampo(2);
            }
            else if(puntoDentro(
                campoPassword,
                posicion))
            {
                activarCampo(3);
            }
            else if(puntoDentro(
                campoIdentificador,
                posicion))
            {
                activarCampo(4);
            }
        }


        ////////////////////////////////////////////////////////
        // TEXTO
        ////////////////////////////////////////////////////////

        if(const auto* texto =
            evento.getIf<
                sf::Event::TextEntered
            >())
        {
            if(texto->unicode == 8)
            {
                borrarCaracter();
            }
            else if(
                texto->unicode >= 32 &&
                texto->unicode <= 126
            )
            {
                agregarCaracter(
                    static_cast<char>(
                        texto->unicode
                    )
                );
            }
        }

        return;
    }


    //////////////////////////////////////////////////////////
    // SCROLL
    //////////////////////////////////////////////////////////

    if(const auto* rueda =
        evento.getIf<
            sf::Event::MouseWheelScrolled
        >())
    {
        desplazamientoUsuarios +=
            rueda->delta * 35.0f;


        float contenido =
            static_cast<float>(
                usuarios.size()
            )
            * (
                altoTarjeta
                + espacioTarjetas
            );


        float limite =
            std::min(
                0.0f,
                650.0f - contenido
            );


        if(desplazamientoUsuarios > 0.0f)
        {
            desplazamientoUsuarios = 0.0f;
        }


        if(desplazamientoUsuarios < limite)
        {
            desplazamientoUsuarios = limite;
        }
    }
}


//////////////////////////////////////////////////////////////
// BOTON AGREGAR
//////////////////////////////////////////////////////////////

bool UsuariosView::botonAgregarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
)
{
    if(!evento.is<
        sf::Event::MouseButtonPressed>())
    {
        return false;
    }


    if(mostrandoFormulario ||
       mostrandoConfirmacion)
    {
        return false;
    }


    sf::Vector2f posicion =
        obtenerPosicionMouse(
            ventana,
            evento
        );


    if(puntoDentro(
        botonAgregar,
        posicion))
    {
        usuarioFormulario =
            UsuarioDatos{
                0,
                "",
                "",
                "",
                "",
                ""
            };

        modoEditar = false;

        mostrandoFormulario = true;

        aceptarFormulario = false;
        cancelarFormulario = false;

        campoActivo = 0;

        mensajeEstado.clear();

        return true;
    }


    return false;
}


//////////////////////////////////////////////////////////////
// BOTON EDITAR
//////////////////////////////////////////////////////////////

bool UsuariosView::botonEditarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento,
    int& indice
)
{
    if(!evento.is<
        sf::Event::MouseButtonPressed>())
    {
        return false;
    }


    if(mostrandoFormulario ||
       mostrandoConfirmacion)
    {
        return false;
    }


    sf::Vector2f posicion =
        obtenerPosicionMouse(
            ventana,
            evento
        );


    //////////////////////////////////////////////////////////
    // BUSCAR DIRECTAMENTE POR POSICION
    //
    // Esto evita problemas cuando se hace scroll.
    //////////////////////////////////////////////////////////

    for(size_t i = 0;
        i < usuarios.size();
        i++)
    {
        float y =
            inicioLista
            + static_cast<float>(i)
            * (altoTarjeta + espacioTarjetas)
            + desplazamientoUsuarios;


        sf::RectangleShape boton;

        boton.setSize(
            sf::Vector2f(
                90.0f,
                38.0f
            )
        );

        boton.setPosition(
            sf::Vector2f(
                1010.0f,
                y + 20.0f
            )
        );


        if(puntoDentro(
            boton,
            posicion))
        {
            indice =
                static_cast<int>(i);

            usuarioSeleccionado =
                indice;

            usuarioFormulario =
                usuarios[i];

            // La contraseña se debe volver a escribir.
            usuarioFormulario.password = "";

            modoEditar = true;

            mostrandoFormulario = true;

            aceptarFormulario = false;
            cancelarFormulario = false;

            campoActivo = 1;

            mensajeEstado.clear();

            return true;
        }
    }


    return false;
}


//////////////////////////////////////////////////////////////
// BOTON ELIMINAR
//////////////////////////////////////////////////////////////

bool UsuariosView::botonEliminarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento,
    int& indice
)
{
    if(!evento.is<
        sf::Event::MouseButtonPressed>())
    {
        return false;
    }


    if(mostrandoFormulario ||
       mostrandoConfirmacion)
    {
        return false;
    }


    sf::Vector2f posicion =
        obtenerPosicionMouse(
            ventana,
            evento
        );


    //////////////////////////////////////////////////////////
    // BUSCAR DIRECTAMENTE POR POSICION
    //
    // Se calcula con el scroll actual.
    //////////////////////////////////////////////////////////

    for(size_t i = 0;
        i < usuarios.size();
        i++)
    {
        float y =
            inicioLista
            + static_cast<float>(i)
            * (altoTarjeta + espacioTarjetas)
            + desplazamientoUsuarios;


        sf::RectangleShape boton;

        boton.setSize(
            sf::Vector2f(
                90.0f,
                38.0f
            )
        );

        boton.setPosition(
            sf::Vector2f(
                1010.0f,
                y + 75.0f
            )
        );


        if(puntoDentro(
            boton,
            posicion))
        {
            indice =
                static_cast<int>(i);

            usuarioEliminar =
                indice;

            usuarioSeleccionado =
                indice;

            mostrandoConfirmacion = true;

            aceptarEliminacion = false;
            cancelarEliminacion = false;

            return true;
        }
    }


    return false;
}


//////////////////////////////////////////////////////////////
// FORMULARIO ACEPTADO
//////////////////////////////////////////////////////////////

bool UsuariosView::formularioAceptado()
{
    //////////////////////////////////////////////////////////
    // SOLAMENTE SE EJECUTA DESPUES DE CLIC EN ACEPTAR
    //////////////////////////////////////////////////////////

    if(!aceptarFormulario)
    {
        return false;
    }

    aceptarFormulario = false;


    //////////////////////////////////////////////////////////
    // VALIDAR CAMPOS
    //////////////////////////////////////////////////////////

    if(usuarioFormulario.rol.empty() ||
       usuarioFormulario.nombre.empty() ||
       usuarioFormulario.correo.empty() ||
       usuarioFormulario.identificador.empty())
    {

        return false;
    }


    //////////////////////////////////////////////////////////
    // CONTRASEÑA
    //////////////////////////////////////////////////////////

    if(usuarioFormulario.password.empty())
    {
        mensajeEstado =
            "La contraseña es obligatoria.";

        return false;
    }


    return true;
}


//////////////////////////////////////////////////////////////
// FORMULARIO CANCELADO
//////////////////////////////////////////////////////////////

bool UsuariosView::formularioCancelado()
{
    if(!cancelarFormulario)
    {
        return false;
    }

    cancelarFormulario = false;

    return true;
}


//////////////////////////////////////////////////////////////
// FORMULARIO ACTIVO
//////////////////////////////////////////////////////////////

bool UsuariosView::formularioActivo() const
{
    return mostrandoFormulario;
}


//////////////////////////////////////////////////////////////
// FORMULARIO ES EDICION
//////////////////////////////////////////////////////////////

bool UsuariosView::formularioEsEdicion() const
{
    return modoEditar;
}


//////////////////////////////////////////////////////////////
// DATOS DEL FORMULARIO
//////////////////////////////////////////////////////////////

const UsuarioDatos&
UsuariosView::obtenerDatosFormulario() const
{
    return usuarioFormulario;
}


//////////////////////////////////////////////////////////////
// CERRAR FORMULARIO
//////////////////////////////////////////////////////////////

void UsuariosView::cerrarFormulario()
{
    mostrandoFormulario = false;

    modoEditar = false;

    aceptarFormulario = false;
    cancelarFormulario = false;

    campoActivo = -1;
}


//////////////////////////////////////////////////////////////
// MOSTRAR MENSAJE
//////////////////////////////////////////////////////////////

void UsuariosView::mostrarMensaje(
    const std::string& mensaje
)
{
    mensajeEstado =
        mensaje;
}


//////////////////////////////////////////////////////////////
// CONFIRMACION ACTIVA
//////////////////////////////////////////////////////////////

bool UsuariosView::confirmacionActiva() const
{
    return mostrandoConfirmacion;
}


//////////////////////////////////////////////////////////////
// CONFIRMACION ACEPTADA
//////////////////////////////////////////////////////////////

bool UsuariosView::confirmacionAceptada()
{
    if(!aceptarEliminacion)
    {
        return false;
    }

    aceptarEliminacion = false;

    return true;
}


//////////////////////////////////////////////////////////////
// CONFIRMACION CANCELADA
//////////////////////////////////////////////////////////////

bool UsuariosView::confirmacionCancelada()
{
    if(!cancelarEliminacion)
    {
        return false;
    }

    cancelarEliminacion = false;

    return true;
}


//////////////////////////////////////////////////////////////
// OBTENER USUARIO A ELIMINAR
//////////////////////////////////////////////////////////////

int UsuariosView::obtenerUsuarioEliminar() const
{
    return usuarioEliminar;
}


//////////////////////////////////////////////////////////////
// DIBUJAR CONFIRMACION
//////////////////////////////////////////////////////////////

void UsuariosView::dibujarConfirmacion(
    sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // FONDO OSCURO
    //////////////////////////////////////////////////////////

    sf::RectangleShape fondo;

    fondo.setSize(
        sf::Vector2f(
            1280.0f,
            720.0f
        )
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(
        fondo
    );


    //////////////////////////////////////////////////////////
    // VENTANA
    //////////////////////////////////////////////////////////

    sf::RectangleShape ventanaConfirmacion;

    ventanaConfirmacion.setSize(
        sf::Vector2f(
            500.0f,
            250.0f
        )
    );

    ventanaConfirmacion.setPosition(
        sf::Vector2f(
            390.0f,
            235.0f
        )
    );

    ventanaConfirmacion.setFillColor(
        tarjetaColor
    );

    ventanaConfirmacion.setOutlineColor(
        bordeTarjeta
    );

    ventanaConfirmacion.setOutlineThickness(
        1.0f
    );

    ventana.draw(
        ventanaConfirmacion
    );


    //////////////////////////////////////////////////////////
    // TITULO
    //////////////////////////////////////////////////////////

    sf::Text titulo(
        font,
        "Eliminar usuario",
        25
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        sf::Vector2f(
            430.0f,
            265.0f
        )
    );

    ventana.draw(
        titulo
    );


    //////////////////////////////////////////////////////////
    // NOMBRE
    //////////////////////////////////////////////////////////

    std::string nombreUsuario = "";

    if(
        usuarioEliminar >= 0 &&
        usuarioEliminar <
            static_cast<int>(usuarios.size())
    )
    {
        nombreUsuario =
            usuarios[usuarioEliminar].nombre;
    }


    sf::Text mensaje(
        font,
        "¿Deseas eliminar este usuario?",
        19
    );

    mensaje.setFillColor(
        textoNegro
    );

    mensaje.setPosition(
        sf::Vector2f(
            430.0f,
            315.0f
        )
    );

    ventana.draw(
        mensaje
    );


    sf::Text nombre(
        font,
        nombreUsuario,
        18
    );

    nombre.setFillColor(
        textoNegro
    );

    nombre.setPosition(
        sf::Vector2f(
            430.0f,
            350.0f
        )
    );

    ventana.draw(
        nombre
    );


    //////////////////////////////////////////////////////////
    // BOTON CANCELAR
    //////////////////////////////////////////////////////////

    botonCancelarEliminacion.setFillColor(
        botonGris
    );

    ventana.draw(
        botonCancelarEliminacion
    );


    sf::Text textoCancelar(
        font,
        "Cancelar",
        17
    );

    textoCancelar.setFillColor(
        textoBlanco
    );

    textoCancelar.setPosition(
        sf::Vector2f(
            487.0f,
            405.0f
        )
    );

    ventana.draw(
        textoCancelar
    );


    //////////////////////////////////////////////////////////
    // BOTON ELIMINAR
    //////////////////////////////////////////////////////////

    botonConfirmarEliminacion.setFillColor(
        botonRojo
    );

    ventana.draw(
        botonConfirmarEliminacion
    );


    sf::Text textoEliminar(
        font,
        "Eliminar",
        17
    );

    textoEliminar.setFillColor(
        textoBlanco
    );

    textoEliminar.setPosition(
        sf::Vector2f(
            650.0f,
            405.0f
        )
    );

    ventana.draw(
        textoEliminar
    );
}


//////////////////////////////////////////////////////////////
// DRAW
//////////////////////////////////////////////////////////////

void UsuariosView::draw(
    sf::RenderWindow& ventana
)
{
    sf::View vistaAnterior =
        ventana.getView();


    sf::View vista(
        sf::FloatRect(
            sf::Vector2f(
                0.0f,
                0.0f
            ),
            sf::Vector2f(
                1280.0f,
                720.0f
            )
        )
    );


    ventana.setView(
        vista
    );


    //////////////////////////////////////////////////////////
    // FONDO
    //////////////////////////////////////////////////////////

    ventana.clear(
        fondoScroll
    );


    //////////////////////////////////////////////////////////
    // ENCABEZADO
    //////////////////////////////////////////////////////////

    sf::RectangleShape encabezado;

    encabezado.setSize(
        sf::Vector2f(
            1280.0f,
            95.0f
        )
    );

    encabezado.setPosition(
        sf::Vector2f(
            0.0f,
            0.0f
        )
    );

    encabezado.setFillColor(
        encabezadoColor
    );

    ventana.draw(
        encabezado
    );


    //////////////////////////////////////////////////////////
    // TITULO
    //////////////////////////////////////////////////////////

    sf::Text titulo(
        font,
        "ADMINISTRACION DE USUARIOS",
        30
    );

    titulo.setFillColor(
        textoBlanco
    );

    titulo.setPosition(
        sf::Vector2f(
            25.0f,
            15.0f
        )
    );

    ventana.draw(
        titulo
    );


    //////////////////////////////////////////////////////////
    // BOTONES
    //////////////////////////////////////////////////////////

    dibujarBotones(
        ventana
    );


    //////////////////////////////////////////////////////////
    // USUARIOS
    //////////////////////////////////////////////////////////

    dibujarUsuarios(
        ventana
    );


    //////////////////////////////////////////////////////////
    // MENSAJE
    //////////////////////////////////////////////////////////

    if(!mensajeEstado.empty())
    {
        dibujarTexto(
            ventana,
            mensajeEstado,
            60.0f,
            680.0f,
            18
        );
    }


    //////////////////////////////////////////////////////////
    // FORMULARIO
    //////////////////////////////////////////////////////////

    if(mostrandoFormulario)
    {
        dibujarFormulario(
            ventana
        );
    }


    //////////////////////////////////////////////////////////
    // CONFIRMACION
    //////////////////////////////////////////////////////////

    if(mostrandoConfirmacion)
    {
        dibujarConfirmacion(
            ventana
        );
    }


    //////////////////////////////////////////////////////////
    // RESTAURAR VISTA
    //////////////////////////////////////////////////////////

    ventana.setView(
        vistaAnterior
    );
}
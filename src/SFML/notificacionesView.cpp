#include "SFML/notificacionesView.hpp"

#include <algorithm>
#include <sstream>

// ============================================================
// CONSTRUCTOR
// ============================================================

NotificacionesView::NotificacionesView()
   : inicioLista(105.f),
     altoTarjeta(150.f),
     espacioTarjetas(15.f),
     desplazamientoNotificaciones(0.f),
     indiceNotificacionSeleccionada(-1),
     indiceEliminarPendiente(-1),
     mensajeEstado("")
{

    botonRegresar.setSize(
        sf::Vector2f(220.f, 45.f)
    );

    botonRegresar.setFillColor(
        sf::Color(70, 90, 110)
    );
}


// ============================================================
// CARGAR FUENTE
// ============================================================

bool NotificacionesView::cargarFuente(
    const std::string& ruta
)
{
    return font.openFromFile(ruta);
}


// ============================================================
// ACTUALIZAR GEOMETRÍA
// ============================================================

void NotificacionesView::actualizarGeometria(
    const sf::RenderWindow& ventana
)
{
    const float ancho = 1280.f;
    const float alto = 720.f;

    (void)ventana;

    botonRegresar.setPosition(
        sf::Vector2f(
            ancho - 250.f,
            15.f
        )
    );
}


// ============================================================
// DIBUJAR TEXTO
// ============================================================

void NotificacionesView::dibujarTexto(
    sf::RenderWindow& ventana,
    const std::string& texto,
    unsigned int tamano,
    float x,
    float y
) const
{
    sf::Text textoDibujar(
        font,
        texto,
        tamano
    );

    textoDibujar.setPosition(
        sf::Vector2f(x, y)
    );

    textoDibujar.setFillColor(
        sf::Color(40, 40, 40)
    );

    ventana.draw(textoDibujar);
}


// ============================================================
// DIBUJAR ENCABEZADO
// ============================================================

void NotificacionesView::dibujarEncabezado(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape encabezado(
        sf::Vector2f(1280.f, 75.f)
    );

    encabezado.setPosition(
        sf::Vector2f(0.f, 0.f)
    );

    encabezado.setFillColor(
        sf::Color(41, 53, 65)
    );

    ventana.draw(encabezado);


    sf::Text titulo(
        font,
        "Notificaciones",
        32
    );

    titulo.setPosition(
        sf::Vector2f(40.f, 20.f)
    );

    titulo.setFillColor(
        sf::Color::White
    );

    ventana.draw(titulo);


    botonRegresar.setFillColor(
        sf::Color(41, 53, 65)
    );

    botonRegresar.setOutlineColor(
        sf::Color::White
    );

    botonRegresar.setOutlineThickness(
        1.f
    );

    ventana.draw(botonRegresar);


    sf::Text textoRegresar(
        font,
        "  Regresar",
        23
    );

    sf::FloatRect limites = textoRegresar.getLocalBounds();

    textoRegresar.setPosition(
        sf::Vector2f(
            botonRegresar.getPosition().x +
                (botonRegresar.getSize().x -
                 limites.size.x) / 2.f,

            botonRegresar.getPosition().y +
                (botonRegresar.getSize().y -
                 limites.size.y) / 2.f -
                limites.position.y
        )
    );

    textoRegresar.setFillColor(
        sf::Color::White
    );

    ventana.draw(textoRegresar);
}


// ============================================================
// DIBUJAR NOTIFICACIÓN
// ============================================================

void NotificacionesView::dibujarNotificacion(
    sf::RenderWindow& ventana,
    const Notificacion& notificacion,
    size_t indice
)
{
    const float x = 55.f;

    const float y =
        inicioLista +
        static_cast<float>(indice) *
        (altoTarjeta + espacioTarjetas) -
        desplazamientoNotificaciones;

    const float ancho = 1170.f;


    // ========================================================
    // TARJETA
    // ========================================================

    sf::RectangleShape tarjeta(
        sf::Vector2f(
            ancho,
            altoTarjeta
        )
    );

    tarjeta.setPosition(
        sf::Vector2f(x, y)
    );


    if(notificacion.estaLeida())
    {
        tarjeta.setFillColor(
            sf::Color(235, 238, 240)
        );

        tarjeta.setOutlineColor(
            sf::Color(190, 195, 200)
        );
    }
    else
    {
        tarjeta.setFillColor(
            sf::Color(225, 238, 248)
        );

        tarjeta.setOutlineColor(
            sf::Color(70, 130, 180)
        );
    }

    tarjeta.setOutlineThickness(
        2.f
    );

    ventana.draw(tarjeta);


    // ========================================================
    // INDICADOR DE NO LEÍDA
    // ========================================================

    if(!notificacion.estaLeida())
    {
        sf::CircleShape indicador(
            7.f
        );

        indicador.setPosition(
            sf::Vector2f(
                x + 18.f,
                y + 18.f
            )
        );

        indicador.setFillColor(
            sf::Color(60, 130, 200)
        );

        ventana.draw(indicador);
    }


    // ========================================================
    // ESTADO DE LA NOTIFICACIÓN
    // ========================================================

    std::string estadoNotificacion = notificacion.estaLeida()
        ? "Leida"
        : "  No leida";

    dibujarTexto(ventana, estadoNotificacion,
        17, x + 25.f, y + 13.f);

    // ========================================================
    // FECHA
    // ========================================================

    sf::Text fecha(
        font,
        notificacion.getFecha(),
        15
    );

    sf::FloatRect limitesFecha =
        fecha.getLocalBounds();

    fecha.setPosition(
        sf::Vector2f(
            x + ancho -
                limitesFecha.size.x -
                25.f,

            y + 15.f
        )
    );

    fecha.setFillColor(
        sf::Color(100, 100, 100)
    );

    ventana.draw(fecha);


    // ========================================================
    // TÍTULO
    // ========================================================

    sf::Text titulo(
        font,
        notificacion.getTitulo(),
        22
    );

    titulo.setPosition(
        sf::Vector2f(
            x + 25.f,
            y + 42.f
        )
    );

    titulo.setFillColor(
        sf::Color(30, 30, 30)
    );

    ventana.draw(titulo);


    // ========================================================
    // REFERENCIA
    // ========================================================

    std::string referencia =
        obtenerReferenciaTexto(
            notificacion
        );

    if(!referencia.empty())
    {
        dibujarTexto(
            ventana,
            referencia,
            15,
            x + 25.f,
            y + 75.f
        );
    }


    // ========================================================
    // MENSAJE
    // ========================================================

    std::string mensaje =
        notificacion.getMensaje();

    if(mensaje.size() > 75)
    {
        mensaje =
            mensaje.substr(0, 72) +
            "...";
    }

    dibujarTexto(
        ventana,
        mensaje,
        15,
        x + 25.f,
        y + 98.f
    );


    // ========================================================
    // BOTÓN MARCAR COMO LEÍDA
    // ========================================================

    botonesLeer[indice].setSize(
        sf::Vector2f(
            170.f,
            30.f
        )
    );

    botonesLeer[indice].setPosition(
        sf::Vector2f(
            x + 690.f,
            y + 100.f
        )
    );

    if(notificacion.estaLeida())
    {
        botonesLeer[indice].setFillColor(
            sf::Color(150, 155, 160)
        );
    }
    else
    {
        botonesLeer[indice].setFillColor(
            sf::Color(60, 130, 180)
        );
    }

    ventana.draw(
        botonesLeer[indice]
    );


    std::string textoLeer =
        notificacion.estaLeida()
            ? "Leida"
            : "Marcar como leida";

    sf::Text textoBotonLeer(
        font,
        textoLeer,
        14
    );

    sf::FloatRect limitesLeer =
        textoBotonLeer.getLocalBounds();

    textoBotonLeer.setPosition(
        sf::Vector2f(
            botonesLeer[indice].getPosition().x +
                (botonesLeer[indice].getSize().x -
                 limitesLeer.size.x) / 2.f,

            botonesLeer[indice].getPosition().y +
                (botonesLeer[indice].getSize().y -
                 limitesLeer.size.y) / 2.f -
                limitesLeer.position.y
        )
    );

    textoBotonLeer.setFillColor(
        sf::Color::White
    );

    ventana.draw(
        textoBotonLeer
    );


    // ========================================================
    // BOTÓN ELIMINAR
    // ========================================================

    botonesEliminar[indice].setSize(
        sf::Vector2f(
            105.f,
            30.f
        )
    );

    botonesEliminar[indice].setPosition(
        sf::Vector2f(
            x + 875.f,
            y + 100.f
        )
    );

    botonesEliminar[indice].setFillColor(
        sf::Color(180, 70, 70)
    );

    ventana.draw(
        botonesEliminar[indice]
    );


   std::string textoEliminar;
    if(static_cast<int>(indice) == indiceEliminarPendiente)
    {
        textoEliminar = "Confirmar";
    }
    else
    {
        textoEliminar = "Eliminar";
    }

    sf::Text textoEliminarDibujar(font, textoEliminar,14);

    sf::FloatRect limitesEliminar = textoEliminarDibujar.getLocalBounds();

    textoEliminarDibujar.setPosition(sf::Vector2f(botonesEliminar[indice].getPosition().x
         + (botonesEliminar[indice].getSize().x - limitesEliminar.size.x) / 2.f,

        botonesEliminar[indice].getPosition().y +
            (botonesEliminar[indice].getSize().y -
             limitesEliminar.size.y) / 2.f -
            limitesEliminar.position.y
    )
);

textoEliminarDibujar.setFillColor(
    sf::Color::White
);

ventana.draw(
    textoEliminarDibujar
);
}


// ============================================================
// DIBUJAR LISTA
// ============================================================

void NotificacionesView::dibujarLista(
    sf::RenderWindow& ventana
)
{
    if(notificaciones.empty())
    {
        dibujarMensajeVacio(
            ventana
        );

        return;
    }


    botonesLeer.resize(
        notificaciones.size()
    );

    botonesEliminar.resize(
        notificaciones.size()
    );


    for(size_t i = 0;
        i < notificaciones.size();
        ++i)
    {
        float y =
            inicioLista +
            static_cast<float>(i) *
            (altoTarjeta + espacioTarjetas) -
            desplazamientoNotificaciones;

        if(
            y + altoTarjeta < 75.f ||
            y > 720.f
        )
        {
            continue;
        }

        dibujarNotificacion(
            ventana,
            notificaciones[i],
            i
        );
    }
}


// ============================================================
// MENSAJE VACÍO
// ============================================================

void NotificacionesView::dibujarMensajeVacio(
    sf::RenderWindow& ventana
)
{
    sf::Text texto(
        font,
        "No tienes notificaciones.",
        24
    );

    sf::FloatRect limites =
        texto.getLocalBounds();

    texto.setPosition(
        sf::Vector2f(
            (1280.f - limites.size.x) / 2.f,
            330.f
        )
    );

    texto.setFillColor(
        sf::Color(100, 100, 100)
    );

    ventana.draw(texto);
}


// ============================================================
// OBTENER NOMBRE DE TAREA
// ============================================================

std::string NotificacionesView::obtenerNombreTarea(
    int idTarea
) const
{
    for(const auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            return tarea.getTitulo();
        }
    }

    return "";
}


// ============================================================
// OBTENER NOMBRE DE MATERIA
// ============================================================

std::string NotificacionesView::obtenerNombreMateria(
    int idMateria
) const
{
    for(const auto& materia : materias)
    {
        if(materia.getId() == idMateria)
        {
            return materia.getNombre();
        }
    }

    return "";
}


// ============================================================
// OBTENER ID DE MATERIA DE UNA TAREA
// ============================================================

int NotificacionesView::obtenerIdMateriaDeTarea(
    int idTarea
) const
{
    for(const auto& tarea : tareas)
    {
        if(tarea.getId() == idTarea)
        {
            return tarea.getMateriaId();
        }
    }

    return 0;
}


// ============================================================
// OBTENER REFERENCIA
// ============================================================

std::string NotificacionesView::obtenerReferenciaTexto(
    const Notificacion& notificacion
) const
{
    int idReferencia =
        notificacion.getIdReferencia();

    TipoReferenciaNotificacion tipo =
        notificacion.getTipoReferencia();


    // ========================================================
    // TAREA
    // ========================================================

    if(tipo == TipoReferenciaNotificacion::TAREA)
    {
        std::string nombreTarea =
            obtenerNombreTarea(
                idReferencia
            );

        int idMateria =
            obtenerIdMateriaDeTarea(
                idReferencia
            );

        std::string nombreMateria =
            obtenerNombreMateria(
                idMateria
            );

        if(
            !nombreTarea.empty() &&
            !nombreMateria.empty()
        )
        {
            return
                "Tarea: " +
                nombreTarea +
                "  |  Materia: " +
                nombreMateria;
        }

        if(!nombreTarea.empty())
        {
            return
                "Tarea: " +
                nombreTarea;
        }

        return
            "Referencia: Tarea #" +
            std::to_string(idReferencia);
    }


    // ========================================================
    // SUBTAREA
    // ========================================================

    if(tipo == TipoReferenciaNotificacion::SUBTAREA)
    {
        return
            "Subtarea #" +
            std::to_string(idReferencia);
    }


    // ========================================================
    // MATERIA
    // ========================================================

    if(tipo == TipoReferenciaNotificacion::MATERIA)
    {
        std::string nombreMateria =
            obtenerNombreMateria(
                idReferencia
            );

        if(!nombreMateria.empty())
        {
            return
                "Materia: " +
                nombreMateria;
        }

        return
            "Materia #" +
            std::to_string(idReferencia);
    }


    // ========================================================
    // PROFESOR
    // ========================================================

    if(tipo == TipoReferenciaNotificacion::PROFESOR)
    {
        return
            "Profesor #" +
            std::to_string(idReferencia);
    }


    return "";
}


// ============================================================
// TIPO DE NOTIFICACIÓN A TEXTO
// ============================================================

std::string NotificacionesView::tipoNotificacionAString(
    TipoNotificacion tipo
) const
{
    switch(tipo)
    {
        case TipoNotificacion::RECORDATORIO:
            return "Recordatorio";

        case TipoNotificacion::NUEVA_TAREA:
            return "Nueva tarea";

        case TipoNotificacion::CAMBIO_FECHA:
            return "Cambio de fecha";

        case TipoNotificacion::MENSAJE_PROFESOR:
            return "Mensaje del profesor";

        case TipoNotificacion::SISTEMA:
            return "Sistema";
    }

    return "Notificación";
}


// ============================================================
// TIPO DE REFERENCIA A TEXTO
// ============================================================

std::string NotificacionesView::tipoReferenciaAString(
    TipoReferenciaNotificacion tipo
) const
{
    switch(tipo)
    {
        case TipoReferenciaNotificacion::NINGUNA:
            return "Ninguna";

        case TipoReferenciaNotificacion::TAREA:
            return "Tarea";

        case TipoReferenciaNotificacion::SUBTAREA:
            return "Subtarea";

        case TipoReferenciaNotificacion::MATERIA:
            return "Materia";

        case TipoReferenciaNotificacion::PROFESOR:
            return "Profesor";
    }

    return "";
}


// ============================================================
// POSICIÓN DEL MOUSE
// ============================================================

sf::Vector2f NotificacionesView::obtenerPosicionMouse(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
) const
{
    const auto* clic =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(clic == nullptr)
    {
        return {0.f, 0.f};
    }


    sf::View vistaLogica(
        sf::FloatRect(
            {0.f, 0.f},
            {1280.f, 720.f}
        )
    );


    return ventana.mapPixelToCoords(
        {
            clic->position.x,
            clic->position.y
        },
        vistaLogica
    );
}


// ============================================================
// SET NOTIFICACIONES
// ============================================================

void NotificacionesView::setNotificaciones(
    const std::vector<Notificacion>& nuevasNotificaciones
)
{
    notificaciones =
        nuevasNotificaciones;

    botonesLeer.resize(
        notificaciones.size()
    );

    botonesEliminar.resize(
        notificaciones.size()
    );


    if(
        indiceNotificacionSeleccionada >=
        static_cast<int>(notificaciones.size())
    )
    {
        indiceNotificacionSeleccionada =
            -1;
    }


    indiceEliminarPendiente = -1;


    reiniciarDesplazamiento();
}


// ============================================================
// LIMPIAR NOTIFICACIONES
// ============================================================

void NotificacionesView::limpiarNotificaciones()
{
    notificaciones.clear();

    botonesLeer.clear();

    botonesEliminar.clear();

    indiceNotificacionSeleccionada =
        -1;

    indiceEliminarPendiente =
        -1;

    reiniciarDesplazamiento();
}


// ============================================================
// OBTENER NOTIFICACIONES
// ============================================================

const std::vector<Notificacion>&
NotificacionesView::obtenerNotificaciones() const
{
    return notificaciones;
}


// ============================================================
// SET TAREAS
// ============================================================

void NotificacionesView::setTareas(
    const std::vector<Tarea>& nuevasTareas
)
{
    tareas =
        nuevasTareas;
}


// ============================================================
// SET MATERIAS
// ============================================================

void NotificacionesView::setMaterias(
    const std::vector<Materia>& nuevasMaterias
)
{
    materias =
        nuevasMaterias;
}


// ============================================================
// MANEJAR EVENTO
// ============================================================

void NotificacionesView::manejarEvento(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    // ========================================================
    // RUEDA DEL MOUSE
    // ========================================================

    if(
        const auto* rueda =
            evento.getIf<
                sf::Event::MouseWheelScrolled
            >()
    )
    {
        desplazamientoNotificaciones -=
            rueda->delta * 35.f;

        if(desplazamientoNotificaciones < 0.f)
        {
            desplazamientoNotificaciones =
                0.f;
        }


        float contenido =
            static_cast<float>(
                notificaciones.size()
            ) *
            (altoTarjeta + espacioTarjetas);


        float limite =
            std::max(
                0.f,
                contenido -
                (720.f - inicioLista) +
                10.f
            );


        if(
            desplazamientoNotificaciones >
            limite
        )
        {
            desplazamientoNotificaciones =
                limite;
        }

        return;
    }


    // ========================================================
    // MOUSE
    // ========================================================

    if(
        evento.is<
            sf::Event::MouseButtonPressed
        >()
    )
    {
        sf::Vector2f posicion =
            obtenerPosicionMouse(
                evento,
                ventana
            );


        // ====================================================
        // SELECCIONAR NOTIFICACIÓN
        // ====================================================

        for(size_t i = 0;
            i < notificaciones.size();
            ++i)
        {
            float y =
                inicioLista +
                static_cast<float>(i) *
                (altoTarjeta + espacioTarjetas) -
                desplazamientoNotificaciones;


            sf::FloatRect areaTarjeta(
                {
                    55.f,
                    y
                },
                {
                    1170.f,
                    altoTarjeta
                }
            );


            if(
                areaTarjeta.contains(
                    posicion
                )
            )
            {
                indiceNotificacionSeleccionada =
                    static_cast<int>(i);

                break;
            }
        }
    }
}


// ============================================================
// DIBUJAR
// ============================================================

void NotificacionesView::draw(
    sf::RenderWindow& ventana
)
{
    actualizarGeometria(
        ventana
    );


    // ========================================================
    // GUARDAR VISTA ACTUAL
    // ========================================================

    sf::View vistaAnterior =
        ventana.getView();


    // ========================================================
    // VISTA LÓGICA 1280 x 720
    // ========================================================

    sf::View vistaLogica(
        sf::FloatRect(
            {0.f, 0.f},
            {1280.f, 720.f}
        )
    );


    ventana.setView(
        vistaLogica
    );


    // ========================================================
    // FONDO
    // ========================================================

    ventana.clear(
        sf::Color(245, 246, 248)
    );


    // ========================================================
    // ENCABEZADO
    // ========================================================

    dibujarEncabezado(
        ventana
    );


    // ========================================================
    // LISTA
    // ========================================================

    dibujarLista(
        ventana
    );


    // ========================================================
    // MENSAJE DE ESTADO
    // ========================================================

    if(!mensajeEstado.empty())
    {
        dibujarTexto(
            ventana,
            mensajeEstado,
            16,
            55.f,
            685.f
        );
    }


    // ========================================================
    // RESTAURAR VISTA
    // ========================================================

    ventana.setView(
        vistaAnterior
    );
}


// ============================================================
// BOTÓN REGRESAR
// ============================================================

bool NotificacionesView::botonRegresarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(
        const auto* clic =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >()
    )
    {
        sf::View vistaLogica(
            sf::FloatRect(
                {0.f, 0.f},
                {1280.f, 720.f}
            )
        );


        sf::Vector2f posicion =
            ventana.mapPixelToCoords(
                {
                    clic->position.x,
                    clic->position.y
                },
                vistaLogica
            );


        return botonRegresar
            .getGlobalBounds()
            .contains(posicion);
    }


    return false;
}


// ============================================================
// BOTÓN MARCAR COMO LEÍDA
// ============================================================

bool NotificacionesView::botonMarcarLeidaPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento,
    int& indice
) const
{
    if(
        const auto* clic =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >()
    )
    {
        sf::View vistaLogica(
            sf::FloatRect(
                {0.f, 0.f},
                {1280.f, 720.f}
            )
        );


        sf::Vector2f posicion =
            ventana.mapPixelToCoords(
                {
                    clic->position.x,
                    clic->position.y
                },
                vistaLogica
            );


        for(size_t i = 0;
            i < notificaciones.size();
            ++i)
        {
            float y =
                inicioLista +
                static_cast<float>(i) *
                (altoTarjeta + espacioTarjetas) -
                desplazamientoNotificaciones;


            sf::FloatRect area =
                botonesLeer[i]
                    .getGlobalBounds();


            area.position.y =
                y + 100.f;

            area.position.x =
                55.f + 690.f;


            if(
                area.contains(
                    posicion
                )
            )
            {
                indice =
                    static_cast<int>(i);

                return true;
            }
        }
    }


    return false;
}


// ============================================================
// BOTÓN ELIMINAR - DOBLE CLIC
// ============================================================

bool NotificacionesView::botonEliminarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento,
    int& indice
)
{
    if(
        const auto* clic =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >()
    )
    {
        sf::View vistaLogica(
            sf::FloatRect(
                {0.f, 0.f},
                {1280.f, 720.f}
            )
        );

        sf::Vector2f posicion =
            ventana.mapPixelToCoords(
                {
                    clic->position.x,
                    clic->position.y
                },
                vistaLogica
            );


        for(size_t i = 0;
            i < notificaciones.size();
            ++i)
        {
            float y =
                inicioLista +
                static_cast<float>(i) *
                (altoTarjeta + espacioTarjetas) -
                desplazamientoNotificaciones;


            sf::FloatRect area(
                {
                    55.f + 875.f,
                    y + 100.f
                },
                {
                    105.f,
                    30.f
                }
            );


            if(
                area.contains(
                    posicion
                )
            )
            {
                int indiceActual =
                    static_cast<int>(i);


                // ====================================================
                // SEGUNDO CLIC
                // ====================================================

                if(
                    indiceEliminarPendiente ==
                    indiceActual
                )
                {
                    if(
                        relojDobleClic.getElapsedTime()
                            .asSeconds() <= 0.5f
                    )
                    {
                        indice =
                            indiceActual;

                        indiceEliminarPendiente =
                            -1;

                        relojDobleClic.restart();

                        return true;
                    }
                }


                // ====================================================
                // PRIMER CLIC
                // ====================================================

                indiceEliminarPendiente =
                    indiceActual;

                relojDobleClic.restart();

                return false;
            }
        }
    }


    // ========================================================
    // CANCELAR CONFIRMACIÓN SI PASÓ EL TIEMPO
    // ========================================================

    if(
        indiceEliminarPendiente != -1 &&
        relojDobleClic.getElapsedTime()
            .asSeconds() > 0.5f
    )
    {
        indiceEliminarPendiente = -1;
    }


    return false;
}


// ============================================================
// NOTIFICACIÓN PRESIONADA
// ============================================================

bool NotificacionesView::notificacionPresionada(
    const sf::RenderWindow& ventana,
    const sf::Event& evento,
    int& indice
) const
{
    if(
        const auto* clic =
            evento.getIf<
                sf::Event::MouseButtonPressed
            >()
    )
    {
        sf::View vistaLogica(
            sf::FloatRect(
                {0.f, 0.f},
                {1280.f, 720.f}
            )
        );


        sf::Vector2f posicion =
            ventana.mapPixelToCoords(
                {
                    clic->position.x,
                    clic->position.y
                },
                vistaLogica
            );


        for(size_t i = 0;
            i < notificaciones.size();
            ++i)
        {
            float y =
                inicioLista +
                static_cast<float>(i) *
                (altoTarjeta + espacioTarjetas) -
                desplazamientoNotificaciones;


            sf::FloatRect area(
                {
                    55.f,
                    y
                },
                {
                    1170.f,
                    altoTarjeta
                }
            );


            if(
                area.contains(
                    posicion
                )
            )
            {
                indice =
                    static_cast<int>(i);

                return true;
            }
        }
    }


    return false;
}


// ============================================================
// OBTENER ID SELECCIONADO
// ============================================================

int NotificacionesView::obtenerIdNotificacionSeleccionada() const
{
    if(
        indiceNotificacionSeleccionada < 0 ||
        indiceNotificacionSeleccionada >=
            static_cast<int>(
                notificaciones.size()
            )
    )
    {
        return 0;
    }


    return notificaciones[
        indiceNotificacionSeleccionada
    ].getId();
}


// ============================================================
// DESPLAZAR
// ============================================================

void NotificacionesView::desplazar(
    float cantidad
)
{
    desplazamientoNotificaciones +=
        cantidad;


    if(desplazamientoNotificaciones < 0.f)
    {
        desplazamientoNotificaciones =
            0.f;
    }


    float contenido =
        static_cast<float>(
            notificaciones.size()
        ) *
        (altoTarjeta + espacioTarjetas);


    float limite =
        std::max(
            0.f,
            contenido -
            (720.f - inicioLista) +
            10.f
        );


    if(
        desplazamientoNotificaciones >
        limite
    )
    {
        desplazamientoNotificaciones =
            limite;
    }
}


// ============================================================
// REINICIAR DESPLAZAMIENTO
// ============================================================

void NotificacionesView::reiniciarDesplazamiento()
{
    desplazamientoNotificaciones =
        0.f;
}


// ============================================================
// MENSAJE
// ============================================================

void NotificacionesView::setMensaje(
    const std::string& mensaje
)
{
    mensajeEstado =
        mensaje;
}


// ============================================================
// OBTENER MENSAJE
// ============================================================

std::string NotificacionesView::obtenerMensaje() const
{
    return mensajeEstado;
}
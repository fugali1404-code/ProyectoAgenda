#ifndef NOTIFICACIONES_VIEW_HPP
#define NOTIFICACIONES_VIEW_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "notificaciones.hpp"
#include "tarea.hpp"
#include "materia.hpp"

class NotificacionesView
{
private:

    // ============================================================
    // FUENTE
    // ============================================================

    sf::Font font;


    // ============================================================
    // DATOS
    // ============================================================

    std::vector<Notificacion> notificaciones;

    std::vector<Tarea> tareas;

    std::vector<Materia> materias;


    // ============================================================
    // GEOMETRÍA
    // ============================================================

    float inicioLista;

    float altoTarjeta;

    float espacioTarjetas;

    float desplazamientoNotificaciones;


    // ============================================================
    // NOTIFICACIÓN SELECCIONADA
    // ============================================================

    int indiceNotificacionSeleccionada;

    // ============================================================
    // CONFIRMACIÓN DE ELIMINACIÓN
    // ============================================================

    int indiceEliminarPendiente;

    sf::Clock relojDobleClic;



    // ============================================================
    // BOTONES POR NOTIFICACIÓN
    // ============================================================

    std::vector<sf::RectangleShape> botonesLeer;

    std::vector<sf::RectangleShape> botonesEliminar;


    // ============================================================
    // BOTÓN REGRESAR
    // ============================================================

    sf::RectangleShape botonRegresar;


    // ============================================================
    // MENSAJES
    // ============================================================

    std::string mensajeEstado;


    // ============================================================
    // GEOMETRÍA
    // ============================================================

    void actualizarGeometria(
        const sf::RenderWindow& ventana
    );


    // ============================================================
    // DIBUJAR
    // ============================================================

    void dibujarTexto(
        sf::RenderWindow& ventana,
        const std::string& texto,
        unsigned int tamano,
        float x,
        float y
    ) const;

    void dibujarEncabezado(
        sf::RenderWindow& ventana
    );

    void dibujarNotificacion(
        sf::RenderWindow& ventana,
        const Notificacion& notificacion,
        size_t indice
    );

    void dibujarLista(
        sf::RenderWindow& ventana
    );

    void dibujarMensajeVacio(
        sf::RenderWindow& ventana
    );


    // ============================================================
    // INFORMACIÓN DE REFERENCIA
    // ============================================================

    std::string obtenerNombreTarea(
        int idTarea
    ) const;

    std::string obtenerNombreMateria(
        int idMateria
    ) const;

    int obtenerIdMateriaDeTarea(
        int idTarea
    ) const;

    std::string obtenerReferenciaTexto(
        const Notificacion& notificacion
    ) const;


    // ============================================================
    // TIPO DE NOTIFICACIÓN
    // ============================================================

    std::string tipoNotificacionAString(
        TipoNotificacion tipo
    ) const;

    std::string tipoReferenciaAString(
        TipoReferenciaNotificacion tipo
    ) const;


    // ============================================================
    // POSICIÓN DEL MOUSE
    // ============================================================

    sf::Vector2f obtenerPosicionMouse(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    ) const;


public:

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    NotificacionesView();


    // ============================================================
    // FUENTE
    // ============================================================

    bool cargarFuente(
        const std::string& ruta
    );


    // ============================================================
    // DATOS
    // ============================================================

    void setNotificaciones(
        const std::vector<Notificacion>& nuevasNotificaciones
    );

    void limpiarNotificaciones();

    const std::vector<Notificacion>&
    obtenerNotificaciones() const;


    // ============================================================
    // TAREAS Y MATERIAS
    // ============================================================

    void setTareas(
        const std::vector<Tarea>& nuevasTareas
    );

    void setMaterias(
        const std::vector<Materia>& nuevasMaterias
    );


    // ============================================================
    // EVENTOS
    // ============================================================

    void manejarEvento(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );


    // ============================================================
    // DIBUJAR
    // ============================================================

    void draw(
        sf::RenderWindow& ventana
    );


    // ============================================================
    // BOTÓN REGRESAR
    // ============================================================

    bool botonRegresarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;


    // ============================================================
    // MARCAR COMO LEÍDA
    // ============================================================

    bool botonMarcarLeidaPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento,
        int& indice
    ) const;


    // ============================================================
    // ELIMINAR
    // ============================================================

    bool botonEliminarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento,
        int& indice
    );


    // ============================================================
    // SELECCIÓN
    // ============================================================

    bool notificacionPresionada(
        const sf::RenderWindow& ventana,
        const sf::Event& evento,
        int& indice
    ) const;


    // ============================================================
    // OBTENER NOTIFICACIÓN SELECCIONADA
    // ============================================================

    int obtenerIdNotificacionSeleccionada() const;


    // ============================================================
    // DESPLAZAMIENTO
    // ============================================================

    void desplazar(
        float cantidad
    );


    void reiniciarDesplazamiento();


    // ============================================================
    // MENSAJE DE ESTADO
    // ============================================================

    void setMensaje(
        const std::string& mensaje
    );

    std::string obtenerMensaje() const;
};

#endif
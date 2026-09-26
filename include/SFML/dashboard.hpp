#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "materia.hpp"

class DashboardView
{
private:

    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    sf::Font font;

    //-------------------------------------------------
    // Datos del usuario
    //-------------------------------------------------

    std::string nombreAlumno;
    std::string boleta;
    std::string rol;

    //-------------------------------------------------
    // Materias
    //-------------------------------------------------

    std::vector<Materia> Materias;

    //-------------------------------------------------
    // Scroll de materias
    //-------------------------------------------------

    float desplazamientoMaterias;

    //-------------------------------------------------
    // Posición del scroll
    //-------------------------------------------------

    float scrollMateriasAnterior;

public:

    //-------------------------------------------------
    // Constructor
    //-------------------------------------------------

    DashboardView();

    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    bool cargarFuente(
        const std::string& ruta
    );

    //-------------------------------------------------
    // Datos del usuario
    //-------------------------------------------------

    void setAlumno(
        const std::string& nombre,
        const std::string& boletaAlumno
    );

    void setRol(
        const std::string& rolUsuario
    );

    //-------------------------------------------------
    // Materias
    //-------------------------------------------------

    void setMaterias(
        const std::vector<Materia>& lista
    );

    void limpiarMaterias();

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
};

#endif
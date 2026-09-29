#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "materia.hpp"

class DashboardView
{
private:
    sf::Font font;

    std::string nombreAlumno;
    std::string boleta;
    std::string rol;

    std::vector<Materia> Materias;

    float desplazamientoMaterias;
    float scrollMateriasAnterior;

public:
    DashboardView();

    bool cargarFuente(const std::string& ruta);

    void setAlumno(
        const std::string& nombre,
        const std::string& boletaAlumno
    );

    void setRol(
        const std::string& rolUsuario
    );

    void setMaterias(
        const std::vector<Materia>& lista
    );

    void limpiarMaterias();

    // Botones del dashboard
    bool botonMateriasPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool botonTareasPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool botonPlannerPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool botonNotificacionesPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    void draw(sf::RenderWindow& window);

    void manejarEvento(
        const sf::Event& event,
        const sf::RenderWindow& window
    );
};

#endif
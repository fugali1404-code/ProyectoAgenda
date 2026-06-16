#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

class DashboardView
{
private:

    sf::Font font;

    std::string nombreAlumno;
    std::string boleta;

public:

    DashboardView();

    bool cargarFuente(
        const std::string& ruta
    );

    void setAlumno(
        const std::string& nombre,
        const std::string& boletaAlumno
    );

    void draw(
        sf::RenderWindow& window
    );
};

#endif
#include <SFML/Graphics.hpp>

#include "SFML/dashboard.hpp"

int main()
{
    sf::RenderWindow window(
    sf::VideoMode({1280,720}),
    "Agenda Escolar"
    );

    DashboardView dashboard;

    dashboard.cargarFuente(
        "../assets/arial.ttf"
    );

    dashboard.setAlumno(
        "Juan Perez",
        "2023601234"
    );

    if(!dashboard.cargarFuente(
        "../assets/arial.ttf"
    ))
    {
        return 1;
    }

    while(window.isOpen())
    {
        while(const auto event =
              window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(
            sf::Color(240,240,240)
        );

        dashboard.draw(window);

        window.display();
    }

    return 0;
}
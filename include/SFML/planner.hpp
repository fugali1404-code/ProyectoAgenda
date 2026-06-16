#ifndef PLANNER_VIEW_HPP
#define PLANNER_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "tarea.hpp"

class PlannerView
{
private:

    sf::Font font;

public:

    PlannerView();

    bool cargarFuente(
        const std::string& ruta
    );

    void draw(
        sf::RenderWindow& window,
        const std::vector<Tarea>& tareas
    );
};

#endif
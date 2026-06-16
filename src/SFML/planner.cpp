#include "SFML/planner.hpp"

PlannerView::PlannerView()
{
}

bool PlannerView::cargarFuente(
    const std::string& ruta)
{
    return font.openFromFile(ruta);
}

void PlannerView::draw(
    sf::RenderWindow& window,
    const std::vector<Tarea>& tareas)
{
    float y = 80.f;

    for(const auto& tarea : tareas)
    {
        sf::Text texto(font);

        texto.setCharacterSize(24);

        texto.setString(
            tarea.getTitulo()
        );

        texto.setPosition(
            {50.f,y}
        );

        window.draw(texto);

        y += 40.f;
    }
}
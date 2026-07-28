#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include <string>

class TextBox
{
private:

    sf::RectangleShape caja;

    std::string contenido;

    bool seleccionado;

    bool password;

public:

    TextBox(
        bool esPassword = false
    );

    void setPosition(
        float x,
        float y
    );

    void setSize(
        float ancho,
        float alto
    );

    void setSelected(
        bool estado
    );

    bool isSelected() const;

    void handleEvent(
        const sf::Event& event,
        const sf::RenderWindow& window
    );

    void draw(
        sf::RenderWindow& window,
        const sf::Font& font,
        unsigned int tamaño = 22
    ) const;

    std::string getText() const;

    void setText(
        const std::string& texto
    );

    void clear();
};

#endif
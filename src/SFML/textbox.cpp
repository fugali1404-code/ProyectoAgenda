#include "SFML/textbox.hpp"

TextBox::TextBox(
    bool esPassword)
{
    password = esPassword;

    seleccionado = false;

    caja.setSize(
        {300.f,40.f}
    );

    caja.setFillColor(
        sf::Color::White
    );

    caja.setOutlineColor(
        sf::Color::Black
    );

    caja.setOutlineThickness(2.f);
}

void TextBox::setPosition(
    float x,
    float y)
{
    caja.setPosition(
        {x,y}
    );
}

void TextBox::setSize(
    float ancho,
    float alto)
{
    caja.setSize(
        {ancho,alto}
    );
}

void TextBox::setSelected(
    bool estado)
{
    seleccionado = estado;

    if(seleccionado)
    {
        caja.setOutlineColor(
            sf::Color::Blue
        );
    }
    else
    {
        caja.setOutlineColor(
            sf::Color::Black
        );
    }
}

bool TextBox::isSelected() const
{
    return seleccionado;
}

void TextBox::handleEvent(
    const sf::Event& event,
    const sf::RenderWindow& window)
{
    //---------------------------------
    // Mouse
    //---------------------------------

    if(const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f pos(
            static_cast<float>(
                mouse->position.x),
            static_cast<float>(
                mouse->position.y)
        );

        setSelected(
            caja.getGlobalBounds()
                .contains(pos)
        );
    }

    if(!seleccionado)
        return;

    //---------------------------------
    // Texto
    //---------------------------------

    if(const auto* txt =
        event.getIf<
            sf::Event::TextEntered>())
    {
        char c =
            static_cast<char>(
                txt->unicode);

        if(c >= 32 && c <= 126)
        {
            contenido += c;
        }
    }

    //---------------------------------
    // Backspace
    //---------------------------------

    if(const auto* key =
        event.getIf<
            sf::Event::KeyPressed>())
    {
        if(key->code ==
            sf::Keyboard::Key::Backspace)
        {
            if(!contenido.empty())
            {
                contenido.pop_back();
            }
        }
    }
}

void TextBox::draw(
    sf::RenderWindow& window,
    const sf::Font& font,
    unsigned int tamaño) const
{
    window.draw(caja);

    sf::Text texto(font);

    if(password)
    {
        texto.setString(
            std::string(
                contenido.size(),
                '*'
            )
        );
    }
    else
    {
        texto.setString(
            contenido
        );
    }

    texto.setCharacterSize(
        tamaño
    );

    texto.setFillColor(
        sf::Color::Black
    );

    texto.setPosition(
        caja.getPosition()
        +
        sf::Vector2f(
            5.f,
            5.f
        )
    );

    window.draw(texto);
}

std::string TextBox::getText() const
{
    return contenido;
}

void TextBox::setText(
    const std::string& texto)
{
    contenido = texto;
}

void TextBox::clear()
{
    contenido.clear();
}
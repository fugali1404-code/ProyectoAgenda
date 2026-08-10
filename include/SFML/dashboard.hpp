#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "materia.hpp"
#include "SFML/textbox.hpp"

class DashboardView
{
private:

    //---------------------------------
    // Fuente
    //---------------------------------

    sf::Font font;

    //---------------------------------
    // Datos del alumno
    //---------------------------------

    std::string nombreAlumno;
    std::string boleta;

    //---------------------------------
    // Materias
    //---------------------------------

    std::vector<Materia> Materias;

    //---------------------------------
    // Agregar materia
    //---------------------------------

    TextBox txtNuevaMateria;

    sf::RectangleShape btnAgregarMateria;

public:

    DashboardView();

    //---------------------------------
    // Fuente
    //---------------------------------

    bool cargarFuente(
        const std::string& ruta
    );

    //---------------------------------
    // Alumno
    //---------------------------------

    void setAlumno(
        const std::string& nombre,
        const std::string& boletaAlumno
    );

    //---------------------------------
    // Materias
    //---------------------------------

    void setMaterias(
        const std::vector<Materia>& lista
    );

    void limpiarMaterias();

    //---------------------------------
    // Dibujar
    //---------------------------------

    void draw(
        sf::RenderWindow& window
    );

    //---------------------------------
    // Eventos
    //---------------------------------

    void manejarEvento(
        const sf::Event& event,
        const sf::RenderWindow& window
    );

    //---------------------------------
    // Botón Agregar
    //---------------------------------

    bool botonAgregarPresionado(
        const sf::RenderWindow& window
    ) const;

    //---------------------------------
    // Nueva materia
    //---------------------------------

    std::string obtenerNuevaMateria() const;

    void limpiarNuevaMateria();
};

#endif
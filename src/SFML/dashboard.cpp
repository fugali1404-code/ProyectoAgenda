#include "SFML/dashboard.hpp"

DashboardView::DashboardView()
{
    nombreAlumno = " ";
    boleta = " ";

    txtNuevaMateria.setPosition(
        330.f,
        400.f
    );

    txtNuevaMateria.setSize(
        250.f,
        35.f
    );

    btnAgregarMateria.setSize(
        {120.f,35.f}
    );

    btnAgregarMateria.setPosition(
        {600.f,400.f}
    );

    btnAgregarMateria.setFillColor(
        sf::Color(60,150,80)
    );
}

bool DashboardView::cargarFuente(
    const std::string& ruta)
{
    return font.openFromFile(ruta);
}

void DashboardView::setAlumno(
    const std::string& nombre,
    const std::string& boletaAlumno)
{
    nombreAlumno = nombre;
    boleta = boletaAlumno;
}

void DashboardView::setMaterias(const std::vector<Materia>& lista)
{
    Materias = lista;
}

void DashboardView::limpiarMaterias()
{
    Materias.clear();
}

void DashboardView::draw(
    sf::RenderWindow& window)
{
    //-----------------------------------
    // COLORES
    //-----------------------------------

    sf::Color sidebarColor(41,53,65);

    sf::Color tarjetaColor(255,255,255);

    sf::Color textoBlanco(255,255,255);

    sf::Color textoNegro(40,40,40);

    sf::Color bordeTarjeta(220,220,220);

    //-----------------------------------
    // BARRA LATERAL
    //-----------------------------------

    sf::RectangleShape sidebar;

    sidebar.setSize({250.f,720.f});

    sidebar.setFillColor(
        sidebarColor
    );

    window.draw(sidebar);

    //-----------------------------------
    // TITULO
    //-----------------------------------

    sf::Text titulo(font);

    titulo.setString(
        "AGENDA\nESCOLAR"
    );

    titulo.setCharacterSize(34);

    titulo.setFillColor(
        textoBlanco
    );

    titulo.setPosition(
        {20.f,20.f}
    );

    window.draw(titulo);

    //-----------------------------------
    // DATOS ALUMNO
    //-----------------------------------

    sf::Text alumno(font);

    alumno.setString(
        "Alumno:\n" + nombreAlumno
    );

    alumno.setCharacterSize(18);

    alumno.setFillColor(
        textoBlanco
    );

    alumno.setPosition(
        {20.f,120.f}
    );

    window.draw(alumno);

    //-----------------------------------

    sf::Text txtBoleta(font);

    txtBoleta.setString(
        "Boleta:\n" + boleta
    );

    txtBoleta.setCharacterSize(18);

    txtBoleta.setFillColor(
        textoBlanco
    );

    txtBoleta.setPosition(
        {20.f,190.f}
    );

    window.draw(txtBoleta);

    //-----------------------------------
    // LINEA SEPARADORA
    //-----------------------------------

    sf::RectangleShape linea;

    linea.setSize(
        {200.f,2.f}
    );

    linea.setPosition(
        {20.f,270.f}
    );

    linea.setFillColor(
        sf::Color(90,105,120)
    );

    window.draw(linea);

    //-----------------------------------
    // MENU
    //-----------------------------------

    sf::Text planner(font);

    planner.setString(
        "Planner"
    );

    planner.setCharacterSize(24);

    planner.setFillColor(
        textoBlanco
    );

    planner.setPosition(
        {30.f,320.f}
    );

    window.draw(planner);

    //-----------------------------------

    sf::Text materias(font);

    materias.setString(
        "Materias"
    );

    materias.setCharacterSize(24);

    materias.setFillColor(
        textoBlanco
    );

    materias.setPosition(
        {30.f,380.f}
    );

    window.draw(materias);

    //-----------------------------------

    sf::Text tareas(font);

    tareas.setString(
        "Tareas"
    );

    tareas.setCharacterSize(24);

    tareas.setFillColor(
        textoBlanco
    );

    tareas.setPosition(
        {30.f,440.f}
    );

    window.draw(tareas);

    //-----------------------------------

    sf::Text notificaciones(font);

    notificaciones.setString(
        "Notificaciones"
    );

    notificaciones.setCharacterSize(24);

    notificaciones.setFillColor(
        textoBlanco
    );

    notificaciones.setPosition(
        {30.f,500.f}
    );

    window.draw(notificaciones);

    //-----------------------------------
    // TARJETA TAREAS
    //-----------------------------------

    sf::RectangleShape tarjeta1;

    tarjeta1.setSize(
        {950.f,220.f}
    );

    tarjeta1.setPosition(
        {290.f,50.f}
    );

    tarjeta1.setFillColor(
        tarjetaColor
    );

    tarjeta1.setOutlineThickness(2);

    tarjeta1.setOutlineColor(
        bordeTarjeta
    );

    window.draw(tarjeta1);

    //-----------------------------------

    sf::Text prox(font);

    prox.setString(
        "Proximas tareas"
    );

    prox.setCharacterSize(28);

    prox.setFillColor(
        textoNegro
    );

    prox.setPosition(
        {320.f,70.f}
    );

    window.draw(prox);

    //-----------------------------------

    sf::Text tarea1(font);

    tarea1.setString(
        "• Proyecto Final Programacion"
    );

    tarea1.setCharacterSize(20);

    tarea1.setFillColor(
        textoNegro
    );

    tarea1.setPosition(
        {330.f,130.f}
    );

    window.draw(tarea1);

    //-----------------------------------

    sf::Text tarea2(font);

    tarea2.setString(
        "• Reporte de Fisica"
    );

    tarea2.setCharacterSize(20);

    tarea2.setFillColor(
        textoNegro
    );

    tarea2.setPosition(
        {330.f,170.f}
    );

    window.draw(tarea2);

    //-----------------------------------
    // TARJETA MATERIAS
    //-----------------------------------

    sf::RectangleShape tarjeta2;

    tarjeta2.setSize(
        {950.f,320.f}
    );

    tarjeta2.setPosition(
        {290.f,320.f}
    );

    tarjeta2.setFillColor(
        tarjetaColor
    );

    tarjeta2.setOutlineThickness(2);

    tarjeta2.setOutlineColor(
        bordeTarjeta
    );

    window.draw(tarjeta2);

    //-----------------------------------

    sf::Text materiasActivas(font);

    materiasActivas.setString(
        "Materias Activas"
    );

    materiasActivas.setCharacterSize(28);

    materiasActivas.setFillColor(
        textoNegro
    );

    materiasActivas.setPosition(
        {320.f,340.f}
    );

    window.draw(materiasActivas);

    //-----------------------------------

    std::string textoMaterias;

    for(const auto& materia : Materias)
    {
        textoMaterias += "• ";
        textoMaterias += materia.getNombre();
        textoMaterias += "\n\n";
    }

    if(textoMaterias.empty())
    {
        textoMaterias = "No hay materias registradas.";
    }

    sf::Text listaMaterias(font);

    listaMaterias.setString(textoMaterias);

    listaMaterias.setCharacterSize(20);

    listaMaterias.setFillColor(textoNegro);

    listaMaterias.setPosition({330.f,400.f});

    window.draw(listaMaterias);

    //-----------------------------------
    // NUEVA MATERIA
    //-----------------------------------

    txtNuevaMateria.draw(
    window,
    font
    );

    window.draw(
        btnAgregarMateria
    );

    sf::Text agregar(font);

    agregar.setString(
        "Agregar"
    );

    agregar.setCharacterSize(
        18
    );

    agregar.setFillColor(
        sf::Color::White
    );

    agregar.setPosition(
        {620.f,405.f}
    );

    window.draw(agregar);
}

void DashboardView::manejarEvento(
    const sf::Event& event,
    const sf::RenderWindow& window
)
{
    txtNuevaMateria.handleEvent(
        event,
        window
    );
}

bool DashboardView::botonAgregarPresionado(
    const sf::RenderWindow& window
) const
{
    if(
        sf::Mouse::isButtonPressed(
            sf::Mouse::Button::Left
        )
    )
    {
        auto posicion =
            window.mapPixelToCoords(
                sf::Mouse::getPosition(window)
            );

        return
            btnAgregarMateria
            .getGlobalBounds()
            .contains(posicion);
    }

    return false;
}

std::string DashboardView::obtenerNuevaMateria() const
{
    return txtNuevaMateria.getText();
}

void DashboardView::limpiarNuevaMateria()
{
    txtNuevaMateria.clear();
}
#include "SFML/dashboard.hpp"

DashboardView::DashboardView()
{
    nombreAlumno = " ";

    boleta = " ";

    rol = " ";

    desplazamientoMaterias = 0.f;

    scrollMateriasAnterior = 0.f;
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

void DashboardView::setRol(
    const std::string& rolUsuario)
{
    rol = rolUsuario;
}

void DashboardView::setMaterias(
    const std::vector<Materia>& lista)
{
    Materias = lista;

    //-----------------------------------
    // Reiniciar scroll
    //-----------------------------------

    desplazamientoMaterias = 0.f;

    scrollMateriasAnterior = 0.f;
}

void DashboardView::limpiarMaterias()
{
    Materias.clear();

    //-----------------------------------
    // Reiniciar scroll
    //-----------------------------------

    desplazamientoMaterias = 0.f;

    scrollMateriasAnterior = 0.f;
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
    sf::Color botonColor(55,70,85);

    //-----------------------------------
    // BARRA LATERAL
    //-----------------------------------

    sf::RectangleShape sidebar;

    sidebar.setSize(
        {250.f,720.f}
    );

    sidebar.setFillColor(
        sidebarColor
    );

    window.draw(
        sidebar
    );

    //-----------------------------------
    // TITULO
    //-----------------------------------

    sf::Text titulo(font);

    titulo.setString(
        "AGENDA\nESCOLAR"
    );

    titulo.setCharacterSize(
        34
    );

    titulo.setFillColor(
        textoBlanco
    );

    titulo.setPosition(
        {20.f,20.f}
    );

    window.draw(
        titulo
    );

    //-----------------------------------
    // DATOS USUARIO
    //-----------------------------------

    sf::Text alumno(font);

    alumno.setString(
        "Usuario:\n" +
        nombreAlumno
    );

    alumno.setCharacterSize(
        18
    );

    alumno.setFillColor(
        textoBlanco
    );

    alumno.setPosition(
        {20.f,120.f}
    );

    window.draw(
        alumno
    );

    sf::Text txtBoleta(font);

    txtBoleta.setString(
        "Identificador:\n" +
        boleta
    );

    txtBoleta.setCharacterSize(
        18
    );

    txtBoleta.setFillColor(
        textoBlanco
    );

    txtBoleta.setPosition(
        {20.f,190.f}
    );

    window.draw(
        txtBoleta
    );

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

    window.draw(
        linea
    );

    //-----------------------------------
    // MENU
    //-----------------------------------

    float posicionY = 320.f;

    //-----------------------------------
    // PLANNER
    //-----------------------------------

    if(rol == "Alumno")
    {
        sf::RectangleShape botonPlanner;

        botonPlanner.setSize(
            {210.f,50.f}
        );

        botonPlanner.setPosition(
            {20.f,posicionY}
        );

        botonPlanner.setFillColor(
            botonColor
        );

        window.draw(
            botonPlanner
        );

        sf::Text planner(font);

        planner.setString(
            "Planner"
        );

        planner.setCharacterSize(
            24
        );

        planner.setFillColor(
            textoBlanco
        );

        planner.setPosition(
            {35.f,posicionY + 10.f}
        );

        window.draw(
            planner
        );

        posicionY += 60.f;
    }

    //-----------------------------------
    // MATERIAS
    //-----------------------------------

    sf::RectangleShape botonMaterias;

    botonMaterias.setSize(
        {210.f,50.f}
    );

    botonMaterias.setPosition(
        {20.f,posicionY}
    );

    botonMaterias.setFillColor(
        botonColor
    );

    window.draw(
        botonMaterias
    );

    sf::Text materias(font);

    materias.setString(
        "Materias"
    );

    materias.setCharacterSize(
        24
    );

    materias.setFillColor(
        textoBlanco
    );

    materias.setPosition(
        {35.f,posicionY + 10.f}
    );

    window.draw(
        materias
    );

    posicionY += 60.f;

    //-----------------------------------
    // TAREAS
    //-----------------------------------

    sf::RectangleShape botonTareas;

    botonTareas.setSize(
        {210.f,50.f}
    );

    botonTareas.setPosition(
        {20.f,posicionY}
    );

    botonTareas.setFillColor(
        botonColor
    );

    window.draw(
        botonTareas
    );

    sf::Text tareas(font);

    tareas.setString(
        "Tareas"
    );

    tareas.setCharacterSize(
        24
    );

    tareas.setFillColor(
        textoBlanco
    );

    tareas.setPosition(
        {35.f,posicionY + 10.f}
    );

    window.draw(
        tareas
    );

    posicionY += 60.f;

    //-----------------------------------
    // NOTIFICACIONES
    //-----------------------------------

    if(rol == "Alumno")
    {
        sf::RectangleShape botonNotificaciones;

        botonNotificaciones.setSize(
            {210.f,50.f}
        );

        botonNotificaciones.setPosition(
            {20.f,posicionY}
        );

        botonNotificaciones.setFillColor(
            botonColor
        );

        window.draw(
            botonNotificaciones
        );

        sf::Text notificaciones(font);

        notificaciones.setString(
            "Notificaciones"
        );

        notificaciones.setCharacterSize(
            24
        );

        notificaciones.setFillColor(
            textoBlanco
        );

        notificaciones.setPosition(
            {35.f,posicionY + 10.f}
        );

        window.draw(
            notificaciones
        );
    }

    //-----------------------------------
    // TARJETA BIENVENIDA
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

    tarjeta1.setOutlineThickness(
        2
    );

    tarjeta1.setOutlineColor(
        bordeTarjeta
    );

    window.draw(
        tarjeta1
    );

    //-----------------------------------
    // TITULO BIENVENIDA
    //-----------------------------------

    sf::Text bienvenida(font);

    bienvenida.setString(
        "Bienvenido a Agenda Academica"
    );

    bienvenida.setCharacterSize(
        28
    );

    bienvenida.setFillColor(
        textoNegro
    );

    bienvenida.setPosition(
        {320.f,80.f}
    );

    window.draw(
        bienvenida
    );

    //-----------------------------------
    // MENSAJE SEGUN EL ROL
    //-----------------------------------

    sf::Text mensaje(font);

    if(rol == "Alumno")
    {
        mensaje.setString(
            "Consulta tus materias, tareas,\n"
            "planner y notificaciones."
        );
    }
    else if(rol == "Profesor")
    {
        mensaje.setString(
            "Administra tus materias y tareas\n"
            "desde el menu."
        );
    }
    else
    {
        mensaje.setString(
            "Selecciona una opcion del menu."
        );
    }

    mensaje.setCharacterSize(
        20
    );

    mensaje.setFillColor(
        textoNegro
    );

    mensaje.setPosition(
        {330.f,135.f}
    );

    window.draw(
        mensaje
    );

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

    tarjeta2.setOutlineThickness(
        2
    );

    tarjeta2.setOutlineColor(
        bordeTarjeta
    );

    window.draw(
        tarjeta2
    );

    sf::Text materiasActivas(font);

    materiasActivas.setString(
        "Materias Activas"
    );

    materiasActivas.setCharacterSize(
        28
    );

    materiasActivas.setFillColor(
        textoNegro
    );

    materiasActivas.setPosition(
        {320.f,340.f}
    );

    window.draw(
        materiasActivas
    );

    //-----------------------------------
    // AREA VISIBLE DE MATERIAS
    //-----------------------------------

    const float inicioX = 330.f;

    const float inicioY = 400.f;

    const float limiteSuperior = 395.f;

    const float limiteInferior = 615.f;

    const float espacioVertical = 45.f;

    //-----------------------------------
    // CANTIDAD DE MATERIAS
    //-----------------------------------

    std::size_t cantidadMaterias =
        Materias.size();

    //-----------------------------------
    // CALCULAR MAXIMO SCROLL
    //-----------------------------------

    float contenidoAltura = 0.f;

    if(cantidadMaterias > 0)
    {
        contenidoAltura =
            cantidadMaterias *
            espacioVertical;
    }

    float areaVisible =
        limiteInferior -
        limiteSuperior;

    float maximoScroll =
        contenidoAltura -
        areaVisible;

    if(maximoScroll < 0.f)
    {
        maximoScroll = 0.f;
    }

    //-----------------------------------
    // ASEGURAR QUE EL SCROLL SEA VALIDO
    //-----------------------------------

    if(desplazamientoMaterias < 0.f)
    {
        desplazamientoMaterias = 0.f;
    }

    if(desplazamientoMaterias > maximoScroll)
    {
        desplazamientoMaterias =
            maximoScroll;
    }

    //-----------------------------------
    // NO HAY MATERIAS
    //-----------------------------------

    if(cantidadMaterias == 0)
    {
        sf::Text listaMaterias(font);

        listaMaterias.setString(
            "No hay materias registradas."
        );

        listaMaterias.setCharacterSize(
            20
        );

        listaMaterias.setFillColor(
            textoNegro
        );

        listaMaterias.setPosition(
            {330.f,400.f}
        );

        window.draw(
            listaMaterias
        );
    }

    //-----------------------------------
    // MATERIAS
    //-----------------------------------

    else
    {
        for(std::size_t i = 0;
            i < cantidadMaterias;
            ++i)
        {
            float y =
                inicioY +
                (i * espacioVertical) -
                desplazamientoMaterias;

            //-----------------------------------
            // Solo dibujar dentro del area
            //-----------------------------------

            if(y < limiteSuperior)
            {
                continue;
            }

            if(y > limiteInferior)
            {
                continue;
            }

            //-----------------------------------
            // Materia
            //-----------------------------------

            sf::Text materiaTexto(font);

            materiaTexto.setString(
                "- " +
                Materias[i].getNombre()
            );

            materiaTexto.setCharacterSize(
                20
            );

            materiaTexto.setFillColor(
                textoNegro
            );

            materiaTexto.setPosition(
                {inicioX,y}
            );

            window.draw(
                materiaTexto
            );
        }
    }

    //-----------------------------------
    // INDICADOR SUPERIOR
    //-----------------------------------

    if(desplazamientoMaterias > 0.f)
    {
        sf::Text flechaArriba(font);

        flechaArriba.setString(
            "^"
        );

        flechaArriba.setCharacterSize(
            18
        );

        flechaArriba.setFillColor(
            textoNegro
        );

        flechaArriba.setPosition(
            {1190.f,390.f}
        );

        window.draw(
            flechaArriba
        );
    }

    //-----------------------------------
    // INDICADOR INFERIOR
    //-----------------------------------

    if(desplazamientoMaterias < maximoScroll)
    {
        sf::Text flechaAbajo(font);

        flechaAbajo.setString(
            "v"
        );

        flechaAbajo.setCharacterSize(
            18
        );

        flechaAbajo.setFillColor(
            textoNegro
        );

        flechaAbajo.setPosition(
            {1190.f,600.f}
        );

        window.draw(
            flechaAbajo
        );
    }

    //-----------------------------------
    // BARRA DE SCROLL
    //-----------------------------------

    if(maximoScroll > 0.f)
    {
        //-----------------------------------
        // Fondo de la barra
        //-----------------------------------

        sf::RectangleShape fondoScroll;

        fondoScroll.setSize(
            {6.f,190.f}
        );

        fondoScroll.setPosition(
            {1200.f,410.f}
        );

        fondoScroll.setFillColor(
            sf::Color(225,225,225)
        );

        window.draw(
            fondoScroll
        );

        //-----------------------------------
        // Tamaño del indicador
        //-----------------------------------

        float alturaBarra =
            190.f *
            (areaVisible / contenidoAltura);

        if(alturaBarra < 30.f)
        {
            alturaBarra = 30.f;
        }

        //-----------------------------------
        // Posición del indicador
        //-----------------------------------

        float posicionBarra =
            410.f +
            (desplazamientoMaterias /
            maximoScroll) *
            (190.f - alturaBarra);

        sf::RectangleShape barraScroll;

        barraScroll.setSize(
            {6.f,alturaBarra}
        );

        barraScroll.setPosition(
            {1200.f,posicionBarra}
        );

        barraScroll.setFillColor(
            sf::Color(120,120,120)
        );

        window.draw(
            barraScroll
        );
    }
}

//-----------------------------------
// BOTON MATERIAS
//-----------------------------------

bool DashboardView::botonMateriasPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!event.is<sf::Event::MouseButtonPressed>())
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
        sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            mouse->position
        );

    float y = 320.f;

    if(rol == "Alumno")
    {
        y = 380.f;
    }

    return (
        posicion.x >= 20.f &&
        posicion.x <= 230.f &&
        posicion.y >= y &&
        posicion.y <= y + 50.f
    );
}

//-----------------------------------
// BOTON TAREAS
//-----------------------------------

bool DashboardView::botonTareasPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!event.is<sf::Event::MouseButtonPressed>())
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
        sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            mouse->position
        );

    float y = 380.f;

    if(rol == "Alumno")
    {
        y = 440.f;
    }

    return (
        posicion.x >= 20.f &&
        posicion.x <= 230.f &&
        posicion.y >= y &&
        posicion.y <= y + 50.f
    );
}

//-----------------------------------
// BOTON PLANNER
//-----------------------------------

bool DashboardView::botonPlannerPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Alumno")
    {
        return false;
    }

    if(!event.is<sf::Event::MouseButtonPressed>())
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
        sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            mouse->position
        );

    return (
        posicion.x >= 20.f &&
        posicion.x <= 230.f &&
        posicion.y >= 320.f &&
        posicion.y <= 370.f
    );
}

//-----------------------------------
// BOTON NOTIFICACIONES
//-----------------------------------

bool DashboardView::botonNotificacionesPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Alumno")
    {
        return false;
    }

    if(!event.is<sf::Event::MouseButtonPressed>())
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
        sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            mouse->position
        );

    return (
        posicion.x >= 20.f &&
        posicion.x <= 230.f &&
        posicion.y >= 500.f &&
        posicion.y <= 550.f
    );
}

void DashboardView::manejarEvento(
    const sf::Event& event,
    const sf::RenderWindow& window
)
{
    //-----------------------------------
    // RUEDA DEL MOUSE
    //-----------------------------------

    if(const auto* rueda =
        event.getIf<
        sf::Event::MouseWheelScrolled>())
    {
        //-----------------------------------
        // Posición del mouse
        //-----------------------------------

        sf::Vector2i posicionMouse =
            sf::Mouse::getPosition(
                window
            );

        //-----------------------------------
        // Verificar si el mouse esta
        // dentro de la tarjeta
        //-----------------------------------

        if(posicionMouse.x >= 290 &&
           posicionMouse.x <= 1240 &&
           posicionMouse.y >= 320 &&
           posicionMouse.y <= 640)
        {
            //-----------------------------------
            // Cantidad de materias
            //-----------------------------------

            std::size_t cantidadMaterias =
                Materias.size();

            //-----------------------------------
            // Espacio disponible
            //-----------------------------------

            float areaVisible =
                615.f - 395.f;

            //-----------------------------------
            // Altura del contenido
            //-----------------------------------

            float contenidoAltura =
                cantidadMaterias * 45.f;

            //-----------------------------------
            // Calcular maximo scroll
            //-----------------------------------

            float maximoScroll =
                contenidoAltura -
                areaVisible;

            if(maximoScroll < 0.f)
            {
                maximoScroll = 0.f;
            }

            //-----------------------------------
            // Desplazar
            //-----------------------------------

            desplazamientoMaterias -=
                rueda->delta * 30.f;

            //-----------------------------------
            // Limites
            //-----------------------------------

            if(desplazamientoMaterias < 0.f)
            {
                desplazamientoMaterias = 0.f;
            }

            if(desplazamientoMaterias > maximoScroll)
            {
                desplazamientoMaterias =
                    maximoScroll;
            }
        }
    }
}

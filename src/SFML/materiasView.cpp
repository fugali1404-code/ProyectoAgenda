#include "SFML/materiasView.hpp"
#include <algorithm>



//////////////////////////////////////////////////////////////
// COLORES DEL PROYECTO
//////////////////////////////////////////////////////////////

static const sf::Color encabezadoColor(41, 53, 65);
static const sf::Color tarjetaColor(255, 255, 255);
static const sf::Color textoBlanco(255, 255, 255);
static const sf::Color textoNegro(40, 40, 40);
static const sf::Color bordeTarjeta(220, 220, 220);
static const sf::Color lineaSeparadora(90, 105, 120);
static const sf::Color fondoScroll(225, 225, 225);
static const sf::Color barraScroll(120, 120, 120);


MateriasView::MateriasView()
{
    rol = "";
    desplazamientoMaterias = 0.f;
    desplazamientoAlumnos = 0.f;

    idMateriaSeleccionada = -1;
    idAlumnoSeleccionado = -1;
    parcialSeleccionado = -1;

    alumnosSeleccionados.clear();
    ponderaciones.clear();

    mostrandoAgregar = false;
    mostrandoEditar = false;
    mostrandoEliminar = false;
    mostrandoAlumnos = false;
    mostrandoPonderaciones = false;
    mostrandoInformacion = false;

    calificacionFinal = 0.0;
    tieneCalificacionFinal = false;

    txtNuevaMateria.setPosition(425.f,280.f);
    txtNuevaMateria.setSize(430.f,40.f);

    txtEditarMateria.setPosition(425.f,280.f);
    txtEditarMateria.setSize(430.f,40.f);

    txtBoletasAlumno.setPosition(425.f,280.f);
    txtBoletasAlumno.setSize(430.f,40.f);

    

    //-------------------------------------------------
    // TextBox de ponderaciones
    //-------------------------------------------------

    txtParcialPonderacion.setPosition(315.f,215.f);
    txtParcialPonderacion.setSize(120.f,35.f);

    txtTarea.setPosition(315.f,285.f);
    txtTarea.setSize(250.f,35.f);

    txtExamen.setPosition(600.f,285.f);
    txtExamen.setSize(250.f,35.f);

    txtPractica.setPosition(315.f,355.f);
    txtPractica.setSize(250.f,35.f);

    txtProyecto.setPosition(600.f,355.f);
    txtProyecto.setSize(250.f,35.f);

    txtTrabajo.setPosition(315.f,425.f);
    txtTrabajo.setSize(250.f,35.f);

    txtOtro.setPosition(600.f,425.f);
    txtOtro.setSize(250.f,35.f);
}

///////////////////////////////////////////////////////////
// Cargar fuente
///////////////////////////////////////////////////////////

bool MateriasView::cargarFuente(
    const std::string& ruta
)
{
    return font.openFromFile(ruta);
}

///////////////////////////////////////////////////////////
// Rol
///////////////////////////////////////////////////////////

void MateriasView::setRol(
    const std::string& rolUsuario
)
{
    rol = rolUsuario;
}

///////////////////////////////////////////////////////////
// Materias
///////////////////////////////////////////////////////////

void MateriasView::setMaterias(
    const std::vector<Materia>& lista
)
{
    Materias = lista;

    //-------------------------------------------------
    // Asegurar que exista una cantidad por materia
    //-------------------------------------------------

    if(cantidadAlumnosMaterias.size() != Materias.size())
    {
        cantidadAlumnosMaterias.resize(Materias.size(),0);
    }
}

void MateriasView::limpiarMaterias()
{
    Materias.clear();
    profesores.clear();
    cantidadAlumnosMaterias.clear();

    alumnosMateria.clear();

    desplazamientoMaterias = 0.f;
    desplazamientoAlumnos = 0.f;

    idMateriaSeleccionada = -1;
    alumnosSeleccionados.clear();

}

///////////////////////////////////////////////////////////
// Profesores
///////////////////////////////////////////////////////////

void MateriasView::setProfesores(
    const std::vector<std::string>& lista
)
{
    profesores = lista;
}

///////////////////////////////////////////////////////////
// Cantidad de alumnos
///////////////////////////////////////////////////////////

void MateriasView::setCantidadAlumnos(
    const std::vector<int>& cantidades
)
{
    cantidadAlumnosMaterias = cantidades;

    //-------------------------------------------------
    // Mantener sincronización con las materias
    //-------------------------------------------------

    if(cantidadAlumnosMaterias.size() < Materias.size())
    {
        cantidadAlumnosMaterias.resize(
            Materias.size(),
            0
        );
    }
}

///////////////////////////////////////////////////////////
// Alumnos
///////////////////////////////////////////////////////////

void MateriasView::setAlumnosMateria(
    const std::vector<AlumnoMateria>& alumnos)
{
    alumnosMateria = alumnos;

    desplazamientoAlumnos = 0.f;

    idAlumnoSeleccionado = -1;

    alumnosSeleccionados.clear();
}

void MateriasView::limpiarAlumnosMateria()
{
    alumnosMateria.clear();

    desplazamientoAlumnos = 0.f;

    idAlumnoSeleccionado = -1;

    alumnosSeleccionados.clear();
}


//////////////////////////////////////////////////
// Manejar eventos
//////////////////////////////////////////////////

void MateriasView::manejarEvento(
    const sf::Event& event,
    const sf::RenderWindow& window
)
{
    //-------------------------------------------------
    // AGREGAR
    //-------------------------------------------------

    if(mostrandoAgregar)
    {
        txtNuevaMateria.handleEvent(
            event,
            window
        );

        return;
    }


    //-------------------------------------------------
    // EDITAR
    //-------------------------------------------------

    if(mostrandoEditar)
    {
        txtEditarMateria.handleEvent(
            event,
            window
        );

        return;
    }


    //-------------------------------------------------
    // ALUMNOS
    //-------------------------------------------------

    if(mostrandoAlumnos)
    {
        txtBoletasAlumno.handleEvent(
            event,
            window
        );

        return;
    }


    //-------------------------------------------------
    // PONDERACIONES
    //-------------------------------------------------

    if(mostrandoPonderaciones)
    {
        txtParcialPonderacion.handleEvent(
            event,
            window
        );

        txtTarea.handleEvent(
            event,
            window
        );

        txtExamen.handleEvent(
            event,
            window
        );

        txtPractica.handleEvent(
            event,
            window
        );

        txtProyecto.handleEvent(
            event,
            window
        );

        txtTrabajo.handleEvent(
            event,
            window
        );

        txtOtro.handleEvent(
            event,
            window
        );

        return;
    }


    //-------------------------------------------------
    // INFORMACION
    //-------------------------------------------------

    if(mostrandoInformacion)
    {
        return;
    }
}

//////////////////////////////////////////////////
// Dibujar
/////////////////////////////////////////////////

void MateriasView::draw(
    sf::RenderWindow& window
)
{
    //-------------------------------------------------
    // Colores del proyecto
    //-------------------------------------------------

    sf::Color fondo(245,246,248);
    sf::Color encabezadoColor(41,53,65);
    sf::Color tarjetaColor(255,255,255);
    sf::Color textoBlanco(255,255,255);
    sf::Color textoNegro(40,40,40);
    sf::Color bordeTarjeta(220,220,220);
    sf::Color azulClaro(232,238,255);
    sf::Color verde(76,175,80);
    sf::Color rojo(220,70,70);
    sf::Color grisBoton(110,110,110);

    //-------------------------------------------------
    // Fondo
    //-------------------------------------------------

    window.clear(fondo);


    //-------------------------------------------------
    // Encabezado
    //-------------------------------------------------

    sf::RectangleShape encabezado(sf::Vector2f(1280.f,75.f));
    encabezado.setPosition(sf::Vector2f(0.f,0.f));
    encabezado.setFillColor(encabezadoColor);

    window.draw(encabezado);


    //-------------------------------------------------
    // Titulo
    //-------------------------------------------------

    sf::Text titulo(font,"Materias",30);
    titulo.setFillColor(textoBlanco);
    titulo.setPosition(sf::Vector2f(40.f,20.f));

    window.draw(titulo);


    //-------------------------------------------------
    // Boton regresar
    //-------------------------------------------------

    sf::RectangleShape botonRegresar;

    botonRegresar.setPosition({1030.f, 18.f});
    botonRegresar.setSize({220.f, 45.f});
    botonRegresar.setFillColor(encabezadoColor);
    botonRegresar.setOutlineColor(lineaSeparadora);
    botonRegresar.setOutlineThickness(1.f);

    window.draw(botonRegresar);


    sf::Text textoRegresar(
        font,
        "Regresar al Dashboard",
        16
    );

    textoRegresar.setFillColor(
        textoBlanco
    );

    sf::FloatRect limites =
        textoRegresar.getLocalBounds();

    float xRegresar =
        1030.f +
        (220.f - limites.size.x) / 2.f -
        limites.position.x;

    float yRegresar =
        18.f +
        (45.f - limites.size.y) / 2.f -
        limites.position.y;

    textoRegresar.setPosition(
        {xRegresar, yRegresar}
    );

    window.draw(textoRegresar);


    //-------------------------------------------------
    // Contenedor principal
    //-------------------------------------------------

    sf::RectangleShape contenedor(
        sf::Vector2f(
            1200.f,
            600.f
        )
    );

    contenedor.setPosition(
        sf::Vector2f(
            40.f,
            100.f
        )
    );

    contenedor.setFillColor(
        tarjetaColor
    );

    window.draw(contenedor);


    //-------------------------------------------------
    // Subtitulo
    //-------------------------------------------------

    sf::Text subtitulo(
        font,
        rol == "Profesor"
            ? "Mis materias"
            : "Mis materias inscritas",
        24
    );

    subtitulo.setFillColor(
        textoNegro
    );

    subtitulo.setPosition(
        sf::Vector2f(
            70.f,
            115.f
        )
    );

    window.draw(subtitulo);


    //-------------------------------------------------
    // Boton agregar
    //-------------------------------------------------

    if(rol == "Profesor")
    {
        sf::RectangleShape botonAgregar(sf::Vector2f(190.f, 42.f));
        botonAgregar.setPosition(sf::Vector2f(1010.f,115.f));
        botonAgregar.setFillColor(encabezadoColor);

        window.draw(botonAgregar);

        sf::Text textoAgregar(font,"Agregar materia", 17);
        textoAgregar.setFillColor(textoBlanco);
        textoAgregar.setPosition(sf::Vector2f(1030.f,126.f));

        window.draw(textoAgregar);
    }


    //-------------------------------------------------
    // Area de tarjetas
    //-------------------------------------------------

    const float inicioX = 70.f;
    const float inicioY = 175.f;
    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;
    const float separacionX = 35.f;
    const float separacionY = 25.f;

    //-------------------------------------------------
    // Dibujar materias
    //-------------------------------------------------

    for(std::size_t i = 0; i < Materias.size(); ++i)
    {
        int fila = static_cast<int>(i / 2);
        int columna = static_cast<int>(i % 2);
        float x = inicioX + columna * (anchoTarjeta + separacionX);
        float y = inicioY + fila * (altoTarjeta + separacionY) - desplazamientoMaterias;


        //-------------------------------------------------
        // Tarjeta
        //-------------------------------------------------

        sf::RectangleShape tarjeta(
            sf::Vector2f(
                anchoTarjeta,
                altoTarjeta
            )
        );

        tarjeta.setPosition(
            sf::Vector2f(
                x,
                y
            )
        );

        tarjeta.setFillColor(
            tarjetaColor
        );

        tarjeta.setOutlineThickness(
            1.f
        );

        tarjeta.setOutlineColor(
            bordeTarjeta
        );

        window.draw(tarjeta);


        //-------------------------------------------------
        // Nombre
        //-------------------------------------------------

        sf::Text nombre(
            font,
            Materias[i].getNombre(),
            22
        );

        nombre.setFillColor(
            textoNegro
        );

        nombre.setPosition(
            sf::Vector2f(
                x + 25.f,
                y + 20.f
            )
        );

        window.draw(nombre);


        //-------------------------------------------------
        // INFORMACION DEL ALUMNO
        //-------------------------------------------------

        if(rol == "Alumno")
        {
            //-------------------------------------------------
            // Profesor
            //-------------------------------------------------

            std::string profesor =
                "Profesor: ";


            if(i < profesores.size())
            {
                profesor +=
                    profesores[i];
            }
            else
            {
                profesor +=
                    "No disponible";
            }


            sf::Text textoProfesor(
                font,
                profesor,
                17
            );

            textoProfesor.setFillColor(
                textoNegro
            );

            textoProfesor.setPosition(
                sf::Vector2f(
                    x + 25.f,
                    y + 65.f
                )
            );

            window.draw(
                textoProfesor
            );


            //-------------------------------------------------
            // Boton Ver informacion
            //-------------------------------------------------

            sf::RectangleShape botonInformacion(
                sf::Vector2f(
                    150.f,
                    38.f
                )
            );

            botonInformacion.setPosition(
                sf::Vector2f(
                    x + 25.f,
                    y + 110.f
                )
            );

            botonInformacion.setFillColor(
                encabezadoColor
            );

            window.draw(
                botonInformacion
            );


            sf::Text textoInformacion(
                font,
                "Ver informacion",
                15
            );

            textoInformacion.setFillColor(
                textoBlanco
            );

            textoInformacion.setPosition(
                sf::Vector2f(
                    x + 45.f,
                    y + 120.f
                )
            );

            window.draw(
                textoInformacion
            );
        }


        //-------------------------------------------------
        // INFORMACION DEL PROFESOR
        //-------------------------------------------------

        else if(rol == "Profesor")
        {
            //-------------------------------------------------
            // Materia a cargo
            //-------------------------------------------------

            sf::Text textoCargo(
                font,
                "Materia a tu cargo",
                17
            );

            textoCargo.setFillColor(
                textoNegro
            );

            textoCargo.setPosition(
                sf::Vector2f(
                    x + 25.f,
                    y + 57.f
                )
            );

            window.draw(
                textoCargo
            );


            //-------------------------------------------------
            // Cantidad de alumnos
            //-------------------------------------------------

            int cantidad = 0;


            if(i < cantidadAlumnosMaterias.size())
            {
                cantidad =
                    cantidadAlumnosMaterias[i];
            }


            sf::Text textoAlumnos(
                font,
                std::to_string(cantidad) +
                    " alumnos",
                17
            );

            textoAlumnos.setFillColor(
                encabezadoColor
            );

            textoAlumnos.setPosition(
                sf::Vector2f(
                    x + 25.f,
                    y + 82.f
                )
            );

            window.draw(textoAlumnos);


            //-------------------------------------------------
            // Boton ponderaciones
            //-------------------------------------------------

            sf::RectangleShape botonPonderaciones(
                sf::Vector2f(
                    150.f,
                    38.f
                )
            );

            botonPonderaciones.setPosition(
                sf::Vector2f(
                    x + 350.f,
                    y + 75.f
                )
            );

            botonPonderaciones.setFillColor(
                encabezadoColor
            );

            window.draw(
                botonPonderaciones
            );


            sf::Text textoPonderaciones(
                font,
                "Ponderaciones",
                15
            );

            textoPonderaciones.setFillColor(
                textoBlanco
            );

            textoPonderaciones.setPosition(
                sf::Vector2f(
                    x + 370.f,
                    y + 85.f
                )
            );

            window.draw(
                textoPonderaciones
            );


            //-------------------------------------------------
            // Boton ver alumnos
            //-------------------------------------------------

            sf::RectangleShape botonAlumnos(
                sf::Vector2f(
                    125.f,
                    38.f
                )
            );

            botonAlumnos.setPosition(
                sf::Vector2f(
                    x + 25.f,
                    y + 118.f
                )
            );

            botonAlumnos.setFillColor(
                encabezadoColor
            );

            window.draw(
                botonAlumnos
            );


            sf::Text textoAlumnosBoton(
                font,
                "Ver alumnos",
                15
            );

            textoAlumnosBoton.setFillColor(
                textoBlanco
            );

            textoAlumnosBoton.setPosition(
                sf::Vector2f(
                    x + 40.f,
                    y + 128.f
                )
            );

            window.draw(
                textoAlumnosBoton
            );


            //-------------------------------------------------
            // Boton editar
            //-------------------------------------------------

            sf::RectangleShape botonEditar(
                sf::Vector2f(
                    110.f,
                    38.f
                )
            );

            botonEditar.setPosition(
                sf::Vector2f(
                    x + 165.f,
                    y + 118.f
                )
            );

            botonEditar.setFillColor(
                encabezadoColor
            );

            window.draw(
                botonEditar
            );


            sf::Text textoEditar(
                font,
                "Editar",
                15
            );

            textoEditar.setFillColor(
                textoBlanco
            );

            textoEditar.setPosition(
                sf::Vector2f(
                    x + 193.f,
                    y + 128.f
                )
            );

            window.draw(
                textoEditar
            );


            //-------------------------------------------------
            // Boton eliminar
            //-------------------------------------------------

            sf::RectangleShape botonEliminar(
                sf::Vector2f(
                    110.f,
                    38.f
                )
            );

            botonEliminar.setPosition(
                sf::Vector2f(
                    x + 290.f,
                    y + 118.f
                )
            );

            botonEliminar.setFillColor(
                rojo
            );

            window.draw(
                botonEliminar
            );


            sf::Text textoEliminar(
                font,
                "Eliminar",
                15
            );

            textoEliminar.setFillColor(
                textoBlanco
            );

            textoEliminar.setPosition(
                sf::Vector2f(
                    x + 310.f,
                    y + 128.f
                )
            );

            window.draw(
                textoEliminar
            );
        }
    }


    //-------------------------------------------------
    // Mensaje sin materias
    //-------------------------------------------------

    if(Materias.empty())
    {
        sf::Text mensaje(
            font,
            "No hay materias disponibles.",
            20
        );

        mensaje.setFillColor(
            textoNegro
        );

        mensaje.setPosition(
            sf::Vector2f(
                450.f,
                300.f
            )
        );

        window.draw(
            mensaje
        );
    }


    ///////////////////////////////////////////////////////
    // VENTANA AGREGAR
    ///////////////////////////////////////////////////////

    if(mostrandoAgregar)
    {
        sf::RectangleShape fondoModal(
            sf::Vector2f(
                500.f,
                280.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                390.f,
                210.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            encabezadoColor
        );

        window.draw(
            fondoModal
        );


        sf::Text tituloModal(
            font,
            "Agregar materia",
            24
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                425.f,
                235.f
            )
        );

        window.draw(
            tituloModal
        );


        txtNuevaMateria.draw(
            window,
            font
        );


        //-------------------------------------------------
        // Guardar
        //-------------------------------------------------

        sf::RectangleShape guardar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        guardar.setPosition(
            sf::Vector2f(
                425.f,
                370.f
            )
        );

        guardar.setFillColor(
            encabezadoColor
        );

        window.draw(
            guardar
        );


        sf::Text textoGuardar(
            font,
            "Guardar",
            16
        );

        textoGuardar.setFillColor(
            textoBlanco
        );

        textoGuardar.setPosition(
            sf::Vector2f(
                450.f,
                381.f
            )
        );

        window.draw(
            textoGuardar
        );


        //-------------------------------------------------
        // Cancelar
        //-------------------------------------------------

        sf::RectangleShape cancelar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        cancelar.setPosition(
            sf::Vector2f(
                600.f,
                370.f
            )
        );

        cancelar.setFillColor(
            grisBoton
        );

        window.draw(
            cancelar
        );


        sf::Text textoCancelar(
            font,
            "Cancelar",
            16
        );

        textoCancelar.setFillColor(
            textoBlanco
        );

        textoCancelar.setPosition(
            sf::Vector2f(
                625.f,
                381.f
            )
        );

        window.draw(
            textoCancelar
        );
    }


    ///////////////////////////////////////////////////////
    // VENTANA EDITAR
    ///////////////////////////////////////////////////////

    if(mostrandoEditar)
    {
        sf::RectangleShape fondoModal(
            sf::Vector2f(
                500.f,
                280.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                390.f,
                210.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            encabezadoColor
        );

        window.draw(
            fondoModal
        );


        sf::Text tituloModal(
            font,
            "Editar materia",
            24
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                425.f,
                235.f
            )
        );

        window.draw(
            tituloModal
        );


        txtEditarMateria.draw(
            window,
            font
        );


        //-------------------------------------------------
        // Guardar
        //-------------------------------------------------

        sf::RectangleShape guardar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        guardar.setPosition(
            sf::Vector2f(
                425.f,
                370.f
            )
        );

        guardar.setFillColor(
            encabezadoColor
        );

        window.draw(
            guardar
        );


        sf::Text textoGuardar(
            font,
            "Guardar",
            16
        );

        textoGuardar.setFillColor(
            textoBlanco
        );

        textoGuardar.setPosition(
            sf::Vector2f(
                450.f,
                381.f
            )
        );

        window.draw(
            textoGuardar
        );


        //-------------------------------------------------
        // Cancelar
        //-------------------------------------------------

        sf::RectangleShape cancelar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        cancelar.setPosition(
            sf::Vector2f(
                600.f,
                370.f
            )
        );

        cancelar.setFillColor(
            grisBoton
        );

        window.draw(
            cancelar
        );


        sf::Text textoCancelar(
            font,
            "Cancelar",
            16
        );

        textoCancelar.setFillColor(
            textoBlanco
        );

        textoCancelar.setPosition(
            sf::Vector2f(
                625.f,
                381.f
            )
        );

        window.draw(
            textoCancelar
        );
    }


    ///////////////////////////////////////////////////////
    // VENTANA ELIMINAR
    ///////////////////////////////////////////////////////

    if(mostrandoEliminar)
    {
        sf::RectangleShape fondoModal(
            sf::Vector2f(
                500.f,
                240.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                390.f,
                220.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            rojo
        );

        window.draw(
            fondoModal
        );


        sf::Text tituloModal(
            font,
            "Eliminar materia",
            24
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                425.f,
                245.f
            )
        );

        window.draw(
            tituloModal
        );


        sf::Text mensaje(
            font,
            "¿Deseas eliminar esta materia?",
            18
        );

        mensaje.setFillColor(
            textoNegro
        );

        mensaje.setPosition(
            sf::Vector2f(
                425.f,
                300.f
            )
        );

        window.draw(
            mensaje
        );


        //-------------------------------------------------
        // Cancelar
        //-------------------------------------------------

        sf::RectangleShape cancelar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        cancelar.setPosition(
            sf::Vector2f(
                425.f,
                365.f
            )
        );

        cancelar.setFillColor(
            grisBoton
        );

        window.draw(
            cancelar
        );


        sf::Text textoCancelar(
            font,
            "Cancelar",
            16
        );

        textoCancelar.setFillColor(
            textoBlanco
        );

        textoCancelar.setPosition(
            sf::Vector2f(
                450.f,
                376.f
            )
        );

        window.draw(
            textoCancelar
        );


        //-------------------------------------------------
        // Eliminar
        //-------------------------------------------------

        sf::RectangleShape eliminar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        eliminar.setPosition(
            sf::Vector2f(
                600.f,
                365.f
            )
        );

        eliminar.setFillColor(
            rojo
        );

        window.draw(
            eliminar
        );


        sf::Text textoEliminar(
            font,
            "Eliminar",
            16
        );

        textoEliminar.setFillColor(
            textoBlanco
        );

        textoEliminar.setPosition(
            sf::Vector2f(
                625.f,
                376.f
            )
        );

        window.draw(
            textoEliminar
        );
    }


    ///////////////////////////////////////////////////////
    // VENTANA ALUMNOS
    ///////////////////////////////////////////////////////

    if(mostrandoAlumnos)
    {
        //-------------------------------------------------
        // Fondo de ventana
        //-------------------------------------------------

        sf::RectangleShape fondoModal(
            sf::Vector2f(
                720.f,
                500.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                280.f,
                120.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            encabezadoColor
        );

        window.draw(
            fondoModal
        );


        //-------------------------------------------------
        // Buscar nombre de materia
        //-------------------------------------------------

        std::string nombreMateria =
            "Materia";


        for(const auto& materia : Materias)
        {
            if(materia.getId() ==
               idMateriaSeleccionada)
            {
                nombreMateria =
                    materia.getNombre();

                break;
            }
        }


        //-------------------------------------------------
        // Titulo
        //-------------------------------------------------

        sf::Text tituloModal(
            font,
            "Alumnos de " +
                nombreMateria,
            24
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                315.f,
                145.f
            )
        );

        window.draw(tituloModal);


        //-------------------------------------------------
        // Cantidad
        //-------------------------------------------------

        sf::Text cantidad(
            font,
            std::to_string(
                alumnosMateria.size()
            ) +
            " alumnos inscritos",
            16
        );

        cantidad.setFillColor(
            encabezadoColor
        );

        cantidad.setPosition(
            sf::Vector2f(
                315.f,
                185.f
            )
        );

        window.draw(cantidad);


        //-------------------------------------------------
        // Texto para boletas
        //-------------------------------------------------

        sf::Text textoBoletas(
            font,
            "Boletas para inscribir:",
            15
        );

        textoBoletas.setFillColor(
            textoNegro
        );

        textoBoletas.setPosition(
            sf::Vector2f(
                315.f,
                210.f
            )
        );

        window.draw(textoBoletas);


        //-------------------------------------------------
        // TextBox de boletas
        //-------------------------------------------------

        txtBoletasAlumno.setPosition(
            315.f,
            232.f
        );

        txtBoletasAlumno.setSize(
            420.f,
            38.f
        );

        txtBoletasAlumno.draw(
            window,
            font
        );


        //-------------------------------------------------
        // Ayuda
        //-------------------------------------------------

        sf::Text ayudaBoletas(
            font,
            "Puedes escribir una o varias boletas separadas por coma.",
            12
        );

        ayudaBoletas.setFillColor(
            sf::Color(
                100,
                100,
                100
            )
        );

        ayudaBoletas.setPosition(
            sf::Vector2f(
                315.f,
                274.f
            )
        );

        window.draw(ayudaBoletas);


        //-------------------------------------------------
        // Area de alumnos
        //-------------------------------------------------

        const float listaX = 315.f;

        const float listaY = 295.f;

        const float listaAncho = 650.f;

        const float listaAlto = 135.f;


        sf::RectangleShape areaLista(
            sf::Vector2f(
                listaAncho,
                listaAlto
            )
        );

        areaLista.setPosition(
            sf::Vector2f(
                listaX,
                listaY
            )
        );

        areaLista.setFillColor(
            tarjetaColor
        );

        areaLista.setOutlineThickness(
            1.f
        );

        areaLista.setOutlineColor(
            bordeTarjeta
        );

        window.draw(areaLista);


        //-------------------------------------------------
        // Dibujar alumnos
        //-------------------------------------------------

        if(alumnosMateria.empty())
        {
            sf::Text sinAlumnos(
                font,
                "No hay alumnos inscritos.",
                18
            );

            sinAlumnos.setFillColor(
                textoNegro
            );

            sinAlumnos.setPosition(
                sf::Vector2f(
                    500.f,
                    345.f
                )
            );

            window.draw(sinAlumnos);
        }
        else
        {
            for(std::size_t i = 0;
                i < alumnosMateria.size();
                ++i)
            {
                float y =
                    listaY +
                    8.f +
                    static_cast<float>(i) *
                    42.f -
                    desplazamientoAlumnos;


                //-------------------------------------------------
                // Evitar dibujar fuera del area
                //-------------------------------------------------

                if(y < listaY ||
                   y > listaY + listaAlto - 36.f)
                {
                    continue;
                }


                //-------------------------------------------------
                // Verificar si está seleccionado
                //-------------------------------------------------

                bool seleccionado = false;


                for(
                    int id :
                    alumnosSeleccionados
                )
                {
                    if(
                        id ==
                        alumnosMateria[i].id
                    )
                    {
                        seleccionado = true;
                        break;
                    }
                }


                //-------------------------------------------------
                // Fondo alumno
                //-------------------------------------------------

                sf::RectangleShape filaAlumno(
                    sf::Vector2f(
                        620.f,
                        36.f
                    )
                );

                filaAlumno.setPosition(
                    sf::Vector2f(
                        listaX + 15.f,
                        y
                    )
                );


                if(seleccionado)
                {
                    filaAlumno.setFillColor(
                        azulClaro
                    );
                }
                else
                {
                    filaAlumno.setFillColor(
                        tarjetaColor
                    );
                }


                filaAlumno.setOutlineThickness(
                    1.f
                );

                filaAlumno.setOutlineColor(
                    bordeTarjeta
                );

                window.draw(filaAlumno);


                //-------------------------------------------------
                // Checkbox
                //-------------------------------------------------

                sf::RectangleShape checkbox(
                    sf::Vector2f(
                        20.f,
                        20.f
                    )
                );

                checkbox.setPosition(
                    sf::Vector2f(
                        listaX + 25.f,
                        y + 8.f
                    )
                );

                checkbox.setFillColor(
                    tarjetaColor
                );

                checkbox.setOutlineThickness(
                    2.f
                );

                checkbox.setOutlineColor(
                    encabezadoColor
                );

                window.draw(checkbox);


                //-------------------------------------------------
                // Marca del checkbox
                //-------------------------------------------------

                if(seleccionado)
                {
                    sf::RectangleShape marca1(
                        sf::Vector2f(
                            8.f,
                            3.f
                        )
                    );

                    marca1.setPosition(
                        sf::Vector2f(
                            listaX + 29.f,
                            y + 19.f
                        )
                    );

                    marca1.setFillColor(
                        encabezadoColor
                    );

                    marca1.setRotation(
                        sf::degrees(45.f)
                    );

                    window.draw(marca1);


                    sf::RectangleShape marca2(
                        sf::Vector2f(
                            13.f,
                            3.f
                        )
                    );

                    marca2.setPosition(
                        sf::Vector2f(
                            listaX + 34.f,
                            y + 21.f
                        )
                    );

                    marca2.setFillColor(
                        encabezadoColor
                    );

                    marca2.setRotation(
                        sf::degrees(-45.f)
                    );

                    window.draw(marca2);
                }


                //-------------------------------------------------
                // Nombre
                //-------------------------------------------------

                sf::Text nombreAlumno(
                    font,
                    alumnosMateria[i].nombre,
                    15
                );

                nombreAlumno.setFillColor(
                    textoNegro
                );

                nombreAlumno.setPosition(
                    sf::Vector2f(
                        listaX + 60.f,
                        y + 3.f
                    )
                );

                window.draw(
                    nombreAlumno
                );


                //-------------------------------------------------
                // Identificador / boleta
                //-------------------------------------------------

                sf::Text identificador(
                    font,
                    alumnosMateria[i].identificador,
                    13
                );

                identificador.setFillColor(
                    sf::Color(
                        100,
                        100,
                        100
                    )
                );

                identificador.setPosition(
                    sf::Vector2f(
                        listaX + 60.f,
                        y + 20.f
                    )
                );

                window.draw(
                    identificador
                );
            }
        }


        //-------------------------------------------------
        // Texto de selección
        //-------------------------------------------------

        sf::Text textoSeleccion(
            font,
            "Selecciona los alumnos que deseas desinscribir.",
            12
        );

        textoSeleccion.setFillColor(
            sf::Color(
                100,
                100,
                100
            )
        );

        textoSeleccion.setPosition(
            sf::Vector2f(
                315.f,
                438.f
            )
        );

        window.draw(
            textoSeleccion
        );


        //-------------------------------------------------
        // Botón inscribir
        //-------------------------------------------------

        sf::RectangleShape botonInscribir(
            sf::Vector2f(
                150.f,
                40.f
            )
        );

        botonInscribir.setPosition(
            sf::Vector2f(
                315.f,
                460.f
            )
        );

        botonInscribir.setFillColor(
            encabezadoColor
        );

        window.draw(
            botonInscribir
        );


        sf::Text textoInscribir(
            font,
            "Inscribir",
            15
        );

        textoInscribir.setFillColor(
            textoBlanco
        );

        textoInscribir.setPosition(
            sf::Vector2f(
                360.f,
                471.f
            )
        );

        window.draw(
            textoInscribir
        );


        //-------------------------------------------------
        // Botón desinscribir
        //-------------------------------------------------

        sf::RectangleShape botonDesinscribir(
            sf::Vector2f(
                190.f,
                40.f
            )
        );

        botonDesinscribir.setPosition(
            sf::Vector2f(
                480.f,
                460.f
            )
        );

        botonDesinscribir.setFillColor(
            rojo
        );

        window.draw(
            botonDesinscribir
        );


        sf::Text textoDesinscribir(
            font,
            "Desinscribir",
            15
        );

        textoDesinscribir.setFillColor(
            textoBlanco
        );

        textoDesinscribir.setPosition(
            sf::Vector2f(
                525.f,
                471.f
            )
        );

        window.draw(
            textoDesinscribir
        );


        //-------------------------------------------------
        // Botón cerrar
        //-------------------------------------------------

        sf::RectangleShape botonCerrar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        botonCerrar.setPosition(
            sf::Vector2f(
                840.f,
                460.f
            )
        );

        botonCerrar.setFillColor(
            grisBoton
        );

        window.draw(
            botonCerrar
        );


        sf::Text textoCerrar(
            font,
            "Cerrar",
            16
        );

        textoCerrar.setFillColor(
            textoBlanco
        );

        textoCerrar.setPosition(
            sf::Vector2f(
                875.f,
                471.f
            )
        );

        window.draw(
            textoCerrar
        );
    }


    ///////////////////////////////////////////////////////
    // VENTANA PONDERACIONES
    ///////////////////////////////////////////////////////

    if(mostrandoPonderaciones)
    {
        sf::RectangleShape fondoModal(
            sf::Vector2f(
                720.f,
                520.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                280.f,
                100.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            encabezadoColor
        );

        window.draw(fondoModal);


        sf::Text tituloModal(
            font,
            "Configurar ponderaciones",
            24
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                315.f,
                125.f
            )
        );

        window.draw(tituloModal);


        //-------------------------------------------------
        // Parcial
        //-------------------------------------------------

        sf::Text textoParcial(
            font,
            "Parcial",
            16
        );

        textoParcial.setFillColor(
            textoNegro
        );

        textoParcial.setPosition(
            sf::Vector2f(
                315.f,
                190.f
            )
        );

        window.draw(textoParcial);

        txtParcialPonderacion.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Botón cargar
        //-------------------------------------------------

        sf::RectangleShape botonCargar(
            sf::Vector2f(
                100.f,
                35.f
            )
        );

        botonCargar.setPosition(
            sf::Vector2f(
                450.f,
                215.f
            )
        );

        botonCargar.setFillColor(
            encabezadoColor
        );

        window.draw(botonCargar);


        sf::Text textoCargar(
            font,
            "Cargar",
            15
        );

        textoCargar.setFillColor(
            textoBlanco
        );

        textoCargar.setPosition(
            sf::Vector2f(
                475.f,
                224.f
            )
        );

        window.draw(textoCargar);


        //-------------------------------------------------
        // Tarea
        //-------------------------------------------------

        sf::Text textoTarea(
            font,
            "Tarea (%)",
            16
        );

        textoTarea.setFillColor(
            textoNegro
        );

        textoTarea.setPosition(
            sf::Vector2f(
                315.f,
                260.f
            )
        );

        window.draw(textoTarea);

        txtTarea.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Examen
        //-------------------------------------------------

        sf::Text textoExamen(
            font,
            "Examen (%)",
            16
        );

        textoExamen.setFillColor(
            textoNegro
        );

        textoExamen.setPosition(
            sf::Vector2f(
                600.f,
                260.f
            )
        );

        window.draw(textoExamen);

        txtExamen.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Practica
        //-------------------------------------------------

        sf::Text textoPractica(
            font,
            "Practica (%)",
            16
        );

        textoPractica.setFillColor(
            textoNegro
        );

        textoPractica.setPosition(
            sf::Vector2f(
                315.f,
                330.f
            )
        );

        window.draw(textoPractica);

        txtPractica.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Proyecto
        //-------------------------------------------------

        sf::Text textoProyecto(
            font,
            "Proyecto (%)",
            16
        );

        textoProyecto.setFillColor(
            textoNegro
        );

        textoProyecto.setPosition(
            sf::Vector2f(
                600.f,
                330.f
            )
        );

        window.draw(textoProyecto);

        txtProyecto.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Trabajo
        //-------------------------------------------------

        sf::Text textoTrabajo(
            font,
            "Trabajo (%)",
            16
        );

        textoTrabajo.setFillColor(
            textoNegro
        );

        textoTrabajo.setPosition(
            sf::Vector2f(
                315.f,
                400.f
            )
        );

        window.draw(textoTrabajo);

        txtTrabajo.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Otro
        //-------------------------------------------------

        sf::Text textoOtro(
            font,
            "Otro (%)",
            16
        );

        textoOtro.setFillColor(
            textoNegro
        );

        textoOtro.setPosition(
            sf::Vector2f(
                600.f,
                400.f
            )
        );

        window.draw(textoOtro);

        txtOtro.draw(
            window,
            font,
            16
        );


        //-------------------------------------------------
        // Guardar
        //-------------------------------------------------

        sf::RectangleShape botonGuardar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        botonGuardar.setPosition(
            sf::Vector2f(
                315.f,
                475.f
            )
        );

        botonGuardar.setFillColor(
            encabezadoColor
        );

        window.draw(botonGuardar);


        sf::Text textoGuardar(
            font,
            "Guardar",
            15
        );

        textoGuardar.setFillColor(
            textoBlanco
        );

        textoGuardar.setPosition(
            sf::Vector2f(
                340.f,
                485.f
            )
        );

        window.draw(textoGuardar);


        //-------------------------------------------------
        // Eliminar
        //-------------------------------------------------

        sf::RectangleShape botonEliminar(
            sf::Vector2f(
                150.f,
                40.f
            )
        );

        botonEliminar.setPosition(
            sf::Vector2f(
                455.f,
                475.f
            )
        );

        botonEliminar.setFillColor(
            rojo
        );

        window.draw(botonEliminar);


        sf::Text textoEliminar(
            font,
            "Eliminar",
            15
        );

        textoEliminar.setFillColor(
            textoBlanco
        );

        textoEliminar.setPosition(
            sf::Vector2f(
                495.f,
                485.f
            )
        );

        window.draw(textoEliminar);


        //-------------------------------------------------
        // Cancelar
        //-------------------------------------------------

        sf::RectangleShape botonCancelar(
            sf::Vector2f(
                125.f,
                40.f
            )
        );

        botonCancelar.setPosition(
            sf::Vector2f(
                840.f,
                475.f
            )
        );

        botonCancelar.setFillColor(
            grisBoton
        );

        window.draw(botonCancelar);


        sf::Text textoCancelar(
            font,
            "Cancelar",
            15
        );

        textoCancelar.setFillColor(
            textoBlanco
        );

        textoCancelar.setPosition(
            sf::Vector2f(
                865.f,
                485.f
            )
        );

        window.draw(textoCancelar);
    }


    ///////////////////////////////////////////////////////
    // VENTANA INFORMACION DE MATERIA - ALUMNO
    ///////////////////////////////////////////////////////

    if(mostrandoInformacion)
    {
        //-------------------------------------------------
        // Fondo de ventana
        //-------------------------------------------------

        sf::RectangleShape fondoModal(
            sf::Vector2f(
                720.f,
                560.f
            )
        );

        fondoModal.setPosition(
            sf::Vector2f(
                280.f,
                80.f
            )
        );

        fondoModal.setFillColor(
            tarjetaColor
        );

        fondoModal.setOutlineThickness(
            2.f
        );

        fondoModal.setOutlineColor(
            encabezadoColor
        );

        window.draw(fondoModal);


        //-------------------------------------------------
        // Titulo
        //-------------------------------------------------

        sf::Text tituloModal(
            font,
            "Informacion de materia",
            25
        );

        tituloModal.setFillColor(
            textoNegro
        );

        tituloModal.setPosition(
            sf::Vector2f(
                315.f,
                105.f
            )
        );

        window.draw(tituloModal);


        //-------------------------------------------------
        // Buscar materia seleccionada
        //-------------------------------------------------

        int indiceMateria = -1;


        for(std::size_t i = 0;
            i < Materias.size();
            ++i)
        {
            if(
                Materias[i].getId() ==
                idMateriaSeleccionada
            )
            {
                indiceMateria =
                    static_cast<int>(i);

                break;
            }
        }


        //-------------------------------------------------
        // Informacion de materia
        //-------------------------------------------------

        if(indiceMateria != -1)
        {
            //-------------------------------------------------
            // Nombre de materia
            //-------------------------------------------------

            sf::Text materiaTexto(
                font,
                "Materia: " +
                    Materias[indiceMateria].getNombre(),
                18
            );

            materiaTexto.setFillColor(
                textoNegro
            );

            materiaTexto.setPosition(
                sf::Vector2f(
                    315.f,
                    150.f
                )
            );

            window.draw(materiaTexto);


            //-------------------------------------------------
            // Profesor
            //-------------------------------------------------

            std::string nombreProfesor =
                "No disponible";


            if(
                indiceMateria <
                static_cast<int>(
                    profesores.size()
                )
            )
            {
                nombreProfesor =
                    profesores[indiceMateria];
            }


            sf::Text profesorTexto(
                font,
                "Profesor: " +
                    nombreProfesor,
                18
            );

            profesorTexto.setFillColor(
                textoNegro
            );

            profesorTexto.setPosition(
                sf::Vector2f(
                    315.f,
                    180.f
                )
            );

            window.draw(profesorTexto);
        }


        //-------------------------------------------------
        // Titulo ponderaciones
        //-------------------------------------------------

        sf::Text tituloPonderaciones(
            font,
            "Ponderaciones",
            20
        );

        tituloPonderaciones.setFillColor(
            textoNegro
        );

        tituloPonderaciones.setPosition(
            sf::Vector2f(
                315.f,
                225.f
            )
        );

        window.draw(tituloPonderaciones);


        //-------------------------------------------------
        // Dibujar ponderaciones
        //-------------------------------------------------

        float yPonderacion =
            260.f;


        if(ponderaciones.empty())
        {
            sf::Text sinPonderaciones(
                font,
                "No hay ponderaciones configuradas.",
                16
            );

            sinPonderaciones.setFillColor(
                sf::Color(
                    100,
                    100,
                    100
                )
            );

            sinPonderaciones.setPosition(
                sf::Vector2f(
                    335.f,
                    yPonderacion
                )
            );

            window.draw(
                sinPonderaciones
            );
        }
        else
        {
            for(
                const auto& ponderacion :
                ponderaciones
            )
            {
                //-------------------------------------------------
                // Nombre del parcial
                //-------------------------------------------------

                std::string textoParcial;


                if(ponderacion.parcial == 0)
                {
                    textoParcial =
                        "Parcial 0 (Acumulativa)";
                }
                else
                {
                    textoParcial =
                        "Parcial " +
                        std::to_string(
                            ponderacion.parcial
                        );
                }


                sf::Text parcialTexto(
                    font,
                    textoParcial,
                    17
                );

                parcialTexto.setFillColor(
                    textoNegro
                );

                parcialTexto.setPosition(
                    sf::Vector2f(
                        315.f,
                        yPonderacion
                    )
                );

                window.draw(
                    parcialTexto
                );


                //-------------------------------------------------
                // Primera fila
                //-------------------------------------------------

                sf::Text valores(
                    font,
                    "Tarea: " +
                    std::to_string(
                        ponderacion.tarea
                    ) +
                    "%   Examen: " +
                    std::to_string(
                        ponderacion.examen
                    ) +
                    "%   Practica: " +
                    std::to_string(
                        ponderacion.practica
                    ) +
                    "%",
                    14
                );

                valores.setFillColor(
                    textoNegro
                );

                valores.setPosition(
                    sf::Vector2f(
                        335.f,
                        yPonderacion + 25.f
                    )
                );

                window.draw(
                    valores
                );


                //-------------------------------------------------
                // Segunda fila
                //-------------------------------------------------

                sf::Text valores2(
                    font,
                    "Proyecto: " +
                    std::to_string(
                        ponderacion.proyecto
                    ) +
                    "%   Trabajo: " +
                    std::to_string(
                        ponderacion.trabajo
                    ) +
                    "%   Otro: " +
                    std::to_string(
                        ponderacion.otro
                    ) +
                    "%",
                    14
                );

                valores2.setFillColor(
                    textoNegro
                );

                valores2.setPosition(
                    sf::Vector2f(
                        335.f,
                        yPonderacion + 47.f
                    )
                );

                window.draw(
                    valores2
                );


                yPonderacion += 80.f;
            }
        }


        //-------------------------------------------------
        // Calificacion final
        //-------------------------------------------------

        if(tieneCalificacionFinal)
        {
            sf::Text textoResultado(
                font,
                "Calificacion final: " +
                    std::to_string(
                        calificacionFinal
                    ),
                18
            );

            textoResultado.setFillColor(
                textoNegro
            );

            textoResultado.setPosition(
                sf::Vector2f(
                    315.f,
                    500.f
                )
            );

            window.draw(
                textoResultado
            );
        }


        //-------------------------------------------------
        // Boton ver calificacion final
        //-------------------------------------------------

        sf::RectangleShape botonCalificacion(
            sf::Vector2f(
                220.f,
                40.f
            )
        );

        botonCalificacion.setPosition(
            sf::Vector2f(
                430.f,
                600.f
            )
        );

        botonCalificacion.setFillColor(
            encabezadoColor
        );

        window.draw(
            botonCalificacion
        );


        sf::Text textoCalificacion(
            font,
            "Ver calificacion final",
            15
        );

        textoCalificacion.setFillColor(
            textoBlanco
        );

        textoCalificacion.setPosition(
            sf::Vector2f(
                450.f,
                610.f
            )
        );

        window.draw(
            textoCalificacion
        );


        //-------------------------------------------------
        // Boton cerrar
        //-------------------------------------------------

        sf::RectangleShape botonCerrar(
            sf::Vector2f(
                120.f,
                40.f
            )
        );

        botonCerrar.setPosition(
            sf::Vector2f(
                680.f,
                600.f
            )
        );

        botonCerrar.setFillColor(
            grisBoton
        );

        window.draw(
            botonCerrar
        );


        sf::Text textoCerrar(
            font,
            "Cerrar",
            15
        );

        textoCerrar.setFillColor(
            textoBlanco
        );

        textoCerrar.setPosition(
            sf::Vector2f(
                715.f,
                610.f
            )
        );

        window.draw(
            textoCerrar
        );
    }
}


///////////////////////////////////////////////////////////
// Boton regresar
///////////////////////////////////////////////////////////

bool MateriasView::botonRegresarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
) const
{
    if(mostrandoAgregar || mostrandoEditar || mostrandoEliminar ||
        mostrandoAlumnos || mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr || mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    return
        posicion.x >= 1080.f &&
        posicion.x <= 1225.f &&
        posicion.y >= 19.f &&
        posicion.y <= 61.f;
}

///////////////////////////////////////////////////////////
// Boton agregar
///////////////////////////////////////////////////////////

bool MateriasView::botonAgregarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Profesor")
    {
        return false;
    }

    if(mostrandoAgregar || mostrandoEditar || mostrandoEliminar ||
        mostrandoAlumnos || mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    if(
        posicion.x >= 1010.f &&
        posicion.x <= 1200.f &&
        posicion.y >= 115.f &&
        posicion.y <= 157.f
    )
    {
        mostrandoAgregar = true;

        txtNuevaMateria.clear();

        txtNuevaMateria.setSelected(true);

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Guardar agregar
///////////////////////////////////////////////////////////

bool MateriasView::botonGuardarAgregarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
) const
{
    if(!mostrandoAgregar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    return
        posicion.x >= 425.f &&
        posicion.x <= 550.f &&
        posicion.y >= 370.f &&
        posicion.y <= 410.f;
}

///////////////////////////////////////////////////////////
// Cancelar agregar
///////////////////////////////////////////////////////////

bool MateriasView::botonCancelarAgregarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoAgregar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    if(
        posicion.x >= 600.f &&
        posicion.x <= 725.f &&
        posicion.y >= 370.f &&
        posicion.y <= 410.f
    )
    {
        mostrandoAgregar = false;

        txtNuevaMateria.clear();

        return true;
    }

    return false;
}

//aqui

///////////////////////////////////////////////////////////
// Boton editar
///////////////////////////////////////////////////////////

bool MateriasView::botonEditarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Profesor")
    {
        return false;
    }

    if(mostrandoAgregar || mostrandoEditar ||mostrandoEliminar ||
        mostrandoAlumnos || mostrandoPonderaciones
    )
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    const float inicioX = 70.f;
    const float inicioY = 175.f;

    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;

    const float separacionX = 35.f;
    const float separacionY = 25.f;

    for(std::size_t i = 0;
        i < Materias.size();
        ++i)
    {
        int fila =
            static_cast<int>(i / 2);

        int columna =
            static_cast<int>(i % 2);

        float x =
            inicioX +
            columna *
            (anchoTarjeta + separacionX);

        float y =
            inicioY +
            fila *
            (altoTarjeta + separacionY) -
            desplazamientoMaterias;

        if(
            posicion.x >= x + 165.f &&
            posicion.x <= x + 275.f &&
            posicion.y >= y + 118.f &&
            posicion.y <= y + 156.f
        )
        {
            idMateriaSeleccionada =
                Materias[i].getId();

            mostrandoEditar = true;

            txtEditarMateria.clear();

            txtEditarMateria.setText(
                Materias[i].getNombre()
            );

            txtEditarMateria.setSelected(true);

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
// Guardar editar
///////////////////////////////////////////////////////////

bool MateriasView::botonGuardarEditarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
) const
{
    if(!mostrandoEditar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    return
        posicion.x >= 425.f &&
        posicion.x <= 550.f &&
        posicion.y >= 370.f &&
        posicion.y <= 410.f;
}

///////////////////////////////////////////////////////////
// Cancelar editar
///////////////////////////////////////////////////////////

bool MateriasView::botonCancelarEditarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoEditar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    if(
        posicion.x >= 600.f &&
        posicion.x <= 725.f &&
        posicion.y >= 370.f &&
        posicion.y <= 410.f
    )
    {
        mostrandoEditar = false;

        idMateriaSeleccionada = -1;

        txtEditarMateria.clear();

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton eliminar
///////////////////////////////////////////////////////////

bool MateriasView::botonEliminarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Profesor")
    {
        return false;
    }

    if(mostrandoAgregar || mostrandoEditar || mostrandoEliminar ||
        mostrandoAlumnos || mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    const float inicioX = 70.f;
    const float inicioY = 175.f;

    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;

    const float separacionX = 35.f;
    const float separacionY = 25.f;

    for(std::size_t i = 0;
        i < Materias.size();
        ++i)
    {
        int fila =
            static_cast<int>(i / 2);

        int columna =
            static_cast<int>(i % 2);

        float x =
            inicioX +
            columna *
            (anchoTarjeta + separacionX);

        float y =
            inicioY +
            fila *
            (altoTarjeta + separacionY) -
            desplazamientoMaterias;

        if(
            posicion.x >= x + 290.f &&
            posicion.x <= x + 400.f &&
            posicion.y >= y + 118.f &&
            posicion.y <= y + 156.f
        )
        {
            idMateriaSeleccionada =
                Materias[i].getId();

            mostrandoEliminar = true;

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
// Confirmar eliminar
///////////////////////////////////////////////////////////

bool MateriasView::botonConfirmarEliminarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoEliminar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    return
        posicion.x >= 600.f &&
        posicion.x <= 725.f &&
        posicion.y >= 365.f &&
        posicion.y <= 405.f;
}

///////////////////////////////////////////////////////////
// Cancelar eliminar
///////////////////////////////////////////////////////////

bool MateriasView::botonCancelarEliminarPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoEliminar)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr ||
       mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    if(
        posicion.x >= 425.f &&
        posicion.x <= 550.f &&
        posicion.y >= 365.f &&
        posicion.y <= 405.f
    )
    {
        mostrandoEliminar = false;

        idMateriaSeleccionada = -1;

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton ver alumnos
///////////////////////////////////////////////////////////

bool MateriasView::botonAlumnosPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Profesor")
    {
        return false;
    }

    if(mostrandoAgregar || mostrandoEditar || mostrandoEliminar ||
        mostrandoAlumnos || mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(
        mouse == nullptr ||
        mouse->button != sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    const float inicioX = 70.f;
    const float inicioY = 175.f;

    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;

    const float separacionX = 35.f;
    const float separacionY = 25.f;

    for(
        std::size_t i = 0;
        i < Materias.size();
        ++i
    )
    {
        int fila =
            static_cast<int>(i / 2);

        int columna =
            static_cast<int>(i % 2);

        float x =
            inicioX +
            columna *
            (anchoTarjeta + separacionX);

        float y =
            inicioY +
            fila *
            (altoTarjeta + separacionY) -
            desplazamientoMaterias;

        if(
            posicion.x >= x + 25.f &&
            posicion.x <= x + 150.f &&
            posicion.y >= y + 118.f &&
            posicion.y <= y + 156.f
        )
        {
            idMateriaSeleccionada =
                Materias[i].getId();

            idAlumnoSeleccionado = -1;

            alumnosSeleccionados.clear();

            alumnosMateria.clear();

            desplazamientoAlumnos = 0.f;

            limpiarBoletasAlumno();

            mostrandoAlumnos = true;

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton inscribir alumno
///////////////////////////////////////////////////////////

bool MateriasView::botonInscribirPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoAlumnos)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(
        mouse == nullptr ||
        mouse->button != sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    //-------------------------------------------------
    // Boton inscribir
    //-------------------------------------------------

    if(
        posicion.x >= 315.f &&
        posicion.x <= 465.f &&
        posicion.y >= 440.f &&
        posicion.y <= 480.f
    )
    {
        return true;
    }

    return false;
}


///////////////////////////////////////////////////////////
// Desinscribir alumno
///////////////////////////////////////////////////////////

bool MateriasView::botonDesinscribirPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoAlumnos)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(
        mouse == nullptr ||
        mouse->button != sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    //-------------------------------------------------
    // Boton desinscribir
    //-------------------------------------------------

    if(
        posicion.x >= 480.f &&
        posicion.x <= 650.f &&
        posicion.y >= 440.f &&
        posicion.y <= 480.f
    )
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Cerrar ventana alumnos
///////////////////////////////////////////////////////////

bool MateriasView::botonCerrarAlumnosPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoAlumnos)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(
        mouse == nullptr ||
        mouse->button != sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    if(
        posicion.x >= 840.f &&
        posicion.x <= 965.f &&
        posicion.y >= 440.f &&
        posicion.y <= 480.f
    )
    {
        mostrandoAlumnos = false;

        idMateriaSeleccionada = -1;
        idAlumnoSeleccionado = -1;

        alumnosSeleccionados.clear();

        alumnosMateria.clear();

        limpiarBoletasAlumno();

        desplazamientoAlumnos = 0.f;

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton informacion presionado
///////////////////////////////////////////////////////////

bool MateriasView::botonInformacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Alumno")
    {
        return false;
    }

    if(
        mostrandoAgregar ||
        mostrandoEditar ||
        mostrandoEliminar ||
        mostrandoAlumnos ||
        mostrandoPonderaciones ||
        mostrandoInformacion
    )
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(
        mouse == nullptr ||
        mouse->button != sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    const float inicioX = 70.f;
    const float inicioY = 175.f;

    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;

    const float separacionX = 35.f;
    const float separacionY = 25.f;

    for(std::size_t i = 0; i < Materias.size(); ++i)
    {
        int fila =
            static_cast<int>(i / 2);

        int columna =
            static_cast<int>(i % 2);

        float x =
            inicioX +
            columna * (anchoTarjeta + separacionX);

        float y =
            inicioY +
            fila * (altoTarjeta + separacionY) -
            desplazamientoMaterias;

        sf::FloatRect boton(
            sf::Vector2f(x + 25.f, y + 110.f),
            sf::Vector2f(150.f, 38.f)
        );

        if(boton.contains(posicion))
        {
            idMateriaSeleccionada =
                Materias[i].getId();

            mostrandoInformacion = true;

            limpiarCalificacionFinal();

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton consulta calificacion final
//////////////////////////////////////////////////////////

bool MateriasView::botonCalificacionFinalPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoInformacion)
        return false;

    if(!event.is<sf::Event::MouseButtonPressed>())
        return false;

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse->button != sf::Mouse::Button::Left)
        return false;

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(430.f, 600.f),
        sf::Vector2f(220.f, 40.f)
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////
//Boton cerrar infomacion
/////////////////////////////////////////////////////////

bool MateriasView::botonCerrarInformacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoInformacion)
        return false;

    if(!event.is<sf::Event::MouseButtonPressed>())
        return false;

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse->button != sf::Mouse::Button::Left)
        return false;

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(680.f, 600.f),
        sf::Vector2f(120.f, 40.f)
    );

    return boton.contains(posicion);
}


//////////////////////////////////////////////////////////
// Boton ponderaciones presionado
/////////////////////////////////////////////////////////

bool MateriasView::botonPonderacionesPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(rol != "Profesor")
    {
        return false;
    }

    if(
        mostrandoAgregar ||
        mostrandoEditar ||
        mostrandoEliminar ||
        mostrandoAlumnos ||
        mostrandoPonderaciones
    )
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    const float inicioX = 70.f;
    const float inicioY = 175.f;
    const float anchoTarjeta = 530.f;
    const float altoTarjeta = 170.f;
    const float separacionX = 35.f;
    const float separacionY = 25.f;

    for(std::size_t i = 0; i < Materias.size(); ++i)
    {
        int fila = static_cast<int>(i / 2);
        int columna = static_cast<int>(i % 2);

        float x =
            inicioX +
            columna * (anchoTarjeta + separacionX);

        float y =
            inicioY +
            fila * (altoTarjeta + separacionY) -
            desplazamientoMaterias;

        sf::FloatRect boton(
            sf::Vector2f(x + 350.f,y + 75.f),
            sf::Vector2f(150.f,38.f)
        );

        if(boton.contains(posicion))
        {
            idMateriaSeleccionada =
                Materias[i].getId();

            parcialSeleccionado = -1;

            txtParcialPonderacion.clear();
            txtTarea.clear();
            txtExamen.clear();
            txtPractica.clear();
            txtProyecto.clear();
            txtTrabajo.clear();
            txtOtro.clear();

            mostrandoPonderaciones = true;

            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////
// Boton cargar presionado
/////////////////////////////////////////////////////////

bool MateriasView::botonCargarPonderacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(450.f,215.f),
        sf::Vector2f(100.f,35.f)
    );

    if(!boton.contains(posicion))
    {
        return false;
    }

    int parcial;

    try
    {
        parcial =
            std::stoi(
                txtParcialPonderacion.getText()
            );
    }
    catch(...)
    {
        return false;
    }

    for(const auto& ponderacion : ponderaciones)
    {
        if(ponderacion.parcial == parcial)
        {
            parcialSeleccionado = parcial;

            txtTarea.setText(
                std::to_string(ponderacion.tarea)
            );

            txtExamen.setText(
                std::to_string(ponderacion.examen)
            );

            txtPractica.setText(
                std::to_string(ponderacion.practica)
            );

            txtProyecto.setText(
                std::to_string(ponderacion.proyecto)
            );

            txtTrabajo.setText(
                std::to_string(ponderacion.trabajo)
            );

            txtOtro.setText(
                std::to_string(ponderacion.otro)
            );

            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton guardar presionado
//////////////////////////////////////////////////////////

bool MateriasView::botonGuardarPonderacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(315.f,475.f),
        sf::Vector2f(125.f,40.f)
    );

    if(boton.contains(posicion))
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton eliminar presionado 
///////////////////////////////////////////////////////////

bool MateriasView::botonEliminarPonderacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(455.f,475.f),
        sf::Vector2f(150.f,40.f)
    );

    if(boton.contains(posicion))
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Boton cancelar preionado
///////////////////////////////////////////////////////////

bool MateriasView::botonCancelarPonderacionPresionado(
    const sf::RenderWindow& window,
    const sf::Event& event
)
{
    if(!mostrandoPonderaciones)
    {
        return false;
    }

    const auto* mouse =
        event.getIf<sf::Event::MouseButtonPressed>();

    if(mouse == nullptr)
    {
        return false;
    }

    if(mouse->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    sf::Vector2f posicion =
        window.mapPixelToCoords(mouse->position);

    sf::FloatRect boton(
        sf::Vector2f(840.f,475.f),
        sf::Vector2f(125.f,40.f)
    );

    if(boton.contains(posicion))
    {
        mostrandoPonderaciones = false;
        parcialSeleccionado = -1;

        txtParcialPonderacion.clear();
        txtTarea.clear();
        txtExamen.clear();
        txtPractica.clear();
        txtProyecto.clear();
        txtTrabajo.clear();
        txtOtro.clear();

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////
// Getter/Setter calificacion final
//////////////////////////////////////////////////////////

void MateriasView::setCalificacionFinal(double calificacion)
{
    calificacionFinal = calificacion;
    tieneCalificacionFinal = true;
}

void MateriasView::limpiarCalificacionFinal()
{
    calificacionFinal = 0.0;
    tieneCalificacionFinal = false;
}

bool MateriasView::estaMostrandoInformacion() const
{
    return mostrandoInformacion;
}

///////////////////////////////////////////////////////////
// Setter de ponderaciones 
///////////////////////////////////////////////////////////

void MateriasView::setPonderaciones(
    const std::vector<PonderacionMateria>& lista
)
{
    ponderaciones = lista;
}

///////////////////////////////////////////////////////////
// Limpiar ponderaciones 
///////////////////////////////////////////////////////////
void MateriasView::limpiarPonderaciones()
{
    ponderaciones.clear();

    parcialSeleccionado = -1;

    txtParcialPonderacion.clear();
    txtTarea.clear();
    txtExamen.clear();
    txtPractica.clear();
    txtProyecto.clear();
    txtTrabajo.clear();
    txtOtro.clear();
}

///////////////////////////////////////////////////////////
// Obtener ponderaciones
//////////////////////////////////////////////////////////

bool MateriasView::obtenerPonderacion(
    PonderacionMateria& ponderacion
) const
{
    try
    {
        ponderacion.parcial =
            std::stoi(txtParcialPonderacion.getText());

        ponderacion.tarea =
            std::stod(txtTarea.getText());

        ponderacion.examen =
            std::stod(txtExamen.getText());

        ponderacion.practica =
            std::stod(txtPractica.getText());

        ponderacion.proyecto =
            std::stod(txtProyecto.getText());

        ponderacion.trabajo =
            std::stod(txtTrabajo.getText());

        ponderacion.otro =
            std::stod(txtOtro.getText());
    }
    catch(...)
    {
        return false;
    }

    if(ponderacion.parcial < 0)
        return false;

    return true;
}

bool MateriasView::estaMostrandoPonderaciones() const
{
    return mostrandoPonderaciones;
}

///////////////////////////////////////////////////////////
// Obtener ID materia seleccionada
///////////////////////////////////////////////////////////

int MateriasView::obtenerIdMateriaSeleccionada() const
{
    return idMateriaSeleccionada;
}


///////////////////////////////////////////////////////////
// Obtener ID alumno seleccionado
///////////////////////////////////////////////////////////

int MateriasView::obtenerIdAlumnoSeleccionado() const
{
    if(alumnosSeleccionados.empty())
    {
        return -1;
    }

    return alumnosSeleccionados.front();
}

///////////////////////////////////////////////////////////
// Obtener alumnos seleccionados
///////////////////////////////////////////////////////////

const std::vector<int>&
MateriasView::obtenerAlumnosSeleccionados() const
{
    return alumnosSeleccionados;
}

///////////////////////////////////////////////////////////
// Alternar alumno seleccionado
///////////////////////////////////////////////////////////

void MateriasView::alternarAlumnoSeleccionado(
    int idAlumno
)
{
    auto it =
        std::find(
            alumnosSeleccionados.begin(),
            alumnosSeleccionados.end(),
            idAlumno
        );

    if(it == alumnosSeleccionados.end())
    {
        alumnosSeleccionados.push_back(idAlumno);
    }
    else
    {
        alumnosSeleccionados.erase(it);
    }
}



///////////////////////////////////////////////////////////
// Obtener boletas
///////////////////////////////////////////////////////////

std::string MateriasView::obtenerBoletasAlumno() const
{
    return txtBoletasAlumno.getText();
}

///////////////////////////////////////////////////////////
// Obtener boletas seleccionadas
///////////////////////////////////////////////////////////

std::string MateriasView::obtenerBoletasSeleccionadas() const
{
    std::string boletas;

    for(int idAlumno : alumnosSeleccionados)
    {
        for(const auto& alumno : alumnosMateria)
        {
            if(alumno.id == idAlumno)
            {
                if(!boletas.empty())
                {
                    boletas += ",";
                }

                boletas += alumno.identificador;

                break;
            }
        }
    }

    return boletas;
}

///////////////////////////////////////////////////////////
// Limpiar boletas
///////////////////////////////////////////////////////////

void MateriasView::limpiarBoletasAlumno()
{
    txtBoletasAlumno.clear();
}

///////////////////////////////////////////////////////////
// Limpiar alumnos seleccionados
///////////////////////////////////////////////////////////

void MateriasView::limpiarAlumnosSeleccionados()
{
    alumnosSeleccionados.clear();

    idAlumnoSeleccionado = -1;
}


///////////////////////////////////////////////////////////
// Nueva materia
///////////////////////////////////////////////////////////

std::string MateriasView::obtenerNuevaMateria() const
{
    return txtNuevaMateria.getText();
}

void MateriasView::limpiarNuevaMateria()
{
    txtNuevaMateria.clear();
}

///////////////////////////////////////////////////////////
// Editar materia
///////////////////////////////////////////////////////////

std::string MateriasView::obtenerNombreEditar() const
{
    return txtEditarMateria.getText();
}

void MateriasView::limpiarEditarMateria()
{
    txtEditarMateria.clear();
}

///////////////////////////////////////////////////////////
// Estado agregar
///////////////////////////////////////////////////////////

bool MateriasView::estaMostrandoAgregar() const
{
    return mostrandoAgregar;
}

///////////////////////////////////////////////////////////
// Estado editar
///////////////////////////////////////////////////////////

bool MateriasView::estaMostrandoEditar() const
{
    return mostrandoEditar;
}

///////////////////////////////////////////////////////////
// Estado eliminar
///////////////////////////////////////////////////////////

bool MateriasView::estaMostrandoEliminar() const
{
    return mostrandoEliminar;
}

///////////////////////////////////////////////////////////
// Estado alumnos
///////////////////////////////////////////////////////////

bool MateriasView::estaMostrandoAlumnos() const
{
    return mostrandoAlumnos;
}




///////////////////////////////////////////////////////////
// Cerrar formularios
///////////////////////////////////////////////////////////

void MateriasView::cerrarFormularios()
{
    mostrandoAgregar = false;
    mostrandoEditar = false;
    mostrandoEliminar = false;
    mostrandoAlumnos = false;
    mostrandoPonderaciones = false;
    mostrandoInformacion = false;

    idMateriaSeleccionada = -1;
    idAlumnoSeleccionado = -1;

    alumnosMateria.clear();

    desplazamientoAlumnos = 0.f;

   limpiarCalificacionFinal();

    txtNuevaMateria.clear();
    txtEditarMateria.clear();
}
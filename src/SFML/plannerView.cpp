#include "SFML/plannerView.hpp"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>

//-------------------------------------------------
// COLORES DEL PROYECTO
//-------------------------------------------------

static const sf::Color encabezadoColor(41, 53, 65);
static const sf::Color tarjetaColor(255, 255, 255);
static const sf::Color textoBlanco(255, 255, 255);
static const sf::Color textoNegro(40, 40, 40);
static const sf::Color bordeTarjeta(220, 220, 220);
static const sf::Color lineaSeparadora(90, 105, 120);
static const sf::Color botonRojo(220, 70, 70);
static const sf::Color botonGris(110, 110, 110);

static bool parsearFechaPlanner(const std::string& texto, int& anio, int& mes, int& dia)
{
    if (texto.size() < 10 || texto[4] != '-' || texto[7] != '-') return false;
    try
    {
        anio = std::stoi(texto.substr(0, 4));
        mes = std::stoi(texto.substr(5, 2));
        dia = std::stoi(texto.substr(8, 2));
    }
    catch (...)
    {
        return false;
    }
    return mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31;
}

static int diasEnMesPlanner(int anio, int mes)
{
    static const int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes < 1 || mes > 12) return 30;
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) return 29;
    return dias[mes - 1];
}

static std::string fechaPlanner(int anio, int mes, int dia)
{
    std::ostringstream salida;
    salida << std::setfill('0') << std::setw(4) << anio << "-"
           << std::setw(2) << mes << "-" << std::setw(2) << dia;
    return salida.str();
}

static int lunesIndexPlanner(int anio, int mes, int dia)
{
    std::tm fecha = {};
    fecha.tm_year = anio - 1900;
    fecha.tm_mon = mes - 1;
    fecha.tm_mday = dia;
    fecha.tm_hour = 12;
    std::mktime(&fecha);
    return (fecha.tm_wday + 6) % 7;
}


//-------------------------------------------------
// CONSTRUCTOR
//-------------------------------------------------

PlannerView::PlannerView()
    : tareaSeleccionada(-1),
      subtareaSeleccionada(-1),
      diaSeleccionado(-1),
      idSubtareaFormulario(-1),
      desplazamientoSubtareas(0),
      prioridadMostrada(PrioridadPlanner::MEDIA),
      estadoMostrado(EstadoSubtarea::PENDIENTE),
      formulario(Formulario::NINGUNO),
      campoActivo(false),
      vistaMensual(false),
      mesMostrado(0),
      anioMostrado(0),
      inicioCalendario(75.f),
      altoCalendario(360.f),
      yPanelDetalles(460.f)
{
}

//-------------------------------------------------
// FUENTE Y DATOS
//-------------------------------------------------

bool PlannerView::cargarFuente(const std::string& ruta)
{
    return font.openFromFile(ruta);
}

void PlannerView::setPlanner(const PlannerSemana& nuevaSemana)
{
    semana = nuevaSemana;
    int anio = 0;
    int mes = 0;
    int dia = 0;
    std::string fechaBase = semana.getDia(3).getFecha();
    if (fechaBase.empty()) fechaBase = semana.getFechaInicio();
    if (fechaBase.empty()) fechaBase = semana.getDia(0).getFecha();
    if (parsearFechaPlanner(fechaBase, anio, mes, dia))
    {
        if (mesMostrado == 0 || anioMostrado == 0)
        {
            mesMostrado = mes;
            anioMostrado = anio;
        }
        if (fechaSeleccionada.empty()) fechaSeleccionada = fechaBase;
    }
}

void PlannerView::setTareas(const std::vector<Tarea>& nuevasTareas)
{
    tareas = nuevasTareas;
}

void PlannerView::setSubtareas(const std::vector<Subtarea>& nuevasSubtareas)
{
    subtareas = nuevasSubtareas;
    const int total = static_cast<int>(obtenerSubtareasTarea(tareaSeleccionada).size());
    desplazamientoSubtareas = std::max(0, std::min(desplazamientoSubtareas, total - 1));
}

void PlannerView::setMaterias(const std::vector<Materia>& nuevasMaterias)
{
    materias = nuevasMaterias;
}

////////////////////////////////////////////////////
// Estados tareas
////////////////////////////////////////////////////

void PlannerView::setEstadosTareas(
    const vector<EstadoTareaPlanner>& nuevosEstados
)
{
    estadosTareas = nuevosEstados;
}

////////////////////////////////////////////////////
// Obtener estados tareas
///////////////////////////////////////////////////

EstadoTarea PlannerView::obtenerEstadoTarea(int idTarea) const
{
    for(const auto& estado : estadosTareas)
    {
        if(estado.idTarea == idTarea)
        {
            return estado.estado;
        }
    }

    return EstadoTarea::NO_COMPLETADO;
}


//-------------------------------------------------
// DIBUJADO
//-------------------------------------------------

void PlannerView::dibujarTexto(
    sf::RenderTarget& ventana,
    const std::string& contenido,
    unsigned int tamano,
    float x,
    float y,
    const sf::Color& color
)
{
    sf::Text texto(font);
    texto.setString(contenido);
    texto.setCharacterSize(tamano);
    texto.setFillColor(color);
    texto.setPosition({x, y});
    ventana.draw(texto);
}

void PlannerView::dibujarBoton(
    sf::RenderTarget& ventana,
    const sf::FloatRect& rectangulo,
    const std::string& contenido,
    const sf::Color& color
)
{
    sf::RectangleShape boton({rectangulo.size.x, rectangulo.size.y});
    boton.setPosition(rectangulo.position);
    boton.setFillColor(color);
    boton.setOutlineThickness(1.f);
    boton.setOutlineColor(bordeTarjeta);
    ventana.draw(boton);

    const unsigned int tamanoTexto = static_cast<unsigned int>(
        std::max(9.f, std::min(13.f, rectangulo.size.y * 0.34f))
    );
    dibujarTexto(
        ventana,
        contenido,
        tamanoTexto,
        rectangulo.position.x + 7.f,
        rectangulo.position.y + 6.f,
        textoBlanco
    );
}

////////////////////////////////////////////////////////////////
// Encabezado
///////////////////////////////////////////////////////////////

void PlannerView::dibujarEncabezado(sf::RenderWindow& ventana)
{
    const float ancho =
        static_cast<float>(ventana.getSize().x);

    //-------------------------------------------------
    // ENCABEZADO
    //-------------------------------------------------

    sf::RectangleShape encabezado(
        {
            ancho,
            75.f
        }
    );

    encabezado.setFillColor(encabezadoColor);
    ventana.draw(encabezado);

    //-------------------------------------------------
    // TITULO
    //-------------------------------------------------

    std::string titulo =
        vistaMensual
            ? "PLANNER | "
              + obtenerNombreMes(mesMostrado)
              + " "
              + std::to_string(anioMostrado)
            : "PLANNER | Semana actual";

    dibujarTexto(
        ventana,
        titulo,
        20,
        20.f,
        40.f,
        textoBlanco
    );

    //-------------------------------------------------
    // BOTONES
    //-------------------------------------------------

    dibujarBoton(
        ventana,
        botonVistaSemanal,
        "Semana",
        botonGris
    );

    dibujarBoton(
        ventana,
        botonVistaMensual,
        "Mes",
        botonGris
    );

    dibujarBoton(
        ventana,
        botonMesAnterior,
        "<",
        botonGris
    );

    dibujarBoton(
        ventana,
        botonMesSiguiente,
        ">",
        botonGris
    );

    dibujarBoton(
        ventana,
        botonRegresar,
        " Regresar",
        encabezadoColor
    );
}

/////////////////////////////////////////////////////////////
// Calendario
////////////////////////////////////////////////////////////

void PlannerView::dibujarCalendario(sf::RenderWindow& ventana)
{
    tarjetasTarea.clear();
    idsTarjetasTarea.clear();
    tarjetasSubtareaDia.clear();
    idsSubtareasDia.clear();
    diasSubtareas.clear();
    celdasMes.clear();
    fechasCeldasMes.clear();
    diasCeldasSemana.clear();

    if (vistaMensual) dibujarVistaMensual(ventana);
    else dibujarVistaSemanal(ventana);
}

//////////////////////////////////////////////////////////////////
// Calendario semanal
/////////////////////////////////////////////////////////////////

void PlannerView::dibujarVistaSemanal(sf::RenderWindow& ventana)
{
    const float ancho = 1280.f;

    const float espacio = 6.f;
    const float margen = 20.f;

    const float anchoColumna =
        (ancho - margen * 2.f - espacio * 6.f) / 7.f;

    const float altoColumna = altoCalendario;

    for(int indiceDia = 0; indiceDia < 7; ++indiceDia)
    {
        const float x =
            margen +
            indiceDia * (anchoColumna + espacio);

        columnas[indiceDia] =
        {
            {x, inicioCalendario},
            {anchoColumna, altoColumna}
        };

        sf::RectangleShape columna(
            {
                anchoColumna,
                altoColumna
            }
        );

        columna.setPosition(
            {
                x,
                inicioCalendario
            }
        );

        columna.setFillColor(tarjetaColor);
        columna.setOutlineThickness(1.f);
        columna.setOutlineColor(bordeTarjeta);

        ventana.draw(columna);

        //-------------------------------------------------
        // ENCABEZADO DEL DIA
        //-------------------------------------------------

        sf::RectangleShape encabezadoDia(
            {
                anchoColumna,
                37.f
            }
        );

        encabezadoDia.setPosition(
            {
                x,
                inicioCalendario
            }
        );

        encabezadoDia.setFillColor(encabezadoColor);

        ventana.draw(encabezadoDia);

        dibujarTexto(
            ventana,
            obtenerNombreDia(indiceDia)
                + " "
                + obtenerFechaCorta(
                    semana.getDia(indiceDia).getFecha()
                ),
            13,
            x + 7.f,
            inicioCalendario + 10.f,
            textoBlanco
        );

        //-------------------------------------------------
        // TAREAS
        //-------------------------------------------------

        float y = inicioCalendario + 43.f;

        const PlannerDia& dia =
            semana.getDia(indiceDia);

        for(const TareaPlanner& tareaPlanner :
            dia.getTareas())
        {
            const Tarea* tarea =
                buscarTarea(tareaPlanner.idTarea);

            if(
                tarea == nullptr ||
                y + 39.f >
                    inicioCalendario + altoColumna
            )
            {
                continue;
            }

            const sf::FloatRect tarjeta =
            {
                {x + 4.f, y},
                {anchoColumna - 8.f, 37.f}
            };

            sf::RectangleShape forma(
                {
                    tarjeta.size.x,
                    tarjeta.size.y
                }
            );

            forma.setPosition(tarjeta.position);
            forma.setFillColor(tarjetaColor);
            forma.setOutlineThickness(1.f);
            forma.setOutlineColor(bordeTarjeta);

            ventana.draw(forma);

            std::string titulo =
                tarea->getTitulo();

            if(titulo.size() > 16)
            {
                titulo =
                    titulo.substr(0, 15) + "...";
            }

            dibujarTexto(
                ventana,
                titulo,
                11,
                x + 8.f,
                y + 3.f
            );

            dibujarTexto(
                ventana,
                obtenerPrioridadTexto(
                    tareaPlanner.prioridad
                ),
                10,
                x + 8.f,
                y + 20.f,
                lineaSeparadora
            );

            tarjetasTarea.push_back(tarjeta);
            idsTarjetasTarea.push_back(
                tareaPlanner.idTarea
            );

            y += 41.f;
        }

        //-------------------------------------------------
        // SUBTAREAS
        //-------------------------------------------------

        for(int idSubtarea :
            dia.getSubtareas())
        {
            const Subtarea* subtarea =
                buscarSubtarea(idSubtarea);

            if(
                subtarea == nullptr ||
                y + 29.f >
                    inicioCalendario + altoColumna
            )
            {
                continue;
            }

            const sf::FloatRect tarjeta =
            {
                {x + 4.f, y},
                {anchoColumna - 8.f, 27.f}
            };

            sf::RectangleShape forma(
                {
                    tarjeta.size.x,
                    tarjeta.size.y
                }
            );

            forma.setPosition(tarjeta.position);
            forma.setFillColor(
                sf::Color(245, 246, 248)
            );
            forma.setOutlineThickness(1.f);
            forma.setOutlineColor(bordeTarjeta);

            ventana.draw(forma);

            std::string descripcion =
                subtarea->getDescripcion();

            if(descripcion.size() > 16)
            {
                descripcion =
                    descripcion.substr(0, 15) + "...";
            }

            dibujarTexto(
                ventana,
                descripcion,
                10,
                x + 8.f,
                y + 5.f,
                textoNegro
            );

            tarjetasSubtareaDia.push_back(tarjeta);
            idsSubtareasDia.push_back(idSubtarea);
            diasSubtareas.push_back(indiceDia);

            y += 30.f;
        }
    }
}

////////////////////////////////////////////////////////////////
// Calendario mensual
////////////////////////////////////////////////////////////////

void PlannerView::dibujarVistaMensual(sf::RenderWindow& ventana)
{
    const float margen = 20.f;
    const float yDias = inicioCalendario;
    const float altoDias = 27.f;

    const float ancho = 1280.f;

    const float espacio = 5.f;

    const float anchoCelda =
        (ancho - margen * 2.f - espacio * 6.f) / 7.f;

    const float altoCelda =
        std::max(
            28.f,
            (altoCalendario - altoDias - 5.f) / 6.f
        );

    static const char* nombres[] =
    {
        "LUN",
        "MAR",
        "MIE",
        "JUE",
        "VIE",
        "SAB",
        "DOM"
    };

    //-------------------------------------------------
    // DIAS DE LA SEMANA
    //-------------------------------------------------

    for(int dia = 0; dia < 7; ++dia)
    {
        const float x =
            margen +
            dia * (anchoCelda + espacio);

        sf::RectangleShape cabecera(
            {
                anchoCelda,
                altoDias
            }
        );

        cabecera.setPosition(
            {
                x,
                yDias
            }
        );

        cabecera.setFillColor(encabezadoColor);

        ventana.draw(cabecera);

        dibujarTexto(
            ventana,
            nombres[dia],
            12,
            x + 8.f,
            yDias + 5.f,
            textoBlanco
        );
    }

    //-------------------------------------------------
    // DIAS DEL MES
    //-------------------------------------------------

    const int offset =
        lunesIndexPlanner(
            anioMostrado,
            mesMostrado,
            1
        );

    const int diasMes =
        diasEnMesPlanner(
            anioMostrado,
            mesMostrado
        );

    for(int indice = 0; indice < 42; ++indice)
    {
        const int diaMes =
            indice - offset + 1;

        int anioCelda = anioMostrado;
        int mesCelda = mesMostrado;
        int diaCelda = diaMes;

        if(diaMes < 1)
        {
            if(--mesCelda == 0)
            {
                mesCelda = 12;
                --anioCelda;
            }

            diaCelda =
                diasEnMesPlanner(
                    anioCelda,
                    mesCelda
                ) + diaMes;
        }
        else if(diaMes > diasMes)
        {
            if(++mesCelda == 13)
            {
                mesCelda = 1;
                ++anioCelda;
            }

            diaCelda =
                diaMes - diasMes;
        }

        const std::string fecha =
            fechaPlanner(
                anioCelda,
                mesCelda,
                diaCelda
            );

        const int fila = indice / 7;
        const int columna = indice % 7;

        const float x =
            margen +
            columna * (anchoCelda + espacio);

        const float y =
            yDias +
            altoDias +
            5.f +
            fila * altoCelda;

        const sf::FloatRect celda =
        {
            {x, y},
            {anchoCelda, altoCelda - 2.f}
        };

        celdasMes.push_back(celda);
        fechasCeldasMes.push_back(fecha);
        diasCeldasSemana.push_back(
            buscarDiaSemana(fecha)
        );

        sf::RectangleShape fondo(
            {
                celda.size.x,
                celda.size.y
            }
        );

        fondo.setPosition(celda.position);

        fondo.setFillColor(
            fecha == fechaSeleccionada
                ? sf::Color(235, 239, 243)
                : tarjetaColor
        );

        fondo.setOutlineThickness(1.f);
        fondo.setOutlineColor(bordeTarjeta);

        ventana.draw(fondo);

        const bool mesActual =
            mesCelda == mesMostrado &&
            anioCelda == anioMostrado;

        dibujarTexto(
            ventana,
            std::to_string(diaCelda),
            11,
            x + 5.f,
            y + 3.f,
            mesActual
                ? textoNegro
                : lineaSeparadora
        );

        //-------------------------------------------------
        // TAREAS DEL DIA
        //-------------------------------------------------

        std::vector<const Tarea*> tareasDia;

        for(const Tarea& tarea : tareas)
        {
            if(tarea.getFechaEntrega() == fecha)
            {
                tareasDia.push_back(&tarea);
            }
        }

        float yTarea = y + 19.f;

        const int mostrar =
            altoCelda >= 55.f ? 2 : 1;

        for(
            int i = 0;
            i < static_cast<int>(tareasDia.size()) &&
            i < mostrar;
            ++i
        )
        {
            const Tarea* tarea =
                tareasDia[i];

            const sf::FloatRect tarjeta =
            {
                {x + 3.f, yTarea},
                {anchoCelda - 6.f, 16.f}
            };

            sf::RectangleShape chip(
                {
                    tarjeta.size.x,
                    tarjeta.size.y
                }
            );

            chip.setPosition(tarjeta.position);
            chip.setFillColor(
                sf::Color(245, 246, 248)
            );

            ventana.draw(chip);

            std::string titulo =
                tarea->getTitulo();

            if(
                titulo.size() >
                static_cast<std::size_t>(
                    std::max(
                        5.f,
                        anchoCelda / 8.f
                    )
                )
            )
            {
                titulo =
                    titulo.substr(
                        0,
                        static_cast<std::size_t>(
                            std::max(
                                4.f,
                                anchoCelda / 8.f - 1.f
                            )
                        )
                    ) + ".";
            }

            dibujarTexto(
                ventana,
                titulo,
                9,
                x + 5.f,
                yTarea + 1.f
            );

            tarjetasTarea.push_back(tarjeta);
            idsTarjetasTarea.push_back(
                tarea->getId()
            );

            yTarea += 17.f;
        }

        if(
            static_cast<int>(tareasDia.size()) >
            mostrar
        )
        {
            dibujarTexto(
                ventana,
                "+" +
                    std::to_string(
                        tareasDia.size() - mostrar
                    ) +
                    " mas",
                9,
                x + 5.f,
                yTarea,
                lineaSeparadora
            );
        }

        //-------------------------------------------------
        // SUBTAREAS
        //-------------------------------------------------

        const int indiceSemana =
            buscarDiaSemana(fecha);

        if(indiceSemana >= 0)
        {
            for(int idSubtarea :
                semana
                    .getDia(indiceSemana)
                    .getSubtareas())
            {
                const Subtarea* subtarea =
                    buscarSubtarea(idSubtarea);

                if(
                    subtarea &&
                    yTarea + 15.f <
                        y + celda.size.y
                )
                {
                    std::string etiqueta =
                        "- " +
                        subtarea->getDescripcion();

                    if(etiqueta.size() > 15)
                    {
                        etiqueta =
                            etiqueta.substr(0, 14)
                            + "...";
                    }

                    dibujarTexto(
                        ventana,
                        etiqueta,
                        9,
                        x + 5.f,
                        yTarea,
                        lineaSeparadora
                    );

                    yTarea += 14.f;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////
// Panel detalles
////////////////////////////////////////////////////////////////

void PlannerView::dibujarPanelDetalles(sf::RenderWindow& ventana)
{
    filasSubtareaDetalle.clear();
    idsSubtareasDetalle.clear();

    // =====================================================
    // TAMAÑO LOGICO DE LA INTERFAZ
    // =====================================================

    const float ancho = 1280.f;
    const float alto = 720.f;

    (void)alto;

    const float yPanel =
        yPanelDetalles;

    const float altoPanel =
        alto - yPanel - 12.f;

    // =====================================================
    // PANEL
    // =====================================================

    sf::RectangleShape panel(
        {
            ancho - 40.f,
            altoPanel
        }
    );

    panel.setPosition(
        {
            20.f,
            yPanel
        }
    );

    panel.setFillColor(tarjetaColor);
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(bordeTarjeta);

    ventana.draw(panel);

    // =====================================================
    // TAREA SELECCIONADA
    // =====================================================

    const Tarea* tarea =
        buscarTarea(tareaSeleccionada);

    if(tarea == nullptr)
    {
        dibujarTexto(
            ventana,
            "Selecciona una tarea del calendario para ver sus detalles y subtareas.",
            16,
            38.f,
            yPanel + 18.f,
            textoNegro
        );

        return;
    }

    // =====================================================
    // MATERIA
    // =====================================================

    const Materia* materia =
        buscarMateria(
            tarea->getMateriaId()
        );

    const std::string nombreMateria =
        materia
            ? materia->getNombre()
            : "Materia " +
              std::to_string(
                  tarea->getMateriaId()
              );

    // =====================================================
    // DISTRIBUCION
    // =====================================================

    const float xSubtareas =
        ancho * 0.30f;

    const float anchoSubtareas =
        ancho * 0.35f;

    const float escalaAcciones = 1.f;

    const float xAcciones =
        ancho - 250.f;

    // =====================================================
    // INFORMACION DE LA TAREA
    // =====================================================

    std::string descripcionTarea =
        tarea->getDescripcion();

    if(
        descripcionTarea.size() >
        static_cast<std::size_t>(
            std::max(
                18.f,
                xSubtareas / 7.f
            )
        )
    )
    {
        descripcionTarea =
            descripcionTarea.substr(
                0,
                static_cast<std::size_t>(
                    std::max(
                        17.f,
                        xSubtareas / 7.f - 1.f
                    )
                )
            ) +
            "...";
    }

    dibujarTexto(
        ventana,
        "Tarea: " + tarea->getTitulo(),
        16,
        30.f,
        yPanel + 10.f
    );

    dibujarTexto(
        ventana,
        "Materia: " + nombreMateria,
        12,
        30.f,
        yPanel + 35.f
    );

    dibujarTexto(
        ventana,
        "Entrega: " +
        tarea->getFechaEntrega() +
        " | " +
        obtenerTipoTareaTexto(
            tarea->getTipo()
        ),
        11,
        30.f,
        yPanel + 55.f
    );

    EstadoTarea estadoTarea =
    obtenerEstadoTarea(tarea->getId());

    dibujarTexto(
        ventana,
        "Estado: " +
        std::string( estadoTarea == EstadoTarea::COMPLETADO
            ? "COMPLETADA"
            : "NO COMPLETADA"),
        11,
        30.f,
        yPanel + 74.f
    );

    dibujarTexto(
        ventana,
        "Descripcion: " +
        descripcionTarea,
        11,
        30.f,
        yPanel + 93.f

    );

    // =====================================================
    // BOTON PRIORIDAD
    // =====================================================

    botonPrioridad =
    {
        {30.f, yPanel + 117.f},
        {150.f * escalaAcciones, 31.f}
    };

    dibujarBoton(
        ventana,
        botonPrioridad,
        "Prioridad: cambiar",
        encabezadoColor
    );

    // =====================================================
    // BOTON AGREGAR
    // =====================================================

    botonAgregar =
    {
        {
            188.f * escalaAcciones,
            yPanel + 117.f
        },
        {
            112.f * escalaAcciones,
            31.f
        }
    };

    dibujarBoton(
        ventana,
        botonAgregar,
        "Agregar",
        encabezadoColor
    );

    // =====================================================
    // SUBTAREAS
    // =====================================================

    const std::vector<const Subtarea*> lista =
        obtenerSubtareasTarea(
            tareaSeleccionada
        );

    dibujarTexto(
        ventana,
        "Subtareas:",
        13,
        xSubtareas,
        yPanel + 8.f,
        textoNegro
    );

    float y =
        yPanel + 29.f;

    const float altoFila = 22.f;
    const std::size_t maxFilas = 4;

    for(
        std::size_t filaIndice = 0;
        filaIndice < maxFilas &&
        static_cast<std::size_t>(
            desplazamientoSubtareas
        ) + filaIndice < lista.size();
        ++filaIndice
    )
    {
        const std::size_t i =
            static_cast<std::size_t>(
                desplazamientoSubtareas
            ) + filaIndice;

        const Subtarea& subtarea =
            *lista[i];

        const sf::FloatRect fila =
        {
            {xSubtareas, y},
            {anchoSubtareas, altoFila}
        };

        sf::RectangleShape forma(
            {
                fila.size.x,
                fila.size.y
            }
        );

        forma.setPosition(
            fila.position
        );

        forma.setFillColor(
            subtarea.getId() ==
            subtareaSeleccionada
                ? sf::Color(225, 225, 225)
                : tarjetaColor
        );

        forma.setOutlineThickness(1.f);
        forma.setOutlineColor(bordeTarjeta);

        ventana.draw(forma);

        std::string descripcion =
            subtarea.getDescripcion();

        if(descripcion.size() > 29)
        {
            descripcion =
                descripcion.substr(0, 28)
                + "...";
        }

        dibujarTexto(
            ventana,
            descripcion +
                " | " +
                obtenerEstadoTexto(
                    subtarea.getEstado()
                ),
            10,
            xSubtareas + 6.f,
            y + 3.f,
            textoNegro
        );

        filasSubtareaDetalle.push_back(fila);

        idsSubtareasDetalle.push_back(
            subtarea.getId()
        );

        y += altoFila + 3.f;
    }

    if(
        lista.size() >
        static_cast<std::size_t>(
            desplazamientoSubtareas
        ) + maxFilas
    )
    {
        dibujarTexto(
            ventana,
            "Hay mas subtareas; selecciona una de las visibles.",
            10,
            xSubtareas,
            y,
            lineaSeparadora
        );
    }

    // =====================================================
    // BOTONES DE ACCIONES
    // =====================================================

    botonEditar =
    {
        {xAcciones, yPanel + 12.f},
        {88.f * escalaAcciones, 30.f}
    };

    botonEliminar =
    {
        {
            xAcciones +
            94.f * escalaAcciones,
            yPanel + 12.f
        },
        {
            112.f * escalaAcciones,
            30.f
        }
    };

    botonEstado =
    {
        {xAcciones, yPanel + 50.f},
        {206.f * escalaAcciones, 30.f}
    };

    botonColocar =
    {
        {xAcciones, yPanel + 88.f},
        {96.f * escalaAcciones, 30.f}
    };

    botonQuitar =
    {
        {
            xAcciones +
            102.f * escalaAcciones,
            yPanel + 88.f
        },
        {
            104.f * escalaAcciones,
            30.f
        }
    };

    // =====================================================
    // COLORES
    // =====================================================

    const bool subtareaElegida =
        buscarSubtarea(
            subtareaSeleccionada
        ) != nullptr;

    const sf::Color colorAccion =
        subtareaElegida
            ? encabezadoColor
            : botonGris;

    const sf::Color colorCalendario =
        subtareaElegida &&
        diaSeleccionado >= 0
            ? encabezadoColor
            : botonGris;

    // =====================================================
    // DIBUJAR BOTONES
    // =====================================================

    dibujarBoton(
        ventana,
        botonEditar,
        "Editar",
        colorAccion
    );

    dibujarBoton(
        ventana,
        botonEliminar,
        "Eliminar",
        colorAccion
    );

    dibujarBoton(
        ventana,
        botonEstado,
        subtareaElegida
            ? "Estado: " +
              obtenerEstadoTexto(
                  estadoMostrado
              )
            : "Cambiar estado",
        colorAccion
    );

    dibujarBoton(
        ventana,
        botonColocar,
        "Planificar",
        colorCalendario
    );

    dibujarBoton(
        ventana,
        botonQuitar,
        "Quitar del dia",
        colorCalendario
    );

    // =====================================================
    // FECHA SELECCIONADA
    // =====================================================

    if(!fechaSeleccionada.empty())
    {
        const std::string destino =
            diaSeleccionado >= 0
                ? "Destino: " + fechaSeleccionada
                : "Destino fuera de la semana disponible: "
                  + fechaSeleccionada;

        dibujarTexto(
            ventana,
            destino,
            11,
            xAcciones,
            yPanel + 123.f,
            textoNegro
        );
    }
}

//////////////////////////////////////////////////////////////////
// Formulario
/////////////////////////////////////////////////////////////////

void PlannerView::dibujarFormulario(sf::RenderWindow& ventana)
{
    if(formulario == Formulario::NINGUNO)
    {
        return;
    }

    // =====================================================
    // TAMAÑO LOGICO DE LA INTERFAZ
    // =====================================================

    const float ancho = 1280.f;
    const float alto = 720.f;

    const float centroX =
        ancho / 2.f;

    const float centroY =
        alto / 2.f;

    // =====================================================
    // FONDO OSCURO
    // =====================================================

    sf::RectangleShape fondo(
        {
            ancho,
            alto
        }
    );

    fondo.setFillColor(
        sf::Color(0, 0, 0, 120)
    );

    ventana.draw(fondo);

    // =====================================================
    // MODAL
    // =====================================================

    sf::RectangleShape modal(
        {
            440.f,
            220.f
        }
    );

    modal.setPosition(
        {
            centroX - 220.f,
            centroY - 110.f
        }
    );

    modal.setFillColor(tarjetaColor);
    modal.setOutlineThickness(2.f);
    modal.setOutlineColor(bordeTarjeta);

    ventana.draw(modal);

    // =====================================================
    // TITULO Y MENSAJE
    // =====================================================

    std::string titulo =
        "Nueva subtarea";

    std::string mensaje =
        "Escribe la descripcion de la subtarea:";

    if(formulario == Formulario::EDITAR)
    {
        titulo =
            "Editar subtarea";
    }
    else if(
        formulario ==
        Formulario::CONFIRMAR_ELIMINAR
    )
    {
        titulo =
            "Eliminar subtarea";

        mensaje =
            "Confirma que deseas eliminar esta subtarea.";
    }

    // =====================================================
    // ENCABEZADO DEL MODAL
    // =====================================================

    sf::RectangleShape encabezado(
        {
            440.f,
            42.f
        }
    );

    encabezado.setPosition(
        {
            centroX - 220.f,
            centroY - 110.f
        }
    );

    encabezado.setFillColor(
        encabezadoColor
    );

    ventana.draw(encabezado);

    dibujarTexto(
        ventana,
        titulo,
        17,
        centroX - 202.f,
        centroY - 101.f,
        textoBlanco
    );

    dibujarTexto(
        ventana,
        mensaje,
        13,
        centroX - 200.f,
        centroY - 52.f,
        textoNegro
    );

    // =====================================================
    // CAMPO DE DESCRIPCION
    // =====================================================

    if(
        formulario !=
        Formulario::CONFIRMAR_ELIMINAR
    )
    {
        campoDescripcion =
        {
            {
                centroX - 200.f,
                centroY - 15.f
            },
            {
                400.f,
                42.f
            }
        };

        sf::RectangleShape campo(
            {
                campoDescripcion.size.x,
                campoDescripcion.size.y
            }
        );

        campo.setPosition(
            campoDescripcion.position
        );

        campo.setFillColor(
            tarjetaColor
        );

        campo.setOutlineThickness(2.f);

        campo.setOutlineColor(
            campoActivo
                ? encabezadoColor
                : bordeTarjeta
        );

        ventana.draw(campo);

        dibujarTexto(
            ventana,
            textoFormulario,
            14,
            centroX - 193.f,
            centroY - 6.f
        );
    }

    // =====================================================
    // BOTONES
    // =====================================================

    botonFormularioAceptar =
    {
        {
            centroX + 15.f,
            centroY + 48.f
        },
        {
            125.f,
            38.f
        }
    };

    botonFormularioCancelar =
    {
        {
            centroX - 145.f,
            centroY + 48.f
        },
        {
            125.f,
            38.f
        }
    };

    dibujarBoton(
        ventana,
        botonFormularioAceptar,
        formulario ==
            Formulario::CONFIRMAR_ELIMINAR
                ? "Eliminar"
                : "Aceptar",
        formulario ==
            Formulario::CONFIRMAR_ELIMINAR
                ? botonRojo
                : encabezadoColor
    );

    dibujarBoton(
        ventana,
        botonFormularioCancelar,
        "Cancelar",
        botonGris
    );
}

////////////////////////////////////////////////////////////////
// Dibujar
///////////////////////////////////////////////////////////////

void PlannerView::draw(sf::RenderWindow& ventana)
{
    actualizarGeometria(ventana);

    sf::View vistaAnterior = ventana.getView();

    sf::View vistaLogica(
        sf::FloatRect(
            {0.f, 0.f},
            {1280.f, 720.f}
        )
    );

    ventana.setView(vistaLogica);

    const float ancho = 1280.f;
    const float alto = 720.f;

    sf::RectangleShape fondo(
        {ancho, alto}
    );

    fondo.setFillColor(
        sf::Color(245, 246, 248)
    );

    ventana.draw(fondo);

    dibujarEncabezado(ventana);
    dibujarCalendario(ventana);
    dibujarPanelDetalles(ventana);

    if(formulario != Formulario::NINGUNO)
    {
        dibujarFormulario(ventana);
    }

    ventana.setView(vistaAnterior);
}

//----------------------------------- EVENTOS ----------------------------------------

//////////////////////////////////////////////////////////////////
// Posicion del mouse
//////////////////////////////////////////////////////////////////

sf::Vector2f PlannerView::obtenerPosicionMouse(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
) const
{
    const auto* clic =
        evento.getIf<sf::Event::MouseButtonPressed>();

    if(clic == nullptr)
    {
        return {0.f, 0.f};
    }

    sf::View vistaLogica(
        sf::FloatRect(
            {0.f, 0.f},
            {1280.f, 720.f}
        )
    );

    return ventana.mapPixelToCoords(
        {
            clic->position.x,
            clic->position.y
        },
        vistaLogica
    );
}

//////////////////////////////////////////////////////////////////
//Eventos del formulario
//////////////////////////////////////////////////////////////////

void PlannerView::manejarEventoFormulario(const sf::Event& evento)
{
    if (const auto* tecla = evento.getIf<sf::Event::KeyPressed>())
    {
        if (tecla->code == sf::Keyboard::Key::Escape)
        {
            cerrarFormulario();
            return;
        }

        if (tecla->code == sf::Keyboard::Key::Enter &&
            formulario != Formulario::CONFIRMAR_ELIMINAR)
        {
            // El clic en Aceptar emite la accion para que main llame al controller.
            return;
        }
    }

    if (formulario != Formulario::CONFIRMAR_ELIMINAR && campoActivo)
    {
        if (const auto* texto = evento.getIf<sf::Event::TextEntered>())
        {
            if (texto->unicode >= 32 && texto->unicode <= 126 &&
                textoFormulario.size() < 120)
            {
                textoFormulario += static_cast<char>(texto->unicode);
            }
        }

        if (const auto* tecla = evento.getIf<sf::Event::KeyPressed>())
        {
            if (tecla->code == sf::Keyboard::Key::Backspace &&
                !textoFormulario.empty())
            {
                textoFormulario.pop_back();
            }
        }
    }
}
/////////////////////////////////////////////////////////////////
// Manejar eventos generales
/////////////////////////////////////////////////////////////////

void PlannerView::manejarEvento(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    actualizarGeometria(ventana);
    if (evento.is<sf::Event::Resized>()) return;

    if (formulario == Formulario::NINGUNO)
    {
        if (const auto* rueda = evento.getIf<sf::Event::MouseWheelScrolled>())
        {
            if (rueda->position.y >= yPanelDetalles)
            {
                const int total = static_cast<int>(obtenerSubtareasTarea(tareaSeleccionada).size());
                desplazamientoSubtareas -= rueda->delta > 0.f ? 1 : -1;
                desplazamientoSubtareas = std::max(0, std::min(desplazamientoSubtareas, std::max(0, total - 1)));
            }
            return;
        }
    }

    if (formulario != Formulario::NINGUNO)
    {
        manejarEventoFormulario(evento);

        const auto* clic = evento.getIf<sf::Event::MouseButtonPressed>();
        if (clic == nullptr || clic->button != sf::Mouse::Button::Left)
        {
            return;
        }

        const sf::Vector2f posicion = obtenerPosicionMouse(evento, ventana);

        if (formulario != Formulario::CONFIRMAR_ELIMINAR &&
            campoDescripcion.contains(posicion))
        {
            campoActivo = true;
            return;
        }

        campoActivo = false;

        if (botonFormularioCancelar.contains(posicion))
        {
            cerrarFormulario();
            return;
        }

        if (!botonFormularioAceptar.contains(posicion))
        {
            return;
        }

        if (formulario == Formulario::CONFIRMAR_ELIMINAR)
        {
            accionPendiente.tipo = TipoAccion::ELIMINAR_SUBTAREA;
            accionPendiente.idSubtarea = idSubtareaFormulario;
            cerrarFormulario();
            return;
        }

        if (textoFormulario.empty())
        {
            return;
        }

        accionPendiente.tipo = formulario == Formulario::AGREGAR
            ? TipoAccion::AGREGAR_SUBTAREA
            : TipoAccion::EDITAR_SUBTAREA;
        accionPendiente.idTarea = tareaSeleccionada;
        accionPendiente.idSubtarea = idSubtareaFormulario;
        accionPendiente.descripcion = textoFormulario;
        cerrarFormulario();
        return;
    }

    const auto* clic = evento.getIf<sf::Event::MouseButtonPressed>();
    if (clic == nullptr || clic->button != sf::Mouse::Button::Left)
    {
        return;
    }

    const sf::Vector2f posicion = obtenerPosicionMouse(evento, ventana);

    if (botonVistaSemanal.contains(posicion)) { vistaMensual = false; return; }
    if (botonVistaMensual.contains(posicion)) { vistaMensual = true; return; }
    if (vistaMensual && botonMesAnterior.contains(posicion)) { moverMes(-1); return; }
    if (vistaMensual && botonMesSiguiente.contains(posicion)) { moverMes(1); return; }

    for (std::size_t i = 0; i < tarjetasTarea.size(); ++i)
    {
        if (tarjetasTarea[i].contains(posicion))
        {
            tareaSeleccionada = idsTarjetasTarea[i];
            subtareaSeleccionada = -1;
            desplazamientoSubtareas = 0;
            const Tarea* tareaClic = buscarTarea(tareaSeleccionada);
            if (tareaClic) { fechaSeleccionada = tareaClic->getFechaEntrega(); diaSeleccionado = buscarDiaSemana(fechaSeleccionada); }
            return;
        }
    }

    if (vistaMensual)
    {
        for (std::size_t i = 0; i < celdasMes.size(); ++i)
        {
            if (celdasMes[i].contains(posicion))
            {
                fechaSeleccionada = fechasCeldasMes[i];
                diaSeleccionado = diasCeldasSemana[i];
                tareaSeleccionada = -1;
                subtareaSeleccionada = -1;
                return;
            }
        }
    }

    for (std::size_t i = 0; i < tarjetasSubtareaDia.size(); ++i)
    {
        if (tarjetasSubtareaDia[i].contains(posicion))
        {
            subtareaSeleccionada = idsSubtareasDia[i];
            diaSeleccionado = diasSubtareas[i];
            return;
        }
    }

    for (std::size_t i = 0; i < filasSubtareaDetalle.size(); ++i)
    {
        if (filasSubtareaDetalle[i].contains(posicion))
        {
            subtareaSeleccionada = idsSubtareasDetalle[i];
            const Subtarea* subtarea = buscarSubtarea(subtareaSeleccionada);
            if (subtarea != nullptr)
            {
                estadoMostrado = subtarea->getEstado();
            }
            return;
        }
    }

    for (int indiceDia = 0; indiceDia < 7; ++indiceDia)
    {
        if (columnas[indiceDia].contains(posicion))
        {
            diaSeleccionado = indiceDia;
            fechaSeleccionada = semana.getDia(indiceDia).getFecha();
            break;
        }
    }

    const Tarea* tarea = buscarTarea(tareaSeleccionada);
    if (tarea != nullptr && botonPrioridad.contains(posicion))
    {
        PrioridadPlanner prioridadActual = PrioridadPlanner::MEDIA;
        for (int indiceDia = 0; indiceDia < 7; ++indiceDia)
        {
            for (const TareaPlanner& tareaPlanner : semana.getDia(indiceDia).getTareas())
            {
                if (tareaPlanner.idTarea == tareaSeleccionada)
                    prioridadActual = tareaPlanner.prioridad;
            }
        }
        accionPendiente = {};
        accionPendiente.tipo = TipoAccion::CAMBIAR_PRIORIDAD;
        accionPendiente.idTarea = tareaSeleccionada;
        accionPendiente.prioridad = siguientePrioridad(prioridadActual);
        return;
    }

    if (tarea != nullptr && botonAgregar.contains(posicion))
    {
        abrirFormulario(Formulario::AGREGAR);
        return;
    }

    if (buscarSubtarea(subtareaSeleccionada) != nullptr)
    {
        if (botonEditar.contains(posicion))
        {
            abrirFormulario(Formulario::EDITAR, subtareaSeleccionada);
            return;
        }

        if (botonEliminar.contains(posicion))
        {
            abrirFormulario(Formulario::CONFIRMAR_ELIMINAR, subtareaSeleccionada);
            return;
        }

        if (botonEstado.contains(posicion))
        {
            accionPendiente = {};
            accionPendiente.tipo = TipoAccion::CAMBIAR_ESTADO_SUBTAREA;
            accionPendiente.idSubtarea = subtareaSeleccionada;
            accionPendiente.estado = siguienteEstado(estadoMostrado);
            estadoMostrado = accionPendiente.estado;
            return;
        }

        if (botonColocar.contains(posicion) && diaSeleccionado >= 0)
        {
            accionPendiente = {};
            accionPendiente.tipo = TipoAccion::COLOCAR_SUBTAREA;
            accionPendiente.idSubtarea = subtareaSeleccionada;
            accionPendiente.fecha = semana.getDia(diaSeleccionado).getFecha();
            return;
        }

        if (botonQuitar.contains(posicion) && diaSeleccionado >= 0)
        {
            accionPendiente = {};
            accionPendiente.tipo = TipoAccion::QUITAR_SUBTAREA;
            accionPendiente.idSubtarea = subtareaSeleccionada;
            accionPendiente.fecha = semana.getDia(diaSeleccionado).getFecha();
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Actualizar geometria
///////////////////////////////////////////////////////////////////////

void PlannerView::actualizarGeometria(const sf::RenderWindow& ventana)
{
    // =====================================================
    // TAMAÑO LOGICO DE LA INTERFAZ
    // =====================================================
    //
    // Las demás vistas de Agenda utilizan una interfaz
    // diseñada para 1280x720.
    //
    // No usamos ventana.getSize() para calcular posiciones,
    // porque al maximizar la ventana ese valor cambia y
    // provoca que PlannerView coloque elementos fuera
    // del espacio lógico de la aplicación.
    //
    const float ancho = 1280.f;
    const float alto = 720.f;

    (void)ventana;


    // =====================================================
    // DISTRIBUCION VERTICAL
    // =====================================================

    inicioCalendario = 95.f;

    const float espacioEntreSecciones = 10.f;
    const float margenInferior = 12.f;

    const float altoDisponible =
        alto
        - inicioCalendario
        - margenInferior;

    altoCalendario =
        altoDisponible * 0.60f;

    altoCalendario =
        std::max(130.f, altoCalendario);

    yPanelDetalles =
        inicioCalendario
        + altoCalendario
        + espacioEntreSecciones;


    // =====================================================
    // BOTONES DEL ENCABEZADO
    // =====================================================
    //
    // [Semana] [Mes] [<] [>]          [Regresar]
    //
    // Se mantiene dentro de 1280x720.
    //

    const float anchoBotonVista = 82.f;
    const float altoBotonVista = 40.f;

    const float anchoBotonMes = 36.f;
    const float altoBotonMes = 40.f;

    const float anchoBotonRegresar = 220.f;
    const float altoBotonRegresar = 45.f;

    const float separacion = 10.f;

    // -----------------------------------------------------
    // REGRESAR
    // -----------------------------------------------------

    botonRegresar = {
        {
            ancho - 250.f,
            15.f
        },
        {
            anchoBotonRegresar,
            altoBotonRegresar
        }
    };


    // -----------------------------------------------------
    // MES SIGUIENTE
    // -----------------------------------------------------

    botonMesSiguiente = {
        {
            botonRegresar.position.x
                - separacion
                - anchoBotonMes,
            17.5f
        },
        {
            anchoBotonMes,
            altoBotonMes
        }
    };


    // -----------------------------------------------------
    // MES ANTERIOR
    // -----------------------------------------------------

    botonMesAnterior = {
        {
            botonMesSiguiente.position.x
                - separacion
                - anchoBotonMes,
            17.5f
        },
        {
            anchoBotonMes,
            altoBotonMes
        }
    };


    // -----------------------------------------------------
    // VISTA MENSUAL
    // -----------------------------------------------------

    botonVistaMensual = {
        {
            botonMesAnterior.position.x
                - separacion
                - anchoBotonVista,
            17.5f
        },
        {
            anchoBotonVista,
            altoBotonVista
        }
    };


    // -----------------------------------------------------
    // VISTA SEMANAL
    // -----------------------------------------------------

    botonVistaSemanal = {
        {
            botonVistaMensual.position.x
                - separacion
                - anchoBotonVista,
            17.5f
        },
        {
            anchoBotonVista,
            altoBotonVista
        }
    };


    // =====================================================
    // BOTONES DEL PANEL DE DETALLES
    // =====================================================

    const float escalaAcciones = 1.f;

    const float xAcciones =
        ancho - 250.f;

    botonPrioridad = {
        {
            30.f,
            yPanelDetalles + 117.f
        },
        {
            150.f * escalaAcciones,
            31.f
        }
    };

    botonAgregar = {
        {
            188.f * escalaAcciones,
            yPanelDetalles + 117.f
        },
        {
            112.f * escalaAcciones,
            31.f
        }
    };

    botonEditar = {
        {
            xAcciones,
            yPanelDetalles + 12.f
        },
        {
            88.f * escalaAcciones,
            30.f
        }
    };

    botonEliminar = {
        {
            xAcciones + 94.f * escalaAcciones,
            yPanelDetalles + 12.f
        },
        {
            112.f * escalaAcciones,
            30.f
        }
    };

    botonEstado = {
        {
            xAcciones,
            yPanelDetalles + 50.f
        },
        {
            206.f * escalaAcciones,
            30.f
        }
    };

    botonColocar = {
        {
            xAcciones,
            yPanelDetalles + 88.f
        },
        {
            96.f * escalaAcciones,
            30.f
        }
    };

    botonQuitar = {
        {
            xAcciones + 102.f * escalaAcciones,
            yPanelDetalles + 88.f
        },
        {
            104.f * escalaAcciones,
            30.f
        }
    };


    // =====================================================
    // FORMULARIO
    // =====================================================

    const float centroX = ancho / 2.f;
    const float centroY = alto / 2.f;

    campoDescripcion = {
        {
            centroX - 200.f,
            centroY - 15.f
        },
        {
            400.f,
            42.f
        }
    };

    botonFormularioAceptar = {
        {
            centroX + 15.f,
            centroY + 48.f
        },
        {
            125.f,
            38.f
        }
    };

    botonFormularioCancelar = {
        {
            centroX - 145.f,
            centroY + 48.f
        },
        {
            125.f,
            38.f
        }
    };


    // =====================================================
    // LIMPIAR HITBOXES DEL CALENDARIO
    // =====================================================

    tarjetasTarea.clear();
    idsTarjetasTarea.clear();

    tarjetasSubtareaDia.clear();
    idsSubtareasDia.clear();

    diasSubtareas.clear();

    celdasMes.clear();
    fechasCeldasMes.clear();

    diasCeldasSemana.clear();

    filasSubtareaDetalle.clear();
    idsSubtareasDetalle.clear();


    // =====================================================
    // CALENDARIO MENSUAL
    // =====================================================

    if (
        vistaMensual &&
        mesMostrado >= 1 &&
        mesMostrado <= 12
    )
    {
        const float margen = 20.f;
        const float espacio = 5.f;

        const float yDias = inicioCalendario;
        const float altoDias = 27.f;

        const float anchoCelda =
            (
                ancho
                - margen * 2.f
                - espacio * 6.f
            ) / 7.f;

        const float altoCelda =
            std::max(
                28.f,
                (
                    altoCalendario
                    - altoDias
                    - 5.f
                ) / 6.f
            );

        const int offset =
            lunesIndexPlanner(
                anioMostrado,
                mesMostrado,
                1
            );

        const int cantidadDias =
            diasEnMesPlanner(
                anioMostrado,
                mesMostrado
            );

        for (int indice = 0; indice < 42; ++indice)
        {
            const int diaInicial =
                indice - offset + 1;

            int anioCelda = anioMostrado;
            int mesCelda = mesMostrado;
            int diaCelda = diaInicial;

            if (diaInicial < 1)
            {
                if (--mesCelda == 0)
                {
                    mesCelda = 12;
                    --anioCelda;
                }

                diaCelda =
                    diasEnMesPlanner(
                        anioCelda,
                        mesCelda
                    ) + diaInicial;
            }
            else if (diaInicial > cantidadDias)
            {
                if (++mesCelda == 13)
                {
                    mesCelda = 1;
                    ++anioCelda;
                }

                diaCelda =
                    diaInicial - cantidadDias;
            }

            const std::string fecha =
                fechaPlanner(
                    anioCelda,
                    mesCelda,
                    diaCelda
                );

            const int fila = indice / 7;
            const int columna = indice % 7;

            const float x =
                margen
                + columna * (anchoCelda + espacio);

            const float y =
                yDias
                + altoDias
                + 5.f
                + fila * altoCelda;

            celdasMes.push_back(
                {
                    {x, y},
                    {anchoCelda, altoCelda - 2.f}
                }
            );

            fechasCeldasMes.push_back(fecha);

            diasCeldasSemana.push_back(
                buscarDiaSemana(fecha)
            );

            std::vector<const Tarea*> tareasDia;

            for (const Tarea& tarea : tareas)
            {
                if (tarea.getFechaEntrega() == fecha)
                    tareasDia.push_back(&tarea);
            }

            float yTarea = y + 19.f;

            const int mostrar =
                altoCelda >= 55.f ? 2 : 1;

            for (
                int i = 0;
                i < static_cast<int>(tareasDia.size())
                && i < mostrar;
                ++i
            )
            {
                tarjetasTarea.push_back(
                    {
                        {x + 3.f, yTarea},
                        {anchoCelda - 6.f, 16.f}
                    }
                );

                idsTarjetasTarea.push_back(
                    tareasDia[i]->getId()
                );

                yTarea += 17.f;
            }

            if (
                static_cast<int>(tareasDia.size())
                > mostrar
            )
            {
                yTarea += 13.f;
            }

            const int indiceSemana =
                buscarDiaSemana(fecha);

            if (indiceSemana >= 0)
            {
                for (
                    int idSubtarea :
                    semana.getDia(indiceSemana).getSubtareas()
                )
                {
                    const Subtarea* subtarea =
                        buscarSubtarea(idSubtarea);

                    if (
                        subtarea &&
                        yTarea + 15.f
                        < y + altoCelda - 2.f
                    )
                    {
                        tarjetasSubtareaDia.push_back(
                            {
                                {x + 3.f, yTarea},
                                {anchoCelda - 6.f, 14.f}
                            }
                        );

                        idsSubtareasDia.push_back(
                            idSubtarea
                        );

                        diasSubtareas.push_back(
                            indiceSemana
                        );

                        yTarea += 14.f;
                    }
                }
            }
        }
    }


    // =====================================================
    // CALENDARIO SEMANAL
    // =====================================================

    else
    {
        const float margen = 20.f;
        const float espacio = 6.f;

        const float anchoColumna =
            (
                ancho
                - margen * 2.f
                - espacio * 6.f
            ) / 7.f;

        for (
            int indiceDia = 0;
            indiceDia < 7;
            ++indiceDia
        )
        {
            const float x =
                margen
                + indiceDia
                * (anchoColumna + espacio);

            columnas[indiceDia] = {
                {x, inicioCalendario},
                {anchoColumna, altoCalendario}
            };

            float y =
                inicioCalendario + 43.f;

            const PlannerDia& dia =
                semana.getDia(indiceDia);

            for (
                const TareaPlanner& tp :
                dia.getTareas()
            )
            {
                if (
                    !buscarTarea(tp.idTarea)
                    ||
                    y + 39.f
                    > inicioCalendario
                    + altoCalendario
                )
                {
                    continue;
                }

                tarjetasTarea.push_back(
                    {
                        {x + 4.f, y},
                        {anchoColumna - 8.f, 37.f}
                    }
                );

                idsTarjetasTarea.push_back(
                    tp.idTarea
                );

                y += 41.f;
            }

            for (int id : dia.getSubtareas())
            {
                if (
                    !buscarSubtarea(id)
                    ||
                    y + 29.f
                    > inicioCalendario
                    + altoCalendario
                )
                {
                    continue;
                }

                tarjetasSubtareaDia.push_back(
                    {
                        {x + 4.f, y},
                        {anchoColumna - 8.f, 27.f}
                    }
                );

                idsSubtareasDia.push_back(id);

                diasSubtareas.push_back(
                    indiceDia
                );

                y += 30.f;
            }
        }
    }


    // =====================================================
    // DETALLE DE LA TAREA SELECCIONADA
    // =====================================================

    if (buscarTarea(tareaSeleccionada))
    {
        const float xSubtareas =
            ancho * 0.30f;

        float y =
            yPanelDetalles + 29.f;

        const std::vector<const Subtarea*> lista =
            obtenerSubtareasTarea(
                tareaSeleccionada
            );

        for (
            std::size_t fila = 0;
            fila < 4 &&
            static_cast<std::size_t>(
                desplazamientoSubtareas
            ) + fila < lista.size();
            ++fila
        )
        {
            const std::size_t i =
                static_cast<std::size_t>(
                    desplazamientoSubtareas
                ) + fila;

            filasSubtareaDetalle.push_back(
                {
                    {xSubtareas, y},
                    {ancho * 0.35f, 22.f}
                }
            );

            idsSubtareasDetalle.push_back(
                lista[i]->getId()
            );

            y += 25.f;
        }
    }
}

/////////////////////////////////////////////////////////////////////
// Cambiar mes
////////////////////////////////////////////////////////////////////

void PlannerView::moverMes(int desplazamiento)
{
    mesMostrado += desplazamiento;
    if (mesMostrado < 1) { mesMostrado = 12; --anioMostrado; }
    else if (mesMostrado > 12) { mesMostrado = 1; ++anioMostrado; }
    tareaSeleccionada = -1;
    subtareaSeleccionada = -1;
}

//-------------------------------------------------
// BOTONES Y SELECCION
//-------------------------------------------------

bool PlannerView::regresarPresionado(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
) const
{
    const auto* clic = evento.getIf<sf::Event::MouseButtonPressed>();
    if (clic == nullptr || clic->button != sf::Mouse::Button::Left)
    {
        return false;
    }

    /*const sf::Vector2f posicion(
        static_cast<float>(clic->position.x),
        static_cast<float>(clic->position.y)
    ); prueba*/

    const sf::Vector2f posicion =
    obtenerPosicionMouse(evento, ventana);

    return formulario == Formulario::NINGUNO &&
           botonRegresar.contains(posicion);
}

int PlannerView::obtenerTareaSeleccionada() const
{
    return tareaSeleccionada;
}

PlannerView::Accion PlannerView::obtenerAccion() const
{
    return accionPendiente;
}

void PlannerView::limpiarAccion()
{
    accionPendiente = Accion{};
}

bool PlannerView::mostrandoFormulario() const
{
    return formulario != Formulario::NINGUNO;
}

void PlannerView::cerrarFormularios()
{
    cerrarFormulario();
}

//-------------------------------------------------
// CONSULTAS Y FORMULARIOS
//-------------------------------------------------

const Tarea* PlannerView::buscarTarea(int idTarea) const
{
    for (const Tarea& tarea : tareas)
    {
        if (tarea.getId() == idTarea)
        {
            return &tarea;
        }
    }
    return nullptr;
}

const Subtarea* PlannerView::buscarSubtarea(int idSubtarea) const
{
    for (const Subtarea& subtarea : subtareas)
    {
        if (subtarea.getId() == idSubtarea)
        {
            return &subtarea;
        }
    }
    return nullptr;
}

const Materia* PlannerView::buscarMateria(int idMateria) const
{
    for (const Materia& materia : materias)
    {
        if (materia.getId() == idMateria)
        {
            return &materia;
        }
    }
    return nullptr;
}

std::vector<const Subtarea*> PlannerView::obtenerSubtareasTarea(int idTarea) const
{
    std::vector<const Subtarea*> resultado;
    for (const Subtarea& subtarea : subtareas)
    {
        if (subtarea.getTareaId() == idTarea)
        {
            resultado.push_back(&subtarea);
        }
    }
    return resultado;
}

std::string PlannerView::obtenerNombreDia(int indice) const
{
    static const std::array<std::string, 7> nombres = {
        "LUN", "MAR", "MIE", "JUE", "VIE", "SAB", "DOM"
    };
    if (indice < 0 || indice >= static_cast<int>(nombres.size()))
    {
        return "";
    }
    return nombres[indice];
}

std::string PlannerView::obtenerNombreMes(int mes) const
{
    static const char* nombres[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    return (mes >= 1 && mes <= 12) ? nombres[mes] : "Mes";
}

int PlannerView::obtenerIndiceDiaSemana(const std::string& fecha) const
{
    int anio = 0, mes = 0, dia = 0;
    if (!parsearFechaPlanner(fecha, anio, mes, dia)) return -1;
    return lunesIndexPlanner(anio, mes, dia);
}

int PlannerView::buscarDiaSemana(const std::string& fecha) const
{
    for (int i = 0; i < 7; ++i)
        if (semana.getDia(i).getFecha() == fecha) return i;
    return -1;
}

std::string PlannerView::obtenerFechaCorta(const std::string& fecha) const
{
    if (fecha.size() < 10)
    {
        return fecha;
    }
    return fecha.substr(8, 2) + "/" + fecha.substr(5, 2);
}

std::string PlannerView::obtenerPrioridadTexto(PrioridadPlanner prioridad) const
{
    switch (prioridad)
    {
        case PrioridadPlanner::BAJA: return "BAJA";
        case PrioridadPlanner::MEDIA: return "MEDIA";
        case PrioridadPlanner::ALTA: return "ALTA";
    }
    return "MEDIA";
}

std::string PlannerView::obtenerTipoTareaTexto(TipoTarea tipo) const
{
    switch (tipo)
    {
        case TipoTarea::TAREA: return "TAREA";
        case TipoTarea::EXAMEN: return "EXAMEN";
        case TipoTarea::PRACTICA: return "PRACTICA";
        case TipoTarea::PROYECTO: return "PROYECTO";
        case TipoTarea::TRABAJO: return "TRABAJO";
        case TipoTarea::OTRO: return "OTRO";
    }
    return "OTRO";
}

std::string PlannerView::obtenerEstadoTexto(EstadoSubtarea estado) const
{
    switch (estado)
    {
        case EstadoSubtarea::PENDIENTE: return "PENDIENTE";
        case EstadoSubtarea::EN_PROGRESO: return "EN PROGRESO";
        case EstadoSubtarea::COMPLETADA: return "COMPLETADA";
    }
    return "PENDIENTE";
}

PrioridadPlanner PlannerView::siguientePrioridad(PrioridadPlanner prioridad) const
{
    if (prioridad == PrioridadPlanner::BAJA) return PrioridadPlanner::MEDIA;
    if (prioridad == PrioridadPlanner::MEDIA) return PrioridadPlanner::ALTA;
    return PrioridadPlanner::BAJA;
}

EstadoSubtarea PlannerView::siguienteEstado(EstadoSubtarea estado) const
{
    if (estado == EstadoSubtarea::PENDIENTE) return EstadoSubtarea::EN_PROGRESO;
    if (estado == EstadoSubtarea::EN_PROGRESO) return EstadoSubtarea::COMPLETADA;
    return EstadoSubtarea::PENDIENTE;
}

void PlannerView::abrirFormulario(Formulario tipo, int idSubtarea)
{
    formulario = tipo;
    idSubtareaFormulario = idSubtarea;
    textoFormulario.clear();
    campoActivo = true;

    if (tipo == Formulario::EDITAR)
    {
        const Subtarea* subtarea = buscarSubtarea(idSubtarea);
        if (subtarea != nullptr)
        {
            textoFormulario = subtarea->getDescripcion();
        }
    }
}

void PlannerView::cerrarFormulario()
{
    formulario = Formulario::NINGUNO;
    idSubtareaFormulario = -1;
    textoFormulario.clear();
    campoActivo = false;
}


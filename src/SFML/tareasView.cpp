#include "SFML/tareasView.hpp"

#include <algorithm>
#include <sstream>
#include <string>

/////////////////////////////////////////////////////////////
// FUNCIONES AUXILIARES LOCALES
//////////////////////////////////////////////////////////////

static std::string tipoTareaAStringLocal(
    TipoTarea tipo
)
{
    switch(tipo)
    {
        case TipoTarea::TAREA:
            return "TAREA";

        case TipoTarea::EXAMEN:
            return "EXAMEN";

        case TipoTarea::PRACTICA:
            return "PRACTICA";

        case TipoTarea::PROYECTO:
            return "PROYECTO";

        case TipoTarea::TRABAJO:
            return "TRABAJO";

        case TipoTarea::OTRO:
            return "OTRO";

        default:
            return "OTRO";
    }
}


static std::string estadoTareaAStringLocal(
    EstadoTarea estado
)
{
    switch(estado)
    {
        case EstadoTarea::COMPLETADO:
            return "COMPLETADO";

        case EstadoTarea::NO_COMPLETADO:
            return "NO_COMPLETADO";

        default:
            return "NO_COMPLETADO";
    }
}




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
static const sf::Color botonRojo(220, 70, 70);
static const sf::Color botonGris(110, 110, 110);

//////////////////////////////////////////////////////////////
// CONSTRUCTOR
//////////////////////////////////////////////////////////////

TareasView::TareasView()
{
    rol = "";

    idMateriaSeleccionada = -1;

    desplazamientoTareas = 0.f;
    velocidadDesplazamiento = 30.f;

    botonRegresar = sf::FloatRect(
        {1030.f, 18.f},
        {220.f, 45.f}
    );

    botonMateriaAnterior = sf::FloatRect(
        {625.f, 90.f},
        {45.f, 40.f}
    );

    botonMateriaSiguiente = sf::FloatRect(
        {680.f, 90.f},
        {45.f, 40.f}
    );

    botonAgregar = sf::FloatRect(
        {850.f, 90.f},
        {180.f, 45.f}
    );

    mostrandoEstados = false;
    idTareaEstados = -1;

    idAlumnoSeleccionado = -1;

    mostrandoAgregarTarea = false;
    idTareaEditando = -1;
    mostrandoEditarTarea = false;

    mostrandoEliminarTarea = false;
    idTareaEliminar = -1;

    mostrandoAgregarCalificacion = false;
    idTareaCalificacion = -1;
    idAlumnoCalificacion = -1;

    mostrandoEditarCalificacion = false;
    idTareaCalificacionEditar = -1;
    idAlumnoCalificacionEditar = -1;

    mostrandoEliminarCalificacion = false;
    idTareaCalificacionEliminar = -1;
    idAlumnoCalificacionEliminar = -1;

    campoActivoAgregar = -1;
    campoActivoEditar = -1;

    //////////////////////////////////////////////////////////
    // TEXTBOX AGREGAR
    //////////////////////////////////////////////////////////

    txtTituloAgregar.setPosition(430.f, 220.f);
    txtTituloAgregar.setSize(420.f, 40.f);

    txtFechaAgregar.setPosition(430.f, 285.f);
    txtFechaAgregar.setSize(420.f, 40.f);

    txtDescripcionAgregar.setPosition(430.f, 350.f);
    txtDescripcionAgregar.setSize(420.f, 40.f);

    txtTipoAgregar.setPosition(430.f, 415.f);
    txtTipoAgregar.setSize(420.f, 40.f);

    txtParcialAgregar.setPosition(430.f, 480.f);
    txtParcialAgregar.setSize(420.f, 40.f);

    //////////////////////////////////////////////////////////
    // TEXTBOX EDITAR
    //////////////////////////////////////////////////////////

    txtTituloEditar.setPosition(430.f, 220.f);
    txtTituloEditar.setSize(420.f, 40.f);

    txtFechaEditar.setPosition(430.f, 285.f);
    txtFechaEditar.setSize(420.f, 40.f);

    txtDescripcionEditar.setPosition(430.f, 350.f);
    txtDescripcionEditar.setSize(420.f, 40.f);

    txtTipoEditar.setPosition(430.f, 415.f);
    txtTipoEditar.setSize(420.f, 40.f);

    txtParcialEditar.setPosition(430.f, 480.f);
    txtParcialEditar.setSize(420.f, 40.f);

    //////////////////////////////////////////////////////////
    // TEXTBOX CALIFICACIONES
    //////////////////////////////////////////////////////////

    txtCalificacionAgregar.setPosition(430.f, 350.f);
    txtCalificacionAgregar.setSize(420.f, 40.f);

    txtCalificacionEditar.setPosition(430.f, 350.f);
    txtCalificacionEditar.setSize(420.f, 40.f);
}

//////////////////////////////////////////////////////////////
// CARGAR FUENTE
//////////////////////////////////////////////////////////////

bool TareasView::cargarFuente(
    const std::string& ruta
)
{
    return font.openFromFile(ruta);
}

//////////////////////////////////////////////////////////////
// ROL
//////////////////////////////////////////////////////////////

void TareasView::setRol(
    const std::string& nuevoRol
)
{
    rol = nuevoRol;
}

//////////////////////////////////////////////////////////////
// MATERIAS
//////////////////////////////////////////////////////////////

void TareasView::setMaterias(
    const std::vector<Materia>& nuevasMaterias
)
{
    materias = nuevasMaterias;

    if(materias.empty())
    {
        idMateriaSeleccionada = -1;
        tareas.clear();
        estadosTarea.clear();
        calificaciones.clear();

        return;
    }

    bool existeMateria = false;

    for(const auto& materia : materias)
    {
        if(
            materia.getId() ==
            idMateriaSeleccionada
        )
        {
            existeMateria = true;
            break;
        }
    }

    if(!existeMateria)
    {
        idMateriaSeleccionada =
            materias[0].getId();
    }
}

//////////////////////////////////////////////////////////////
// MATERIA SELECCIONADA
//////////////////////////////////////////////////////////////

void TareasView::setMateriaSeleccionada(
    int idMateria
)
{
    idMateriaSeleccionada = idMateria;

    desplazamientoTareas = 0.f;
}

//////////////////////////////////////////////////////////////

int TareasView::obtenerMateriaSeleccionada() const
{
    return idMateriaSeleccionada;
}

void TareasView::setProfesores(
    const std::vector<std::string>& nuevosProfesores
)
{
    profesores = nuevosProfesores;
}

//////////////////////////////////////////////////////////////
// TAREAS
//////////////////////////////////////////////////////////////

void TareasView::setTareas(
    const std::vector<Tarea>& nuevasTareas
)
{
    tareas = nuevasTareas;

    desplazamientoTareas = 0.f;

    botonesEditar.clear();
    botonesEliminar.clear();
    botonesAlumnos.clear();
    botonesCompletar.clear();
    botonesNoCompletar.clear();
}

//////////////////////////////////////////////////////////////

void TareasView::limpiarTareas()
{
    tareas.clear();

    botonesEditar.clear();
    botonesEliminar.clear();
    botonesAlumnos.clear();

    botonesCompletar.clear();
    botonesNoCompletar.clear();

    desplazamientoTareas = 0.f;
}

//////////////////////////////////////////////////////////////
// RECARGAR
//////////////////////////////////////////////////////////////

void TareasView::recargar()
{
    // La vista ya no carga información.
    // La recarga se realiza desde mainSFML.cpp.
}

//////////////////////////////////////////////////////////////
// ESTADOS
//////////////////////////////////////////////////////////////

void TareasView::setEstadosTarea(
    const std::vector<EstadoTareaAlumno>& nuevosEstados
)
{
    estadosTarea = nuevosEstados;

    //////////////////////////////////////////////////////////
    // Si se está mostrando una tarea para el profesor,
    // estos mismos estados representan a los alumnos.
    //////////////////////////////////////////////////////////

    if(mostrandoEstados)
    {
        alumnosEstados = nuevosEstados;
    }
}

//////////////////////////////////////////////////////////////

void TareasView::limpiarEstados()
{
    estadosTarea.clear();
    alumnosEstados.clear();
    calificacionesEstados.clear();

    mostrandoEstados = false;

    idTareaEstados = -1;
    idAlumnoSeleccionado = -1;
}


/////////////////////////////////////////////////////////////
// NOMBRE ALUMNOS
/////////////////////////////////////////////////////////////

void TareasView::setAlumnosTarea(
    const std::vector<AlumnoTarea>& nuevosAlumnos
)
{
    alumnosTarea = nuevosAlumnos;
}

void TareasView::limpiarAlumnosTarea()
{
    alumnosTarea.clear();
}



//////////////////////////////////////////////////////////////
// CALIFICACIONES
//////////////////////////////////////////////////////////////

void TareasView::setCalificaciones(
    const std::vector<Calificacion>& nuevasCalificaciones
)
{
    calificaciones = nuevasCalificaciones;

    if(mostrandoEstados)
    {
        calificacionesEstados =
            nuevasCalificaciones;
    }
}

//////////////////////////////////////////////////////////////

void TareasView::limpiarCalificaciones()
{
    calificaciones.clear();
    calificacionesEstados.clear();
}

//////////////////////////////////////////////////////////////
// MANEJO GENERAL DE EVENTOS
//////////////////////////////////////////////////////////////

void TareasView::manejarEvento(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // MODALES
    //////////////////////////////////////////////////////////

    if(mostrandoAgregarTarea)
    {
        manejarEventosAgregarTarea(
            evento,
            ventana
        );

        return;
    }

    if(mostrandoEditarTarea)
    {
        manejarEventosEditarTarea(
            evento,
            ventana
        );

        return;
    }

    if(mostrandoEliminarTarea)
    {
        manejarEventosEliminarTarea(
            evento,
            ventana
        );

        return;
    }

    //////////////////////////////////////////////////////////////
    // MODALES DE CALIFICACIONES
    //////////////////////////////////////////////////////////////

    if(mostrandoAgregarCalificacion)
    {
        manejarEventosAgregarCalificacion(evento,ventana);

        return;
    }

    if(mostrandoEditarCalificacion)
    {   
        manejarEventosEditarCalificacion(evento,ventana);

        return;
    }

    if(mostrandoEliminarCalificacion)
    {
        manejarEventosEliminarCalificacion(evento,ventana);

        return;
    }

    //////////////////////////////////////////////////////////////
    // VENTANA DE ESTADOS / ALUMNOS
    //////////////////////////////////////////////////////////////

    if(mostrandoEstados)
    {
        manejarEventosEstados(evento,ventana);

        return;
    }

    //////////////////////////////////////////////////////////
    // EVENTOS NORMALES
    //////////////////////////////////////////////////////////

    manejarEventosGenerales(
        evento,
        ventana
    );

    if(rol == "Profesor")
    {
        manejarEventosProfesor(
            evento,
            ventana
        );
    }
    else if(rol == "Alumno")
    {
        manejarEventosAlumno(
            evento,
            ventana
        );
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS GENERALES
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosGenerales(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // SCROLL
    //////////////////////////////////////////////////////////

    if(
        const auto* rueda =
        evento.getIf<sf::Event::MouseWheelScrolled>()
    )
    {
        desplazamientoTareas -=
            rueda->delta *
            velocidadDesplazamiento;

        if(desplazamientoTareas < 0.f)
        {
            desplazamientoTareas = 0.f;
        }

        float alturaContenido =
            tareas.size() *
            (
                rol == "Profesor"
                    ? 185.f
                    : 170.f
            );

        float alturaVisible = 500.f;

        float maxDesplazamiento =
            alturaContenido >
            alturaVisible
                ? alturaContenido - alturaVisible
                : 0.f;

        if(
            desplazamientoTareas >
            maxDesplazamiento
        )
        {
            desplazamientoTareas =
                maxDesplazamiento;
        }

        return;
    }

    //////////////////////////////////////////////////////////
    // CLICK
    //////////////////////////////////////////////////////////

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    sf::Vector2f posicionMouse =
        ventana.mapPixelToCoords(
            mouse->position
        );

    //////////////////////////////////////////////////////////
    // REGRESAR
    //////////////////////////////////////////////////////////

    if(
        botonRegresar.contains(
            posicionMouse
        )
    )
    {
        return;
    }

    //////////////////////////////////////////////////////////
    // MATERIA ANTERIOR
    //////////////////////////////////////////////////////////

    if(
        botonMateriaAnterior.contains(
            posicionMouse
        )
    )
    {
        if(!materias.empty())
        {
            int indiceActual = -1;

            for(
                std::size_t i = 0;
                i < materias.size();
                ++i
            )
            {
                if(
                    materias[i].getId() ==
                    idMateriaSeleccionada
                )
                {
                    indiceActual =
                        static_cast<int>(i);

                    break;
                }
            }

            if(indiceActual > 0)
            {
                idMateriaSeleccionada =
                    materias[
                        indiceActual - 1
                    ].getId();

                desplazamientoTareas = 0.f;
            }
        }

        return;
    }

    //////////////////////////////////////////////////////////
    // MATERIA SIGUIENTE
    //////////////////////////////////////////////////////////

    if(
        botonMateriaSiguiente.contains(
            posicionMouse
        )
    )
    {
        if(!materias.empty())
        {
            int indiceActual = -1;

            for(
                std::size_t i = 0;
                i < materias.size();
                ++i
            )
            {
                if(
                    materias[i].getId() ==
                    idMateriaSeleccionada
                )
                {
                    indiceActual =
                        static_cast<int>(i);

                    break;
                }
            }

            if(
                indiceActual >= 0 &&
                indiceActual + 1 <
                    static_cast<int>(
                        materias.size()
                    )
            )
            {
                idMateriaSeleccionada =
                    materias[
                        indiceActual + 1
                    ].getId();

                desplazamientoTareas = 0.f;
            }
        }

        return;
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS DEL PROFESOR
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosProfesor(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    sf::Vector2f posicionMouse =
        ventana.mapPixelToCoords(
            mouse->position
        );

    //////////////////////////////////////////////////////////
    // AGREGAR TAREA
    //////////////////////////////////////////////////////////

    if(
        botonAgregar.contains(
            posicionMouse
        )
    )
    {
        mostrandoAgregarTarea = true;

        limpiarFormularioAgregar();

        return;
    }

    //////////////////////////////////////////////////////////
    // EDITAR
    //////////////////////////////////////////////////////////

    for(
        std::size_t i = 0;
        i < botonesEditar.size();
        ++i
    )
    {
        if(
            botonesEditar[i].contains(
                posicionMouse
            )
        )
        {
            if(i < tareas.size())
            {
                idTareaEditando =
                    tareas[i].getId();

                txtTituloEditar.setText(
                    tareas[i].getTitulo()
                );

                txtFechaEditar.setText(
                    tareas[i].getFechaEntrega()
                );

                txtDescripcionEditar.setText(
                    tareas[i].getDescripcion()
                );

                txtTipoEditar.setText(
                    tipoTareaAStringLocal(
                        tareas[i].getTipo()
                    )
                );

                txtParcialEditar.setText(
                    std::to_string(
                        tareas[i].getParcial()
                    )
                );

                campoActivoEditar = -1;

                mostrandoEditarTarea = true;
            }

            return;
        }
    }

    //////////////////////////////////////////////////////////
    // ELIMINAR
    //////////////////////////////////////////////////////////

    for(
        std::size_t i = 0;
        i < botonesEliminar.size();
        ++i
    )
    {
        if(
            botonesEliminar[i].contains(
                posicionMouse
            )
        )
        {
            if(i < tareas.size())
            {
                idTareaEliminar =
                    tareas[i].getId();

                mostrandoEliminarTarea = true;
            }

            return;
        }
    }

    //////////////////////////////////////////////////////////
    // VER ALUMNOS
    //////////////////////////////////////////////////////////

    for(
        std::size_t i = 0;
        i < botonesAlumnos.size();
        ++i
    )
    {
        if(
            botonesAlumnos[i].contains(
                posicionMouse
            )
        )
        {
            if(i < tareas.size())
            {
                idTareaEstados =
                    tareas[i].getId();

                mostrandoEstados = true;

                idAlumnoSeleccionado = -1;

                //////////////////////////////////////////////////
                // Los datos serán colocados desde mainSFML.cpp
                // mediante setEstadosTarea() y
                // setCalificaciones().
                //////////////////////////////////////////////////

                alumnosEstados.clear();
                calificacionesEstados.clear();
            }

            return;
        }
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS DEL ALUMNO
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosAlumno(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    sf::Vector2f posicionMouse =
        ventana.mapPixelToCoords(
            mouse->position
        );

    //////////////////////////////////////////////////////////
    // COMPLETAR
    //////////////////////////////////////////////////////////

    for(
        std::size_t i = 0;
        i < botonesCompletar.size();
        ++i
    )
    {
        if(
            botonesCompletar[i].contains(
                posicionMouse
            )
        )
        {
            if(i < tareas.size())
            {
                idAlumnoSeleccionado =
                    tareas[i].getId();

                return;
            }
        }
    }

    //////////////////////////////////////////////////////////
    // NO COMPLETAR
    //////////////////////////////////////////////////////////

    for(
        std::size_t i = 0;
        i < botonesNoCompletar.size();
        ++i
    )
    {
        if(
            botonesNoCompletar[i].contains(
                posicionMouse
            )
        )
        {
            if(i < tareas.size())
            {
                idAlumnoSeleccionado =
                    tareas[i].getId();

                return;
            }
        }
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS AGREGAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosAgregarTarea(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    txtTituloAgregar.handleEvent(
        evento,
        ventana
    );

    txtFechaAgregar.handleEvent(
        evento,
        ventana
    );

    txtDescripcionAgregar.handleEvent(
        evento,
        ventana
    );

    txtTipoAgregar.handleEvent(
        evento,
        ventana
    );

    txtParcialAgregar.handleEvent(
        evento,
        ventana
    );

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    if(txtTituloAgregar.isSelected())
    {
        campoActivoAgregar = 0;
    }
    else if(txtFechaAgregar.isSelected())
    {
        campoActivoAgregar = 1;
    }
    else if(txtDescripcionAgregar.isSelected())
    {
        campoActivoAgregar = 2;
    }
    else if(txtTipoAgregar.isSelected())
    {
        campoActivoAgregar = 3;
    }
    else if(txtParcialAgregar.isSelected())
    {
        campoActivoAgregar = 4;
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS EDITAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosEditarTarea(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    txtTituloEditar.handleEvent(
        evento,
        ventana
    );

    txtFechaEditar.handleEvent(
        evento,
        ventana
    );

    txtDescripcionEditar.handleEvent(
        evento,
        ventana
    );

    txtTipoEditar.handleEvent(
        evento,
        ventana
    );

    txtParcialEditar.handleEvent(
        evento,
        ventana
    );

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    if(txtTituloEditar.isSelected())
    {
        campoActivoEditar = 0;
    }
    else if(txtFechaEditar.isSelected())
    {
        campoActivoEditar = 1;
    }
    else if(txtDescripcionEditar.isSelected())
    {
        campoActivoEditar = 2;
    }
    else if(txtTipoEditar.isSelected())
    {
        campoActivoEditar = 3;
    }
    else if(txtParcialEditar.isSelected())
    {
        campoActivoEditar = 4;
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS ELIMINAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosEliminarTarea(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    (void)evento;
    (void)ventana;
}

//////////////////////////////////////////////////////////////
// EVENTOS ESTADOS / ALUMNOS
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosEstados(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return;
    }

    sf::Vector2f posicionMouse =
        ventana.mapPixelToCoords(
            mouse->position
        );

    //////////////////////////////////////////////////////////
    // CERRAR
    //////////////////////////////////////////////////////////

    sf::FloatRect botonCerrar(
        {1000.f, 620.f},
        {140.f, 45.f}
    );

    if(
        botonCerrar.contains(
            posicionMouse
        )
    )
    {
        mostrandoEstados = false;
        idTareaEstados = -1;
        idAlumnoSeleccionado = -1;

        return;
    }

    //////////////////////////////////////////////////////////
    // CALIFICAR / EDITAR / ELIMINAR
    //////////////////////////////////////////////////////////

    float posicionY = 180.f;

    for(std::size_t i = 0;i < alumnosEstados.size();++i)
    {
        int idAlumno = alumnosEstados[i].getAlumnoId();

        float y = posicionY + static_cast<float>(i) * 65.f;

        bool tiene = tieneCalificacion(idAlumno,idTareaEstados);

        if(!tiene)
        {
            sf::FloatRect botonCalificar({950.f, y},{110.f, 35.f});

            if(botonCalificar.contains(posicionMouse))
            {
                idTareaCalificacion = idTareaEstados;

                idAlumnoCalificacion = idAlumno;

                txtCalificacionAgregar.clear();
                
                txtCalificacionAgregar.setSelected(false);

                mostrandoAgregarCalificacion = true;

                return;
            }
        }
        else
        {
            sf::FloatRect botonEditar({930.f, y},{80.f, 35.f});

            sf::FloatRect botonEliminar({1020.f, y},{100.f, 35.f});

            if(
                botonEditar.contains(
                    posicionMouse
                )
            )
            {
                idTareaCalificacionEditar =
                    idTareaEstados;

                idAlumnoCalificacionEditar =
                    idAlumno;

                double calificacion =
                    obtenerCalificacion(
                        idAlumno,
                        idTareaEstados
                    );

                std::ostringstream salida;
                salida << calificacion;

                txtCalificacionEditar.setText(
                    salida.str()
                );

                txtCalificacionEditar.setSelected(
                    false
                );

                mostrandoEditarCalificacion =
                    true;

                return;
            }

            if(
                botonEliminar.contains(
                    posicionMouse
                )
            )
            {
                idTareaCalificacionEliminar =
                    idTareaEstados;

                idAlumnoCalificacionEliminar =
                    idAlumno;

                mostrandoEliminarCalificacion =
                    true;

                return;
            }
        }
    }
}

//////////////////////////////////////////////////////////////
// EVENTOS AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosAgregarCalificacion(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    txtCalificacionAgregar.handleEvent(
        evento,
        ventana
    );
}

//////////////////////////////////////////////////////////////
// EVENTOS EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosEditarCalificacion(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    txtCalificacionEditar.handleEvent(
        evento,
        ventana
    );
}

//////////////////////////////////////////////////////////////
// EVENTOS ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::manejarEventosEliminarCalificacion(
    const sf::Event& evento,
    const sf::RenderWindow& ventana
)
{
    (void)evento;
    (void)ventana;
}

//////////////////////////////////////////////////////////////
// LIMPIAR FORMULARIO AGREGAR
//////////////////////////////////////////////////////////////

void TareasView::limpiarFormularioAgregar()
{
    txtTituloAgregar.clear();
    txtFechaAgregar.clear();
    txtDescripcionAgregar.clear();
    txtTipoAgregar.clear();
    txtParcialAgregar.clear();

    txtTituloAgregar.setSelected(false);
    txtFechaAgregar.setSelected(false);
    txtDescripcionAgregar.setSelected(false);
    txtTipoAgregar.setSelected(false);
    txtParcialAgregar.setSelected(false);

    campoActivoAgregar = -1;
}

//////////////////////////////////////////////////////////////
// LIMPIAR FORMULARIO EDITAR
//////////////////////////////////////////////////////////////

void TareasView::limpiarFormularioEditar()
{
    txtTituloEditar.clear();
    txtFechaEditar.clear();
    txtDescripcionEditar.clear();
    txtTipoEditar.clear();
    txtParcialEditar.clear();

    txtTituloEditar.setSelected(false);
    txtFechaEditar.setSelected(false);
    txtDescripcionEditar.setSelected(false);
    txtTipoEditar.setSelected(false);
    txtParcialEditar.setSelected(false);

    campoActivoEditar = -1;

    idTareaEditando = -1;
}

//////////////////////////////////////////////////////////////
// LIMPIAR FORMULARIO CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::limpiarFormularioCalificacion()
{
    txtCalificacionAgregar.clear();
    txtCalificacionEditar.clear();

    txtCalificacionAgregar.setSelected(false);
    txtCalificacionEditar.setSelected(false);

    idTareaCalificacion = -1;
    idAlumnoCalificacion = -1;

    idTareaCalificacionEditar = -1;
    idAlumnoCalificacionEditar = -1;

    idTareaCalificacionEliminar = -1;
    idAlumnoCalificacionEliminar = -1;
}



//////////////////////////////////////////////////////////////
// OBTENER ESTADO
//////////////////////////////////////////////////////////////

std::string TareasView::obtenerEstadoTexto(
    int idTarea
) const
{
    for(const auto& estado : estadosTarea)
    {
        if(
            estado.getTareaId() ==
            idTarea
        )
        {
            return estadoTareaAStringLocal(
                estado.getEstado()
            );
        }
    }

    return "NO_COMPLETADO";
}

//////////////////////////////////////////////////////////////
// OBTENER CALIFICACION
//////////////////////////////////////////////////////////////

std::string TareasView::obtenerCalificacionTexto(
    int idTarea
) const
{
    for(const auto& calificacion :
        calificaciones)
    {
        if(
            calificacion.getIdTarea() ==
            idTarea
        )
        {
            std::ostringstream salida;

            salida
                << calificacion.getCalificacion();

            return salida.str();
        }
    }

    return "Sin calificacion";
}

//////////////////////////////////////////////////////////////
// TIENE CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::tieneCalificacion(
    int idAlumno,
    int idTarea
) const
{
    for(const auto& calificacion :
        calificacionesEstados)
    {
        if(
            calificacion.getIdAlumno() ==
                idAlumno &&
            calificacion.getIdTarea() ==
                idTarea
        )
        {
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////
// OBTENER CALIFICACION
//////////////////////////////////////////////////////////////

double TareasView::obtenerCalificacion(
    int idAlumno,
    int idTarea
) const
{
    for(const auto& calificacion :
        calificacionesEstados)
    {
        if(
            calificacion.getIdAlumno() ==
                idAlumno &&
            calificacion.getIdTarea() ==
                idTarea
        )
        {
            return calificacion.getCalificacion();
        }
    }

    return -1.0;
}

//////////////////////////////////////////////////////////////
// DRAW
//////////////////////////////////////////////////////////////

void TareasView::draw(
    sf::RenderWindow& ventana
)
{
    ventana.clear(
        sf::Color(245, 246, 248)
    );

    dibujarEncabezado(ventana);

    dibujarSelectorMateria(ventana);

    botonesEditar.clear();
    botonesEliminar.clear();
    botonesAlumnos.clear();

    botonesCompletar.clear();
    botonesNoCompletar.clear();


    botonesEditar.resize(tareas.size());
    botonesEliminar.resize(tareas.size());
    botonesAlumnos.resize(tareas.size());

    botonesCompletar.resize(tareas.size());
    botonesNoCompletar.resize(tareas.size());


    float posicionY = 165.f - desplazamientoTareas;

    for(std::size_t i = 0; i < tareas.size(); ++i)
    {
        dibujarTarea(
            ventana,
            tareas[i],
            posicionY,
            static_cast<int>(i)
        );

        posicionY +=
            rol == "Profesor"
                ? 185.f
                : 170.f;
    }

    //////////////////////////////////////////////////////////
    // SCROLL
    //////////////////////////////////////////////////////////

    float alturaContenido =
        tareas.size() *
        (
            rol == "Profesor"
                ? 185.f
                : 170.f
        );

    float alturaVisible = 500.f;

    if(
        alturaContenido >
        alturaVisible
    )
    {
        float alturaBarra =
            (
                alturaVisible /
                alturaContenido
            ) *
            alturaVisible;

        if(alturaBarra < 50.f)
        {
            alturaBarra = 50.f;
        }

        float maxDesplazamiento =
            alturaContenido -
            alturaVisible;

        float posicionBarra =
            165.f +
            (
                desplazamientoTareas /
                maxDesplazamiento
            ) *
            (
                alturaVisible -
                alturaBarra
            );

        sf::RectangleShape fondoBarra;

        fondoBarra.setPosition(
            {1240.f, 165.f}
        );

        fondoBarra.setSize(
            {10.f, alturaVisible}
        );

        fondoBarra.setFillColor(
            fondoScroll
        );

        ventana.draw(
            fondoBarra
        );

        sf::RectangleShape barra;

        barra.setPosition(
            {1240.f, posicionBarra}
        );

        barra.setSize(
            {10.f, alturaBarra}
        );

        barra.setFillColor(
            barraScroll
        );

        ventana.draw(barra);
    }

    //////////////////////////////////////////////////////////
    // MODALES
    //////////////////////////////////////////////////////////

    if(mostrandoAgregarTarea)
    {
        dibujarVentanaAgregarTarea(
            ventana
        );
    }

    if(mostrandoEditarTarea)
    {
        dibujarVentanaEditarTarea(
            ventana
        );
    }

    if(mostrandoEliminarTarea)
    {
        dibujarVentanaEliminarTarea(
            ventana
        );
    }

    if(mostrandoEstados)
    {
        dibujarVentanaEstados(
            ventana
        );
    }

    if(mostrandoAgregarCalificacion)
    {
        dibujarVentanaAgregarCalificacion(
            ventana
        );
    }

    if(mostrandoEditarCalificacion)
    {
        dibujarVentanaEditarCalificacion(
            ventana
        );
    }

    if(mostrandoEliminarCalificacion)
    {
        dibujarVentanaEliminarCalificacion(
            ventana
        );
    }
}

//////////////////////////////////////////////////////////////
// ENCABEZADO
//////////////////////////////////////////////////////////////

void TareasView::dibujarEncabezado(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape encabezado;

    encabezado.setPosition(
        {0.f, 0.f}
    );

    encabezado.setSize(
        {1280.f, 75.f}
    );

    encabezado.setFillColor(
        encabezadoColor
    );

    ventana.draw(encabezado);

    sf::Text titulo(
        font,
        "Tareas",
        30
    );

    titulo.setFillColor(
        textoBlanco
    );

    titulo.setPosition(
        {40.f, 20.f}
    );

    ventana.draw(titulo);

    dibujarBoton(
        ventana,
        botonRegresar,
        "Regresar al Dashboard",
        encabezadoColor

    );

    if(rol == "Profesor")
    {
        dibujarBoton(
            ventana,
            botonAgregar,
            "Agregar tarea",
            encabezadoColor
        );
    }
}

//////////////////////////////////////////////////////////////
// SELECTOR DE MATERIA
//////////////////////////////////////////////////////////////

void TareasView::dibujarSelectorMateria(
    sf::RenderWindow& ventana
)
{
    sf::Text etiqueta(
        font,
        "Materia:",
        20
    );

    etiqueta.setFillColor(
        textoNegro
    );

    etiqueta.setPosition(
        {40.f, 92.f}
    );

    ventana.draw(etiqueta);

    std::string nombreMateria =
        "Sin materia";

    for(const auto& materia : materias)
    {
        if(
            materia.getId() ==
            idMateriaSeleccionada
        )
        {
            nombreMateria =
                materia.getNombre();

            break;
        }
    }

    sf::RectangleShape tarjetaMateria;

    tarjetaMateria.setPosition(
        {140.f, 82.f}
    );

    tarjetaMateria.setSize(
        {460.f, 55.f}
    );

    tarjetaMateria.setFillColor(
        tarjetaColor
    );

    tarjetaMateria.setOutlineColor(
        bordeTarjeta
    );

    tarjetaMateria.setOutlineThickness(
        1.f
    );

    ventana.draw(
        tarjetaMateria
    );

    sf::Text textoMateria(
        font,
        nombreMateria,
        20
    );

    textoMateria.setFillColor(
        textoNegro
    );

    textoMateria.setPosition(
        {160.f, 98.f}
    );

    ventana.draw(
        textoMateria
    );

    dibujarBoton(
        ventana,
        botonMateriaAnterior,
        "<",
        encabezadoColor
    );

    dibujarBoton(
        ventana,
        botonMateriaSiguiente,
        ">",
        encabezadoColor
    );
}

//////////////////////////////////////////////////////////////
// DIBUJAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::dibujarTarea(
    sf::RenderWindow& ventana,
    const Tarea& tarea,
    float posicionY,
    int indice
)
{
    if(posicionY + 175.f < 155.f || posicionY > 720.f)
    {
        return;
    }

    //////////////////////////////////////////////////////////
    // TARJETA
    //////////////////////////////////////////////////////////

    sf::RectangleShape tarjeta;

    tarjeta.setPosition({40.f, posicionY});

    tarjeta.setSize({1160.f, 165.f});

    tarjeta.setFillColor(tarjetaColor);

    tarjeta.setOutlineColor(bordeTarjeta);

    tarjeta.setOutlineThickness(1.f);

    ventana.draw(tarjeta);

    //////////////////////////////////////////////////////////
    // TITULO
    //////////////////////////////////////////////////////////

    sf::Text titulo(font,tarea.getTitulo(),23);

    titulo.setFillColor(textoNegro);

    titulo.setPosition({65.f, posicionY + 18.f});

    ventana.draw(titulo);

    //////////////////////////////////////////////////////////
    // TIPO
    //////////////////////////////////////////////////////////

    sf::Text tipo(font,"Tipo: " + tipoTareaAStringLocal(tarea.getTipo()),17);

    tipo.setFillColor(textoNegro);

    tipo.setPosition({65.f, posicionY + 55.f});

    ventana.draw(tipo);

    //////////////////////////////////////////////////////////
    // PARCIAL
    //////////////////////////////////////////////////////////

    sf::Text parcial(font,"Parcial: " + std::to_string(tarea.getParcial()),17);

    parcial.setFillColor(textoNegro);

    parcial.setPosition({65.f, posicionY + 82.f});

    ventana.draw(parcial);

    //////////////////////////////////////////////////////////
    // FECHA
    //////////////////////////////////////////////////////////

    sf::Text fecha(font,"Entrega: " + tarea.getFechaEntrega(),17);

    fecha.setFillColor(textoNegro);

    fecha.setPosition({65.f, posicionY + 109.f});

    ventana.draw(fecha);

    //////////////////////////////////////////////////////////
    // DESCRIPCION
    //////////////////////////////////////////////////////////

    std::string descripcion = tarea.getDescripcion();

    if(descripcion.size() > 55)
    {
        descripcion = descripcion.substr(0, 52) + "...";
    }

    sf::Text textoDescripcion(font,descripcion,16);

    textoDescripcion.setFillColor(textoNegro);

    textoDescripcion.setPosition({450.f, posicionY + 55.f});

    ventana.draw(textoDescripcion);

    //////////////////////////////////////////////////////////
    // PROFESOR
    //////////////////////////////////////////////////////////

    if(rol == "Profesor")
    {
        sf::FloatRect botonEditar(
            {
                940.f,
                posicionY + 20.f
            },
            {
                95.f,
                38.f
            }
        );

        sf::FloatRect botonEliminar(
            {
                1045.f,
                posicionY + 20.f
            },
            {
                95.f,
                38.f
            }
        );

        sf::FloatRect botonAlumnos(
            {
                940.f,
                posicionY + 70.f
            },
            {
                200.f,
                38.f
            }
        );

        botonesEditar[indice] = botonEditar;
        botonesEliminar[indice] = botonEliminar;
        botonesAlumnos[indice] = botonAlumnos;

        dibujarBoton(ventana,botonEditar,"Editar",encabezadoColor);

        dibujarBoton(ventana,botonEliminar,"Eliminar", botonRojo);

        dibujarBoton(ventana,botonAlumnos,"Ver alumnos",encabezadoColor);

    }

    
    
    //////////////////////////////////////////////////////////
    // AlUMNO
    //////////////////////////////////////////////////////////
    
    if(rol == "Alumno")
    {
        // PROFESOR
        std::string nombreProfesor = "No disponible";

        for(std::size_t i = 0; i < materias.size(); ++i)
        {
            if(materias[i].getId() == tarea.getMateriaId())
            {
                if(i < profesores.size())
                {
                    nombreProfesor = profesores[i];
                }

                break;
            }
        }

        sf::Text profesor(font,"Profesor: " + nombreProfesor,16);
        
        profesor.setFillColor(textoNegro);
        
        profesor.setPosition({450.f,posicionY+18.f});
        
        ventana.draw(profesor);

        //////////////////////////////////////////////////////////
        // ESTADOS
        //////////////////////////////////////////////////////////


        sf::FloatRect botonCompletar({970.f,posicionY+25.f},{105.f,38.f});

        sf::FloatRect botonNoCompletar({970.f,posicionY+75.f},{105.f,38.f});

        botonesCompletar[indice] = botonCompletar;
        botonesNoCompletar[indice] = botonNoCompletar;
        
        dibujarBoton(ventana,botonCompletar,"Completar",encabezadoColor);
        
        dibujarBoton(ventana,botonNoCompletar,"Pendiente",botonRojo);

        sf::Text estado(font,"Estado: " + obtenerEstadoTexto(tarea.getId()),16);

        estado.setFillColor(textoNegro);
    
        estado.setPosition({450.f,posicionY+90.f});
        
        ventana.draw(estado);


        //////////////////////////////////////////////////////////
        // CALIFICACIONES
        //////////////////////////////////////////////////////////

        sf::Text calificacion(font,"Calificacion: " +
            obtenerCalificacionTexto(tarea.getId()),16);

        calificacion.setFillColor(textoNegro);
        
        calificacion.setPosition({800.f,posicionY+5.f});
        
        ventana.draw(calificacion);
    }

    //////////////////////////////////////////////////////////
    // LINEA INFERIOR
    //////////////////////////////////////////////////////////

    sf::RectangleShape linea;

    linea.setPosition(
        {
            65.f,
            posicionY + 145.f
        }
    );

    linea.setSize(
        {1110.f, 1.f}
    );

    linea.setFillColor(
        bordeTarjeta
    );

    ventana.draw(linea);
}

//////////////////////////////////////////////////////////////
// BOTON GENERICO
//////////////////////////////////////////////////////////////

void TareasView::dibujarBoton(
    sf::RenderWindow& ventana,
    const sf::FloatRect& rectangulo,
    const std::string& texto,
    const sf::Color& color
)
{
    sf::RectangleShape boton;

    boton.setPosition(
        rectangulo.position
    );

    boton.setSize(
        rectangulo.size
    );

    boton.setFillColor(
        color
    );

    boton.setOutlineColor(
        lineaSeparadora
    );

    boton.setOutlineThickness(
        1.f
    );

    ventana.draw(boton);

    sf::Text textoBoton(
        font,
        texto,
        16
    );

    textoBoton.setFillColor(
        textoBlanco
    );

    sf::FloatRect limites =
        textoBoton.getLocalBounds();

    float x =
        rectangulo.position.x +
        (
            rectangulo.size.x -
            limites.size.x
        ) / 2.f -
        limites.position.x;

    float y =
        rectangulo.position.y +
        (
            rectangulo.size.y -
            limites.size.y
        ) / 2.f -
        limites.position.y;

    textoBoton.setPosition(
        {x, y}
    );

    ventana.draw(
        textoBoton
    );
}

//////////////////////////////////////////////////////////////
// VENTANA AGREGAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaAgregarTarea(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {350.f, 80.f}
    );

    ventanaModal.setSize(
        {580.f, 570.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    sf::Text titulo(
        font,
        "Agregar tarea",
        26
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {430.f, 110.f}
    );

    ventana.draw(titulo);

    sf::Text etiquetaTitulo(
        font,
        "Titulo",
        16
    );

    etiquetaTitulo.setFillColor(
        textoNegro
    );

    etiquetaTitulo.setPosition(
        {430.f, 195.f}
    );

    ventana.draw(
        etiquetaTitulo
    );

    sf::Text etiquetaFecha(
        font,
        "Fecha de entrega",
        16
    );

    etiquetaFecha.setFillColor(
        textoNegro
    );

    etiquetaFecha.setPosition(
        {430.f, 260.f}
    );

    ventana.draw(
        etiquetaFecha
    );

    sf::Text etiquetaDescripcion(
        font,
        "Descripcion",
        16
    );

    etiquetaDescripcion.setFillColor(
        textoNegro
    );

    etiquetaDescripcion.setPosition(
        {430.f, 325.f}
    );

    ventana.draw(
        etiquetaDescripcion
    );

    sf::Text etiquetaTipo(
        font,
        "Tipo",
        16
    );

    etiquetaTipo.setFillColor(
        textoNegro
    );

    etiquetaTipo.setPosition(
        {430.f, 390.f}
    );

    ventana.draw(
        etiquetaTipo
    );

    sf::Text etiquetaParcial(
        font,
        "Parcial",
        16
    );

    etiquetaParcial.setFillColor(
        textoNegro
    );

    etiquetaParcial.setPosition(
        {430.f, 455.f}
    );

    ventana.draw(
        etiquetaParcial
    );

    txtTituloAgregar.draw(
        ventana,
        font,
        18
    );

    txtFechaAgregar.draw(
        ventana,
        font,
        18
    );

    txtDescripcionAgregar.draw(
        ventana,
        font,
        18
    );

    txtTipoAgregar.draw(
        ventana,
        font,
        18
    );

    txtParcialAgregar.draw(
        ventana,
        font,
        18
    );

    sf::FloatRect botonGuardar(
        {420.f, 545.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {610.f, 545.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonGuardar,
        "Guardar",
        encabezadoColor

    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// VENTANA EDITAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaEditarTarea(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {350.f, 80.f}
    );

    ventanaModal.setSize(
        {580.f, 570.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    sf::Text titulo(
        font,
        "Editar tarea",
        26
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {430.f, 110.f}
    );

    ventana.draw(titulo);

    sf::Text etiquetaTitulo(
        font,
        "Titulo",
        16
    );

    etiquetaTitulo.setFillColor(
        textoNegro
    );

    etiquetaTitulo.setPosition(
        {430.f, 195.f}
    );

    ventana.draw(
        etiquetaTitulo
    );

    sf::Text etiquetaFecha(
        font,
        "Fecha de entrega",
        16
    );

    etiquetaFecha.setFillColor(
        textoNegro
    );

    etiquetaFecha.setPosition(
        {430.f, 260.f}
    );

    ventana.draw(
        etiquetaFecha
    );

    sf::Text etiquetaDescripcion(
        font,
        "Descripcion",
        16
    );

    etiquetaDescripcion.setFillColor(
        textoNegro
    );

    etiquetaDescripcion.setPosition(
        {430.f, 325.f}
    );

    ventana.draw(
        etiquetaDescripcion
    );

    sf::Text etiquetaTipo(
        font,
        "Tipo",
        16
    );

    etiquetaTipo.setFillColor(
        textoNegro
    );

    etiquetaTipo.setPosition(
        {430.f, 390.f}
    );

    ventana.draw(
        etiquetaTipo
    );

    sf::Text etiquetaParcial(
        font,
        "Parcial",
        16
    );

    etiquetaParcial.setFillColor(
        textoNegro
    );

    etiquetaParcial.setPosition(
        {430.f, 455.f}
    );

    ventana.draw(
        etiquetaParcial
    );

    txtTituloEditar.draw(
        ventana,
        font,
        18
    );

    txtFechaEditar.draw(
        ventana,
        font,
        18
    );

    txtDescripcionEditar.draw(
        ventana,
        font,
        18
    );

    txtTipoEditar.draw(
        ventana,
        font,
        18
    );

    txtParcialEditar.draw(
        ventana,
        font,
        18
    );

    sf::FloatRect botonGuardar(
        {420.f, 545.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {610.f, 545.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonGuardar,
        "Guardar",
        encabezadoColor
    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// VENTANA ELIMINAR TAREA
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaEliminarTarea(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {390.f, 220.f}
    );

    ventanaModal.setSize(
        {500.f, 260.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(ventanaModal);

    sf::Text titulo(
        font,
        "Eliminar tarea",
        26
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {500.f, 250.f}
    );

    ventana.draw(titulo);

    sf::Text mensaje(
        font,
        "¿Deseas eliminar esta tarea?",
        19
    );

    mensaje.setFillColor(
        textoNegro
    );

    sf::FloatRect limites =
        mensaje.getLocalBounds();

    float x = 640.f - limites.size.x / 2.f;

    mensaje.setPosition(
        {
            x,
            315.f
        }
    );

    ventana.draw(
        mensaje
    );

    sf::FloatRect botonConfirmar(
        {440.f, 380.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {690.f, 380.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonConfirmar,
        "Eliminar",
        botonRojo
    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

/////////////////////////////////////////////////////////////
// VENTANA ESTADOS / ALUMNOS
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaEstados(
    sf::RenderWindow& ventana
)
{
    //////////////////////////////////////////////////////////
    // OSCURECER
    //////////////////////////////////////////////////////////

    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    //////////////////////////////////////////////////////////
    // VENTANA
    //////////////////////////////////////////////////////////

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {100.f, 70.f}
    );

    ventanaModal.setSize(
        {1080.f, 610.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    //////////////////////////////////////////////////////////
    // TITULO
    //////////////////////////////////////////////////////////

    std::string tituloTarea =
        "Alumnos - Tarea";

    for(const auto& tarea : tareas)
    {
        if(
            tarea.getId() ==
            idTareaEstados
        )
        {
            tituloTarea =
                "Alumnos - " +
                tarea.getTitulo();

            break;
        }
    }

    sf::Text titulo(
        font,
        tituloTarea,
        25
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {140.f, 100.f}
    );

    ventana.draw(titulo);

    //////////////////////////////////////////////////////////
    // ENCABEZADOS
    //////////////////////////////////////////////////////////

    sf::Text encabezadoAlumno(
        font,
        "Alumno",
        17
    );

    encabezadoAlumno.setFillColor(
        textoNegro
    );

    encabezadoAlumno.setPosition(
        {140.f, 150.f}
    );

    ventana.draw(
        encabezadoAlumno
    );


    sf::Text encabezadoBoleta(
        font,
        "Boleta",
        17
    );

    encabezadoBoleta.setFillColor(
        textoNegro
    );

    encabezadoBoleta.setPosition(
        {400.f, 150.f}
    );

    ventana.draw(encabezadoBoleta);


    sf::Text encabezadoEstado(
        font,
        "Estado",
        17
    );

    encabezadoEstado.setFillColor(
        textoNegro
    );

    encabezadoEstado.setPosition(
        {650.f, 150.f}
    );

    ventana.draw(
        encabezadoEstado
    );


    sf::Text encabezadoCalificacion(
        font,
        "Calificacion",
        17
    );

    encabezadoCalificacion.setFillColor(
        textoNegro
    );

    encabezadoCalificacion.setPosition(
        {800.f, 150.f}
    );

    ventana.draw(encabezadoCalificacion);

    //////////////////////////////////////////////////////////
    // ALUMNOS
    //////////////////////////////////////////////////////////

    float posicionY = 180.f;

    for(
        std::size_t i = 0;
        i < alumnosEstados.size();
        ++i
    )
    {
        const auto& estado =
            alumnosEstados[i];

        float y =
            posicionY +
            static_cast<float>(i) * 65.f;

        //////////////////////////////////////////////////////
        // LINEA
        //////////////////////////////////////////////////////

        sf::RectangleShape linea;

        linea.setPosition(
            {125.f, y + 45.f}
        );

        linea.setSize(
            {1000.f, 1.f}
        );

        linea.setFillColor(
            bordeTarjeta
        );

        ventana.draw(
            linea
        );

        //////////////////////////////////////////////////////
        // BUSCAR ALUMNO
        //////////////////////////////////////////////////////

        std::string nombreAlumno =
            "Alumno desconocido";

        std::string identificadorAlumno =
            "";

        for(const auto& alumno : alumnosTarea)
        {
            if(
                alumno.id ==
                estado.getAlumnoId()
            )
            {
                nombreAlumno =
                    alumno.nombre;

                identificadorAlumno =
                    alumno.identificador;

                break;
            }
        }

        //////////////////////////////////////////////////////
        // NOMBRE
        //////////////////////////////////////////////////////

        sf::Text textoAlumno(
            font,
            nombreAlumno,
            16
        );

        textoAlumno.setFillColor(
            textoNegro
        );

        textoAlumno.setPosition(
            {140.f, y + 5.f}
        );

        ventana.draw(
            textoAlumno
        );

        //////////////////////////////////////////////////////
        // BOLETA
        //////////////////////////////////////////////////////

        sf::Text textoBoleta(
            font,
            identificadorAlumno,
            16
        );

        textoBoleta.setFillColor(
            textoNegro
        );

        textoBoleta.setPosition(
            {400.f, y + 5.f}
        );

        ventana.draw(
            textoBoleta
        );

        //////////////////////////////////////////////////////
        // ESTADO
        //////////////////////////////////////////////////////

        std::string textoEstadoAlumno =
            estadoTareaAStringLocal(
                estado.getEstado()
            );

        sf::Text textoEstado(
            font,
            textoEstadoAlumno,
            16
        );

        textoEstado.setFillColor(
            textoNegro
        );

        textoEstado.setPosition(
            {650.f, y + 5.f}
        );

        ventana.draw(
            textoEstado
        );

        //////////////////////////////////////////////////////
        // CALIFICACION
        //////////////////////////////////////////////////////

        bool tiene =
            tieneCalificacion(
                estado.getAlumnoId(),
                idTareaEstados
            );

        std::string textoCalificacion =
            "Sin calificar";

        if(tiene)
        {
            std::ostringstream salida;

            salida <<
                obtenerCalificacion(
                    estado.getAlumnoId(),
                    idTareaEstados
                );

            textoCalificacion =
                salida.str();
        }

        sf::Text textoCalificacionVista(
            font,
            textoCalificacion,
            16
        );

        textoCalificacionVista.setFillColor(
            textoNegro
        );

        textoCalificacionVista.setPosition(
            {800.f, y + 5.f}
        );

        ventana.draw(
            textoCalificacionVista
        );

        //////////////////////////////////////////////////////
        // BOTONES
        //////////////////////////////////////////////////////

        if(!tiene)
        {
            sf::FloatRect botonCalificar(
                {950.f, y},
                {110.f, 35.f}
            );

            dibujarBoton(
                ventana,
                botonCalificar,
                "Calificar",
                encabezadoColor
            );
        }
        else
        {
            sf::FloatRect botonEditar(
                {930.f, y},
                {80.f, 35.f}
            );

            sf::FloatRect botonEliminar(
                {1020.f, y},
                {100.f, 35.f}
            );

            dibujarBoton(
                ventana,
                botonEditar,
                "Editar",
                encabezadoColor
            );

            dibujarBoton(
                ventana,
                botonEliminar,
                "Eliminar",
                botonRojo
            );
        }
    }

    //////////////////////////////////////////////////////////
    // CERRAR
    //////////////////////////////////////////////////////////

    sf::FloatRect botonCerrar(
        {1000.f, 620.f},
        {140.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonCerrar,
        "Cerrar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// VENTANA AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaAgregarCalificacion(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {350.f, 210.f}
    );

    ventanaModal.setSize(
        {580.f, 300.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    sf::Text titulo(
        font,
        "Agregar calificacion",
        25
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {430.f, 240.f}
    );

    ventana.draw(titulo);

    

    sf::Text etiqueta(
        font,
        "Calificacion",
        16
    );

    etiqueta.setFillColor(
        textoNegro
    );

    etiqueta.setPosition(
        {430.f, 325.f}
    );

    ventana.draw(etiqueta);

    txtCalificacionAgregar.draw(
        ventana,
        font,
        18
    );

    sf::FloatRect botonGuardar(
        {420.f, 430.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {610.f, 430.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonGuardar,
        "Guardar",
        encabezadoColor
    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// VENTANA EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaEditarCalificacion(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {350.f, 210.f}
    );

    ventanaModal.setSize(
        {580.f, 300.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    sf::Text titulo(
        font,
        "Editar calificacion",
        25
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {430.f, 240.f}
    );

    ventana.draw(titulo);


    sf::Text etiqueta(
        font,
        "Calificacion",
        16
    );

    etiqueta.setFillColor(
        textoNegro
    );

    etiqueta.setPosition(
        {430.f, 325.f}
    );

    ventana.draw(etiqueta);

    txtCalificacionEditar.draw(
        ventana,
        font,
        18
    );

    sf::FloatRect botonGuardar(
        {420.f, 430.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {610.f, 430.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonGuardar,
        "Guardar",
        encabezadoColor
    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// VENTANA ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::dibujarVentanaEliminarCalificacion(
    sf::RenderWindow& ventana
)
{
    sf::RectangleShape fondo;

    fondo.setPosition(
        {0.f, 0.f}
    );

    fondo.setSize(
        {1280.f, 720.f}
    );

    fondo.setFillColor(
        sf::Color(
            0,
            0,
            0,
            120
        )
    );

    ventana.draw(fondo);

    sf::RectangleShape ventanaModal;

    ventanaModal.setPosition(
        {390.f, 220.f}
    );

    ventanaModal.setSize(
        {500.f, 260.f}
    );

    ventanaModal.setFillColor(
        tarjetaColor
    );

    ventanaModal.setOutlineColor(
        bordeTarjeta
    );

    ventanaModal.setOutlineThickness(
        2.f
    );

    ventana.draw(
        ventanaModal
    );

    sf::Text titulo(
        font,
        "Eliminar calificacion",
        25
    );

    titulo.setFillColor(
        textoNegro
    );

    titulo.setPosition(
        {465.f, 250.f}
    );

    ventana.draw(titulo);

    sf::Text mensaje(
        font,
        "¿Deseas eliminar esta calificacion?",
        18
    );

    mensaje.setFillColor(
        textoNegro
    );

    sf::FloatRect limites =
        mensaje.getLocalBounds();

    float x =
        640.f -
        limites.size.x / 2.f;

    mensaje.setPosition(
        {
            x,
            315.f
        }
    );

    ventana.draw(mensaje);

    sf::FloatRect botonConfirmar(
        {440.f, 380.f},
        {150.f, 45.f}
    );

    sf::FloatRect botonCancelar(
        {690.f, 380.f},
        {150.f, 45.f}
    );

    dibujarBoton(
        ventana,
        botonConfirmar,
        "Eliminar",
        botonRojo
    );

    dibujarBoton(
        ventana,
        botonCancelar,
        "Cancelar",
        botonGris
    );
}

//////////////////////////////////////////////////////////////
// BOTON REGRESAR
//////////////////////////////////////////////////////////////

bool TareasView::regresarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    return botonRegresar.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// AGREGAR
//////////////////////////////////////////////////////////////

bool TareasView::agregarPresionado() const
{
    return mostrandoAgregarTarea;
}

//////////////////////////////////////////////////////////////
// GUARDAR AGREGAR
//////////////////////////////////////////////////////////////

bool TareasView::guardarAgregarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoAgregarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {420.f, 545.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// CANCELAR AGREGAR
//////////////////////////////////////////////////////////////

bool TareasView::cancelarAgregarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoAgregarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {610.f, 545.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// EDITAR
//////////////////////////////////////////////////////////////

int TareasView::editarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(rol != "Profesor")
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    for(
        std::size_t i = 0;
        i < botonesEditar.size();
        ++i
    )
    {
        if(
            botonesEditar[i].contains(
                posicion
            )
        )
        {
            if(i < tareas.size())
            {
                return tareas[i].getId();
            }
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// GUARDAR EDITAR
//////////////////////////////////////////////////////////////

bool TareasView::guardarEditarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEditarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {420.f, 545.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// CANCELAR EDITAR
//////////////////////////////////////////////////////////////

bool TareasView::cancelarEditarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEditarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {610.f, 545.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// ELIMINAR
//////////////////////////////////////////////////////////////

int TareasView::eliminarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(rol != "Profesor")
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    for(
        std::size_t i = 0;
        i < botonesEliminar.size();
        ++i
    )
    {
        if(
            botonesEliminar[i].contains(
                posicion
            )
        )
        {
            if(i < tareas.size())
            {
                return tareas[i].getId();
            }
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// CONFIRMAR ELIMINAR
//////////////////////////////////////////////////////////////

bool TareasView::confirmarEliminarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEliminarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {440.f, 380.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// CANCELAR ELIMINAR
//////////////////////////////////////////////////////////////

bool TareasView::cancelarEliminarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEliminarTarea)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {690.f, 380.f},
        {150.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// VER ALUMNOS
//////////////////////////////////////////////////////////////

int TareasView::alumnosPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(rol != "Profesor")
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    for(
        std::size_t i = 0;
        i < botonesAlumnos.size();
        ++i
    )
    {
        if(
            botonesAlumnos[i].contains(
                posicion
            )
        )
        {
            if(i < tareas.size())
            {
                return tareas[i].getId();
            }
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// CERRAR ESTADOS
//////////////////////////////////////////////////////////////

bool TareasView::cerrarEstadosPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEstados)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {1000.f, 620.f},
        {140.f, 45.f}
    );

    return boton.contains(posicion);
}

//////////////////////////////////////////////////////////////
// COMPLETAR
//////////////////////////////////////////////////////////////

int TareasView::completarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(rol != "Alumno")
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    for(
        std::size_t i = 0;
        i < botonesCompletar.size();
        ++i
    )
    {
        if(
            botonesCompletar[i].contains(
                posicion
            )
        )
        {
            if(i < tareas.size())
            {
                return tareas[i].getId();
            }
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// NO COMPLETAR
//////////////////////////////////////////////////////////////

int TareasView::noCompletarPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(rol != "Alumno")
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    for(
        std::size_t i = 0;
        i < botonesNoCompletar.size();
        ++i
    )
    {
        if(
            botonesNoCompletar[i].contains(
                posicion
            )
        )
        {
            if(i < tareas.size())
            {
                return tareas[i].getId();
            }
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

int TareasView::agregarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEstados)
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    float posicionY = 180.f;

    for(
        std::size_t i = 0;
        i < alumnosEstados.size();
        ++i
    )
    {
        int idAlumno =
            alumnosEstados[i].getAlumnoId();

        if(
            tieneCalificacion(
                idAlumno,
                idTareaEstados
            )
        )
        {
            continue;
        }

        float y =
            posicionY +
            static_cast<float>(i) * 65.f;

        sf::FloatRect boton(
            {790.f, y},
            {110.f, 35.f}
        );

        if(
            boton.contains(
                posicion
            )
        )
        {
            return idAlumno;
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

int TareasView::editarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEstados)
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    float posicionY = 180.f;

    for(
        std::size_t i = 0;
        i < alumnosEstados.size();
        ++i
    )
    {
        int idAlumno =
            alumnosEstados[i].getAlumnoId();

        if(
            !tieneCalificacion(
                idAlumno,
                idTareaEstados
            )
        )
        {
            continue;
        }

        float y =
            posicionY +
            static_cast<float>(i) * 65.f;

        sf::FloatRect boton(
            {700.f, y},
            {90.f, 35.f}
        );

        if(
            boton.contains(
                posicion
            )
        )
        {
            return idAlumno;
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

int TareasView::eliminarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEstados)
    {
        return -1;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return -1;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return -1;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    float posicionY = 180.f;

    for(
        std::size_t i = 0;
        i < alumnosEstados.size();
        ++i
    )
    {
        int idAlumno =
            alumnosEstados[i].getAlumnoId();

        if(
            !tieneCalificacion(
                idAlumno,
                idTareaEstados
            )
        )
        {
            continue;
        }

        float y =
            posicionY +
            static_cast<float>(i) * 65.f;

        sf::FloatRect boton(
            {800.f, y},
            {100.f, 35.f}
        );

        if(
            boton.contains(
                posicion
            )
        )
        {
            return idAlumno;
        }
    }

    return -1;
}

//////////////////////////////////////////////////////////////
// GUARDAR AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::guardarAgregarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoAgregarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {420.f, 430.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// CANCELAR AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::cancelarAgregarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoAgregarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {610.f, 430.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// GUARDAR EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::guardarEditarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEditarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {420.f, 430.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// CANCELAR EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::cancelarEditarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEditarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {610.f, 430.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// CONFIRMAR ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::confirmarEliminarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEliminarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {440.f, 380.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// CANCELAR ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::cancelarEliminarCalificacionPresionado(
    const sf::RenderWindow& ventana,
    const sf::Event& evento
) const
{
    if(!mostrandoEliminarCalificacion)
    {
        return false;
    }

    const auto* mouse =
        evento.getIf<
            sf::Event::MouseButtonPressed
        >();

    if(mouse == nullptr)
    {
        return false;
    }

    if(
        mouse->button !=
        sf::Mouse::Button::Left
    )
    {
        return false;
    }

    sf::Vector2f posicion =
        ventana.mapPixelToCoords(
            mouse->position
        );

    sf::FloatRect boton(
        {690.f, 380.f},
        {150.f, 45.f}
    );

    return boton.contains(
        posicion
    );
}

//////////////////////////////////////////////////////////////
// GETTERS AGREGAR
//////////////////////////////////////////////////////////////

std::string TareasView::obtenerTituloAgregar() const
{
    return txtTituloAgregar.getText();
}

std::string TareasView::obtenerFechaAgregar() const
{
    return txtFechaAgregar.getText();
}

std::string TareasView::obtenerDescripcionAgregar() const
{
    return txtDescripcionAgregar.getText();
}

std::string TareasView::obtenerTipoAgregar() const
{
    return txtTipoAgregar.getText();
}

std::string TareasView::obtenerParcialAgregar() const
{
    return txtParcialAgregar.getText();
}

//////////////////////////////////////////////////////////////
// GETTERS EDITAR
//////////////////////////////////////////////////////////////

std::string TareasView::obtenerTituloEditar() const
{
    return txtTituloEditar.getText();
}

std::string TareasView::obtenerFechaEditar() const
{
    return txtFechaEditar.getText();
}

std::string TareasView::obtenerDescripcionEditar() const
{
    return txtDescripcionEditar.getText();
}

std::string TareasView::obtenerTipoEditar() const
{
    return txtTipoEditar.getText();
}

std::string TareasView::obtenerParcialEditar() const
{
    return txtParcialEditar.getText();
}

//////////////////////////////////////////////////////////////
// GETTERS CALIFICACIONES
//////////////////////////////////////////////////////////////

std::string TareasView::obtenerCalificacionAgregar() const
{
    return txtCalificacionAgregar.getText();
}

std::string TareasView::obtenerCalificacionEditar() const
{
    return txtCalificacionEditar.getText();
}

int TareasView::obtenerTareaEditando() const
{
    return idTareaEditando;
}

int TareasView::obtenerTareaEliminar() const
{
    return idTareaEliminar;
}

int TareasView::obtenerTareaCalificacion() const
{
    return idTareaCalificacion;
}

int TareasView::obtenerAlumnoCalificacion() const
{
    return idAlumnoCalificacion;
}

int TareasView::obtenerTareaCalificacionEditar() const
{
    return idTareaCalificacionEditar;
}

int TareasView::obtenerAlumnoCalificacionEditar() const
{
    return idAlumnoCalificacionEditar;
}

int TareasView::obtenerTareaCalificacionEliminar() const
{
    return idTareaCalificacionEliminar;
}

int TareasView::obtenerAlumnoCalificacionEliminar() const
{
    return idAlumnoCalificacionEliminar;
}

//////////////////////////////////////////////////////////////
// LIMPIAR AGREGAR
//////////////////////////////////////////////////////////////

void TareasView::limpiarAgregar()
{
    limpiarFormularioAgregar();

    mostrandoAgregarTarea = false;
}

//////////////////////////////////////////////////////////////
// LIMPIAR EDITAR
//////////////////////////////////////////////////////////////

void TareasView::limpiarEditar()
{
    limpiarFormularioEditar();

    mostrandoEditarTarea = false;
}

//////////////////////////////////////////////////////////////
// LIMPIAR CALIFICACION
//////////////////////////////////////////////////////////////

void TareasView::limpiarCalificacion()
{
    limpiarFormularioCalificacion();

    mostrandoAgregarCalificacion = false;
    mostrandoEditarCalificacion = false;
    mostrandoEliminarCalificacion = false;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO AGREGAR
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioAgregar() const
{
    return mostrandoAgregarTarea;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO EDITAR
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioEditar() const
{
    return mostrandoEditarTarea;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO ELIMINAR
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioEliminar() const
{
    return mostrandoEliminarTarea;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO ESTADOS
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoVentanaEstados() const
{
    return mostrandoEstados;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO AGREGAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioAgregarCalificacion() const
{
    return mostrandoAgregarCalificacion;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO EDITAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioEditarCalificacion() const
{
    return mostrandoEditarCalificacion;
}

//////////////////////////////////////////////////////////////
// MOSTRANDO ELIMINAR CALIFICACION
//////////////////////////////////////////////////////////////

bool TareasView::mostrandoFormularioEliminarCalificacion() const
{
    return mostrandoEliminarCalificacion;
}

//////////////////////////////////////////////////////////////
// CERRAR FORMULARIOS
//////////////////////////////////////////////////////////////

void TareasView::cerrarFormularios()
{
    mostrandoAgregarTarea = false;
    mostrandoEditarTarea = false;
    mostrandoEliminarTarea = false;

    mostrandoEstados = false;

    mostrandoAgregarCalificacion = false;
    mostrandoEditarCalificacion = false;
    mostrandoEliminarCalificacion = false;

    idTareaEditando = -1;
    idTareaEliminar = -1;

    idTareaEstados = -1;
    idAlumnoSeleccionado = -1;

    limpiarFormularioAgregar();
    limpiarFormularioEditar();
    limpiarFormularioCalificacion();

    alumnosEstados.clear();
    calificacionesEstados.clear();
}
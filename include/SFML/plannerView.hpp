#ifndef PLANNER_HPP
#define PLANNER_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <vector>
#include <iostream>

#include "materia.hpp"
#include "plannerSemana.hpp"
#include "subtarea.hpp"
#include "tarea.hpp"
#include "estadoTareaAlumno.hpp"

using namespace std;

class PlannerView
{

public:

    //-------------------------------------------------
    // ACCIONES
    //-------------------------------------------------

    enum class TipoAccion
    {
        NINGUNA,
        CAMBIAR_PRIORIDAD,
        AGREGAR_SUBTAREA,
        EDITAR_SUBTAREA,
        ELIMINAR_SUBTAREA,
        CAMBIAR_ESTADO_SUBTAREA,
        COLOCAR_SUBTAREA,
        QUITAR_SUBTAREA
    };

    struct Accion
    {
        TipoAccion tipo = TipoAccion::NINGUNA;

        int idTarea = -1;
        int idSubtarea = -1;

        PrioridadPlanner prioridad =
            PrioridadPlanner::MEDIA;

        EstadoSubtarea estado =
            EstadoSubtarea::PENDIENTE;

        std::string descripcion;
        std::string fecha;
    };

    struct EstadoTareaPlanner
    {
        int idTarea;
        EstadoTarea estado;
    };

private:

    //-------------------------------------------------
    // FORMULARIOS
    //-------------------------------------------------

    enum class Formulario
    {
        NINGUNO,
        AGREGAR,
        EDITAR,
        CONFIRMAR_ELIMINAR
    };

    //-------------------------------------------------
    // DATOS
    //-------------------------------------------------

    sf::Font font;

    PlannerSemana semana;

    vector<Tarea> tareas;
    vector<Subtarea> subtareas;
    vector<Materia> materias;
    vector<EstadoTareaPlanner> estadosTareas;
    

    //-------------------------------------------------
    // ELEMENTOS DEL CALENDARIO
    //-------------------------------------------------

    array<sf::FloatRect, 7> columnas;

    vector<sf::FloatRect> tarjetasTarea;
    vector<int> idsTarjetasTarea;

    vector<sf::FloatRect> tarjetasSubtareaDia;
    vector<int> idsSubtareasDia;
    vector<int> diasSubtareas;

    vector<sf::FloatRect> celdasMes;
    vector<std::string> fechasCeldasMes;
    vector<int> diasCeldasSemana;

    std::vector<sf::FloatRect> filasSubtareaDetalle;
    std::vector<int> idsSubtareasDetalle;

    //-------------------------------------------------
    // BOTONES
    //-------------------------------------------------

    sf::FloatRect botonRegresar;

    sf::FloatRect botonVistaSemanal;
    sf::FloatRect botonVistaMensual;

    sf::FloatRect botonMesAnterior;
    sf::FloatRect botonMesSiguiente;

    sf::FloatRect botonPrioridad;
    sf::FloatRect botonAgregar;

    sf::FloatRect botonEditar;
    sf::FloatRect botonEliminar;
    sf::FloatRect botonEstado;

    sf::FloatRect botonColocar;
    sf::FloatRect botonQuitar;

    sf::FloatRect botonFormularioAceptar;
    sf::FloatRect botonFormularioCancelar;

    sf::FloatRect campoDescripcion;

    //-------------------------------------------------
    // ESTADO
    //-------------------------------------------------

    int tareaSeleccionada;
    int subtareaSeleccionada;
    int diaSeleccionado;

    int idSubtareaFormulario;

    int desplazamientoSubtareas;

    PrioridadPlanner prioridadMostrada;
    EstadoSubtarea estadoMostrado;

    Formulario formulario;

    std::string textoFormulario;

    bool campoActivo;

    bool vistaMensual;

    int mesMostrado;
    int anioMostrado;

    std::string fechaSeleccionada;

    //-------------------------------------------------
    // GEOMETRIA
    //-------------------------------------------------

    float inicioCalendario;
    float altoCalendario;
    float yPanelDetalles;

    //-------------------------------------------------
    // ACCION PENDIENTE
    //-------------------------------------------------

    Accion accionPendiente;

    //-------------------------------------------------
    // EVENTOS
    //-------------------------------------------------

    void manejarEventoFormulario(
        const sf::Event& evento
    );

    //-------------------------------------------------
    // POSICION DEL MOUSE
    //-------------------------------------------------

    sf::Vector2f obtenerPosicionMouse(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    ) const;

    //-------------------------------------------------
    // DIBUJADO
    //-------------------------------------------------

    void dibujarEncabezado(
        sf::RenderWindow& ventana
    );

    void dibujarCalendario(
        sf::RenderWindow& ventana
    );

    void dibujarVistaSemanal(
        sf::RenderWindow& ventana
    );

    void dibujarVistaMensual(
        sf::RenderWindow& ventana
    );

    void actualizarGeometria(
        const sf::RenderWindow& ventana
    );

    void moverMes(
        int desplazamiento
    );

    void dibujarPanelDetalles(
        sf::RenderWindow& ventana
    );

    void dibujarFormulario(
        sf::RenderWindow& ventana
    );

    void dibujarTexto(
        sf::RenderTarget& ventana,
        const std::string& texto,
        unsigned int tamano,
        float x,
        float y,
        const sf::Color& color =
            sf::Color(40, 40, 40)
    );

    void dibujarBoton(
        sf::RenderTarget& ventana,
        const sf::FloatRect& rectangulo,
        const std::string& texto,
        const sf::Color& color
    );

    //-------------------------------------------------
    // BUSQUEDAS
    //-------------------------------------------------

    const Tarea* buscarTarea(
        int idTarea
    ) const;

    const Subtarea* buscarSubtarea(
        int idSubtarea
    ) const;

    const Materia* buscarMateria(
        int idMateria
    ) const;

    std::vector<const Subtarea*> obtenerSubtareasTarea(
        int idTarea
    ) const;

    //-------------------------------------------------
    // FECHAS Y TEXTOS
    //-------------------------------------------------

    std::string obtenerNombreDia(
        int indice
    ) const;

    std::string obtenerNombreMes(
        int mes
    ) const;

    int obtenerIndiceDiaSemana(
        const std::string& fecha
    ) const;

    int buscarDiaSemana(
        const std::string& fecha
    ) const;

    std::string obtenerFechaCorta(
        const std::string& fecha
    ) const;

    std::string obtenerPrioridadTexto(
        PrioridadPlanner prioridad
    ) const;

    std::string obtenerEstadoTexto(
        EstadoSubtarea estado
    ) const;

    std::string obtenerTipoTareaTexto(
        TipoTarea tipo
    ) const;

    PrioridadPlanner siguientePrioridad(
        PrioridadPlanner prioridad
    ) const;

    EstadoSubtarea siguienteEstado(
        EstadoSubtarea estado
    ) const;

    //-------------------------------------------------
    // FORMULARIOS
    //-------------------------------------------------

    void abrirFormulario(
        Formulario tipo,
        int idSubtarea = -1
    );

    void cerrarFormulario();

public:

    //-------------------------------------------------
    // CONSTRUCTOR
    //-------------------------------------------------

    PlannerView();

    //-------------------------------------------------
    // FUENTE Y DATOS
    //-------------------------------------------------

    bool cargarFuente(
        const std::string& ruta
    );

    void setPlanner(
        const PlannerSemana& nuevaSemana
    );

    void setTareas(
        const std::vector<Tarea>& nuevasTareas
    );

    void setSubtareas(
        const std::vector<Subtarea>& nuevasSubtareas
    );

    void setMaterias(
        const std::vector<Materia>& nuevasMaterias
    );

    void setEstadosTareas(
        const vector<EstadoTareaPlanner>& nuevosEstados
    );

    EstadoTarea obtenerEstadoTarea(
        int idTarea
    )  const;
   

    //-------------------------------------------------
    // EVENTOS Y DIBUJADO
    //-------------------------------------------------

    void draw(
        sf::RenderWindow& ventana
    );

    void manejarEvento(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    //-------------------------------------------------
    // BOTON REGRESAR
    //-------------------------------------------------

    bool regresarPresionado(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    ) const;

    //-------------------------------------------------
    // ACCIONES
    //-------------------------------------------------

    int obtenerTareaSeleccionada() const;

    Accion obtenerAccion() const;

    void limpiarAccion();

    //-------------------------------------------------
    // FORMULARIOS
    //-------------------------------------------------

    bool mostrandoFormulario() const;

    void cerrarFormularios();
};

#endif
#ifndef TAREASVIEW_HPP
#define TAREASVIEW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Tarea.hpp"
#include "estadoTareaAlumno.hpp"
#include "calificaciones.hpp"
#include "materia.hpp"

#include "SFML/textbox.hpp"

class TareasView
{
public:

    struct AlumnoTarea
    {
        int id;
        std::string nombre;
        std::string identificador;
    };

private:

    sf::Font font;

    std::string rol;

    std::vector<Materia> materias;
    std::vector<Tarea> tareas;
    std::vector<std::string> profesores;
    std::vector<EstadoTareaAlumno> estadosTarea;
    std::vector<Calificacion> calificaciones;

    int idMateriaSeleccionada;

    float desplazamientoTareas;
    float velocidadDesplazamiento;

    sf::FloatRect botonRegresar;
    sf::FloatRect botonMateriaAnterior;
    sf::FloatRect botonMateriaSiguiente;
    sf::FloatRect botonAgregar;

    std::vector<sf::FloatRect> botonesEditar;
    std::vector<sf::FloatRect> botonesEliminar;
    std::vector<sf::FloatRect> botonesAlumnos;
    std::vector<sf::FloatRect> botonesCompletar;
    std::vector<sf::FloatRect> botonesNoCompletar;

    bool mostrandoEstados;
    int idTareaEstados;

    std::vector<EstadoTareaAlumno> alumnosEstados;
    std::vector<AlumnoTarea> alumnosTarea;
    std::vector<Calificacion> calificacionesEstados;

    int idAlumnoSeleccionado;

    //-------------------------------------------------
    // AGREGAR TAREA
    //-------------------------------------------------

    bool mostrandoAgregarTarea;

    TextBox txtTituloAgregar;
    TextBox txtFechaAgregar;
    TextBox txtDescripcionAgregar;
    TextBox txtTipoAgregar;
    TextBox txtParcialAgregar;

    int campoActivoAgregar;

    //-------------------------------------------------
    // EDITAR TAREA
    //-------------------------------------------------

    bool mostrandoEditarTarea;

    int idTareaEditando;

    TextBox txtTituloEditar;
    TextBox txtFechaEditar;
    TextBox txtDescripcionEditar;
    TextBox txtTipoEditar;
    TextBox txtParcialEditar;

    int campoActivoEditar;

    //-------------------------------------------------
    // ELIMINAR TAREA
    //-------------------------------------------------

    bool mostrandoEliminarTarea;

    int idTareaEliminar;

    //-------------------------------------------------
    // AGREGAR CALIFICACION
    //-------------------------------------------------

    bool mostrandoAgregarCalificacion;

    int idTareaCalificacion;
    int idAlumnoCalificacion;

    TextBox txtCalificacionAgregar;

    //-------------------------------------------------
    // EDITAR CALIFICACION
    //-------------------------------------------------

    bool mostrandoEditarCalificacion;

    int idTareaCalificacionEditar;
    int idAlumnoCalificacionEditar;

    TextBox txtCalificacionEditar;

    //-------------------------------------------------
    // ELIMINAR CALIFICACION
    //-------------------------------------------------

    bool mostrandoEliminarCalificacion;

    int idTareaCalificacionEliminar;
    int idAlumnoCalificacionEliminar;

    //-------------------------------------------------
    // METODOS PRIVADOS
    //-------------------------------------------------

    void manejarEventosGenerales(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosProfesor(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosAlumno(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosAgregarTarea(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosEditarTarea(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosEliminarTarea(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosEstados(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosAgregarCalificacion(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosEditarCalificacion(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    void manejarEventosEliminarCalificacion(
        const sf::Event& evento,
        const sf::RenderWindow& ventana
    );

    //-------------------------------------------------
    // DIBUJADO
    //-------------------------------------------------

    void dibujarEncabezado(
        sf::RenderWindow& ventana
    );

    void dibujarSelectorMateria(
        sf::RenderWindow& ventana
    );

    void dibujarTarea(
        sf::RenderWindow& ventana,
        const Tarea& tarea,
        float posicionY,
        int indice
    );

    void dibujarBoton(
        sf::RenderWindow& ventana,
        const sf::FloatRect& rectangulo,
        const std::string& texto,
        const sf::Color& color
    );

    void dibujarVentanaAgregarTarea(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaEditarTarea(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaEliminarTarea(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaEstados(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaAgregarCalificacion(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaEditarCalificacion(
        sf::RenderWindow& ventana
    );

    void dibujarVentanaEliminarCalificacion(
        sf::RenderWindow& ventana
    );

    //-------------------------------------------------
    // CONSULTAS
    //-------------------------------------------------

    std::string obtenerEstadoTexto(
        int idTarea
    ) const;

    std::string obtenerCalificacionTexto(
        int idTarea
    ) const;

    bool tieneCalificacion(
        int idAlumno,
        int idTarea
    ) const;

    double obtenerCalificacion(
        int idAlumno,
        int idTarea
    ) const;

    //-------------------------------------------------
    // LIMPIAR FORMULARIOS
    //-------------------------------------------------

    void limpiarFormularioAgregar();

    void limpiarFormularioEditar();

    void limpiarFormularioCalificacion();

public:

    //-------------------------------------------------
    // CONSTRUCTOR
    //-------------------------------------------------

    TareasView();

    //-------------------------------------------------
    // FUENTE Y DATOS
    //-------------------------------------------------

    bool cargarFuente(
        const std::string& ruta
    );

    void setRol(
        const std::string& nuevoRol
    );

    void setMaterias(
        const std::vector<Materia>& nuevasMaterias
    );

    void setMateriaSeleccionada(
        int idMateria
    );

    int obtenerMateriaSeleccionada() const;


    void setProfesores(
        const std::vector<std::string>& nuevosProfesores
    );


    void setTareas(
        const std::vector<Tarea>& nuevasTareas
    );

    void limpiarTareas();

    void recargar();

    void setEstadosTarea(
        const std::vector<EstadoTareaAlumno>& nuevosEstados
    );

    void limpiarEstados();

    void setCalificaciones(
        const std::vector<Calificacion>& nuevasCalificaciones
    );

    void limpiarCalificaciones();

    void setAlumnosTarea(
        const std::vector<AlumnoTarea>& nuevosAlumnos
    );

    void limpiarAlumnosTarea();

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
    // BOTONES
    //-------------------------------------------------

    bool regresarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool agregarPresionado() const;

    bool guardarAgregarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarAgregarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int editarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool guardarEditarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarEditarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int eliminarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool confirmarEliminarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarEliminarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int alumnosPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cerrarEstadosPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int completarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int noCompletarPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    //-------------------------------------------------
    // CALIFICACIONES
    //-------------------------------------------------

    int agregarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int editarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    int eliminarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool guardarAgregarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarAgregarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool guardarEditarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarEditarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool confirmarEliminarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    bool cancelarEliminarCalificacionPresionado(
        const sf::RenderWindow& ventana,
        const sf::Event& evento
    ) const;

    //-------------------------------------------------
    // GETTERS FORMULARIO AGREGAR
    //-------------------------------------------------

    std::string obtenerTituloAgregar() const;

    std::string obtenerFechaAgregar() const;

    std::string obtenerDescripcionAgregar() const;

    std::string obtenerTipoAgregar() const;

    std::string obtenerParcialAgregar() const;

    //-------------------------------------------------
    // GETTERS FORMULARIO EDITAR
    //-------------------------------------------------

    std::string obtenerTituloEditar() const;

    std::string obtenerFechaEditar() const;

    std::string obtenerDescripcionEditar() const;

    std::string obtenerTipoEditar() const;

    std::string obtenerParcialEditar() const;

    //-------------------------------------------------
    // GETTERS CALIFICACIONES
    //-------------------------------------------------

    std::string obtenerCalificacionAgregar() const;

    std::string obtenerCalificacionEditar() const;

    int obtenerTareaEditando() const;

    int obtenerTareaEliminar() const;

    int obtenerTareaCalificacion() const;

    int obtenerAlumnoCalificacion() const;

    int obtenerTareaCalificacionEditar() const;

    int obtenerAlumnoCalificacionEditar() const;

    int obtenerTareaCalificacionEliminar() const;

    int obtenerAlumnoCalificacionEliminar() const;

    //-------------------------------------------------
    // LIMPIAR
    //-------------------------------------------------

    void limpiarAgregar();

    void limpiarEditar();

    void limpiarCalificacion();

    //-------------------------------------------------
    // ESTADO DE FORMULARIOS
    //-------------------------------------------------

    bool mostrandoFormularioAgregar() const;

    bool mostrandoFormularioEditar() const;

    bool mostrandoFormularioEliminar() const;

    bool mostrandoVentanaEstados() const;

    bool mostrandoFormularioAgregarCalificacion() const;

    bool mostrandoFormularioEditarCalificacion() const;

    bool mostrandoFormularioEliminarCalificacion() const;

    void cerrarFormularios();
};

#endif

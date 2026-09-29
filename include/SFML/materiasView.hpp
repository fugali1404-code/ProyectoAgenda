#ifndef MATERIASVIEW_HPP
#define MATERIASVIEW_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "materia.hpp"
#include "SFML/textbox.hpp"


class MateriasView
{

public:

    //-------------------------------------------------
    // Alumno de una materia
    //-------------------------------------------------

    struct AlumnoMateria
    {
        int id;
        std::string nombre;
        std::string identificador;
    };

    //-------------------------------------------------
    // Calificacion de un alumno
    //-------------------------------------------------

    struct CalificacionAlumno
    {   
        int idAlumno;
        std::string nombre;
        std::string identificador;
        double calificacion;
        bool tieneCalificacion;
    };

    
    // Constructor
    MateriasView();


    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    bool cargarFuente(
        const std::string& ruta
    );


    //-------------------------------------------------
    // Rol
    //-------------------------------------------------

    void setRol(
        const std::string& rolUsuario
    );


    //-------------------------------------------------
    // Materias
    //-------------------------------------------------

    void setMaterias(
        const std::vector<Materia>& lista
    );

    void limpiarMaterias();


    //-------------------------------------------------
    // Profesores
    //-------------------------------------------------

    void setProfesores(
        const std::vector<std::string>& lista
    );


    //-------------------------------------------------
    // Cantidad de alumnos
    //-------------------------------------------------

    void setCantidadAlumnos(
        const std::vector<int>& cantidades
    );


    //-------------------------------------------------
    // Alumnos
    //-------------------------------------------------

    void setAlumnosMateria(
        const std::vector<AlumnoMateria>& alumnos
    );

    void limpiarAlumnosMateria();


    //-------------------------------------------------
    // Dibujar
    //-------------------------------------------------

    void draw(
        sf::RenderWindow& window
    );


    void manejarEvento(
        const sf::Event& event,
        const sf::RenderWindow& window
    );


    //-------------------------------------------------
    // Botón regresar
    //-------------------------------------------------

    bool botonRegresarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    ) const;


    //-------------------------------------------------
    // Botón agregar
    //-------------------------------------------------

    bool botonAgregarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Guardar nueva materia
    //-------------------------------------------------

    bool botonGuardarAgregarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    ) const;


    //-------------------------------------------------
    // Cancelar agregar
    //-------------------------------------------------

    bool botonCancelarAgregarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Botón editar
    //-------------------------------------------------

    bool botonEditarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Guardar edición
    //-------------------------------------------------

    bool botonGuardarEditarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    ) const;


    //-------------------------------------------------
    // Cancelar editar
    //-------------------------------------------------

    bool botonCancelarEditarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Botón eliminar
    //-------------------------------------------------

    bool botonEliminarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Confirmar eliminar
    //-------------------------------------------------

    bool botonConfirmarEliminarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Cancelar eliminar
    //-------------------------------------------------

    bool botonCancelarEliminarPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Botón ver alumnos
    //-------------------------------------------------

    bool botonAlumnosPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    bool botonInformacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool botonCerrarInformacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool estaMostrandoInformacion() const;

    //-------------------------------------------------
    // Botón inscribir alumno
    //-------------------------------------------------

    bool botonInscribirPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Botón desinscribir alumno
    //-------------------------------------------------

    bool botonDesinscribirPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Cerrar ventana alumnos
    //-------------------------------------------------

    bool botonCerrarAlumnosPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    //-------------------------------------------------
    // Obtener materia seleccionada
    //-------------------------------------------------

    int obtenerIdMateriaSeleccionada() const;


    //-------------------------------------------------
    // Obtener alumno seleccionado
    //
    // Se conserva para compatibilidad con código
    // anterior. Si hay varios seleccionados,
    // devuelve el primero.
    //-------------------------------------------------

    int obtenerIdAlumnoSeleccionado() const;


    //-------------------------------------------------
    // Alumnos seleccionados
    //-------------------------------------------------

    const std::vector<int>&
    obtenerAlumnosSeleccionados() const;

    void limpiarAlumnosSeleccionados();

    void alternarAlumnoSeleccionado(
        int idAlumno
    );


    //-------------------------------------------------
    // Nueva materia
    //-------------------------------------------------

    std::string obtenerNuevaMateria() const;

    void limpiarNuevaMateria();


    //-------------------------------------------------
    // Editar materia
    //-------------------------------------------------

    std::string obtenerNombreEditar() const;

    void limpiarEditarMateria();


    //-------------------------------------------------
    // Boletas de alumnos
    //-------------------------------------------------

    std::string obtenerBoletasAlumno() const;

    void limpiarBoletasAlumno();

    //-------------------------------------------------
    // Boletas de alumnos seleccionados
    //-------------------------------------------------

    std::string obtenerBoletasSeleccionadas() const;

    //-------------------------------------------------
    // Estado de ventanas
    //-------------------------------------------------

    bool estaMostrandoAgregar() const;

    bool estaMostrandoEditar() const;

    bool estaMostrandoEliminar() const;

    bool estaMostrandoAlumnos() const;

    //-------------------------------------------------
    // Calificaciones finales de alumnos
    //-------------------------------------------------

    bool botonCalificacionesAlumnosPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool botonCerrarCalificacionesAlumnosPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    bool estaMostrandoCalificacionesAlumnos() const;

    void setCalificacionesAlumnos(
        const std::vector<CalificacionAlumno>& lista
    );

    void limpiarCalificacionesAlumnos();

    const std::vector<CalificacionAlumno>& obtenerCalificacionesAlumnos() const;


    //-------------------------------------------------
    // Limpiar estado
    //-------------------------------------------------

    void cerrarFormularios();


    //-------------------------------------------------
    // Ponderacion de materia
    //-------------------------------------------------

    struct PonderacionMateria
    {
        int parcial;
        double tarea;
        double examen;
        double practica;
        double proyecto;
        double trabajo;
        double otro;
    };


    void setPonderaciones(
        const std::vector<PonderacionMateria>& lista
    );

    void limpiarPonderaciones();

    //-------------------------------------------------
    // Boton ponderaciones
    //-------------------------------------------------

    bool botonPonderacionesPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    //-------------------------------------------------
    // Guardar ponderacion
    //-------------------------------------------------

    
    bool botonCargarPonderacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );
    
    bool botonGuardarPonderacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    //-------------------------------------------------
    // Eliminar ponderacion
    //-------------------------------------------------

    bool botonEliminarPonderacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    //-------------------------------------------------
    // Cancelar ponderaciones
    //-------------------------------------------------

    bool botonCancelarPonderacionPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );

    //-------------------------------------------------
    // Obtener parcial seleccionado
    //-------------------------------------------------

    int obtenerParcialPonderacion() const;

    //-------------------------------------------------
    // Obtener ponderacion
    //-------------------------------------------------

    bool obtenerPonderacion(
        PonderacionMateria& ponderacion
    ) const;

    //-------------------------------------------------
    // Estado ventana ponderaciones
    //-------------------------------------------------

    bool estaMostrandoPonderaciones() const;

    //-------------------------------------------------
    // Calificacion final
    //-------------------------------------------------

    bool botonCalificacionFinalPresionado(
        const sf::RenderWindow& window,
        const sf::Event& event
    );


    void setCalificacionFinal(double calificacion);

    void limpiarCalificacionFinal();

    



private:

    //-------------------------------------------------
    // Fuente
    //-------------------------------------------------

    sf::Font font;


    //-------------------------------------------------
    // Datos del usuario
    //-------------------------------------------------

    std::string rol;


    //-------------------------------------------------
    // Materias
    //-------------------------------------------------

    std::vector<Materia> Materias;


    //-------------------------------------------------
    // Profesores
    //-------------------------------------------------

    std::vector<std::string> profesores;


    //-------------------------------------------------
    // Cantidad de alumnos por materia
    //-------------------------------------------------

    std::vector<int> cantidadAlumnosMaterias;


    //-------------------------------------------------
    // Alumnos de la materia seleccionada
    //-------------------------------------------------

    std::vector<AlumnoMateria> alumnosMateria;

    //-------------------------------------------------
    // Calificaciones finales de los alumnos
    //-------------------------------------------------

    std::vector<CalificacionAlumno> calificacionesAlumnos;

    //-------------------------------------------------
    // Scroll materias
    //-------------------------------------------------

    float desplazamientoMaterias;


    //-------------------------------------------------
    // Scroll de alumnos
    //-------------------------------------------------

    float desplazamientoAlumnos;


    //-------------------------------------------------
    // TextBox
    //-------------------------------------------------

    TextBox txtNuevaMateria;

    TextBox txtEditarMateria;

    TextBox txtBoletasAlumno;

    TextBox txtParcialPonderacion;
    TextBox txtTarea;
    TextBox txtExamen;
    TextBox txtPractica;
    TextBox txtProyecto;
    TextBox txtTrabajo;
    TextBox txtOtro;


    //-------------------------------------------------
    // Materia seleccionada
    //-------------------------------------------------

    int idMateriaSeleccionada;


    //-------------------------------------------------
    // Alumno seleccionado
    //
    // Se conserva para compatibilidad con el código
    // anterior. La selección real ahora se maneja
    // mediante alumnosSeleccionados.
    //-------------------------------------------------

    int idAlumnoSeleccionado;

    //-------------------------------------------------
    // Alumnos seleccionados para desinscribir
    //-------------------------------------------------

    std::vector<int> alumnosSeleccionados;


    //-------------------------------------------------
    // Estado
    //-------------------------------------------------

    bool mostrandoAgregar;

    bool mostrandoEditar;

    bool mostrandoEliminar;

    bool mostrandoAlumnos;
    
    //-------------------------------------------------
    // Ponderaciones
    //-------------------------------------------------

    std::vector<PonderacionMateria> ponderaciones;

    int parcialSeleccionado;

    bool mostrandoPonderaciones;

    //-------------------------------------------------
    // Calificacion Final
    //-------------------------------------------------

    bool mostrandoInformacion;
    double calificacionFinal;
    bool tieneCalificacionFinal;

    //-------------------------------------------------
    // Ventana de calificaciones del profesor
    //-------------------------------------------------

    bool mostrandoCalificacionesAlumnos;

    

};


#endif
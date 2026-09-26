#ifndef MATERIASCONTROLLER_HPP
#define MATERIASCONTROLLER_HPP

#include <string>
#include <vector>

#include "materia.hpp"
#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

class MateriasController
{

    
private:

    //-------------------------------------------------
    // Red
    //-------------------------------------------------

    NetworkManager& network;

    //-------------------------------------------------
    // Sesion
    //-------------------------------------------------

    SessionClient& session;

    //-------------------------------------------------
    // Profesores
    //-------------------------------------------------

    std::vector<std::string> profesores;

    //-------------------------------------------------
    // Convertir respuesta
    //-------------------------------------------------

    std::vector<Materia> convertirMaterias(
        const std::string& respuesta
    ) const;

public:

    //-------------------------------------------------
    // Constructor
    //-------------------------------------------------

    MateriasController(
        NetworkManager& networkManager,
        SessionClient& sessionClient
    );

    //-------------------------------------------------
    // Obtener materias
    //-------------------------------------------------

    std::vector<Materia> obtenerMaterias();

    //-------------------------------------------------
    // Obtener profesores
    //-------------------------------------------------

    const std::vector<std::string>&
    obtenerProfesores() const;

    //-------------------------------------------------
    // Agregar materia
    //-------------------------------------------------

    bool agregarMateria(
        const std::string& nombre
    );

    //-------------------------------------------------
    // Actualizar materia
    //-------------------------------------------------

    bool actualizarMateria(
        int idMateria,
        const std::string& nombre
    );

    //-------------------------------------------------
    // Eliminar materia
    //-------------------------------------------------

    bool eliminarMateria(
        int idMateria
    );

    //-------------------------------------------------
    // Obtener alumnos de una materia
    //-------------------------------------------------

    
    //-------------------------------------------------
    // Alumno inscrito
    //-------------------------------------------------

    struct AlumnoMateria
    {
        int id;
        std::string nombre;
        std::string identificador;
    };
    
    
    std::vector<AlumnoMateria> obtenerAlumnosMateria(
        int idMateria
    );


    //-------------------------------------------------
    // Ponderacion
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

    //-------------------------------------------------
    // Ponderaciones
    //-------------------------------------------------

    std::vector<PonderacionMateria> obtenerPonderaciones(
        int idMateria
    );

    bool configurarPonderacion(
        int idMateria,
        int parcial,
        double tarea,
        double examen,
        double practica,
        double proyecto,
        double trabajo,
        double otro
    );

    bool eliminarPonderacion(
        int idMateria,
        int parcial
    );

    //-------------------------------------------------
    // Calificacion final
    //-------------------------------------------------

    bool obtenerCalificacionFinal(
        int idAlumno,
        int idMateria,
        double& calificacionFinal
    );


    //-------------------------------------------------
    // Inscribir alumno
    //-------------------------------------------------

    bool inscribirAlumno(
        int idMateria,
        const std::string& boletas
    );


    //-------------------------------------------------
    // Desinscribir alumno
    //-------------------------------------------------

    bool desinscribirAlumno(
        int idMateria,
        const std::string& boletas
    );

    
};

#endif
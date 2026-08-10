#ifndef INSCRIPCIONES_HPP
#define INSCRIPCIONES_HPP

#include <string>
#include <vector>

struct Inscripcion
{
    int idMateria;
    int idAlumno;
};

class Inscripciones
{
public:

    //========================================
    // Cargar inscripciones
    //========================================

    static bool cargar(
        std::vector<Inscripcion>& inscripciones,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Guardar inscripciones
    //========================================

    static bool guardar(
        const std::vector<Inscripcion>& inscripciones,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Inscribir alumno
    //========================================

    static bool inscribirAlumno(
        int idMateria,
        int idAlumno,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Desinscribir alumno
    //========================================

    static bool desinscribirAlumno(
        int idMateria,
        int idAlumno,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Obtener alumnos de una materia
    //========================================

    static std::vector<int> obtenerAlumnosMateria(
        int idMateria,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Obtener materias de un alumno
    //========================================

    static std::vector<int> obtenerMateriasAlumno(
        int idAlumno,
        const std::string& archivo = "inscripciones.txt"
    );

    //========================================
    // Verificar inscripción
    //========================================

    static bool estaInscrito(
        int idMateria,
        int idAlumno,
        const std::string& archivo = "inscripciones.txt"
    );
};

#endif
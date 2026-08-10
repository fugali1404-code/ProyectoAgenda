#include "inscripciones.hpp"

#include <fstream>
#include <sstream>


//==================================================
// Cargar inscripciones
//==================================================

bool Inscripciones::cargar(
    std::vector<Inscripcion>& inscripciones,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
    {
        // Si todavía no existe el archivo,
        // no hay inscripciones.
        inscripciones.clear();

        return true;
    }

    inscripciones.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string campo;

        Inscripcion inscripcion;

        //==========================================
        // ID Materia
        //==========================================

        if(!std::getline(ss, campo, '|'))
        {
            continue;
        }

        try
        {
            inscripcion.idMateria = std::stoi(campo);
        }
        catch(...)
        {
            continue;
        }

        //==========================================
        // ID Alumno
        //==========================================

        if(!std::getline(ss, campo, '|'))
        {
            continue;
        }

        try
        {
            inscripcion.idAlumno = std::stoi(campo);
        }
        catch(...)
        {
            continue;
        }

        inscripciones.push_back(inscripcion);
    }

    in.close();

    return true;
}


//==================================================
// Guardar inscripciones
//==================================================

bool Inscripciones::guardar(
    const std::vector<Inscripcion>& inscripciones,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    for(const auto& inscripcion : inscripciones)
    {
        out
            << inscripcion.idMateria
            << "|"
            << inscripcion.idAlumno
            << "\n";
    }

    out.close();

    return true;
}


//==================================================
// Inscribir alumno
//==================================================

bool Inscripciones::inscribirAlumno(
    int idMateria,
    int idAlumno,
    const std::string& archivo)
{
    // IDs inválidos
    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    std::vector<Inscripcion> inscripciones;

    if(!cargar(inscripciones, archivo))
    {
        return false;
    }

    //==============================================
    // Evitar inscripción duplicada
    //==============================================

    for(const auto& inscripcion : inscripciones)
    {
        if(inscripcion.idMateria == idMateria &&
           inscripcion.idAlumno == idAlumno)
        {
            return false;
        }
    }

    //==============================================
    // Crear inscripción
    //==============================================

    Inscripcion nueva;

    nueva.idMateria = idMateria;
    nueva.idAlumno = idAlumno;

    inscripciones.push_back(nueva);

    //==============================================
    // Guardar
    //==============================================

    return guardar(
        inscripciones,
        archivo
    );
}


//==================================================
// Desinscribir alumno
//==================================================

bool Inscripciones::desinscribirAlumno(
    int idMateria,
    int idAlumno,
    const std::string& archivo)
{
    std::vector<Inscripcion> inscripciones;

    if(!cargar(inscripciones, archivo))
    {
        return false;
    }

    for(auto it = inscripciones.begin();
        it != inscripciones.end();
        ++it)
    {
        if(it->idMateria == idMateria &&
           it->idAlumno == idAlumno)
        {
            inscripciones.erase(it);

            return guardar(
                inscripciones,
                archivo
            );
        }
    }

    // La inscripción no existe
    return false;
}


//==================================================
// Obtener alumnos de una materia
//==================================================

std::vector<int>
Inscripciones::obtenerAlumnosMateria(
    int idMateria,
    const std::string& archivo)
{
    std::vector<Inscripcion> inscripciones;

    std::vector<int> alumnos;

    if(!cargar(inscripciones, archivo))
    {
        return alumnos;
    }

    for(const auto& inscripcion : inscripciones)
    {
        if(inscripcion.idMateria == idMateria)
        {
            alumnos.push_back(
                inscripcion.idAlumno
            );
        }
    }

    return alumnos;
}


//==================================================
// Obtener materias de un alumno
//==================================================

std::vector<int>
Inscripciones::obtenerMateriasAlumno(
    int idAlumno,
    const std::string& archivo)
{
    std::vector<Inscripcion> inscripciones;

    std::vector<int> materias;

    if(!cargar(inscripciones, archivo))
    {
        return materias;
    }

    for(const auto& inscripcion : inscripciones)
    {
        if(inscripcion.idAlumno == idAlumno)
        {
            materias.push_back(
                inscripcion.idMateria
            );
        }
    }

    return materias;
}


//==================================================
// Verificar inscripción
//==================================================

bool Inscripciones::estaInscrito(
    int idMateria,
    int idAlumno,
    const std::string& archivo)
{
    std::vector<Inscripcion> inscripciones;

    if(!cargar(inscripciones, archivo))
    {
        return false;
    }

    for(const auto& inscripcion : inscripciones)
    {
        if(inscripcion.idMateria == idMateria &&
           inscripcion.idAlumno == idAlumno)
        {
            return true;
        }
    }

    return false;
}
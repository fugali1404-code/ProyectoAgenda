#include "inscripciones.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>


//======================================================
// CARGAR INSCRIPCIONES
//======================================================

bool Inscripciones::cargar(
    std::vector<std::pair<int, std::vector<int>>>& inscripciones,
    const std::string& archivo
)
{
    std::ifstream in(archivo);

    if(!in.is_open())
    {
        // Si todavía no existe el archivo,
        // no es un error grave.
        inscripciones.clear();
        return true;
    }

    inscripciones.clear();

    std::string linea;

    //==================================================
    // Ignorar encabezado
    //==================================================

    std::getline(in, linea);

    //==================================================
    // Leer registros
    //==================================================

    while(std::getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string campoMateria;
        std::string campoAlumnos;

        //================================================
        // ID DE MATERIA
        //================================================

        if(!std::getline(ss, campoMateria, '|'))
        {
            continue;
        }

        //================================================
        // ALUMNOS
        //================================================

        if(!std::getline(ss, campoAlumnos))
        {
            continue;
        }

        int idMateria;

        try
        {
            idMateria = std::stoi(campoMateria);
        }
        catch(...)
        {
            continue;
        }

        std::vector<int> alumnos;

        //================================================
        // Separar alumnos por coma
        //================================================

        std::stringstream alumnosStream(campoAlumnos);

        std::string campoAlumno;

        while(std::getline(
            alumnosStream,
            campoAlumno,
            ','
        ))
        {
            if(campoAlumno.empty())
            {
                continue;
            }

            try
            {
                int idAlumno = std::stoi(campoAlumno);

                alumnos.push_back(idAlumno);
            }
            catch(...)
            {
                continue;
            }
        }

        //================================================
        // Guardar materia y alumnos
        //================================================

        inscripciones.push_back(
            {
                idMateria,
                alumnos
            }
        );
    }

    in.close();

    return true;
}


//======================================================
// GUARDAR INSCRIPCIONES
//======================================================

bool Inscripciones::guardar(
    const std::vector<std::pair<int, std::vector<int>>>& inscripciones,
    const std::string& archivo
)
{
    std::ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    //==================================================
    // Encabezado
    //==================================================

    out << "idMateria|idAlumnos\n";

    //==================================================
    // Guardar registros
    //==================================================

    for(const auto& registro : inscripciones)
    {
        int idMateria = registro.first;

        const std::vector<int>& alumnos =
            registro.second;

        out << idMateria << "|";

        for(std::size_t i = 0;
            i < alumnos.size();
            ++i)
        {
            out << alumnos[i];

            if(i + 1 < alumnos.size())
            {
                out << ",";
            }
        }

        out << "\n";
    }

    out.close();

    return true;
}


//======================================================
// INSCRIBIR ALUMNO
//======================================================

bool Inscripciones::inscribirAlumno(
    int idMateria,
    int idAlumno,
    const std::string& archivo
)
{
    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar datos
    //==================================================

    std::vector<
        std::pair<int, std::vector<int>>
    > inscripciones;

    if(!cargar(
        inscripciones,
        archivo
    ))
    {
        return false;
    }

    //==================================================
    // Buscar materia
    //==================================================

    for(auto& registro : inscripciones)
    {
        if(registro.first == idMateria)
        {
            std::vector<int>& alumnos =
                registro.second;

            //==========================================
            // Verificar duplicado
            //==========================================

            if(std::find(
                alumnos.begin(),
                alumnos.end(),
                idAlumno
            ) != alumnos.end())
            {
                return false;
            }

            //==========================================
            // Agregar alumno
            //==========================================

            alumnos.push_back(idAlumno);

            return guardar(
                inscripciones,
                archivo
            );
        }
    }

    //==================================================
    // La materia todavía no tiene registros
    //==================================================

    inscripciones.push_back(
        {
            idMateria,
            { idAlumno }
        }
    );

    return guardar(
        inscripciones,
        archivo
    );
}


//======================================================
// DESINSCRIBIR ALUMNO
//======================================================

bool Inscripciones::desinscribirAlumno(
    int idMateria,
    int idAlumno,
    const std::string& archivo
)
{
    if(idMateria <= 0 || idAlumno <= 0)
    {
        return false;
    }

    //==================================================
    // Cargar datos
    //==================================================

    std::vector<
        std::pair<int, std::vector<int>>
    > inscripciones;

    if(!cargar(
        inscripciones,
        archivo
    ))
    {
        return false;
    }

    //==================================================
    // Buscar materia
    //==================================================

    for(auto it = inscripciones.begin();
        it != inscripciones.end();
        ++it)
    {
        if(it->first == idMateria)
        {
            std::vector<int>& alumnos =
                it->second;

            //==========================================
            // Buscar alumno
            //==========================================

            auto alumnoIt = std::find(
                alumnos.begin(),
                alumnos.end(),
                idAlumno
            );

            if(alumnoIt == alumnos.end())
            {
                return false;
            }

            //==========================================
            // Eliminar alumno
            //==========================================

            alumnos.erase(alumnoIt);

            //==========================================
            // Si ya no hay alumnos,
            // eliminar registro de materia
            //==========================================

            if(alumnos.empty())
            {
                inscripciones.erase(it);
            }

            return guardar(
                inscripciones,
                archivo
            );
        }
    }

    return false;
}


//======================================================
// VERIFICAR INSCRIPCIÓN
//======================================================

bool Inscripciones::estaInscrito(
    int idMateria,
    int idAlumno,
    const std::string& archivo
)
{
    std::vector<
        std::pair<int, std::vector<int>>
    > inscripciones;

    if(!cargar(
        inscripciones,
        archivo
    ))
    {
        return false;
    }

    for(const auto& registro : inscripciones)
    {
        if(registro.first == idMateria)
        {
            const std::vector<int>& alumnos =
                registro.second;

            return std::find(
                alumnos.begin(),
                alumnos.end(),
                idAlumno
            ) != alumnos.end();
        }
    }

    return false;
}


//======================================================
// OBTENER ALUMNOS DE UNA MATERIA
//======================================================

std::vector<int>
Inscripciones::obtenerAlumnosMateria(
    int idMateria,
    const std::string& archivo
)
{
    std::vector<int> resultado;

    std::vector<
        std::pair<int, std::vector<int>>
    > inscripciones;

    if(!cargar(
        inscripciones,
        archivo
    ))
    {
        return resultado;
    }

    for(const auto& registro : inscripciones)
    {
        if(registro.first == idMateria)
        {
            resultado = registro.second;
            break;
        }
    }

    return resultado;
}


//======================================================
// OBTENER MATERIAS DE UN ALUMNO
//======================================================

std::vector<int>
Inscripciones::obtenerMateriasAlumno(
    int idAlumno,
    const std::string& archivo
)
{
    std::vector<int> resultado;

    std::vector<
        std::pair<int, std::vector<int>>
    > inscripciones;

    if(!cargar(
        inscripciones,
        archivo
    ))
    {
        return resultado;
    }

    //==================================================
    // Recorrer materias
    //==================================================

    for(const auto& registro : inscripciones)
    {
        int idMateria = registro.first;

        const std::vector<int>& alumnos =
            registro.second;

        //================================================
        // Buscar alumno
        //================================================

        if(std::find(
            alumnos.begin(),
            alumnos.end(),
            idAlumno
        ) != alumnos.end())
        {
            resultado.push_back(
                idMateria
            );
        }
    }

    return resultado;
}
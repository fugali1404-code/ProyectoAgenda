#include "persistencia.hpp"
#include "alumno.hpp"
#include "profesor.hpp"

#include <fstream>
#include <sstream>
#include <vector>

///////////////////////////////////////////////////////////
// Conversión de Prioridad
///////////////////////////////////////////////////////////

std::string Persistencia::prioridadAString(
    Prioridad prioridad)
{
    switch (prioridad)
    {
        case Prioridad::BAJA:
            return "BAJA";

        case Prioridad::MEDIA:
            return "MEDIA";

        case Prioridad::ALTA:
            return "ALTA";
    }

    return "MEDIA";
}

Prioridad Persistencia::stringAPrioridad(
    const std::string& texto)
{
    if(texto == "BAJA")
        return Prioridad::BAJA;

    if(texto == "ALTA")
        return Prioridad::ALTA;

    return Prioridad::MEDIA;
}

////////////////////////////////////////////////////////////
// Guardar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::guardarTareas(
    const std::vector<Tarea>& tareas,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return false;

    for(const auto& tarea : tareas)
    {
        out
        << tarea.getId() << "|"
        << tarea.getMateriaId()<< "|"
        << tarea.getTitulo() << "|"
        << tarea.getDescripcion() << "|"
        << tarea.getFechaEntrega() << "|"
        << prioridadAString(
                tarea.getPrioridad())
        << "|"
        << tarea.getCompletada()
        << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::cargarTareas(
    std::vector<Tarea>& tareas,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    tareas.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
            continue;

        std::stringstream ss(linea);

        std::string campo;

        Tarea tarea;

        // id
        std::getline(ss, campo, '|');
        tarea.setId(std::stoi(campo));

        // materiaId
        std::getline(ss, campo, '|');
        tarea.setMateriaId(std::stoi(campo));


        // titulo
        std::getline(ss, campo, '|');
        tarea.setTitulo(campo);

        // descripcion
        std::getline(ss, campo, '|');
        tarea.setDescripcion(campo);

        // fecha
        std::getline(ss, campo, '|');
        tarea.setFechaEntrega(campo);

        // prioridad
        std::getline(ss, campo, '|');
        tarea.setPrioridad(
            stringAPrioridad(campo));

        // completada
        std::getline(ss, campo, '|');

        tarea.setCompletada(
            campo == "1"
        );

        tareas.push_back(tarea);
    }

    in.close();

    return true;
}

///////////////////////////////////////////////////////////
// Guardar Materias
///////////////////////////////////////////////////////////

bool Persistencia::guardarMaterias(
    int profesorId,
    const std::vector<Materia>& materias,
    const std::string& archivo)
{
    //--------------------------------------------------
    // Leer las materias de otros profesores
    //--------------------------------------------------

    std::vector<std::string> lineas;

    {
        std::ifstream in(archivo);

        if(in.is_open())
        {
            std::string linea;

            while(std::getline(in, linea))
            {
                if(linea.empty())
                {
                    continue;
                }

                std::stringstream ss(linea);

                std::string campo;

                // Primer campo = idMateria
                if(!std::getline(ss, campo, ';'))
                {
                    continue;
                }

                // Segundo campo = nombre
                if(!std::getline(ss, campo, ';'))
                {
                    continue;
                }

                // Tercer campo = profesorId
                if(!std::getline(ss, campo, ';'))
                {
                    continue;
                }

                int profesorIdArchivo = std::stoi(campo);

                // Conservar únicamente materias
                // de otros profesores
                if(profesorIdArchivo != profesorId)
                {
                    lineas.push_back(linea);
                }
            }

            in.close();
        }
    }

    //--------------------------------------------------
    // Agregar las materias del profesor actual
    //--------------------------------------------------

    for(const auto& materia : materias)
    {
        std::stringstream ss;

        ss
            << materia.getId()
            << ";"
            << materia.getNombre()
            << ";"
            << materia.getProfesorId();

        lineas.push_back(
            ss.str()
        );
    }

    //--------------------------------------------------
    // Reescribir el archivo completo
    //--------------------------------------------------

    std::ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    for(const auto& linea : lineas)
    {
        out << linea << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Materias
///////////////////////////////////////////////////////////

bool Persistencia::cargarMaterias(
    std::vector<Materia>& materias,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
    {
        return false;
    }

    materias.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string campo;

        int idMateria;
        std::string nombre;
        int profesorId;

        // ID de materia
        if(!std::getline(ss, campo, ';'))
        {
            continue;
        }

        idMateria = std::stoi(campo);

        // Nombre
        if(!std::getline(ss, nombre, ';'))
        {
            continue;
        }

        // ID del profesor
        if(!std::getline(ss, campo, ';'))
        {
            continue;
        }

        profesorId = std::stoi(campo);

        Materia materia;

        materia.setId(idMateria);
        materia.setNombre(nombre);
        materia.setProfesorId(profesorId);

        materias.push_back(materia);
    }

    in.close();

    return true;
}

////////////////////////////////////////////////////////////
// Generar ID de Materia
///////////////////////////////////////////////////////////

int Persistencia::generarIdMateria(
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
    {
        return 1;
    }

    int mayorId = 0;

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string campo;

        // Primer campo = idMateria
        if(!std::getline(ss, campo, ';'))
        {
            continue;
        }

        int id = std::stoi(campo);

        if(id > mayorId)
        {
            mayorId = id;
        }
    }

    in.close();

    return mayorId + 1;
}


///////////////////////////////////////////////////////////
// Guardar Usuarios
///////////////////////////////////////////////////////////

bool Persistencia::guardarUsuarios(
    const std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return false;

    for(const auto usuario : usuarios)
    {
        if(usuario == nullptr)
            continue;

        out
            << usuario->getRol() << "|"
            << usuario->getId() << "|"
            << usuario->getNombre() << "|"
            << usuario->getCorreo() << "|"
            << usuario->getPassword() << "|"
            << usuario->getIdentificador()
            << "\n";
    }

    out.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Usuarios
///////////////////////////////////////////////////////////

bool Persistencia::cargarUsuarios(
    std::vector<Usuario*>& usuarios,
    const std::string& archivo)
{
    std::ifstream in(archivo);

    if(!in.is_open())
        return false;

    // Liberar memoria anterior
    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    usuarios.clear();

    std::string linea;

    while(std::getline(in, linea))
    {
        if(linea.empty())
            continue;

        std::stringstream ss(linea);

        std::string rol;
        std::string campo;

        std::getline(ss, rol, '|');

        // id
        std::getline(ss, campo, '|');
        int id = std::stoi(campo);

        // nombre
        std::string nombre;
        std::getline(ss, nombre, '|');

        // correo
        std::string correo;
        std::getline(ss, correo, '|');

        // password
        std::string password;
        std::getline(ss, password, '|');

        // identificador
        std::string identificador;
        std::getline(ss, identificador);

        Usuario* usuario = nullptr;

        if(rol == "Alumno")
        {
            usuario = new Alumno(
            id,
            nombre,
            correo,
            password,
            identificador);
        }
        
        else if(rol == "Profesor")
        {
            usuario = new Profesor(
            id,
            nombre,
            correo,
            password,
            identificador);
        }

        else if(rol == "Administrador")
        {
            usuario = new Administrador(
            id,
            nombre,
            correo,
            password,
            identificador);
        }

        if(usuario != nullptr)
        {
            usuarios.push_back(usuario);
        }
    }

    in.close();

    return true;
}


////////////////////////////////////////////////////////////
// Autenticar Usuario
///////////////////////////////////////////////////////////

Usuario* Persistencia::autenticarUsuario(
    const std::string& usuario,
    const std::string& password,
    const std::string& archivo
)
{
    std::vector<Usuario*> usuarios;

    if(!cargarUsuarios(
        usuarios,
        archivo))
    {
        return nullptr;
    }

    Usuario* encontrado = nullptr;

    for(auto u : usuarios)
    {
        if(
            u->getCorreo() == usuario &&
            u->getPassword() == password
        )
        {
            if(u->getRol() == "Administrador")
            {
                encontrado = new Administrador(
                u->getId(),
                u->getNombre(),
                u->getCorreo(),
                u->getPassword(),
                u->getIdentificador()

                );
            }
            else if(u->getRol() == "Profesor")
            {
                encontrado = new Profesor(
                u->getId(),
                u->getNombre(),
                u->getCorreo(),
                u->getPassword(),
                u->getIdentificador()
                );
            }
            
            else if(u->getRol() == "Alumno")
            {
                encontrado = new Alumno(
                u->getId(),
                u->getNombre(),
                u->getCorreo(),
                u->getPassword(),
                u->getIdentificador()
                );
            }

            break;
        }
    }

    for(auto u : usuarios)
    {
        delete u;
    }

    return encontrado;
}

///////////////////////////////////////////////////////////
// Generar ID Usuario
///////////////////////////////////////////////////////////

int Persistencia::generarIdUsuario(
    const std::string& archivo
)
{
    std::vector<Usuario*> usuarios;

    if(!cargarUsuarios(
        usuarios,
        archivo))
    {
        return 1;
    }

    int mayorId = 0;

    for(auto usuario : usuarios)
    {
        if(usuario->getId() > mayorId)
        {
            mayorId = usuario->getId();
        }
    }

    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    return mayorId + 1;
}

///////////////////////////////////////////////////////////
// Agregar Usuario
///////////////////////////////////////////////////////////

bool Persistencia::agregarUsuario(
    const Usuario& usuario,
    const std::string& archivo
)
{
    std::vector<Usuario*> usuarios;

    cargarUsuarios(
        usuarios,
        archivo
    );

    Usuario* nuevo = nullptr;

    if(usuario.getRol() == "Alumno")
    {
        nuevo = new Alumno(
            usuario.getId(),
            usuario.getNombre(),
            usuario.getCorreo(),
            usuario.getPassword(),
            usuario.getIdentificador()
        );
    }
    else if(usuario.getRol() == "Profesor")
    {
        nuevo = new Profesor(
            usuario.getId(),
            usuario.getNombre(),
            usuario.getCorreo(),
            usuario.getPassword(),
            usuario.getIdentificador()
        );
    }
    else if(usuario.getRol() == "Administrador")
    {
        nuevo = new Administrador(
            usuario.getId(),
            usuario.getNombre(),
            usuario.getCorreo(),
            usuario.getPassword(),
            usuario.getIdentificador()
        );
    }

    if(nuevo == nullptr)
    {
        return false;
    }

    usuarios.push_back(nuevo);

    bool ok =
        guardarUsuarios(
            usuarios,
            archivo
        );

    for(auto u : usuarios)
    {
        delete u;
    }

    return ok;
}

///////////////////////////////////////////////////////////
// Actualizar Usuario
///////////////////////////////////////////////////////////

bool Persistencia::actualizarUsuario(
    const Usuario& usuario,
    const std::string& archivo
)
{
    std::vector<Usuario*> usuarios;

    if(!cargarUsuarios(
        usuarios,
        archivo))
    {
        return false;
    }

    bool encontrado = false;

    for(auto u : usuarios)
    {
        if(u->getId() == usuario.getId())
        {
            u->setNombre(
                usuario.getNombre()
            );

            u->setCorreo(
                usuario.getCorreo()
            );

            encontrado = true;

            break;
        }
    }

    bool ok = false;

    if(encontrado)
    {
        ok =
            guardarUsuarios(
                usuarios,
                archivo
            );
    }

    for(auto u : usuarios)
    {
        delete u;
    }

    return ok;
}

///////////////////////////////////////////////////////////
// Eliminar Usuario
///////////////////////////////////////////////////////////

bool Persistencia::eliminarUsuario(
    int idUsuario,
    const std::string& archivo
)
{
    std::vector<Usuario*> usuarios;

    if(!cargarUsuarios(
        usuarios,
        archivo))
    {
        return false;
    }

    for(auto it = usuarios.begin();
        it != usuarios.end();
        ++it)
    {
        if((*it)->getId() == idUsuario)
        {
            delete *it;

            usuarios.erase(it);

            break;
        }
    }

    bool ok =
        guardarUsuarios(
            usuarios,
            archivo
        );

    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    return ok;
}

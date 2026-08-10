#include "sessionmaneger.hpp"
#include "administrador.hpp"
#include "alumno.hpp"
#include "profesor.hpp"
#include "persistencia.hpp"


SessionManager::SessionManager()
{
    
}

bool SessionManager::login(
    const std::string& correo,
    const std::string& password
)
{
    Usuario* usuario = Persistencia::autenticarUsuario(
            correo,
            password,
            "usuarios.txt"
        );

    if(usuario == nullptr)
    {
        datos.limpiar();
        return false;
    }

    datos.setUsuarioId(
        usuario->getId()
    );

    datos.setUsuario(
        usuario->getCorreo()
    );

    datos.setNombreCompleto(
        usuario->getNombre()
    );

    datos.setRol(
        usuario->getRol()
    );

    //---------------------------------
    // Identificador
    //---------------------------------

    datos.setIdentificador(
        usuario->getIdentificador()
    );

    //---------------------------------
    // Materias
    //---------------------------------

    std::vector<Materia> todasLasMaterias;

    Persistencia::cargarMaterias(todasLasMaterias,"materias.txt");

    std::vector<Materia> materiasProfesor;

    for(const auto& materia : todasLasMaterias)
    {
        if(materia.getProfesorId() == usuario->getId())
        {
            materiasProfesor.push_back(materia);
        }
    
    }

datos.setMaterias(materiasProfesor);

    //---------------------------------
    // Sesión autenticada
    //---------------------------------

    datos.setAutenticado(true);

    delete usuario;

    return true;
}

//-------------------------------------------------------------

void SessionManager::logout()
{
    datos.limpiar();
}

bool SessionManager::estaAutenticado() const
{
    return datos.estaAutenticado();
}

std::string SessionManager::obtenerUsuario() const
{
    return datos.obtenerUsuario();
}

////////////////////////////////////////////////////////////////////
//Agregar Materia
////////////////////////////////////////////////////////////////////

bool SessionManager::agregarMateria(
    const std::string& nombre
)
{
    if(!datos.estaAutenticado())
    {
        return false;
    }

    Materia nueva;

    nueva.setId(
        Persistencia::generarIdMateria(
            "materias.txt"
        )
    );

    nueva.setNombre(nombre);

    // El profesor autenticado es el propietario
    // de la materia.
    nueva.setProfesorId(
        datos.obtenerUsuarioId()
    );

    if(!datos.agregarMateria(nueva))
    {
        return false;
    }

    return Persistencia::guardarMaterias(
        datos.obtenerUsuarioId(),
        datos.obtenerVectorMaterias(),
        "materias.txt"
    );
}

//////////////////////////////////////////////////////////////
// Obtener Materias
//////////////////////////////////////////////////////////////

std::string SessionManager::obtenerMaterias() const
{
    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    std::string lista = datos.obtenerMaterias();

    if(lista == "SIN_MATERIAS")
    {
        return "MATERIAS|";
    }

    return "MATERIAS|" + lista;
}

/////////////////////////////////////////////////////////////
/// Eliminar Materia
/////////////////////////////////////////////////////////////

bool SessionManager::eliminarMateria(
    int id
)
{
    // Debe estar autenticado
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Solo profesores pueden eliminar materias
    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    std::vector<Materia>& materias =
        datos.obtenerVectorMaterias();

    // Buscar la materia
    for(auto it = materias.begin();
        it != materias.end();
        ++it)
    {
        // Verificar que sea la materia indicada
        if(it->getId() == id)
        {
            // Verificar que pertenece al profesor actual
            if(it->getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            // Eliminar de la sesión
            materias.erase(it);

            // Guardar cambios
            return Persistencia::guardarMaterias(
                datos.obtenerUsuarioId(),
                materias,
                "materias.txt"
            );
        }
    }

    // No se encontró la materia
    return false;
}

/////////////////////////////////////////////////////////////
// Editar Materia
/////////////////////////////////////////////////////////////

bool SessionManager::editarMateria(
    int id,
    const std::string& nombre
)
{
    // Debe estar autenticado
    if(!datos.estaAutenticado())
    {
        return false;
    }

    // Solo profesores pueden editar materias
    if(datos.obtenerRol() != "Profesor")
    {
        return false;
    }

    // Buscar la materia
    std::vector<Materia>& materias =
        datos.obtenerVectorMaterias();

    for(auto& materia : materias)
    {
        if(materia.getId() == id)
        {
            // La materia pertenece al profesor actual
            if(materia.getProfesorId() != datos.obtenerUsuarioId())
            {
                return false;
            }

            // Evitar nombre vacío
            if(nombre.empty())
            {
                return false;
            }

            // Evitar nombres repetidos
            for(const auto& otra : materias)
            {
                if(otra.getId() != id &&
                   otra.getNombre() == nombre)
                {
                    return false;
                }
            }

            materia.setNombre(nombre);

            return Persistencia::guardarMaterias(
                datos.obtenerUsuarioId(),
                materias,
                "materias.txt"
            );
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
//planner Semana
/////////////////////////////////////////////////////////////
PlannerSemana& 
SessionManager::obtenerPlanner()
{
    return datos.obtenerPlanner();
}

std::string SessionManager::obtenerInformacionUsuario() const
{
    if(!datos.estaAutenticado())
    {
        return "NO_LOGIN";
    }

    return "USUARIO|" +
           datos.obtenerNombreCompleto() +
           "|" +
           datos.obtenerIdentificador();
}

const std::vector<Materia>&
SessionManager::obtenerVectorMaterias() const
{
    return datos.obtenerVectorMaterias();
}

int SessionManager::obtenerUsuarioId() const
{
    return datos.obtenerUsuarioId();
}


std::string SessionManager::obtenerNombreCompleto() const
{
    return datos.obtenerNombreCompleto();
}

std::string SessionManager::obtenerIdentificador() const
{
    return datos.obtenerIdentificador();
}

///////////////////////////////////////////////////////////
// Agregar Usuario
///////////////////////////////////////////////////////////

bool SessionManager::agregarUsuario(
    const std::string& rol,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{
    // ---------------------------------------------
    // Solo el Administrador puede crear usuarios
    // ---------------------------------------------

    if(!datos.estaAutenticado())
    {
        return false;
    }

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // ---------------------------------------------
    // Validar campos vacíos
    // ---------------------------------------------

    if(nombre.empty() ||
       correo.empty() ||
       password.empty() ||
       identificador.empty())
    {
        return false;
    }

    // ---------------------------------------------
    // Validar rol
    // ---------------------------------------------

    if(rol != "Administrador" &&
       rol != "Profesor" &&
       rol != "Alumno")
    {
        return false;
    }

    // ---------------------------------------------
    // Cargar usuarios existentes
    // ---------------------------------------------

    std::vector<Usuario*> usuarios;

    Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"
    );

    // ---------------------------------------------
    // Validar correo e identificador repetidos
    // ---------------------------------------------

    for(const auto* usuario : usuarios)
    {
        if(usuario == nullptr)
        {
            continue;
        }

        if(usuario->getCorreo() == correo)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }

        if(usuario->getIdentificador() == identificador)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }
    }

    // ---------------------------------------------
    // Generar ID
    // ---------------------------------------------

    int id = Persistencia::generarIdUsuario(
        "usuarios.txt"
    );

    // ---------------------------------------------
    // Crear usuario según el rol
    // ---------------------------------------------

    Usuario* nuevoUsuario = nullptr;

    if(rol == "Administrador")
    {
        nuevoUsuario = new Administrador(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }
    else if(rol == "Profesor")
    {
        nuevoUsuario = new Profesor(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }
    else if(rol == "Alumno")
    {
        nuevoUsuario = new Alumno(
            id,
            nombre,
            correo,
            password,
            identificador
        );
    }

    if(nuevoUsuario == nullptr)
    {
        for(auto* u : usuarios)
        {
            delete u;
        }

        return false;
    }

    // ---------------------------------------------
    // Guardar usuario
    // ---------------------------------------------

    bool resultado =
        Persistencia::agregarUsuario(
            *nuevoUsuario,
            "usuarios.txt"
        );

    // ---------------------------------------------
    // Liberar memoria
    // ---------------------------------------------

    delete nuevoUsuario;

    for(auto* u : usuarios)
    {
        delete u;
    }

    return resultado;
}

///////////////////////////////////////////////////////////
// Obtener Usuarios
///////////////////////////////////////////////////////////

std::string SessionManager::obtenerUsuarios() const
{
    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(usuarios))
    {
        return "ERROR";
    }

    std::string respuesta = "USUARIOS|";

    for(size_t i = 0; i < usuarios.size(); i++)
    {
        respuesta +=
            std::to_string(
                usuarios[i]->getId()
            );

        respuesta += ",";

        respuesta +=
            usuarios[i]->getRol();

        respuesta += ",";

        respuesta +=
            usuarios[i]->getNombre();

        respuesta += ",";

        respuesta +=
            usuarios[i]->getCorreo();

        if(i != usuarios.size()-1)
        {
            respuesta += ";";
        }
    }

    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    return respuesta;
}

///////////////////////////////////////////////////////////
// Actualizar Usuario
///////////////////////////////////////////////////////////

bool SessionManager::actualizarUsuario(
    int id,
    const std::string& nombre,
    const std::string& correo,
    const std::string& password,
    const std::string& identificador
)
{
    // ---------------------------------------------
    // Debe estar autenticado
    // ---------------------------------------------

    if(!datos.estaAutenticado())
    {
        return false;
    }

    // ---------------------------------------------
    // Solo el Administrador puede actualizar
    // usuarios
    // ---------------------------------------------

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // ---------------------------------------------
    // Validar campos
    // ---------------------------------------------

    if(nombre.empty() ||
       correo.empty() ||
       password.empty() ||
       identificador.empty())
    {
        return false;
    }

    // ---------------------------------------------
    // Cargar usuarios
    // ---------------------------------------------

    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"))
    {
        return false;
    }

    Usuario* usuarioObjetivo = nullptr;

    // ---------------------------------------------
    // Buscar usuario
    // ---------------------------------------------

    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr &&
           usuario->getId() == id)
        {
            usuarioObjetivo = usuario;
            break;
        }
    }

    if(usuarioObjetivo == nullptr)
    {
        for(auto* usuario : usuarios)
        {
            delete usuario;
        }

        return false;
    }

    // ---------------------------------------------
    // Validar correo e identificador duplicados
    // ---------------------------------------------

    for(auto* usuario : usuarios)
    {
        if(usuario == nullptr)
        {
            continue;
        }

        // No comparar el usuario consigo mismo
        if(usuario->getId() == id)
        {
            continue;
        }

        if(usuario->getCorreo() == correo)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }

        if(usuario->getIdentificador() == identificador)
        {
            for(auto* u : usuarios)
            {
                delete u;
            }

            return false;
        }
    }

    // ---------------------------------------------
    // Actualizar datos
    // ---------------------------------------------

    usuarioObjetivo->setNombre(nombre);
    usuarioObjetivo->setCorreo(correo);
    usuarioObjetivo->setPassword(password);
    usuarioObjetivo->setIdentificador(identificador);

    // ---------------------------------------------
    // Guardar
    // ---------------------------------------------

    bool resultado =
        Persistencia::actualizarUsuario(
            *usuarioObjetivo,
            "usuarios.txt"
        );

    // ---------------------------------------------
    // Liberar memoria
    // ---------------------------------------------

    for(auto* usuario : usuarios)
    {
        delete usuario;
    }

    return resultado;
}

///////////////////////////////////////////////////////////
// Eliminar Usuario
///////////////////////////////////////////////////////////

bool SessionManager::eliminarUsuario(
    int id
)
{
    // ---------------------------------------------
    // Debe estar autenticado
    // ---------------------------------------------

    if(!datos.estaAutenticado())
    {
        return false;
    }

    // ---------------------------------------------
    // Solo el Administrador puede eliminar
    // usuarios
    // ---------------------------------------------

    if(datos.obtenerRol() != "Administrador")
    {
        return false;
    }

    // ---------------------------------------------
    // No puede eliminarse a sí mismo
    // ---------------------------------------------

    if(id == datos.obtenerUsuarioId())
    {
        return false;
    }

    // ---------------------------------------------
    // Cargar usuarios
    // ---------------------------------------------

    std::vector<Usuario*> usuarios;

    if(!Persistencia::cargarUsuarios(
        usuarios,
        "usuarios.txt"))
    {
        return false;
    }

    // ---------------------------------------------
    // Buscar usuario
    // ---------------------------------------------

    Usuario* usuarioObjetivo = nullptr;

    for(auto* usuario : usuarios)
    {
        if(usuario != nullptr &&
           usuario->getId() == id)
        {
            usuarioObjetivo = usuario;
            break;
        }
    }

    if(usuarioObjetivo == nullptr)
    {
        for(auto* usuario : usuarios)
        {
            delete usuario;
        }

        return false;
    }

    // ---------------------------------------------
    // No eliminar al último administrador
    // ---------------------------------------------

    if(usuarioObjetivo->getRol() == "Administrador")
    {
        int cantidadAdministradores = 0;

        for(auto* usuario : usuarios)
        {
           if(usuario != nullptr && usuario->getRol() == "Administrador")
            {
                cantidadAdministradores++;
            }
        }

        if(cantidadAdministradores <= 1)
        {
            for(auto* usuario : usuarios)
            {
                delete usuario;
            }

        return false;
        }
    }

    // ---------------------------------------------
    // Eliminar de Persistencia
    // ---------------------------------------------

    bool resultado = Persistencia::eliminarUsuario(
        id,
        "usuarios.txt"
    );

    // ---------------------------------------------
    // Liberar memoria
    // ---------------------------------------------

    for(auto* usuario : usuarios)
    {
        delete usuario;
    }

    return resultado;
}   

///////////////////////////////////////////////////////////
// Obtener Rol
///////////////////////////////////////////////////////////

std::string SessionManager::obtenerRol() const
{
    return datos.obtenerRol();
}
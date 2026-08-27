#include "persistencia.hpp"


#include <fstream>
#include <sstream>
#include <vector>


///////////////////////////////////////////////////////////
// Guardar Materias
///////////////////////////////////////////////////////////

bool Persistencia::guardarMaterias(
    int profesorId,
    const std::vector<Materia>& materias,
    const std::string& archivo
)
{
    if(profesorId <= 0)
    {
        return false;
    }

    //--------------------------------------------------
    // Materias que pertenecen a otros profesores
    //--------------------------------------------------

    std::vector<Materia> materiasOtrosProfesores;

    {
        std::vector<Materia> todasLasMaterias;

        if(!cargarMaterias(
            todasLasMaterias,
            archivo))
        {
            // Si el archivo todavía no existe,
            // simplemente continuamos.
            todasLasMaterias.clear();
        }

        for(const auto& materia : todasLasMaterias)
        {
            if(materia.getProfesorId() != profesorId)
            {
                materiasOtrosProfesores.push_back(
                    materia
                );
            }
        }
    }

    //--------------------------------------------------
    // Crear lista final
    //--------------------------------------------------

    std::vector<Materia> materiasFinales =
        materiasOtrosProfesores;

    //--------------------------------------------------
    // Agregar materias del profesor actual
    //
    // IMPORTANTE:
    // Solo se agregan materias que realmente
    // pertenecen al profesor recibido.
    //--------------------------------------------------

    for(const auto& materia : materias)
    {
        if(materia.getProfesorId() != profesorId)
        {
            continue;
        }

        //------------------------------------------------
        // Evitar duplicar ID
        //------------------------------------------------

        bool existe = false;

        for(const auto& existente : materiasFinales)
        {
            if(existente.getId() == materia.getId())
            {
                existe = true;
                break;
            }
        }

        if(!existe)
        {
            materiasFinales.push_back(
                materia
            );
        }
    }

    //--------------------------------------------------
    // Abrir archivo para reemplazarlo completamente
    //--------------------------------------------------

    std::ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    //--------------------------------------------------
    // Encabezado
    //--------------------------------------------------

    out
        << "idMateria;"
        << "nombre;"
        << "profesorId"
        << "\n";

    //--------------------------------------------------
    // Guardar materias finales
    //--------------------------------------------------

    for(const auto& materia : materiasFinales)
    {
        out
            << materia.getId()
            << ";"
            << materia.getNombre()
            << ";"
            << materia.getProfesorId()
            << "\n";
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

    std::getline(in, linea);

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

    // Si el archivo no existe,
    // comenzamos desde 1
    if(!in.is_open())
    {
        return 1;
    }

    int mayorId = 0;

    std::string linea;

    // =============================================
    // Ignorar encabezado
    // =============================================

    std::getline(in, linea);

    // =============================================
    // Leer materias
    // =============================================

    while(std::getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string campo;

        // =========================================
        // idMateria
        // =========================================

        if(!std::getline(ss, campo, ';'))
        {
            continue;
        }

        try
        {
            int id = std::stoi(campo);

            if(id > mayorId)
            {
                mayorId = id;
            }
        }
        catch(const std::invalid_argument&)
        {
            // Ignorar líneas que no tengan
            // un ID numérico válido
            continue;
        }
        catch(const std::out_of_range&)
        {
            continue;
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
    {
        return false;
    }

    out << "rol|id|nombre|correo|password|identificador\n";

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
    {
        return false;
    }

    // Liberar memoria anterior
    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    usuarios.clear();

    std::string linea;

    std::getline(in, linea);    

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

    bool ok = guardarUsuarios(
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
            u->setNombre(usuario.getNombre());
            u->setPassword(usuario.getPassword());
            u->setCorreo(usuario.getCorreo());
            u->setIdentificador(usuario.getIdentificador());

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

///////////////////////////////////////////////////////////
// Conversión de Tipo de Tarea
///////////////////////////////////////////////////////////

std::string Persistencia::tipoTareaAString(
    TipoTarea tipo)
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
    }

    return "OTRO";
}

TipoTarea Persistencia::stringATipoTarea(
    const std::string& texto)
{
    if(texto == "TAREA")
        return TipoTarea::TAREA;

    if(texto == "EXAMEN")
        return TipoTarea::EXAMEN;

    if(texto == "PRACTICA")
        return TipoTarea::PRACTICA;

    if(texto == "PROYECTO")
        return TipoTarea::PROYECTO;

    if(texto == "TRABAJO")
        return TipoTarea::TRABAJO;

    return TipoTarea::OTRO;
}

///////////////////////////////////////////////////////////
// Guardar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::guardarTareas(
    const std::vector<Tarea>& tareas,
    const std::string& archivo
)
{
    std::ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    //==================================================
    // ENCABEZADO
    //==================================================

    salida
        << "idTarea|"
        << "idMateria|"
        << "titulo|"
        << "fechaEntrega|"
        << "descripcion|"
        << "tipo"
        << "\n";


    //==================================================
    // TAREAS
    //==================================================

    for(const auto& tarea : tareas)
    {
        salida
            << tarea.getId() << "|"
            << tarea.getMateriaId() << "|"
            << tarea.getTitulo() << "|"
            << tarea.getFechaEntrega() << "|"
            << tarea.getDescripcion() << "|"
            << tipoTareaAString(
                tarea.getTipo()
            )
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::cargarTareas(
    std::vector<Tarea>& tareas,
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    tareas.clear();

    std::string linea;


    //==================================================
    // SALTAR ENCABEZADO
    //==================================================

    if(!std::getline(entrada, linea))
    {
        entrada.close();

        return true;
    }


    //==================================================
    // CARGAR TAREAS
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;
        std::string materiaIdTexto;
        std::string titulo;
        std::string fechaEntrega;
        std::string descripcion;
        std::string tipoTexto;


        //==============================================
        // SEPARAR CAMPOS
        //==============================================

        std::getline(
            ss,
            idTexto,
            '|'
        );

        std::getline(
            ss,
            materiaIdTexto,
            '|'
        );

        std::getline(
            ss,
            titulo,
            '|'
        );

        std::getline(
            ss,
            fechaEntrega,
            '|'
        );

        std::getline(
            ss,
            descripcion,
            '|'
        );

        std::getline(
            ss,
            tipoTexto,
            '|'
        );


        //==============================================
        // VALIDAR CAMPOS
        //==============================================

        if(
            idTexto.empty() ||
            materiaIdTexto.empty() ||
            titulo.empty() ||
            fechaEntrega.empty() ||
            descripcion.empty() ||
            tipoTexto.empty()
        )
        {
            continue;
        }


        //==============================================
        // CONVERTIR IDS
        //==============================================

        int id;
        int materiaId;

        try
        {
            id = std::stoi(
                idTexto
            );

            materiaId = std::stoi(
                materiaIdTexto
            );
        }
        catch(...)
        {
            continue;
        }


        //==============================================
        // CONVERTIR TIPO
        //==============================================

        TipoTarea tipo =
            stringATipoTarea(
                tipoTexto
            );


        //==============================================
        // CREAR TAREA
        //==============================================

        Tarea tarea(
            id,
            materiaId,
            titulo,
            descripcion,
            fechaEntrega,
            tipo
        );


        tareas.push_back(tarea);
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// Generar ID de Tarea
///////////////////////////////////////////////////////////

int Persistencia::generarIdTarea(
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    int mayorId = 0;

    std::string linea;

    //==================================================
    // Saltar encabezado
    //==================================================

    if(!std::getline(entrada, linea))
    {
        entrada.close();
        return 1;
    }

    //==================================================
    // Leer tareas
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;

        // El archivo utiliza |
        std::getline(
            ss,
            idTexto,
            '|'
        );

        if(idTexto.empty())
        {
            continue;
        }

        int id;

        try
        {
            id = std::stoi(idTexto);
        }
        catch(...)
        {
            continue;
        }

        if(id > mayorId)
        {
            mayorId = id;
        }
    }

    entrada.close();

    return mayorId + 1;
}

///////////////////////////////////////////////////////////
// Guardar Subtareas
///////////////////////////////////////////////////////////

bool Persistencia::guardarSubtareas(
    const std::vector<Subtarea>& subtareas,
    const std::string& archivo
)
{
    std::ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    //==================================================
    // ENCABEZADO
    //==================================================

    salida
        << "idSubtarea|"
        << "idTarea|"
        << "idAlumno|"
        << "descripcion|"
        << "estado"
        << "\n";


    //==================================================
    // SUBTAREAS
    //==================================================

    for(const auto& subtarea : subtareas)
    {
        salida
            << subtarea.getId() << "|"
            << subtarea.getTareaId() << "|"
            << subtarea.getAlumnoId() << "|"
            << subtarea.getDescripcion() << "|"
            << estadoSubtareaAString(
                subtarea.getEstado()
            )
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Subtareas
///////////////////////////////////////////////////////////

bool Persistencia::cargarSubtareas(
    std::vector<Subtarea>& subtareas,
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    subtareas.clear();

    std::string linea;

    //==================================================
    // SALTAR ENCABEZADO
    //==================================================

    if(!std::getline(entrada, linea))
    {
        entrada.close();

        return true;
    }


    //==================================================
    // LEER SUBTAREAS
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;
        std::string tareaIdTexto;
        std::string alumnoIdTexto;
        std::string descripcion;
        std::string estadoTexto;


        //==============================================
        // SEPARAR CAMPOS
        //==============================================

        std::getline(
            ss,
            idTexto,
            '|'
        );

        std::getline(
            ss,
            tareaIdTexto,
            '|'
        );

        std::getline(
            ss,
            alumnoIdTexto,
            '|'
        );

        std::getline(
            ss,
            descripcion,
            '|'
        );

        std::getline(
            ss,
            estadoTexto,
            '|'
        );


        //==============================================
        // VALIDAR
        //==============================================

        if(
            idTexto.empty() ||
            tareaIdTexto.empty() ||
            alumnoIdTexto.empty() ||
            descripcion.empty() ||
            estadoTexto.empty()
        )
        {
            continue;
        }


        //==============================================
        // CONVERTIR IDS
        //==============================================

        int id;
        int tareaId;
        int alumnoId;

        try
        {
            id = std::stoi(
                idTexto
            );

            tareaId = std::stoi(
                tareaIdTexto
            );

            alumnoId = std::stoi(
                alumnoIdTexto
            );
        }
        catch(...)
        {
            continue;
        }


        //==============================================
        // CONVERTIR ESTADO
        //==============================================

        EstadoSubtarea estado =
            stringAEstadoSubtarea(
                estadoTexto
            );


        //==============================================
        // CREAR SUBTAREA
        //==============================================

        Subtarea subtarea(
            id,
            tareaId,
            alumnoId,
            descripcion
        );

        subtarea.setEstado(
            estado
        );

        subtareas.push_back(
            subtarea
        );
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// Generar ID de Subtarea
///////////////////////////////////////////////////////////

int Persistencia::generarIdSubtarea(
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    int mayorId = 0;

    std::string linea;


    //==================================================
    // SALTAR ENCABEZADO
    //==================================================

    if(!std::getline(entrada, linea))
    {
        entrada.close();

        return 1;
    }


    //==================================================
    // LEER SUBTAREAS
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;

        std::getline(
            ss,
            idTexto,
            '|'
        );

        if(idTexto.empty())
        {
            continue;
        }

        int id;

        try
        {
            id = std::stoi(
                idTexto
            );
        }
        catch(...)
        {
            continue;
        }

        if(id > mayorId)
        {
            mayorId = id;
        }
    }

    entrada.close();

    return mayorId + 1;
}

///////////////////////////////////////////////////////////
// Conversión de Estado de Subtarea
///////////////////////////////////////////////////////////

std::string Persistencia::estadoSubtareaAString(
    EstadoSubtarea estado
)
{
    switch(estado)
    {
        case EstadoSubtarea::PENDIENTE:
            return "PENDIENTE";

        case EstadoSubtarea::EN_PROGRESO:
            return "EN_PROGRESO";

        case EstadoSubtarea::COMPLETADA:
            return "COMPLETADA";
    }

    return "PENDIENTE";
}

EstadoSubtarea Persistencia::stringAEstadoSubtarea(
    const std::string& texto
)
{
    if(texto == "EN_PROGRESO")
    {
        return EstadoSubtarea::EN_PROGRESO;
    }

    if(texto == "COMPLETADA")
    {
        return EstadoSubtarea::COMPLETADA;
    }

    return EstadoSubtarea::PENDIENTE;
}

///////////////////////////////////////////////////////////
// Guardar Planner
///////////////////////////////////////////////////////////

bool Persistencia::guardarPlanner(
    int alumnoId,
    const PlannerSemana& planner,
    const std::string& archivo
)
{
    //==================================================
    // Validar ID
    //==================================================

    if(alumnoId <= 0)
    {
        return false;
    }

    //==================================================
    // Archivo temporal
    //
    // Primero escribimos todo aquí.
    // NO modificamos todavía planner.txt
    //==================================================

    const std::string archivoTemporal =
        archivo + ".tmp";

    //==================================================
    // Cargar registros de otros alumnos
    //==================================================

    std::vector<std::string> registros;

    std::ifstream entrada(archivo);

    if(entrada.is_open())
    {
        std::string linea;

        // Saltar encabezado
        std::getline(
            entrada,
            linea
        );

        while(std::getline(entrada,linea))
        {
            if(linea.empty())
            {
                continue;
            }

            std::stringstream ss(linea);

            std::string alumnoTexto;

            std::getline(
                ss,
                alumnoTexto,
                '|'
            );

            if(alumnoTexto.empty())
            {
                continue;
            }

            int idAlumnoArchivo;

            try
            {
                idAlumnoArchivo =
                    std::stoi(alumnoTexto);
            }
            catch(...)
            {
                // Registro inválido.
                // No lo copiamos.
                continue;
            }

            //==================================================
            // IMPORTANTE:
            //
            // Solo conservamos los registros de otros alumnos.
            //
            // Los registros del alumno actual serán reemplazados
            // por el contenido actual de PlannerSemana.
            //==================================================

            if(idAlumnoArchivo != alumnoId)
            {
                registros.push_back(linea);
            }
        }

        entrada.close();
    }

    //==================================================
    // Abrir archivo temporal
    //==================================================

    std::ofstream salida(
        archivoTemporal,
        std::ios::trunc
    );

    if(!salida.is_open())
    {
        return false;
    }

    //==================================================
    // Encabezado
    //==================================================

    salida
        << "idAlumno|"
        << "fecha|"
        << "tipo|"
        << "idElemento|"
        << "prioridad"
        << "\n";

    //==================================================
    // Verificar escritura del encabezado
    //==================================================

    if(!salida.good())
    {
        salida.close();
        std::remove(archivoTemporal.c_str());

        return false;
    }

    //==================================================
    // Restaurar registros de otros alumnos
    //==================================================

    for(const std::string& registro : registros)
    {
        salida
            << registro
            << "\n";

        if(!salida.good())
        {
            salida.close();
            std::remove(archivoTemporal.c_str());

            return false;
        }
    }

    //==================================================
    // Guardar Planner del alumno actual
    //==================================================

    for(int i = 0; i < 7; ++i)
    {
        const PlannerDia& dia =
            planner.getDia(i);

        const std::string& fecha =
            dia.getFecha();

        //==================================================
        // TAREAS
        //==================================================

        for(const TareaPlanner& tarea :
            dia.getTareas())
        {
            salida
                << alumnoId
                << "|"
                << fecha
                << "|"
                << "TAREA"
                << "|"
                << tarea.idTarea
                << "|"
                << prioridadPlannerAString(
                    tarea.prioridad
                )
                << "\n";

            if(!salida.good())
            {
                salida.close();
                std::remove(archivoTemporal.c_str());

                return false;
            }
        }

        //==================================================
        // SUBTAREAS
        //==================================================

        for(int idSubtarea :
            dia.getSubtareas())
        {
            salida
                << alumnoId
                << "|"
                << fecha
                << "|"
                << "SUBTAREA"
                << "|"
                << idSubtarea
                << "|"
                << "-"
                << "\n";

            if(!salida.good())
            {
                salida.close();
                std::remove(archivoTemporal.c_str());

                return false;
            }
        }
    }

    //==================================================
    // Cerrar archivo temporal
    //==================================================

    salida.close();

    if(salida.fail())
    {
        std::remove(archivoTemporal.c_str());

        return false;
    }

    //==================================================
    // Reemplazar archivo original
    //==================================================

    // Eliminar archivo anterior
    if(std::remove(archivo.c_str()) != 0)
    {
        // Puede no existir todavía.
        // En ese caso continuamos.
    }

    // Renombrar temporal como archivo definitivo
    if(std::rename(
        archivoTemporal.c_str(),
        archivo.c_str()
    ) != 0)
    {
        // Si el renombrado falla,
        // eliminar el temporal.
        std::remove(archivoTemporal.c_str());

        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Planner
///////////////////////////////////////////////////////////

bool Persistencia::cargarPlanner(
    int alumnoId,
    PlannerSemana& planner,
    const std::string& archivo
)
{
    if(alumnoId <= 0)
    {
        return false;
    }

    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    std::string linea;

    //==================================================
    // Saltar encabezado
    //==================================================

    if(!std::getline(entrada,linea))
    {
        entrada.close();
        return true;
    }

    //==================================================
    // Leer registros
    //==================================================

    while(std::getline(entrada,linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);
        
        std::string alumnoTexto;
        std::string fecha;
        std::string tipo;
        std::string idElementoTexto;
        std::string prioridadTexto;

        //==============================================
        // Separar campos
        //==============================================

        std::getline(ss,alumnoTexto,'|');
        std::getline(ss,fecha,'|');
        std::getline(ss,tipo,'|');
        std::getline(ss,idElementoTexto,'|');
        std::getline(ss,prioridadTexto,'|');

        //==============================================
        // Validar
        //==============================================

        if(alumnoTexto.empty() || fecha.empty() || tipo.empty() ||
           idElementoTexto.empty())
        {
            continue;
        }

        //==============================================
        // Convertir alumno
        //==============================================

        int idAlumnoArchivo;

        try
        {
            idAlumnoArchivo = std::stoi(alumnoTexto);
        }
        catch(...)
        {
            continue;
        }

        // Solo cargar datos del alumno actual
        if(idAlumnoArchivo != alumnoId)
        {
            continue;
        }

        //==============================================
        // Convertir ID
        //==============================================

        int idElemento;

        try
        {
            idElemento = std::stoi(idElementoTexto);
        }
        catch(...)
        {
            continue;
        }

        //==============================================
        // Buscar el día correspondiente
        //==============================================

        PlannerDia* diaEncontrado = nullptr;

        for(int i = 0; i < 7; ++i)
        {
            PlannerDia& dia = planner.getDia(i);

            if(dia.getFecha() == fecha)
            {
                diaEncontrado = &dia;
                break;
            }
        }

        // Si la fecha no pertenece a la semana
        // actual, no se carga.
        if(diaEncontrado == nullptr)
        {
            continue;
        }

        //==============================================
        // TAREA
        //==============================================

        if(tipo == "TAREA")
        {
            PrioridadPlanner prioridad = stringAPrioridadPlanner(prioridadTexto);

            diaEncontrado->agregarTarea(idElemento,prioridad);
        }

        //==============================================
        // SUBTAREA
        //==============================================

        else if(tipo == "SUBTAREA")
        {
            diaEncontrado->agregarSubtarea(idElemento);
        }
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// Conversión de String a Prioridad del Planner
///////////////////////////////////////////////////////////

PrioridadPlanner Persistencia::stringAPrioridadPlanner(
    const std::string& texto
)
{
    if(texto == "BAJA")
    {
        return PrioridadPlanner::BAJA;
    }

    if(texto == "ALTA")
    {
        return PrioridadPlanner::ALTA;
    }

    return PrioridadPlanner::MEDIA;
}

///////////////////////////////////////////////////////////
// Conversión de Prioridad del Planner a String
///////////////////////////////////////////////////////////

std::string Persistencia::prioridadPlannerAString(
    PrioridadPlanner prioridad
)
{
    switch(prioridad)
    {
        case PrioridadPlanner::BAJA:
            return "BAJA";

        case PrioridadPlanner::MEDIA:
            return "MEDIA";

        case PrioridadPlanner::ALTA:
            return "ALTA";
    }

    return "MEDIA";
}

///////////////////////////////////////////////////////////
// ESTADO TAREA -> STRING
///////////////////////////////////////////////////////////

std::string Persistencia::estadoTareaAString(
    EstadoTarea estado
)
{
    switch(estado)
    {
        case EstadoTarea::COMPLETADO:
            return "COMPLETADO";

        case EstadoTarea::NO_COMPLETADO:
            return "NO_COMPLETADO";
    }

    return "NO_COMPLETADO";
}

///////////////////////////////////////////////////////////
// STRING -> ESTADO TAREA
///////////////////////////////////////////////////////////

EstadoTarea Persistencia::stringAEstadoTarea(
    const std::string& estado
)
{
    if(estado == "COMPLETADO")
    {
        return EstadoTarea::COMPLETADO;
    }

    return EstadoTarea::NO_COMPLETADO;
}

///////////////////////////////////////////////////////////
// GUARDAR ESTADOS DE TAREAS
///////////////////////////////////////////////////////////

bool Persistencia::guardarEstadosTareas(
    const std::vector<EstadoTareaAlumno>& estados,
    const std::string& archivo
)
{
    std::ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    salida << "idTarea|idAlumno|estado\n";

    for(const auto& estado : estados)
    {
        salida
            << estado.getTareaId()
            << "|"
            << estado.getAlumnoId()
            << "|"
            << estadoTareaAString(
                   estado.getEstado()
               )
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// CARGAR ESTADOS DE TAREAS
///////////////////////////////////////////////////////////

bool Persistencia::cargarEstadosTareas(
    std::vector<EstadoTareaAlumno>& estados,
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    estados.clear();

    std::string linea;

    // Saltar encabezado
    std::getline(entrada,linea);

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTareaTexto;
        std::string idAlumnoTexto;
        std::string estadoTexto;

        std::getline(ss,idTareaTexto,'|');
        std::getline(ss,idAlumnoTexto,'|');
        std::getline(ss,estadoTexto,'|');

        try
        {
            int idTarea = std::stoi(idTareaTexto);
            int idAlumno = std::stoi(idAlumnoTexto);

            EstadoTarea estado = stringAEstadoTarea(estadoTexto);

            estados.emplace_back(idTarea,idAlumno,estado);
        }
        catch(...)
        {
            continue;
        }
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// TIPO NOTIFICACION -> STRING
///////////////////////////////////////////////////////////

std::string Persistencia::tipoNotificacionAString(
    TipoNotificacion tipo
)
{
    switch(tipo)
    {
        case TipoNotificacion::RECORDATORIO:
            return "RECORDATORIO";

        case TipoNotificacion::NUEVA_TAREA:
            return "NUEVA_TAREA";

        case TipoNotificacion::CAMBIO_FECHA:
            return "CAMBIO_FECHA";

        case TipoNotificacion::MENSAJE_PROFESOR:
            return "MENSAJE_PROFESOR";

        case TipoNotificacion::SISTEMA:
            return "SISTEMA";

        default:
            return "SISTEMA";
    }
}


///////////////////////////////////////////////////////////
// STRING -> TIPO NOTIFICACION
///////////////////////////////////////////////////////////

TipoNotificacion Persistencia::stringATipoNotificacion(
    const std::string& texto
)
{
    if(texto == "RECORDATORIO")
    {
        return TipoNotificacion::RECORDATORIO;
    }

    if(texto == "NUEVA_TAREA")
    {
        return TipoNotificacion::NUEVA_TAREA;
    }

    if(texto == "CAMBIO_FECHA")
    {
        return TipoNotificacion::CAMBIO_FECHA;
    }

    if(texto == "MENSAJE_PROFESOR")
    {
        return TipoNotificacion::MENSAJE_PROFESOR;
    }

    return TipoNotificacion::SISTEMA;
}


///////////////////////////////////////////////////////////
// CONVERTIR TIPO REFERENCIA NOTIFICACIÓN A STRING
///////////////////////////////////////////////////////////

std::string Persistencia::tipoReferenciaNotificacionAString(
    TipoReferenciaNotificacion tipo
)
{
    switch(tipo)
    {
        case TipoReferenciaNotificacion::NINGUNA:
            return "NINGUNA";

        case TipoReferenciaNotificacion::TAREA:
            return "TAREA";

        case TipoReferenciaNotificacion::MATERIA:
            return "MATERIA";

        case TipoReferenciaNotificacion::PROFESOR:
            return "PROFESOR";
    }

    return "NINGUNA";
}

///////////////////////////////////////////////////////////
// CONVERTIR STRING A TIPO REFERENCIA NOTIFICACIÓN
///////////////////////////////////////////////////////////

TipoReferenciaNotificacion
Persistencia::stringATipoReferenciaNotificacion(
    const std::string& texto
)
{
    if(texto == "NINGUNA")
    {
        return TipoReferenciaNotificacion::NINGUNA;
    }

    if(texto == "TAREA")
    {
        return TipoReferenciaNotificacion::TAREA;
    }

    if(texto == "MATERIA")
    {
        return TipoReferenciaNotificacion::MATERIA;
    }

    if(texto == "PROFESOR")
    {
        return TipoReferenciaNotificacion::PROFESOR;
    }

    // Valor por defecto si el texto
    // no corresponde a ningún tipo
    return TipoReferenciaNotificacion::NINGUNA;
}

////////////////////////////////////////////////////////////
// GUARDAR NOTIFICACIONES
////////////////////////////////////////////////////////////

bool Persistencia::guardarNotificaciones(
    const std::vector<Notificacion>& notificaciones,
    const std::string& archivo
)
{
    std::ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    //==================================================
    // ENCABEZADO
    //==================================================

    salida
        << "id|idUsuario|tipo|idReferencia|tipoReferencia|titulo|mensaje|fecha|leida"
        << "\n";

    //==================================================
    // REGISTROS
    //==================================================

    for(const auto& notificacion : notificaciones)
    {
        salida
            << notificacion.getId()
            << "|"
            << notificacion.getUsuarioId()
            << "|"
            << tipoNotificacionAString(
                notificacion.getTipo()
            )
            << "|"
            << notificacion.getIdReferencia()
            << "|"
            << tipoReferenciaNotificacionAString(
                notificacion.getTipoReferencia()
            )
            << "|"
            << notificacion.getTitulo()
            << "|"
            << notificacion.getMensaje()
            << "|"
            << notificacion.getFecha()
            << "|"
            << (
                notificacion.estaLeida()
                ? "1"
                : "0"
            )
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// CARGAR NOTIFICACIONES
///////////////////////////////////////////////////////////

bool Persistencia::cargarNotificaciones(
    std::vector<Notificacion>& notificaciones,
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    notificaciones.clear();

    std::string linea;

    //==================================================
    // SALTAR ENCABEZADO
    //==================================================

    if(!std::getline(entrada, linea))
    {
        entrada.close();
        return true;
    }

    //==================================================
    // LEER REGISTROS
    //==================================================

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;
        std::string idUsuarioTexto;
        std::string tipoTexto;
        std::string idReferenciaTexto;
        std::string tipoReferenciaTexto;
        std::string titulo;
        std::string mensaje;
        std::string fecha;
        std::string leidaTexto;

        //================================================
        // SEPARAR CAMPOS
        //================================================

        std::getline(
            ss,
            idTexto,
            '|'
        );

        std::getline(
            ss,
            idUsuarioTexto,
            '|'
        );

        std::getline(
            ss,
            tipoTexto,
            '|'
        );

        std::getline(
            ss,
            idReferenciaTexto,
            '|'
        );

        std::getline(
            ss,
            tipoReferenciaTexto,
            '|'
        );

        std::getline(
            ss,
            titulo,
            '|'
        );

        std::getline(
            ss,
            mensaje,
            '|'
        );

        std::getline(
            ss,
            fecha,
            '|'
        );

        std::getline(
            ss,
            leidaTexto,
            '|'
        );

        //================================================
        // VALIDAR CAMPOS OBLIGATORIOS
        //================================================

        if(
            idTexto.empty() ||
            idUsuarioTexto.empty() ||
            tipoTexto.empty() ||
            idReferenciaTexto.empty() ||
            tipoReferenciaTexto.empty() ||
            titulo.empty() ||
            mensaje.empty() ||
            fecha.empty()
        )
        {
            continue;
        }

        //================================================
        // CONVERTIR DATOS
        //================================================

        try
        {
            int id =
                std::stoi(idTexto);

            int idUsuario =
                std::stoi(idUsuarioTexto);

            int idReferencia =
                std::stoi(idReferenciaTexto);

            TipoNotificacion tipo =
                stringATipoNotificacion(
                    tipoTexto
                );

            TipoReferenciaNotificacion tipoReferencia =
                stringATipoReferenciaNotificacion(
                    tipoReferenciaTexto
                );

            //================================================
            // CREAR NOTIFICACIÓN
            //================================================

            Notificacion notificacion(
                id,
                idUsuario,
                tipo,
                idReferencia,
                tipoReferencia,
                titulo,
                mensaje,
                fecha
            );

            //================================================
            // RESTAURAR ESTADO DE LECTURA
            //================================================

            if(leidaTexto == "1")
            {
                notificacion.marcarComoLeida();
            }
            else
            {
                notificacion.marcarComoNoLeida();
            }

            //================================================
            // AGREGAR AL VECTOR
            //================================================

            notificaciones.push_back(
                notificacion
            );
        }
        catch(...)
        {
            // Si un registro está corrupto,
            // simplemente se ignora.
            continue;
        }
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// GENERAR ID NOTIFICACIÓN
///////////////////////////////////////////////////////////

int Persistencia::generarIdNotificacion(
    const std::string& archivo
)
{
    std::ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    std::string linea;

    // Saltar encabezado
    std::getline(entrada, linea);

    int mayorId = 0;

    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        std::stringstream ss(linea);

        std::string idTexto;

        std::getline(
            ss,
            idTexto,
            '|'
        );

        try
        {
            int id =
                std::stoi(idTexto);

            if(id > mayorId)
            {
                mayorId = id;
            }
        }
        catch(...)
        {
            continue;
        }
    }

    entrada.close();

    return mayorId + 1;
}
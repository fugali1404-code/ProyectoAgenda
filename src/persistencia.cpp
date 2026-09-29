#include "persistencia.hpp"

#include <fstream>
#include <sstream>
#include <vector>


///////////////////////////////////////////////////////////
// Guardar Materias
///////////////////////////////////////////////////////////

bool Persistencia::guardarMaterias(
    int profesorId,
    const vector<Materia>& materias,
    const string& archivo
)
{
    if(profesorId <= 0)
    {
        return false;
    }

    //--------------------------------------------------
    // Materias que pertenecen a otros profesores
    //--------------------------------------------------

    vector<Materia> materiasOtrosProfesores;

    {
        vector<Materia> todasLasMaterias;

        if(!cargarMaterias(todasLasMaterias,archivo))
        {
            todasLasMaterias.clear();
        }

        for(const auto& materia : todasLasMaterias)
        {
            if(materia.getProfesorId() != profesorId)
            {
                materiasOtrosProfesores.push_back(materia);
            }
        }
    }

    // Crear lista final
    vector<Materia> materiasFinales = materiasOtrosProfesores;

    // Agregar materias del profesor actual
    for(const auto& materia : materias)
    {
        if(materia.getProfesorId() != profesorId)
        {
            continue;
        }

        // Evitar duplicar ID
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
            materiasFinales.push_back(materia);
        }
    }

    
    // Abrir archivo para reemplazarlo completamente
    ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    // Encabezado
    out
        << "idMateria;"
        << "nombre;"
        << "profesorId"
        << "\n";

    
    // Guardar materias finales
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
    vector<Materia>& materias,
    const string& archivo)
{
    ifstream in(archivo);

    if(!in.is_open())
    {
        return false;
    }

    materias.clear();

    string linea;

    getline(in, linea);

    while(getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string campo;
        int idMateria;
        string nombre;
        int profesorId;

        // ID de materia
        if(!getline(ss, campo, ';'))
        {
            continue;
        }

        idMateria = stoi(campo);

        // Nombre
        if(!getline(ss, nombre, ';'))
        {
            continue;
        }

        // ID del profesor
        if(!getline(ss, campo, ';'))
        {
            continue;
        }

        profesorId = stoi(campo);
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
    const string& archivo)
{
    ifstream in(archivo);

    // Si el archivo no existe,comenzamos desde 1
    if(!in.is_open())
    {
        return 1;
    }

    int mayorId = 0;
    string linea;

    // Ignorar encabezado
    getline(in, linea);

    // Leer materias
    while(getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string campo;

        // idMateria
        if(!std::getline(ss, campo, ';'))
        {
            continue;
        }

        try
        {
            int id = stoi(campo);

            if(id > mayorId)
            {
                mayorId = id;
            }
        }
        catch(const invalid_argument&)
        {
            // Ignorar líneas que no tengan un ID numérico válido
            continue;
        }
        catch(const out_of_range&)
        {
            continue;
        }
    }

    in.close();

    return mayorId + 1;
}


///////////////////////////////////////////////////////////
// Guardar Calificaciones
///////////////////////////////////////////////////////////

bool Persistencia::guardarCalificaciones(
    const vector<Calificacion>& calificaciones,
    const string& archivo
)
{
    ofstream out(archivo);

    if(!out.is_open())
    {
        return false;
    }

    // Encabezado
    out
        << "idAlumno|"
        << "idTarea|"
        << "calificacion"
        << "\n";

    // Guardar calificaciones
    for(const auto& calificacion : calificaciones)
    {
        out
            << calificacion.getIdAlumno()
            << "|"
            << calificacion.getIdTarea()
            << "|"
            << calificacion.getCalificacion()
            << "\n";
    }

    out.close();

    return true;
}


///////////////////////////////////////////////////////////
// Cargar Calificaciones
///////////////////////////////////////////////////////////

bool Persistencia::cargarCalificaciones(
    vector<Calificacion>& calificaciones,
    const string& archivo
)
{
    ifstream in(archivo);

    if(!in.is_open())
    {
        return false;
    }

    calificaciones.clear();

    string linea;

    //--------------------------------------------------
    // Ignorar encabezado
    //--------------------------------------------------

    getline(in, linea);

    //--------------------------------------------------
    // Leer registros
    //--------------------------------------------------

    while(getline(in, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idAlumnoTexto;
        string idTareaTexto;
        string calificacionTexto;

        // ID alumno
        if(!getline(ss,idAlumnoTexto,'|'))
        {
            continue;
        }

        // ID tarea
        if(!std::getline(ss,idTareaTexto,'|'))
        {
            continue;
        }

        // Calificación
        if(!std::getline(ss,calificacionTexto,'|'))
        {
            continue;
        }

        // Convertir valores
        int idAlumno;
        int idTarea;
        double calificacion;

        try
        {
            idAlumno = std::stoi(idAlumnoTexto);
            idTarea = std::stoi(idTareaTexto);
            calificacion = std::stod(calificacionTexto);
        }
        catch(...)
        {
            continue;
        }

        // Validar
        if(idAlumno <= 0)
        {
            continue;
        }

        if(idTarea <= 0)
        {
            continue;
        }

        if(calificacion < 0.0 || calificacion > 10.0)
        {
            continue;
        }

        // Crear calificación
        Calificacion nuevaCalificacion(
            idAlumno,
            idTarea,
            calificacion
        );

        calificaciones.push_back(nuevaCalificacion);
    }

    in.close();

    return true;
}


///////////////////////////////////////////////////////////
// Guardar Usuarios
///////////////////////////////////////////////////////////

bool Persistencia::guardarUsuarios(
    const vector<Usuario*>& usuarios,
    const string& archivo)
{
    ofstream out(archivo);

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
    vector<Usuario*>& usuarios,
    const string& archivo)
{
    ifstream in(archivo);

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

    string linea;
    getline(in, linea);    

    while(getline(in, linea))
    {
        if(linea.empty())
            continue;
        
        stringstream ss(linea);
        string rol;
        string campo;
        getline(ss, rol, '|');
        getline(ss, campo, '|'); // id
        int id = stoi(campo);
        string nombre;
        getline(ss, nombre, '|');
        string correo;
        getline(ss, correo, '|');
        string password;
        getline(ss, password, '|');
        string identificador;
        getline(ss, identificador);

        Usuario* usuario = nullptr;

        if(rol == "Alumno")
        {
            usuario = new Alumno(
                id,
                nombre,
                correo,
                password,
                identificador
            );
        }
        
        else if(rol == "Profesor")
        {
            usuario = new Profesor(
                id,
                nombre,
                correo,
                password,
                identificador
            );
        }

        else if(rol == "Administrador")
        {
            usuario = new Administrador(
                id,
                nombre,
                correo,
                password,
                identificador
            );
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
    const string& usuario,
    const string& password,
    const string& archivo
)
{
    vector<Usuario*> usuarios;

    if(!cargarUsuarios(usuarios,archivo))
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
    const string& archivo
)
{
    vector<Usuario*> usuarios;

    if(!cargarUsuarios(usuarios,archivo))
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
    const string& archivo
)
{
    vector<Usuario*> usuarios;

    cargarUsuarios(usuarios,archivo);

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

    bool ok = guardarUsuarios(usuarios,archivo);

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
    const string& archivo
)
{
    vector<Usuario*> usuarios;

    if(!cargarUsuarios(usuarios,archivo))
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
        ok = guardarUsuarios(usuarios,archivo);
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
    const string& archivo
)
{
    vector<Usuario*> usuarios;

    if(!cargarUsuarios(usuarios,archivo))
    {
        return false;
    }

    for(auto it = usuarios.begin(); it != usuarios.end(); ++it)
    {
        if((*it)->getId() == idUsuario)
        {
            delete *it;

            usuarios.erase(it);

            break;
        }
    }

    bool ok = guardarUsuarios(usuarios,archivo);

    for(auto usuario : usuarios)
    {
        delete usuario;
    }

    return ok;
}

///////////////////////////////////////////////////////////
// Conversión de Tipo de Tarea
///////////////////////////////////////////////////////////

string Persistencia::tipoTareaAString(
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
    const string& texto)
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
    const vector<Tarea>& tareas,
    const string& archivo
)
{
    ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    
    // ENCABEZADO
    salida
        << "idTarea|"
        << "idMateria|"
        << "titulo|"
        << "fechaEntrega|"
        << "descripcion|"
        << "tipo|"
        << "parcial"
        << "\n";


    // TAREAS
    for(const auto& tarea : tareas)
    {
        salida
            << tarea.getId() << "|"
            << tarea.getMateriaId() << "|"
            << tarea.getTitulo() << "|"
            << tarea.getFechaEntrega() << "|"
            << tarea.getDescripcion() << "|"
            << tipoTareaAString(tarea.getTipo()) << "|"
            << tarea.getParcial()
            << "\n";
    }

    salida.close();

    return true;
}

///////////////////////////////////////////////////////////
// Cargar Tareas
///////////////////////////////////////////////////////////

bool Persistencia::cargarTareas(
    vector<Tarea>& tareas,
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    tareas.clear();

    string linea;

    // SALTAR ENCABEZADO
    if(!getline(entrada, linea))
    {
        entrada.close();
        return true;
    }

    // CARGAR TAREAS
    while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);

        string idTexto;
        string materiaIdTexto;
        string titulo;
        string fechaEntrega;
        string descripcion;
        string tipoTexto;
        string parcialTexto;

        // SEPARAR CAMPOS
        getline(ss,idTexto,'|');
        getline(ss,materiaIdTexto,'|');
        getline(ss,titulo,'|');
        getline(ss,fechaEntrega,'|');
        getline(ss,descripcion,'|');
        getline(ss,tipoTexto,'|');
        getline(ss,parcialTexto,'|');

        // VALIDAR CAMPOS
        if(idTexto.empty() || materiaIdTexto.empty() || titulo.empty() ||
            fechaEntrega.empty() || descripcion.empty() ||tipoTexto.empty() ||
            parcialTexto.empty())
        {
            continue;
        }

        // CONVERTIR IDS
        int id;
        int materiaId;
        int parcial;

        try
        {
            id = stoi(idTexto);
            materiaId = stoi(materiaIdTexto);
            parcial = stoi(parcialTexto);
        }
        catch(...)
        {
            continue;
        }

        // VALIDAR PARCIAL
        if(parcial < 0)
        {
            continue;
        }
        
        // CONVERTIR TIPO
        TipoTarea tipo = stringATipoTarea(tipoTexto);

        // CREAR TAREA
        Tarea tarea(
            id,
            materiaId,
            titulo,
            descripcion,
            fechaEntrega,
            tipo,
            parcial
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
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    int mayorId = 0;
    string linea;

    // Saltar encabezado
    if(!getline(entrada, linea))
    {
        entrada.close();
        return 1;
    }

    // Leer tareas
    while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTexto;

        // El archivo utiliza |
        getline(ss, idTexto,'|');

        if(idTexto.empty())
        {
            continue;
        }

        int id;

        try
        {
            id = stoi(idTexto);
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
// Guardar Ponderaciones
///////////////////////////////////////////////////////////

bool Persistencia::guardarPonderaciones(
    const vector<Ponderacion>& ponderaciones,
    const string& archivo
)
{
    ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    // ENCABEZADO
    salida
        << "idMateria|"
        << "parcial|"
        << "tarea|"
        << "examen|"
        << "practica|"
        << "proyecto|"
        << "trabajo|"
        << "otro"
        << "\n";
   
    // PONDERACIONES
     for(const auto& ponderacion : ponderaciones)
    {
        salida
            << ponderacion.getIdMateria()
            << "|"
            << ponderacion.getParcial()
            << "|"
            << ponderacion.getTarea()
            << "|"
            << ponderacion.getExamen()
            << "|"
            << ponderacion.getPractica()
            << "|"
            << ponderacion.getProyecto()
            << "|"
            << ponderacion.getTrabajo()
            << "|"
            << ponderacion.getOtro()
            << "\n";
    }

    salida.close();

    return true;
}


///////////////////////////////////////////////////////////
// Cargar Ponderaciones
///////////////////////////////////////////////////////////

bool Persistencia::cargarPonderaciones(
    vector<Ponderacion>& ponderaciones,
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    // LIMPIAR VECTOR
    ponderaciones.clear();

    string linea;

    // SALTAR ENCABEZADO
    if(!getline(entrada,linea))
    {
        entrada.close();

        return true;
    }
    
    // CARGAR PONDERACIONES
    while(getline(entrada,linea))
    {
        if(linea.empty())
        {
            continue;
        }

        // VARIABLES
        stringstream ss(linea);

        string idMateriaTexto;
        string parcialTexto;
        string tareaTexto;
        string examenTexto;
        string practicaTexto;
        string proyectoTexto;
        string trabajoTexto;
        string otroTexto;
        
        // SEPARAR CAMPOS
        getline(ss,idMateriaTexto,'|');
        getline(ss,parcialTexto,'|');
        getline(ss,tareaTexto,'|');
        getline(ss,examenTexto,'|');
        getline(ss,practicaTexto,'|');
        getline(ss,proyectoTexto,'|');
        getline(ss,trabajoTexto,'|');
        getline(ss,otroTexto,'|');
        
        // VALIDAR CAMPOS
       if(idMateriaTexto.empty() || parcialTexto.empty() || tareaTexto.empty() ||
            examenTexto.empty() || practicaTexto.empty() || proyectoTexto.empty() ||
            trabajoTexto.empty() || otroTexto.empty())
        {
            continue;
        }
      
        // VARIABLES CONVERTIDAS
        int idMateria;
        int parcial;
        double tarea;
        double examen;
        double practica;
        double proyecto;
        double trabajo;
        double otro;

        // CONVERTIR DATOS
        try
        {
            idMateria = stoi(idMateriaTexto);
            parcial = stoi(parcialTexto);
            tarea = stod(tareaTexto);
            examen = stod(examenTexto);
            practica = stod(practicaTexto);
            proyecto = stod(proyectoTexto);
            trabajo = stod(trabajoTexto);
            otro = stod(otroTexto);

        }
        catch(...)
        {
            // Si una línea tiene datos inválidos,
            // se ignora y se continúa con la siguiente.

            continue;
        }

        // VALIDAR DATOS
        if(idMateria <= 0 || parcial < 0 || tarea < 0 || examen < 0 || practica < 0 ||
            proyecto < 0 || trabajo < 0 || otro < 0)
        {
            continue;
        }

        // CREAR PONDERACIÓN
        Ponderacion ponderacion(
            idMateria,
            parcial,
            tarea,
            examen,
            practica,
            proyecto,
            trabajo,
            otro
        );

        // AGREGAR AL VECTOR
        ponderaciones.push_back(ponderacion);

    }


    entrada.close();

    return true;
}


///////////////////////////////////////////////////////////
// Guardar Subtareas
///////////////////////////////////////////////////////////

bool Persistencia::guardarSubtareas(
    const vector<Subtarea>& subtareas,
    const string& archivo
)
{
    ofstream salida(archivo);

    if(!salida.is_open())
    {
        return false;
    }

    // ENCABEZADO
    salida
        << "idSubtarea|"
        << "idTarea|"
        << "idAlumno|"
        << "descripcion|"
        << "estado"
        << "\n";


    
    // SUBTAREAS
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
    vector<Subtarea>& subtareas,
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    subtareas.clear();

    string linea;

    // SALTAR ENCABEZADO
    if(!getline(entrada, linea))
    {
        entrada.close();

        return true;
    }

    // LEER SUBTAREAS
    while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTexto;
        string tareaIdTexto;
        string alumnoIdTexto;
        string descripcion;
        string estadoTexto;

        // SEPARAR CAMPOS
        getline(ss,idTexto,'|');
        getline(ss,tareaIdTexto,'|');
        getline(ss,alumnoIdTexto,'|');
        getline(ss,descripcion,'|');
        std::getline(ss,estadoTexto,'|');

        // VALIDAR
        if(idTexto.empty() || tareaIdTexto.empty() || alumnoIdTexto.empty() ||
            descripcion.empty() || estadoTexto.empty())
        {
            continue;
        }

        // CONVERTIR IDS
        int id;
        int tareaId;
        int alumnoId;

        try
        {
            id = stoi(idTexto);
            tareaId = stoi(tareaIdTexto);
            alumnoId = stoi(alumnoIdTexto);
        }
        catch(...)
        {
            continue;
        }

        // CONVERTIR ESTADO
        EstadoSubtarea estado = stringAEstadoSubtarea(estadoTexto);

        // CREAR SUBTAREA
        Subtarea subtarea(
            id,
            tareaId,
            alumnoId,
            descripcion
        );

        subtarea.setEstado(estado);

        subtareas.push_back(subtarea);
    }

    entrada.close();

    return true;
}

///////////////////////////////////////////////////////////
// Generar ID de Subtarea
///////////////////////////////////////////////////////////

int Persistencia::generarIdSubtarea(
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    int mayorId = 0;
    string linea;

    // SALTAR ENCABEZADO
    if(!getline(entrada, linea))
    {
        entrada.close();

        return 1;
    }

    // LEER SUBTAREAS
    while(std::getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTexto;
        getline(ss,idTexto,'|');

        if(idTexto.empty())
        {
            continue;
        }

        int id;

        try
        {
            id = stoi(idTexto);
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
    const string& texto
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
    const string& archivo
)
{
    // Validar ID
     if(alumnoId <= 0)
    {
        return false;
    }

    // Archivo temporal
    const string archivoTemporal = archivo + ".tmp";

    // Cargar registros de otros alumnos
    vector<std::string> registros;
    ifstream entrada(archivo);

    if(entrada.is_open())
    {
        string linea;

        // Saltar encabezado
        getline(entrada,linea);

        while(getline(entrada,linea))
        {
            if(linea.empty())
            {
                continue;
            }

            stringstream ss(linea);
            string alumnoTexto;
            getline(ss,alumnoTexto,'|');

            if(alumnoTexto.empty())
            {
                continue;
            }

            int idAlumnoArchivo;

            try
            {
                idAlumnoArchivo = stoi(alumnoTexto);
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
            // Solo se conserva los registros de otros alumnos.
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


    // Abrir archivo temporal
    ofstream salida(archivoTemporal,ios::trunc);

    if(!salida.is_open())
    {
        return false;
    }

    
    // Encabezado
    salida
        << "idAlumno|"
        << "fecha|"
        << "tipo|"
        << "idElemento|"
        << "prioridad"
        << "\n";

    
    // Verificar escritura del encabezado
    if(!salida.good())
    {
        salida.close();
        remove(archivoTemporal.c_str());

        return false;
    }

    // Restaurar registros de otros alumnos
    for(const string& registro : registros)
    {
        salida
            << registro
            << "\n";

        if(!salida.good())
        {
            salida.close();
            remove(archivoTemporal.c_str());

            return false;
        }
    }

    // Guardar Planner del alumno actual
    for(int i = 0; i < 7; ++i)
    {
        const PlannerDia& dia = planner.getDia(i);
        const std::string& fecha = dia.getFecha();

        // TAREAS
        for(const TareaPlanner& tarea : dia.getTareas())
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
                << prioridadPlannerAString(tarea.prioridad)
                << "\n";

            if(!salida.good())
            {
                salida.close();
                remove(archivoTemporal.c_str());

                return false;
            }
        }

        // SUBTAREAS
        for(int idSubtarea : dia.getSubtareas())
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
                remove(archivoTemporal.c_str());

                return false;
            }
        }
    }

    
    // Cerrar archivo temporal
    salida.close();

    if(salida.fail())
    {
        remove(archivoTemporal.c_str());

        return false;
    }

    // Reemplazar archivo original
    // Eliminar archivo anterior
    if(remove(archivo.c_str()) != 0)
    {
        // Puede no existir todavía.
        // En ese caso continuamos.
    }

    // Renombrar temporal como archivo definitivo
    if(rename(archivoTemporal.c_str(), archivo.c_str()) != 0)
    {
        // Si el renombrado falla,
        // eliminar el temporal.
        remove(archivoTemporal.c_str());

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

    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    string linea;

    //==================================================
    // Saltar encabezado
    //==================================================

    if(!getline(entrada,linea))
    {
        entrada.close();
        return true;
    }

    // Leer registros
    while(getline(entrada,linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string alumnoTexto;
        string fecha;
        string tipo;
        string idElementoTexto;
        string prioridadTexto;

        
        // Separar campos
        getline(ss,alumnoTexto,'|');
        getline(ss,fecha,'|');
        getline(ss,tipo,'|');
        getline(ss,idElementoTexto,'|');
        getline(ss,prioridadTexto,'|');

        // Validar
        if(alumnoTexto.empty() || fecha.empty() || tipo.empty() ||
           idElementoTexto.empty())
        {
            continue;
        }

        // Convertir alumno
        int idAlumnoArchivo;

        try
        {
            idAlumnoArchivo = stoi(alumnoTexto);
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

        // Convertir ID
        int idElemento;

        try
        {
            idElemento = stoi(idElementoTexto);
        }
        catch(...)
        {
            continue;
        }

        // Buscar el día correspondiente
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

        // SUBTAREA
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
    const string& texto
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

string Persistencia::prioridadPlannerAString(
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

string Persistencia::estadoTareaAString(
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
    const string& estado
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
    const vector<EstadoTareaAlumno>& estados,
    const string& archivo
)
{
    ofstream salida(archivo);

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
    vector<EstadoTareaAlumno>& estados,
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    estados.clear();

    string linea;

    // Saltar encabezado
    getline(entrada,linea);

    while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTareaTexto;
        string idAlumnoTexto;
        string estadoTexto;

        getline(ss,idTareaTexto,'|');
        getline(ss,idAlumnoTexto,'|');
        getline(ss,estadoTexto,'|');

        try
        {
            int idTarea = stoi(idTareaTexto);
            int idAlumno = stoi(idAlumnoTexto);

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

string Persistencia::tipoNotificacionAString(
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
    const string& texto
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

string Persistencia::tipoReferenciaNotificacionAString(
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

        case TipoReferenciaNotificacion::SUBTAREA:
            return "SUBTAREA";
            
        case TipoReferenciaNotificacion::PROFESOR:
            return "PROFESOR";
        
    }

    return "NINGUNA";
}

///////////////////////////////////////////////////////////
// CONVERTIR STRING A TIPO REFERENCIA NOTIFICACIÓN
///////////////////////////////////////////////////////////

TipoReferenciaNotificacion Persistencia::stringATipoReferenciaNotificacion(
    const string& texto
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

    if(texto == "SUBTAREA")
    {
        return TipoReferenciaNotificacion::SUBTAREA;
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
    const vector<Notificacion>& notificaciones,
    const string& archivo
)
{
    ofstream salida(archivo);

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
    vector<Notificacion>& notificaciones,
    const std::string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return false;
    }

    notificaciones.clear();

    string linea;

    // SALTAR ENCABEZADO
    if(!getline(entrada, linea))
    {
        entrada.close();
        return true;
    }

    // LEER REGISTROS
     while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTexto;
        string idUsuarioTexto;
        string tipoTexto;
        string idReferenciaTexto;
        string tipoReferenciaTexto;
        string titulo;
        string mensaje;
        string fecha;
        string leidaTexto;

        // SEPARAR CAMPOS
        getline(ss,idTexto, '|');
        getline(ss,idUsuarioTexto,'|');
        getline(ss,tipoTexto,'|');
        getline(ss, idReferenciaTexto, '|');
        getline(ss,tipoReferenciaTexto,'|');
        getline(ss,titulo,'|');
        getline(ss,mensaje,'|');
        getline(ss,fecha,'|');
        getline(ss,leidaTexto,'|');

        // VALIDAR CAMPOS OBLIGATORIOS
        if(idTexto.empty() || idUsuarioTexto.empty() || tipoTexto.empty() ||
            idReferenciaTexto.empty() || tipoReferenciaTexto.empty() ||
            titulo.empty() || mensaje.empty() || fecha.empty())
        {
            continue;
        }

        // CONVERTIR DATOS
        try
        {
            int id = stoi(idTexto);
            int idUsuario = stoi(idUsuarioTexto);
            int idReferencia = stoi(idReferenciaTexto);
            TipoNotificacion tipo = stringATipoNotificacion(tipoTexto);
            TipoReferenciaNotificacion tipoReferencia =
                stringATipoReferenciaNotificacion(tipoReferenciaTexto);

            // CREAR NOTIFICACIÓN
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

            // RESTAURAR ESTADO DE LECTURA
            if(leidaTexto == "1")
            {
                notificacion.marcarComoLeida();
            }
            else
            {
                notificacion.marcarComoNoLeida();
            }

            // AGREGAR AL VECTOR
            notificaciones.push_back(notificacion);
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
    const string& archivo
)
{
    ifstream entrada(archivo);

    if(!entrada.is_open())
    {
        return 1;
    }

    string linea;
    getline(entrada, linea);

    int mayorId = 0;

    while(getline(entrada, linea))
    {
        if(linea.empty())
        {
            continue;
        }

        stringstream ss(linea);
        string idTexto;
        getline(ss,idTexto, '|');

        try
        {
            int id = stoi(idTexto);

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
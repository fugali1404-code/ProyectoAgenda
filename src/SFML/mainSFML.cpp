#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "SFML/login.hpp"
#include "SFML/dashboard.hpp"
#include "SFML/materiasView.hpp"
#include "SFML/tareasView.hpp"

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

#include "controllers/loginController.hpp"
#include "controllers/materiasController.hpp"
#include "controllers/tareasController.hpp"

#include "SFML/vistaActual.hpp"

#include "materia.hpp"
#include "tarea.hpp"
#include "estadoTareaAlumno.hpp"
#include "calificaciones.hpp"


int main()
{
    //-------------------------------------------------
    // Ventana
    //-------------------------------------------------

    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "Agenda Escolar"
    );


    //-------------------------------------------------
    // Login
    //-------------------------------------------------

    LoginView login;

    if(!login.cargarFuente(
        "../assets/arial.ttf"))
    {
        return 1;
    }


    //-------------------------------------------------
    // Dashboard
    //-------------------------------------------------

    DashboardView dashboard;

    if(!dashboard.cargarFuente("../assets/arial.ttf"))
    {
        return 1;
    }


    //-------------------------------------------------
    // Materias
    //-------------------------------------------------

    MateriasView materiasView;

    if(!materiasView.cargarFuente("../assets/arial.ttf"))
    {
        return 1;
    }


    //-------------------------------------------------
    // Tareas
    //-------------------------------------------------

    TareasView tareasView;

    if(!tareasView.cargarFuente("../assets/arial.ttf"))
    {
        return 1;
    }


    //-------------------------------------------------
    // Conexion al servidor
    //-------------------------------------------------

    NetworkManager network;

    if(!network.conectar(
        "127.0.0.1",
        54000))
    {
        std::cout
            << "No fue posible conectar con el servidor."
            << std::endl;
    }


    //-------------------------------------------------
    // Sesion
    //-------------------------------------------------

    SessionClient session;


    //-------------------------------------------------
    // Controller Login
    //-------------------------------------------------

    LoginController loginController(
        login,
        network,
        session
    );


    //-------------------------------------------------
    // Controller Materias
    //-------------------------------------------------

    MateriasController materiasController(
        network,
        session
    );


    //-------------------------------------------------
    // Controller Tareas
    //-------------------------------------------------

    std::vector<int> idsAlumnosTarea;

    TareasController tareasController(
        network,
        session
    );


    //-------------------------------------------------
    // Vista actual
    //-------------------------------------------------

    VistaActual vistaActual =
        VistaActual::LOGIN;


    //-------------------------------------------------
    // FUNCION PARA ACTUALIZAR MATERIAS
    //-------------------------------------------------

    auto cargarMaterias = [&]()
    {
        std::vector<Materia> materias =
            materiasController.obtenerMaterias();

        //-------------------------------------------------
        // Materias en Dashboard
        //-------------------------------------------------

        dashboard.setMaterias(
            materias
        );


        //-------------------------------------------------
        // Materias en MateriasView
        //-------------------------------------------------

        materiasView.setMaterias(
            materias
        );


        //-------------------------------------------------
        // Profesores
        //-------------------------------------------------

        materiasView.setProfesores(
            materiasController.obtenerProfesores()
        );

        tareasView.setProfesores(
            materiasController.obtenerProfesores()
        );

        //-------------------------------------------------
        // Cantidad de alumnos
        //-------------------------------------------------

        if(session.obtenerRol() == "Profesor")
        {
            std::vector<int> cantidades;

            for(const Materia& materia : materias)
            {
                std::vector<
                    MateriasController::AlumnoMateria
                > alumnos =
                    materiasController.obtenerAlumnosMateria(
                        materia.getId()
                    );

                cantidades.push_back(
                    static_cast<int>(
                        alumnos.size()
                    )
                );
            }

            materiasView.setCantidadAlumnos(
                cantidades
            );
        }
    };

    
    //-------------------------------------------------
    // FUNCION PARA CARGAR PONDERACIONES
    //-------------------------------------------------

    auto cargarPonderaciones =[&](int idMateria)
    {
        std::vector<MateriasController::PonderacionMateria> 
            ponderacionesController = materiasController.obtenerPonderaciones(idMateria);

        std::vector<MateriasView::PonderacionMateria> ponderacionesView;

        for(const auto& ponderacion : ponderacionesController)
        {

            MateriasView::PonderacionMateria dato;

           dato.parcial = ponderacion.parcial;
           dato.tarea = ponderacion.tarea;
           dato.examen = ponderacion.examen;
           dato.practica = ponderacion.practica;
           dato.proyecto = ponderacion.proyecto;
           dato.trabajo = ponderacion.trabajo;
           dato.otro = ponderacion.otro;

           ponderacionesView.push_back(dato);
        }

        materiasView.setPonderaciones(ponderacionesView);


    };
    
    
    
    //-------------------------------------------------
    // FUNCION PARA ACTUALIZAR TAREAS
    //-------------------------------------------------

    auto cargarTareas = [&]()
    {
        //-------------------------------------------------
        // Cargar tareas desde servidor
        //-------------------------------------------------

        if(!tareasController.cargarTareas())
        {
            tareasView.limpiarTareas();
            return;
        }


        //-------------------------------------------------
        // Obtener tareas de la materia seleccionada
        //-------------------------------------------------

        int idMateria = tareasView.obtenerMateriaSeleccionada();

        if(idMateria == -1)
        {
            tareasView.limpiarTareas();
            return;
        }


        tareasView.setTareas(
            tareasController.obtenerTareasMateria(
                idMateria
            )
        );


        //-------------------------------------------------
        // Estados del alumno
        //-------------------------------------------------

        tareasView.limpiarEstados();

        
        if(session.obtenerRol() == "Alumno")
        {
    std::vector<EstadoTareaAlumno> estados;

    if(tareasController.cargarEstadosAlumno(
        EstadoTarea::COMPLETADO))
    {
        const auto completados =
            tareasController.obtenerEstadosTarea();

        for(const auto& estado : completados)
        {
            for(const auto& tarea :
                tareasController.obtenerTareasMateria(
                    idMateria))
            {
                if(
                    estado.getTareaId() ==
                    tarea.getId()
                )
                {
                    estados.push_back(estado);
                    break;
                }
            }
        }
    }

    if(tareasController.cargarEstadosAlumno(EstadoTarea::NO_COMPLETADO))
    {
        const auto noCompletados =
            tareasController.obtenerEstadosTarea();

        for(const auto& estado :
            noCompletados)
        {
            bool existe = false;

            for(const auto& existente : estados)
            {
                if(
                    existente.getTareaId() ==
                    estado.getTareaId()
                )
                {
                    existe = true;
                    break;
                }
            }

            if(!existe)
            {
                for(const auto& tarea :
                    tareasController.obtenerTareasMateria(
                        idMateria))
                {
                    if(
                        estado.getTareaId() ==
                        tarea.getId()
                    )
                    {
                        estados.push_back(estado);
                        break;
                    }
                }
            }
        }
    }

    tareasView.setEstadosTarea(estados);

    //-------------------------------------------------
    // CARGAR CALIFICACIONES DEL ALUMNO
    //-------------------------------------------------

    try
    {
        int idAlumno = -1;

if(
    tareasController.obtenerIdAlumnoActual(
        idAlumno
    )
)
{
    if(
        tareasController.cargarCalificacionesAlumno(
            idAlumno
        )
    )
    {
        tareasView.setCalificaciones(
            tareasController.obtenerCalificaciones()
        );
    }
    else
    {
        tareasView.limpiarCalificaciones();
    }
}
else
{
    tareasView.limpiarCalificaciones();
}
    }
    catch(...)
    {
        tareasView.limpiarCalificaciones();
    }
}
    };


    //-------------------------------------------------
    // BUCLE PRINCIPAL
    //-------------------------------------------------

    while(window.isOpen())
    {
        while(const auto event =
            window.pollEvent())
        {

            //-------------------------------------------------
            // CERRAR VENTANA
            //-------------------------------------------------

            if(event->is<
                sf::Event::Closed>())
            {
                window.close();
            }


            //-------------------------------------------------
            // LOGIN
            //-------------------------------------------------

            if(vistaActual ==
                VistaActual::LOGIN)
            {
                loginController.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // BOTON LOGIN
                //-------------------------------------------------

                if(loginController.loginPresionado(
                    window,
                    *event))
                {
                    std::string correo =
                        login.obtenerUsuario();

                    std::string password =
                        login.obtenerPassword();


                    //-------------------------------------------------
                    // PROCESAR LOGIN
                    //-------------------------------------------------

                    if(loginController.procesarLogin(
                        correo,
                        password))
                    {

                        //-------------------------------------------------
                        // Dashboard
                        //-------------------------------------------------

                        dashboard.setAlumno(
                            session.obtenerNombre(),
                            session.obtenerIdentificador()
                        );

                        dashboard.setRol(
                            session.obtenerRol()
                        );


                        //-------------------------------------------------
                        // Materias
                        //-------------------------------------------------

                        materiasView.setRol(
                            session.obtenerRol()
                        );


                        //-------------------------------------------------
                        // Tareas
                        //-------------------------------------------------

                        //cargarMaterias();
                        tareasView.setRol(
                            session.obtenerRol()
                        );

                        //-------------------------------------------------
                        // Seleccionar primera materia
                        //-------------------------------------------------

                        std::vector<Materia> materias =
                            materiasController.obtenerMaterias();

                        tareasView.setProfesores(
                            materiasController.obtenerProfesores()
                        );

                        if(!materias.empty())
                        {
                            tareasView.setMaterias(materias);

                            tareasView.setMateriaSeleccionada(materias[0].getId());

                        }
                        else
                        {
                            tareasView.setMaterias(
                                materias
                            );
                        }


                        //-------------------------------------------------
                        // Cargar tareas
                        //-------------------------------------------------

                        cargarTareas();


                        //-------------------------------------------------
                        // Cambiar a Dashboard
                        //-------------------------------------------------

                        vistaActual =
                            VistaActual::DASHBOARD;
                    }
                }
            }


            //-------------------------------------------------
            // DASHBOARD
            //-------------------------------------------------

            else if(vistaActual ==
                    VistaActual::DASHBOARD)
            {
                dashboard.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // MATERIAS
                //
                // Se mantiene M como acceso a materias.
                //-------------------------------------------------

                if(event->is<
                    sf::Event::KeyPressed>())
                {
                    const auto* tecla =
                        event->getIf<
                            sf::Event::KeyPressed>();


                    if(tecla != nullptr)
                    {
                        //-------------------------------------------------
                        // MATERIAS
                        //-------------------------------------------------

                        if(tecla->code ==
                            sf::Keyboard::Key::M)
                        {
                            cargarMaterias();

                            vistaActual =
                                VistaActual::MATERIAS;
                        }


                        //-------------------------------------------------
                        // TAREAS
                        //-------------------------------------------------

                        else if(tecla->code ==
                            sf::Keyboard::Key::T)
                        {
                            cargarMaterias();

                            std::vector<Materia> materias =
                                materiasController.obtenerMaterias();

                            tareasView.setMaterias(
                                materias
                            );

                            if(!materias.empty())
                            {
                                tareasView.setMateriaSeleccionada(
                                    materias[0].getId()
                                );

                                cargarTareas();
                            }
                            else
                            {
                                tareasView.limpiarTareas();
                            }

                            vistaActual =
                                VistaActual::TAREAS;
                        }
                    }
                }
            }


            //-------------------------------------------------
            // MATERIAS
            //-------------------------------------------------

            else if(vistaActual == VistaActual::MATERIAS)
            {


                //-------------------------------------------------
                // Eventos de la vista
                //-------------------------------------------------

                materiasView.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // REGRESAR
                //-------------------------------------------------

                if(materiasView.botonRegresarPresionado(
                    window,
                    *event))
                {
                    materiasView.cerrarFormularios();

                    vistaActual = VistaActual::DASHBOARD;
                }


                //-------------------------------------------------
                // AGREGAR
                //-------------------------------------------------

                if(materiasView.botonAgregarPresionado(
                    window,
                    *event))
                {
                    // Solo abre formulario.
                }


                //-------------------------------------------------
                // GUARDAR AGREGAR
                //-------------------------------------------------

                if(materiasView.botonGuardarAgregarPresionado(window, *event))
                {
                    std::string nombre = materiasView.obtenerNuevaMateria();

                    if(materiasController.agregarMateria(nombre))
                    {
                        cargarMaterias();

                        materiasView.limpiarNuevaMateria();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR AGREGAR
                //-------------------------------------------------

                if(materiasView.botonCancelarAgregarPresionado(window, *event))
                {
                    materiasView.limpiarNuevaMateria();

                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // EDITAR
                //-------------------------------------------------

                if(materiasView.botonEditarPresionado(window, *event))
                {
                    // Solo abre formulario.
                }


                //-------------------------------------------------
                // GUARDAR EDITAR
                //-------------------------------------------------

                if(materiasView.botonGuardarEditarPresionado(window, *event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    std::string nombre = materiasView.obtenerNombreEditar();


                    if(materiasController.actualizarMateria(idMateria,nombre))
                    {
                        cargarMaterias();

                        materiasView.limpiarEditarMateria();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR EDITAR
                //-------------------------------------------------

                if(materiasView.botonCancelarEditarPresionado(window, *event))
                {
                    materiasView.limpiarEditarMateria();

                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonEliminarPresionado(window,*event))
                {
                    // Solo abre confirmacion.
                }


                //-------------------------------------------------
                // CONFIRMAR ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonConfirmarEliminarPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();


                    if(materiasController.eliminarMateria(idMateria))
                    {
                        cargarMaterias();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonCancelarEliminarPresionado( window,*event))
                {
                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // VER ALUMNOS
                //-------------------------------------------------

                if(materiasView.botonAlumnosPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();


                    std::vector< MateriasController::AlumnoMateria>alumnosController 
                        = materiasController.obtenerAlumnosMateria(idMateria);


                    std::vector<MateriasView::AlumnoMateria> alumnosView;


                    for(const auto& alumno : alumnosController)
                    {
                        MateriasView::AlumnoMateria alumnoView;

                        alumnoView.id = alumno.id;

                        alumnoView.nombre = alumno.nombre;

                        alumnoView.identificador = alumno.identificador;

                        alumnosView.push_back(alumnoView);

                    }


                    materiasView.setAlumnosMateria(alumnosView);
                
                }


                //-------------------------------------------------
                // INSCRIBIR
                //-------------------------------------------------

                if(materiasView.botonInscribirPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    std::string boletas = materiasView.obtenerBoletasAlumno();


                    if( idMateria != -1 && !boletas.empty())
                    {
                        if(materiasController.inscribirAlumno(idMateria,boletas))
                        {
                            std::vector<
                                MateriasController::AlumnoMateria
                            > alumnosController =
                                materiasController.obtenerAlumnosMateria(
                                    idMateria
                                );


                            std::vector<MateriasView::AlumnoMateria> alumnosView;


                            for(const auto& alumno : alumnosController)
                            {
                                MateriasView::AlumnoMateria alumnoView;

                                alumnoView.id = alumno.id;

                                alumnoView.nombre = alumno.nombre;

                                alumnoView.identificador = alumno.identificador;

                                alumnosView.push_back( alumnoView );
                            }


                            materiasView.setAlumnosMateria(alumnosView);

                            materiasView.limpiarBoletasAlumno();

                            cargarMaterias();
                        }
                    }
                }


                //-------------------------------------------------
                // DESINSCRIBIR
                //-------------------------------------------------

                if(materiasView.botonDesinscribirPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();

                    std::string boletas =
                        materiasView.obtenerBoletasSeleccionadas();


                    if(
                        idMateria != -1 &&
                        !boletas.empty()
                    )
                    {
                        if(materiasController.desinscribirAlumno(
                            idMateria,
                            boletas))
                        {
                            std::vector<
                                MateriasController::AlumnoMateria
                            > alumnosController =
                                materiasController.obtenerAlumnosMateria(
                                    idMateria
                                );


                            std::vector<
                                MateriasView::AlumnoMateria
                            > alumnosView;


                            for(const auto& alumno :
                                alumnosController)
                            {
                                MateriasView::AlumnoMateria alumnoView;

                                alumnoView.id =
                                    alumno.id;

                                alumnoView.nombre =
                                    alumno.nombre;

                                alumnoView.identificador =
                                    alumno.identificador;

                                alumnosView.push_back(
                                    alumnoView
                                );
                            }


                            materiasView.setAlumnosMateria(
                                alumnosView
                            );

                            materiasView.limpiarAlumnosSeleccionados();

                            cargarMaterias();
                        }
                    }
                }


                //-------------------------------------------------
                // PONDERACIONES
                //-------------------------------------------------

               if(materiasView.botonPonderacionesPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    if(idMateria != -1)
                    {
                        cargarPonderaciones(idMateria);
                    }
                }

                //-------------------------------------------------
                // CARGAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonCargarPonderacionPresionado(window,*event))
                {
                    // La vista carga la ponderacion
                    // seleccionada en sus campos.
                }

                //-------------------------------------------------
                // GUARDAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonGuardarPonderacionPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    MateriasView::PonderacionMateria ponderacion;

                    if(idMateria != -1 && materiasView.obtenerPonderacion(ponderacion))
                    {
                        if(materiasController.configurarPonderacion(
                               idMateria,
                               ponderacion.parcial,
                               ponderacion.tarea,
                               ponderacion.examen,
                               ponderacion.practica,
                               ponderacion.proyecto,
                               ponderacion.trabajo,
                               ponderacion.otro))
                        {
                            //-------------------------------------------------
                            // Recargar ponderaciones
                            //-------------------------------------------------

                            cargarPonderaciones(idMateria);

                            //-------------------------------------------------
                            // Cerrar ventana
                            //-------------------------------------------------

                           materiasView.cerrarFormularios();
                        }
                    }
                }
            
                //-------------------------------------------------
                // ELIMINAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonEliminarPonderacionPresionado(window,*event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                   MateriasView::PonderacionMateria ponderacion;

                   if(idMateria != -1 && materiasView.obtenerPonderacion(ponderacion))
                   {
                        if(materiasController.eliminarPonderacion(
                            idMateria,ponderacion.parcial))
                        {
                           //-------------------------------------------------
                           // Recargar ponderaciones
                           //-------------------------------------------------

                            cargarPonderaciones(idMateria);

                           //-------------------------------------------------
                           // Cerrar ventana
                           //-------------------------------------------------

                           materiasView.cerrarFormularios();
                        }
                    }
                }
                
                //-------------------------------------------------
                // CANCELAR PONDERACIONES
                //-------------------------------------------------

                if(materiasView.botonCancelarPonderacionPresionado(window,*event))
                {
                    materiasView.cerrarFormularios();
                }


                //------------------------------------------------
                // Ver informción
                //------------------------------------------------

                if(materiasView.botonInformacionPresionado(window, *event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    if(idMateria != -1)
                    {
                        cargarPonderaciones(idMateria);
                    }
                }

                //------------------------------------------------
                // Ver calificación final
                //------------------------------------------------

                if(materiasView.botonCalificacionFinalPresionado(window, *event))
                {
                    int idMateria = materiasView.obtenerIdMateriaSeleccionada();

                    int idAlumno;

                    if(idMateria != -1 &&
                        tareasController.obtenerIdAlumnoActual(idAlumno))
                    {
                        
                        double calificacionFinal;

                        if(materiasController.obtenerCalificacionFinal(
                            idAlumno, idMateria,calificacionFinal))
                        {
                            
                            materiasView.setCalificacionFinal(calificacionFinal);
                        }
                    }
                }

                //------------------------------------------------
                // Cerrar ver información
                //------------------------------------------------

                if(materiasView.botonCerrarInformacionPresionado(window, *event))
                {
                    materiasView.cerrarFormularios();
                }
                
                //-------------------------------------------------
                // CERRAR ALUMNOS
                //-------------------------------------------------

                if(materiasView.botonCerrarAlumnosPresionado(window,*event))
                {
                    materiasView.cerrarFormularios();
                }
            }


            //-------------------------------------------------
            // TAREAS
            //-------------------------------------------------

            else if(vistaActual ==
                    VistaActual::TAREAS)
            {
                //-------------------------------------------------
                // Eventos internos de TareasView
                //-------------------------------------------------

                tareasView.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // REGRESAR AL DASHBOARD
                //-------------------------------------------------

                if(tareasView.regresarPresionado(
                    window,
                    *event))
                {
                    tareasView.cerrarFormularios();

                    vistaActual =
                        VistaActual::DASHBOARD;
                }


                //-------------------------------------------------
                // CAMBIAR MATERIA
                //-------------------------------------------------
                //
                // Los botones < y > son manejados visualmente
                // por TareasView.
                //
                // Aquí detectamos el cambio comparando la
                // materia seleccionada.
                //-------------------------------------------------

                static int ultimaMateria =
                    -1;

                int materiaActual =
                    tareasView.obtenerMateriaSeleccionada();

                if(materiaActual != ultimaMateria)
                {
                    ultimaMateria =
                        materiaActual;

                    cargarTareas();
                }


                //-------------------------------------------------
                // AGREGAR TAREA
                //-------------------------------------------------

                if(tareasView.agregarPresionado())
                {
                    // Solo abre formulario.
                }


                //-------------------------------------------------
                // GUARDAR AGREGAR TAREA
                //-------------------------------------------------

                if(tareasView.guardarAgregarPresionado(
                    window,
                    *event))
                {
                    std::string titulo =
                        tareasView.obtenerTituloAgregar();

                    std::string fecha =
                        tareasView.obtenerFechaAgregar();

                    std::string descripcion =
                        tareasView.obtenerDescripcionAgregar();

                    std::string tipoTexto =
                        tareasView.obtenerTipoAgregar();

                    std::string parcialTexto =
                        tareasView.obtenerParcialAgregar();


                    TipoTarea tipo =
                        TareasController::stringATipoTarea(
                            tipoTexto
                        );


                    int parcial = 0;

                    try
                    {
                        parcial =
                            std::stoi(
                                parcialTexto
                            );
                    }
                    catch(...)
                    {
                        parcial = 0;
                    }


                    if(tareasController.agregarTarea(
                        tareasView.obtenerMateriaSeleccionada(),
                        titulo,
                        fecha,
                        descripcion,
                        tipo,
                        parcial))
                    {
                        cargarTareas();

                        tareasView.limpiarAgregar();

                        tareasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR AGREGAR
                //-------------------------------------------------

                if(tareasView.cancelarAgregarPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarAgregar();

                    tareasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // EDITAR TAREA
                //-------------------------------------------------

                int idTareaEditar =
                    tareasView.editarPresionado(
                        window,
                        *event
                    );

                if(idTareaEditar != -1)
                {
                    // El formulario ya fue abierto por la vista.
                }


                //-------------------------------------------------
                // GUARDAR EDITAR TAREA
                //-------------------------------------------------

                if(tareasView.guardarEditarPresionado(
                    window,
                    *event))
                {
                    int idTarea =
                        tareasView.obtenerTareaEditando();

                    std::string titulo =
                        tareasView.obtenerTituloEditar();

                    std::string fecha =
                        tareasView.obtenerFechaEditar();

                    std::string descripcion =
                        tareasView.obtenerDescripcionEditar();

                    std::string tipoTexto =
                        tareasView.obtenerTipoEditar();

                    std::string parcialTexto =
                        tareasView.obtenerParcialEditar();


                    TipoTarea tipo =
                        TareasController::stringATipoTarea(
                            tipoTexto
                        );


                    int parcial = 0;

                    try
                    {
                        parcial =
                            std::stoi(
                                parcialTexto
                            );
                    }
                    catch(...)
                    {
                        parcial = 0;
                    }


                    if(tareasController.editarTarea(
                        idTarea,
                        titulo,
                        fecha,
                        descripcion,
                        tipo,
                        parcial))
                    {
                        cargarTareas();

                        tareasView.limpiarEditar();

                        tareasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR EDITAR
                //-------------------------------------------------

                if(tareasView.cancelarEditarPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarEditar();

                    tareasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // ELIMINAR TAREA
                //-------------------------------------------------

                int idTareaEliminar =
                    tareasView.eliminarPresionado(
                        window,
                        *event
                    );

                if(idTareaEliminar != -1)
                {
                    // Solo abre confirmacion.
                }


                //-------------------------------------------------
                // CONFIRMAR ELIMINAR
                //-------------------------------------------------

                if(tareasView.confirmarEliminarPresionado(
                    window,
                    *event))
                {
                    int idTarea =
                        tareasView.obtenerTareaEliminar();


                    if(tareasController.eliminarTarea(
                        idTarea))
                    {
                        cargarTareas();

                        tareasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR ELIMINAR
                //-------------------------------------------------

                if(tareasView.cancelarEliminarPresionado(
                    window,
                    *event))
                {
                    tareasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // VER ALUMNOS / ESTADOS
                //-------------------------------------------------

                int idTareaEstados = tareasView.alumnosPresionado(window,*event);

                if(idTareaEstados != -1)
                {
                    //-------------------------------------------------
                    // Obtener alumnos inscritos en la materia
                    //-------------------------------------------------

                    int idMateria = tareasView.obtenerMateriaSeleccionada();

                    std::vector<MateriasController::AlumnoMateria> alumnosMateria =
                        materiasController.obtenerAlumnosMateria(idMateria);

                    std::vector<TareasView::AlumnoTarea> alumnosTarea;

                    for(const auto& alumno : alumnosMateria)
                    {
                        TareasView::AlumnoTarea dato;

                        dato.id = alumno.id;

                        dato.nombre = alumno.nombre;

                        dato.identificador = alumno.identificador;

                        alumnosTarea.push_back(dato);
                    }

                    tareasView.setAlumnosTarea(alumnosTarea);

                    //-------------------------------------------------
                    // Obtener estados de los alumnos
                    //-------------------------------------------------

                    if(tareasController.cargarEstadosTarea(idTareaEstados))
                    {
                        const auto estados = tareasController.obtenerEstadosTarea();

                        tareasView.setEstadosTarea(estados);

                        //-------------------------------------------------
                        // Obtener IDs de alumnos
                        //-------------------------------------------------

                        idsAlumnosTarea.clear();

                        for(const auto& estado : estados)
                        {
                            int idAlumno = estado.getAlumnoId();

                            bool existe = false;

                            for(int id : idsAlumnosTarea)
                            {
                                if(id == idAlumno)
                                {
                                    existe = true;
                                    break;
                                }
                            }

                            if(!existe)
                            {
                                idsAlumnosTarea.push_back(idAlumno);
                            }
                        }

                        //-------------------------------------------------
                        // Cargar calificaciones
                        //-------------------------------------------------

                        tareasController.cargarCalificacionesTarea(
                            idTareaEstados,idsAlumnosTarea);

                        tareasView.setCalificaciones(
                            tareasController.obtenerCalificaciones());
                    }
                    else
                    {
                        tareasView.limpiarEstados();
                        tareasView.limpiarCalificaciones();
                    }
                }

                //-------------------------------------------------
                // CERRAR VENTANA DE ESTADOS
                //-------------------------------------------------

                if(tareasView.cerrarEstadosPresionado(window,*event))
                {
                    tareasView.limpiarEstados();

                    tareasView.limpiarAlumnosTarea();

                    tareasView.limpiarCalificacion();

                    tareasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // COMPLETAR TAREA
                //-------------------------------------------------

                int idTareaCompletar = tareasView.completarPresionado(window,*event);

                if(idTareaCompletar != -1)
                {
                    if(tareasController.cambiarEstadoTarea(
                        idTareaCompletar,
                        EstadoTarea::COMPLETADO))
                    {
                        cargarTareas();
                    }
                }


                //-------------------------------------------------
                // NO COMPLETAR TAREA
                //-------------------------------------------------

                int idTareaNoCompletar =
                    tareasView.noCompletarPresionado(
                        window,
                        *event
                    );

                if(idTareaNoCompletar != -1)
                {
                    if(tareasController.cambiarEstadoTarea(
                        idTareaNoCompletar,
                        EstadoTarea::NO_COMPLETADO))
                    {
                        cargarTareas();
                    }
                }


                //-------------------------------------------------
                // AGREGAR CALIFICACION
                //-------------------------------------------------

                int alumnoAgregarCalificacion = tareasView.agregarCalificacionPresionado(
                        window, *event);

                if(alumnoAgregarCalificacion != -1)
                {
                    // La vista abre el formulario.
                    // El ID del alumno queda guardado
                    // dentro de TareasView.
                }


                //-------------------------------------------------
                // GUARDAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.guardarAgregarCalificacionPresionado(window,*event))
                {
                    int idAlumno = tareasView.obtenerAlumnoCalificacion();

                    int idTarea =
                        tareasView.obtenerTareaCalificacion();

                    std::string textoCalificacion =
                        tareasView.obtenerCalificacionAgregar();


                    double calificacion = 0.0;

                    try
                    {
                        calificacion =
                            std::stod(
                                textoCalificacion
                            );
                    }
                    catch(...)
                    {
                        calificacion = -1.0;
                    }


                    if(
                        idAlumno != -1 &&
                        idTarea != -1 &&
                        calificacion >= 0.0
                    )
                    {
                        if(tareasController.agregarCalificacion(
                            idAlumno,
                            idTarea,
                            calificacion))
                        {
                            //-------------------------------------------------
                            // Recargar calificacion de la tarea
                            //-------------------------------------------------

                            tareasController.cargarCalificacionesTarea(
                                idTarea,
                                idsAlumnosTarea
                            );


                            tareasView.setCalificaciones(
                                tareasController.obtenerCalificaciones()
                            );


                            tareasView.limpiarCalificacion();

                            
                        }
                    }
                }


                //-------------------------------------------------
                // CANCELAR AGREGAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.cancelarAgregarCalificacionPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarCalificacion();

                }


                //-------------------------------------------------
                // EDITAR CALIFICACION
                //-------------------------------------------------

                int alumnoEditarCalificacion = tareasView.editarCalificacionPresionado(
                        window,*event);

                if(alumnoEditarCalificacion != -1)
                {
                    // La vista abre el formulario.
                }


                //-------------------------------------------------
                // GUARDAR EDITAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.guardarEditarCalificacionPresionado(window,*event))
                {
                    int idAlumno = tareasView.obtenerAlumnoCalificacionEditar();

                    int idTarea = tareasView.obtenerTareaCalificacionEditar();

                    std::string textoCalificacion = tareasView.obtenerCalificacionEditar();


                    double calificacion = 0.0;

                    try
                    {
                        calificacion = std::stod(textoCalificacion);

                    }
                    catch(...)
                    {
                        calificacion = -1.0;
                    }


                    if(
                        idAlumno != -1 &&
                        idTarea != -1 &&
                        calificacion >= 0.0
                    )
                    {
                        if(tareasController.editarCalificacion(
                            idAlumno,
                            idTarea,
                            calificacion))
                        {
                            //-------------------------------------------------
                            // Recargar calificaciones
                            //-------------------------------------------------

                            tareasController.cargarCalificacionesTarea(
                                idTarea,idsAlumnosTarea);


                            tareasView.setCalificaciones(
                                tareasController.obtenerCalificaciones());


                            tareasView.limpiarCalificacion();

                            
                        }
                    }
                }


                //-------------------------------------------------
                // CANCELAR EDITAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.cancelarEditarCalificacionPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarCalificacion();

                }


                //-------------------------------------------------
                // ELIMINAR CALIFICACION
                //-------------------------------------------------

                int alumnoEliminarCalificacion =
                    tareasView.eliminarCalificacionPresionado(window,*event);

                if(alumnoEliminarCalificacion != -1)
                {
                    // Solo abre confirmacion.
                }


                //-------------------------------------------------
                // CONFIRMAR ELIMINAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.confirmarEliminarCalificacionPresionado(window,*event))
                {
                    int idAlumno = tareasView.obtenerAlumnoCalificacionEliminar();

                    int idTarea = tareasView.obtenerTareaCalificacionEliminar();


                    if(idAlumno != -1 && idTarea != -1)
                    {
                        if(tareasController.eliminarCalificacion(idAlumno,idTarea))
                        {
                            tareasController.cargarCalificacionesTarea(
                                idTarea,idsAlumnosTarea);


                            tareasView.setCalificaciones(
                                tareasController.obtenerCalificaciones());


                            tareasView.limpiarCalificacion();

                            
                        }
                    }
                }


                //-------------------------------------------------
                // CANCELAR ELIMINAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.cancelarEliminarCalificacionPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarCalificacion();

                }
            }
        }


        //-------------------------------------------------
        // DIBUJAR
        //-------------------------------------------------

        window.clear(
            sf::Color(240, 240, 240)
        );


        //-------------------------------------------------
        // LOGIN
        //-------------------------------------------------

        if(vistaActual ==
            VistaActual::LOGIN)
        {
            login.draw(
                window
            );
        }


        //-------------------------------------------------
        // DASHBOARD
        //-------------------------------------------------

        else if(vistaActual ==
            VistaActual::DASHBOARD)
        {
            dashboard.draw(
                window
            );
        }


        //-------------------------------------------------
        // MATERIAS
        //-------------------------------------------------

        else if(vistaActual ==
            VistaActual::MATERIAS)
        {
            materiasView.draw(
                window
            );
        }


        //-------------------------------------------------
        // TAREAS
        //-------------------------------------------------

        else if(vistaActual ==
            VistaActual::TAREAS)
        {
            tareasView.draw(
                window
            );
        }


        //-------------------------------------------------
        // Mostrar ventana
        //-------------------------------------------------

        window.display();
    }


    //-------------------------------------------------
    // Desconectar
    //-------------------------------------------------

    network.desconectar();


    return 0;
}
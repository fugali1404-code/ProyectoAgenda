#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "SFML/login.hpp"
#include "SFML/dashboard.hpp"
#include "SFML/materiasView.hpp"
#include "SFML/tareasView.hpp"
#include "SFML/plannerView.hpp"
#include "SFML/notificacionesView.hpp"
#include "SFML/usuariosView.hpp"

#include "networkmanager.hpp"
#include "SFML/sessioncliente.hpp"

#include "controllers/loginController.hpp"
#include "controllers/materiasController.hpp"
#include "controllers/tareasController.hpp"
#include "controllers/plannerController.hpp"
#include "controllers/notificacionesController.hpp"
#include "controllers/usuariosController.hpp"

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

    if(!login.cargarFuente("../assets/arial.ttf"))
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
    // USUARIOS
    //-------------------------------------------------

    UsuariosView usuariosView;

    if(!usuariosView.cargarFuente("../assets/arial.ttf"))
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
    // Planner
    //-------------------------------------------------

    PlannerView plannerView;

    if(!plannerView.cargarFuente("../assets/arial.ttf"))
    {
        return 1;
    }


    //-------------------------------------------------
    // Notificaciones
    //-------------------------------------------------

    NotificacionesView notificacionesView;

    if(!notificacionesView.cargarFuente("../assets/arial.ttf"))
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
    // Controller Usuarios
    //-------------------------------------------------

    UsuariosController usuariosController(
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
    // Controller Planner
    //-------------------------------------------------

    PlannerController plannerController(
        network,
        session
    );


    //-------------------------------------------------
    // Controller Notificaciones
    //-------------------------------------------------

    NotificacionesController notificacionesController(
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

    auto cargarPonderaciones = [&](int idMateria)
    {
        std::vector<
            MateriasController::PonderacionMateria
        > ponderacionesController =
            materiasController.obtenerPonderaciones(
                idMateria
            );

        std::vector<
            MateriasView::PonderacionMateria
        > ponderacionesView;


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

        materiasView.setPonderaciones(
            ponderacionesView
        );
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
        // Obtener materia seleccionada
        //-------------------------------------------------

        int idMateria =
            tareasView.obtenerMateriaSeleccionada();

        if(idMateria == -1)
        {
            tareasView.limpiarTareas();
            return;
        }


        //-------------------------------------------------
        // Cargar tareas
        //-------------------------------------------------

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


            //-------------------------------------------------
            // Completados
            //-------------------------------------------------

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
                        if(estado.getTareaId() ==
                           tarea.getId())
                        {
                            estados.push_back(estado);
                            break;
                        }
                    }
                }
            }


            //-------------------------------------------------
            // No completados
            //-------------------------------------------------

            if(tareasController.cargarEstadosAlumno(
                EstadoTarea::NO_COMPLETADO))
            {
                const auto noCompletados =
                    tareasController.obtenerEstadosTarea();

                for(const auto& estado : noCompletados)
                {
                    bool existe = false;

                    for(const auto& existente : estados)
                    {
                        if(existente.getTareaId() ==
                           estado.getTareaId())
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
                            if(estado.getTareaId() ==
                               tarea.getId())
                            {
                                estados.push_back(estado);
                                break;
                            }
                        }
                    }
                }
            }


            tareasView.setEstadosTarea(
                estados
            );


            //-------------------------------------------------
            // Calificaciones del alumno
            //-------------------------------------------------

            try
            {
                int idAlumno = -1;

                if(tareasController.obtenerIdAlumnoActual(
                    idAlumno))
                {
                    if(tareasController.cargarCalificacionesAlumno(
                        idAlumno))
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
    // FUNCION PARA ACTUALIZAR PLANNER
    //-------------------------------------------------

    auto cargarPlanner = [&]()
    {
        if(session.obtenerRol() != "Alumno")
        {
            return false;
        }

        if(!tareasController.cargarTareas() ||
           !plannerController.recargar())
        {
            return false;
        }

        plannerView.setPlanner(
            plannerController.obtenerPlanner()
        );

        plannerView.setSubtareas(
            plannerController.obtenerSubtareas()
        );

        plannerView.setTareas(
            tareasController.obtenerTareas()
        );

        plannerView.setMaterias(
            materiasController.obtenerMaterias()
        );


        std::vector<
            PlannerView::EstadoTareaPlanner
        > estadosPlanner;


        for(const auto& estado :
            plannerController.obtenerEstadosTareas())
        {
            PlannerView::EstadoTareaPlanner estadoView;

            estadoView.idTarea = estado.idTarea;
            estadoView.estado = estado.estado;

            estadosPlanner.push_back(
                estadoView
            );
        }


        plannerView.setEstadosTareas(
            estadosPlanner
        );

        return true;
    };


    //-------------------------------------------------
    // FUNCION PARA ACTUALIZAR NOTIFICACIONES
    //-------------------------------------------------

    auto cargarNotificaciones = [&]()
    {
        if(!notificacionesController.cargarNotificaciones())
        {
            notificacionesView.limpiarNotificaciones();
            return false;
        }

        tareasController.cargarTareas();


        notificacionesView.setNotificaciones(
            notificacionesController.obtenerNotificaciones()
        );


        notificacionesView.setTareas(
            tareasController.obtenerTareas()
        );


        notificacionesView.setMaterias(
            materiasController.obtenerMaterias()
        );

        return true;
    };


    //-------------------------------------------------
    // BUCLE PRINCIPAL
    //-------------------------------------------------

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
        {
            //-------------------------------------------------
            // CERRAR VENTANA
            //-------------------------------------------------

            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }


            //-------------------------------------------------
            // LOGIN
            //-------------------------------------------------

            if(vistaActual == VistaActual::LOGIN)
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
                        // ADMINISTRADOR
                        //-------------------------------------------------

                        if(session.obtenerRol() ==
                           "Administrador")
                        {
                            //-------------------------------------------------
                            // Cargar usuarios
                            //-------------------------------------------------

                            if(usuariosController.cargarUsuarios())
                            {
                                usuariosView.setUsuarios(
                                    usuariosController.obtenerUsuarios()
                                );
                            }
                            else
                            {
                                usuariosView.setUsuarios(
                                    {}
                                );
                            }


                            //-------------------------------------------------
                            // Entrar directamente a Usuarios
                            //-------------------------------------------------

                            vistaActual =
                                VistaActual::USUARIOS;

                            continue;
                        }


                        //-------------------------------------------------
                        // DASHBOARD
                        // Alumno / Profesor
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

                        tareasView.setRol(
                            session.obtenerRol()
                        );


                        //-------------------------------------------------
                        // Cargar materias
                        //-------------------------------------------------

                        std::vector<Materia> materias =
                            materiasController.obtenerMaterias();


                        //-------------------------------------------------
                        // Dashboard
                        //-------------------------------------------------

                        dashboard.setMaterias(
                            materias
                        );


                        //-------------------------------------------------
                        // Tareas - profesores
                        //-------------------------------------------------

                        tareasView.setProfesores(
                            materiasController.obtenerProfesores()
                        );


                        //-------------------------------------------------
                        // Primera materia
                        //-------------------------------------------------

                        if(!materias.empty())
                        {
                            tareasView.setMaterias(
                                materias
                            );

                            tareasView.setMateriaSeleccionada(
                                materias[0].getId()
                            );
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
                        // Entrar al Dashboard
                        //-------------------------------------------------

                        vistaActual =
                            VistaActual::DASHBOARD;
                    }
                }
            }


            //-------------------------------------------------
            // USUARIOS
            //-------------------------------------------------

            //-------------------------------------------------
// USUARIOS
//-------------------------------------------------
else if(vistaActual == VistaActual::USUARIOS)
{
    usuariosView.manejarEvento(
        *event,
        window
    );


    //-------------------------------------------------
    // AGREGAR
    //-------------------------------------------------

    if(usuariosView.botonAgregarPresionado(
        window,
        *event))
    {
        // UsuariosView abre el formulario.
    }


    //-------------------------------------------------
    // EDITAR
    //-------------------------------------------------

    int indiceEditar = -1;

    if(usuariosView.botonEditarPresionado(
        window,
        *event,
        indiceEditar))
    {
        // UsuariosView abre el formulario.
    }


    //-------------------------------------------------
    // ELIMINAR
    //-------------------------------------------------

    int indiceEliminar = -1;

    if(usuariosView.botonEliminarPresionado(
        window,
        *event,
        indiceEliminar))
    {
        // UsuariosView solamente muestra
        // la ventana de confirmación.
    }


    //-------------------------------------------------
    // CONFIRMACIÓN DE ELIMINACIÓN
    //-------------------------------------------------

    if(usuariosView.confirmacionAceptada())
    {
        int indice =
            usuariosView.obtenerUsuarioEliminar();

        const auto& usuarios =
            usuariosController.obtenerUsuarios();

        if(indice >= 0 &&
           indice < static_cast<int>(usuarios.size()))
        {
            int idUsuario =
                usuarios[indice].id;

            if(usuariosController.eliminarUsuario(
                idUsuario))
            {
                usuariosController.recargar();

                usuariosView.setUsuarios(
                    usuariosController.obtenerUsuarios()
                );

                usuariosView.mostrarMensaje(
                    "Usuario eliminado correctamente."
                );
            }
            else
            {
                usuariosView.mostrarMensaje(
                    "No se pudo eliminar el usuario."
                );
            }
        }
    }


    //-------------------------------------------------
    // CANCELAR ELIMINACIÓN
    //-------------------------------------------------

    if(usuariosView.confirmacionCancelada())
    {
        // La ventana de confirmación ya fue cerrada
        // por UsuariosView.
    }


    //-------------------------------------------------
    // FORMULARIO ACEPTADO
    //-------------------------------------------------

    if(usuariosView.formularioAceptado())
    {
        const UsuarioDatos& datos =
            usuariosView.obtenerDatosFormulario();


        //-------------------------------------------------
        // EDITAR USUARIO
        //-------------------------------------------------

        if(usuariosView.formularioEsEdicion())
        {
            if(usuariosController.actualizarUsuario(
                datos.id,
                datos.nombre,
                datos.correo,
                datos.password,
                datos.identificador))
            {
                usuariosController.recargar();

                usuariosView.setUsuarios(
                    usuariosController.obtenerUsuarios()
                );

                usuariosView.mostrarMensaje(
                    "Usuario actualizado correctamente."
                );

                usuariosView.cerrarFormulario();
            }
            else
            {
                usuariosView.mostrarMensaje(
                    "No se pudo actualizar el usuario."
                );
            }
        }


        //-------------------------------------------------
        // AGREGAR USUARIO
        //-------------------------------------------------

        else
        {
            if(usuariosController.agregarUsuario(
                datos.rol,
                datos.nombre,
                datos.correo,
                datos.password,
                datos.identificador))
            {
                usuariosController.recargar();

                usuariosView.setUsuarios(
                    usuariosController.obtenerUsuarios()
                );

                usuariosView.mostrarMensaje(
                    "Usuario agregado correctamente."
                );

                usuariosView.cerrarFormulario();
            }
            else
            {
                usuariosView.mostrarMensaje(
                    "No se pudo agregar el usuario."
                );
            }
        }
    }


    //-------------------------------------------------
    // CANCELAR FORMULARIO
    //-------------------------------------------------

    if(usuariosView.formularioCancelado())
    {
        usuariosView.cerrarFormulario();
    }
}


            //-------------------------------------------------
            // DASHBOARD
            //-------------------------------------------------

            else if(vistaActual == VistaActual::DASHBOARD)
            {
                dashboard.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // BOTON MATERIAS
                //-------------------------------------------------

                if(dashboard.botonMateriasPresionado(
                    window,
                    *event))
                {
                    cargarMaterias();

                    vistaActual =
                        VistaActual::MATERIAS;
                }


                //-------------------------------------------------
                // BOTON TAREAS
                //-------------------------------------------------

                else if(dashboard.botonTareasPresionado(
                    window,
                    *event))
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


                //-------------------------------------------------
                // BOTON PLANNER
                //-------------------------------------------------

                else if(dashboard.botonPlannerPresionado(
                    window,
                    *event))
                {
                    if(session.obtenerRol() == "Alumno")
                    {
                        if(cargarPlanner())
                        {
                            vistaActual =
                                VistaActual::PLANNER;
                        }
                    }
                }


                //-------------------------------------------------
                // BOTON NOTIFICACIONES
                //-------------------------------------------------

                else if(dashboard.botonNotificacionesPresionado(
                    window,
                    *event))
                {
                    if(session.obtenerRol() == "Alumno")
                    {
                        if(cargarNotificaciones())
                        {
                            vistaActual =
                                VistaActual::NOTIFICACIONES;
                        }
                    }
                }
            }


            //-------------------------------------------------
            // MATERIAS
            //-------------------------------------------------

            else if(vistaActual == VistaActual::MATERIAS)
            {
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

                    vistaActual =
                        VistaActual::DASHBOARD;
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

                if(materiasView.botonGuardarAgregarPresionado(
                    window,
                    *event))
                {
                    std::string nombre =
                        materiasView.obtenerNuevaMateria();


                    if(materiasController.agregarMateria(
                        nombre))
                    {
                        cargarMaterias();

                        materiasView.limpiarNuevaMateria();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR AGREGAR
                //-------------------------------------------------

                if(materiasView.botonCancelarAgregarPresionado(
                    window,
                    *event))
                {
                    materiasView.limpiarNuevaMateria();

                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // EDITAR
                //-------------------------------------------------

                if(materiasView.botonEditarPresionado(
                    window,
                    *event))
                {
                    // Solo abre formulario.
                }


                //-------------------------------------------------
                // GUARDAR EDITAR
                //-------------------------------------------------

                if(materiasView.botonGuardarEditarPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();

                    std::string nombre =
                        materiasView.obtenerNombreEditar();


                    if(materiasController.actualizarMateria(
                        idMateria,
                        nombre))
                    {
                        cargarMaterias();

                        materiasView.limpiarEditarMateria();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR EDITAR
                //-------------------------------------------------

                if(materiasView.botonCancelarEditarPresionado(
                    window,
                    *event))
                {
                    materiasView.limpiarEditarMateria();

                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonEliminarPresionado(
                    window,
                    *event))
                {
                    // Solo abre confirmacion.
                }


                //-------------------------------------------------
                // CONFIRMAR ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonConfirmarEliminarPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    if(materiasController.eliminarMateria(
                        idMateria))
                    {
                        cargarMaterias();

                        materiasView.cerrarFormularios();
                    }
                }


                //-------------------------------------------------
                // CANCELAR ELIMINAR
                //-------------------------------------------------

                if(materiasView.botonCancelarEliminarPresionado(
                    window,
                    *event))
                {
                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // VER ALUMNOS
                //-------------------------------------------------

                if(materiasView.botonAlumnosPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    std::vector<
                        MateriasController::AlumnoMateria
                    > alumnosController =
                        materiasController.obtenerAlumnosMateria(
                            idMateria
                        );


                    std::vector<
                        MateriasView::AlumnoMateria
                    > alumnosView;


                    for(const auto& alumno : alumnosController)
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
                }


                //-------------------------------------------------
                // VER CALIFICACIONES ALUMNOS
                //-------------------------------------------------

                if(materiasView.botonCalificacionesAlumnosPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    if(idMateria != -1)
                    {
                        std::vector<
                            MateriasController::AlumnoMateria
                        > alumnosController =
                            materiasController.obtenerAlumnosMateria(
                                idMateria
                            );


                        std::vector<
                            MateriasView::CalificacionAlumno
                        > calificacionesView;


                        for(const auto& alumno :
                            alumnosController)
                        {
                            MateriasView::CalificacionAlumno dato;

                            dato.idAlumno =
                                alumno.id;

                            dato.nombre =
                                alumno.nombre;

                            dato.identificador =
                                alumno.identificador;

                            dato.calificacion =
                                0.0;

                            dato.tieneCalificacion =
                                false;

                            calificacionesView.push_back(
                                dato
                            );
                        }


                        materiasView.setCalificacionesAlumnos(
                            calificacionesView
                        );
                    }
                }


                //-------------------------------------------------
                // INSCRIBIR
                //-------------------------------------------------

                if(materiasView.botonInscribirPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();

                    std::string boletas =
                        materiasView.obtenerBoletasAlumno();


                    if(idMateria != -1 &&
                       !boletas.empty())
                    {
                        if(materiasController.inscribirAlumno(
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


                    if(idMateria != -1 &&
                       !boletas.empty())
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

                if(materiasView.botonPonderacionesPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    if(idMateria != -1)
                    {
                        cargarPonderaciones(
                            idMateria
                        );
                    }
                }


                //-------------------------------------------------
                // CARGAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonCargarPonderacionPresionado(
                    window,
                    *event))
                {
                    // La vista carga la ponderacion.
                }


                //-------------------------------------------------
                // GUARDAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonGuardarPonderacionPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    MateriasView::PonderacionMateria ponderacion;


                    if(idMateria != -1 &&
                       materiasView.obtenerPonderacion(
                           ponderacion))
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
                            cargarPonderaciones(
                                idMateria
                            );

                            materiasView.cerrarFormularios();
                        }
                    }
                }


                //-------------------------------------------------
                // ELIMINAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonEliminarPonderacionPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    MateriasView::PonderacionMateria ponderacion;


                    if(idMateria != -1 &&
                       materiasView.obtenerPonderacion(
                           ponderacion))
                    {
                        if(materiasController.eliminarPonderacion(
                            idMateria,
                            ponderacion.parcial))
                        {
                            cargarPonderaciones(
                                idMateria
                            );

                            materiasView.cerrarFormularios();
                        }
                    }
                }


                //-------------------------------------------------
                // CANCELAR PONDERACION
                //-------------------------------------------------

                if(materiasView.botonCancelarPonderacionPresionado(
                    window,
                    *event))
                {
                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // INFORMACION
                //-------------------------------------------------

                if(materiasView.botonInformacionPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();


                    if(idMateria != -1)
                    {
                        cargarPonderaciones(
                            idMateria
                        );
                    }
                }


                //-------------------------------------------------
                // CALIFICACION FINAL
                //-------------------------------------------------

                if(materiasView.botonCalificacionFinalPresionado(
                    window,
                    *event))
                {
                    int idMateria =
                        materiasView.obtenerIdMateriaSeleccionada();

                    int idAlumno;


                    if(idMateria != -1 &&
                       tareasController.obtenerIdAlumnoActual(
                           idAlumno))
                    {
                        double calificacionFinal;


                        if(materiasController.obtenerCalificacionFinal(
                            idAlumno,
                            idMateria,
                            calificacionFinal))
                        {
                            materiasView.setCalificacionFinal(
                                calificacionFinal
                            );
                        }
                    }
                }


                //-------------------------------------------------
                // CERRAR INFORMACION
                //-------------------------------------------------

                if(materiasView.botonCerrarInformacionPresionado(
                    window,
                    *event))
                {
                    materiasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // CERRAR ALUMNOS
                //-------------------------------------------------

                if(materiasView.botonCerrarAlumnosPresionado(
                    window,
                    *event))
                {
                    materiasView.cerrarFormularios();
                }
            }


            //-------------------------------------------------
            // TAREAS
            //-------------------------------------------------

            else if(vistaActual == VistaActual::TAREAS)
            {
                tareasView.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // REGRESAR
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

                static int ultimaMateria = -1;

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
                            std::stoi(parcialTexto);
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
                    // El formulario ya fue abierto.
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
                            std::stoi(parcialTexto);
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

                int idTareaEstados =
                    tareasView.alumnosPresionado(
                        window,
                        *event
                    );


                if(idTareaEstados != -1)
                {
                    int idMateria =
                        tareasView.obtenerMateriaSeleccionada();


                    std::vector<
                        MateriasController::AlumnoMateria
                    > alumnosMateria =
                        materiasController.obtenerAlumnosMateria(
                            idMateria
                        );


                    std::vector<
                        TareasView::AlumnoTarea
                    > alumnosTarea;


                    for(const auto& alumno :
                        alumnosMateria)
                    {
                        TareasView::AlumnoTarea dato;

                        dato.id =
                            alumno.id;

                        dato.nombre =
                            alumno.nombre;

                        dato.identificador =
                            alumno.identificador;

                        alumnosTarea.push_back(
                            dato
                        );
                    }


                    tareasView.setAlumnosTarea(
                        alumnosTarea
                    );


                    //-------------------------------------------------
                    // Estados
                    //-------------------------------------------------

                    if(tareasController.cargarEstadosTarea(
                        idTareaEstados))
                    {
                        const auto estados =
                            tareasController.obtenerEstadosTarea();


                        tareasView.setEstadosTarea(
                            estados
                        );


                        //-------------------------------------------------
                        // IDs alumnos
                        //-------------------------------------------------

                        idsAlumnosTarea.clear();


                        for(const auto& estado :
                            estados)
                        {
                            int idAlumno =
                                estado.getAlumnoId();


                            bool existe = false;


                            for(int id :
                                idsAlumnosTarea)
                            {
                                if(id == idAlumno)
                                {
                                    existe = true;
                                    break;
                                }
                            }


                            if(!existe)
                            {
                                idsAlumnosTarea.push_back(
                                    idAlumno
                                );
                            }
                        }


                        //-------------------------------------------------
                        // Calificaciones
                        //-------------------------------------------------

                        tareasController.cargarCalificacionesTarea(
                            idTareaEstados,
                            idsAlumnosTarea
                        );


                        tareasView.setCalificaciones(
                            tareasController.obtenerCalificaciones()
                        );
                    }
                    else
                    {
                        tareasView.limpiarEstados();

                        tareasView.limpiarCalificaciones();
                    }
                }


                //-------------------------------------------------
                // CERRAR ESTADOS
                //-------------------------------------------------

                if(tareasView.cerrarEstadosPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarEstados();

                    tareasView.limpiarAlumnosTarea();

                    tareasView.limpiarCalificacion();

                    tareasView.cerrarFormularios();
                }


                //-------------------------------------------------
                // COMPLETAR
                //-------------------------------------------------

                int idTareaCompletar =
                    tareasView.completarPresionado(
                        window,
                        *event
                    );


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
                // NO COMPLETAR
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

                int alumnoAgregarCalificacion =
                    tareasView.agregarCalificacionPresionado(
                        window,
                        *event
                    );


                if(alumnoAgregarCalificacion != -1)
                {
                    // La vista abre el formulario.
                }


                //-------------------------------------------------
                // GUARDAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.guardarAgregarCalificacionPresionado(
                    window,
                    *event))
                {
                    int idAlumno =
                        tareasView.obtenerAlumnoCalificacion();

                    int idTarea =
                        tareasView.obtenerTareaCalificacion();

                    std::string textoCalificacion =
                        tareasView.obtenerCalificacionAgregar();


                    double calificacion = 0.0;


                    try
                    {
                        calificacion =
                            std::stod(textoCalificacion);
                    }
                    catch(...)
                    {
                        calificacion = -1.0;
                    }


                    if(idAlumno != -1 &&
                       idTarea != -1 &&
                       calificacion >= 0.0)
                    {
                        if(tareasController.agregarCalificacion(
                            idAlumno,
                            idTarea,
                            calificacion))
                        {
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

                int alumnoEditarCalificacion =
                    tareasView.editarCalificacionPresionado(
                        window,
                        *event
                    );


                if(alumnoEditarCalificacion != -1)
                {
                    // La vista abre el formulario.
                }


                //-------------------------------------------------
                // GUARDAR EDITAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.guardarEditarCalificacionPresionado(
                    window,
                    *event))
                {
                    int idAlumno =
                        tareasView.obtenerAlumnoCalificacionEditar();

                    int idTarea =
                        tareasView.obtenerTareaCalificacionEditar();

                    std::string textoCalificacion =
                        tareasView.obtenerCalificacionEditar();


                    double calificacion = 0.0;


                    try
                    {
                        calificacion =
                            std::stod(textoCalificacion);
                    }
                    catch(...)
                    {
                        calificacion = -1.0;
                    }


                    if(idAlumno != -1 &&
                       idTarea != -1 &&
                       calificacion >= 0.0)
                    {
                        if(tareasController.editarCalificacion(
                            idAlumno,
                            idTarea,
                            calificacion))
                        {
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
                    tareasView.eliminarCalificacionPresionado(
                        window,
                        *event
                    );


                if(alumnoEliminarCalificacion != -1)
                {
                    // Solo abre confirmacion.
                }


                //-------------------------------------------------
                // CONFIRMAR ELIMINAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.confirmarEliminarCalificacionPresionado(
                    window,
                    *event))
                {
                    int idAlumno =
                        tareasView.obtenerAlumnoCalificacionEliminar();

                    int idTarea =
                        tareasView.obtenerTareaCalificacionEliminar();


                    if(idAlumno != -1 &&
                       idTarea != -1)
                    {
                        if(tareasController.eliminarCalificacion(
                            idAlumno,
                            idTarea))
                        {
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
                // CANCELAR ELIMINAR CALIFICACION
                //-------------------------------------------------

                if(tareasView.cancelarEliminarCalificacionPresionado(
                    window,
                    *event))
                {
                    tareasView.limpiarCalificacion();
                }
            }


            //-------------------------------------------------
            // PLANNER
            //-------------------------------------------------

            else if(vistaActual == VistaActual::PLANNER)
            {
                plannerView.manejarEvento(
                    *event,
                    window
                );


                if(plannerView.regresarPresionado(
                    *event,
                    window))
                {
                    vistaActual =
                        VistaActual::DASHBOARD;
                }


                const PlannerView::Accion accion =
                    plannerView.obtenerAccion();


                bool operacionCorrecta = false;


                switch(accion.tipo)
                {
                    case PlannerView::TipoAccion::CAMBIAR_PRIORIDAD:

                        operacionCorrecta =
                            plannerController.cambiarPrioridadTarea(
                                accion.idTarea,
                                accion.prioridad
                            );

                        break;


                    case PlannerView::TipoAccion::AGREGAR_SUBTAREA:

                        operacionCorrecta =
                            plannerController.agregarSubtarea(
                                accion.idTarea,
                                accion.descripcion
                            );

                        break;


                    case PlannerView::TipoAccion::EDITAR_SUBTAREA:

                        operacionCorrecta =
                            plannerController.editarSubtarea(
                                accion.idSubtarea,
                                accion.descripcion
                            );

                        break;


                    case PlannerView::TipoAccion::ELIMINAR_SUBTAREA:

                        operacionCorrecta =
                            plannerController.eliminarSubtarea(
                                accion.idSubtarea
                            );

                        break;


                    case PlannerView::TipoAccion::CAMBIAR_ESTADO_SUBTAREA:

                        operacionCorrecta =
                            plannerController.cambiarEstadoSubtarea(
                                accion.idSubtarea,
                                accion.estado
                            );

                        break;


                    case PlannerView::TipoAccion::COLOCAR_SUBTAREA:

                        operacionCorrecta =
                            plannerController.agregarSubtareaPlanner(
                                accion.idSubtarea,
                                accion.fecha
                            );

                        break;


                    case PlannerView::TipoAccion::QUITAR_SUBTAREA:

                        operacionCorrecta =
                            plannerController.eliminarSubtareaPlanner(
                                accion.idSubtarea,
                                accion.fecha
                            );

                        break;


                    case PlannerView::TipoAccion::NINGUNA:

                        break;
                }


                plannerView.limpiarAccion();


                if(operacionCorrecta)
                {
                    cargarPlanner();
                }
            }


            //-------------------------------------------------
            // NOTIFICACIONES
            //-------------------------------------------------

            else if(vistaActual == VistaActual::NOTIFICACIONES)
            {
                notificacionesView.manejarEvento(
                    *event,
                    window
                );


                //-------------------------------------------------
                // REGRESAR
                //-------------------------------------------------

                if(notificacionesView.botonRegresarPresionado(
                    window,
                    *event))
                {
                    vistaActual =
                        VistaActual::DASHBOARD;
                }


                //-------------------------------------------------
                // MARCAR COMO LEIDA
                //-------------------------------------------------

                int indiceLeer = -1;


                if(notificacionesView.botonMarcarLeidaPresionado(
                    window,
                    *event,
                    indiceLeer))
                {
                    const auto& notificaciones =
                        notificacionesView.obtenerNotificaciones();


                    if(indiceLeer >= 0 &&
                       indiceLeer <
                       static_cast<int>(notificaciones.size()))
                    {
                        int idNotificacion =
                            notificaciones[indiceLeer].getId();


                        if(notificacionesController.marcarComoLeida(
                            idNotificacion))
                        {
                            cargarNotificaciones();
                        }
                    }
                }


                //-------------------------------------------------
                // ELIMINAR
                //-------------------------------------------------

                int indiceEliminar = -1;


                if(notificacionesView.botonEliminarPresionado(
                    window,
                    *event,
                    indiceEliminar))
                {
                    const auto& notificaciones =
                        notificacionesView.obtenerNotificaciones();


                    if(indiceEliminar >= 0 &&
                       indiceEliminar <
                       static_cast<int>(notificaciones.size()))
                    {
                        int idNotificacion =
                            notificaciones[indiceEliminar].getId();


                        if(notificacionesController.eliminarNotificacion(
                            idNotificacion))
                        {
                            cargarNotificaciones();
                        }
                    }
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

        if(vistaActual == VistaActual::LOGIN)
        {
            login.draw(
                window
            );
        }


        //-------------------------------------------------
        // USUARIOS
        //-------------------------------------------------

        else if(vistaActual == VistaActual::USUARIOS)
        {
            usuariosView.draw(
                window
            );
        }


        //-------------------------------------------------
        // DASHBOARD
        //-------------------------------------------------

        else if(vistaActual == VistaActual::DASHBOARD)
        {
            dashboard.draw(
                window
            );
        }


        //-------------------------------------------------
        // MATERIAS
        //-------------------------------------------------

        else if(vistaActual == VistaActual::MATERIAS)
        {
            materiasView.draw(
                window
            );
        }


        //-------------------------------------------------
        // TAREAS
        //-------------------------------------------------

        else if(vistaActual == VistaActual::TAREAS)
        {
            tareasView.draw(
                window
            );
        }


        //-------------------------------------------------
        // PLANNER
        //-------------------------------------------------

        else if(vistaActual == VistaActual::PLANNER)
        {
            plannerView.draw(
                window
            );
        }


        //-------------------------------------------------
        // NOTIFICACIONES
        //-------------------------------------------------

        else if(vistaActual == VistaActual::NOTIFICACIONES)
        {
            notificacionesView.draw(
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
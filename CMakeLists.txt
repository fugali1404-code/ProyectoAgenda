cmake_minimum_required(VERSION 3.10)

project(AgendaAcademica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================
# Directorios de inclusión
# ==========================

include_directories(
    include
)

# ==========================
# SFML (MSYS2)
# ==========================

set(
    CMAKE_PREFIX_PATH
    "C:/msys64/ucrt64"
)

find_package(
    SFML 3 REQUIRED
    COMPONENTS
        Graphics
        Window
        System
)

# ==========================
# Ejecutable principal
# ==========================

add_executable(
    AgendaAcademica

    # Main
    src/SFML/mainSFML.cpp

    # Modelo
    src/usuario.cpp
    src/alumno.cpp
    src/profesor.cpp
    src/administrador.cpp

    src/subtarea.cpp
    src/tarea.cpp
    src/materia.cpp

    src/plannerDia.cpp
    src/plannerSemana.cpp

    # Red
    src/cliente.cpp
    src/servidor.cpp
    src/networkmanager.cpp
    src/protocolo.cpp
    src/commandprocessor.cpp
    src/sessionmaneger.cpp
    src/sessiondata.cpp

    # Persistencia
    src/persistencia.cpp
    src/notificaciones.cpp
    src/mensaje.cpp

    # SFML
    src/SFML/login.cpp
    src/SFML/dashboard.cpp
    src/SFML/planner.cpp
    src/SFML/textbox.cpp
)

# ==========================
# Librerías del proyecto principal
# ==========================

target_link_libraries(
    AgendaAcademica
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        ws2_32
)

# ==========================
# Ejecutable de prueba: servidor
# ==========================

add_executable(
    PruebaServidor

    src/pruebas/mainServidor.cpp

    src/servidor.cpp
    src/protocolo.cpp
    src/commandprocessor.cpp
    src/sessionmaneger.cpp
    src/sessiondata.cpp

    # Persistencia
    src/persistencia.cpp

    # Modelo
    src/usuario.cpp
    src/alumno.cpp
    src/profesor.cpp
    src/administrador.cpp

    src/subtarea.cpp
    src/tarea.cpp
    src/materia.cpp

    src/plannerDia.cpp
    src/plannerSemana.cpp
)

target_link_libraries(
    PruebaServidor
    PRIVATE
        ws2_32
)

# ==========================
# Ejecutable de prueba: cliente
# ==========================

add_executable(
    PruebaCliente
    src/pruebas/mainCliente.cpp
    src/cliente.cpp
)

target_link_libraries(
    PruebaCliente
    PRIVATE
        ws2_32
)


# ==========================
# Ejecutable de prueba: network
# ==========================

add_executable(
    PruebaNetwork

    src/pruebas/mainNetwork.cpp

    src/networkmanager.cpp
    src/cliente.cpp
    src/protocolo.cpp

    src/commandprocessor.cpp
    src/sessionmaneger.cpp
    src/sessiondata.cpp

    # Persistencia
    src/persistencia.cpp

    # Modelo
    src/usuario.cpp
    src/alumno.cpp
    src/profesor.cpp
    src/administrador.cpp

    src/subtarea.cpp
    src/tarea.cpp
    src/materia.cpp

    src/plannerDia.cpp
    src/plannerSemana.cpp
)

target_link_libraries(
    PruebaNetwork
    PRIVATE
        ws2_32
)



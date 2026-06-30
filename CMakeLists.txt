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

    src/subtarea.cpp
    src/tarea.cpp
    src/materia.cpp

    src/plannerDia.cpp
    src/plannerSemana.cpp

    # Red
    src/cliente.cpp
    src/servidor.cpp
    #src/networkmaneger.cpp

    # Persistencia
    src/persistencia.cpp
    src/notificaciones.cpp
    src/mensaje.cpp

    # SFML
    src/SFML/login.cpp
    src/SFML/dashboard.cpp
    src/SFML/planner.cpp
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
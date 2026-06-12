cmake_minimum_required(VERSION 3.10)

project(AgendaAcademica)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(

    AgendaAcademica

    src/mainPrueba.cpp

    src/usuario.cpp
    src/alumno.cpp
    src/profesor.cpp

    src/subtarea.cpp
    src/tarea.cpp

    src/materia.cpp

    src/plannerDia.cpp
    src/plannerSemana.cpp
)
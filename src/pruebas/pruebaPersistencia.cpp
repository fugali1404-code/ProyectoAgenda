#include <iostream>

#include "sessiondata.hpp"

int main()
{
    std::cout << "=====================================\n";
    std::cout << "      PRUEBA DE PERSISTENCIA\n";
    std::cout << "=====================================\n\n";

    //------------------------------------------------
    // Primera sesión
    //------------------------------------------------

    std::cout << "Creando primera sesion...\n";

    SessionData sesion1;

    sesion1.setAutenticado(true);

    std::cout << "Agregando materias...\n";

    sesion1.agregarMateria("Programacion Avanzada");
    sesion1.agregarMateria("Redes");
    sesion1.agregarMateria("Bases de Datos");

    std::cout << "\nMaterias en memoria:\n";

    std::cout
        << sesion1.obtenerMaterias()
        << std::endl;

    std::cout
        << "\nLos datos fueron guardados.\n";

    //------------------------------------------------
    // Segunda sesión
    //------------------------------------------------

    std::cout
        << "\n-------------------------------------\n";

    std::cout
        << "Creando una NUEVA sesion...\n";

    SessionData sesion2;

    sesion2.setAutenticado(true);

    std::cout
        << "\nMaterias cargadas desde el archivo:\n";

    std::cout
        << sesion2.obtenerMaterias()
        << std::endl;

    std::cout
        << "\n=====================================\n";

    std::cout
        << "Si aparecen las tres materias,\n";

    std::cout
        << "la persistencia funciona correctamente.\n";

    std::cout
        << "=====================================\n";

    return 0;
}
#include <iostream>
#include <SFML/Graphics.hpp> 

#include "SFML/login.hpp" 
#include "SFML/dashboard.hpp" 
#include "networkmanager.hpp" 
#include "protocolo.hpp"

int main(){ 
    sf::RenderWindow window( sf::VideoMode({1280,720}), "Agenda Escolar" );
    
    //---------------------------------
    // Login 
    //--------------------------------- 
    LoginView login; 
    
    if(!login.cargarFuente("../assets/arial.ttf")) 
        { 
            return 1; 
        }

    //--------------------------------- 
    // Dashboard 
    //--------------------------------- 
    DashboardView dashboard; 
    
    if(!dashboard.cargarFuente("../assets/arial.ttf")) 
    { 
        return 1; 
    } 
    
    
    //--------------------------------- 
    // Estado de la aplicación 
    //--------------------------------- 
    
    bool mostrarLogin = true;

    //--------------------------------- 
    // Conexion al serevidor 
    //--------------------------------- 
    NetworkManager network; 
    std::string respuesta; 
    
    if(!network.conectar("127.0.0.1",54000)) 
    { 
        std::cout << "No fue posible conectar con el servidor."<< std::endl; 
    }

    //---------------------------------
    // Bucle principal
    //---------------------------------

    while(window.isOpen())
    {
        while(const auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        

            //---------------------------------
            // LOGIN
            //---------------------------------

            if(!mostrarLogin)
            {
                dashboard.manejarEvento(*event, window);
            }

            if(!mostrarLogin)
            {
                if(dashboard.botonAgregarPresionado(window))
                {
                    std::string nombre = dashboard.obtenerNuevaMateria();

                    if(!nombre.empty())
                    {
                        std::string respuesta = network.enviarComando("ADD_MATERIA|" + nombre);

                        if(respuesta == "MATERIA_AGREGADA")
                        {
                            dashboard.limpiarNuevaMateria();
                        }
                    }
                }
            }

            if(mostrarLogin)
            {
                login.manejarEvento(*event, window);

                if(login.loginPresionado(window,*event))
                {
                    std::string correo = login.obtenerUsuario();
                    std::string password = login.obtenerPassword();
                    
                    respuesta = network.enviarComando("LOGIN|" + correo + "|" + password);

                    auto datos = Protocol::dividir(respuesta);

                    if(!datos.empty() && datos[0] == "LOGIN_OK")
                    {
                        login.setMensaje("Login correcto");

                        //---------------------------------
                        // Nombre
                        //---------------------------------

                        if(datos.size() >= 2)
                        {
                            std::string nombre = datos[1];

                            std::string boleta = "";

                            if(datos.size() >= 3)
                            {
                                boleta = datos[2];
                            }

                            dashboard.setAlumno(nombre, boleta);
                        }

                        mostrarLogin = false;
                    }
                    else
                    {
                        login.setMensaje("Correo o contraseña incorrectos");

                    }
                }
            }
        }

        //---------------------------------
        // Dibujar
        //---------------------------------

        window.clear(sf::Color(240,240,240));

        if(mostrarLogin)
        {
         login.draw(window);
        }

        else
        {
            dashboard.draw(window);
        }

        window.display();
    }   

return 0;
}
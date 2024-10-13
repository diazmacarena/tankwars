#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

class INterface {
private:
    sf::RenderWindow ventana;
    sf::RectangleShape barraProgreso;
    sf::Text textoCargando;
    sf::Font fuente;
    float progreso;

public:
    // Constructor
    INterface() : ventana(sf::VideoMode(800, 600), "Pantalla de Carga"), progreso(0.0f) {
        // Configuración de la barra de progreso
        barraProgreso.setSize(sf::Vector2f(400, 30));
        barraProgreso.setFillColor(sf::Color::Green);
        barraProgreso.setPosition(200, 300);

        // Cargar una fuente para el texto
        if (!fuente.loadFromFile("Extra Days.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente 'Extra Days.ttf'" << std::endl;
        }

        // Configurar el texto de "Cargando..."
        textoCargando.setFont(fuente);
        textoCargando.setString("Cargando...");
        textoCargando.setCharacterSize(30);
        textoCargando.setFillColor(sf::Color::White);
        textoCargando.setPosition(300, 250);
    }

    // Ejecutar la pantalla de carga y luego iniciar el juego
    void ejecutar() {
        while (ventana.isOpen()) {
            procesarEventos();
            actualizar();
            renderizar();

            // Simular carga incrementando el progreso
            progreso += 0.05f;

            // Si la barra de progreso está llena, iniciar el juego
            if (progreso >= 400.0f) {
                ventana.close();
                iniciarJuego();  // Llamar al método para iniciar el juego
            }
        }
    }

private:
    // Procesar los eventos (como cerrar la ventana)
    void procesarEventos() {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
            }
        }
    }

    // Actualizar la barra de progreso
    void actualizar() {
        barraProgreso.setSize(sf::Vector2f(progreso, 30));
    }

    // Renderizar la pantalla de carga
    void renderizar() {
        ventana.clear();
        ventana.draw(barraProgreso);
        ventana.draw(textoCargando);
        ventana.display();
    }

    // Método para iniciar el juego después de la pantalla de carga
    void iniciarJuego() {
        // Crear una instancia del juego
        Game juego;
        juego.run();  // Iniciar el juego
    }
};

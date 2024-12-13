#include "GameInterface.h"
#include "Game.h"
#include <string>
#include <iostream>

GameInterface::GameInterface() : currentOption(0), player1TankType(0), player2TankType(0), indexColors(0) {
    // Carga de la fuente
    if (!font.loadFromFile("Extra Days.ttf")) {
        std::cerr << "Error al cargar la fuente Extra Days.ttf\n";
    }

    // Inicializar el título
    Title.setFont(font); // Carga la fuente
    Title.setString("TANK WARS"); // Titulo del juego
    Title.setCharacterSize(100); // Establece el tamaño
    Title.setPosition(600, 50); // Ubica el titulo 
    Title.setFillColor(sf::Color::White); // Establecer color inicial

    // Crear el vector de los colores para el cambio 
    titleColors = { 
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan
    };

    indexColors = 0; // Iniciar en el primer color

    // Reiniciar el reloj de cambio de color
    CambioColorClock.restart();

    // Opciones del menú
    
    //Vector que contiene las opciones del menu
    std::vector<std::string> options = {"Seleccion de niveles", "Como se juega", "Salir"};
    //Dibujar las partes del menu y ponerle color a la opcion seleccionada
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(options[i], font, 80);
        text.setPosition(500, 350 + i * 200);
        text.setFillColor(i == currentOption ? sf::Color::Green : sf::Color::White);
        menuOptions.push_back(text);
    }

    // Cargar los sonidos
    if (!moveBuffer.loadFromFile("move_interface.wav")) {
        std::cerr << "Error al cargar move_interface.wav\n";
    } else {
        moveSound.setBuffer(moveBuffer);
        moveSound.setVolume(50); // Ajustar volumen si es necesario
    }

    if (!selectBuffer.loadFromFile("select.wav")) {
        std::cerr << "Error al cargar select.wav\n";
    } else {
        selectSound.setBuffer(selectBuffer);
        selectSound.setVolume(50);
    }

    if (!gameStartBuffer.loadFromFile("game_start.wav")) {
        std::cerr << "Error al cargar game_start.wav\n";
    } else {
        gameStartSound.setBuffer(gameStartBuffer);
        gameStartSound.setVolume(70);
    }
}

void GameInterface::run() {
    // Dibuja la ventana con sus dimensiones y el titulo del menú
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "TankWars Menu");
    
    // Bucle principal de la interface
    while (window.isOpen()) {
        eventos(window);
        window.clear();
        dibujarMenu(window);
        window.display();
    }
}

void GameInterface::eventos(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close(); //Cerrar la ventana si le dan a cerrar
        }
        //Para cambiar niveles   
        else if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Up) {
                int previousOption = currentOption;
                currentOption = (currentOption - 1 + menuOptions.size()) % menuOptions.size();
                if (currentOption != previousOption) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            } else if (event.key.code == sf::Keyboard::Down) {
                int previousOption = currentOption;
                currentOption = (currentOption + 1) % menuOptions.size();
                if (currentOption != previousOption) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Reproducir sonido de selección
                selectSound.play();
                if (currentOption == 0) {
                    // Reproducir sonido de inicio del juego
                    gameStartSound.play();
                    seleccionarNivel(window);
                } else if (currentOption == 1) {
                    instrucciones(window);
                } else if (currentOption == 2) {
                    window.close();
                }
            }
        }
    }
}

void GameInterface::dibujarMenu(sf::RenderWindow &window) {
    // Dibujar el texto de bienvenida
    sf::Text Welcome("Welcome to...", font, 25);
    Welcome.setPosition(570, 35);
    Welcome.setFillColor(sf::Color::White);
    window.draw(Welcome);

    // Actualizar el color del título si ha pasado el tiempo
    if (CambioColorClock.getElapsedTime().asSeconds() >= 0.3f) {
        // Cambiar al siguiente color
        indexColors = (indexColors + 1) % titleColors.size();
        Title.setFillColor(titleColors[indexColors]);

        // Reiniciar el reloj
        CambioColorClock.restart();
    }

    // Dibujar el título
    window.draw(Title);

    // Dibujar las opciones del menú
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        menuOptions[i].setFillColor(i == currentOption ? sf::Color::Green : sf::Color::White);
        window.draw(menuOptions[i]);
    }
}


/*Permite selecionar el nivel*/
void GameInterface::seleccionarNivel(sf::RenderWindow &window) {
    // Nivel 1 por defecto
    int level = 1;

    // Bucle para mantener la ventana abierta
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                bool moved = false; // Bandera para verificar si se debe reproducir el sonido de movimiento
                if (event.key.code == sf::Keyboard::Down) {
                    level = (level > 1) ? level - 1 : 10;
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Up) {
                    level = (level < 10) ? level + 1 : 1;
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    player2TankType = (player2TankType == 0) ? 1 : 0; // Cambiar tanque del jugador 2
                    moved = true;
                } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D) {
                    player1TankType = (player1TankType == 0) ? 1 : 0; // Cambiar tanque del jugador 1
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Reproducir sonido de selección
                    selectSound.play();

                    std::string mapFile = "nivel" + std::to_string(level) + ".txt";

                    // Configurar los tipos de tanque según las selecciones
                    int playersType = 0;
                    if (player1TankType == 0 && player2TankType == 0) {
                        playersType = 1; // Tank vs Tank
                    } else if (player1TankType == 0 && player2TankType == 1) {
                        playersType = 2; // Tank vs TankEscopeta
                    } else if (player1TankType == 1 && player2TankType == 0) {
                        playersType = 3; // TankEscopeta vs Tank
                    } else if (player1TankType == 1 && player2TankType == 1) {
                        playersType = 4; // TankEscopeta vs TankEscopeta
                    }

                    Game juego(playersType, mapFile);
                    juego.correr();
                    return;
                }

                if (moved) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            }
        }

        window.clear();

        // Mostrar selección de nivel
        sf::Text levelText("Selecciona Nivel: " + std::to_string(level), font, 50);
        levelText.setPosition(700, 300);
        levelText.setFillColor(sf::Color::Green);
        window.draw(levelText);

        // Mostrar tanque seleccionado para jugador 1
        sf::Text player1Text(
            "Jugador 1 (WASD): " + std::string(player1TankType == 0 ? "Tank" : "Tank Escopetin"),
            font, 40);
        player1Text.setPosition(300, 500);
        player1Text.setFillColor(sf::Color::Red);
        window.draw(player1Text);

        // Mostrar tanque seleccionado para jugador 2
        sf::Text player2Text(
            "Jugador 2 (Flechas): " + std::string(player2TankType == 0 ? "Tank" : "Tank Escopetin"),
            font, 40);
        player2Text.setPosition(300, 900);
        player2Text.setFillColor(sf::Color::Blue);
        window.draw(player2Text);

        window.display();
    }
}

void GameInterface::instrucciones(sf::RenderWindow &window) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                // Reproducir sonido de selección
                selectSound.play();
                return;
            }
        }

        window.clear(); // Limpia la ventana para nuevos objetos
        
        // Coloca las instrucciones con su respectivo tamaño
        sf::Text controlsText("Controles para los tanques:" , font , 50 );
        // Ubica el texto
        controlsText.setPosition(50, 50);
        // Le da un color
        controlsText.setFillColor(sf::Color::Green);
        // Dibuja los controles del tanque 
        window.draw(controlsText);

        // Coloca los contrles del jugador 1 
        sf::Text instructionsText1("Tanque 1: W A S D para moverse", font, 30);
        // Ubica las instrucciones
        instructionsText1.setPosition(50, 200);
        // Color de las instrucciones 
        instructionsText1.setFillColor(sf::Color::Red);
        // Dibuja las instrucciones en la ventana
        window.draw(instructionsText1);

        sf::Text instructionsText2("Tanque 2: Usa las flechas para moverse", font, 30);
        instructionsText2.setPosition(50, 300);
        instructionsText2.setFillColor(sf::Color::Blue);
        window.draw(instructionsText2);

        sf::Text tanksTypes("Informacion de nuestros tanques:" , font , 50);
        tanksTypes.setPosition(50, 400);
        tanksTypes.setFillColor(sf::Color::Green);
        window.draw(tanksTypes);

        sf::Text tankInfoText("Tank = Eres un tanque normal, disparas una bala pero tienes balas ilimitadas", font, 30);
        tankInfoText.setPosition(50, 500);
        tankInfoText.setFillColor(sf::Color::Magenta);
        window.draw(tankInfoText);

        sf::Text tankEscopetinInfoText("Tank escopetin = Disparas 4 balas dispersas por disparo, pero solo tienes 3 disparos!", font , 30);
        tankEscopetinInfoText.setPosition(50,600);
        tankEscopetinInfoText.setFillColor(sf::Color::Magenta); // Corregido: Cambiar color del texto correcto
        window.draw(tankEscopetinInfoText);

        sf::Text instructionsText3("Presiona Enter para volver", font, 50);
        instructionsText3.setPosition(1, 1000);
        instructionsText3.setFillColor(sf::Color::White);
        window.draw(instructionsText3);

        window.display();
    }
}

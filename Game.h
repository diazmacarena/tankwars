#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Tank.h"
#include "TankEscopeta.h"
#include "Bullet.h"
#include "Wall.h"
#include "DestructibleWall.h"  
#include <vector>
#include <string>

// Clase que maneja la lógica principal del juego
class Game {
private:
    sf::RenderWindow window;   // Ventana principal donde se renderiza el juego
    Tank* player1;             // Puntero al tanque del jugador 1
    Tank* player2;             // Puntero al tanque del jugador 2
    std::vector<Bullet> bullets; // Vector que almacena todas las balas activas en el juego
    std::string mapFile;       // Nombre del archivo que contiene el mapa
    sf::Texture bulletTexture; // Textura que se usará para las balas

    // Parámetros de juego
    const float velocidadDeMovimiento = 0.65f;   // Velocidad de movimiento para los tanques
    const float velocidadDeRotacion = 0.5f;   // Velocidad de rotación de los tanques
    const float intervaloDisparos = 0.25f;  // Tiempo minimo entre disparos
    const float recarga = 1.2f;      // Tiempo para recargar
    const int balasMaximas = 6;           // Númuero de balas para cada jugador 

    int player1Balas = 5;
    int player2Balas = 5;

    // Relojes para manejar tiempos de disparo y recarga de cada jugador
    sf::Clock shootClockPlayer1;
    sf::Clock shootClockPlayer2;
    sf::Clock reloadClockPlayer1;
    sf::Clock reloadClockPlayer2;
    sf::Clock tiempoMostrarGanador; // Reloj para manejar EL TIEMPO QUE SE MUESTRA AL GANADOR

    // Muros y paredes 
    std::vector<Wall> walls; // Vector para almacenar los muros indestructibles
    std::vector<DestructibleWall> destructibleWalls; // Vector para almacenar los muros destructibles
    sf::Vector2f ultimaPosicionValidaPlayer1; // Última posicion del jugador1
    sf::Vector2f ultimaPosicionValidaPlayer2; // Última posicion del jugadro2
 
    // Texto y estado del juego
    sf::Font font; // Fuente para textos en pantalla
    sf::Text textoGanador; // Texto para mostrar el ganador
    bool gameOver; // Estado para verificar si el juego ha terminado

    // Música y efectos de sonido
    sf::Music backgroundMusic; // Música de fondo
    sf::SoundBuffer winBuffer; // Buffer(contenedor) para guardar los sonidos
    sf::Sound winSound;        // Sonido de victoria
    sf::SoundBuffer takeDamageBuffer;  // Buffer para sonido de daño 
    sf::SoundBuffer destructionBuffer; // Buffer para sonido de destrucción
    sf::SoundBuffer bulletHitBuffer;   // Buffer para sonido de impacto de la bala
    sf::Sound bulletHitSound;          // Sonido de impacto de bala

    // Convierte ángulos en grados a radianes
    float degreesToRadians(float degrees);
   
    // Maneja eventos del juego (teclado, mouse, etc.)
    void manejarEventos();
   
    // Mueve el tanque especificado, actualizando su posición
    void moverTanque(Tank &player, float speed, sf::Vector2f &lastpositions);
   
    // Actualiza la lógica del juego (movimientos, colisiones, etc.)
    void actualizar();
   
    // Renderiza (dibuja) los objetos en la pantalla
    void renderizar();
   
    // Dispara una bala desde el tanque especificado
    void dispararBala(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock);
    
    // Dispara varias balas tipo escopeta desde el tanque especificado   
    void dispararEscopeta(TankEscopeta &player, sf::Clock &shootClock);

    // Carga el nivel desde un archivo de configuración
    void cargarNivel(const std::string& filename);

public:  // 2 jugadores (1 Tank, 1 Tank)
    
    // Constructor: Inicializa el juego con el tipo de jugadores y el mapa
    Game(int playersType, const std::string& mapFile);           
    
    // Inicia el ciclo principal del juego
    void correr();
    
    // Destructor para liberar memoria y recursos
    ~Game();
};

#endif // GAME_H

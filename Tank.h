#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <string>
class Bullet;

class Tank {
public:
    sf::Sprite sprite; //Sprite que representa gráficamente al tanque.
    sf::Texture texture; // Textura del sprite del tanque.
    float speed; // Velocidad del Tanque
    sf::Vector2f direction; // Dirección en la que se mueve o apunta el tanque.
    int vidas;  // Atributo para almacenar las vidas del tanque

    Tank();
    // Constructor de la clase Tank
    Tank(const std::string& textureFile, float initialX, float initialY);

    // Funciones virtuales para ser sobrescritas por clases derivadas
    virtual void rotate(float angle); //Ángulo en grados por el cual rotar el tanque.
    virtual void move(float offsetX, float offsetY);//Mueve el tanque por una distancia específica.
    virtual void draw(sf::RenderWindow &window);//Dibuja el tanque en la ventana del juego.

    // Método virtual para restar vida del tanque.
    virtual void restarVida();

    /**
     Dispara un proyectil desde el tanque.
     bullets Vector donde se almacenan las balas disparadas.
     shootClock Reloj que mide el intervalo entre disparos.
     bulletCount Contador del número de balas disparadas.
     shootInterval Tiempo mínimo entre disparos consecutivos.
     reloadTime Tiempo necesario para recargar el arma.
     reloadClock Reloj que mide el tiempo de recarga.
     */
    virtual void shoot(std::vector<Bullet>& bullets, sf::Clock &shootClock,
                         int &bulletCount, const float shootInterval,
                         const float reloadTime, sf::Clock &reloadClock);

    // Verificar si el tanque ha sido destruido
    bool estaDestruido() const; //`true` si el tanque no tiene más vidas, `false` en caso contrario.

    void ocultar(); // Oculta el tanque (por ejemplo, cuando está destruido).

    void setBulletTexture(const sf::Texture& texture); //Referencia a la textura que usarán las balas.
    void setTakeDamageSound(const sf::SoundBuffer& buffer); //Referencia al buffer de sonido para el efecto de daño.
    void setDestructionSound(const sf::SoundBuffer& buffer); //Referencia al buffer de sonido para el efecto de destrucción.

protected:
    static float degreesToRadians(float degres); //Ángulo en grados.
    const sf::Texture* bulletTexture;//Textura de las balas.
    sf::Sound takeDamageSound; //Sonido que se reproduce cuando el tanque recibe daño.
    sf::Sound destructionSound; // Sonido que se reproduce cuando el tanque es destruido.
};

#endif // TANK_H

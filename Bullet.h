#include <SFML/Graphics.hpp>
#include <iostream>  // Include for std::cerr
#include <cmath>

class Bullet {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isActive;

    // Constructor de la clase Bullet
    Bullet(const std::string& textureFile, float startX, float startY, sf::Vector2f direction) {
        
        if (!texture.loadFromFile(textureFile)) {
            // Manejar error
            std::cerr << "Error loading texture from " << textureFile << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(startX, startY);
        
        // Cambiar el tamaño de la bala: 0.3 significa reducir su tamaño al 30%
        sprite.setScale(0.05f, 0.05f);  // Cambiar los factores de escala según prefieras
        ////std::cout << "Bullet Size: " << sprite.getGlobalBounds().width << " x " << sprite.getGlobalBounds().height << std::endl;

        // Normalizar la dirección y multiplicar por la velocidad
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;  // Normalize
        }
        velocity = direction * 5.0f;  // Velocidad de la bala
        isActive = true;
    }

    // Movimiento de la bala
    void update() {
        if (isActive) {
            sprite.move(velocity);
            // Si la bala sale de los límites de la pantalla, la desactivamos
            if (sprite.getPosition().x < 0 || sprite.getPosition().x > 1920 ||
                sprite.getPosition().y < 0 || sprite.getPosition().y > 1080) {
                isActive = false;
            }
        }
    }

    // Dibujar la bala
    void draw(sf::RenderWindow &window) {
        if (isActive) {
            window.draw(sprite);
        }
    }

    // Detectar colisiones con otro sprite
    bool checkCollision(sf::Sprite target) {
        return isActive && sprite.getGlobalBounds().intersects(target.getGlobalBounds());
    }
};

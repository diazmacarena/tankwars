#include <SFML/Graphics.hpp>
#include <iostream>

class Tank {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    sf::Vector2f direction;
    int vidas;  // Atributo para almacenar las vidas del tanque

    // Constructor de la clase Tank
    Tank(const std::string& textureFile, float initialX, float initialY) : vidas(3) {  // Inicializamos con 3 vidas
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Error: No se pudo cargar la textura " << textureFile << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setScale(0.25f, 0.25f);  
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setPosition(initialX, initialY); 

        speed = 1.0f;  // Velocidad del tanque
        direction = sf::Vector2f(0, 0);
    }

    // Rotación
    void rotate(float angle) {
        sprite.rotate(angle); 
    }
    
    // Movimiento
    void move(float offsetX, float offsetY) {
        sprite.move(offsetX * speed, offsetY * speed);
        direction.x = offsetX;
        direction.y = offsetY;
    }

    // Método para restar una vida
    void restarVida() {
        if (vidas > 0) {
            vidas--;
        }
    }

    // Verificar si el tanque ha sido destruido
    bool estaDestruido() const {
        return vidas <= 0;
    }

    // Renderizar el tanque
    void draw(sf::RenderWindow &window) {
        window.draw(sprite);
    }
};

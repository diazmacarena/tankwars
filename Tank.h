#include <SFML/Graphics.hpp>
#include <iostream>

class Tank {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    sf::Vector2f direction;
    sf::Transformable transformable;

    // Constructor
    // Constructor de la clase Tank
    Tank(const std::string& textureFile, float initialX, float initialY) {
        if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: No se pudo cargar la textura " << textureFile << std::endl;
    }

        sprite.setTexture(texture);

        sprite.setScale(0.5f, 0.5f);  

        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        sprite.setPosition(initialX, initialY); 

        speed = 2.0f;  // Velocidad del tanque
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

    // Renderizar el tanque
    void draw(sf::RenderWindow &window) {
        window.draw(sprite);
    }
};


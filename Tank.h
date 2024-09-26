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
    // Constructor de la clase Tank
    Tank(const std::string& textureFile, float initialX, float initialY) {
        if (!texture.loadFromFile(textureFile)) {
            // Manejar error
            std::cerr << "Error loading texture from " << textureFile << std::endl;
        }
        sprite.setTexture(texture);

        // Cambiar el tamaño del tanque: 0.5 significa reducir a la mitad su tamaño
        sprite.setScale(0.5f, 0.5f);  // Cambiar los factores de escala como desees

        // Set the origin to the center of the sprite for better scaling behavior
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        sprite.setPosition(initialX, initialY); // Set position after scaling

        speed = 2.0f;  // Velocidad del tanque
        direction = sf::Vector2f(0, 0);
}


    void rotate() {
        float rotacion = 1;
        float rotacionverdadera = rotacion/5;
        
        sprite.rotate(rotacionverdadera);
    }
    void inRotate(){
        float rotacion = 1;
        float rotacionverdadera = rotacion/-5;
        sprite.rotate(rotacionverdadera);
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


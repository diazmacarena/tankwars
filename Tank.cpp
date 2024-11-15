#include "Tank.h"

Tank::Tank(const std::string& textureFile, float initialX, float initialY) : vidas(3) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: No se pudo cargar la textura " << textureFile << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.15f, 0.15f);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(initialX, initialY);

    speed = 1.0f;  // Velocidad del tanque
    direction = sf::Vector2f(0, 0);
}

void Tank::rotate(float angle) {
    sprite.rotate(angle);
}

void Tank::move(float offsetX, float offsetY) {
    sprite.move(offsetX * speed, offsetY * speed);
    direction.x = offsetX;
    direction.y = offsetY;
}

void Tank::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Tank::restarVida() {
    if (vidas > 0) {
        vidas--;
        std::cout << "Vida del tanque reducida. Vidas restantes: " << vidas << std::endl;
    }

    if (vidas == 0) {
        std::cout << "El tanque ha sido destruido." << std::endl;
        ocultar();
    }
}


bool Tank::estaDestruido() const {
    return vidas <= 0;
}

void Tank::ocultar() {
    sprite.setPosition(-1000, -1000);
}

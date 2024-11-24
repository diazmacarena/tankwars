#include "Tank.h"
#include "Bullet.h"
#include <cmath>

// Constructor por defecto
Tank::Tank() : vidas(3), speed(1.0f), direction(0, 0) {
    std::cout << "Tanque creado con valores predeterminados.\n";
    // Opcionalmente podrías cargar una textura predeterminada
    if (!texture.loadFromFile("default_tank.png")) {
        std::cerr << "Error: No se pudo cargar la textura predeterminada.\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.09f, 0.09f);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(0, 0);  // Posición inicial predeterminada
}

Tank::Tank(const std::string& textureFile, float initialX, float initialY) : vidas(3), speed(1.0f) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: No se pudo cargar la textura " << textureFile << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.15f , 0.15f);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(initialX, initialY);

    direction = sf::Vector2f(0, 0);
}

void Tank::rotate(float angle) {
    sprite.rotate(angle);
}

void Tank::move(float offsetX, float offsetY) {
    // No multiplicamos por 'speed' aquí porque ya se aplicó en Game::moverTanque
    sprite.move(offsetX, offsetY);
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

        takeDamageSound.setVolume(50);
        takeDamageSound.play();
    }

    if (vidas == 0) {
        std::cout << "El tanque ha sido destruido." << std::endl;

        destructionSound.setVolume(40);
        destructionSound.play();

        ocultar();
    }
}

bool Tank::estaDestruido() const {
    return vidas <= 0;
}

void Tank::ocultar() {
    sprite.setPosition(-1000, -1000);
}

float Tank::degreesToRadians(float degrees) {
    return degrees * (3.14159265f / 180.0f);
}

void Tank::shoot(std::vector<Bullet>& bullets, sf::Clock &shootClock, int &bulletCount,
                 const float shootInterval, const float reloadTime, sf::Clock &reloadClock) {
    if (shootClock.getElapsedTime().asSeconds() >= shootInterval && bulletCount > 0) {
        float angleRadians = degreesToRadians(sprite.getRotation());
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        // Use bulletTexture instead of "bullet.png"
        bullets.emplace_back(*bulletTexture, sprite.getPosition().x, sprite.getPosition().y, direction, this);

        bulletCount--;
        shootClock.restart();
        reloadClock.restart();
    }
}

void Tank::setTakeDamageSound(const sf::SoundBuffer& buffer) {
    takeDamageSound.setBuffer(buffer);
    takeDamageSound.setVolume(100); // Volumen máximo
}

void Tank::setDestructionSound(const sf::SoundBuffer& buffer) {
    destructionSound.setBuffer(buffer);
    destructionSound.setVolume(100); // Volumen máximo
}

void Tank::setBulletTexture(const sf::Texture& texture) {
    bulletTexture = &texture;
}

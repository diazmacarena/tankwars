#include <SFML/Graphics.hpp>
#include <vector>

class Entity {
public:
    int positionX, positionY;
    int health;

    Entity(int x, int y, int hp) : positionX(x), positionY(y), health(hp) {}

    virtual void move(int dx, int dy) {
        positionX += dx;
        positionY += dy;
    }

    virtual void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    virtual void draw(sf::RenderWindow& window) = 0;

    virtual ~Entity() {}
};

class Wall : public Entity {
public:
    int sizeWidth, sizeHeight;
    bool isDestructible;
    sf::RectangleShape wallShape;

    Wall(int x1, int y1, int width, int height, bool destructible, int hp)
        : Entity(x1, y1, hp), sizeWidth(width), sizeHeight(height), isDestructible(destructible) {
        wallShape.setSize(sf::Vector2f(sizeWidth, sizeHeight));
        wallShape.setPosition(positionX, positionY);
        wallShape.setFillColor(sf::Color::Blue);
    }

    void takeDamage(int amount) override {
        if (isDestructible) {
            health -= amount;
            if (health < 0) health = 0;
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(wallShape);
    }
};

class Bullet {
public:
    int posX, posY;
    int velocityX, velocityY;
    int bouncesLeft;
    sf::CircleShape bulletShape;

    Bullet(int x, int y, int vx, int vy, int maxBounces)
        : posX(x), posY(y), velocityX(vx), velocityY(vy), bouncesLeft(maxBounces) {
        bulletShape.setRadius(5.f);
        bulletShape.setFillColor(sf::Color::Red);
        bulletShape.setPosition(posX, posY);
    }

    void move() {
        posX += velocityX;
        posY += velocityY;
        bulletShape.setPosition(posX, posY);
    }

    void bounce() {
        bouncesLeft--;
        if (bouncesLeft <= 0) {
            // Destroy bullet or mark it as inactive
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bulletShape);
    }
};

class Tank : public Entity {
public:
    int bulletsAvailable;
    std::vector<Bullet> bullets;
    sf::CircleShape tankShape;

    Tank(int x, int y, int hp, int bulletCount) : Entity(x, y, hp), bulletsAvailable(bulletCount) {
        tankShape.setRadius(20.f);
        tankShape.setFillColor(sf::Color::Green);
        tankShape.setPosition(positionX, positionY);
    }

    void shoot(int vx, int vy) {
        if (bulletsAvailable > 0) {
            bullets.push_back(Bullet(positionX, positionY, vx, vy, 3)); // 3 bounces allowed
            bulletsAvailable--;
        }
    }

    void move(int dx, int dy) override {
        positionX += dx;
        positionY += dy;
        tankShape.setPosition(positionX, positionY);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(tankShape);
        for (Bullet& b : bullets) {
            b.draw(window);
        }
    }
};


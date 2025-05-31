#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Forward declarations
class Enemy;

// Base weapon class
class Weapon {
public:
    enum class Type {
        MELEE,
        RANGED
    };

    Weapon(Type type, int damage, float cooldown, float range);
    virtual ~Weapon() = default;

    // Pure virtual methods that must be implemented by derived classes
    virtual bool canAttack(float deltaTime) = 0;
    virtual void attack(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
    virtual void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const = 0;
    virtual void update(float deltaTime) = 0;

    // Common weapon properties
    Type getType() const { return type_; }
    int getDamage() const { return damage_; }
    float getRange() const { return range_; }
    bool isOnCooldown() const { return cooldownTimer_ > 0.0f; }

    // Public method to check if there are enemies in range
    bool hasEnemiesInRange(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) const;

protected:
    // Helper function to find closest enemy
    Enemy* findClosestEnemy(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) const;
    
    // Helper function to calculate distance
    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const;

    Type type_;
    int damage_;
    float cooldown_;
    float range_;
    float cooldownTimer_;
};

// Melee weapon class
class MeleeWeapon : public Weapon {
public:
    MeleeWeapon(int damage, float cooldown, float range, float swingDuration = 0.3f);

    bool canAttack(float deltaTime) override;
    void attack(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    void update(float deltaTime) override;

private:
    float swingDuration_;
    float swingTimer_;
    bool isSwinging_;
    sf::Vector2f swingDirection_;
};

// Ranged weapon class
class RangedWeapon : public Weapon {
public:
    RangedWeapon(int damage, float cooldown, float range, float projectileSpeed = 400.0f);

    bool canAttack(float deltaTime) override;
    void attack(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) override;
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    void update(float deltaTime) override;

    // Public method for updating projectiles with enemy collision
    void updateProjectiles(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies);

private:
    struct Projectile {
        sf::Vector2f position;
        sf::Vector2f velocity;
        float lifetime;
        bool active;
        
        Projectile(sf::Vector2f pos, sf::Vector2f vel) 
            : position(pos), velocity(vel), lifetime(3.0f), active(true) {}
    };

    float projectileSpeed_;
    std::vector<Projectile> projectiles_;
    
    void checkProjectileCollisions(const std::vector<std::unique_ptr<Enemy>>& enemies);
};

// Specific weapon implementations
class Sword : public MeleeWeapon {
public:
    Sword() : MeleeWeapon(25, 0.8f, 60.0f, 0.3f) {}
};

class Dagger : public MeleeWeapon {
public:
    Dagger() : MeleeWeapon(15, 0.5f, 45.0f, 0.2f) {}
};

class Bow : public RangedWeapon {
public:
    Bow() : RangedWeapon(20, 1.0f, 250.0f, 350.0f) {}
};

class Crossbow : public RangedWeapon {
public:
    Crossbow() : RangedWeapon(35, 1.5f, 300.0f, 500.0f) {}
}; 
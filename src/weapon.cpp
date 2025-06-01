#include "weapon.h"
#include "enemy.h"
#include <cmath>
#include <algorithm>

// Base Weapon implementation
Weapon::Weapon(Type type, int damage, float cooldown, float range)
    : type_(type), damage_(damage), cooldown_(cooldown), range_(range), cooldownTimer_(0.0f) {
}

bool Weapon::hasEnemiesInRange(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) const {
    return findClosestEnemy(playerPos, enemies) != nullptr;
}

Enemy* Weapon::findClosestEnemy(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) const {
    Enemy* closestEnemy = nullptr;
    float closestDistance = range_;

    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            float distance = getDistance(playerPos, enemy->getWorldPosition());
            if (distance < closestDistance) {
                closestDistance = distance;
                closestEnemy = enemy.get();
            }
        }
    }

    return closestEnemy;
}

float Weapon::getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dy * dy);
}

// MeleeWeapon implementation
MeleeWeapon::MeleeWeapon(int damage, float cooldown, float range, float swingDuration)
    : Weapon(Type::MELEE, damage, cooldown, range), 
      swingDuration_(swingDuration), swingTimer_(0.0f), isSwinging_(false) {
}

bool MeleeWeapon::canAttack(float deltaTime) {
    cooldownTimer_ -= deltaTime;
    if (cooldownTimer_ < 0.0f) cooldownTimer_ = 0.0f;
    
    return cooldownTimer_ <= 0.0f && !isSwinging_;
}

void MeleeWeapon::attack(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!canAttack(0.0f)) return;

    // Find closest enemy for swing direction (visual purposes)
    Enemy* closestTarget = findClosestEnemy(playerPos, enemies);
    if (closestTarget) {
        // Start swing animation
        isSwinging_ = true;
        swingTimer_ = swingDuration_;
        cooldownTimer_ = cooldown_;
        
        // Calculate swing direction based on closest enemy
        swingDirection_ = closestTarget->getWorldPosition() - playerPos;
        float length = std::sqrt(swingDirection_.x * swingDirection_.x + swingDirection_.y * swingDirection_.y);
        if (length > 0.0f) {
            swingDirection_ /= length; // Normalize
        }
        
        // Deal damage to ALL enemies within range (splash damage)
        for (const auto& enemy : enemies) {
            if (enemy->isAlive()) {
                float distance = getDistance(playerPos, enemy->getWorldPosition());
                if (distance <= range_) {
                    enemy->takeDamage(damage_);
                }
            }
        }
    }
}

void MeleeWeapon::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    // Draw range circle
    sf::CircleShape rangeCircle(range_);
    rangeCircle.setFillColor(sf::Color::Transparent);
    rangeCircle.setOutlineThickness(2.0f);
    rangeCircle.setOutlineColor(sf::Color(100, 100, 255, 80));
    rangeCircle.setOrigin(range_, range_);
    rangeCircle.setPosition(playerPos);
    window.draw(rangeCircle);
    
    if (isSwinging_) {
        // Draw swing arc
        sf::CircleShape swingArc(range_);
        swingArc.setFillColor(sf::Color(255, 255, 255, 150)); // Brighter for attack animation
        swingArc.setOrigin(range_, range_);
        swingArc.setPosition(playerPos);
        
        // Calculate swing angle based on direction
        float angle = std::atan2(swingDirection_.y, swingDirection_.x) * 180.0f / 3.14159f;
        swingArc.setRotation(angle);
        
        // Scale based on swing progress
        float swingProgress = (swingDuration_ - swingTimer_) / swingDuration_;
        float scale = 0.5f + 0.5f * swingProgress;
        swingArc.setScale(scale, scale);
        
        window.draw(swingArc);
    }
}

void MeleeWeapon::update(float deltaTime) {
    if (isSwinging_) {
        swingTimer_ -= deltaTime;
        if (swingTimer_ <= 0.0f) {
            isSwinging_ = false;
            swingTimer_ = 0.0f;
        }
    }
}

std::string MeleeWeapon::getName() const {
    return "Melee Weapon";
}

// RangedWeapon implementation
RangedWeapon::RangedWeapon(int damage, float cooldown, float range, float projectileSpeed)
    : Weapon(Type::RANGED, damage, cooldown, range), projectileSpeed_(projectileSpeed) {
}

bool RangedWeapon::canAttack(float deltaTime) {
    cooldownTimer_ -= deltaTime;
    if (cooldownTimer_ < 0.0f) cooldownTimer_ = 0.0f;
    
    return cooldownTimer_ <= 0.0f;
}

void RangedWeapon::attack(const sf::Vector2f& playerPos, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!canAttack(0.0f)) return;

    Enemy* target = findClosestEnemy(playerPos, enemies);
    if (target) {
        // Calculate projectile direction
        sf::Vector2f direction = target->getWorldPosition() - playerPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (length > 0.0f) {
            direction /= length; // Normalize
            
            // Create projectile
            sf::Vector2f velocity = direction * projectileSpeed_;
            projectiles_.emplace_back(playerPos, velocity);
            
            cooldownTimer_ = cooldown_;
        }
    }
}

void RangedWeapon::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    // Draw range circle
    sf::CircleShape rangeCircle(range_);
    rangeCircle.setFillColor(sf::Color::Transparent);
    rangeCircle.setOutlineThickness(2.0f);
    rangeCircle.setOutlineColor(sf::Color(100, 100, 255, 80));
    rangeCircle.setOrigin(range_, range_);
    rangeCircle.setPosition(playerPos);
    window.draw(rangeCircle);
    
    // Draw projectiles
    for (const auto& projectile : projectiles_) {
        if (projectile.active) {
            sf::CircleShape projectileShape(3.0f);
            projectileShape.setFillColor(sf::Color::Yellow);
            projectileShape.setOrigin(3.0f, 3.0f);
            projectileShape.setPosition(projectile.position);
            window.draw(projectileShape);
        }
    }
}

void RangedWeapon::update(float deltaTime) {
    updateProjectiles(deltaTime, std::vector<std::unique_ptr<Enemy>>{}); // Empty vector for now
}

std::string RangedWeapon::getName() const {
    return "Ranged Weapon";
}

void RangedWeapon::updateProjectiles(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& projectile : projectiles_) {
        if (projectile.active) {
            // Update position
            projectile.position += projectile.velocity * deltaTime;
            
            // Update lifetime
            projectile.lifetime -= deltaTime;
            if (projectile.lifetime <= 0.0f) {
                projectile.active = false;
                continue;
            }
            
            // Check collision with enemies
            for (const auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    float distance = getDistance(projectile.position, enemy->getWorldPosition());
                    if (distance < 20.0f) { // Hit radius
                        enemy->takeDamage(damage_);
                        projectile.active = false;
                        break;
                    }
                }
            }
        }
    }
    
    // Remove inactive projectiles
    projectiles_.erase(
        std::remove_if(projectiles_.begin(), projectiles_.end(),
            [](const Projectile& p) { return !p.active; }),
        projectiles_.end()
    );
} 
#include <string>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "config.h"
#include "enemy.h"
#include "weapons/weapon_factory.h"
#include "talents/talent_factory.h"

Player::Player() : 
    health(Config::PLAYER_START_HEALTH), 
    maxHealth_(Config::PLAYER_START_HEALTH),
    level(1), 
    experience(0), 
    gold(0), 
    speed(Config::PLAYER_SPEED),
    currentWeaponIndex_(0),
    autoAttack_(true),
    pendingLevelUps_(0),
    damageMultiplier_(1.0f),
    attackSpeedMultiplier_(1.0f),
    critChance_(0.0f),
    critMultiplier_(2.0f),
    lifeStealPercent_(0.0f),
    armorValue_(0.0f),
    regenRate_(0.0f),
    experienceMultiplier_(1.0f),
    goldMultiplier_(1.0f),
    dodgeChance_(0.0f) {
    shape.setFillColor(sf::Color::Green);
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setOrigin(25.f, 25.f);
    
    // Initialize world position
    worldPosition = sf::Vector2f(Config::WORLD_WIDTH/2, Config::WORLD_HEIGHT/2);
    shape.setPosition(worldPosition);
    
    // Initialize font and text
    if (!font.loadFromFile("ARIAL.TTF")) {
        // Handle font loading error
    }
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::White);

    // Initialize with default weapons using the factory
    weapons_.push_back(createSword());
    weapons_.push_back(createBow());
    weapons_.push_back(createDagger());
    weapons_.push_back(createCrossbow());
    
    // Generate the talent tree once at game start
    talentTree_.generatePermanentTree();
}

void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        worldPosition.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        worldPosition.x += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        worldPosition.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        worldPosition.y += speed * deltaTime;
    }
    
    // Update shape position to match world position
    shape.setPosition(worldPosition);
}

void Player::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Handle regeneration
    if (regenRate_ > 0.0f && health < maxHealth_) {
        static float regenTimer = 0.0f;
        regenTimer += deltaTime;
        if (regenTimer >= 1.0f) { // Regenerate every second
            int regenAmount = static_cast<int>(maxHealth_ * regenRate_);
            healPlayer(regenAmount);
            regenTimer = 0.0f;
        }
    }
    
    // Update current weapon
    if (getCurrentWeapon()) {
        getCurrentWeapon()->update(deltaTime);
        
        // Handle ranged weapon projectile updates with enemy collision
        if (getCurrentWeapon()->getType() == Weapon::Type::RANGED) {
            RangedWeapon* rangedWeapon = static_cast<RangedWeapon*>(getCurrentWeapon());
            rangedWeapon->updateProjectiles(deltaTime, enemies, this);
        }
    }

    // Handle weapon switching
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && weapons_.size() > 0) {
        switchWeapon(0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && weapons_.size() > 1) {
        switchWeapon(1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && weapons_.size() > 2) {
        switchWeapon(2);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && weapons_.size() > 3) {
        switchWeapon(3);
    }

    // Handle manual attack
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        attack(enemies);
    }

    // Auto-attack if enabled and enemies are nearby
    if (autoAttack_ && getCurrentWeapon() && getCurrentWeapon()->canAttack(deltaTime, this)) {
        // Check if there are enemies in range
        if (getCurrentWeapon()->hasEnemiesInRange(worldPosition, enemies)) {
            attack(enemies);
        }
    }
}

void Player::attack(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (getCurrentWeapon()) {
        getCurrentWeapon()->attack(worldPosition, enemies, this);
    }
}

void Player::switchWeapon(int weaponIndex) {
    if (weaponIndex >= 0 && weaponIndex < static_cast<int>(weapons_.size())) {
        currentWeaponIndex_ = weaponIndex;
    }
}

void Player::addWeapon(std::unique_ptr<Weapon> weapon) {
    weapons_.push_back(std::move(weapon));
}

Weapon* Player::getCurrentWeapon() const {
    if (currentWeaponIndex_ >= 0 && currentWeaponIndex_ < static_cast<int>(weapons_.size())) {
        return weapons_[currentWeaponIndex_].get();
    }
    return nullptr;
}

void Player::updatePosition(const sf::Vector2f& cameraOffset) {
    // Update the shape's position relative to the camera
    shape.setPosition(worldPosition - cameraOffset);
}

void Player::takeDamage(int damage) {
    // Check for dodge chance
    if (dodgeChance_ > 0.0f) {
        float dodgeRoll = static_cast<float>(rand()) / RAND_MAX;
        if (dodgeRoll < dodgeChance_) {
            return; // Dodged the attack
        }
    }
    
    // Apply armor reduction
    float damageReduction = armorValue_ / (armorValue_ + 100.0f); // Diminishing returns formula
    int reducedDamage = static_cast<int>(damage * (1.0f - damageReduction));
    
    health -= reducedDamage;
    if (health < 0) health = 0;
}

void Player::gainExperience(int exp) {
    // Apply experience multiplier from talents
    int adjustedExp = static_cast<int>(exp * experienceMultiplier_);
    experience += adjustedExp;
    
    // Check for level up(s)
    while (experience >= getExperienceNeeded()) {
        int expNeededForThisLevel = getExperienceNeeded(); // Store before level increment
        level++;
        experience -= expNeededForThisLevel; // Use the stored value
        pendingLevelUps_++;
    }
}

void Player::processLevelUp() {
    if (pendingLevelUps_ > 0) {
        // For the first level up, unlock the top row if no talents are unlocked yet
        auto availableTalents = talentTree_.getAvailableTalents();
        if (availableTalents.empty()) {
            talentTree_.unlockTopRow();
        }
        // For subsequent level ups, the selectTalent method handles unlocking
        pendingLevelUps_--;
    }
}

void Player::selectTalent(int talentIndex) {
    if (talentTree_.selectTalent(talentIndex, *this)) {
        // Talent was successfully selected
        // Note: We don't clear the talent tree anymore - it persists throughout the game
    }
}

void Player::addGold(int amount) {
    // Apply gold multiplier from talents
    int adjustedGold = static_cast<int>(amount * goldMultiplier_);
    gold += adjustedGold;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::healPlayer(int amount) {
    health += amount;
    if (health > maxHealth_) {
        health = maxHealth_;
    }
}

int Player::getExperienceNeeded() const {
    return level * 100; // Simple scaling: level 1 = 100 exp, level 2 = 200 exp, etc.
}

void Player::draw(sf::RenderWindow& window) {
    // Draw the player shape
    window.draw(shape);
    
    // Draw current weapon effects
    if (getCurrentWeapon()) {
        getCurrentWeapon()->draw(window, worldPosition);
    }
    
    // Update and draw health text
    healthText.setString("HP: " + std::to_string(health) + "/" + std::to_string(maxHealth_));
    // Position text above player
    sf::FloatRect textBounds = healthText.getLocalBounds();
    healthText.setPosition(
        shape.getPosition().x - textBounds.width / 2,
        shape.getPosition().y - shape.getSize().y / 2 - 30
    );
    window.draw(healthText);

    // Draw weapon info
    if (getCurrentWeapon()) {
        sf::Text weaponText;
        weaponText.setFont(font);
        weaponText.setCharacterSize(16);
        weaponText.setFillColor(sf::Color::Cyan);
        
        std::string weaponInfo = "Weapon: " + getCurrentWeapon()->getName();
        weaponInfo += " (" + std::to_string(currentWeaponIndex_ + 1) + "/" + std::to_string(weapons_.size()) + ")";
        weaponText.setString(weaponInfo);
        
        // Position weapon text below health
        sf::FloatRect weaponBounds = weaponText.getLocalBounds();
        weaponText.setPosition(
            shape.getPosition().x - weaponBounds.width / 2,
            shape.getPosition().y - shape.getSize().y / 2 - 50
        );
        window.draw(weaponText);
    }
    
    // Draw level and experience info
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(14);
    levelText.setFillColor(sf::Color(255, 215, 0)); // Gold color using RGB values
    
    std::string levelInfo = "Level " + std::to_string(level) + " (";
    levelInfo += std::to_string(experience) + "/" + std::to_string(getExperienceNeeded()) + " XP)";
    if (pendingLevelUps_ > 0) {
        levelInfo += " - LEVEL UP!";
    }
    levelText.setString(levelInfo);
    
    // Position level text below weapon info
    sf::FloatRect levelBounds = levelText.getLocalBounds();
    levelText.setPosition(
        shape.getPosition().x - levelBounds.width / 2,
        shape.getPosition().y - shape.getSize().y / 2 - 70
    );
    window.draw(levelText);
}

void Player::wrapPosition() {
    // Wrap position if player goes out of bounds
    if (worldPosition.x < 0) worldPosition.x = Config::WORLD_WIDTH;
    if (worldPosition.x > Config::WORLD_WIDTH) worldPosition.x = 0;
    if (worldPosition.y < 0) worldPosition.y = Config::WORLD_HEIGHT;
    if (worldPosition.y > Config::WORLD_HEIGHT) worldPosition.y = 1;
}

int Player::calculateModifiedDamage(int baseDamage) const {
    float modifiedDamage = static_cast<float>(baseDamage) * damageMultiplier_;
    
    // Apply critical strike chance
    if (critChance_ > 0.0f) {
        float critRoll = static_cast<float>(rand()) / RAND_MAX;
        if (critRoll < critChance_) {
            modifiedDamage *= critMultiplier_;
        }
    }
    
    return static_cast<int>(modifiedDamage);
}

float Player::calculateModifiedCooldown(float baseCooldown) const {
    // Attack speed multiplier reduces cooldown
    return baseCooldown / (1.0f + attackSpeedMultiplier_);
}

void Player::applyLifeSteal(int damageDealt) {
    if (lifeStealPercent_ > 0.0f) {
        int healAmount = static_cast<int>(damageDealt * lifeStealPercent_);
        healPlayer(healAmount);
    }
}


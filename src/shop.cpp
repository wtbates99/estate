#include "shop.h"
#include "player.h"
#include "config.h"
#include "weapons/weapon_factory.h"
#include "weapon_tier_system.h"
#include <random>
#include <cmath>
#include <sstream>

Shop::Shop() : 
    visible_(true), teleportTimer_(0.0f), teleportCooldown_(150.0f), // Random between 120-180
    warningTimer_(0.0f), showWarning_(false),
    selectedCategory_(0), selectedItem_(0), showUI_(false) {
    
    // Generate random teleport cooldown between 120-180 seconds
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(120.0f, 180.0f);
    teleportCooldown_ = dist(gen);
    
    // Initialize position
    generateRandomTeleportLocation();
    
    // Load font
    if (!font_.loadFromFile("ARIAL.TTF")) {
        // Handle font loading error
    }
    
    // Initialize shop visual
    shopShape_.setSize(sf::Vector2f(80.0f, 80.0f));
    shopShape_.setFillColor(sf::Color(139, 69, 19)); // Brown color for shop
    shopShape_.setOutlineColor(sf::Color::Yellow);
    shopShape_.setOutlineThickness(3.0f);
    shopShape_.setOrigin(40.0f, 40.0f);
    
    // Range indicator
    rangeIndicator_.setRadius(100.0f);
    rangeIndicator_.setFillColor(sf::Color(255, 255, 0, 30)); // Semi-transparent yellow
    rangeIndicator_.setOutlineColor(sf::Color::Yellow);
    rangeIndicator_.setOutlineThickness(2.0f);
    rangeIndicator_.setOrigin(100.0f, 100.0f);
    
    // Shop text
    shopText_.setFont(font_);
    shopText_.setString("SHOP");
    shopText_.setCharacterSize(16);
    shopText_.setFillColor(sf::Color::White);
    shopText_.setStyle(sf::Text::Bold);
    
    // Timer text
    timerText_.setFont(font_);
    timerText_.setCharacterSize(14);
    timerText_.setFillColor(sf::Color::White);
    
    // Warning text
    warningText_.setFont(font_);
    warningText_.setString("SHOP TELEPORTING IN 30s!");
    warningText_.setCharacterSize(18);
    warningText_.setFillColor(sf::Color::Red);
    warningText_.setStyle(sf::Text::Bold);
    
    // UI components
    uiBackground_.setSize(sf::Vector2f(600.0f, 400.0f));
    uiBackground_.setFillColor(sf::Color(0, 0, 0, 200));
    uiBackground_.setOutlineColor(sf::Color::Yellow);
    uiBackground_.setOutlineThickness(3.0f);
    
    categoryText_.setFont(font_);
    categoryText_.setCharacterSize(20);
    categoryText_.setFillColor(sf::Color::Yellow);
    categoryText_.setStyle(sf::Text::Bold);
    
    itemListText_.setFont(font_);
    itemListText_.setCharacterSize(14);
    itemListText_.setFillColor(sf::Color::White);
    
    detailsText_.setFont(font_);
    detailsText_.setCharacterSize(12);
    detailsText_.setFillColor(sf::Color::Cyan);
    
    instructionsText_.setFont(font_);
    instructionsText_.setString("A/D: Change Category | W/S: Select Item | E: Purchase | Q: Close");
    instructionsText_.setCharacterSize(12);
    instructionsText_.setFillColor(sf::Color::Green);
    
    initializeInventory();
}

void Shop::update(float deltaTime, const Player& player) {
    if (!visible_) return;
    
    updateTimer(deltaTime);
    
    // Player can manually open UI with E key when in range
    // UI closes automatically when player leaves range
    if (!isPlayerInRange(player)) {
        showUI_ = false;
    }
    
    // Update positions
    shopShape_.setPosition(position_);
    rangeIndicator_.setPosition(position_);
    
    // Center shop text
    sf::FloatRect shopTextBounds = shopText_.getLocalBounds();
    shopText_.setOrigin(shopTextBounds.width / 2.0f, shopTextBounds.height / 2.0f);
    shopText_.setPosition(position_.x, position_.y - 50.0f);
}

void Shop::draw(sf::RenderWindow& window) const {
    if (!visible_) return;
    
    // Draw range indicator
    window.draw(rangeIndicator_);
    
    // Draw shop
    window.draw(shopShape_);
    window.draw(shopText_);
    
    // Draw timer
    drawTimer(window);
    
    // Draw warning if needed
    if (showWarning_) {
        sf::FloatRect warningBounds = warningText_.getLocalBounds();
        sf::Vector2f warningPos = position_;
        warningPos.y -= 80.0f;
        warningPos.x -= warningBounds.width / 2.0f;
        
        sf::Text warningCopy = warningText_;
        warningCopy.setPosition(warningPos);
        window.draw(warningCopy);
    }
    
    // Note: Shop UI is drawn separately in main.cpp in UI view
}

void Shop::drawUI(sf::RenderWindow& window) const {
    if (showUI_) {
        drawShopUI(window);
    }
}

void Shop::handleInput(sf::Event& event, Player& player) {
    if (!showUI_ || !visible_) return;
    
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                handleCategoryChange(-1);
                break;
            case sf::Keyboard::D:
                handleCategoryChange(1);
                break;
            case sf::Keyboard::W:
                handleItemSelection(-1);
                break;
            case sf::Keyboard::S:
                handleItemSelection(1);
                break;
            case sf::Keyboard::E:
                handlePurchase(player);
                break;
            case sf::Keyboard::Q:
                showUI_ = false;
                break;
            default:
                break;
        }
    }
}

bool Shop::isPlayerInRange(const Player& player) const {
    sf::Vector2f playerPos = player.getPosition();
    float distance = std::sqrt(std::pow(playerPos.x - position_.x, 2) + 
                              std::pow(playerPos.y - position_.y, 2));
    return distance <= 100.0f; // Range indicator radius
}

void Shop::teleportToNewLocation() {
    generateRandomTeleportLocation();
    
    // Reset timer
    teleportTimer_ = 0.0f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(120.0f, 180.0f);
    teleportCooldown_ = dist(gen);
    
    showWarning_ = false;
    warningTimer_ = 0.0f;
}

void Shop::generateRandomTeleportLocation() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(100.0f, Config::WORLD_WIDTH - 100.0f);
    std::uniform_real_distribution<float> yDist(100.0f, Config::WORLD_HEIGHT - 100.0f);
    
    position_ = sf::Vector2f(xDist(gen), yDist(gen));
}

void Shop::updateTimer(float deltaTime) {
    teleportTimer_ += deltaTime;
    
    // Check for warning period (last 30 seconds)
    if (teleportTimer_ >= teleportCooldown_ - 30.0f && !showWarning_) {
        showWarning_ = true;
    }
    
    // Update warning timer
    if (showWarning_) {
        warningTimer_ += deltaTime;
    }
    
    // Teleport when timer expires
    if (teleportTimer_ >= teleportCooldown_) {
        teleportToNewLocation();
    }
}

void Shop::drawTimer(sf::RenderWindow& window) const {
    float timeRemaining = teleportCooldown_ - teleportTimer_;
    
    std::stringstream ss;
    if (timeRemaining > 60.0f) {
        int minutes = static_cast<int>(timeRemaining) / 60;
        int seconds = static_cast<int>(timeRemaining) % 60;
        ss << "Next teleport: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
    } else {
        ss << "Teleporting in: " << static_cast<int>(timeRemaining) << "s";
    }
    
    sf::Text timerCopy = timerText_;
    timerCopy.setString(ss.str());
    
    sf::FloatRect timerBounds = timerCopy.getLocalBounds();
    sf::Vector2f timerPos = position_;
    timerPos.y += 60.0f;
    timerPos.x -= timerBounds.width / 2.0f;
    timerCopy.setPosition(timerPos);
    
    window.draw(timerCopy);
}

void Shop::initializeInventory() {
    WeaponTierSystem& tierSystem = WeaponTierSystem::getInstance();
    
    // Add weapons from all tiers
    auto categories = tierSystem.getAllCategories();
    int weaponId = 1;
    
    for (int tierIndex = 0; tierIndex < 3; ++tierIndex) {
        WeaponTier tier = static_cast<WeaponTier>(tierIndex);
        
        for (auto category : categories) {
            WeaponStats stats = tierSystem.getWeaponStats(category, tier);
            
            ShopItem weapon(ItemType::WEAPON, stats.name, stats.description, stats.cost, weaponId);
            weapon.tier = tier;
            
            // Add base weapon
            weapons_.push_back(weapon);
            weaponId++;
            
            // Add upgraded versions
            for (int upgradeLevel = 1; upgradeLevel <= 3; ++upgradeLevel) {
                WeaponUpgrade upgrade = tierSystem.getUpgrade(upgradeLevel);
                WeaponStats upgradedStats = tierSystem.calculateUpgradedStats(stats, upgrade);
                
                ShopItem upgradedWeapon(ItemType::WEAPON, upgradedStats.name, upgradedStats.description, upgradedStats.cost, weaponId);
                upgradedWeapon.tier = tier;
                upgradedWeapon.upgradeLevel = upgradeLevel;
                
                weapons_.push_back(upgradedWeapon);
                weaponId++;
            }
        }
    }
    
    // Initialize talents
    talents_.push_back(ShopItem(ItemType::TALENT, "Gold Magnet", "Increases gold pickup range by 50%", 100, 101));
    talents_.push_back(ShopItem(ItemType::TALENT, "Battle Frenzy", "Attack speed bonus after kill", 150, 102));
    talents_.push_back(ShopItem(ItemType::TALENT, "Treasure Hunter", "Double gold from enemies", 200, 103));
    talents_.push_back(ShopItem(ItemType::TALENT, "Weapon Master", "Reduced weapon cooldowns by 25%", 175, 104));
    talents_.push_back(ShopItem(ItemType::TALENT, "Berserker Rage", "Damage increases as health decreases", 250, 105));
    talents_.push_back(ShopItem(ItemType::TALENT, "Lucky Strike", "Chance to deal critical damage", 300, 106));
    talents_.push_back(ShopItem(ItemType::TALENT, "Shield Bearer", "Reduces incoming damage by 20%", 350, 107));
    talents_.push_back(ShopItem(ItemType::TALENT, "Swift Feet", "Increases movement speed by 30%", 200, 108));
    
    refreshItemList();
}

void Shop::drawShopUI(sf::RenderWindow& window) const {
    // Position UI in center of screen
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f uiPos(windowSize.x / 2.0f - 300.0f, windowSize.y / 2.0f - 200.0f);
    
    sf::RectangleShape uiCopy = uiBackground_;
    uiCopy.setPosition(uiPos);
    window.draw(uiCopy);
    
    // Draw category header
    sf::Text categoryCopy = categoryText_;
    std::vector<std::string> categories = {"WEAPONS", "TALENTS", "UPGRADES"};
    categoryCopy.setString(categories[selectedCategory_]);
    categoryCopy.setPosition(uiPos.x + 20.0f, uiPos.y + 20.0f);
    window.draw(categoryCopy);
    
    // Draw item list
    sf::Text itemCopy = itemListText_;
    itemCopy.setString(formatItemList());
    itemCopy.setPosition(uiPos.x + 20.0f, uiPos.y + 60.0f);
    window.draw(itemCopy);
    
    // Draw details
    sf::Text detailsCopy = detailsText_;
    detailsCopy.setString(formatItemDetails());
    detailsCopy.setPosition(uiPos.x + 320.0f, uiPos.y + 60.0f);
    window.draw(detailsCopy);
    
    // Draw instructions
    sf::Text instructionsCopy = instructionsText_;
    instructionsCopy.setPosition(uiPos.x + 20.0f, uiPos.y + 360.0f);
    window.draw(instructionsCopy);
}

std::string Shop::formatItemList() const {
    std::stringstream ss;
    
    const std::vector<ShopItem>* currentItems = nullptr;
    
    switch (selectedCategory_) {
        case 0: currentItems = &weapons_; break;
        case 1: currentItems = &talents_; break;
        case 2: currentItems = &upgrades_; break;
    }
    
    if (currentItems) {
        for (size_t i = 0; i < currentItems->size(); ++i) {
            if (static_cast<int>(i) == selectedItem_) {
                ss << "> ";
            } else {
                ss << "  ";
            }
            ss << (*currentItems)[i].name << " - " << (*currentItems)[i].cost << " gold\n";
        }
    }
    
    return ss.str();
}

std::string Shop::formatItemDetails() const {
    const std::vector<ShopItem>* currentItems = nullptr;
    
    switch (selectedCategory_) {
        case 0: currentItems = &weapons_; break;
        case 1: currentItems = &talents_; break;
        case 2: currentItems = &upgrades_; break;
    }
    
    if (currentItems && selectedItem_ >= 0 && selectedItem_ < static_cast<int>(currentItems->size())) {
        const ShopItem& item = (*currentItems)[selectedItem_];
        std::stringstream ss;
        ss << "Name: " << item.name << "\n\n";
        ss << "Description:\n" << item.description << "\n\n";
        ss << "Cost: " << item.cost << " gold";
        return ss.str();
    }
    
    return "No item selected";
}

void Shop::handleCategoryChange(int direction) {
    selectedCategory_ = (selectedCategory_ + direction + 3) % 3;
    selectedItem_ = 0;
    refreshItemList();
}

void Shop::handleItemSelection(int direction) {
    const std::vector<ShopItem>* currentItems = nullptr;
    
    switch (selectedCategory_) {
        case 0: currentItems = &weapons_; break;
        case 1: currentItems = &talents_; break;
        case 2: currentItems = &upgrades_; break;
    }
    
    if (currentItems && !currentItems->empty()) {
        selectedItem_ = (selectedItem_ + direction + static_cast<int>(currentItems->size())) % static_cast<int>(currentItems->size());
    }
}

void Shop::handlePurchase(Player& player) {
    const std::vector<ShopItem>* currentItems = nullptr;
    
    switch (selectedCategory_) {
        case 0: currentItems = &weapons_; break;
        case 1: currentItems = &talents_; break;
        case 2: currentItems = &upgrades_; break;
    }
    
    if (currentItems && selectedItem_ >= 0 && selectedItem_ < static_cast<int>(currentItems->size())) {
        const ShopItem& item = (*currentItems)[selectedItem_];
        purchaseItem(item, player);
    }
}

bool Shop::purchaseItem(const ShopItem& item, Player& player) {
    if (player.getGold() >= item.cost) {
        player.addGold(-item.cost);
        
        // Handle different item types
        switch (item.type) {
            case ItemType::WEAPON: {
                // Create weapon using tier system
                WeaponTierSystem& tierSystem = WeaponTierSystem::getInstance();
                
                // Find the weapon category from the item name/id
                // For now, we'll use a simple mapping based on weapon ID
                auto categories = tierSystem.getAllCategories();
                int categoryIndex = (item.id - 1) % (static_cast<int>(categories.size()) * 4); // 4 = base + 3 upgrades
                int categoryBase = categoryIndex / 4;
                
                if (categoryBase < static_cast<int>(categories.size())) {
                    WeaponCategory category = categories[categoryBase];
                    auto weapon = tierSystem.createWeapon(category, item.tier, item.upgradeLevel);
                    player.addWeapon(std::move(weapon));
                }
                break;
            }
            case ItemType::TALENT:
                // Apply talent effects directly to player stats
                switch (item.id) {
                    case 101: // Gold Magnet - implement gold pickup range increase
                        player.addGoldMultiplier(0.5f);
                        break;
                    case 102: // Battle Frenzy - implement attack speed bonus
                        player.addAttackSpeedMultiplier(0.3f);
                        break;
                    case 103: // Treasure Hunter - double gold
                        player.addGoldMultiplier(1.0f);
                        break;
                    case 104: // Weapon Master - reduced cooldowns
                        player.addAttackSpeedMultiplier(0.25f);
                        break;
                    case 105: // Berserker Rage - damage based on missing health
                        player.addDamageMultiplier(0.5f);
                        break;
                    case 106: // Lucky Strike - critical chance
                        player.addCritChance(0.15f);
                        break;
                    case 107: // Shield Bearer - damage reduction
                        player.addArmor(20.0f);
                        break;
                    case 108: // Swift Feet - movement speed
                        player.setSpeed(player.getSpeed() * 1.3f);
                        break;
                }
                break;
            case ItemType::UPGRADE:
                // Apply upgrade (implementation needed)
                break;
        }
        
        return true;
    }
    
    return false;
}

void Shop::refreshItemList() {
    // Reset selection if out of bounds
    const std::vector<ShopItem>* currentItems = nullptr;
    
    switch (selectedCategory_) {
        case 0: currentItems = &weapons_; break;
        case 1: currentItems = &talents_; break;
        case 2: currentItems = &upgrades_; break;
    }
    
    if (currentItems && selectedItem_ >= static_cast<int>(currentItems->size())) {
        selectedItem_ = 0;
    }
}
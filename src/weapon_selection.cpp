#include "weapon_selection.h"
#include "player.h"
#include <sstream>

WeaponSelection::WeaponSelection() : 
    active_(false), selectionComplete_(false), selectedWeapon_(0) {
    
    // Load font
    if (!font_.loadFromFile("ARIAL.TTF")) {
        // Handle font loading error
    }
    
    // Initialize background
    background_.setSize(sf::Vector2f(800.0f, 600.0f));
    background_.setFillColor(sf::Color(0, 0, 0, 220));
    background_.setOutlineColor(sf::Color::White);
    background_.setOutlineThickness(3.0f);
    
    // Initialize text components
    titleText_.setFont(font_);
    titleText_.setString("Choose Your Starting Weapon");
    titleText_.setCharacterSize(32);
    titleText_.setFillColor(sf::Color::Yellow);
    titleText_.setStyle(sf::Text::Bold);
    
    instructionText_.setFont(font_);
    instructionText_.setString("Use W/S to navigate, E to select");
    instructionText_.setCharacterSize(18);
    instructionText_.setFillColor(sf::Color::Green);
    
    weaponListText_.setFont(font_);
    weaponListText_.setCharacterSize(16);
    weaponListText_.setFillColor(sf::Color::White);
    
    weaponDetailsText_.setFont(font_);
    weaponDetailsText_.setCharacterSize(14);
    weaponDetailsText_.setFillColor(sf::Color::Cyan);
    
    initializeAvailableWeapons();
    updateWeaponDisplay();
}

void WeaponSelection::draw(sf::RenderWindow& window) const {
    if (!active_) return;
    
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f center(windowSize.x / 2.0f, windowSize.y / 2.0f);
    
    // Draw background
    sf::RectangleShape bg = background_;
    bg.setPosition(center.x - 400.0f, center.y - 300.0f);
    window.draw(bg);
    
    // Draw title
    sf::Text title = titleText_;
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(center.x - titleBounds.width / 2.0f, center.y - 250.0f);
    window.draw(title);
    
    // Draw instructions
    sf::Text instructions = instructionText_;
    sf::FloatRect instrBounds = instructions.getLocalBounds();
    instructions.setPosition(center.x - instrBounds.width / 2.0f, center.y - 200.0f);
    window.draw(instructions);
    
    // Draw weapon list
    sf::Text weaponList = weaponListText_;
    weaponList.setPosition(center.x - 350.0f, center.y - 150.0f);
    window.draw(weaponList);
    
    // Draw weapon details
    sf::Text weaponDetails = weaponDetailsText_;
    weaponDetails.setPosition(center.x + 50.0f, center.y - 150.0f);
    window.draw(weaponDetails);
}

void WeaponSelection::handleInput(sf::Event& event, Player& player) {
    if (!active_ || selectionComplete_) return;
    
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                selectedWeapon_ = (selectedWeapon_ - 1 + static_cast<int>(availableWeapons_.size())) % static_cast<int>(availableWeapons_.size());
                updateWeaponDisplay();
                break;
                
            case sf::Keyboard::S:
                selectedWeapon_ = (selectedWeapon_ + 1) % static_cast<int>(availableWeapons_.size());
                updateWeaponDisplay();
                break;
                
            case sf::Keyboard::E:
                selectWeapon(player);
                break;
                
            default:
                break;
        }
    }
}

void WeaponSelection::update(float deltaTime) {
    // Can add animations or other updates here if needed
}

void WeaponSelection::initializeAvailableWeapons() {
    // Only include basic tier weapons that are suitable for starting
    availableWeapons_ = {
        WeaponCategory::SWORD,
        WeaponCategory::DAGGER,
        WeaponCategory::BOW,
        WeaponCategory::CROSSBOW
    };
}

void WeaponSelection::updateWeaponDisplay() {
    weaponListText_.setString(formatWeaponList());
    weaponDetailsText_.setString(formatWeaponDetails());
}

std::string WeaponSelection::formatWeaponList() const {
    std::stringstream ss;
    ss << "Available Weapons:\n\n";
    
    WeaponTierSystem& tierSystem = WeaponTierSystem::getInstance();
    
    for (size_t i = 0; i < availableWeapons_.size(); ++i) {
        if (static_cast<int>(i) == selectedWeapon_) {
            ss << "> ";
        } else {
            ss << "  ";
        }
        
        WeaponStats stats = tierSystem.getWeaponStats(availableWeapons_[i], WeaponTier::BASIC);
        ss << stats.name << "\n";
    }
    
    return ss.str();
}

std::string WeaponSelection::formatWeaponDetails() const {
    if (selectedWeapon_ >= 0 && selectedWeapon_ < static_cast<int>(availableWeapons_.size())) {
        WeaponTierSystem& tierSystem = WeaponTierSystem::getInstance();
        WeaponStats stats = tierSystem.getWeaponStats(availableWeapons_[selectedWeapon_], WeaponTier::BASIC);
        
        std::stringstream ss;
        ss << "Weapon Details:\n\n";
        ss << "Name: " << stats.name << "\n\n";
        ss << "Description:\n" << stats.description << "\n\n";
        ss << "Stats:\n";
        ss << "• Damage: " << stats.baseDamage << "\n";
        ss << "• Cooldown: " << stats.baseCooldown << "s\n";
        ss << "• Range: " << static_cast<int>(stats.baseRange) << "\n\n";
        ss << "Category: " << tierSystem.getCategoryName(availableWeapons_[selectedWeapon_]) << "\n";
        ss << "Tier: " << tierSystem.getTierName(WeaponTier::BASIC);
        
        return ss.str();
    }
    
    return "No weapon selected";
}

void WeaponSelection::selectWeapon(Player& player) {
    if (selectedWeapon_ >= 0 && selectedWeapon_ < static_cast<int>(availableWeapons_.size())) {
        WeaponTierSystem& tierSystem = WeaponTierSystem::getInstance();
        
        // Create the selected weapon
        auto weapon = tierSystem.createWeapon(availableWeapons_[selectedWeapon_], WeaponTier::BASIC, 0);
        
        // Clear player's existing weapons and add the selected one
        player.clearWeapons();
        player.addWeapon(std::move(weapon));
        
        selectionComplete_ = true;
        active_ = false;
    }
}
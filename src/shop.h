#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "weapon_tier_system.h"

class Player;
class Weapon;
class Talent;

enum class ItemType {
    WEAPON,
    TALENT,
    UPGRADE
};

struct ShopItem {
    ItemType type;
    std::string name;
    std::string description;
    int cost;
    int id;
    WeaponTier tier = WeaponTier::BASIC;
    int upgradeLevel = 0; // 0-3, where 0 is base weapon
    
    ShopItem(ItemType t, const std::string& n, const std::string& desc, int c, int itemId)
        : type(t), name(n), description(desc), cost(c), id(itemId) {}
};

class Shop {
public:
    Shop();
    
    void update(float deltaTime, const Player& player);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const;
    void handleInput(sf::Event& event, Player& player);
    
    bool isVisible() const { return visible_; }
    bool isUIShowing() const { return showUI_; }
    bool isPlayerInRange(const Player& player) const;
    sf::Vector2f getPosition() const { return position_; }
    
    // Shop management
    void teleportToNewLocation();
    void setVisible(bool visible) { visible_ = visible; }
    void openUI() { showUI_ = true; refreshItemList(); }
    
private:
    // Shop state
    sf::Vector2f position_;
    bool visible_;
    float teleportTimer_;
    float teleportCooldown_;
    float warningTimer_;
    bool showWarning_;
    
    // Visual components
    sf::RectangleShape shopShape_;
    sf::CircleShape rangeIndicator_;
    sf::Text shopText_;
    sf::Text timerText_;
    sf::Text warningText_;
    sf::Font font_;
    
    // Shop inventory
    std::vector<ShopItem> weapons_;
    std::vector<ShopItem> talents_;
    std::vector<ShopItem> upgrades_;
    
    // UI state
    int selectedCategory_; // 0=weapons, 1=talents, 2=upgrades
    int selectedItem_;
    bool showUI_;
    
    // UI components
    sf::RectangleShape uiBackground_;
    sf::Text categoryText_;
    sf::Text itemListText_;
    sf::Text detailsText_;
    sf::Text instructionsText_;
    
    // Private methods
    void initializeInventory();
    void generateRandomTeleportLocation();
    void drawShopUI(sf::RenderWindow& window) const;
    void drawTimer(sf::RenderWindow& window) const;
    void updateTimer(float deltaTime);
    bool purchaseItem(const ShopItem& item, Player& player);
    void refreshItemList();
    std::string formatItemList() const;
    std::string formatItemDetails() const;
    
    // Input handling
    void handleCategoryChange(int direction);
    void handleItemSelection(int direction);
    void handlePurchase(Player& player);
};
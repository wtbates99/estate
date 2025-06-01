#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Player;
class Enemy;

enum class TalentCategory {
    COMBAT,
    DEFENSIVE, 
    UTILITY,
    WEAPON_MASTERY
};

enum class TalentRarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

// Base talent class
class Talent {
public:
    Talent(const std::string& name, const std::string& description, 
           TalentCategory category, TalentRarity rarity);
    virtual ~Talent() = default;

    // Pure virtual methods that must be implemented
    virtual void apply(Player& player) = 0;
    virtual void onLevelUp(Player& player) {};  // Optional override for level-specific effects
    virtual std::unique_ptr<Talent> createUpgrade() const = 0;  // For talent evolution/upgrades

    // Getters
    const std::string& getName() const { return name_; }
    const std::string& getDescription() const { return description_; }
    TalentCategory getCategory() const { return category_; }
    TalentRarity getRarity() const { return rarity_; }
    int getLevel() const { return level_; }
    int getMaxLevel() const { return maxLevel_; }
    bool canUpgrade() const { return level_ < maxLevel_; }

    // Leveling
    void levelUp(Player& player);
    
    // UI
    virtual void draw(sf::RenderWindow& window, const sf::Vector2f& position, bool isSelected = false) const;
    sf::Color getRarityColor() const;

protected:
    std::string name_;
    std::string description_;
    TalentCategory category_;
    TalentRarity rarity_;
    int level_;
    int maxLevel_;
    
    // Visual representation
    mutable sf::Font font_;
    mutable bool fontLoaded_;
    void loadFont() const;
};

// Talent tree node structure
struct TalentNode {
    std::unique_ptr<Talent> talent;
    std::vector<int> prerequisites;  // Indices of required talents
    std::vector<int> unlocks;        // Indices of talents this unlocks
    bool isUnlocked;
    bool isSelected;
    sf::Vector2f position;          // Position in talent tree UI
    
    TalentNode() : isUnlocked(false), isSelected(false) {}
};

// Talent tree management
class TalentTree {
public:
    TalentTree();
    ~TalentTree() = default;

    // Tree generation (randomly generated patterns)
    void generateRandomTree(int playerLevel);
    void generatePermanentTree();  // Generate the tree once at game start
    void clear();

    // Talent selection
    std::vector<int> getAvailableTalents() const;
    bool selectTalent(int nodeIndex, Player& player);
    
    // Tree management
    void unlockTopRow();  // Unlock the top row of talents when leveling up
    
    // UI and drawing
    void draw(sf::RenderWindow& window, const sf::Vector2f& offset = sf::Vector2f(0, 0)) const;
    void drawConnections(sf::RenderWindow& window, const sf::Vector2f& offset) const;
    
    // Getters
    const std::vector<TalentNode>& getNodes() const { return nodes_; }
    int getSelectedTalentCount() const;
    bool isEmpty() const { return nodes_.empty(); }

private:
    std::vector<TalentNode> nodes_;
    sf::Vector2f treeSize_;  // Total size of the talent tree layout
    
    // Tree generation helpers
    void addTalentNode(std::unique_ptr<Talent> talent, const sf::Vector2f& position);
    void addDependency(int fromIndex, int toIndex);
    void calculateLayout();
    void validateTree();  // Ensure tree structure is valid
    std::unique_ptr<Talent> createTalentByRarity(TalentCategory category, TalentRarity minRarity, TalentRarity maxRarity);
}; 
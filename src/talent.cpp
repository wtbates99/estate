#include "talent.h"
#include "player.h"
#include "talents/talent_factory.h"
#include <cmath>
#include <algorithm>
#include <functional>

// Talent implementation
Talent::Talent(const std::string& name, const std::string& description, 
               TalentCategory category, TalentRarity rarity)
    : name_(name), description_(description), category_(category), 
      rarity_(rarity), level_(0), maxLevel_(3), fontLoaded_(false) {
}

void Talent::levelUp(Player& player) {
    if (canUpgrade()) {
        level_++;
        onLevelUp(player);
    }
}

void Talent::draw(sf::RenderWindow& window, const sf::Vector2f& position, bool isSelected) const {
    loadFont();
    
    // Draw talent box
    sf::RectangleShape box(sf::Vector2f(180.f, 120.f));
    box.setPosition(position);
    box.setFillColor(isSelected ? sf::Color(100, 100, 100, 200) : sf::Color(50, 50, 50, 200));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(getRarityColor());
    window.draw(box);
    
    if (fontLoaded_) {
        // Draw talent name
        sf::Text nameText;
        nameText.setFont(font_);
        nameText.setString(name_);
        nameText.setCharacterSize(14);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(position.x + 5, position.y + 5);
        window.draw(nameText);
        
        // Draw level indicator
        if (level_ > 0) {
            sf::Text levelText;
            levelText.setFont(font_);
            levelText.setString("Lv " + std::to_string(level_) + "/" + std::to_string(maxLevel_));
            levelText.setCharacterSize(10);
            levelText.setFillColor(sf::Color::Yellow);
            levelText.setPosition(position.x + 5, position.y + 25);
            window.draw(levelText);
        }
        
        // Draw description (wrapped)
        sf::Text descText;
        descText.setFont(font_);
        descText.setString(description_);
        descText.setCharacterSize(10);
        descText.setFillColor(sf::Color(200, 200, 200));
        descText.setPosition(position.x + 5, position.y + 45);
        window.draw(descText);
    }
}

sf::Color Talent::getRarityColor() const {
    switch (rarity_) {
        case TalentRarity::COMMON: return sf::Color::White;
        case TalentRarity::UNCOMMON: return sf::Color::Green;
        case TalentRarity::RARE: return sf::Color::Blue;
        case TalentRarity::EPIC: return sf::Color::Magenta;
        case TalentRarity::LEGENDARY: return sf::Color::Yellow;
        default: return sf::Color::White;
    }
}

void Talent::loadFont() const {
    if (!fontLoaded_) {
        if (font_.loadFromFile("ARIAL.TTF")) {
            fontLoaded_ = true;
        }
    }
}

// TalentTree implementation
TalentTree::TalentTree() : treeSize_(800.f, 600.f) {
}

void TalentTree::generatePermanentTree() {
    clear();
    
    // Define the 3x3 grid structure
    const int COLUMNS = 3;
    const int ROWS = 3;
    const float COLUMN_WIDTH = 220.0f;
    const float ROW_HEIGHT = 140.0f;
    const float START_X = 80.0f;
    const float START_Y = 80.0f;
    
    // Column categories
    TalentCategory categories[COLUMNS] = {
        TalentCategory::COMBAT,
        TalentCategory::DEFENSIVE,
        TalentCategory::UTILITY
    };
    
    // Generate talents for each column with proper rarity progression
    for (int col = 0; col < COLUMNS; col++) {
        for (int row = 0; row < ROWS; row++) {
            std::unique_ptr<Talent> talent;
            
            if (row == 0) {
                // Top row: Common talents
                talent = createTalentByRarity(categories[col], TalentRarity::COMMON, TalentRarity::COMMON);
            } else if (row == 1) {
                // Middle row: Uncommon talents
                talent = createTalentByRarity(categories[col], TalentRarity::UNCOMMON, TalentRarity::UNCOMMON);
            } else {
                // Bottom row: Rare/Epic talents
                talent = createTalentByRarity(categories[col], TalentRarity::RARE, TalentRarity::EPIC);
            }
            
            // Calculate position
            sf::Vector2f position(
                START_X + col * COLUMN_WIDTH,
                START_Y + row * ROW_HEIGHT
            );
            
            // Add talent to tree
            addTalentNode(std::move(talent), position);
        }
    }
    
    validateTree();
}

void TalentTree::generateRandomTree(int playerLevel) {
    clear();
    
    // Define the 3x3 grid structure
    const int COLUMNS = 3;
    const int ROWS = 3;
    const float COLUMN_WIDTH = 220.0f;  // Reduced width for better screen fit
    const float ROW_HEIGHT = 140.0f;    // Reduced height for better screen fit
    const float START_X = 80.0f;        // Adjusted start position
    const float START_Y = 80.0f;        // Adjusted start position
    
    // Column categories
    TalentCategory categories[COLUMNS] = {
        TalentCategory::COMBAT,
        TalentCategory::DEFENSIVE,
        TalentCategory::UTILITY
    };
    
    // Generate talents for each column
    for (int col = 0; col < COLUMNS; col++) {
        // Fill pool with talents of increasing rarity for each row
        for (int row = 0; row < ROWS; row++) {
            std::unique_ptr<Talent> talent;
            
            if (row == 0) {
                // Top row: Common/Uncommon talents
                talent = createTalentByRarity(categories[col], TalentRarity::COMMON, TalentRarity::UNCOMMON);
            } else if (row == 1) {
                // Middle row: Uncommon/Rare talents
                talent = createTalentByRarity(categories[col], TalentRarity::UNCOMMON, TalentRarity::RARE);
            } else {
                // Bottom row: Rare/Epic talents
                talent = createTalentByRarity(categories[col], TalentRarity::RARE, TalentRarity::EPIC);
            }
            
            // Calculate position
            sf::Vector2f position(
                START_X + col * COLUMN_WIDTH,
                START_Y + row * ROW_HEIGHT
            );
            
            // Add talent to tree
            addTalentNode(std::move(talent), position);
        }
    }
    
    // Set up correct dependencies - each talent depends on the one directly above it in the same column
    for (int col = 0; col < COLUMNS; col++) {
        for (int row = 1; row < ROWS; row++) {  // Start from row 1 (second row)
            int currentIndex = col * ROWS + row;        // Current talent index
            int prerequisiteIndex = col * ROWS + (row - 1);  // Talent directly above it
            
            if (prerequisiteIndex >= 0 && prerequisiteIndex < static_cast<int>(nodes_.size()) &&
                currentIndex >= 0 && currentIndex < static_cast<int>(nodes_.size())) {
                addDependency(prerequisiteIndex, currentIndex);
            }
        }
    }
    
    // IMPORTANT: Only unlock top row talents when player levels up, not immediately
    // This will be handled by the level up process
    
    validateTree();
}

void TalentTree::clear() {
    nodes_.clear();
}

std::vector<int> TalentTree::getAvailableTalents() const {
    std::vector<int> available;
    
    for (int i = 0; i < static_cast<int>(nodes_.size()); i++) {
        const auto& node = nodes_[i];
        
        // In tier-based system, a talent is available if it's unlocked and not already selected
        if (node.isUnlocked && !node.isSelected) {
            available.push_back(i);
        }
    }
    
    return available;
}

bool TalentTree::selectTalent(int nodeIndex, Player& player) {
    if (nodeIndex < 0 || nodeIndex >= static_cast<int>(nodes_.size())) {
        return false;
    }
    
    auto& node = nodes_[nodeIndex];
    
    // Check if talent is available for selection (unlocked and not selected)
    if (!node.isUnlocked || node.isSelected) {
        return false;
    }
    
    // Select the talent and apply its effect
    node.isSelected = true;
    node.talent->apply(player);
    
    // Simple tier unlocking: unlock the next talent in the same column
    const int ROWS = 3;
    int column = nodeIndex / ROWS;
    int row = nodeIndex % ROWS;
    
    // Debug output (can be removed later)
    #ifdef DEBUG_TALENTS
    std::cout << "Selected talent " << nodeIndex << " (Col " << column << ", Row " << row << ")" << std::endl;
    #endif
    
    // If this is not the bottom row, unlock the next talent in this column
    if (row < ROWS - 1) {
        int nextTalentIndex = column * ROWS + (row + 1);
        if (nextTalentIndex < static_cast<int>(nodes_.size())) {
            nodes_[nextTalentIndex].isUnlocked = true;
            #ifdef DEBUG_TALENTS
            std::cout << "Unlocked next talent " << nextTalentIndex << std::endl;
            #endif
        }
    }
    
    return true;
}

void TalentTree::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    // Draw column headers first
    sf::Font font;
    bool fontLoaded = font.loadFromFile("ARIAL.TTF");
    
    if (fontLoaded) {
        std::string headers[3] = {"COMBAT", "DEFENSIVE", "UTILITY"};
        sf::Color headerColors[3] = {
            sf::Color::Red,
            sf::Color::Blue, 
            sf::Color::Green
        };
        
        const float COLUMN_WIDTH = 220.0f;  // Match the positioning from generateRandomTree
        const float START_X = 80.0f;
        
        for (int col = 0; col < 3; col++) {
            sf::Text headerText;
            headerText.setFont(font);
            headerText.setString(headers[col]);
            headerText.setCharacterSize(18);  // Slightly smaller for better fit
            headerText.setFillColor(headerColors[col]);
            headerText.setStyle(sf::Text::Bold);
            
            // Position header above each column, centered
            sf::FloatRect textBounds = headerText.getLocalBounds();
            sf::Vector2f headerPos(
                START_X + col * COLUMN_WIDTH + (180.0f - textBounds.width) / 2,  // Center text in talent box width
                40.0f  // Above the talents
            );
            headerText.setPosition(headerPos + offset);
            window.draw(headerText);
        }
    }
    
    // Draw connections first (so they appear behind nodes)
    drawConnections(window, offset);
    
    // Draw talent nodes
    auto available = getAvailableTalents();
    
    for (int i = 0; i < static_cast<int>(nodes_.size()); i++) {
        const auto& node = nodes_[i];
        bool isAvailable = std::find(available.begin(), available.end(), i) != available.end();
        
        if (node.talent) {
            sf::Vector2f drawPos = node.position + offset;
            
            // Modify appearance based on state
            if (!node.isUnlocked) {
                // Draw grayed out for locked talents
                sf::RectangleShape lockBox(sf::Vector2f(180.f, 120.f));
                lockBox.setPosition(drawPos);
                lockBox.setFillColor(sf::Color(30, 30, 30, 150));
                lockBox.setOutlineThickness(2.f);
                lockBox.setOutlineColor(sf::Color(80, 80, 80));
                window.draw(lockBox);
                
                // Draw lock icon or text
                if (fontLoaded) {
                    sf::Text lockText;
                    lockText.setFont(font);
                    lockText.setString("LOCKED");
                    lockText.setCharacterSize(14);
                    lockText.setFillColor(sf::Color(120, 120, 120));
                    lockText.setPosition(drawPos.x + 55, drawPos.y + 50);
                    window.draw(lockText);
                    
                    // Show talent name even when locked
                    sf::Text nameText;
                    nameText.setFont(font);
                    nameText.setString(node.talent->getName());
                    nameText.setCharacterSize(12);
                    nameText.setFillColor(sf::Color(100, 100, 100));
                    nameText.setPosition(drawPos.x + 5, drawPos.y + 5);
                    window.draw(nameText);
                }
            } else {
                node.talent->draw(window, drawPos, isAvailable);
            }
        }
    }
}

void TalentTree::drawConnections(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    const int COLUMNS = 3;
    const int ROWS = 3;
    
    // Draw vertical connections within each column (tier progression)
    for (int col = 0; col < COLUMNS; col++) {
        for (int row = 0; row < ROWS - 1; row++) {
            int currentIndex = col * ROWS + row;
            int nextIndex = col * ROWS + (row + 1);
            
            if (currentIndex < static_cast<int>(nodes_.size()) && 
                nextIndex < static_cast<int>(nodes_.size())) {
                
                const auto& currentNode = nodes_[currentIndex];
                const auto& nextNode = nodes_[nextIndex];
                
                sf::Vector2f fromPos = currentNode.position + offset + sf::Vector2f(90.f, 120.f); // Bottom center of current node
                sf::Vector2f toPos = nextNode.position + offset + sf::Vector2f(90.f, 0.f); // Top center of next node
                
                // Determine line color based on selection status
                sf::Color lineColor = sf::Color(100, 100, 100); // Default gray
                if (currentNode.isSelected) {
                    lineColor = sf::Color::Green; // Green if current tier is selected
                } else if (currentNode.isUnlocked) {
                    lineColor = sf::Color(150, 150, 150); // Lighter gray if unlocked
                }
                
                // Draw line between tiers
                sf::Vertex line[] = {
                    sf::Vertex(fromPos, lineColor),
                    sf::Vertex(toPos, lineColor)
                };
                window.draw(line, 2, sf::Lines);
                
                // Draw arrow head pointing down
                sf::Vector2f arrowPos = toPos + sf::Vector2f(0.f, -15.f);  // Position arrow above target
                
                sf::CircleShape arrow(5.f);
                arrow.setPosition(arrowPos - sf::Vector2f(5.f, 5.f));
                arrow.setFillColor(lineColor);
                window.draw(arrow);
            }
        }
    }
}

int TalentTree::getSelectedTalentCount() const {
    int count = 0;
    for (const auto& node : nodes_) {
        if (node.isSelected) {
            count++;
        }
    }
    return count;
}

void TalentTree::addTalentNode(std::unique_ptr<Talent> talent, const sf::Vector2f& position) {
    TalentNode node;
    node.talent = std::move(talent);
    node.position = position;
    nodes_.push_back(std::move(node));
}

void TalentTree::addDependency(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < static_cast<int>(nodes_.size()) &&
        toIndex >= 0 && toIndex < static_cast<int>(nodes_.size()) &&
        fromIndex != toIndex) {
        
        nodes_[fromIndex].unlocks.push_back(toIndex);
        nodes_[toIndex].prerequisites.push_back(fromIndex);
    }
}

void TalentTree::calculateLayout() {
    // This could be enhanced with more sophisticated layout algorithms
    // For now, we use the grid-based approach in generateRandomTree
}

void TalentTree::validateTree() {
    // For the tier-based system, we don't need complex dependencies
    // Just ensure all talents are properly initialized
    const int COLUMNS = 3;
    const int ROWS = 3;
    
    // Clear any existing dependencies since we use tier-based progression
    for (auto& node : nodes_) {
        node.prerequisites.clear();
        node.unlocks.clear();
    }
    
    // Debug: Print tree structure (can be removed later)
    #ifdef DEBUG_TALENTS
    for (int i = 0; i < static_cast<int>(nodes_.size()); i++) {
        const auto& node = nodes_[i];
        int col = i / ROWS;
        int row = i % ROWS;
        std::cout << "Talent " << i << " (Col " << col << ", Row " << row << ") (" << node.talent->getName() << "): ";
        std::cout << "Unlocked: " << (node.isUnlocked ? "YES" : "NO") << std::endl;
    }
    #endif
}

// Helper function to create talents by rarity range
std::unique_ptr<Talent> TalentTree::createTalentByRarity(TalentCategory category, TalentRarity minRarity, TalentRarity maxRarity) {
    // Create pools of talents for each rarity level
    std::vector<std::function<std::unique_ptr<Talent>()>> talentPool;
    
    switch (category) {
        case TalentCategory::COMBAT:
            // Add talents based on desired rarity range
            if (static_cast<int>(TalentRarity::COMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::COMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createDamageBoostTalent);
                talentPool.push_back(createAttackSpeedTalent);
            }
            if (static_cast<int>(TalentRarity::UNCOMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::UNCOMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createCriticalStrikeTalent);
                talentPool.push_back(createWeaponMasteryTalent);
            }
            if (static_cast<int>(TalentRarity::RARE) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::RARE) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createVampirismTalent);
                talentPool.push_back(createDoubleStrikeTalent);
            }
            if (static_cast<int>(TalentRarity::EPIC) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::EPIC) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createBerserkTalent);
                talentPool.push_back(createExecutionerTalent);
            }
            break;
            
        case TalentCategory::DEFENSIVE:
            // Add defensive talents by rarity (assuming similar distribution)
            if (static_cast<int>(TalentRarity::COMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::COMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createHealthBoostTalent);
                talentPool.push_back(createArmorTalent);
            }
            if (static_cast<int>(TalentRarity::UNCOMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::UNCOMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createRegenerationTalent);
            }
            if (static_cast<int>(TalentRarity::RARE) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::RARE) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createShieldTalent);
            }
            if (static_cast<int>(TalentRarity::EPIC) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::EPIC) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createDodgeTalent);
            }
            break;
            
        case TalentCategory::UTILITY:
            // Add utility talents by rarity (assuming similar distribution)
            if (static_cast<int>(TalentRarity::COMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::COMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createSpeedBoostTalent);
                talentPool.push_back(createExperienceBoostTalent);
            }
            if (static_cast<int>(TalentRarity::UNCOMMON) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::UNCOMMON) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createGoldFindTalent);
            }
            if (static_cast<int>(TalentRarity::RARE) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::RARE) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createLuckyDropsTalent);
            }
            if (static_cast<int>(TalentRarity::EPIC) >= static_cast<int>(minRarity) && 
                static_cast<int>(TalentRarity::EPIC) <= static_cast<int>(maxRarity)) {
                talentPool.push_back(createMinimapMasteryTalent);
            }
            break;
            
        default:
            return createDamageBoostTalent(); // Fallback
    }
    
    if (talentPool.empty()) {
        // Fallback if no talents match the rarity range
        return createDamageBoostTalent();
    }
    
    // Randomly select from the filtered pool
    int randomIndex = rand() % talentPool.size();
    return talentPool[randomIndex]();
}

// New method to unlock the top row when player levels up
void TalentTree::unlockTopRow() {
    const int COLUMNS = 3;
    const int ROWS = 3;
    
    // Unlock all top row talents (first talent in each column)
    for (int col = 0; col < COLUMNS; col++) {
        int topRowIndex = col * ROWS;  // First talent in each column
        if (topRowIndex < static_cast<int>(nodes_.size())) {
            nodes_[topRowIndex].isUnlocked = true;
        }
    }
}


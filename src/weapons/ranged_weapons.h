#pragma once
#include "../weapon.h"

// Ranged weapon implementations
class Bow : public RangedWeapon {
public:
    Bow();
    void loadAssets();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    std::string getName() const override { return "Bow"; }

private:
    sf::Texture weaponTexture_;
    sf::Sprite weaponSprite_;
    bool textureLoaded_;
};

class Crossbow : public RangedWeapon {
public:
    Crossbow();
    void loadAssets();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    std::string getName() const override { return "Crossbow"; }

private:
    sf::Texture weaponTexture_;
    sf::Sprite weaponSprite_;
    bool textureLoaded_;
}; 
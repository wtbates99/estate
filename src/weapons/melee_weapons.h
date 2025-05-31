#pragma once
#include "../weapon.h"

// Melee weapon implementations
class Sword : public MeleeWeapon {
public:
    Sword();
    void loadAssets();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    std::string getName() const override { return "Sword"; }

private:
    sf::Texture weaponTexture_;
    sf::Sprite weaponSprite_;
    bool textureLoaded_;
};

class Dagger : public MeleeWeapon {
public:
    Dagger();
    void loadAssets();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const override;
    std::string getName() const override { return "Dagger"; }

private:
    sf::Texture weaponTexture_;
    sf::Sprite weaponSprite_;
    bool textureLoaded_;
}; 
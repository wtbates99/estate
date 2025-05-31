#include "background_types.h"

std::vector<BackgroundInfo> getAvailableBackgrounds() {
    return {
        BackgroundInfo(BackgroundType::FOREST, "Forest", "Dense woodland with tall trees and green foliage", "assets/backgrounds/forest.png"),
        BackgroundInfo(BackgroundType::DESERT, "Desert", "Sandy dunes under a burning sun", "assets/backgrounds/desert.png"),
        BackgroundInfo(BackgroundType::OCEAN, "Ocean", "Deep blue waters with waves and islands", "assets/backgrounds/ocean.png"),
        BackgroundInfo(BackgroundType::MOUNTAINS, "Mountains", "Snow-capped peaks and rocky terrain", "assets/backgrounds/mountains.png"),
        BackgroundInfo(BackgroundType::CITY, "City", "Urban landscape with buildings and streets", "assets/backgrounds/city.png"),
        BackgroundInfo(BackgroundType::SPACE, "Space", "Cosmic void with stars and nebulae", "assets/backgrounds/space.png"),
        BackgroundInfo(BackgroundType::CUSTOM, "Custom", "Your own custom background", "assets/background.png")
    };
}

BackgroundInfo getBackgroundInfo(BackgroundType type) {
    auto backgrounds = getAvailableBackgrounds();
    for (const auto& bg : backgrounds) {
        if (bg.type == type) {
            return bg;
        }
    }
    // Default to forest if not found
    return backgrounds[0];
} 
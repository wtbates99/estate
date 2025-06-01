#include "background_factory.h"
#include <random>
#include <iostream>

std::unique_ptr<Background> createBackground(BackgroundType type) {
    auto background = std::make_unique<Background>();
    BackgroundInfo info = getBackgroundInfo(type);
    
    if (!background->loadFromFile(info.filename)) {
        return nullptr;
    }
    
    return background;
}

std::unique_ptr<Background> createRandomBackground() {
    auto backgrounds = getAvailableBackgrounds();
    
    // Don't include CUSTOM in random selection
    std::vector<BackgroundInfo> selectableBackgrounds;
    for (const auto& bg : backgrounds) {
        if (bg.type != BackgroundType::CUSTOM) {
            selectableBackgrounds.push_back(bg);
        }
    }
    
    if (selectableBackgrounds.empty()) {
        return nullptr;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, selectableBackgrounds.size() - 1);
    
    BackgroundType randomType = selectableBackgrounds[dis(gen)].type;
    return createBackground(randomType);
}

std::unique_ptr<Background> createBackgroundWithFallback(BackgroundType primaryType, BackgroundType fallbackType) {
    auto background = createBackground(primaryType);
    if (background) {
        return background;
    }
    
    return createBackground(fallbackType);
} 

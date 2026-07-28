#pragma once

#include <memory>
#include "../background.h"
#include "background_types.h"

// Factory function for creating backgrounds by type
std::unique_ptr<Background> createBackground(BackgroundType type);

// Factory function for creating a random background
std::unique_ptr<Background> createRandomBackground();

// Factory function for creating background with fallback
std::unique_ptr<Background> createBackgroundWithFallback(BackgroundType primaryType, BackgroundType fallbackType = BackgroundType::FOREST); 
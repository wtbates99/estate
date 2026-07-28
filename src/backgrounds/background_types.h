#pragma once

#include "../background.h"
#include <string>
#include <vector>

// Background type enumeration
enum class BackgroundType {
    FOREST,
    DESERT,
    OCEAN,
    MOUNTAINS,
    CITY,
    SPACE,
    CUSTOM
};

// Background information structure
struct BackgroundInfo {
    BackgroundType type;
    std::string name;
    std::string description;
    std::string filename;
    
    BackgroundInfo(BackgroundType t, const std::string& n, const std::string& d, const std::string& f)
        : type(t), name(n), description(d), filename(f) {}
};

// Get all available backgrounds
std::vector<BackgroundInfo> getAvailableBackgrounds();

// Get background info by type
BackgroundInfo getBackgroundInfo(BackgroundType type); 
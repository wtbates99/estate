#pragma once

// Development configuration
namespace Config {
    // Debug settings
    constexpr bool DEBUG_MODE = true;
    constexpr bool SHOW_FPS = true;
    constexpr bool SHOW_COLLISION_BOXES = true;
    
    // Game settings
    constexpr float PLAYER_SPEED = 200.0f;
    constexpr float ENEMY_SPEED = 200.0f;
    constexpr int PLAYER_START_HEALTH = 100;
    constexpr int ENEMY_START_HEALTH = 100;
    constexpr int ENEMY_DAMAGE = 10;
    
    // Window settings
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr char WINDOW_TITLE[] = "Estate (Dev Build)";

    // World boundaries
    constexpr float WORLD_WIDTH = 5000.0f;
    constexpr float WORLD_HEIGHT = 5000.0f;
    
    // Minimap settings
    constexpr float MINIMAP_SIZE = 150.0f;
    constexpr float MINIMAP_PADDING = 10.0f;
    constexpr float MINIMAP_SCALE = 0.03f;  // Scale factor to fit world into minimap
    constexpr float MINIMAP_PLAYER_SIZE = 4.0f;
    constexpr float MINIMAP_ENEMY_SIZE = 3.0f;
} 
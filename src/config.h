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
} 
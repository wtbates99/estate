cmake_minimum_required(VERSION 3.10)
project(estate)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help CMake find SFML on macOS (Homebrew) and Linux
if(APPLE)
    # macOS: Point to Homebrew's sfml@2
    set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2;/usr/local/opt/sfml@2" ${CMAKE_PREFIX_PATH})
elseif(UNIX AND NOT APPLE)
    # Linux: Use pkg-config or standard paths
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SFML sfml-all>=2.5)
    endif()
endif()

# Set SFML to use shared libraries by default
set(SFML_STATIC_LIBRARIES FALSE)

# Find SFML (fall back to find_package if pkg-config not used)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/player.cpp
    src/enemy.cpp
    src/weapon.cpp
    src/minimap.cpp
    src/background.cpp
    src/background_menu.cpp
    src/talent.cpp
    src/backgrounds/background_types.cpp
    src/backgrounds/background_factory.cpp
    src/enemies/basic_enemies.cpp
    src/enemies/combat_enemies.cpp
    src/enemies/special_enemies.cpp
    src/enemies/enemy_factory.cpp
    src/weapons/melee_weapons.cpp
    src/weapons/ranged_weapons.cpp
    src/weapons/weapon_factory.cpp
    src/talents/combat_talents.cpp
    src/talents/defensive_talents.cpp
    src/talents/utility_talents.cpp
    src/talents/talent_factory.cpp
)

# Add header files
set(HEADERS
    src/player.h
    src/enemy.h
    src/weapon.h
    src/minimap.h
    src/background.h
    src/background_menu.h
    src/talent.h
    src/backgrounds/background_types.h
    src/backgrounds/background_factory.h
    src/enemies/basic_enemies.h
    src/enemies/combat_enemies.h
    src/enemies/special_enemies.h
    src/enemies/enemy_factory.h
    src/weapons/melee_weapons.h
    src/weapons/ranged_weapons.h
    src/weapons/weapon_factory.h
    src/talents/combat_talents.h
    src/talents/defensive_talents.h
    src/talents/utility_talents.h
    src/talents/talent_factory.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

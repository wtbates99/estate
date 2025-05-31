#!/usr/bin/env python3
"""
Background Generator for Estate Game
Generates 5000x5000 top-down game map backgrounds for all themes
"""

import os
import random
import math
from PIL import Image, ImageDraw, ImageFilter
import numpy as np

# Constants
BACKGROUND_SIZE = (5000, 5000)
OUTPUT_DIR = "assets/backgrounds"

def ensure_output_dir():
    """Create output directory if it doesn't exist"""
    os.makedirs(OUTPUT_DIR, exist_ok=True)

def add_noise(image, intensity=15):
    """Add random noise to an image for texture"""
    np_image = np.array(image)
    noise = np.random.randint(-intensity, intensity + 1, np_image.shape)
    np_image = np.clip(np_image + noise, 0, 255)
    return Image.fromarray(np_image.astype('uint8'))

def create_seamless_texture(base_color, variation_colors, dot_density=0.3, dot_size_range=(2, 8)):
    """Create a seamless textured background"""
    image = Image.new('RGB', BACKGROUND_SIZE, base_color)
    draw = ImageDraw.Draw(image)
    
    # Add texture dots/particles
    num_dots = int(BACKGROUND_SIZE[0] * BACKGROUND_SIZE[1] * dot_density / 10000)
    for _ in range(num_dots):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(*dot_size_range)
        color = random.choice(variation_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    return image

def generate_forest_background():
    """Generate a top-down forest map with grass, trees, and paths"""
    print("Generating forest background...")
    
    # Base grass color
    image = Image.new('RGB', BACKGROUND_SIZE, (34, 139, 34))  # Forest green
    draw = ImageDraw.Draw(image)
    
    # Add grass texture
    grass_colors = [(46, 125, 50), (56, 142, 60), (27, 94, 32), (40, 105, 40)]
    for _ in range(50000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(3, 12)
        color = random.choice(grass_colors)
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add dirt paths
    for _ in range(20):
        start_x = random.randint(0, BACKGROUND_SIZE[0])
        start_y = random.randint(0, BACKGROUND_SIZE[1])
        path_width = random.randint(40, 80)
        
        # Create winding path
        current_x, current_y = start_x, start_y
        for _ in range(100):
            # Move in a somewhat random direction
            current_x += random.randint(-50, 50)
            current_y += random.randint(-50, 50)
            
            # Keep within bounds
            current_x = max(0, min(current_x, BACKGROUND_SIZE[0]))
            current_y = max(0, min(current_y, BACKGROUND_SIZE[1]))
            
            # Draw path segment
            draw.ellipse([current_x - path_width//2, current_y - path_width//2, 
                         current_x + path_width//2, current_y + path_width//2], 
                        fill=(101, 67, 33))  # Brown dirt
    
    # Add tree clusters (circular dark spots representing tree canopies from above)
    for _ in range(800):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        tree_size = random.randint(20, 60)
        
        # Dark green tree canopy
        tree_colors = [(0, 100, 0), (0, 80, 0), (25, 75, 25)]
        color = random.choice(tree_colors)
        
        draw.ellipse([x - tree_size, y - tree_size, x + tree_size, y + tree_size], fill=color)
    
    # Add small bushes and undergrowth
    for _ in range(2000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        bush_size = random.randint(5, 20)
        
        bush_colors = [(85, 107, 47), (107, 142, 35), (124, 252, 0)]
        color = random.choice(bush_colors)
        
        draw.ellipse([x - bush_size, y - bush_size, x + bush_size, y + bush_size], fill=color)
    
    # Add subtle noise for organic feel
    image = add_noise(image, 10)
    
    return image

def generate_desert_background():
    """Generate a top-down desert map with sand, rocks, and dunes"""
    print("Generating desert background...")
    
    # Base sand color
    image = Image.new('RGB', BACKGROUND_SIZE, (238, 203, 173))  # Sandy beige
    draw = ImageDraw.Draw(image)
    
    # Add sand texture
    sand_colors = [(194, 178, 128), (218, 165, 32), (245, 222, 179), (210, 180, 140)]
    for _ in range(30000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(2, 8)
        color = random.choice(sand_colors)
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add sand dune patterns (curved lines)
    for _ in range(200):
        start_x = random.randint(0, BACKGROUND_SIZE[0])
        start_y = random.randint(0, BACKGROUND_SIZE[1])
        
        # Create curved dune lines
        points = []
        for i in range(20):
            offset_x = i * 50 + random.randint(-20, 20)
            offset_y = 30 * math.sin(i * 0.3) + random.randint(-15, 15)
            points.append((start_x + offset_x, start_y + offset_y))
        
        # Draw dune ridge
        for i in range(len(points) - 1):
            draw.line([points[i], points[i + 1]], fill=(180, 150, 100), width=8)
    
    # Add rocky outcrops
    for _ in range(150):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        rock_size = random.randint(15, 40)
        
        rock_colors = [(139, 137, 137), (105, 105, 105), (128, 128, 128)]
        color = random.choice(rock_colors)
        
        # Irregular rock shape
        points = []
        for angle in range(0, 360, 30):
            rad = math.radians(angle)
            radius = rock_size * random.uniform(0.6, 1.0)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            points.append((px, py))
        
        draw.polygon(points, fill=color)
    
    # Add cacti (small green dots)
    for _ in range(100):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        cactus_size = random.randint(8, 20)
        
        draw.ellipse([x - cactus_size, y - cactus_size, x + cactus_size, y + cactus_size], 
                    fill=(0, 128, 0))
    
    # Add noise for sandy texture
    image = add_noise(image, 12)
    
    return image

def generate_ocean_background():
    """Generate a top-down ocean map with water, waves, and islands"""
    print("Generating ocean background...")
    
    # Base ocean color
    image = Image.new('RGB', BACKGROUND_SIZE, (0, 105, 148))  # Deep ocean blue
    draw = ImageDraw.Draw(image)
    
    # Add water texture with ripples
    water_colors = [(0, 119, 190), (30, 144, 255), (65, 105, 225), (0, 90, 120)]
    for _ in range(40000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(2, 6)
        color = random.choice(water_colors)
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add wave patterns (parallel lines)
    for wave_set in range(50):
        start_x = random.randint(-500, BACKGROUND_SIZE[0] + 500)
        start_y = random.randint(0, BACKGROUND_SIZE[1])
        wave_angle = random.uniform(0, math.pi)
        
        for i in range(20):
            offset = i * 100
            x1 = start_x + offset * math.cos(wave_angle)
            y1 = start_y + offset * math.sin(wave_angle)
            x2 = x1 + 80 * math.cos(wave_angle + math.pi/2)
            y2 = y1 + 80 * math.sin(wave_angle + math.pi/2)
            
            draw.line([(x1, y1), (x2, y2)], fill=(100, 149, 237), width=3)
    
    # Add islands
    for _ in range(30):
        x = random.randint(100, BACKGROUND_SIZE[0] - 100)
        y = random.randint(100, BACKGROUND_SIZE[1] - 100)
        island_size = random.randint(50, 200)
        
        # Island shape (irregular)
        points = []
        for angle in range(0, 360, 20):
            rad = math.radians(angle)
            radius = island_size * random.uniform(0.7, 1.0)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            points.append((px, py))
        
        # Green island
        draw.polygon(points, fill=(34, 139, 34))
        
        # Sandy beach around island
        beach_points = []
        for angle in range(0, 360, 20):
            rad = math.radians(angle)
            radius = island_size * random.uniform(1.1, 1.2)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            beach_points.append((px, py))
        
        draw.polygon(beach_points, fill=(238, 203, 173))
    
    # Add white foam spots
    for _ in range(1000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        foam_size = random.randint(3, 12)
        
        draw.ellipse([x - foam_size, y - foam_size, x + foam_size, y + foam_size], 
                    fill=(255, 255, 255))
    
    return image

def generate_mountains_background():
    """Generate a top-down mountain map with rocky terrain and paths"""
    print("Generating mountains background...")
    
    # Base rocky color
    image = Image.new('RGB', BACKGROUND_SIZE, (139, 137, 137))  # Gray stone
    draw = ImageDraw.Draw(image)
    
    # Add stone texture
    stone_colors = [(105, 105, 105), (119, 136, 153), (128, 128, 128), (160, 160, 160)]
    for _ in range(35000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(3, 10)
        color = random.choice(stone_colors)
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add mountain ridges (darker areas representing higher elevation)
    for _ in range(100):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        ridge_size = random.randint(100, 300)
        
        # Dark mountain ridge
        draw.ellipse([x - ridge_size, y - ridge_size, x + ridge_size, y + ridge_size], 
                    fill=(80, 80, 80))
    
    # Add snow patches on higher areas
    for _ in range(300):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        snow_size = random.randint(20, 80)
        
        # White snow
        draw.ellipse([x - snow_size, y - snow_size, x + snow_size, y + snow_size], 
                    fill=(255, 250, 250))
    
    # Add rocky paths between peaks
    for _ in range(15):
        start_x = random.randint(0, BACKGROUND_SIZE[0])
        start_y = random.randint(0, BACKGROUND_SIZE[1])
        path_width = random.randint(30, 60)
        
        # Create mountain path
        current_x, current_y = start_x, start_y
        for _ in range(80):
            current_x += random.randint(-40, 40)
            current_y += random.randint(-40, 40)
            
            current_x = max(0, min(current_x, BACKGROUND_SIZE[0]))
            current_y = max(0, min(current_y, BACKGROUND_SIZE[1]))
            
            # Lighter stone path
            draw.ellipse([current_x - path_width//2, current_y - path_width//2, 
                         current_x + path_width//2, current_y + path_width//2], 
                        fill=(160, 160, 160))
    
    # Add scattered boulders
    for _ in range(500):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        boulder_size = random.randint(8, 25)
        
        boulder_colors = [(90, 90, 90), (70, 70, 70), (110, 110, 110)]
        color = random.choice(boulder_colors)
        
        draw.ellipse([x - boulder_size, y - boulder_size, x + boulder_size, y + boulder_size], 
                    fill=color)
    
    # Add sparse vegetation
    for _ in range(200):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        plant_size = random.randint(5, 15)
        
        draw.ellipse([x - plant_size, y - plant_size, x + plant_size, y + plant_size], 
                    fill=(85, 107, 47))  # Olive green
    
    # Add noise for rocky texture
    image = add_noise(image, 15)
    
    return image

def generate_city_background():
    """Generate a top-down city map with roads, buildings, and urban areas"""
    print("Generating city background...")
    
    # Base asphalt/concrete color
    image = Image.new('RGB', BACKGROUND_SIZE, (64, 64, 64))  # Dark gray
    draw = ImageDraw.Draw(image)
    
    # Create city grid with roads
    road_width = 80
    block_size = 400
    
    # Draw main roads (grid pattern)
    for x in range(0, BACKGROUND_SIZE[0], block_size):
        draw.rectangle([x, 0, x + road_width, BACKGROUND_SIZE[1]], fill=(48, 48, 48))  # Darker roads
    
    for y in range(0, BACKGROUND_SIZE[1], block_size):
        draw.rectangle([0, y, BACKGROUND_SIZE[0], y + road_width], fill=(48, 48, 48))  # Darker roads
    
    # Add road markings
    for x in range(0, BACKGROUND_SIZE[0], block_size):
        for y in range(0, BACKGROUND_SIZE[1], 200):
            # Yellow lines
            draw.rectangle([x + road_width//2 - 5, y, x + road_width//2 + 5, y + 100], 
                          fill=(255, 255, 0))
    
    # Add buildings in blocks
    for block_x in range(road_width, BACKGROUND_SIZE[0], block_size):
        for block_y in range(road_width, BACKGROUND_SIZE[1], block_size):
            # Fill block with buildings
            for _ in range(15):
                building_x = block_x + random.randint(10, block_size - road_width - 60)
                building_y = block_y + random.randint(10, block_size - road_width - 60)
                building_width = random.randint(40, 80)
                building_height = random.randint(40, 80)
                
                building_colors = [(128, 128, 128), (105, 105, 105), (169, 169, 169), (96, 96, 96)]
                color = random.choice(building_colors)
                
                draw.rectangle([building_x, building_y, 
                               building_x + building_width, building_y + building_height], 
                              fill=color)
                
                # Add building details (windows/roof features)
                detail_color = tuple(max(0, c - 30) for c in color)  # Darker details
                draw.rectangle([building_x + 5, building_y + 5, 
                               building_x + building_width - 5, building_y + building_height - 5], 
                              outline=detail_color, width=2)
    
    # Add parks (green spaces)
    for _ in range(20):
        park_x = random.randint(road_width, BACKGROUND_SIZE[0] - 200)
        park_y = random.randint(road_width, BACKGROUND_SIZE[1] - 200)
        park_size = random.randint(100, 200)
        
        draw.ellipse([park_x, park_y, park_x + park_size, park_y + park_size], 
                    fill=(34, 139, 34))  # Green
        
        # Add trees in park
        for _ in range(10):
            tree_x = park_x + random.randint(10, park_size - 10)
            tree_y = park_y + random.randint(10, park_size - 10)
            tree_size = random.randint(8, 20)
            
            draw.ellipse([tree_x - tree_size, tree_y - tree_size, 
                         tree_x + tree_size, tree_y + tree_size], 
                        fill=(0, 100, 0))
    
    # Add urban texture
    for _ in range(10000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(1, 4)
        
        texture_colors = [(80, 80, 80), (90, 90, 90), (70, 70, 70)]
        color = random.choice(texture_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    return image

def generate_space_background():
    """Generate a top-down space map with asteroids, stations, and cosmic debris"""
    print("Generating space background...")
    
    # Base space color (very dark)
    image = Image.new('RGB', BACKGROUND_SIZE, (5, 5, 15))  # Very dark space
    draw = ImageDraw.Draw(image)
    
    # Add star field
    for _ in range(8000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        star_size = random.randint(1, 3)
        
        star_colors = [(255, 255, 255), (255, 255, 200), (200, 200, 255), (255, 200, 200)]
        color = random.choice(star_colors)
        
        draw.ellipse([x - star_size, y - star_size, x + star_size, y + star_size], fill=color)
    
    # Add asteroid fields
    for _ in range(200):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        asteroid_size = random.randint(15, 60)
        
        # Irregular asteroid shape
        points = []
        for angle in range(0, 360, 30):
            rad = math.radians(angle)
            radius = asteroid_size * random.uniform(0.5, 1.0)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            points.append((px, py))
        
        asteroid_colors = [(139, 137, 137), (105, 105, 105), (85, 85, 85)]
        color = random.choice(asteroid_colors)
        
        draw.polygon(points, fill=color)
    
    # Add space stations (geometric shapes)
    for _ in range(20):
        x = random.randint(100, BACKGROUND_SIZE[0] - 100)
        y = random.randint(100, BACKGROUND_SIZE[1] - 100)
        station_size = random.randint(40, 100)
        
        # Hexagonal space station
        points = []
        for angle in range(0, 360, 60):
            rad = math.radians(angle)
            px = x + station_size * math.cos(rad)
            py = y + station_size * math.sin(rad)
            points.append((px, py))
        
        draw.polygon(points, fill=(128, 128, 128))  # Gray station
        
        # Add station details
        draw.polygon(points, outline=(200, 200, 200), width=3)
        draw.ellipse([x - 15, y - 15, x + 15, y + 15], fill=(0, 255, 0))  # Green center
    
    # Add nebula clouds (colored gas clouds)
    for _ in range(30):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        cloud_size = random.randint(200, 500)
        
        nebula_colors = [(128, 0, 128, 30), (75, 0, 130, 30), (255, 20, 147, 30), (0, 191, 255, 30)]
        color = random.choice(nebula_colors)[:3]  # Remove alpha
        
        # Create cloud effect with multiple overlapping circles
        for _ in range(20):
            cloud_x = x + random.randint(-cloud_size//2, cloud_size//2)
            cloud_y = y + random.randint(-cloud_size//2, cloud_size//2)
            cloud_radius = random.randint(50, cloud_size//3)
            
            draw.ellipse([cloud_x - cloud_radius, cloud_y - cloud_radius, 
                         cloud_x + cloud_radius, cloud_y + cloud_radius], 
                        fill=color)
    
    # Add space debris
    for _ in range(1000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        debris_size = random.randint(3, 12)
        
        debris_colors = [(169, 169, 169), (139, 69, 19), (255, 140, 0)]
        color = random.choice(debris_colors)
        
        draw.ellipse([x - debris_size, y - debris_size, x + debris_size, y + debris_size], 
                    fill=color)
    
    return image

def main():
    """Generate all background images"""
    ensure_output_dir()
    
    backgrounds = {
        'forest.png': generate_forest_background,
        'desert.png': generate_desert_background,
        'ocean.png': generate_ocean_background,
        'mountains.png': generate_mountains_background,
        'city.png': generate_city_background,
        'space.png': generate_space_background,
    }
    
    print(f"Generating {len(backgrounds)} top-down game map backgrounds...")
    print(f"Output directory: {OUTPUT_DIR}")
    print(f"Image size: {BACKGROUND_SIZE[0]}x{BACKGROUND_SIZE[1]}")
    print(f"Optimized for 800x600 window with top-down camera")
    print("-" * 50)
    
    for filename, generator_func in backgrounds.items():
        try:
            image = generator_func()
            output_path = os.path.join(OUTPUT_DIR, filename)
            image.save(output_path, 'PNG')
            print(f"✓ Saved {filename}")
        except Exception as e:
            print(f"✗ Failed to generate {filename}: {e}")
    
    print("-" * 50)
    print("Background generation complete!")
    print(f"Images saved in: {OUTPUT_DIR}")
    print("\nTo use these backgrounds:")
    print("1. Run ./build.sh to compile the game")
    print("2. Run ./build/estate to start the game")
    print("3. Select your background from the menu")
    print("\nThese backgrounds are designed for top-down 2D gameplay!")

if __name__ == "__main__":
    main() 
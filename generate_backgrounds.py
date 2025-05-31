#!/usr/bin/env python3
"""
Background Generator for Estate Game
Generates 5000x5000 procedural backgrounds for all themes
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

def create_gradient(size, color1, color2, direction='vertical'):
    """Create a gradient background"""
    width, height = size
    image = Image.new('RGB', size)
    draw = ImageDraw.Draw(image)
    
    if direction == 'vertical':
        for y in range(height):
            ratio = y / height
            r = int(color1[0] * (1 - ratio) + color2[0] * ratio)
            g = int(color1[1] * (1 - ratio) + color2[1] * ratio)
            b = int(color1[2] * (1 - ratio) + color2[2] * ratio)
            draw.line([(0, y), (width, y)], fill=(r, g, b))
    else:  # horizontal
        for x in range(width):
            ratio = x / width
            r = int(color1[0] * (1 - ratio) + color2[0] * ratio)
            g = int(color1[1] * (1 - ratio) + color2[1] * ratio)
            b = int(color1[2] * (1 - ratio) + color2[2] * ratio)
            draw.line([(x, 0), (x, height)], fill=(r, g, b))
    
    return image

def add_noise(image, intensity=20):
    """Add random noise to an image"""
    np_image = np.array(image)
    noise = np.random.randint(-intensity, intensity + 1, np_image.shape)
    np_image = np.clip(np_image + noise, 0, 255)
    return Image.fromarray(np_image.astype('uint8'))

def generate_forest_background():
    """Generate a forest background with trees and foliage"""
    print("Generating forest background...")
    
    # Create base gradient (sky to ground)
    image = create_gradient(BACKGROUND_SIZE, (135, 206, 235), (34, 139, 34))  # Sky blue to forest green
    draw = ImageDraw.Draw(image)
    
    # Add tree trunks
    for _ in range(800):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] // 2, BACKGROUND_SIZE[1])
        trunk_width = random.randint(30, 80)
        trunk_height = random.randint(200, 600)
        
        # Brown trunk
        draw.rectangle([x - trunk_width//2, y - trunk_height, x + trunk_width//2, y], 
                      fill=(101, 67, 33))
    
    # Add tree canopies
    for _ in range(1200):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] // 3, BACKGROUND_SIZE[1] * 2 // 3)
        radius = random.randint(100, 300)
        
        # Various shades of green
        green_colors = [(34, 139, 34), (0, 128, 0), (0, 100, 0), (46, 125, 50)]
        color = random.choice(green_colors)
        
        draw.ellipse([x - radius, y - radius, x + radius, y + radius], fill=color)
    
    # Add some undergrowth
    for _ in range(2000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] * 3 // 4, BACKGROUND_SIZE[1])
        size = random.randint(10, 50)
        
        grass_colors = [(85, 107, 47), (107, 142, 35), (124, 252, 0)]
        color = random.choice(grass_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add some subtle noise
    image = add_noise(image, 15)
    
    return image

def generate_desert_background():
    """Generate a desert background with dunes and sand"""
    print("Generating desert background...")
    
    # Create base gradient (sky to sand)
    image = create_gradient(BACKGROUND_SIZE, (255, 215, 0), (238, 203, 173))  # Golden sky to sandy beige
    draw = ImageDraw.Draw(image)
    
    # Add sand dunes
    for _ in range(200):
        # Create dune shapes
        points = []
        center_x = random.randint(0, BACKGROUND_SIZE[0])
        center_y = random.randint(BACKGROUND_SIZE[1] // 2, BACKGROUND_SIZE[1])
        width = random.randint(300, 800)
        height = random.randint(100, 300)
        
        # Create smooth dune curves
        for angle in range(0, 360, 10):
            rad = math.radians(angle)
            offset_x = width * math.cos(rad) * random.uniform(0.5, 1.0)
            offset_y = height * math.sin(rad) * random.uniform(0.3, 0.7)
            points.append((center_x + offset_x, center_y + offset_y))
        
        sand_colors = [(194, 178, 128), (218, 165, 32), (238, 203, 173)]
        color = random.choice(sand_colors)
        
        draw.polygon(points, fill=color)
    
    # Add sand particles/texture
    for _ in range(5000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(2, 8)
        
        particle_colors = [(210, 180, 140), (222, 184, 135), (245, 222, 179)]
        color = random.choice(particle_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Add some cacti
    for _ in range(50):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] * 2 // 3, BACKGROUND_SIZE[1])
        width = random.randint(20, 40)
        height = random.randint(100, 300)
        
        # Green cactus
        draw.rectangle([x - width//2, y - height, x + width//2, y], fill=(0, 128, 0))
        
        # Add some arms
        if random.random() > 0.5:
            arm_y = y - height // 2
            arm_length = random.randint(50, 150)
            draw.rectangle([x + width//2, arm_y - 15, x + width//2 + arm_length, arm_y + 15], fill=(0, 128, 0))
    
    # Add noise
    image = add_noise(image, 10)
    
    return image

def generate_ocean_background():
    """Generate an ocean background with waves and islands"""
    print("Generating ocean background...")
    
    # Create base gradient (sky to deep ocean)
    image = create_gradient(BACKGROUND_SIZE, (135, 206, 235), (0, 105, 148))  # Sky blue to deep ocean blue
    draw = ImageDraw.Draw(image)
    
    # Add wave patterns
    for wave_layer in range(20):
        y_base = BACKGROUND_SIZE[1] // 4 + wave_layer * 200
        amplitude = 50 + wave_layer * 10
        frequency = 0.002 + wave_layer * 0.0005
        
        wave_points = []
        for x in range(0, BACKGROUND_SIZE[0], 10):
            y = y_base + amplitude * math.sin(x * frequency + wave_layer)
            wave_points.append((x, y))
        
        # Add bottom points to close the polygon
        wave_points.append((BACKGROUND_SIZE[0], BACKGROUND_SIZE[1]))
        wave_points.append((0, BACKGROUND_SIZE[1]))
        
        # Varying shades of blue for depth
        blue_shades = [(0, 105, 148), (0, 119, 190), (30, 144, 255), (65, 105, 225)]
        color = blue_shades[min(wave_layer // 5, len(blue_shades) - 1)]
        
        draw.polygon(wave_points, fill=color)
    
    # Add some islands
    for _ in range(15):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] // 3, BACKGROUND_SIZE[1] * 2 // 3)
        size = random.randint(100, 400)
        
        # Island shape (irregular circle)
        points = []
        for angle in range(0, 360, 20):
            rad = math.radians(angle)
            radius = size * random.uniform(0.6, 1.0)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            points.append((px, py))
        
        # Green/brown island
        island_colors = [(34, 139, 34), (107, 142, 35), (160, 82, 45)]
        color = random.choice(island_colors)
        
        draw.polygon(points, fill=color)
        
        # Add beach around island
        beach_points = []
        for angle in range(0, 360, 20):
            rad = math.radians(angle)
            radius = size * random.uniform(1.1, 1.3)
            px = x + radius * math.cos(rad)
            py = y + radius * math.sin(rad)
            beach_points.append((px, py))
        
        draw.polygon(beach_points, fill=(238, 203, 173))  # Sandy beach
    
    # Add foam and wave crests
    for _ in range(1000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] // 2, BACKGROUND_SIZE[1])
        size = random.randint(5, 20)
        
        # White foam
        draw.ellipse([x - size, y - size, x + size, y + size], fill=(255, 255, 255))
    
    return image

def generate_mountains_background():
    """Generate a mountainous background with peaks and snow"""
    print("Generating mountains background...")
    
    # Create base gradient (sky to ground)
    image = create_gradient(BACKGROUND_SIZE, (135, 206, 235), (139, 137, 137))  # Sky blue to gray
    draw = ImageDraw.Draw(image)
    
    # Add mountain layers (background to foreground)
    mountain_layers = [
        {'y_base': BACKGROUND_SIZE[1] * 3 // 4, 'color': (169, 169, 169), 'peaks': 30},  # Background mountains
        {'y_base': BACKGROUND_SIZE[1] * 5 // 6, 'color': (128, 128, 128), 'peaks': 20},  # Mid mountains
        {'y_base': BACKGROUND_SIZE[1] * 7 // 8, 'color': (105, 105, 105), 'peaks': 15},  # Foreground mountains
    ]
    
    for layer in mountain_layers:
        y_base = layer['y_base']
        color = layer['color']
        num_peaks = layer['peaks']
        
        # Generate mountain silhouette
        mountain_points = [(0, BACKGROUND_SIZE[1])]  # Start from bottom left
        
        for i in range(num_peaks):
            x = (i / num_peaks) * BACKGROUND_SIZE[0]
            peak_height = random.randint(200, 600)
            y = y_base - peak_height
            
            # Add some variation to the x position
            x += random.randint(-100, 100)
            x = max(0, min(x, BACKGROUND_SIZE[0]))
            
            mountain_points.append((x, y))
        
        mountain_points.append((BACKGROUND_SIZE[0], BACKGROUND_SIZE[1]))  # End at bottom right
        
        draw.polygon(mountain_points, fill=color)
        
        # Add snow caps to highest peaks
        for i in range(1, len(mountain_points) - 1):
            x, y = mountain_points[i]
            if y < y_base - 300:  # Only add snow to high peaks
                snow_size = random.randint(50, 150)
                snow_points = [
                    (x, y),
                    (x - snow_size, y + snow_size),
                    (x + snow_size, y + snow_size)
                ]
                draw.polygon(snow_points, fill=(255, 250, 250))  # Snow white
    
    # Add some trees on lower slopes
    for _ in range(300):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] * 3 // 4, BACKGROUND_SIZE[1])
        size = random.randint(20, 60)
        
        # Pine tree shape
        tree_points = [
            (x, y - size),  # Top
            (x - size//3, y),  # Bottom left
            (x + size//3, y)   # Bottom right
        ]
        
        draw.polygon(tree_points, fill=(0, 100, 0))  # Dark green
    
    # Add rocks and boulders
    for _ in range(200):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(BACKGROUND_SIZE[1] * 2 // 3, BACKGROUND_SIZE[1])
        size = random.randint(30, 100)
        
        rock_colors = [(105, 105, 105), (119, 136, 153), (128, 128, 128)]
        color = random.choice(rock_colors)
        
        draw.ellipse([x - size, y - size//2, x + size, y + size//2], fill=color)
    
    return image

def generate_city_background():
    """Generate a city background with buildings and urban elements"""
    print("Generating city background...")
    
    # Create base gradient (sky to urban ground)
    image = create_gradient(BACKGROUND_SIZE, (135, 206, 235), (64, 64, 64))  # Sky blue to dark gray
    draw = ImageDraw.Draw(image)
    
    # Add buildings
    building_colors = [(128, 128, 128), (105, 105, 105), (169, 169, 169), (47, 79, 79)]
    
    for _ in range(300):
        x = random.randint(0, BACKGROUND_SIZE[0] - 200)
        width = random.randint(100, 400)
        height = random.randint(300, 1500)
        y = BACKGROUND_SIZE[1] - height
        
        # Building body
        color = random.choice(building_colors)
        draw.rectangle([x, y, x + width, BACKGROUND_SIZE[1]], fill=color)
        
        # Add windows
        window_size = 20
        window_spacing = 40
        for window_y in range(y + 50, BACKGROUND_SIZE[1] - 50, window_spacing):
            for window_x in range(x + 20, x + width - 20, window_spacing):
                if random.random() > 0.3:  # Some windows are lit
                    window_color = (255, 255, 0) if random.random() > 0.7 else (200, 200, 255)
                    draw.rectangle([window_x, window_y, window_x + window_size, window_y + window_size], 
                                 fill=window_color)
    
    # Add some skyscrapers
    for _ in range(50):
        x = random.randint(0, BACKGROUND_SIZE[0] - 300)
        width = random.randint(150, 300)
        height = random.randint(1000, 2500)
        y = BACKGROUND_SIZE[1] - height
        
        # Skyscraper body
        color = random.choice(building_colors)
        draw.rectangle([x, y, x + width, BACKGROUND_SIZE[1]], fill=color)
        
        # Add antenna/spire
        if random.random() > 0.5:
            antenna_height = random.randint(100, 300)
            draw.line([(x + width//2, y), (x + width//2, y - antenna_height)], 
                     fill=(255, 0, 0), width=10)  # Red antenna light
    
    # Add roads
    for _ in range(20):
        road_y = random.randint(BACKGROUND_SIZE[1] * 3 // 4, BACKGROUND_SIZE[1])
        draw.rectangle([0, road_y, BACKGROUND_SIZE[0], road_y + 50], fill=(64, 64, 64))
        
        # Add road markings
        for x in range(0, BACKGROUND_SIZE[0], 200):
            draw.rectangle([x, road_y + 22, x + 100, road_y + 28], fill=(255, 255, 255))
    
    # Add some smog/atmosphere
    for _ in range(500):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1] // 2)
        size = random.randint(50, 200)
        
        # Semi-transparent gray smog
        overlay = Image.new('RGBA', (size*2, size*2), (128, 128, 128, 30))
        image.paste(overlay, (x-size, y-size), overlay)
    
    return image

def generate_space_background():
    """Generate a space background with stars, nebulae, and cosmic elements"""
    print("Generating space background...")
    
    # Create base dark space
    image = Image.new('RGB', BACKGROUND_SIZE, (0, 0, 20))  # Very dark blue-black
    draw = ImageDraw.Draw(image)
    
    # Add nebulae
    nebula_colors = [
        (128, 0, 128),  # Purple
        (75, 0, 130),   # Indigo
        (255, 20, 147), # Deep pink
        (0, 191, 255),  # Deep sky blue
        (255, 69, 0),   # Red orange
    ]
    
    for _ in range(20):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(500, 1500)
        color = random.choice(nebula_colors)
        
        # Create nebula with gradient effect
        for radius in range(size, 0, -50):
            alpha = int(30 * (radius / size))
            nebula_color = (*color, alpha)
            
            # Create temporary image for blending
            temp_img = Image.new('RGBA', BACKGROUND_SIZE, (0, 0, 0, 0))
            temp_draw = ImageDraw.Draw(temp_img)
            temp_draw.ellipse([x - radius, y - radius, x + radius, y + radius], 
                            fill=nebula_color)
            
            # Apply blur
            temp_img = temp_img.filter(ImageFilter.GaussianBlur(radius=20))
            
            # Paste onto main image
            image.paste(Image.alpha_composite(image.convert('RGBA'), temp_img).convert('RGB'))
    
    # Add stars
    star_colors = [(255, 255, 255), (255, 255, 200), (200, 200, 255), (255, 200, 200)]
    
    # Small stars
    for _ in range(8000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(1, 3)
        color = random.choice(star_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
    
    # Medium stars
    for _ in range(1000):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(3, 8)
        color = random.choice(star_colors)
        
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
        
        # Add star glow
        if random.random() > 0.7:
            glow_size = size * 3
            glow_color = (*color, 50)
            temp_img = Image.new('RGBA', BACKGROUND_SIZE, (0, 0, 0, 0))
            temp_draw = ImageDraw.Draw(temp_img)
            temp_draw.ellipse([x - glow_size, y - glow_size, x + glow_size, y + glow_size], 
                            fill=glow_color)
            temp_img = temp_img.filter(ImageFilter.GaussianBlur(radius=10))
            image.paste(Image.alpha_composite(image.convert('RGBA'), temp_img).convert('RGB'))
    
    # Add large bright stars
    for _ in range(100):
        x = random.randint(0, BACKGROUND_SIZE[0])
        y = random.randint(0, BACKGROUND_SIZE[1])
        size = random.randint(8, 15)
        color = random.choice(star_colors)
        
        # Main star
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
        
        # Star spikes
        spike_length = size * 4
        draw.line([(x - spike_length, y), (x + spike_length, y)], fill=color, width=3)
        draw.line([(x, y - spike_length), (x, y + spike_length)], fill=color, width=3)
    
    # Add some planets/moons
    for _ in range(5):
        x = random.randint(200, BACKGROUND_SIZE[0] - 200)
        y = random.randint(200, BACKGROUND_SIZE[1] - 200)
        size = random.randint(100, 400)
        
        planet_colors = [(169, 169, 169), (139, 69, 19), (255, 140, 0), (100, 149, 237)]
        color = random.choice(planet_colors)
        
        # Planet body
        draw.ellipse([x - size, y - size, x + size, y + size], fill=color)
        
        # Add some surface features
        for _ in range(10):
            crater_size = random.randint(10, size // 3)
            crater_x = x + random.randint(-size//2, size//2)
            crater_y = y + random.randint(-size//2, size//2)
            
            darker_color = tuple(max(0, c - 30) for c in color)
            draw.ellipse([crater_x - crater_size, crater_y - crater_size, 
                         crater_x + crater_size, crater_y + crater_size], fill=darker_color)
    
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
    
    print(f"Generating {len(backgrounds)} background images...")
    print(f"Output directory: {OUTPUT_DIR}")
    print(f"Image size: {BACKGROUND_SIZE[0]}x{BACKGROUND_SIZE[1]}")
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

if __name__ == "__main__":
    main() 
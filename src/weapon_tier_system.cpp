#include "weapon_tier_system.h"
#include "weapons/melee_weapons.h"
#include "weapons/ranged_weapons.h"
#include <stdexcept>

WeaponTierSystem& WeaponTierSystem::getInstance() {
    static WeaponTierSystem instance;
    return instance;
}

WeaponTierSystem::WeaponTierSystem() {
    initializeWeaponStats();
    initializeUpgrades();
}

void WeaponTierSystem::initializeWeaponStats() {
    // Initialize weapon stats for each category and tier
    // Format: [category][tier] = WeaponStats(damage, cooldown, range, cost, name, description)
    
    weaponStats_.resize(50); // 50 weapon categories
    
    // === CLASSIC WEAPONS ===
    
    // SWORD
    weaponStats_[static_cast<int>(WeaponCategory::SWORD)] = {
        WeaponStats(25, 1.0f, 60.0f, 0, "Rusty Sword", "A worn blade from humble beginnings"),
        WeaponStats(45, 0.8f, 70.0f, 150, "Steel Sword", "A well-crafted blade of quality steel"),
        WeaponStats(80, 0.6f, 80.0f, 500, "Legendary Blade", "A masterwork of steel and magic")
    };
    
    // DAGGER
    weaponStats_[static_cast<int>(WeaponCategory::DAGGER)] = {
        WeaponStats(15, 0.6f, 40.0f, 0, "Simple Dagger", "Quick strikes with a basic blade"),
        WeaponStats(28, 0.4f, 45.0f, 120, "Assassin's Blade", "Swift and deadly precision weapon"),
        WeaponStats(50, 0.3f, 50.0f, 400, "Shadowfang", "A dagger forged in darkness itself")
    };
    
    // BOW
    weaponStats_[static_cast<int>(WeaponCategory::BOW)] = {
        WeaponStats(20, 1.2f, 200.0f, 0, "Wooden Bow", "A simple bow for hunting"),
        WeaponStats(35, 1.0f, 250.0f, 180, "Composite Bow", "Enhanced range and power"),
        WeaponStats(65, 0.8f, 300.0f, 600, "Elven Longbow", "Crafted by master elven artisans")
    };
    
    // CROSSBOW
    weaponStats_[static_cast<int>(WeaponCategory::CROSSBOW)] = {
        WeaponStats(30, 1.8f, 180.0f, 0, "Light Crossbow", "Mechanical precision in a compact form"),
        WeaponStats(55, 1.5f, 220.0f, 200, "Heavy Crossbow", "Devastating power with slower reload"),
        WeaponStats(95, 1.2f, 280.0f, 700, "Siege Breaker", "A crossbow that can pierce any armor")
    };
    
    // AXE
    weaponStats_[static_cast<int>(WeaponCategory::AXE)] = {
        WeaponStats(35, 1.5f, 50.0f, 80, "Woodcutter's Axe", "Heavy but effective melee weapon"),
        WeaponStats(60, 1.2f, 55.0f, 250, "War Axe", "Designed for battle, not trees"),
        WeaponStats(110, 1.0f, 60.0f, 800, "Berserker's Fury", "An axe that thirsts for blood")
    };
    
    // STAFF
    weaponStats_[static_cast<int>(WeaponCategory::STAFF)] = {
        WeaponStats(18, 1.0f, 150.0f, 100, "Apprentice Staff", "Channel basic magical energies"),
        WeaponStats(32, 0.8f, 180.0f, 300, "Wizard's Staff", "Enhanced magical focus and power"),
        WeaponStats(60, 0.6f, 220.0f, 900, "Archmage's Rod", "Ultimate magical weapon of destruction")
    };
    
    // HAMMER
    weaponStats_[static_cast<int>(WeaponCategory::HAMMER)] = {
        WeaponStats(40, 2.0f, 45.0f, 120, "Smith's Hammer", "Heavy impact, slow swing"),
        WeaponStats(70, 1.6f, 50.0f, 350, "War Hammer", "Crushes armor and bone alike"),
        WeaponStats(120, 1.3f, 55.0f, 1000, "Thunder Maul", "Strikes with the force of lightning")
    };
    
    // SPEAR
    weaponStats_[static_cast<int>(WeaponCategory::SPEAR)] = {
        WeaponStats(22, 1.1f, 80.0f, 60, "Pike", "Extended reach for keeping enemies at bay"),
        WeaponStats(38, 0.9f, 90.0f, 220, "Halberd", "Versatile polearm for experienced warriors"),
        WeaponStats(70, 0.7f, 100.0f, 750, "Gungnir", "The spear that never misses its target")
    };
    
    // WAND
    weaponStats_[static_cast<int>(WeaponCategory::WAND)] = {
        WeaponStats(12, 0.5f, 120.0f, 80, "Magic Wand", "Quick magical projectiles"),
        WeaponStats(22, 0.4f, 140.0f, 280, "Enchanted Wand", "Rapid-fire magical attacks"),
        WeaponStats(40, 0.3f, 160.0f, 850, "Wand of Wonder", "Incredible speed and magical power")
    };
    
    // CANNON
    weaponStats_[static_cast<int>(WeaponCategory::CANNON)] = {
        WeaponStats(60, 3.0f, 250.0f, 400, "Hand Cannon", "Explosive projectiles with area damage"),
        WeaponStats(100, 2.5f, 300.0f, 800, "Dragon Cannon", "Devastating explosive weapon"),
        WeaponStats(180, 2.0f, 350.0f, 1500, "Annihilator", "Ultimate destruction in portable form")
    };
    
    // === BADASS WEAPONS ===
    
    // CHAINSAW
    weaponStats_[static_cast<int>(WeaponCategory::CHAINSAW)] = {
        WeaponStats(50, 0.8f, 35.0f, 300, "Rusty Chainsaw", "RRRR RRRR! Needs oil badly"),
        WeaponStats(85, 0.6f, 40.0f, 600, "Demon Ripper", "Tears through anything with ease"),
        WeaponStats(140, 0.4f, 45.0f, 1200, "Soul Shredder", "The screams are part of the damage")
    };
    
    // ROCKET_LAUNCHER
    weaponStats_[static_cast<int>(WeaponCategory::ROCKET_LAUNCHER)] = {
        WeaponStats(80, 2.5f, 300.0f, 500, "RPG-Basic", "Point, shoot, BOOM!"),
        WeaponStats(130, 2.0f, 350.0f, 1000, "Devastator", "Nukes everything in sight"),
        WeaponStats(220, 1.5f, 400.0f, 2000, "Planet Buster", "May cause continental drift")
    };
    
    // FLAMETHROWER
    weaponStats_[static_cast<int>(WeaponCategory::FLAMETHROWER)] = {
        WeaponStats(35, 0.3f, 80.0f, 350, "Lighter Deluxe", "Toasty warm death"),
        WeaponStats(60, 0.25f, 100.0f, 700, "Dragon's Breath", "BBQ enemies to perfection"),
        WeaponStats(100, 0.2f, 120.0f, 1400, "Inferno Engine", "Turn everything to ash")
    };
    
    // MINIGUN
    weaponStats_[static_cast<int>(WeaponCategory::MINIGUN)] = {
        WeaponStats(25, 0.1f, 180.0f, 600, "Bullet Hose", "Spray and pray methodology"),
        WeaponStats(40, 0.08f, 220.0f, 1200, "Lead Storm", "1000 rounds per minute"),
        WeaponStats(65, 0.06f, 260.0f, 2400, "Death Dealer", "Bullets? Yes. Mercy? No.")
    };
    
    // LIGHTNING_ROD
    weaponStats_[static_cast<int>(WeaponCategory::LIGHTNING_ROD)] = {
        WeaponStats(45, 1.2f, 200.0f, 400, "Static Stick", "Shocking personality"),
        WeaponStats(75, 1.0f, 250.0f, 800, "Thunder Rod", "Zeus would be jealous"),
        WeaponStats(125, 0.8f, 300.0f, 1600, "Storm Caller", "Channel the fury of the heavens")
    };
    
    // SOUL_REAPER
    weaponStats_[static_cast<int>(WeaponCategory::SOUL_REAPER)] = {
        WeaponStats(55, 1.8f, 70.0f, 666, "Soul Poker", "Mildly uncomfortable afterlife"),
        WeaponStats(90, 1.5f, 80.0f, 1333, "Life Stealer", "Your soul belongs to me now"),
        WeaponStats(150, 1.2f, 90.0f, 2666, "Death Incarnate", "The end of all things")
    };
    
    // DEMON_CLAW
    weaponStats_[static_cast<int>(WeaponCategory::DEMON_CLAW)] = {
        WeaponStats(40, 0.7f, 50.0f, 333, "Imp Scratchers", "Kitty cat from hell"),
        WeaponStats(70, 0.5f, 60.0f, 666, "Hellhound Claws", "Fetch souls, good boy"),
        WeaponStats(115, 0.3f, 70.0f, 1333, "Demon Lord's Talons", "Rip reality itself apart")
    };
    
    // VOID_BLADE
    weaponStats_[static_cast<int>(WeaponCategory::VOID_BLADE)] = {
        WeaponStats(50, 1.1f, 65.0f, 777, "Shadow Slicer", "Cuts through light itself"),
        WeaponStats(85, 0.9f, 75.0f, 1554, "Void Cutter", "Slices holes in space"),
        WeaponStats(140, 0.7f, 85.0f, 3108, "Reality Ripper", "Unravels the fabric of existence")
    };
    
    // CRYSTAL_LAUNCHER
    weaponStats_[static_cast<int>(WeaponCategory::CRYSTAL_LAUNCHER)] = {
        WeaponStats(35, 1.3f, 190.0f, 450, "Gem Thrower", "Pretty and deadly"),
        WeaponStats(60, 1.1f, 230.0f, 900, "Crystal Cannon", "Shatter enemies like glass"),
        WeaponStats(100, 0.9f, 270.0f, 1800, "Prism of Destruction", "Refracts pain into rainbows")
    };
    
    // STORM_HAMMER
    weaponStats_[static_cast<int>(WeaponCategory::STORM_HAMMER)] = {
        WeaponStats(65, 2.2f, 55.0f, 550, "Thunder Mallet", "Makes enemies see stars"),
        WeaponStats(105, 1.8f, 65.0f, 1100, "Lightning Sledge", "Strikes twice as fast"),
        WeaponStats(170, 1.4f, 75.0f, 2200, "Tempest Destroyer", "Brings the storm with every swing")
    };
    
    // === FUNNY WEAPONS ===
    
    // RUBBER_CHICKEN
    weaponStats_[static_cast<int>(WeaponCategory::RUBBER_CHICKEN)] = {
        WeaponStats(10, 0.5f, 40.0f, 50, "Squeaky Chicken", "SQUEAK! The enemy's dignity dies first"),
        WeaponStats(25, 0.4f, 45.0f, 100, "Angry Rooster", "Clucks of fury and rubber fury"),
        WeaponStats(50, 0.3f, 50.0f, 200, "Chicken of Doom", "The final squeak of destiny")
    };
    
    // BANANA_LAUNCHER
    weaponStats_[static_cast<int>(WeaponCategory::BANANA_LAUNCHER)] = {
        WeaponStats(20, 1.0f, 150.0f, 75, "Fruit Flinger", "Potassium-powered projectiles"),
        WeaponStats(35, 0.8f, 180.0f, 150, "Banana Bazooka", "Slip up your enemies"),
        WeaponStats(60, 0.6f, 210.0f, 300, "Monkey's Revenge", "Go bananas on your foes")
    };
    
    // FISH_SLAPPER
    weaponStats_[static_cast<int>(WeaponCategory::FISH_SLAPPER)] = {
        WeaponStats(15, 0.6f, 45.0f, 25, "Tiny Trout", "Smells terrible, works great"),
        WeaponStats(30, 0.5f, 50.0f, 50, "Massive Mackerel", "The slap heard round the world"),
        WeaponStats(55, 0.4f, 55.0f, 100, "Legendary Salmon", "Swimming upstream to victory")
    };
    
    // POOL_NOODLE
    weaponStats_[static_cast<int>(WeaponCategory::POOL_NOODLE)] = {
        WeaponStats(8, 0.3f, 70.0f, 30, "Foam Fighter", "Splish splash, you're taking a bath"),
        WeaponStats(18, 0.25f, 80.0f, 60, "Aqua Annihilator", "Pool party of pain"),
        WeaponStats(35, 0.2f, 90.0f, 120, "Noodle of Destruction", "Al dente death")
    };
    
    // FEATHER_DUSTER
    weaponStats_[static_cast<int>(WeaponCategory::FEATHER_DUSTER)] = {
        WeaponStats(12, 0.4f, 55.0f, 40, "Dust Bunny", "Clean up your act (and enemies)"),
        WeaponStats(22, 0.3f, 60.0f, 80, "Maid's Revenge", "Spotless destruction"),
        WeaponStats(40, 0.25f, 65.0f, 160, "Feathers of Fury", "Tickle them to death")
    };
    
    // SQUIRT_GUN
    weaponStats_[static_cast<int>(WeaponCategory::SQUIRT_GUN)] = {
        WeaponStats(5, 0.2f, 80.0f, 20, "Water Pistol", "Pew pew! Getting enemies wet"),
        WeaponStats(12, 0.15f, 100.0f, 40, "Super Soaker", "Maximum wetness achieved"),
        WeaponStats(25, 0.1f, 120.0f, 80, "Hydro Cannon", "Pressure washing people")
    };
    
    // PILLOW_CANNON
    weaponStats_[static_cast<int>(WeaponCategory::PILLOW_CANNON)] = {
        WeaponStats(18, 1.5f, 120.0f, 60, "Sleepy Shooter", "Nap time for everyone"),
        WeaponStats(32, 1.2f, 150.0f, 120, "Dream Launcher", "Sweet dreams are made of this"),
        WeaponStats(55, 1.0f, 180.0f, 240, "Nightmare Mortar", "No rest for the wicked")
    };
    
    // RUBBER_DUCK_BOMBS
    weaponStats_[static_cast<int>(WeaponCategory::RUBBER_DUCK_BOMBS)] = {
        WeaponStats(25, 2.0f, 140.0f, 80, "Bath Bombs", "Squeaky clean explosions"),
        WeaponStats(45, 1.7f, 170.0f, 160, "Duckie Destroyer", "Quack BOOM!"),
        WeaponStats(75, 1.4f, 200.0f, 320, "Rubber Apocalypse", "The end times sound adorable")
    };
    
    // PIZZA_SLICER
    weaponStats_[static_cast<int>(WeaponCategory::PIZZA_SLICER)] = {
        WeaponStats(20, 0.8f, 50.0f, 45, "Cheese Cutter", "Slice of life... and death"),
        WeaponStats(35, 0.6f, 55.0f, 90, "Pepperoni Peeler", "Extra crispy enemies"),
        WeaponStats(60, 0.4f, 60.0f, 180, "Supreme Slicer", "Delivering hot death in 30 minutes")
    };
    
    // SOCK_LAUNCHER
    weaponStats_[static_cast<int>(WeaponCategory::SOCK_LAUNCHER)] = {
        WeaponStats(14, 0.7f, 90.0f, 35, "Dirty Laundry", "Stink bomb delivery system"),
        WeaponStats(26, 0.5f, 110.0f, 70, "Sock Rocket", "Toe-tally devastating"),
        WeaponStats(45, 0.4f, 130.0f, 140, "Stinky Striker", "Knock enemies off their feet")
    };
    
    // === MYTHICAL/EPIC WEAPONS ===
    
    // EXCALIBUR
    weaponStats_[static_cast<int>(WeaponCategory::EXCALIBUR)] = {
        WeaponStats(100, 0.8f, 85.0f, 1000, "Arthur's Blade", "The sword of legend awakens"),
        WeaponStats(160, 0.6f, 95.0f, 2000, "Caliburn Reborn", "Power of the rightful king"),
        WeaponStats(250, 0.4f, 105.0f, 4000, "Excalibur Prime", "The one true sword of destiny")
    };
    
    // MJOLNIR
    weaponStats_[static_cast<int>(WeaponCategory::MJOLNIR)] = {
        WeaponStats(120, 1.8f, 60.0f, 1200, "Thor's Hammer", "Only the worthy may wield it"),
        WeaponStats(190, 1.4f, 70.0f, 2400, "Thunder God's Might", "Lightning courses through your veins"),
        WeaponStats(300, 1.0f, 80.0f, 4800, "Mjolnir Unleashed", "The power of Asgard itself")
    };
    
    // DRAGONSLAYER
    weaponStats_[static_cast<int>(WeaponCategory::DRAGONSLAYER)] = {
        WeaponStats(90, 1.5f, 75.0f, 900, "Wyrm Bane", "Forged from dragon scales"),
        WeaponStats(145, 1.2f, 85.0f, 1800, "Ancient Slayer", "Death to all dragonkind"),
        WeaponStats(230, 0.9f, 95.0f, 3600, "Legendary Dragonslayer", "Even gods fear this blade")
    };
    
    // SHADOWBANE
    weaponStats_[static_cast<int>(WeaponCategory::SHADOWBANE)] = {
        WeaponStats(85, 1.0f, 80.0f, 850, "Light Bringer", "Banishes darkness forever"),
        WeaponStats(135, 0.8f, 90.0f, 1700, "Shadow's End", "No darkness can hide from this"),
        WeaponStats(215, 0.6f, 100.0f, 3400, "Bane of All Evil", "Purifies everything it touches")
    };
    
    // FROSTMOURNE
    weaponStats_[static_cast<int>(WeaponCategory::FROSTMOURNE)] = {
        WeaponStats(95, 1.3f, 75.0f, 950, "Icy Blade", "Hunger... eternal hunger"),
        WeaponStats(150, 1.0f, 85.0f, 1900, "Soul Reaver", "Feeds on the life force of enemies"),
        WeaponStats(240, 0.7f, 95.0f, 3800, "Frostmourne", "The cursed blade of the Lich King")
    };
    
    // FLAMBERGE
    weaponStats_[static_cast<int>(WeaponCategory::FLAMBERGE)] = {
        WeaponStats(80, 1.1f, 70.0f, 800, "Flame Tongue", "Burns with inner fire"),
        WeaponStats(130, 0.9f, 80.0f, 1600, "Infernal Blade", "Forged in the depths of hell"),
        WeaponStats(205, 0.7f, 90.0f, 3200, "Flamberge Supreme", "The eternal flame of destruction")
    };
    
    // WORLD_ENDER
    weaponStats_[static_cast<int>(WeaponCategory::WORLD_ENDER)] = {
        WeaponStats(150, 2.5f, 100.0f, 1500, "Apocalypse Bringer", "The beginning of the end"),
        WeaponStats(240, 2.0f, 120.0f, 3000, "Doomsday Device", "Civilizations tremble"),
        WeaponStats(400, 1.5f, 140.0f, 6000, "World Ender", "The final chapter of everything")
    };
    
    // GALAXY_PIERCER
    weaponStats_[static_cast<int>(WeaponCategory::GALAXY_PIERCER)] = {
        WeaponStats(110, 1.6f, 250.0f, 1100, "Star Shooter", "Reaches across solar systems"),
        WeaponStats(175, 1.3f, 300.0f, 2200, "Nebula Needle", "Pierces through spacetime"),
        WeaponStats(280, 1.0f, 350.0f, 4400, "Galaxy Piercer", "One shot, one galaxy")
    };
    
    // TIME_RIPPER
    weaponStats_[static_cast<int>(WeaponCategory::TIME_RIPPER)] = {
        WeaponStats(75, 0.9f, 85.0f, 750, "Chronos Cutter", "Slices through time itself"),
        WeaponStats(125, 0.7f, 95.0f, 1500, "Temporal Blade", "Past, present, future - all cut"),
        WeaponStats(200, 0.5f, 105.0f, 3000, "Time Ripper", "Tears holes in the timeline")
    };
    
    // REALITY_SHARD
    weaponStats_[static_cast<int>(WeaponCategory::REALITY_SHARD)] = {
        WeaponStats(130, 1.4f, 90.0f, 1300, "Dimension Slice", "Cuts between realities"),
        WeaponStats(205, 1.1f, 100.0f, 2600, "Existence Splitter", "Divides what is from what isn't"),
        WeaponStats(325, 0.8f, 110.0f, 5200, "Reality Shard", "The universe bleeds where it cuts")
    };
    
    // === MODERN/SCI-FI WEAPONS ===
    
    // PLASMA_RIFLE
    weaponStats_[static_cast<int>(WeaponCategory::PLASMA_RIFLE)] = {
        WeaponStats(45, 1.1f, 200.0f, 450, "Ion Blaster", "Superheated death rays"),
        WeaponStats(75, 0.9f, 240.0f, 900, "Plasma Cannon", "Melts through anything"),
        WeaponStats(125, 0.7f, 280.0f, 1800, "Solar Flare Rifle", "Harness the power of stars")
    };
    
    // LASER_CANNON
    weaponStats_[static_cast<int>(WeaponCategory::LASER_CANNON)] = {
        WeaponStats(40, 0.8f, 220.0f, 400, "Photon Beam", "Light speed destruction"),
        WeaponStats(70, 0.6f, 260.0f, 800, "Coherent Destroyer", "Focused annihilation"),
        WeaponStats(115, 0.4f, 300.0f, 1600, "Death Star Laser", "That's no moon...")
    };
    
    // GRAVITY_GUN
    weaponStats_[static_cast<int>(WeaponCategory::GRAVITY_GUN)] = {
        WeaponStats(55, 1.8f, 150.0f, 550, "Mass Driver", "Physics is your weapon"),
        WeaponStats(90, 1.5f, 180.0f, 1100, "Graviton Pulse", "Bend spacetime to your will"),
        WeaponStats(145, 1.2f, 210.0f, 2200, "Singularity Cannon", "Create black holes on demand")
    };
    
    // QUANTUM_BLASTER
    weaponStats_[static_cast<int>(WeaponCategory::QUANTUM_BLASTER)] = {
        WeaponStats(60, 1.3f, 190.0f, 600, "Particle Beam", "Quantum mechanics made weaponized"),
        WeaponStats(95, 1.0f, 230.0f, 1200, "Subatomic Destroyer", "Erases enemies at the molecular level"),
        WeaponStats(155, 0.8f, 270.0f, 2400, "Quantum Tunneler", "Ignores all known physics")
    };
    
    // ION_SWORD
    weaponStats_[static_cast<int>(WeaponCategory::ION_SWORD)] = {
        WeaponStats(50, 0.9f, 65.0f, 500, "Energy Blade", "Cuts through any material"),
        WeaponStats(85, 0.7f, 75.0f, 1000, "Plasma Saber", "The weapon of the future"),
        WeaponStats(135, 0.5f, 85.0f, 2000, "Ion Storm Blade", "Lightning contained in sword form")
    };
    
    // NANO_SWARM
    weaponStats_[static_cast<int>(WeaponCategory::NANO_SWARM)] = {
        WeaponStats(30, 0.4f, 120.0f, 300, "Micro Bots", "Tiny robots, big problems"),
        WeaponStats(50, 0.3f, 150.0f, 600, "Nano Cloud", "Self-replicating destruction"),
        WeaponStats(85, 0.2f, 180.0f, 1200, "Grey Goo Swarm", "The end of all matter")
    };
    
    // TESLA_COIL
    weaponStats_[static_cast<int>(WeaponCategory::TESLA_COIL)] = {
        WeaponStats(40, 1.4f, 160.0f, 400, "Electric Tower", "Wireless power transmission of death"),
        WeaponStats(70, 1.1f, 200.0f, 800, "Lightning Generator", "Tesla's dream realized"),
        WeaponStats(115, 0.8f, 240.0f, 1600, "Storm Engine", "Unlimited power!")
    };
    
    // PHOTON_LANCE
    weaponStats_[static_cast<int>(WeaponCategory::PHOTON_LANCE)] = {
        WeaponStats(65, 1.6f, 280.0f, 650, "Light Spear", "Pierces at light speed"),
        WeaponStats(105, 1.3f, 320.0f, 1300, "Photonic Pike", "Harnesses pure light energy"),
        WeaponStats(170, 1.0f, 360.0f, 2600, "Lance of Illumination", "Blinds and destroys simultaneously")
    };
    
    // ANTIMATTER_BOMB
    weaponStats_[static_cast<int>(WeaponCategory::ANTIMATTER_BOMB)] = {
        WeaponStats(100, 3.5f, 200.0f, 1000, "Annihilation Grenade", "E=mc² in explosive form"),
        WeaponStats(160, 3.0f, 250.0f, 2000, "Matter Destroyer", "Converts mass to pure energy"),
        WeaponStats(260, 2.5f, 300.0f, 4000, "Antimatter Warhead", "Erases everything from existence")
    };
    
    // BLACK_HOLE_GUN
    weaponStats_[static_cast<int>(WeaponCategory::BLACK_HOLE_GUN)] = {
        WeaponStats(80, 4.0f, 180.0f, 800, "Event Horizon", "Where physics goes to die"),
        WeaponStats(130, 3.5f, 220.0f, 1600, "Hawking Radiation", "Not even light escapes"),
        WeaponStats(210, 3.0f, 260.0f, 3200, "Galactic Devourer", "Consumes entire star systems")
    };
}

void WeaponTierSystem::initializeUpgrades() {
    // Initialize upgrade levels
    upgrades_ = {
        WeaponUpgrade(1, "+1", 1.3f, 0.9f, 1.1f, 100, "Minor improvements to all stats"),
        WeaponUpgrade(2, "+2", 1.6f, 0.8f, 1.2f, 250, "Significant enhancements"),
        WeaponUpgrade(3, "+3", 2.0f, 0.7f, 1.3f, 500, "Master-crafted perfection")
    };
}

WeaponStats WeaponTierSystem::getWeaponStats(WeaponCategory category, WeaponTier tier) const {
    int catIndex = static_cast<int>(category);
    int tierIndex = static_cast<int>(tier);
    
    if (catIndex >= 0 && catIndex < static_cast<int>(weaponStats_.size()) &&
        tierIndex >= 0 && tierIndex < static_cast<int>(weaponStats_[catIndex].size())) {
        return weaponStats_[catIndex][tierIndex];
    }
    
    throw std::invalid_argument("Invalid weapon category or tier");
}

WeaponUpgrade WeaponTierSystem::getUpgrade(int level) const {
    if (level >= 1 && level <= static_cast<int>(upgrades_.size())) {
        return upgrades_[level - 1];
    }
    
    throw std::invalid_argument("Invalid upgrade level");
}

WeaponStats WeaponTierSystem::calculateUpgradedStats(const WeaponStats& base, const WeaponUpgrade& upgrade) const {
    WeaponStats upgraded = base;
    
    upgraded.baseDamage = static_cast<int>(base.baseDamage * upgrade.damageMultiplier);
    upgraded.baseCooldown = base.baseCooldown * upgrade.cooldownMultiplier;
    upgraded.baseRange = base.baseRange * upgrade.rangeMultiplier;
    upgraded.name = base.name + " " + upgrade.suffix;
    upgraded.cost = base.cost + upgrade.upgradeCost;
    
    return upgraded;
}

std::unique_ptr<Weapon> WeaponTierSystem::createWeapon(WeaponCategory category, WeaponTier tier, int upgradeLevel) const {
    WeaponStats stats = getWeaponStats(category, tier);
    
    // Apply upgrades if specified
    if (upgradeLevel > 0) {
        WeaponUpgrade upgrade = getUpgrade(upgradeLevel);
        stats = calculateUpgradedStats(stats, upgrade);
    }
    
    // Create appropriate weapon type based on category
    switch (category) {
        case WeaponCategory::SWORD:
        case WeaponCategory::DAGGER:
        case WeaponCategory::AXE:
        case WeaponCategory::HAMMER:
        case WeaponCategory::SPEAR:
            return std::make_unique<MeleeWeapon>(stats.baseDamage, stats.baseCooldown, stats.baseRange);
            
        case WeaponCategory::BOW:
        case WeaponCategory::CROSSBOW:
        case WeaponCategory::STAFF:
        case WeaponCategory::WAND:
        case WeaponCategory::CANNON:
            return std::make_unique<RangedWeapon>(stats.baseDamage, stats.baseCooldown, stats.baseRange);
            
        default:
            throw std::invalid_argument("Unknown weapon category");
    }
}

std::vector<WeaponStats> WeaponTierSystem::getWeaponsForTier(WeaponTier tier) const {
    std::vector<WeaponStats> weapons;
    int tierIndex = static_cast<int>(tier);
    
    for (int catIndex = 0; catIndex < static_cast<int>(weaponStats_.size()); ++catIndex) {
        if (tierIndex < static_cast<int>(weaponStats_[catIndex].size())) {
            weapons.push_back(weaponStats_[catIndex][tierIndex]);
        }
    }
    
    return weapons;
}

std::string WeaponTierSystem::getTierName(WeaponTier tier) const {
    switch (tier) {
        case WeaponTier::BASIC: return "Basic";
        case WeaponTier::ADVANCED: return "Advanced";
        case WeaponTier::LEGENDARY: return "Legendary";
        default: return "Unknown";
    }
}

std::string WeaponTierSystem::getCategoryName(WeaponCategory category) const {
    switch (category) {
        // Classic Weapons
        case WeaponCategory::SWORD: return "Sword";
        case WeaponCategory::DAGGER: return "Dagger";
        case WeaponCategory::BOW: return "Bow";
        case WeaponCategory::CROSSBOW: return "Crossbow";
        case WeaponCategory::AXE: return "Axe";
        case WeaponCategory::STAFF: return "Staff";
        case WeaponCategory::HAMMER: return "Hammer";
        case WeaponCategory::SPEAR: return "Spear";
        case WeaponCategory::WAND: return "Wand";
        case WeaponCategory::CANNON: return "Cannon";
        
        // Badass Weapons
        case WeaponCategory::CHAINSAW: return "Chainsaw";
        case WeaponCategory::ROCKET_LAUNCHER: return "Rocket Launcher";
        case WeaponCategory::FLAMETHROWER: return "Flamethrower";
        case WeaponCategory::MINIGUN: return "Minigun";
        case WeaponCategory::LIGHTNING_ROD: return "Lightning Rod";
        case WeaponCategory::SOUL_REAPER: return "Soul Reaper";
        case WeaponCategory::DEMON_CLAW: return "Demon Claw";
        case WeaponCategory::VOID_BLADE: return "Void Blade";
        case WeaponCategory::CRYSTAL_LAUNCHER: return "Crystal Launcher";
        case WeaponCategory::STORM_HAMMER: return "Storm Hammer";
        
        // Funny Weapons
        case WeaponCategory::RUBBER_CHICKEN: return "Rubber Chicken";
        case WeaponCategory::BANANA_LAUNCHER: return "Banana Launcher";
        case WeaponCategory::FISH_SLAPPER: return "Fish Slapper";
        case WeaponCategory::POOL_NOODLE: return "Pool Noodle";
        case WeaponCategory::FEATHER_DUSTER: return "Feather Duster";
        case WeaponCategory::SQUIRT_GUN: return "Squirt Gun";
        case WeaponCategory::PILLOW_CANNON: return "Pillow Cannon";
        case WeaponCategory::RUBBER_DUCK_BOMBS: return "Rubber Duck Bombs";
        case WeaponCategory::PIZZA_SLICER: return "Pizza Slicer";
        case WeaponCategory::SOCK_LAUNCHER: return "Sock Launcher";
        
        // Mythical/Epic Weapons
        case WeaponCategory::EXCALIBUR: return "Excalibur";
        case WeaponCategory::MJOLNIR: return "Mjolnir";
        case WeaponCategory::DRAGONSLAYER: return "Dragonslayer";
        case WeaponCategory::SHADOWBANE: return "Shadowbane";
        case WeaponCategory::FROSTMOURNE: return "Frostmourne";
        case WeaponCategory::FLAMBERGE: return "Flamberge";
        case WeaponCategory::WORLD_ENDER: return "World Ender";
        case WeaponCategory::GALAXY_PIERCER: return "Galaxy Piercer";
        case WeaponCategory::TIME_RIPPER: return "Time Ripper";
        case WeaponCategory::REALITY_SHARD: return "Reality Shard";
        
        // Modern/Sci-Fi Weapons
        case WeaponCategory::PLASMA_RIFLE: return "Plasma Rifle";
        case WeaponCategory::LASER_CANNON: return "Laser Cannon";
        case WeaponCategory::GRAVITY_GUN: return "Gravity Gun";
        case WeaponCategory::QUANTUM_BLASTER: return "Quantum Blaster";
        case WeaponCategory::ION_SWORD: return "Ion Sword";
        case WeaponCategory::NANO_SWARM: return "Nano Swarm";
        case WeaponCategory::TESLA_COIL: return "Tesla Coil";
        case WeaponCategory::PHOTON_LANCE: return "Photon Lance";
        case WeaponCategory::ANTIMATTER_BOMB: return "Antimatter Bomb";
        case WeaponCategory::BLACK_HOLE_GUN: return "Black Hole Gun";
        
        default: return "Unknown";
    }
}

std::vector<WeaponCategory> WeaponTierSystem::getAllCategories() const {
    return {
        // Classic Weapons
        WeaponCategory::SWORD, WeaponCategory::DAGGER, WeaponCategory::BOW,
        WeaponCategory::CROSSBOW, WeaponCategory::AXE, WeaponCategory::STAFF,
        WeaponCategory::HAMMER, WeaponCategory::SPEAR, WeaponCategory::WAND,
        WeaponCategory::CANNON,
        
        // Badass Weapons
        WeaponCategory::CHAINSAW, WeaponCategory::ROCKET_LAUNCHER, WeaponCategory::FLAMETHROWER,
        WeaponCategory::MINIGUN, WeaponCategory::LIGHTNING_ROD, WeaponCategory::SOUL_REAPER,
        WeaponCategory::DEMON_CLAW, WeaponCategory::VOID_BLADE, WeaponCategory::CRYSTAL_LAUNCHER,
        WeaponCategory::STORM_HAMMER,
        
        // Funny Weapons
        WeaponCategory::RUBBER_CHICKEN, WeaponCategory::BANANA_LAUNCHER, WeaponCategory::FISH_SLAPPER,
        WeaponCategory::POOL_NOODLE, WeaponCategory::FEATHER_DUSTER, WeaponCategory::SQUIRT_GUN,
        WeaponCategory::PILLOW_CANNON, WeaponCategory::RUBBER_DUCK_BOMBS, WeaponCategory::PIZZA_SLICER,
        WeaponCategory::SOCK_LAUNCHER,
        
        // Mythical/Epic Weapons
        WeaponCategory::EXCALIBUR, WeaponCategory::MJOLNIR, WeaponCategory::DRAGONSLAYER,
        WeaponCategory::SHADOWBANE, WeaponCategory::FROSTMOURNE, WeaponCategory::FLAMBERGE,
        WeaponCategory::WORLD_ENDER, WeaponCategory::GALAXY_PIERCER, WeaponCategory::TIME_RIPPER,
        WeaponCategory::REALITY_SHARD,
        
        // Modern/Sci-Fi Weapons
        WeaponCategory::PLASMA_RIFLE, WeaponCategory::LASER_CANNON, WeaponCategory::GRAVITY_GUN,
        WeaponCategory::QUANTUM_BLASTER, WeaponCategory::ION_SWORD, WeaponCategory::NANO_SWARM,
        WeaponCategory::TESLA_COIL, WeaponCategory::PHOTON_LANCE, WeaponCategory::ANTIMATTER_BOMB,
        WeaponCategory::BLACK_HOLE_GUN
    };
}
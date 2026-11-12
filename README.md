# Skibidi Adventure

A 2D top-down action game built entirely in **C** using the **Allegro 5** game library. Navigate through a tile-based dungeon, shoot down slime enemies, collect coins, upgrade your character, and reach the trophy to win — all while racing the clock for a spot on the leaderboard.

> Final project for **Introduction to Programming (I) — NTHU, Fall 2024**

---

## Gameplay Overview

- Move your character through a tile-based dungeon loaded from a text map file
- Shoot enemies with your weapon (aim with the mouse)
- Collect coins dropped around the map
- Survive long enough to reach the trophy and clear the stage
- Beat your best time and climb the leaderboard

---

## Features

| Feature | Details |
|---|---|
| Scene system | Menu → Loading → Game → Win/Lose → Shop → Settings → Leaderboard |
| Combat | Directional shooting with mouse aim, knockback physics, two weapon types (Basic, Sniper) |
| Enemies | Slime enemies with pathfinding, hit animations, and death animations |
| Tile map | Loaded from `.txt` files, supports Floor, Wall, Door, Hole, Coin, Trophy, and Nurse tiles |
| Shop | Spend coins to permanently upgrade Max Health (+10) or Move Speed (+1) |
| Leaderboard | Completion times saved to file and sorted fastest-first (top 8 entries) |
| Audio | Background music per scene + sound effects (shooting, coins, win), with live volume control |
| Settings | Interactive BGM and SFX volume sliders |
| Persistent state | Coins, health, and speed carry over between play sessions via file I/O |
| Camera | Smooth camera that follows the player across the map |
| Debug flags | `DRAW_HITBOX` and `LOG_ENABLE` macros for development |

---

## Tech Stack

- **Language**: C (C11)
- **Graphics / Audio / Input**: [Allegro 5.2](https://liballeg.org/) — `allegro_image`, `allegro_font`, `allegro_ttf`, `allegro_audio`, `allegro_acodec`, `allegro_primitives`
- **Build**: GNU Make
- **IDE**: Xcode (macOS)

---

## Architecture

The game is built around a **scene management system** using C function pointers. Every scene (menu, gameplay, shop, etc.) is a `Scene` struct with four lifecycle callbacks:

```c
typedef struct Scene_ {
    char*          name;
    function_void  init;     // allocate resources
    function_void  update;   // game logic (runs every tick)
    function_void  draw;     // render frame
    function_void  destroy;  // free resources
} Scene;
```

`change_scene()` calls `destroy` on the current scene, swaps it, then calls `init` on the next — giving clean transitions with no memory leaks between scenes.

**Enemy management** uses a singly-linked list (`enemyNode`) so enemies can be removed in O(1) when they die, keeping the update loop efficient regardless of how many enemies are on screen.

**Maps** are defined in plain `.txt` files (up to 100×100 tiles). Each character in the file maps to a tile type (floor, wall, coin, trophy, enemy spawn, etc.), making it trivial to design new levels without recompiling.

```
Scene Graph
───────────
Menu
 ├─ Play ──→ Loading ──→ Game ──→ Win  ──→ Menu
 │                           └──→ Lose ──→ Menu
 ├─ Shop
 ├─ Settings
 └─ Leaderboard
```

---

## Project Structure

```
skibidi_toilet/
├── main.c               # Entry point
├── game.c / game.h      # Game loop, event handling, scene manager
├── utility.c / utility.h# Global constants, logging, file I/O helpers
├── map.c / map.h        # Tile map loading and rendering
├── player.c / player.h  # Player state, movement, animation
├── enemy.c / enemy.h    # Enemy AI, linked list, hit/death logic
├── weapon.c / weapon.h  # Weapon types and shooting
├── bullet.c / bullet.h  # Bullet physics and collision
├── trophy.c / trophy.h  # Trophy collectible (win condition)
├── UI.c / UI.h          # Reusable button component (hover states)
├── *_scene.c            # Individual scenes: menu, game, shop, settings, leaderboard, win, lose
├── Assets/              # Sprites, fonts, audio, map files
└── Makefile
```

---

## Build & Run

### Prerequisites

Install Allegro 5 via Homebrew (macOS):

```bash
brew install allegro
```

### Compile

```bash
cd skibidi_toilet
make
```

### Run

```bash
./game
```

### Clean

```bash
make clean
```

---

## Controls

| Input | Action |
|---|---|
| `W A S D` | Move player |
| Mouse | Aim weapon |
| Left Click | Shoot |
| `H` (in shop) | Buy Health upgrade |
| `S` (in shop) | Buy Speed upgrade |
| `Enter` | Confirm / Back |

---

## What I Learned

- Designing a **modular architecture in C** without classes — using structs and function pointers to replicate scene lifecycle patterns
- Managing **dynamic memory** manually (linked lists for enemies and bullets, explicit init/destroy on every resource)
- **Game loop fundamentals**: fixed-timestep update/draw separation, event-driven input handling with Allegro's event queue
- **File I/O** for persistent game state and leaderboard sorting
- Integrating **audio** (BGM + SFX) with real-time volume control
- Working with a **tile-based map system** and camera offset rendering

---

## Credits

Developed by **Nahathai Wonganawat** — NTHU Department of Computer Science, 2024

Assets: custom pixel art sprites, Minecraft.ttf font, original audio tracks

# Freedom: Raylib Wave Shooter

Freedom is a top-down 2D wave shooter built with C++ and Raylib. The player moves with the keyboard, aims with the mouse, survives escalating enemy waves, earns coins, buys upgrades, saves progress, and fights a final boss.

The project focuses on a clear real-time game loop, object-oriented entity design, collision handling, wave progression, UI screens, save/load persistence, and a modular C++ source layout.

## Features

- WASD movement with mouse aiming
- Cooldown-based bullet shooting
- Ten-wave progression with loading screens between waves
- Basic, tank, fast, and boss enemy types
- Boss wave and victory screen
- Player health, enemy contact damage, game-over state, and restart flow
- Coin rewards, medkit purchase, and shooting-speed upgrade
- Pause menu with save/load support
- Persistent high score in `data/highscore.txt`
- Save data stored in `data/save.txt`
- Custom sprites, level screens, UI artwork, and background music
- Modular C++ source files organized by game responsibility

## Controls

| Action | Input |
| --- | --- |
| Move | `W`, `A`, `S`, `D` |
| Aim | Mouse movement |
| Shoot | Hold left mouse button |
| Open shop | Click the shop icon |
| Pause | `P` |
| Save progress | Pause with `P`, then click Save |
| Load progress | Pause with `P`, then click Load |
| Continue loading screen | Any key or mouse click |
| Restart after game over | Click the restart button |

## Gameplay

The game starts on a title screen. After the player begins, wave 1 starts immediately. Normal waves grow in size and difficulty, while only one or two normal enemies are active at a time to keep combat readable.

Enemy progression:

- Waves 1-3: basic enemies
- Waves 4-6: basic and tank enemies
- Waves 7-9: basic, tank, and fast enemies
- Wave 10: boss only

After clearing wave 9, the game shows the final transition screens before spawning the boss. Defeating the boss displays the victory screen.

## Build And Run

This repository uses Premake-generated makefiles and Raylib.

On Windows with MinGW-w64 available in `PATH`, build with:

```bash
mingw32-make config=debug_x64
```

Run the game:

```bash
bin/Debug/raylib-wave-shooter.exe
```

For a clean rebuild:

```bash
mingw32-make clean
mingw32-make config=debug_x64
```

You can also use the included Windows build script:

```bash
build-MinGW-W64.bat
```

## Project Structure

```text
raylib-wave-shooter/
|-- assets/
|   |-- images/                 Sprites, screen art, UI icons, and level screens
|   `-- sounds/                 Background music
|-- build/                      Premake, Raylib build files, and generated build data
|-- data/                       Save file and high-score file
|-- include/                    Shared generated include file
|-- resources/                  Raylib template resource
|-- src/
|   |-- core/                   Game controller, state, constants, and split Game logic
|   |   |-- Constants.h
|   |   |-- Game.h
|   |   |-- Game.cpp
|   |   |-- GameResources.cpp
|   |   |-- GameInput.cpp
|   |   |-- GameSpawning.cpp
|   |   |-- GameUpdate.cpp
|   |   |-- GameDraw.cpp
|   |   `-- GameState.h
|   |-- entities/               Entity, player, enemies, and enemy variants
|   |-- systems/                Collision, save/load, shop, and wave management
|   |-- ui/                     HUD, buttons, start screen, and text helpers
|   |-- weapons/                Bullet logic
|   |-- application.rc
|   |-- icon.ico
|   `-- main.cpp
|-- Makefile
|-- raylib.make
|-- raylib-clean.make
|-- raylib-wave-shooter.make
|-- build-MinGW-W64.bat
`-- README.md
```

Generated folders such as `bin/` and `obj/` are created by the build process and are not required for source review.

## Code Organization

The `Game` class coordinates the main loop and high-level state, but its implementation is split by responsibility:

- `GameResources.cpp`: loading and unloading textures/music
- `GameInput.cpp`: menus, pause, shop, save/load input
- `GameSpawning.cpp`: enemy creation and wave spawn rules
- `GameUpdate.cpp`: gameplay updates, collisions, score, death, and victory
- `GameDraw.cpp`: rendering the active screen and UI

Entities use inheritance for shared behavior:

- `Entity` is the base class for transform, drawing, and hitbox data.
- `Player`, `Enemy`, and `Bullet` inherit from `Entity`.
- `BasicEnemy`, `FastEnemy`, `TankEnemy`, and `Boss` inherit from `Enemy`.

## Technologies

- C++17
- Raylib
- MinGW-w64
- Premake-generated Makefiles

## Notes

- Run the executable from the project layout shown above so relative asset and data paths resolve correctly.
- The game expects `assets/`, `resources/`, and `data/` to remain available beside the executable/build layout.
- Save/load uses simple text files so saved values can be inspected during testing.

## Future Improvements

- Add controller support
- Add more enemy attack patterns and boss phases
- Add difficulty settings
- Add animation and particle effects
- Improve save-file versioning
- Add automated gameplay/system tests where practical

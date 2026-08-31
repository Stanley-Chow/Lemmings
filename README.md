# Lemmings Game Engine

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/build-CMake-064F8C?logo=cmake&logoColor=white)
![SFML](https://img.shields.io/badge/graphics-SFML-8CC445)

A tick-driven puzzle-game simulation written in C++17. The engine coordinates
autonomous characters, environmental hazards, interactive tools, scoring, and
level progression on a data-driven 20 x 20 grid.

## Highlights

| System | Implementation |
| --- | --- |
| Actor model | Polymorphic hierarchy for characters, terrain, hazards, tools, and input |
| Character behavior | Explicit walking, falling, climbing, and bouncing state machine |
| World simulation | Deterministic tick loop with deferred entity cleanup |
| Interactions | Collision, freezing, burning, bouncing, saving, targeting, and placement |
| Levels | Text-based layouts mapped to actor types at runtime |

## Architecture

```text
GameController
`-- StudentWorld
    |-- loads levels and tracks score, time, and lives
    |-- owns and updates Actor instances
    |-- mediates actor-to-actor interactions
    `-- Actor
        |-- Static/reactive
        |   `-- FloorBrick, Exit, Trampoline, Spring, Net,
        |       Pheromone, OneWayDoor, Bonfire
        |-- Autonomous
        |   `-- LemmingFactory, Lemming, IceMonster
        `-- Input-driven
            `-- Cursor
```

`StudentWorld` acts as the simulation boundary. Actors query the world for
collisions and interactions instead of owning or mutating one another directly.
Entities marked dead during a tick are removed after the update pass, preserving
iterator validity.

## Gameplay systems

- Factories spawn lemmings on a timed schedule.
- Lemmings walk, fall, climb nets, bounce from springs and trampolines, and
  respond to pheromones.
- Ice monsters patrol supported terrain and eliminate nearby lemmings.
- Bonfires, exits, one-way doors, and player-placed tools alter movement and
  survival.
- Level completion combines saved-lemming requirements, remaining entities,
  lives, score, and a countdown timer.

## Controls

| Input | Action |
| --- | --- |
| Arrow keys | Move the placement cursor |
| `T` | Place a trampoline |
| `N` | Place a climbing net |
| `P` | Place a pheromone |
| `S` | Place a spring |
| `<` / `>` | Place a one-way door |

A tool can be placed only on an empty cell and when it remains in the current
level's inventory.

## Build and run

### Requirements

- CMake 3.21 or newer
- A C++17 compiler
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set

### Windows / PowerShell

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
Set-Location build/Release
./lemmings.exe
```

The vcpkg manifest resolves SFML 3. CMake copies the runtime assets and required
dynamic libraries beside the executable after a successful build.

## Repository layout

| Path | Purpose |
| --- | --- |
| `Actor.h`, `Actor.cpp` | Actor hierarchy and gameplay behavior |
| `StudentWorld.h`, `StudentWorld.cpp` | World lifecycle, level loading, interactions, and scoring |
| `Assets/` | Level definitions and runtime media |
| `CMakeLists.txt` | Portable C++17 build configuration |
| `vcpkg.json` | SFML dependency manifest |

## Implementation scope

My gameplay implementation is contained in `Actor.h`, `Actor.cpp`,
`StudentWorld.h`, and `StudentWorld.cpp`. The surrounding application framework
and runtime assets were provided as project support material and are not
represented as original work. See [Third-party material and
authorship](THIRD_PARTY_NOTICE.md) for details.

## Validation

- Windows x64 Release build
- CMake configuration with SFML 3.0.2
- Runtime dependency and asset packaging
- Launch smoke test

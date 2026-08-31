# Lemmings Game Engine

A C++17, tick-driven puzzle-game simulation built around an extensible actor
hierarchy. The implementation coordinates autonomous characters, hazards,
interactive tools, level progression, scoring, and deterministic per-tick
cleanup on a 20¡Á20 grid.

I implemented the gameplay model in `Actor.h`, `Actor.cpp`, `StudentWorld.h`,
and `StudentWorld.cpp`. The application framework and runtime assets were
provided as project support material. See
[Third-party material and authorship](THIRD_PARTY_NOTICE.md) for the exact
boundary.

## What the implementation demonstrates

- **Polymorphic actor model:** floor bricks, factories, lemmings, monsters,
  exits, hazards, traversal aids, directional doors, pheromones, and the
  placement cursor share a common lifecycle.
- **Explicit character state machine:** lemmings transition among walking,
  falling, climbing, and bouncing states while tracking fall distance, bounce
  height, direction, and action timing.
- **World-mediated interactions:** actors query `StudentWorld` for collision,
  climbing, targeting, freezing, burning, bouncing, saving, and tool placement
  instead of directly owning one another.
- **Safe entity lifecycle:** dead actors are marked during simulation and
  deleted after the update pass, avoiding iterator invalidation.
- **Data-driven levels:** level text files define the board layout and available
  tools; the world maps each symbol to its corresponding actor type.

## Architecture

```text
Game framework
  ©¸©¤ StudentWorld
       ©À©¤ owns and updates Actor instances
       ©À©¤ loads levels and tracks score/time/lives
       ©¸©¤ mediates actor-to-actor interactions

Actor
  ©À©¤ Static/reactive: FloorBrick, Exit, Trampoline, Spring, Net,
  ©¦                   Pheromone, OneWayDoor, Bonfire
  ©À©¤ Autonomous:      LemmingFactory, Lemming, IceMonster
  ©¸©¤ Input-driven:    Cursor
```

## Build

Requirements:

- CMake 3.21 or newer
- A C++17 compiler
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set

PowerShell:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
Set-Location build/Release
./lemmings.exe
```

For a single-config generator, run the executable from its output directory
(for example, `./build/lemmings`). CMake copies `Assets/` beside the executable
after every successful build.

## Controls

- Arrow keys: move the placement cursor
- `T`: place a trampoline
- `N`: place a climbing net
- `P`: place a pheromone
- `S`: place a spring
- `<` / `>`: place a one-way door

A placement succeeds only on an empty cell and when that tool remains in the
current level's inventory.

## Repository hygiene

Build output, dependency installations, IDE-specific settings, local property
sheets, archives, and debug symbols are intentionally excluded. Dependencies
are declared in `vcpkg.json` instead of being committed.

# Lemmings 2D: An Interactive Game Engine in C++

Welcome to the official repository for **Lemmings 2D**, a complete graphical grid-based video game engine written from scratch in modern C++. This project implements an interactive ecosystem featuring dynamic pathfinding, structural mechanics, tool placement systems, and a fully real-time simulation loop.

---

## Project Architecture Overview

The system architecture utilizes polymorphism to model game components dynamically. The engine operates on an elegant inheritance hierarchy to orchestrate life cycles, spatial movement, collisions, and entity rendering inside a real-time game world.

```text
├── src/
│   ├── Actor.h / .cpp       # Polymorphic base class and concrete game objects
│   ├── StudentWorld.h/.cpp  # Game world manager; handles simulation logic and grid interactions
│   ├── GameConstants.h      # Directional macros, sound constants, and asset specifications
│   ├── provided.h           # Core engine definitions, framework hooks, and base utilities
│   └── provided.cpp         # Low-level rendering layer and asset loader engine
├── assets/
│   └── levels/              # Text files (.txt) containing structural grid layouts
└── main.cpp                 # Game runtime driver initialization shell

```

---

## Core Architectural Engine (`provided.h` / `provided.cpp`)

The foundational mechanics, runtime graphics loop, window configurations, and asset indexing pipelines are encapsulated cleanly inside the pre-configured backend utilities defined in `provided.h` and `provided.cpp`. This high-performance subsystem abstractly manages the complex underlying engine loops so that the gameplay architecture remains separate and clean.

### Key Functionalities Managed Inside the Core Subsystem:

* **Abstract Base Hooks & Game Control Interfaces**: Defines pure virtual template trees (such as the base world wrapper and display controllers) to bind gameplay assets seamlessly to the underlying system loop.
* **Low-Level Graphical Layer & Asset Loader**: Manages the cross-platform rendering context. It loads standard structural level files (`.txt`), tracks resource paths, draws sprite data matrices to the screen buffer in real-time, and refreshes the graphics context at fixed time steps.
* **Sound & Event Synthesizer Infrastructure**: Houses the system hooks to trigger event sound effects (`SOUND_LEMMING_DIE`, `SOUND_LEVEL_COMPLETED`, etc.) across individual gameplay contexts.
* **Input Peripherals Hook Layer**: Tracks key presses (such as cursor movements and action selections) and pushes active keyboard coordinates straight down to the application layer.

---

## Engine Pipelines & Gameplay Subsystems

### 1. Object-Oriented Polymorphic Game Objects (`Actor`)

To keep individual entities lightweight and scalable, the game uses a structured inheritance hierarchy rooted in a base `Actor` object.

* **Shared Attributes**: Every entity tracks active 2D grid coordinates, structural type flags, image state variables, and directional indices.
* **Dynamic Lifecycle Execution**: An abstract `doSomething()` interface enforces unique state machine logic for each object during every execution pass of the game loop.
* **Sub-Systems Implemented**: Includes specialized actors like basic Lemmings, blocks/terrain obstacles, exits, trap doors, and interactive tools.

### 2. The Simulation Manager (`StudentWorld`)

The container environment acts as the central coordinator for the runtime environment:

1. **Level Setup (`init`)**: Parses text layout templates, dynamically creates and stores structural game objects, and positions coordinates.
2. **Real-Time Frame Tick (`move`)**: Prompts every active entity to execute its state machine logic, sweeps away destroyed objects via cleanup sweeps, updates dashboard score summaries, and checks win conditions.
3. **Resource Destruction (`cleanUp`)**: Safely deallocates all remaining actor pointers on the heap whenever a level transitions, preventing memory leaks.

### 3. State-Machine AI Pathfinding

The game characters utilize autonomous algorithmic rules to explore the environment:

* **Gravity & Terminal Fall Bounds**: Evaluates step-down trajectories each frame. If a character falls beyond maximum structural damage limits, death procedures execute automatically.
* **Directional Traversal & Obstacle Check**: Tests front-facing grid locations for walls, reversing horizontal steps immediately upon tracking blocked paths.

---

## Compilation & Verification

The codebase requires a modern compiler supporting standard C++11 or higher, alongside dependencies configured within the graphics project ecosystem.

### Build and Compilation

```bash
g++ -std=c++17 src/StudentWorld.cpp src/Actor.cpp src/provided.cpp main.cpp -o LemmingsGame -lglut -lGL -lGLU

```

### Running the Project

Ensure that all level layouts are stored inside your asset tracking directories before initializing the executable runtime:

```bash
./LemmingsGame

```

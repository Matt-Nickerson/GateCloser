
# GATECLOSER
Matthew Nickerson (mwnickerson@wpi.edu) - Gate + Visitor Assets, Gate logic, Gate collision, Visitor Logic, first version Wave Spawning resource loader functions, Scoring, Streaks, Lives

Richard DeBlasio (rcdeblasio@wpi.edu) - Advanced Wave Spawning,Background, Title Screen, GameOver Screen, Sound Effects, Additonal Assets, lots of Bug Fixes.

A tiny C++/Dragonfly ASCII game where you control three gates to manage the flow of GOOD, EVIL, and WIZARD visitors. Use timing to block the right ones, let the right ones through, and rack up points with a streak multiplier.

## Gameplay Overview

- There are 3 gates arranged vertically: Top, Middle, Bottom.
- You control them with a simple rule: **Middle open <-> Outside closed** (and vice-versa).
- Visitors spawn on the left and move right toward the gates.

### Visitor Rules
- **GOOD** - should be let through when their gate is **OPEN**.
- **EVIL** - should be **blocked** by a **CLOSED** gate (or **crushed** while it’s **CLOSING**).
- **WIZARD** - ignores a **CLOSED** door; the only way to stop a wizard is to **shut the door on them** (while **CLOSING** and overlapping the doorway).
- Breaking any rule costs a life (score system already wires in lives & multiplier).

## Controls

- **SPACE** - Toggle the gate pattern (only when all gates are idle):
  - If middle is **OPEN**, it starts **closing** and the outside gates start **opening**.
  - If middle is **CLOSED**, it starts **opening** and the outside gates start **closing**.

## Scoring & Lives

**Correct actions:**
- GOOD let through -> **+100 × multiplier**
- EVIL blocked/crushed -> **+100 × multiplier**
- WIZARD crushed while door is **CLOSING** → **+150 × multiplier**

**Wrong actions:**
- Any rule violation -> **lose 1 life** and **reset multiplier**

**Multiplier:**
- Starts at **×1.0**; increases by **+0.5** per correct streak up to **×5.0**.

**HUD (top of screen):**
- **Score**, **Lives**, **Mult xN.N**

Everything above is handled by `ScoreSystem` and integrated via `Visitor::correct()` / `Visitor::wrong()`.

## Wave Spawning

Waves are spawned in intervals with ONLY Valid Combinations for the Gate to Interact with!

Waves are encouraged to spawn smaller visitor waves over ones of bigger sizes. 

Wizard waves are all spawned in this combination to mix up gameplay.

Every run of GATECLOSER is different from the last!

## Gate Logic (4-sprite animation)

Each gate uses four sprites and lets the engine advance frames smoothly:

- `gate_opening` - opening animation (slowdown 1-2)
- `gate_closing` - closing animation (slowdown 1-2)
- `gate_open_idle` - single-frame idle
- `gate_closed_idle` - single-frame idle

**Solidness by state:**
- **OPEN/OPENING** -> `SOFT` (no blocking)
- **CLOSED/CLOSING** -> `HARD` (blocks/crushes)

This ensures collisions happen only when intended.

## Project Structure

```text
sprites/
  door_opening.txt
  door_closing.txt
  door_open_idle.txt
  door_closed_idle.txt
  good_visitor.txt
  evil_visitor.txt
  wizard_visitor.txt
  walkway.txt
  wizard_visitor.txt
  gameover-spr.txt
  gamestart-spr.txt

sounds/
  explosion.wav
  gate.wav
  scream.wav

src/
  Gate.h
  Gate.cpp
  Visitor.h
  Visitor.cpp
  ScoreSystem.h
  ScoreSystem.cpp
  Wave.h
  Wave.cpp
  WaveSpawner.cpp
  WaveSpawner.h
  Walkway.h
  Walkway.cpp
  Star.h
  Star.cpp
  GameStart.h
  GameStart.cpp
  GameOver.h
  GameOver.cpp
  game.cpp
```

## Build & Run

### Requirements
- C++17
- Dragonfly engine (course build)
- Visual Studio 2022 (or your preferred toolchain)

### Setup
1. Add Dragonfly include/lib paths to your project.
2. Add all `*.cpp` and `*.h` files to your project.

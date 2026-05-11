# modular_pico

A Raspberry Pi Pico W C++ project built with the Pico SDK and CMake.

Use with the official Raspberry Pi Pico Project Extension on VS Code
for best results

## Overview

This project currently demonstrates modular wrappers for:

- LED control (`LED` class)
- UART/USB serial I/O (`UART` class)
- Button control (`Button` class)
- ADC joystick control (`Joystick` class)
- 8x8 LED matrix control with multiplexing (`Matrix` class)

The current application is a playable Snake game running on an 8x8 LED matrix.

Gameplay flow:

1. Idle animation displays while waiting to start.
2. Press the start button to begin.
3. Move with the joystick and collect goal pixels to grow.
4. Snake auto-moves in the last selected direction.
5. On collision, an end-game animation plays and returns to idle.

## Project Layout

- `main.cpp`: application loop (render, sample input, start game, auto-move, input update)
- `lib/uart_com.hpp`, `lib/uart_com.cpp`: UART/USB serial class
- `lib/led.hpp`, `lib/led.cpp`: LED helper class
- `lib/button.hpp`, `lib/button.cpp`: start button input
- `lib/joystick.hpp`, `lib/joystick.cpp`: analog joystick sampling + digital direction mapping
- `lib/matrix_display.hpp`, `lib/matrix_display.cpp`: 8x8 LED matrix shifting/multiplex rendering
- `lib/player.hpp`, `lib/player.cpp`: snake state, movement, growth, body drawing
- `lib/game.hpp`, `lib/game.cpp`: game state machine, goal spawning, animation, movement timing
- `CMakeLists.txt`: Pico SDK + target configuration
- `pico_sdk_import.cmake`: Pico SDK import helper
- `.vscode/tasks.json`: build/flash/run tasks used by VS Code

## Build Configuration

From `CMakeLists.txt`:

- C standard: `C11`
- C++ standard: `C++17`
- Board: `pico2_w`
- Target: `modular_pico`
- Linked libraries:
  - `pico_stdlib`
  - `hardware_uart`
  - `hardware_adc`
- stdio enabled:
  - UART: enabled
  - USB: enabled

## Controls

- Start button (`GP16`): starts a new game from idle.
- Joystick (`GP26`, `GP27`): selects movement direction.
- Once a direction is chosen, movement continues automatically at a fixed interval.

## Timing and Gameplay Tuning

Useful values you can tweak:

- Auto-move interval: `AUTO_MOVE_INTERVAL` in `lib/game.hpp` (microseconds).
- Input sample cadence: `5000` us gate in `main.cpp`.
- Goal breathing animation speed: 30 ms phase update in `Game::render_screen()`.
- End-game animation timings: phase thresholds in `Game::render_screen()`.

## Build and Run

### VS Code Tasks (already configured)

Use:

- `Compile Project`: builds with Ninja in `build/`
- `Run Project`: loads `build/modular_pico.elf` via `picotool`
- `Flash`: programs via OpenOCD
- `Rescue Reset` / `RISC-V Reset (RP2350)`: recovery/debug utility tasks

### CLI (if you want to build manually)

If `build/` is not configured yet:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
ninja -C build
```

Run on device (one example):

```bash
${HOME}/.pico-sdk/picotool/2.2.0-a4/picotool/picotool load build/modular_pico.elf -fx
```

## Current Output Artifacts

A successful build generates outputs such as:

- `build/modular_pico.elf`
- `build/modular_pico.uf2`
- `build/modular_pico.dis`
- disassembly files in `build/`

## Notes

- Matrix rendering uses active-low bit patterns (bit cleared means LED on).
- This repository still includes reusable wrappers (`UART`, `LED`) even though the game loop is now the primary app behavior.

## Git

This repository is initialized with Git and includes a `.gitignore` for Pico/CMake build artifacts and local machine-specific editor files.

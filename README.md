# modular_pico

A Raspberry Pi Pico W C++ project built with the Pico SDK and CMake.

Use with the official Raspberry Pi Pico Project Extension on VS Code
for best results

## Overview

This project currently demonstrates modular wrappers for:

- LED control (`LED` class)
- UART/USB serial I/O (`UART` class)
- Button class is still being worked on, I got distracted with UART

The current `main.cpp` runs a simple serial echo loop:

1. Initializes stdio
2. Creates `UART serial("usb")`
3. Reads a line into a fixed buffer
4. Prints the line back

## Project Layout

- `main.cpp`: application entry point and echo loop
- `lib/uart_com.hpp`, `lib/uart_com.cpp`: UART/USB serial class
- `lib/led.hpp`, `lib/led.cpp`: LED helper class
- `lib/button.hpp`, `lib/button.cpp`: button class scaffold
- `CMakeLists.txt`: Pico SDK + target configuration
- `pico_sdk_import.cmake`: Pico SDK import helper
- `.vscode/tasks.json`: build/flash/run tasks used by VS Code

## Build Configuration

From `CMakeLists.txt`:

- C standard: `C11`
- C++ standard: `C++17`
- Board: `pico_w`
- Target: `modular_pico`
- Linked libraries:
  - `pico_stdlib`
  - `hardware_uart`
- stdio enabled:
  - UART: enabled
  - USB: enabled

## Build and Run

### VS Code Tasks (already configured)

Use:

- `Compile Project`: builds with Ninja in `build/`
- `Run Project`: loads `build/modular_pico.elf` via `picotool`

### CLI (if you want to build manually)

If `build/` is not configured yet:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
ninja -C build
```

## Serial Behavior Notes

- In USB mode (`UART("usb")`), `UART` waits for a USB serial connection (`stdio_usb_connected()`) before continuing.
- `read_line(...)` reads until newline (`\n`) or carriage return (`\r`).
- If your serial monitor sends only raw text without line endings, the line will not complete.

## Current Output Artifacts

A successful build generates outputs such as:

- `build/modular_pico.elf`
- `build/modular_pico.uf2`
- disassembly files in `build/`

## Git

This repository is initialized with Git and includes a `.gitignore` for Pico/CMake build artifacts and local machine-specific editor files.

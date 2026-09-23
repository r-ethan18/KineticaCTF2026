# Jump Around & Find Out - CTF Challenge

A custom C/C++ Reverse Engineering CTF (Capture The Flag) challenge designed for binary analysis, function patching, and debugger-based memory inspection.

## Overview

- **Core Mechanics**: The player must figure out the correct sequence of 6 "jump" functions to reconstruct a hidden target string.
- **Obfuscation**: Secret payload chunks are obfuscated at compile-time via PRNG byte shuffling with fixed seeds to prevent static `strings` analysis.
- **Verification**: Rolling SHA-256 checksums update global step match flags (`step_0_matched` .. `step_5_matched`) in memory as functions are invoked.
- **History & Rollback**: An `undo_jump` function allows rolling back the last state traversal.

## Project Structure

```text
├── Makefile                   # Build configuration
├── flake.nix                  # Nix dev environment configuration
├── executable/
│   ├── INSTRUCTIONS.md        # Distribution patching notes
│   └── jump-around-find-out   # Standalone binary target for distribution
├── scripts/
│   └── generate_challenge.py  # Python script to generate src/main.c
└── src/
    ├── main.c                 # Main challenge logic (generated or edited)
    ├── sha256_wrapper.cpp     # C++ SHA-256 implementation wrapper
    └── sha256_wrapper.h       # SHA-256 C header interface
```

## Build Instructions
> [!WARNING]  
> `make generate`, `make all` and `make` will regenerate the source from the python script, thus discarding all changes done to the source file

> [!IMPORTANT]  
> `make build` is the preferred build command

### Prerequisites
- GCC / G++ (C11 and C++17 support)
- Python 3
- (Optional) Nix with Flakes (`nix develop`) for NixOS users

### Available Build Commands

- **`make clean`**  
  Removes built executables (`jump-around-find-out`, `executable/jump-around-find-out`), object files (`src/*.o`), and `compile_commands.json`.

- **`make generate`**  
  Executes `scripts/generate_challenge.py` to generate or regenerate `src/main.c` with obfuscated string chunks and expected SHA-256 step hashes.

- **`make build`**  
  Compiles the C/C++ source code directly into the `jump-around-find-out` executable and copies it to `executable/jump-around-find-out` (automatically patching interpreter and RPATH on NixOS).

- **`make`**  
  Default target. Compiles the executable using the current `src/main.c` without overwriting custom edits.

- **`make compile_commands.json`**  
  Generates `compile_commands.json` using `bear` for LSP / IDE integration.

## Challenge Architecture & Development Notes

1. **City Jump Functions**:
   - `jump_to_venice()` (Chunk 0)
   - `jump_to_florence()` (Chunk 1)
   - `jump_to_naples()` (Chunk 2)
   - `jump_to_verona()` (Chunk 3)
   - `jump_to_palermo()` (Chunk 4)
   - `jump_to_milan()` (Chunk 5)
   - `undo_jump()` (Reverts last jump step)

2. **NixOS Compatibility**:
   When built on NixOS, the Makefile automatically patches `executable/jump-around-find-out` with `patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2` and removes RPATH to produce a portable binary for general Linux distribution.

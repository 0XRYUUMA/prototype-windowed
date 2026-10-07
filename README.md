# prototype-windowed

A small Win32 wrapper for **Prototype (2009)** that keeps the excellent [PrototypeFix](https://www.nexusmods.com/prototype/mods/52) windowed-mode logic, but restores a normal Windows frame after the game creates its borderless window.

## What it does

- Keeps `BorderlessWindow = true` in PrototypeFix so the game reliably enters windowed mode.
- Loads the original PrototypeFix as `prototype_fix_original.dll`.
- Waits for the game window to exist.
- Reapplies a normal `WS_OVERLAPPEDWINDOW` frame:
  - title bar
  - borders
  - minimize / maximize / close
  - movable window
  - resizable window
- Removes `WS_EX_TOPMOST` when present.
- Starts at 1920×1080 and centers the window.
- If another hook removes the frame during startup, the wrapper restores it without fighting the user's later window position.
- Writes `prototype_window_frame.log` for diagnostics.

## Installation

1. Install **PrototypeFix 1.8** normally.
2. Rename the original `prototype_fix.asi` from PrototypeFix to:
   `prototype_fix_original.dll`
3. Copy these files from this repository's `dist/` folder into the game directory:
   - `prototype_fix.asi`
   - `prototype_fix.ini`
4. Keep `BorderlessWindow = true` in the INI.
5. Launch the game normally.

> The wrapper does **not** patch or replace `prototypef.exe`.

## Why this approach works

PrototypeFix needs its borderless routine enabled to reliably force Prototype into windowed mode. Disabling `BorderlessWindow` also disables the part we need.

Instead of patching PrototypeFix internally, this project lets PrototypeFix finish creating the window first, then changes the final Win32 window style afterwards. This avoids fighting the game's/PrototypeFix's creation path.

## Files

- `src/prototype_window_frame.cpp` — wrapper source.
- `dist/prototype_fix.asi` — tested 32-bit wrapper binary.
- `dist/prototype_fix.ini` — working 1920×1080 configuration.
- `dist/INSTALL.txt` — quick installation instructions.

## Credits

- **PrototypeFix** by emoose — original game fixes/windowed-mode implementation.
- Wrapper integration and window-frame logic developed for this repository.

This repository intentionally does **not** include proprietary game binaries such as `prototypef.exe`, `prototypeenginef.dll`, or Bink DLLs.

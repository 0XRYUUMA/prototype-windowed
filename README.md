# prototype-windowed

An independent addon for **Prototype (2009)** + **PrototypeFix 1.8**.

PrototypeFix reliably forces the game into windowed mode through its `BorderlessWindow` path. This addon waits until that setup is finished, then restores a normal Windows frame around the final game window.

## Result

- Windowed mode
- 1920×1080 initial client area
- Automatic centering
- Normal title bar and borders
- Movable and resizable window
- Minimize / maximize / close buttons
- PrototypeFix remains untouched
- No BAT, no terminal, no renamed files

## Required dependency

This addon requires **PrototypeFix 1.8 by emoose**.

Official Nexus Mods page:
https://www.nexusmods.com/prototype/mods/52

Install PrototypeFix first. This repository does not redistribute it.

## Installation

1. Install **PrototypeFix 1.8** from the official page above.
2. Download this mod.
3. Copy these two files into the game folder:
   - `prototype_windowed.asi`
   - `prototype_fix.ini`
4. When Windows asks, replace the existing `prototype_fix.ini`.
5. Launch the game normally.

That's it.

The original `prototype_fix.asi` from PrototypeFix stays exactly where it is. This addon is loaded beside it:

```text
prototype_fix.asi       <- original PrototypeFix
prototype_windowed.asi  <- this addon
prototype_fix.ini       <- preconfigured for this addon
```

## Configuration included

The supplied INI already contains:

```ini
BorderlessWindow = true
WindowResolution.Width = 1920
WindowResolution.Height = 1080
```

`BorderlessWindow = true` must remain enabled because PrototypeFix uses that path to reliably force the game into windowed mode. This addon restores the normal Windows frame afterwards.

## Files

- `dist/prototype_windowed.asi` — independent 32-bit addon.
- `dist/prototype_fix.ini` — ready-to-use PrototypeFix configuration.
- `src/prototype_window_frame.cpp` — reference source for the addon.

## How it works

The addon does **not** replace, wrap or patch `prototype_fix.asi`.

It waits briefly for PrototypeFix and the game to finish creating/styling the real game window. It then reapplies `WS_OVERLAPPEDWINDOW`, removes `WS_EX_TOPMOST`, sizes the client area for 1920×1080 and centers the window.

If another startup hook removes the frame again, the addon restores the frame without continuously forcing the user's window position.

## Credits

- **PrototypeFix** by emoose — required dependency and original windowed-mode/game-fix implementation: https://www.nexusmods.com/prototype/mods/52
- **prototype-windowed** — independent window-frame addon maintained in this repository.

This repository does not redistribute PrototypeFix or proprietary Prototype game binaries.

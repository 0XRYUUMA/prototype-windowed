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
- PrototypeFix remains completely untouched
- No renamed PrototypeFix files

## Easy installation

1. Install **PrototypeFix 1.8** normally.
2. Copy these two files from `dist/` to the game folder:
   - `prototype_windowed.asi`
   - `Install_PrototypeWindowed.bat`
3. Run `Install_PrototypeWindowed.bat` once.
4. Launch the game normally.

That's it.

The installer only updates the existing `prototype_fix.ini` so that:

```ini
BorderlessWindow = true
WindowResolution.Width = 1920
WindowResolution.Height = 1080
```

Before changing the INI it creates:

`prototype_fix.ini.prototype-windowed-backup`

## Files

- `dist/prototype_windowed.asi` — independent 32-bit addon.
- `dist/Install_PrototypeWindowed.bat` — one-click configuration helper.
- `src/prototype_window_frame.cpp` — reference source for the addon.

## How it works

The addon does **not** replace, wrap or patch `prototype_fix.asi`.

Both ASIs are loaded normally:

```text
prototype_fix.asi       <- original PrototypeFix
prototype_windowed.asi  <- this addon
```

The addon waits briefly for PrototypeFix and the game to finish creating/styling the real game window. It then reapplies `WS_OVERLAPPEDWINDOW`, removes `WS_EX_TOPMOST`, sizes the client area for 1920×1080 and centers the window.

If another startup hook removes the frame again, the addon restores the frame without continuously forcing the user's window position.

## Credits

- **PrototypeFix** by emoose — required dependency and original windowed-mode/game-fix implementation.
- **prototype-windowed** — independent window-frame addon maintained in this repository.

This repository does not redistribute PrototypeFix or proprietary Prototype game binaries.

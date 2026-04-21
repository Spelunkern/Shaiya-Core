# Client Module

This module contains `Game.exe` hooks and client-side quality-of-life patches.

## Environment

- Windows 10 or newer
- Visual Studio 2022
- C++23
- Microsoft DirectX SDK (June 2010)
- x86 build target

## Build

```powershell
$env:DXSDK_DIR='C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\'
& 'C:\BuildTools\MSBuild\Current\Bin\MSBuild.exe' ..\Shaiya-Core.sln /t:sdev-client /p:Configuration=Release /p:Platform=x86 /m
```

## CONFIG.INI

The client reads several user-facing options from `CONFIG.INI`.

```ini
[ADVANCED]
; 1 skips the updater token check and lets Game.exe launch directly.
; 0 keeps the stock updater-required behavior.
SKIPUPDATER=0

; Empty or missing defaults to 127.0.0.1.
IP=

; Enables or disables the GM/admin ID view patch where supported.
IDVIEW=OFF

; Visual title rendering from cloak data.
TITLES=ON

; Visual colored-name rendering from helmet data.
COLOUR=ON
```

## Commands

- `/font` opens the Windows font picker and persists the selected in-game font.
- `/titles on` and `/titles off` toggle visual item titles at runtime.
- `/colour on` and `/colour off` toggle visual colored names at runtime.

## Stable Client Features

- PNG interface support for known UI texture paths.
- PNG screenshot output instead of JPG.
- Chat UTF-8 support for composed Unicode input, including Vietnamese IME text, without enabling the global Vietnam codepage branch.
- Unicode-safe main window handling while preserving the normal `Shaiya` window title.
- EP4 UI support for the selected HUD pieces, excluding inventory and the stock EXP/Bless bar handling.
- Raid 150 UI component using PNG raid textures.
- Battleground button under the main stats UI.
- Single-server selection skip with a safe delayed selection path.
- Login splash skip, documented in code as `Login crap skip`.
- Skip mode selection and force Ultimate Mode at character creation.
- Name availability check bypass for character creation.
- Client-side buy/sell quantity limit increased to 255.
- Target HP viewer anchored to the native target frame and using the configured game font.
- Movable/persistent buff layout with F6 and left-click behavior.
- Item titles from cloak data and colored names from helmet data, including rainbow color mode.
- Discord RPC with a static configurable message.
- New resolution entries.
- Cooldown for subaction messages (20 seconds).
- Level-up message texture suppression.
- Visual timer alignment for logout and ress-leader timers.

## Asset Notes

- Interface assets used by the PNG redirect must exist in the client `Data/interface` tree.
- Raid button assets are expected as PNG.
- The client intentionally keeps icon assets outside the broad PNG redirect unless a feature explicitly handles them.

## Removed Or Disabled Work

- The experimental Item Mall vehicle preview patch was removed because it did not provide reliable behavior.
- The large cooldown-number overlay experiment was removed because it had too many rendering defects.
- Experimental map select-screen WLD rendering work is not part of this module.

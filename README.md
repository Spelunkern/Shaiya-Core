# Shaiya Core

Shaiya Core is a C++ patching and extension project for Shaiya private-server research, preservation, and development.

The repository contains server-side modules for `ps_game.exe`, client-side modules for `Game.exe`, shared packet/structure definitions, and small login/database-agent helpers. The goal is to keep binary patches practical, auditable, and easy to disable while documenting why each feature exists.

## Repository Layout

- `sdev/` - game-service/server hooks and gameplay fixes.
- `sdev-client/` - `Game.exe` hooks, UI support, Unicode/chat fixes, and client quality-of-life patches.
- `sdev-login/` - login-service hooks.
- `sdev-db/` - database-agent hooks.
- `shaiya/` - shared game structures, enums, and packet definitions.
- `util/` - common memory patching helpers.
- `external/` - vendored third-party code used by client features.
- `mini-gmp/` - vendored dependency used by the project.

## Build

This project targets Windows, Visual Studio 2022/MSBuild, and x86 builds.

Server module:

```powershell
& 'C:\BuildTools\MSBuild\Current\Bin\MSBuild.exe' .\Shaiya-Core.sln /t:sdev /p:Configuration=Release /p:Platform=x86 /m
```

Client module:

```powershell
$env:DXSDK_DIR='C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\'
& 'C:\BuildTools\MSBuild\Current\Bin\MSBuild.exe' .\Shaiya-Core.sln /t:sdev-client /p:Configuration=Release /p:Platform=x86 /m
```

Build outputs are ignored by git. Public releases should be built from a clean checkout and tested against the matching binaries.

## Configuration

Runtime behavior can depend on external files such as `CONFIG.INI`, PNG interface assets, and feature-specific INI files. Keep production secrets and server-specific data out of commits. Prefer documenting example values instead of committing live configuration.

## Feature Documentation

- Client features are documented in `sdev-client/README.md`.
- Server features are documented in `sdev/README.md`.
- Shared packet/structure notes are documented in `shaiya/README.md`.

## Stability Notes

Most patches are intentionally small and grouped by feature in code comments. Experimental or broken work should stay disabled and be documented as future work before the repository is published.

Current known disabled future work:

- Custom deterministic recreation runes: marked in code as `Future feature - broken right now`.

## License

This project's own code is licensed under the BSD 3-Clause License. See `LICENSE`.

Third-party code keeps its original license terms. See `THIRD_PARTY_NOTICES.md`.

## Disclaimer

This project is provided as-is, without warranties. Test every patch in a controlled environment before using it in production.

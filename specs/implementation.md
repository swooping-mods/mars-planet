# Mars Planet: behavior and implementation

Describes the exported implementation **0.1.0**. The linked source and metadata define this version; gameplay qualification is a separate requirement.

## Behavior

A historical native experiment adds a distinct generated Mars destination selected before loading: Ctrl+Alt+M chooses Mars and Ctrl+Alt+N returns to Moon in the main menu. The source clones world settings, changes the seed/terrain/dust parameters and gravity to 3.71 while preserving the base radius/spawn layout. Direct selection is enabled by default; otherwise the observed Moon ending unlocks it. It is separate from the external MarsSweepers mod.

## State and lifecycle

Mars slots/backups live in a seed-specific namespace beside the native Moon save root, never inside its migration-scanned children. Menu selection and cloned settings are temporary; an unlock preference persists. The shared original settings asset and a running Moon world are not modified. Joined guests are refused because no destination handshake exists.

## Integration and configuration

No additional feature-mod runtime dependency is declared. Included shared headers are source reuse, not extra DLL dependencies.

The [module metadata](../source/mods/mars-planet/mod.json) owns the version and declared integration contract. Native modules start at `MoonLab_ModStart`. Game/Unity integrations target the pinned Steam Windows x64 IL2CPP build.

Packaged defaults in [settings.ini](../source/mods/mars-planet/settings.ini); supported bounds and interpretation remain in the linked parser.

| Section | Setting | Default |
|---|---|---|
| `[mars-planet]` | `seed` | `20261109` |
| `[mars-planet]` | `direct_menu_selection` | `1` |

## Implementation sources

- [mod.c](../source/mods/mars-planet/mod.c)
- [ui.h](../source/mods/mars-planet/ui.h)

## Verification boundary

Historical source/payload only: current game-build compatibility and loaded-world safety are unqualified. No Mars co-op handshake, save conversion or compatibility guarantee with MarsSweepers. Changing seed changes the destination namespace.

`python3 build.py --check` verifies the exported source hashes. It does not execute the mod or qualify the behaviors above. Installation and independent build prerequisites are in the [README](../README.md).

# Ring Warriors Vita — original prototype

An original text-based wrestling prototype for PlayStation Vita, using VitaSDK.
It is not a port of Wrestling MPire or any other commercial game.

## Controls
- LEFT / RIGHT: choose wrestler
- CROSS: start match / strike
- SQUARE: grapple
- TRIANGLE: recover stamina
- CIRCLE: pin attempt when opponent is weakened
- START: quit or leave match

## Build
Push these files to the `main` branch of your private GitHub repository.
The GitHub Actions workflow uses the VitaSDK container and uploads the VPK as
the `ring-warriors-vita` artifact after a successful build.

This is an early gameplay prototype, not a finished 3D game. It does not yet
include 3D graphics, animations, career mode, or a large simulation world.

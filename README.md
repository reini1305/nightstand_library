# Nightstand Library
Easy to use library to implement a nightstand mode in pebble watchfaces

Usage boils down to:
- `include "nightstand.h"` in your source file
- call `nightstand_window_init()` in your init code
- call `nightstand_window_deinit()` in your deinit code
- call `nightstand_window_update()` in your update code (e.g. once per minute). If it returns `true` the nightstand mode is active and you don't have to draw because your window will be hidden by the nightstand window.

## Credits
credits go to Yuriy Galanter for the Effect Layer library, that I included here in a stripped-down version.
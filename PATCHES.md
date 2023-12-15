# Patches & their purpose

## Fixes
- [x] normalize-whitespace -- make sure all tabs & eol spaces are normal
- [x] dwm-preserve-on-restart -- makes sure windows stay in place across dwm restarts
- [x] dwm-fix-multi-mon -- focus on status bar when moving mouse to empty monitor
- [x] dwm-ewmhtags -- add desktop information like current_desktop/names/viewport/...
- [x] dwm-refresh-rate
- [x] dwm-windowmap - map() and unmap() windows on creation/removal (for compositor?)

## Hotkeys
- [x] dwm-actual-fullscreen - allow toggling fullscreen mode
- [ ] dwm-keycodes - ? use keycodes instead of keysyms for shortcuts (maybe?)

## Navigation
- [x] dwm-access-nth-monitor - user altmod to jump directly to other monitors
- [x] dwm-focus-adjacent-tag - use mod keys to cross to left/right tags

## Clients
- [x] dwm-pertag-with-sel - change layouts for each tag individually
- [x] dwm-cfacts - allow to resize each client individually (not just mfactor)
- [x] dwm-functional-gaps - control the size of window gaps
- [x] dwm-set-border-px
- [x] dwm-center - center, but only very specific windows

## Status bar
- [x] dwm-staticstatus - keep main statusbar on a static monitor
- [x] dwm-barpadding  - status bar gaps
- [x] dwm-statuspadding - inner padding of the status bar
- [ ] ?? dwm-bar-height-spacing ... probably better to go with font-sized sizing
- [x] dwm-multimon-monitor-marker
- [x] dwm-centeredwindowname - makes the name move to the center of the bar

## Tiling
- [x] dwm-attach-aside-and-below - creates new windows as aside instead of master


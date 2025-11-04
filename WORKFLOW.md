# DWM Development Workflow

This document describes the complete workflow for making changes to DWM with quilt patches and updating the Nix packages.

## Prerequisites

- `quilt` for patch management
- `nix-shell` with build dependencies
- Access to push to `git@github.com:kesor/dwm.git`

## Making Changes to DWM

### 1. Apply All Patches
```bash
cd ~/src/suckless/dwm
quilt push -a
```
This applies all patches and gets you to the `kesor-personalization.diff` patch where you make changes.

### 2. Make Your Changes
Edit `config.def.h` or other source files as needed.

**For adding keybindings:**
1. Add command definition in the commands section:
   ```c
   static const char *newcmd[] = { "command", "args", NULL };
   ```
2. Add keybinding in the `keys[]` array:
   ```c
   { MODKEY, XK_key, spawn, {.v = newcmd } },
   ```

### 3. Test the Build
```bash
nix-shell -p gcc xorg.libX11 xorg.libXft xorg.libXinerama freetype fontconfig --run "make clean && make"
```

### 4. Update the Patch
```bash
quilt refresh
```
This updates `kesor-personalization.diff` with your changes.

### 5. Clean Up
```bash
quilt pop -a
```
This removes all patches and returns to clean state.

## Committing Changes

### 1. Check Git Status
```bash
git status
```
Should show `patches/kesor-personalization.diff` as modified.

### 2. Commit the Patch
```bash
git add patches/kesor-personalization.diff
git commit -m "Descriptive commit message about the change"
```

### 3. Push to Remote
```bash
git push kesor master-patched
```

### 4. Get New Commit Hash
```bash
git rev-parse HEAD
```

## Updating Nix Packages

### 1. Get New Hash
```bash
nix-prefetch-url --unpack https://github.com/kesor/dwm/archive/NEW_COMMIT_HASH.tar.gz
```

### 2. Update Package Files
Update both files with the new commit hash and sha256:

**Home Manager:** `~/src/dotfiles/config-nix-hm/modules/gui/wm/dwm/default.nix`
**NixOS:** `~/src/dotfiles/config-nix-hm/nixos/modules/desktop/dwm/package.nix`

Change:
```nix
rev = "NEW_COMMIT_HASH";
sha256 = "sha256-NEW_HASH_FROM_NIX_PREFETCH=";
```

### 3. Commit Package Updates
```bash
cd ~/src/dotfiles/config-nix-hm
git add modules/gui/wm/dwm/default.nix nixos/modules/desktop/dwm/package.nix
git commit -m "Update dwm packages to NEW_COMMIT_HASH

- Description of changes made"
```

## DMENU Workflow

The same workflow applies to dmenu:
- **Repository:** `~/src/suckless/dmenu/`
- **Remote:** `git@github.com:kesor/dmenu.git`
- **Branch:** `master-patched`
- **Package files:**
  - Home Manager: `~/src/dotfiles/config-nix-hm/modules/gui/wm/dwm/default.nix` (patched-dmenu)
  - NixOS: Similar pattern if exists

## Quick Reference Commands

```bash
# Start workflow
cd ~/src/suckless/dwm && quilt push -a

# Test build
nix-shell -p gcc xorg.libX11 xorg.libXft xorg.libXinerama freetype fontconfig --run "make clean && make"

# Save changes
quilt refresh && quilt pop -a

# Commit and push
git add patches/kesor-personalization.diff
git commit -m "Description"
git push kesor master-patched

# Get commit hash and prefetch
git rev-parse HEAD
nix-prefetch-url --unpack https://github.com/kesor/dwm/archive/COMMIT_HASH.tar.gz
```

## Notes

- Always test build before committing
- Use descriptive commit messages
- Update both Home Manager and NixOS packages
- The workflow is identical for dmenu
- Keep this document updated when the workflow changes

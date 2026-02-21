# Quick Start - Building straDellaMIDI

## The Simplest Way to Build

After pulling the latest changes:

```bash
# 1. Clean old builds
./clean.sh

# 2. Build everything
./build.sh
```

That's it! The script handles the VST3 helper build order issue automatically.

## What Gets Built

- **AU Plugin**: `Builds/MacOSX/build/Debug/straDellaMIDI.component`
- **VST3 Plugin**: `Builds/MacOSX/build/Debug/straDellaMIDI.vst3`

Both should be **50-100MB** (not 8KB!).

## Installing the Plugins

After building:

```bash
# Install AU (for Logic Pro, GarageBand, etc.)
cp -r Builds/MacOSX/build/Debug/straDellaMIDI.component ~/Library/Audio/Plug-Ins/Components/

# Install VST3 (for other DAWs)
cp -r Builds/MacOSX/build/Debug/straDellaMIDI.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

## For Release Builds

```bash
./clean.sh
./build.sh Release
```

Then install from `Builds/MacOSX/build/Release/` instead of `Debug/`.

## If You Changed the Code

If you've made code changes:

1. Regenerate build files:
   ```bash
   Projucer straDellaMIDI.jucer
   # Save (Cmd+S)
   ```

2. Clean and rebuild:
   ```bash
   ./clean.sh
   ./build.sh
   ```

## Common Issues

### "juce_vst3_helper: No such file"
**Solution:** Use `./build.sh` instead of building in Xcode. The script builds targets in the correct order.

### ".component is only 8KB"
**Solutions:**
1. Verify global JUCE path in Projucer (File → Global Paths)
2. Regenerate: Open `.jucer` in Projucer and save
3. Clean and rebuild: `./clean.sh && ./build.sh`

### "Module not found" errors
**Solution:** 
1. Set global JUCE path in Projucer to `/Users/[yourname]/JUCE`
2. Regenerate the project

## Using Xcode GUI

If you prefer Xcode:

1. Open `Builds/MacOSX/straDellaMIDI.xcodeproj`
2. Build in this order:
   - Select "juce_vst3_helper" → Build
   - Select "straDellaMIDI - AU" → Build
   - Select "straDellaMIDI - VST3" → Build

**Don't** use the "All" scheme - it will fail.

## Testing the Plugin

### In Logic Pro (AU)
1. Install with the command above
2. Restart Logic Pro
3. Create a software instrument track
4. Load "straDellaMIDI" from the instrument menu

### In Other DAWs (VST3)
1. Install with the command above
2. Restart your DAW
3. Scan for new plugins
4. Load "straDellaMIDI"

## Project Structure

```
straDellaMIDI/
├── Source/                      # Plugin source code
├── straDellaMIDI.jucer         # JUCE project file
├── build.sh                     # Build script (use this!)
├── clean.sh                     # Clean script
├── Builds/MacOSX/              # Generated Xcode project
└── Documentation files (.md)
```

## Need Help?

See detailed documentation:
- **VST3_SIMPLE_SOLUTION.md** - VST3 helper issue fix
- **CONFIGURATION_RESTORED.md** - About the current config
- **FIX_8KB_COMPONENT_LINKING.md** - If component is still 8KB

## Summary

✅ Use `./build.sh` to build correctly
✅ Use `./clean.sh` before rebuilding
✅ Plugins should be 50-100MB, not 8KB
✅ Install to `~/Library/Audio/Plug-Ins/`

# Troubleshooting 8KB Component File After Regenerating Build Files

## Symptoms

You've done everything right:
- ✅ Regenerated build files with Projucer
- ✅ `verify_build_config.sh` passes all checks
- ✅ Info-AU.plist and Info-VST3.plist exist
- ✅ Built the project in Xcode

**BUT** you still get an 8KB .component file instead of 50-100MB!

## Most Common Cause: Building the Wrong Target

When Projucer generates build files for a JUCE audio plugin project, it creates **multiple targets** in Xcode:

- ✅ **straDellaMIDI - AU** (Audio Unit plugin) ← BUILD THIS
- ✅ **straDellaMIDI - VST3** (VST3 plugin) ← BUILD THIS  
- ✅ **straDellaMIDI - All** (builds both plugins) ← OR BUILD THIS
- ℹ️ **straDellaMIDI - Shared Code** (library used by both plugins) ← AUTO-BUILT, don't build directly
- ❌ **straDellaMIDI - Standalone** (standalone app, if enabled) ← NOT THIS

**Note about Shared Code**: This target is NORMAL and should NOT be removed. It compiles the common code (PluginProcessor, PluginEditor, etc.) into a static library that both AU and VST3 targets link against. This is a build optimization. You should never build this target directly - it will automatically be built when you build the plugin targets.

### The Problem

If you just click "Build" or "Run" in Xcode without selecting the correct scheme/target, Xcode might build:
- The default target (often the first one alphabetically)
- A standalone app target (if present)
- **Just the shared code library** (which creates a .a file but no plugin bundle) ← **This causes the 8KB issue!**

This results in the 8KB component file issue!

## Solution 1: Build the Correct Target in Xcode GUI

### Step 1: Open the Project
```bash
cd Builds/MacOSX
open straDellaMIDI.xcodeproj
```

### Step 2: Select the Correct Scheme

In Xcode:
1. Look at the **top left corner** next to the Stop button
2. You'll see the current scheme (e.g., "straDellaMIDI > My Mac")
3. **Click on the scheme selector**
4. Choose **"straDellaMIDI - AU"** or **"straDellaMIDI - VST3"** or **"straDellaMIDI - All"**

![Xcode Scheme Selector](https://developer.apple.com/library/archive/documentation/ToolsLanguages/Conceptual/Xcode_Overview/Art/2x/scheme_selector_2x.png)

### Step 3: Build

Press **Cmd+B** or **Product → Build**

### Step 4: Verify Output

Check the build output location (usually `Builds/MacOSX/build/Release/` or `~/Library/Developer/Xcode/DerivedData/`):

```bash
# Check the size
ls -lh build/Release/straDellaMIDI.component
ls -lh build/Release/straDellaMIDI.vst3

# Should show 50-100MB, NOT 8KB!
```

## Solution 2: Build from Command Line (Recommended)

This ensures you're building the correct target:

```bash
cd Builds/MacOSX

# Build AU plugin
xcodebuild -project straDellaMIDI.xcodeproj \
           -scheme "straDellaMIDI - AU" \
           -configuration Release

# Build VST3 plugin  
xcodebuild -project straDellaMIDI.xcodeproj \
           -scheme "straDellaMIDI - VST3" \
           -configuration Release

# Or build both at once
xcodebuild -project straDellaMIDI.xcodeproj \
           -scheme "straDellaMIDI - All" \
           -configuration Release
```

## How to Check What Targets Exist

Run the verification script, which now shows all targets:

```bash
./verify_build_config.sh
```

Or manually:

```bash
cd Builds/MacOSX
xcodebuild -project straDellaMIDI.xcodeproj -list
```

You should see output like:
```
Targets:
    straDellaMIDI - AU
    straDellaMIDI - VST3
    straDellaMIDI - All
    straDellaMIDI - Shared Code
```

**Note**: The "Shared Code" target is normal and necessary - it's a static library that both AU and VST3 plugins link against. Don't remove it, and don't build it directly. Always build one of the plugin targets (AU, VST3, or All).

## Other Possible Issues

### Issue 1: Building for Wrong Architecture

**Symptom**: Build succeeds, file size is correct, but plugin doesn't load in your DAW.

**Solution**: Make sure you're building for the correct architecture:
- Apple Silicon Mac (M1/M2): Build for ARM64
- Intel Mac: Build for x86_64
- Universal: Build for both

Check your DAW's architecture and build accordingly.

### Issue 2: Standalone App Target Still Exists

**Symptom**: You see "straDellaMIDI - Standalone" or "straDellaMIDI - App" in the target list.

**Cause**: Main.cpp and MainComponent.cpp files in the Source directory might have been picked up by Projucer.

**Solution**: 
1. In Projucer, make sure the project type is "Audio Plug-In"
2. Make sure "Standalone Plugin" is NOT checked in plugin formats
3. Resave the project
4. Make sure you're NOT building the Standalone target

### Issue 3: Old Build Artifacts

**Symptom**: You rebuild but still get the old 8KB file.

**Solution**: Clean the build directory:

```bash
# In Xcode: Product → Clean Build Folder (Shift+Cmd+K)

# Or from command line:
cd Builds/MacOSX
rm -rf build/
rm -rf ~/Library/Developer/Xcode/DerivedData/straDellaMIDI-*
```

Then rebuild with the correct target/scheme.

### Issue 4: Building Only the Shared Code Target

**Symptom**: Build succeeds, you see a large .a file (e.g., libstraDellaMIDI.a ~177MB), but .component file is only 8KB.

**Cause**: You're building just the "Shared Code" target instead of the plugin targets.

**What is Shared Code?** When you have multiple plugin formats (AU, VST3), JUCE creates a "Shared Code" target that compiles all common code (PluginProcessor, PluginEditor, etc.) into a static library. Both AU and VST3 targets then link against this library to avoid recompiling the same code twice.

**Solution**: 
- **Don't build the Shared Code target directly**
- Build "straDellaMIDI - AU", "straDellaMIDI - VST3", or "straDellaMIDI - All" instead
- These targets will automatically build the Shared Code as a dependency
- The Shared Code target is NORMAL and should NOT be removed from the project

### Issue 5: Code Signing Issues

**Symptom**: Build succeeds, but file is small or plugin crashes.

**Solution**: 
1. In Xcode, go to the target settings
2. Under "Signing & Capabilities", make sure code signing is set up
3. Use "Sign to Run Locally" for development

## Verification Checklist

After building, verify:

```bash
# 1. Check file size (should be 50-100MB each)
ls -lh Builds/MacOSX/build/Release/straDellaMIDI.component
ls -lh Builds/MacOSX/build/Release/straDellaMIDI.vst3

# 2. Check it's a bundle, not just a file
file Builds/MacOSX/build/Release/straDellaMIDI.component
# Should say: "Mach-O bundle" or similar

# 3. Check the binary inside has code
ls -lh Builds/MacOSX/build/Release/straDellaMIDI.component/Contents/MacOS/straDellaMIDI
# This file should also be 50-100MB

# 4. Check with otool (optional)
otool -L Builds/MacOSX/build/Release/straDellaMIDI.component/Contents/MacOS/straDellaMIDI
# Should show linked frameworks
```

## Still Not Working?

If you've tried everything above and still get 8KB files:

1. **Delete and regenerate build files completely**:
   ```bash
   rm -rf Builds/MacOSX/straDellaMIDI.xcodeproj
   rm -rf Builds/MacOSX/*.plist
   # Then regenerate in Projucer
   ```

2. **Check Projucer configuration**:
   - Open straDellaMIDI.jucer in Projucer
   - Verify Project Type is "Audio Plug-In" 
   - Verify Plugin Formats shows "AU, VST3" (NOT Standalone)
   - Save project again

3. **Check for file conflicts**:
   ```bash
   # Make sure these files exist and are for plugins:
   cat Source/PluginProcessor.h | head -20
   cat Source/PluginEditor.h | head -20
   ```

4. **Try a clean clone**:
   ```bash
   cd ..
   git clone <repo-url> straDellaMIDI-fresh
   cd straDellaMIDI-fresh
   # Open in Projucer, set JUCE path, save, build
   ```

## Summary

**The #1 cause of 8KB component files after regenerating build files is building the wrong target.**

✅ **DO**: Build "straDellaMIDI - AU", "straDellaMIDI - VST3", or "straDellaMIDI - All"

❌ **DON'T**: Build "straDellaMIDI - Standalone", "straDellaMIDI - App", "straDellaMIDI - Shared Code", or just "straDellaMIDI"

Use `./verify_build_config.sh` to see what targets are available, and always specify the scheme when building.

## FAQ

**Q: Should I remove the "Shared Code" target from my project?**

A: **NO!** The Shared Code target is a normal and necessary part of JUCE multi-format plugin builds. It compiles common code once into a static library that both AU and VST3 plugins link against. This is a build optimization. You should never:
- Remove this target
- Build this target directly
- Modify this target

Just ignore it and build the plugin targets (AU, VST3, All) instead.

**Q: Why do I have a 177MB .a file?**

A: That's the static library created by the Shared Code target. It contains all your compiled code. If your .component file is only 8KB, it means you built ONLY the Shared Code target instead of the plugin targets. Build "straDellaMIDI - AU" or "straDellaMIDI - VST3" instead, which will link this library into the final plugin bundle.

**Q: What's the difference between "straDellaMIDI - All" and building each plugin separately?**

A: "All" builds both AU and VST3 plugins in one command. Building them separately gives you the same result, just one at a time. Use "All" for convenience.

**Q: Can I build a standalone version of this plugin?**

A: The project is configured as plugin-only. To build a standalone app, you would need to change the project type in Projucer from "Audio Plug-In" to "GUI Application" or enable standalone plugin format, but this is not recommended unless you need it for a specific reason.

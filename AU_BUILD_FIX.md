# AU Build Error Fix: PhaseScriptExecution Failed

## Problem

When building the AU (Audio Unit) format, you may encounter the error:
```
Command PhaseScriptExecution failed with a nonzero exit code
```

## Root Cause

This error occurs because the Xcode build files in `Builds/MacOSX/` need to be regenerated with Projucer to properly configure the AU plugin build process. The current build files may be outdated or missing proper AU-specific build phases and configurations.

## Solution

### Step 1: Regenerate Build Files with Projucer

**This is REQUIRED before building the AU plugin!**

1. **Install JUCE and Projucer** (if not already installed)
   - Download from [juce.com](https://juce.com/)
   - Install the Projucer application

2. **Open the project in Projucer**
   ```bash
   # Option A: Double-click the file (if Projucer is default handler)
   open straDellaMIDI.jucer
   
   # Option B: Launch Projucer, then open the file
   /Applications/JUCE/Projucer.app/Contents/MacOS/Projucer straDellaMIDI.jucer
   ```

3. **Configure Global Paths**
   - In Projucer menu: **File → Global Paths**
   - Set "Path to JUCE" to your JUCE installation directory
   - Typically: `~/JUCE` or `/Applications/JUCE/modules`

4. **Verify Plugin Configuration**
   - In Projucer, check that the project settings show:
     - **Project Type**: Audio Plug-In
     - **Plugin Formats**: `buildAU, buildVST3` (both checked)
     - **Plugin Manufacturer**: PapaCoyote
     - **Plugin Code**: Strd
     - **Plugin AU Main Type**: 'aumi' (MIDI effect)

5. **Save/Export Project**
   - Click **"Save Project"** button or press `Cmd+S` (macOS) / `Ctrl+S` (Windows)
   - This regenerates ALL build files in `Builds/MacOSX/`
   - You should see new files created:
     - `Info-AU.plist` (AU plugin configuration)
     - `Info-VST3.plist` (VST3 plugin configuration)
     - Updated `straDellaMIDI.xcodeproj`

### Step 2: Build the Plugin

After regenerating the build files:

```bash
# Open in Xcode
cd Builds/MacOSX
open straDellaMIDI.xcodeproj

# Select the target:
# - "straDellaMIDI - AU" for Audio Unit plugin
# - "straDellaMIDI - VST3" for VST3 plugin
# - "straDellaMIDI - All" to build both

# Build (Cmd+B in Xcode, or from command line):
xcodebuild -project straDellaMIDI.xcodeproj -configuration Release -target "straDellaMIDI - AU"
```

### Step 3: Verify Output

The AU plugin will be built to:
```
Builds/MacOSX/build/Release/straDellaMIDI.component
```

Install it to your system:
```bash
cp -r Builds/MacOSX/build/Release/straDellaMIDI.component \
      ~/Library/Audio/Plug-Ins/Components/
```

## Why This Happens

The `PhaseScriptExecution` error during AU builds typically occurs when:

1. **Outdated build files**: The Xcode project was generated with an older configuration
2. **Missing AU validation scripts**: Projucer adds specific build phases for AU validation
3. **Incorrect bundle structure**: AU plugins require specific Info.plist entries and bundle structure
4. **Code signing issues**: AU plugins need proper code signing configuration

Projucer automatically handles all of these when it regenerates the build files.

## Alternative: Command Line Regeneration

If you prefer command line:

```bash
# Navigate to Projucer
cd /Applications/JUCE/Projucer.app/Contents/MacOS/

# Regenerate build files
./Projucer --resave /path/to/straDellaMIDI/straDellaMIDI.jucer

# Now build
cd /path/to/straDellaMIDI/Builds/MacOSX
xcodebuild -project straDellaMIDI.xcodeproj -configuration Release
```

## Troubleshooting

### Error: "Invalid JUCE modules path"
**Solution**: Set the correct path in Projucer → File → Global Paths

### Error: "auval not found"
**Solution**: Install Xcode Command Line Tools:
```bash
xcode-select --install
```

### Error: Code signing failed
**Solution**: In Xcode, go to project settings and either:
- Set "Code Signing Identity" to "Sign to Run Locally"
- Or configure with your Apple Developer certificate

### Still getting PhaseScriptExecution error?
1. Clean build folder: `xcodebuild clean` or Xcode → Product → Clean Build Folder
2. Delete derived data: `rm -rf ~/Library/Developer/Xcode/DerivedData/straDellaMIDI-*`
3. Regenerate build files again with Projucer
4. Try building VST3 format first to verify code compiles

## Related Documentation

- [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md) - Detailed regeneration guide
- [VST3_BUILD_GUIDE.md](VST3_BUILD_GUIDE.md) - Complete build instructions
- [README.md](README.md) - Project overview

## Code Fixes Applied

The following code issues have been fixed in this PR:

1. **Unused variable** in `StradellaKeyboardMapper.cpp:24`
   - Removed unused `const int fKeyNote = 24;` declaration

2. **Font constructor** in `MouseMidiSettingsWindow.cpp:20`
   - Uses standard JUCE Font constructor: `juce::Font(18.0f, juce::Font::bold)`
   - This is the correct and widely-used JUCE API for creating styled fonts
   - Consistent with other font usage in the codebase (e.g., MIDIMessageDisplay.cpp)

These fixes eliminate the compilation warnings. The AU build error requires the build file regeneration steps above.

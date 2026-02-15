# Building straDellaMIDI as an AU/VST3 Plugin

This guide explains how to build the straDellaMIDI application as an **AU (Audio Unit) and VST3 plugin** compatible with Logic Pro and other DAWs.

## ⚠️ CRITICAL: Build Files Must Be Regenerated!

**If you've just cloned or pulled this repository**, the build files in `Builds/` are outdated and still configured for the old GUI application. You **MUST regenerate them using Projucer** before building, or you'll get a standalone .app instead of plugins!

See [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md) for detailed instructions.

## Prerequisites

1. **JUCE Framework** (v6.0 or later recommended)
   - Download from [juce.com](https://juce.com/)
   - Install Projucer (JUCE's project generator)

2. **Development Tools**
   - **macOS**: Xcode 12+ with Command Line Tools
   - **Windows**: Visual Studio 2019+ with C++ support
   - **Linux**: GCC/Clang and appropriate build tools

## Building the Plugin

### Step 1: Open the Project in Projucer ⚠️ REQUIRED

1. Launch Projucer
2. Open `straDellaMIDI.jucer` from the project root directory
3. In Projucer, ensure your JUCE modules path is correctly set:
   - Go to **File → Global Paths**
   - Set the path to your JUCE modules directory

### Step 2: Configure Build Settings

The project is already configured to build as an AU and VST3 plugin with the following settings:

- **Project Type**: Audio Plug-In
- **Plugin Formats**: AU (Audio Unit), VST3
- **Plugin Characteristics**:
  - MIDI Effect Plugin: Yes
  - Wants MIDI Input: Yes
  - Produces MIDI Output: Yes
  - Editor Requires Keyboard Focus: Yes

### Step 3: Generate IDE Project Files ⚠️ CRITICAL STEP

**This step regenerates the build files and is REQUIRED for plugin building!**

1. In Projucer, select your target platform:
   - **macOS**: Xcode
   - **Windows**: Visual Studio 2019/2022
   - **Linux**: Linux Makefile

2. Click **"Save Project"** or press `Cmd+S` (Mac) / `Ctrl+S` (Win)
   - This regenerates ALL build files in the `Builds/` directory
   - Old GUI app configuration is replaced with plugin configuration
   - Creates proper Info-AU.plist and Info-VST3.plist files

### Step 4: Build the Plugin

#### macOS (Xcode)
```bash
# Navigate to the build directory
cd Builds/MacOSX

# Open in Xcode
open straDellaMIDI.xcodeproj

# Or build from command line (builds both AU and VST3)
xcodebuild -project straDellaMIDI.xcodeproj -configuration Release
```

The plugins will be built to:
```
Builds/MacOSX/build/Release/
├── straDellaMIDI.component  (AU plugin - best for Logic Pro)
└── straDellaMIDI.vst3       (VST3 plugin)
```

**Note**: If you only see a `.app` file, you forgot to regenerate the build files in Projucer!

#### Windows (Visual Studio)
```cmd
# Navigate to the build directory
cd Builds\VisualStudio2022

# Open the solution
start straDellaMIDI.sln

# Or build from command line
msbuild straDellaMIDI.sln /p:Configuration=Release
```

The VST3 plugin will be built to:
```
Builds\VisualStudio2022\x64\Release\VST3\straDellaMIDI.vst3
```

**Note**: Audio Unit (AU) format is only available on macOS.

#### Linux (Makefile)
```bash
# Navigate to the build directory
cd Builds/LinuxMakefile

# Build the plugin
make CONFIG=Release

# Or for debug build
make CONFIG=Debug
```

The VST3 plugin will be built to:
```
Builds/LinuxMakefile/build/straDellaMIDI.vst3
```

## Installing the Plugin

### macOS

Copy both plugin formats to their respective locations:

**Audio Unit (AU) - Recommended for Logic Pro:**
```bash
# Copy AU plugin (recommended for Logic Pro)
cp -r Builds/MacOSX/build/Release/straDellaMIDI.component \
     ~/Library/Audio/Plug-Ins/Components/

# Verify installation
ls -la ~/Library/Audio/Plug-Ins/Components/straDellaMIDI.component
```

**VST3:**
```bash
# Copy VST3 plugin (for other DAWs)
cp -r Builds/MacOSX/build/Release/straDellaMIDI.vst3 \
     ~/Library/Audio/Plug-Ins/VST3/

# System location (requires admin)
# sudo cp -r ... /Library/Audio/Plug-Ins/VST3/
```

**Why AU for Logic Pro?**
- AU is Apple's native format with better macOS/Logic Pro integration
- Direct support for all Logic Pro features
- Faster plugin scanning and loading
- Better stability and compatibility

### Windows
Copy the `.vst3` folder to:
```
C:\Program Files\Common Files\VST3\straDellaMIDI.vst3
```

**Note**: Audio Unit format is not available on Windows.

### Linux
Copy the `.vst3` folder to:
```bash
# User location
~/.vst3/straDellaMIDI.vst3

# System location
/usr/local/lib/vst3/straDellaMIDI.vst3
```

## Using the Plugin in Logic Pro

1. **Load the Plugin**
   - In Logic Pro, create a new Software Instrument track or MIDI track
   - In the MIDI FX slot, select **straDellaMIDI**

2. **Route MIDI Output**
   - The plugin produces MIDI output that can be routed to any instrument
   - Create a second instrument track to receive the MIDI from straDellaMIDI
   - In Logic Pro's Environment, route the MIDI output accordingly

3. **Keyboard Control**
   - Click on the plugin window to give it keyboard focus
   - Use your computer keyboard to play notes as described in the README
   - The plugin will generate MIDI output based on your key presses

4. **Expression Control**
   - Use mouse movement for expression control (bellows simulation)
   - Configure settings via the "Expression Settings" button

## Plugin Architecture

### MIDI Routing

Unlike the standalone application which sends MIDI directly to hardware/software ports, the plugin routes MIDI through the host DAW:

```
Keyboard Input → Plugin Editor → Plugin Processor → MIDI Buffer → DAW → Instruments
```

### Key Features in Plugin Mode

- **Full keyboard mapping** (Stradella accordion layout)
- **MIDI CC expression** via mouse movement
- **Low-latency MIDI generation**
- **Visual feedback** of pressed keys
- **MIDI message logging** for debugging
- **Configurable settings** for expression curves

## Troubleshooting

### Plugin Not Appearing in DAW

1. **Verify installation path**: Ensure the `.vst3` file is in the correct directory
2. **Rescan plugins**: In your DAW, trigger a plugin rescan
3. **Check plugin validation**: Some DAWs log plugin validation errors
4. **Architecture mismatch**: Ensure the plugin architecture (x64/ARM) matches your DAW

### Keyboard Input Not Working

1. **Click the plugin window** to give it keyboard focus
2. **Check keyboard focus setting**: The plugin requires keyboard focus to function
3. **DAW shortcuts**: Some keys might be captured by DAW shortcuts - consider disabling them

### MIDI Not Routing

1. **Check MIDI routing** in your DAW's mixer or environment
2. **Verify the plugin is in MIDI FX slot**, not an audio insert
3. **Enable MIDI output** for the track containing the plugin

### Build Errors

1. **JUCE modules path**: Verify the path in Projucer's Global Paths
2. **SDK versions**: Ensure you have the correct SDK version installed
3. **Clean build**: Try cleaning and rebuilding the project
4. **Check JUCE version**: This project requires JUCE 6.0 or later

## Development Notes

### Backwards Compatibility

The standalone application files (`Main.cpp`, `MainComponent.h`, `MainComponent.cpp`) are kept in the repository for users who prefer the standalone version. To build the standalone app:

1. In Projucer, change **Project Type** from "Audio Plug-In" to "GUI Application"
2. Remove `PluginProcessor.*` and `PluginEditor.*` from the file list
3. Add back `Main.cpp` to the compilation
4. Save and rebuild

### Plugin Configuration

Key plugin parameters in `straDellaMIDI.jucer`:
- `pluginCode`: "Strd" (4-character unique ID)
- `pluginManufacturerCode`: "Papa" (4-character manufacturer ID)
- `pluginIsMidiEffectPlugin`: true
- `pluginEditorRequiresKeys`: true (crucial for keyboard input)

## Further Information

- See [README.md](README.md) for keyboard mapping details
- See [ARCHITECTURE.md](ARCHITECTURE.md) for code structure
- See [TESTING_GUIDE.md](TESTING_GUIDE.md) for testing procedures

## Support

For issues or questions:
1. Check the [GitHub Issues](https://github.com/PapaCoyote/straDellaMIDI/issues)
2. Review JUCE documentation for plugin development
3. Consult your DAW's manual for plugin usage

## License

This project uses the JUCE framework. Ensure compliance with JUCE licensing requirements for plugin distribution.

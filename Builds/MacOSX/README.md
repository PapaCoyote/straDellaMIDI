# macOS Build Files - Regeneration Required

⚠️ **The Xcode project files for this directory must be generated using Projucer.**

## Why are the build files missing?

The build files are **generated from the `.jucer` source file** and should not be committed to version control. This ensures:
- ✅ Clean repository without platform-specific build artifacts
- ✅ Everyone builds with their own JUCE installation and settings
- ✅ No conflicts between different JUCE versions or paths
- ✅ Proper plugin targets (AU, VST3) are created correctly

## How to generate the build files

### Quick Start

1. **Install JUCE and Projucer** from [juce.com](https://juce.com/)

2. **Open the project file** in Projucer:
   ```bash
   Projucer ../../straDellaMIDI.jucer
   ```
   Or double-click `straDellaMIDI.jucer` in the project root

3. **Configure JUCE modules path**:
   - In Projucer: **File → Global Paths**
   - Set path to your JUCE modules directory (e.g., `~/JUCE/modules`)

4. **Save/Export the project**:
   - Click **"Save Project"** or press `Cmd+S`
   - This generates the Xcode project in this directory

5. **Build the plugin**:
   ```bash
   cd Builds/MacOSX
   xcodebuild -project straDellaMIDI.xcodeproj -configuration Release
   ```

### What gets generated?

After running Projucer, you'll see:
- `straDellaMIDI.xcodeproj/` - Xcode project with AU and VST3 targets
- `Info-AU.plist` - Audio Unit plugin configuration
- `Info-VST3.plist` - VST3 plugin configuration
- `RecentFilesMenuTemplate.nib` - GUI resources

### Expected output after building

```
Builds/MacOSX/build/Release/
├── straDellaMIDI.component  (AU plugin - 50-100MB, properly linked)
└── straDellaMIDI.vst3       (VST3 plugin)
```

**NOT** an 8KB .component file - that indicates incorrect build configuration!

## Troubleshooting

### "Invalid JUCE modules path" error
Set the correct path in Projucer → File → Global Paths

### Only getting a .app file instead of plugins?
The project is configured as an audio plugin. Make sure you're using the latest version of Projucer and that the `.jucer` file hasn't been corrupted.

### Build fails with missing headers?
Install Xcode Command Line Tools:
```bash
xcode-select --install
```

## More Information

- 📖 [REGENERATE_BUILD_FILES.md](../../REGENERATE_BUILD_FILES.md) - Detailed regeneration guide
- 📖 [VST3_BUILD_GUIDE.md](../../VST3_BUILD_GUIDE.md) - Complete build and installation instructions
- 📖 [FIX_8KB_COMPONENT_ISSUE.md](../../FIX_8KB_COMPONENT_ISSUE.md) - Troubleshooting 8KB .component file
- 📖 [AU_BUILD_FIX.md](../../AU_BUILD_FIX.md) - Audio Unit specific troubleshooting

## For Repository Maintainers

Build files are excluded from version control via `.gitignore`. The `.jucer` file is the single source of truth for build configuration.

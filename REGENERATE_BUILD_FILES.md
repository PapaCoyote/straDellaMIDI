# ⚠️ IMPORTANT: BUILD FILES MUST BE REGENERATED ⚠️

## The .jucer file has been updated, but build files need regeneration!

### Why is this needed?

The `straDellaMIDI.jucer` project file has been changed from a standalone GUI application to an **Audio Plugin (AU + VST3)**. However, the actual build files in the `Builds/` folder are **NOT automatically updated** when you change the `.jucer` file.

### Current State

❌ **Builds/MacOSX/** - Contains OLD GUI application Xcode project  
✅ **straDellaMIDI.jucer** - Updated to Audio Plugin configuration

### What happens if you build without regenerating?

If you open `Builds/MacOSX/straDellaMIDI.xcodeproj` directly and build, you will get:
- ❌ A standalone GUI application (.app)
- ❌ NOT a VST3 or AU plugin
- ❌ The old configuration

### How to Fix: Regenerate Build Files

#### Option 1: Using Projucer (Recommended)

1. **Install JUCE and Projucer**
   - Download from [juce.com](https://juce.com/)
   
2. **Open the .jucer file in Projucer**
   ```bash
   Projucer straDellaMIDI.jucer
   ```
   Or double-click `straDellaMIDI.jucer` if Projucer is your default app

3. **Configure Global Paths**
   - In Projucer: File → Global Paths
   - Set path to your JUCE modules directory

4. **Save/Export the Project**
   - Click "Save Project" or press Cmd+S (macOS) / Ctrl+S (Windows)
   - This regenerates all build files in `Builds/`

5. **Build the Plugin**
   - Now open `Builds/MacOSX/straDellaMIDI.xcodeproj` in Xcode
   - Build targets will include:
     - straDellaMIDI (AU plugin)
     - straDellaMIDI (VST3 plugin)
   
#### Option 2: Using Projucer Command Line

```bash
# Navigate to Projucer binary location
cd /Applications/JUCE/Projucer.app/Contents/MacOS/

# Regenerate build files
./Projucer --resave /path/to/straDellaMIDI/straDellaMIDI.jucer
```

### After Regeneration

Once you've regenerated the build files, you'll see:
- ✅ Info-VST3.plist (for VST3 plugin)
- ✅ Info-AU.plist (for Audio Unit plugin)
- ✅ Proper plugin build targets in Xcode
- ✅ Plugin bundle outputs (.vst3 and .component)

### Verification

After regenerating and building, check that you get:
```
Builds/MacOSX/build/Release/
├── straDellaMIDI.component    (AU plugin)
└── straDellaMIDI.vst3         (VST3 plugin)
```

NOT:
```
❌ straDellaMIDI.app  (standalone GUI app - old)
```

### Plugin Formats Enabled

The project is now configured to build:
- ✅ **AU (Audio Unit)** - Native macOS format, best for Logic Pro
- ✅ **VST3** - Cross-platform format

### Next Steps

1. Regenerate build files with Projucer (see above)
2. Build the plugin with Xcode
3. Install to:
   - AU: `~/Library/Audio/Plug-Ins/Components/straDellaMIDI.component`
   - VST3: `~/Library/Audio/Plug-Ins/VST3/straDellaMIDI.vst3`
4. Launch Logic Pro and scan for new plugins
5. Add straDellaMIDI as a MIDI FX plugin

### Troubleshooting

**Q: I built but still got a .app file?**  
A: You didn't regenerate the build files. Follow the steps above.

**Q: Projucer says "Invalid JUCE modules path"?**  
A: Go to File → Global Paths and set the correct path to your JUCE modules.

**Q: Build fails with missing AU headers?**  
A: Make sure you have Xcode with Audio Unit SDK installed.

### For More Information

See [VST3_BUILD_GUIDE.md](VST3_BUILD_GUIDE.md) for complete build instructions.

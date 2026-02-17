# IMPORTANT: Build Files Must Be Regenerated!

## ⚠️ If You're Seeing "file not found" Errors

The JUCER file has been updated with correct module paths, but **you must regenerate the build files** using Projucer before building.

## Quick Fix

1. **Open the project in Projucer:**
   ```bash
   Projucer straDellaMIDI.jucer
   ```

2. **Save the project** (Cmd+S or File → Save Project)
   - This regenerates all build files with the updated configuration

3. **Clean and rebuild** in Xcode

## Why This Is Needed

The files in `JuceLibraryCode/` and `Builds/MacOSX/` are **auto-generated** by Projucer. When you pull changes to the `.jucer` file, these generated files become stale and must be regenerated.

## Required Project Location

This project must be located **inside your JUCE installation directory**:

```
JUCE/
├── modules/          ← JUCE framework modules
└── straDellaMIDI/    ← This project
```

Example on macOS:
```
/Users/yourname/JUCE/
├── modules/
│   ├── juce_audio_utils/
│   ├── juce_core/
│   └── ...
└── straDellaMIDI/
    ├── straDellaMIDI.jucer
    ├── Builds/
    └── Source/
```

## Module Path Configuration

Module paths in the JUCER file are set to `../../../modules` which resolves from `Builds/MacOSX/` to the JUCE modules directory:

```
Builds/MacOSX/ → ../../../modules = JUCE/modules/
```

## Full Build Instructions

See the main [README.md](README.md) for complete build instructions.

## Troubleshooting

**Error: "juce_audio_utils/juce_audio_utils.mm file not found"**
- You haven't regenerated build files after pulling changes
- Solution: Open in Projucer and save

**Error: "Invalid JUCE modules path"** 
- Your project is not in the JUCE directory
- Solution: Move project to JUCE directory OR adjust module paths

**Projucer can't find modules:**
- Set global JUCE path: Projucer → File → Global Paths

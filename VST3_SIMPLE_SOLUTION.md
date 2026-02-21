# VST3 Helper Build Issue - SIMPLE SOLUTION

## The Problem

When building the VST3 plugin, you get this error:
```
juce_vst3_helper: No such file or directory
```

This happens because the VST3 plugin needs a helper program to be built first, but the build order isn't correct.

## The Simple Solution

**Use the provided build scripts instead of building in Xcode directly.**

### Step 1: Clean Old Build Artifacts

```bash
./clean.sh
```

### Step 2: Build Everything in Correct Order

```bash
./build.sh
```

That's it! The script will:
1. Build the VST3 helper first
2. Then build the AU plugin
3. Then build the VST3 plugin

### For Release Builds

```bash
./build.sh Release
```

## What the Script Does

The `build.sh` script builds targets in this specific order:
1. **juce_vst3_helper** - The utility program
2. **straDellaMIDI - AU** - The Audio Unit plugin
3. **straDellaMIDI - VST3** - The VST3 plugin

This avoids the race condition that causes the error.

## If You Must Use Xcode

If you prefer to use Xcode's GUI:

1. **First**, select scheme: **juce_vst3_helper**
   - Build it (Cmd+B)

2. **Then**, select scheme: **straDellaMIDI - VST3**
   - Build it (Cmd+B)

3. **Then**, select scheme: **straDellaMIDI - AU**
   - Build it (Cmd+B)

**DO NOT** use the "All" scheme - it will fail with the helper error.

## Verifying the Build

After running `./build.sh`, check:

```bash
ls -lh Builds/MacOSX/build/Debug/straDellaMIDI.component/Contents/MacOS/straDellaMIDI
ls -lh Builds/MacOSX/build/Debug/straDellaMIDI.vst3/Contents/MacOS/straDellaMIDI
```

Both files should be **50-100MB**, not 8KB!

## Installing the Plugins

The build script will show you the commands to install:

```bash
# Install AU
cp -r Builds/MacOSX/build/Debug/straDellaMIDI.component ~/Library/Audio/Plug-Ins/Components/

# Install VST3
cp -r Builds/MacOSX/build/Debug/straDellaMIDI.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

## Why This Works

The VST3 build process has a script phase that runs the `juce_vst3_helper` to generate metadata. If you build all targets at once, the VST3 target might try to run the helper before it's been built, causing the error.

By building the helper first, it's guaranteed to exist when the VST3 plugin needs it.

## Troubleshooting

**If the build script fails:**

1. Make sure you've regenerated the Xcode project:
   ```bash
   Projucer straDellaMIDI.jucer
   # Save (Cmd+S)
   ```

2. Make sure your global JUCE path is set in Projucer

3. Try cleaning first:
   ```bash
   ./clean.sh
   ./build.sh
   ```

**If you don't have the scripts:**

They should be in the repository root:
- `build.sh` - Builds in correct order
- `clean.sh` - Cleans build artifacts

If missing, pull the latest changes:
```bash
git pull
```

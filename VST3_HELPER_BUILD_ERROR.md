# VST3 Helper Build Error - Troubleshooting

## Error Message

```
/Users/evanstuckless/JUCE/straDellaMIDI/Builds/MacOSX/build/Debug/juce_vst3_helper: No such file or directory
Command PhaseScriptExecution failed with a nonzero exit code
```

This error occurs during the "Plugin copy step" when building the VST3 target.

## What is juce_vst3_helper?

The `juce_vst3_helper` is a small utility program that JUCE uses to generate VST3 metadata (the `moduleinfo.json` file). It should be automatically built as part of the VST3 build process.

## Why This Happens

When Projucer generates the Xcode project, it creates:
1. A "juce_vst3_helper" target (small utility program)
2. A "straDellaMIDI - VST3" target (the actual plugin)
3. Build dependencies so helper is built before plugin

If the helper isn't being built or found, it's usually because:
- Build order is wrong (dependency not set)
- Helper target doesn't exist or is misconfigured
- Path to helper is incorrect in build script

## Solutions

### Solution 1: Build Targets in Correct Order (RECOMMENDED)

Instead of building "All" target, build targets individually in this order:

1. **First, build the helper:**
   ```bash
   cd Builds/MacOSX
   xcodebuild -scheme "juce_vst3_helper" -configuration Debug
   ```

2. **Then build AU plugin:**
   ```bash
   xcodebuild -scheme "straDellaMIDI - AU" -configuration Debug
   ```

3. **Then build VST3 plugin:**
   ```bash
   xcodebuild -scheme "straDellaMIDI - VST3" -configuration Debug
   ```

**In Xcode GUI:**
- Select "juce_vst3_helper" scheme from dropdown
- Build (Cmd+B)
- Then select "straDellaMIDI - VST3" scheme
- Build again

### Solution 2: Verify Xcode Project Has Helper Target

1. Open `Builds/MacOSX/straDellaMIDI.xcodeproj` in Xcode
2. Look at the scheme selector (top left)
3. Check if "juce_vst3_helper" appears in the list

**If helper target is missing:**
- Your Projucer version may have a bug
- Update to latest JUCE/Projucer from https://juce.com
- Regenerate the project (open .jucer, save)

### Solution 3: Check Build Dependencies

In Xcode:
1. Select the "straDellaMIDI - VST3" target
2. Go to "Build Phases" tab
3. Check "Target Dependencies"
4. Ensure "juce_vst3_helper" is listed

**If not listed:**
- Click "+"
- Add "juce_vst3_helper" to dependencies
- This ensures helper builds before VST3 plugin

**Note:** This change will be overwritten if you regenerate from Projucer.

### Solution 4: Clean and Rebuild

Sometimes build artifacts get corrupted:

```bash
cd Builds/MacOSX

# Clean all build artifacts
rm -rf build/
rm -rf ~/Library/Developer/Xcode/DerivedData/straDellaMIDI-*

# Rebuild
xcodebuild -scheme "juce_vst3_helper" -configuration Debug
xcodebuild -scheme "straDellaMIDI - VST3" -configuration Debug
```

### Solution 5: Update JUCE and Projucer

If using an older version of JUCE:
1. Download latest JUCE from https://juce.com/get-juce
2. Install newer version
3. Open straDellaMIDI.jucer in new Projucer
4. Save to regenerate with updated code
5. Build again

## Verification Steps

After trying solutions, verify:

```bash
# Check if helper was built
ls -la Builds/MacOSX/build/Debug/juce_vst3_helper
# Should show the executable (not "No such file")

# Check if it's executable
file Builds/MacOSX/build/Debug/juce_vst3_helper
# Should show: Mach-O executable

# Try running it
./Builds/MacOSX/build/Debug/juce_vst3_helper --help
# Should show usage information
```

## Why "All" Target May Fail

The "All" target tries to build everything in parallel, which can cause race conditions:
- VST3 plugin starts building
- Tries to run helper in a script phase
- But helper hasn't finished building yet
- Error occurs

**Workaround:** Build targets sequentially instead of using "All".

## JUCER File Configuration

The JUCER file is correctly configured:
```xml
<JUCERPROJECT ... pluginFormats="buildAU,buildVST3" ...>
```

This tells Projucer to:
- Create AU plugin target ✓
- Create VST3 plugin target ✓
- Create juce_vst3_helper target ✓

If helper target is missing after regeneration, it's likely a Projucer bug or JUCE version issue.

## If Nothing Works

Try building just the AU plugin (which doesn't need the helper):

```bash
xcodebuild -scheme "straDellaMIDI - AU" -configuration Debug
```

The AU plugin should build successfully and can be tested in Logic Pro or other AU hosts.

For VST3 specifically, you may need to:
1. Update JUCE to latest version
2. Report issue to JUCE forum if problem persists
3. Check JUCE GitHub issues for similar problems

## Related Files

- JUCER config: `straDellaMIDI.jucer`
- Generated project: `Builds/MacOSX/straDellaMIDI.xcodeproj`
- Helper should be at: `Builds/MacOSX/build/Debug/juce_vst3_helper`

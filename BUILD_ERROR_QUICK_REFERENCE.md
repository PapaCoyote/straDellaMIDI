# Build Error Quick Reference

This guide provides quick solutions to common build errors in the straDellaMIDI project.

## Error: "juce_vst3_helper: No such file or directory"

**Full Error**:
```
Command PhaseScriptExecution failed with a nonzero exit code
.../juce_vst3_helper: No such file or directory
```

**Quick Fix**: Regenerate build files with Projucer
```bash
# 1. Open in Projucer
Projucer straDellaMIDI.jucer

# 2. Set JUCE path in File → Global Paths

# 3. Save project (Cmd+S)

# 4. Build in Xcode
cd Builds/MacOSX
open straDellaMIDI.xcodeproj
```

**Details**: [VST3_HELPER_ERROR_FIX.md](VST3_HELPER_ERROR_FIX.md)

---

## Error: "Command PhaseScriptExecution failed" (AU Build)

**Quick Fix**: Same as above - regenerate with Projucer

**Details**: [AU_BUILD_FIX.md](AU_BUILD_FIX.md)

---

## Error: "Unused variable 'fKeyNote'"

**Status**: ✅ Fixed in this PR

**What was done**: Removed unused variable from StradellaKeyboardMapper.cpp

---

## Error: "No viable conversion from 'juce::Font::FontStyleFlags' to 'String'"

**Status**: ✅ Fixed in this PR

**What was done**: Changed to standard Font constructor:
```cpp
titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
```

---

## Why Regeneration is Needed

The `.jucer` file is configured for **Audio Plugins (AU + VST3)**, but the Xcode project in `Builds/MacOSX/` was generated for a **standalone application**.

### Current State
- ✅ `straDellaMIDI.jucer` - Plugin configuration
- ❌ `Builds/MacOSX/` - Standalone app configuration
- **Mismatch!** → Build errors

### After Regeneration
- ✅ `straDellaMIDI.jucer` - Plugin configuration  
- ✅ `Builds/MacOSX/` - Plugin configuration
- **Match!** → Successful builds

---

## What Regeneration Does

Projucer will regenerate the Xcode project with:

### Before (Current)
- 593 lines in project.pbxproj
- 2 VST3 references
- No VST3 helper target
- No AU build phases
- Standalone app only

### After (Expected)
- 2000+ lines in project.pbxproj
- 100+ VST3 references
- juce_vst3_helper target ✓
- Complete AU build configuration ✓
- Plugin targets (VST3 + AU) ✓

---

## One-Time Setup Required

**What**: Regenerate build files with Projucer
**When**: Once (takes ~30 seconds)
**Why**: Sync Xcode project with .jucer plugin configuration
**How**: See [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md)

After this one-time setup, normal Xcode builds will work.

---

## Summary of All Fixes

### Code Fixes (Already Done) ✅
1. Removed unused variable
2. Fixed Font constructor

### Build Configuration (User Action Required) ⚠️
3. Regenerate with Projucer (fixes both AU and VST3 errors)

---

## Quick Links

- **VST3 Helper Error**: [VST3_HELPER_ERROR_FIX.md](VST3_HELPER_ERROR_FIX.md)
- **AU Build Error**: [AU_BUILD_FIX.md](AU_BUILD_FIX.md)
- **Regeneration Guide**: [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md)
- **Complete Build Guide**: [VST3_BUILD_GUIDE.md](VST3_BUILD_GUIDE.md)
- **All Fixes Summary**: [BUILD_FIXES_COMPLETE.md](BUILD_FIXES_COMPLETE.md)

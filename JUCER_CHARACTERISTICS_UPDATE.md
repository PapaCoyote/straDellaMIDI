# JUCER Plugin Characteristics Update

## Changes Made

This document describes the changes made to the plugin characteristics in `straDellaMIDI.jucer`.

### Plugin Configuration Updates

**File Modified:** `straDellaMIDI.jucer` (line 7)

#### 1. VST3 Category Changed to Instrument
- **Previous:** `pluginIsSynth="0"` (Effect/Fx category)
- **Updated:** `pluginIsSynth="1"` (Instrument category)
- **Reason:** The plugin generates MIDI output like a synthesizer/instrument, not just processing audio

#### 2. MIDI Input Deselected
- **Previous:** `pluginWantsMidiIn="1"` (Accepts MIDI input)
- **Updated:** `pluginWantsMidiIn="0"` (Does not accept MIDI input)
- **Reason:** This plugin generates MIDI from keyboard/mouse input, it doesn't process incoming MIDI

#### 3. MIDI Output Confirmed Selected
- **Setting:** `pluginProducesMidiOut="1"` (Produces MIDI output)
- **Status:** Already correctly configured, no change needed
- **Reason:** The plugin's primary function is to generate MIDI output

## Plugin Type Summary

**Before:**
- Type: MIDI Effect Plugin
- Category: Fx (Effect)
- MIDI Input: ✓ Enabled
- MIDI Output: ✓ Enabled

**After:**
- Type: MIDI Effect Plugin (still marked as MIDI effect)
- Category: Instrument
- MIDI Input: ✗ Disabled
- MIDI Output: ✓ Enabled

## Impact

### VST3 Hosts
- The plugin will now appear in the **Instrument** category instead of **Fx/Effect** category
- This better reflects the plugin's purpose as a MIDI generator

### DAW Behavior
- The plugin will no longer accept MIDI input from external sources
- It will only generate MIDI output based on keyboard/mouse interaction
- This prevents confusion about the plugin's MIDI routing

### AU (Audio Unit) Configuration
- AU main type remains `'aumi'` (MIDI effect)
- The VST3 categorization doesn't affect AU classification

## Next Steps

After making these changes to the `.jucer` file:

1. **Regenerate build files with Projucer**
   ```bash
   Projucer straDellaMIDI.jucer
   # File → Save Project (Cmd+S)
   ```

2. **Rebuild the plugin**
   ```bash
   cd Builds/MacOSX
   xcodebuild -configuration Release
   ```

3. **Verify in DAW**
   - Check that the plugin appears under "Instrument" category (VST3)
   - Confirm MIDI input is not available
   - Confirm MIDI output works correctly

## Technical Notes

### JUCE Plugin Flags

- `pluginIsSynth`: Determines VST3 subcategory (0=Fx, 1=Instrument|Synth)
- `pluginWantsMidiIn`: Whether plugin accepts MIDI input
- `pluginProducesMidiOut`: Whether plugin produces MIDI output
- `pluginIsMidiEffectPlugin`: Marks plugin as MIDI effect type

### Compatibility

These changes maintain backward compatibility because:
- No audio processing logic changed
- No source code modifications required
- Only metadata/categorization updated

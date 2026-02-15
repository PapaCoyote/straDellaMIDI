# Conversion to VST3 Plugin - Technical Summary

This document describes the conversion of straDellaMIDI from a standalone GUI application to a VST3 plugin compatible with Logic Pro and other DAWs.

## Overview

The conversion transforms the application from a standalone MIDI generator to a plugin that integrates with DAW workflows, allowing users to:
- Load the plugin in any VST3-compatible DAW
- Use keyboard input within the DAW environment
- Route generated MIDI to any instrument in the DAW
- Integrate with DAW automation and routing

## Key Changes

### 1. Project Type Change

**File**: `straDellaMIDI.jucer`

Changed from standalone GUI application to audio plugin:

```xml
<!-- Before -->
<JUCERPROJECT projectType="guiapp" ...>

<!-- After -->
<JUCERPROJECT projectType="audioplug" 
              pluginFormats="buildVST3"
              pluginManufacturer="PapaCoyote" 
              pluginCode="Strd" 
              pluginManufacturerCode="Papa"
              pluginName="straDellaMIDI" 
              pluginDesc="Stradella MIDI Accordion Emulator"
              pluginIsSynth="0" 
              pluginWantsMidiIn="1" 
              pluginProducesMidiOut="1"
              pluginIsMidiEffectPlugin="1" 
              pluginEditorRequiresKeys="1">
```

**Important Plugin Flags**:
- `pluginIsMidiEffectPlugin="1"`: Identifies this as a MIDI effect/generator
- `pluginEditorRequiresKeys="1"`: Critical for keyboard input to work in the plugin
- `pluginProducesMidiOut="1"`: Indicates the plugin generates MIDI output

### 2. New Architecture

#### PluginProcessor (`PluginProcessor.h/cpp`)

Replaces the standalone application's direct MIDI output with host-managed MIDI routing:

```cpp
class StraDellaMIDIAudioProcessor : public juce::AudioProcessor
{
    // Manages:
    // - Audio/MIDI bus configuration
    // - MIDI message buffering from editor
    // - State save/load
    // - Plugin metadata
    
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);
    void addMidiMessageToBuffer(const juce::MidiMessage& message);
};
```

**Key Responsibilities**:
- Receives MIDI messages from the editor via `addMidiMessageToBuffer()`
- Buffers messages thread-safely using `juce::CriticalSection`
- Outputs buffered MIDI in `processBlock()` for the DAW to route

#### PluginEditor (`PluginEditor.h/cpp`)

Adapts the `MainComponent` logic to work as a plugin editor:

```cpp
class StraDellaMIDIAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           private juce::KeyListener
{
    // Maintains all UI components:
    // - KeyboardGUI
    // - MIDIMessageDisplay  
    // - MouseMidiExpression
    // - Settings buttons
    
    // Handles keyboard input and routes MIDI to processor
};
```

**Key Differences from MainComponent**:
- Extends `AudioProcessorEditor` instead of `Component`
- Sends MIDI to processor instead of directly to MIDI devices
- Accesses shared state through the processor reference

### 3. MIDI Routing Flow

#### Standalone Application
```
Keyboard Input → MainComponent → MidiOutput::sendMessageNow() → MIDI Device
```

#### VST3 Plugin
```
Keyboard Input → PluginEditor → PluginProcessor::addMidiMessageToBuffer()
                                         ↓
                                  processBlock()
                                         ↓
                                  MidiBuffer
                                         ↓
                                    DAW Host
                                         ↓
                                   Instruments
```

### 4. Thread Safety

The plugin architecture requires thread-safe MIDI buffering:

```cpp
// In PluginProcessor.cpp
void StraDellaMIDIAudioProcessor::addMidiMessageToBuffer(
    const juce::MidiMessage& message)
{
    juce::ScopedLock lock(midiBufferLock);
    pendingMidiMessages.addEvent(message, 0);
}

void StraDellaMIDIAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer, 
    juce::MidiBuffer& midiMessages)
{
    juce::ScopedLock lock(midiBufferLock);
    midiMessages.addEvents(pendingMidiMessages, 0, buffer.getNumSamples(), 0);
    pendingMidiMessages.clear();
}
```

**Why This Matters**:
- Editor runs on the message thread (UI thread)
- `processBlock()` runs on the audio thread
- `CriticalSection` prevents race conditions

### 5. File Structure Changes

#### New Files
- `Source/PluginProcessor.h` - Plugin processor interface
- `Source/PluginProcessor.cpp` - Plugin processor implementation
- `Source/PluginEditor.h` - Plugin editor interface
- `Source/PluginEditor.cpp` - Plugin editor implementation (adapted from MainComponent)
- `VST3_BUILD_GUIDE.md` - Build instructions for plugin

#### Preserved Files (For Backwards Compatibility)
- `Source/Main.cpp` - Standalone entry point
- `Source/MainComponent.h` - Standalone UI
- `Source/MainComponent.cpp` - Standalone logic

These files allow users to still build a standalone version by changing the project type in Projucer.

#### Unchanged Files
- `Source/StradellaKeyboardMapper.h/cpp` - Core MIDI mapping logic
- `Source/KeyboardGUI.h/cpp` - Visual keyboard display
- `Source/MIDIMessageDisplay.h/cpp` - MIDI log display
- `Source/MouseMidiExpression.h/cpp` - Mouse-based expression
- `Source/MouseMidiSettingsWindow.h/cpp` - Settings UI

### 6. Key Plugin Characteristics

#### MIDI Effect vs. Instrument

This plugin is configured as a **MIDI effect** (not an instrument):
- `pluginIsSynth="0"`: Not a synthesizer
- `pluginIsMidiEffectPlugin="1"`: Is a MIDI effect
- `pluginProducesMidiOut="1"`: Generates MIDI output

**Why MIDI Effect?**
- The plugin doesn't generate audio
- It transforms keyboard input into MIDI notes
- It can be chained with other MIDI effects
- It requires a separate instrument to produce sound

#### Keyboard Focus Requirement

The plugin requires keyboard focus to function:
- `pluginEditorRequiresKeys="1"`: Tells the host to give keyboard events to the plugin
- Without this, the DAW would consume keyboard shortcuts instead
- Users must click the plugin window to activate keyboard input

### 7. Plugin Metadata

```cpp
// Unique plugin identification
pluginCode="Strd"              // 4-character unique ID
pluginManufacturerCode="Papa"  // 4-character manufacturer ID

// These IDs ensure the plugin is uniquely identified by DAWs
// Change these if you fork the project!
```

### 8. State Management

The processor includes state save/load methods for future parameter automation:

```cpp
void getStateInformation(juce::MemoryBlock& destData) override;
void setStateInformation(const void* data, int sizeInBytes) override;
```

Currently placeholder implementations, but allow for:
- Saving keyboard mappings
- Saving expression settings
- DAW automation of parameters

## Building and Testing

### Build Process

1. Open `straDellaMIDI.jucer` in Projucer
2. Projucer generates platform-specific build files
3. Build using Xcode (macOS), Visual Studio (Windows), or Make (Linux)
4. Output: `straDellaMIDI.vst3` plugin bundle

### Installation Paths

**macOS**:
```
~/Library/Audio/Plug-Ins/VST3/straDellaMIDI.vst3
```

**Windows**:
```
C:\Program Files\Common Files\VST3\straDellaMIDI.vst3
```

**Linux**:
```
~/.vst3/straDellaMIDI.vst3
```

### Testing in Logic Pro

1. Launch Logic Pro
2. Create a new project with a Software Instrument track
3. Add straDellaMIDI as a MIDI FX plugin
4. Add a software instrument below it
5. Click the plugin window to focus
6. Play notes on your keyboard
7. The instrument should produce sound based on your input

## Comparison: Standalone vs. Plugin

| Feature | Standalone | VST3 Plugin |
|---------|-----------|-------------|
| **Entry Point** | `Main.cpp` with `JUCEApplication` | Plugin processor factory function |
| **Window** | `DocumentWindow` | Plugin editor window managed by host |
| **MIDI Output** | Direct to system MIDI ports | Through DAW's MIDI routing |
| **Keyboard Input** | Always available when focused | Requires `pluginEditorRequiresKeys` |
| **Launch** | Standalone executable | Loaded by DAW |
| **Automation** | N/A | Can be automated by DAW (future) |
| **Multi-Instance** | Must launch multiple apps | Can load multiple instances in one DAW |
| **MIDI Routing** | System MIDI connections | DAW internal routing |

## Benefits of Plugin Version

1. **DAW Integration**
   - Works within existing music production workflow
   - Access to DAW's MIDI routing
   - Can use with any virtual instrument

2. **Multiple Instances**
   - Run multiple instances with different settings
   - Each instance can route to different instruments

3. **Session Recall**
   - Settings saved with DAW project
   - Consistent setup across sessions

4. **Professional Workflow**
   - Standard plugin format
   - Compatible with automation
   - Works in commercial DAWs

5. **No External MIDI Routing**
   - No need for virtual MIDI cables
   - Everything routed internally in the DAW

## Limitations and Future Work

### Current Limitations

1. **Keyboard Focus**: Users must click the plugin window to enable keyboard input
2. **No Parameter Automation**: Expression settings not yet exposed as automatable parameters
3. **Single MIDI Channel**: All output on channel 1 (could be made configurable)

### Future Enhancements

1. **Parameter Automation**
   - Expose velocity curves as parameters
   - Automate expression settings
   - MIDI channel selection

2. **Preset Management**
   - Save/load keyboard mappings
   - Preset browser in plugin

3. **Additional Plugin Formats**
   - AU (Audio Unit) for better Logic Pro integration
   - AAX for Pro Tools compatibility

4. **Enhanced DAW Integration**
   - Host sync for tempo-related features
   - MIDI learn for custom mappings

## Migration Guide for Users

### From Standalone to Plugin

1. **Install the Plugin**: Copy `.vst3` to the appropriate folder
2. **Open DAW**: Launch Logic Pro or other VST3-compatible DAW
3. **Insert Plugin**: Add as MIDI FX on a track
4. **Route Output**: Create instrument track to receive MIDI
5. **Configure**: Settings carry over, but are now per-instance

### Keeping Standalone Version

Users who prefer standalone can:
1. Keep the current build
2. Build from source using original project type
3. Both versions can coexist

## Conclusion

The conversion to VST3 plugin maintains all core functionality while enabling professional DAW integration. The architecture cleanly separates concerns:
- **Processor**: Handles plugin lifecycle and MIDI routing
- **Editor**: Manages UI and keyboard input
- **Core Components**: Unchanged keyboard mapping and expression logic

This design allows the plugin to work seamlessly in DAW environments while preserving the low-latency, responsive behavior of the original standalone application.

# Stradella MIDI - Accordion Emulator AU/VST3 Plugin

A JUCE-based **Audio Unit (AU) and VST3 plugin** that emulates a Stradella bass accordion using your computer keyboard to generate MIDI output. **Optimized for Logic Pro** with native AU support, and compatible with other DAWs via VST3.

> **🚨 BUILD ERRORS or 8KB .component FILE?** → See **[START_HERE.md](START_HERE.md)** for quick fix (30 seconds)

> **⚠️ IMPORTANT**: Build files are NOT in the repository! You **MUST regenerate build files with Projucer** before building! See [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md)
>
> If you get an 8KB .component file or 177MB .a library, you're using outdated build files!
>
> **Still getting 8KB after regenerating?** → See [TROUBLESHOOT_8KB_AFTER_REGENERATION.md](TROUBLESHOOT_8KB_AFTER_REGENERATION.md) - you're probably building the wrong Xcode target!

> **Note**: This project builds as AU/VST3 plugins. The standalone application files are kept for backwards compatibility.

## 🎯 Optimized for Low Latency

This application is designed for **real-time MIDI output with minimal latency**:
- **Simultaneous chord playback** - all notes trigger within microseconds
- **Zero buffering** - immediate response to rapid key presses  
- **MIDI-only architecture** - no audio processing overhead
- **Asynchronous GUI** - display updates never block MIDI output

See [LATENCY_IMPROVEMENTS.md](LATENCY_IMPROVEMENTS.md) and [ASYNC_MESSAGE_DISPLAY.md](ASYNC_MESSAGE_DISPLAY.md) for technical details.

## Features

### Mouse MIDI Expression (NEW!)
Emulates accordion bellows using mouse movement for expressive control:
- **CC7 (Volume)**: Controlled by mouse movement velocity
- **CC11 (Expression)**: Controlled by mouse Y position
- **Configurable curves**: Linear, Exponential, or Logarithmic response
- **Settings window**: Easy configuration via "Mouse Settings" button

See [MOUSE_MIDI_EXPRESSION.md](MOUSE_MIDI_EXPRESSION.md) for detailed documentation.

### Keyboard Mapping
The application maps computer keyboard keys to MIDI notes following a Stradella accordion layout:

#### Row 1: Numbers (1-0) - Minor Chords
- Keys `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`, `0`
- Generate minor triads (root + minor third + perfect fifth)
- All notes in Octave 2 as per Stradella bass system
- Cycle of fifths: Eb, Bb, F, C, G, D, A, E, B, F#
- Example: Key `1` = Eb2 Minor (Eb, Gb, Bb), Key `4` = C2 Minor (C, Eb, G)

#### Row 2: Top Letters (Q-P) - Major Chords
- Keys `Q`, `W`, `E`, `R`, `T`, `Y`, `U`, `I`, `O`, `P`
- Generate major triads (root + major third + perfect fifth)
- All notes in Octave 2 as per Stradella bass system
- Cycle of fifths: Eb, Bb, F, C, G, D, A, E, B, F#
- Example: Key `Q` = Eb2 Major (Eb, G, Bb), Key `R` = C2 Major (C, E, G)

#### Row 3: Home Row (A-;) - Single Notes (Cycle of Fifths)
- Keys `A`, `S`, `D`, `F`, `G`, `H`, `J`, `K`, `L`, `;`
- Generate single notes arranged in a cycle of fifths
- Key `F` = C1 (MIDI note 24)
- All notes in Octave 1 as per Stradella bass system
- Full mapping: A=Eb1, S=Bb1, D=F1, F=C1, G=G1, H=D1, J=A1, K=E1, L=B1, ;=F#1

#### Row 4: Bottom Row (Z-/) - Third Intervals
- Keys `Z`, `X`, `C`, `V`, `B`, `N`, `M`, `,`, `.`, `/`
- Generate notes a major third (4 semitones) above the corresponding single notes
- All notes in Octave 1 as per Stradella bass system
- Example: Key `V` = E1 (third above C1 which is the F key)

### Visual Interface

The application provides a visual representation of the keyboard:
- **Color-coded keys** by function:
  - Orange: Minor chords
  - Green: Major chords
  - Blue: Single notes
  - Cyan: Third interval notes
- **Real-time visual feedback**: Keys light up when pressed
- **Note labels**: Each key shows its corresponding note(s)

### MIDI Message Display

A collapsible panel at the bottom shows:
- All MIDI messages in real-time
- Note names and MIDI note numbers
- Timestamps for each event
- Note on/off events with velocities

## Components

### Core Classes

#### `StradellaKeyboardMapper`
- Maps keyboard keys to MIDI note numbers
- Implements cycle of fifths logic
- Supports loading custom configurations
- Provides note name conversion utilities

#### `KeyboardGUI`
- Visual representation of the keyboard layout
- Real-time visual feedback for key presses
- Color-coded keys by type
- Displays note information on each key

#### `MIDIMessageDisplay`
- Shows MIDI messages as they occur
- Collapsible/expandable panel
- Scrollable message history (last 100 messages)
- Formatted timestamps and note information

#### `MainComponent`
- Main application window
- Handles keyboard input events
- Manages MIDI output
- Coordinates between all components

## Configuration

The keyboard mapping can be customized through a configuration file:
- Default configuration: `Source/default_keyboard_mapping.txt`
- Format: `KEY = MIDI_NOTE(S)`
- Comments start with `#`
- Chords use comma-separated note numbers

Example:
```
# Single note
F = 24      # C1

# Major chord (C2 Major)
R = 36,40,43    # C, E, G
```

## Building the Plugin

This project builds as an **AU (Audio Unit) and VST3 plugin** for use in DAWs.

> **⚠️ CRITICAL**: You must regenerate build files with Projucer! See [REGENERATE_BUILD_FILES.md](REGENERATE_BUILD_FILES.md)

### Quick Start

1. **Install JUCE Framework**
   - Download from [juce.com](https://juce.com/)
   - Install Projucer

2. **Regenerate Build Files** ⚠️ **REQUIRED**
   - Open `straDellaMIDI.jucer` in Projucer
   - Set your JUCE modules path (File → Global Paths)
   - **Click "Save Project"** to regenerate build files
   - This creates proper plugin targets (AU, VST3)
   - **Verify**: Run `./verify_build_config.sh` to check configuration

3. **Build the Plugin**
   - Open generated project in Xcode/Visual Studio
   - Build using the IDE (builds both AU and VST3 on macOS)

4. **Install the Plugin**
   - **macOS AU** (recommended for Logic Pro): Copy to `~/Library/Audio/Plug-Ins/Components/`
   - **macOS VST3**: Copy to `~/Library/Audio/Plug-Ins/VST3/`
   - **Windows VST3**: Copy to `C:\Program Files\Common Files\VST3\`
   - **Linux VST3**: Copy to `~/.vst3/`

For detailed build instructions, see [VST3_BUILD_GUIDE.md](VST3_BUILD_GUIDE.md).

### Building as Standalone Application (Legacy)

The original standalone application can still be built by:
1. Changing the project type in Projucer from "Audio Plug-In" to "GUI Application"
2. Replacing plugin files with the original `Main.cpp`
3. Rebuilding the project

## Usage

### In a DAW (Logic Pro, etc.)

1. **Load the Plugin**
   - In Logic Pro: Insert straDellaMIDI as a **MIDI FX** plugin (not audio insert)
   - Use AU format for best Logic Pro integration
   - The plugin window will open showing the keyboard interface

2. **Route MIDI Output**
   - Create an instrument track to receive MIDI from the plugin
   - Route the plugin's MIDI output to your desired instrument

3. **Play Notes**
   - Click the plugin window to give it keyboard focus
   - Press keys to generate MIDI notes:
     - Single notes: A, S, D, F, G, H, J, K, L, ;
     - Third intervals: Z, X, C, V, B, N, M, ,, ., /
     - Major chords: Q, W, E, R, T, Y, U, I, O, P
     - Minor chords: 1, 2, 3, 4, 5, 6, 7, 8, 9, 0

4. **Expression Control**
   - Move your mouse for bellows-style expression
   - Click "Expression Settings" to configure curves
   - Monitor MIDI messages in the bottom panel

### As Standalone Application (Legacy)

If you've built the standalone version:

1. Launch the application
2. Ensure a MIDI output device is available
3. The application automatically grabs keyboard focus - just start playing!
4. Press keys to generate MIDI notes (see mapping above)
5. Watch the visual feedback on the keyboard display
6. Monitor MIDI messages in the bottom panel

## Testing

See [TESTING_GUIDE.md](TESTING_GUIDE.md) for comprehensive testing instructions including:
- Chord simultaneity verification
- Rapid input testing
- Latency measurement with MIDI monitoring software

## MIDI Output

### As VST3 Plugin
- The plugin produces MIDI output that is routed through your DAW
- All notes are sent on MIDI channel 1
- Note velocity is controlled by mouse Y position (default ~100)
- Both note-on and note-off messages are sent
- MIDI CC7 (Volume) and CC11 (Expression) are sent based on mouse movement

### As Standalone Application (Legacy)
- The application automatically connects to the first available MIDI output device
- All notes are sent on MIDI channel 1
- Note velocity is set to 100 (approximately mf - mezzo-forte)
- Both note-on and note-off messages are sent

## Technical Details

### Low-Latency MIDI Architecture
- **MIDI-only design**: No audio processing overhead
- **Direct MIDI output**: Uses `sendMessageNow()` for immediate transmission
- **Zero audio latency**: No audio device initialization or buffer delays
- **Lightweight component**: Inherits from `juce::Component` (not `AudioAppComponent`)

### Key Press Handling
- Uses JUCE's `KeyListener` interface
- Prevents OS key repeat by tracking pressed keys
- Immediate key-to-MIDI conversion with no buffering
- Properly handles key release events
- Case-insensitive letter key handling

### MIDI Generation
- Uses JUCE's `MidiOutput` class
- Real-time message sending with `sendMessageNow()`
- Supports multiple simultaneous notes (chords)
- Proper note-off messages for sustained playing
- Direct hardware communication without queuing

### GUI Updates
- Immediate visibility on startup (no loading delay)
- Efficient repainting on key state changes
- Component-based architecture
- Responsive layout with resizable components

## Extending the Application

### Adding New Key Mappings
1. Edit `default_keyboard_mapping.txt`
2. Add new mappings using the same format
3. Reload the application

### Custom Configuration Loading
The `StradellaKeyboardMapper::loadConfiguration()` method can be extended to:
- Load custom configuration files at runtime
- Parse different file formats
- Validate mapping configurations
- Support multiple mapping profiles

### Future Enhancements
- Multiple mapping profiles with hot-swapping
- MIDI input for reverse mapping
- Recording and playback functionality
- Adjustable velocity sensitivity
- Octave shifting controls
- Sustain pedal support
- Customizable colors and themes

## Troubleshooting Build Errors

### ⚠️ Common Build Issues

If you encounter build errors, see our troubleshooting guides:

**"juce_vst3_helper: No such file or directory"**
- See: [VST3_HELPER_ERROR_FIX.md](VST3_HELPER_ERROR_FIX.md)
- Fix: Regenerate build files with Projucer

**"Command PhaseScriptExecution failed"**
- See: [AU_BUILD_FIX.md](AU_BUILD_FIX.md) or [VST3_HELPER_ERROR_FIX.md](VST3_HELPER_ERROR_FIX.md)
- Fix: Regenerate build files with Projucer

**Quick Reference**: [BUILD_ERROR_QUICK_REFERENCE.md](BUILD_ERROR_QUICK_REFERENCE.md)

**Why This Happens**: The `.jucer` file is configured for plugins, but the Xcode project in `Builds/MacOSX/` needs to be regenerated to match. This is a one-time setup step that takes about 30 seconds.


## License

This project uses the JUCE framework. Please ensure compliance with JUCE licensing requirements.

## Credits

Built with [JUCE](https://juce.com/) - Cross-platform C++ framework for audio applications.

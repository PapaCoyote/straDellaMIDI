# Stradella MIDI - Accordion Emulator

A JUCE-based application that emulates a Stradella bass accordion using your computer keyboard to generate MIDI output.

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

## Building the Application

This is a JUCE project. To build:

1. **Install JUCE Framework**
   - Download from [juce.com](https://juce.com/)
   - Install Projucer

2. **Open the Project**
   - Open `straDellaMIDI.jucer` in Projucer
   - Set your JUCE modules path
   - Select your target platform (Xcode, Visual Studio, Linux Makefile)

3. **Generate Build Files**
   - Click "Save Project" in Projucer
   - This generates native IDE projects in the `Builds` folder

4. **Build**
   - **macOS**: Open `Builds/MacOSX/straDellaMIDI.xcodeproj` in Xcode and build
   - **Windows**: Open the Visual Studio solution and build
   - **Linux**: Use the generated Makefile

## Usage

1. Launch the application
2. Ensure a MIDI output device is available
3. The application automatically grabs keyboard focus - just start playing!
4. Press keys to generate MIDI notes:
   - Single notes: A, S, D, F, G, H, J, K, L, ;
   - Third intervals: Z, X, C, V, B, N, M, ,, ., /
   - Major chords: Q, W, E, R, T, Y, U, I, O, P
   - Minor chords: 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
5. Watch the visual feedback on the keyboard display
6. Monitor MIDI messages in the bottom panel (updates asynchronously)
7. Click "MIDI Messages" button to collapse/expand the message log

**Note**: If keys don't respond, click the application window to ensure it has focus.

## Testing

See [TESTING_GUIDE.md](TESTING_GUIDE.md) for comprehensive testing instructions including:
- Chord simultaneity verification
- Rapid input testing
- Latency measurement with MIDI monitoring software

## MIDI Output

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

## License

This project uses the JUCE framework. Please ensure compliance with JUCE licensing requirements.

## Credits

Built with [JUCE](https://juce.com/) - Cross-platform C++ framework for audio applications.

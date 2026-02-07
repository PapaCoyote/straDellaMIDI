# Mouse MIDI Expression Feature

## Overview

The Mouse MIDI Expression feature emulates the action of accordion bellows by translating mouse movement into MIDI Control Change (CC) messages. This provides expressive control over modulation and expression in real-time.

**Key Features:**
- **Global mouse tracking** - Works across entire desktop, not just within the application window
- **No visual area required** - Mouse tracking happens in the background
- **Real-time MIDI CC generation** - Immediate response to mouse movement

## Features

### CC1 (Modulation Wheel) - Mouse Velocity
- Controlled by the speed of mouse movement
- Faster mouse movement = higher modulation value
- Range: 0-127 (MIDI standard)
- Tracks velocity in pixels per second
- Independent enable/disable control

### CC11 (Expression) - Y Position  
- Controlled by the vertical (Y) position of the mouse on the screen
- Top of the screen = maximum expression (127)
- Bottom of the screen = minimum expression (0)
- Provides continuous positional feedback based on desktop coordinates
- Independent enable/disable control

## User Interface

### Mouse Expression Tracking
- **Global tracking** - Mouse movement is tracked across the entire desktop
- **No visual area** - No dedicated GUI component needed
- **Transparent operation** - Works in the background while you use the application

### Settings Window
Access via the "Mouse Settings" button in the top-right corner.

**Available Settings:**
1. **CC1 (Modulation Wheel) Checkbox** - Enable/disable velocity-based modulation control
2. **CC11 (Expression) Checkbox** - Enable/disable Y-position-based expression control
3. **Response Curve Selector** - Choose how MIDI values respond to input:
   - **Linear**: Direct 1:1 mapping (default)
   - **Exponential**: Slower response at low values, faster at high values (x²)
   - **Logarithmic**: Faster response at low values, slower at high values (√x)

## Usage

1. **Launch the application** and ensure MIDI output is connected
2. **Move your mouse** anywhere on your desktop
3. **Adjust settings** by clicking "Mouse Settings" button:
   - Check/uncheck CC1 or CC11 as needed
   - Select desired response curve
   - Close settings window
4. **Play notes** using the keyboard while moving the mouse for expression

The mouse tracking works globally - you don't need to keep your mouse over the application window.

## Technical Details

### MIDI CC Messages
- **CC1**: MIDI standard for Modulation Wheel
- **CC11**: MIDI standard for Expression
- **Channel**: All messages sent on MIDI Channel 1
- **Value Range**: 0-127 for both controllers

### Global Mouse Tracking
- Uses JUCE's Desktop API to get global mouse position
- Polls at ~60 Hz (16ms intervals) via Timer
- Works across entire primary display
- No window focus required

### Velocity Calculation
```
velocity = distance / time_delta
normalized_velocity = min(1.0, velocity / 2000.0)
midi_value = apply_curve(normalized_velocity) * 127
```

### Expression Calculation
```
normalized_y = 1.0 - (mouse_screen_y / screen_height)
midi_value = apply_curve(normalized_y) * 127
```

### Curve Functions
- **Linear**: `f(x) = x`
- **Exponential**: `f(x) = x²`
- **Logarithmic**: `f(x) = √x`

### Performance Optimizations
- Only sends CC messages when value changes by ≥1
- Caches last sent values to avoid redundant MIDI traffic
- Uses callback architecture for non-blocking MIDI output
- Visual updates use JUCE's efficient repaint system

## Integration

### Components
- **MouseMidiExpression**: Core component handling mouse input and MIDI generation
  - Uses Timer for global mouse position polling
  - No visual component needed
  - Tracks mouse across entire desktop
- **MouseMidiSettingsWindow**: Configuration UI for user preferences
- **MainComponent**: Integrates mouse expression with existing keyboard functionality

### MIDI Output Flow
```
Global Mouse Movement → Timer Poll → MouseMidiExpression → MainComponent::sendMidiMessage() → MIDI Output Device
```

### Message Display
All CC messages are logged in the MIDI Message Display panel at the bottom of the window, alongside note on/off messages.

## Future Enhancements

Possible additions for future versions:
- Velocity sensitivity slider
- Custom curve editor with visual preview
- CC value range limiting (min/max)
- Multiple assignable CC numbers
- Preset management for different configurations
- Mouse button modifiers for temporary effect changes
- Smoothing/filtering options for CC output

## Troubleshooting

**Mouse movements aren't generating MIDI:**
- Ensure at least one CC type is enabled in settings
- Check MIDI output device is connected
- Verify application is running (tracking works even when window is not focused)
- Check console logs for debug messages

**CC values seem wrong or inverted:**
- Try different curve types in settings
- For CC11, remember: top of screen = high, bottom = low
- Check that your DAW is receiving on MIDI Channel 1

**DAW not receiving CC messages:**
- Verify the application's MIDI output is connected to your DAW input
- Check your DAW's MIDI learn or monitor to see incoming messages
- Some DAWs require you to enable CC input or map controllers
- Check console logs to verify messages are being sent with correct format

## Code Files

- `Source/MouseMidiExpression.h` - Header for mouse MIDI component
- `Source/MouseMidiExpression.cpp` - Implementation of mouse MIDI logic
- `Source/MouseMidiSettingsWindow.h` - Header for settings UI
- `Source/MouseMidiSettingsWindow.cpp` - Implementation of settings UI
- `Source/MainComponent.h` - Updated to integrate mouse expression
- `Source/MainComponent.cpp` - Updated to wire mouse expression to MIDI output

## Building

After adding these files, rebuild the project using Projucer:
1. Open `straDellaMIDI.jucer` in Projucer
2. Click "Save Project" to regenerate build files
3. Build using your platform's IDE (Xcode, Visual Studio, etc.)

The new source files are already included in the .jucer project file.

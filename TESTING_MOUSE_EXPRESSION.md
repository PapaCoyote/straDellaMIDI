# Testing Guide for Mouse MIDI Expression Feature

## Pre-Testing Setup

1. **Open the project in Projucer**
   ```
   Open: straDellaMIDI.jucer
   ```

2. **Verify new files are included**
   - Check that the following files appear in the Source group:
     - MouseMidiExpression.h
     - MouseMidiExpression.cpp
     - MouseMidiSettingsWindow.h
     - MouseMidiSettingsWindow.cpp

3. **Generate build files**
   - In Projucer, click "Save Project"
   - This will regenerate platform-specific build files

4. **Build the application**
   - **macOS**: Open `Builds/MacOSX/straDellaMIDI.xcodeproj` and build
   - **Windows**: Open the Visual Studio solution and build
   - **Linux**: Use the generated Makefile

## Testing Checklist

### Basic Functionality
- [ ] Application launches without errors
- [ ] "Mouse Settings" button appears in top-right corner
- [ ] Mouse expression area visible on right side (200px wide)
- [ ] Keyboard functionality still works as before

### Mouse Expression Area
- [ ] Moving mouse over the area shows a crosshair
- [ ] Crosshair follows mouse position accurately
- [ ] Info text displays current enabled CC types
- [ ] Border and background are visible

### Settings Window
- [ ] Clicking "Mouse Settings" button opens the settings window
- [ ] Settings window appears centered
- [ ] Window shows:
  - Title: "Mouse MIDI Expression Settings"
  - CC7 (Volume) checkbox
  - CC11 (Expression) checkbox
  - Response Curve dropdown
  - Info text
  - Close button
- [ ] Both checkboxes are checked by default
- [ ] Curve selector shows "Linear" by default
- [ ] Clicking "Close" hides the window
- [ ] Clicking "Mouse Settings" again toggles window visibility

### CC7 (Volume) - Mouse Velocity
**Setup**: Enable MIDI monitoring software (e.g., MIDI Monitor on macOS)

- [ ] Fast mouse movement generates high CC7 values (closer to 127)
- [ ] Slow mouse movement generates low CC7 values (closer to 0)
- [ ] Velocity is calculated regardless of direction
- [ ] CC7 messages appear in bottom MIDI log panel
- [ ] Disabling CC7 in settings stops CC7 messages
- [ ] Re-enabling CC7 resumes CC7 messages

### CC11 (Expression) - Y Position
**Setup**: Enable MIDI monitoring software

- [ ] Mouse at top of area generates high CC11 values (127)
- [ ] Mouse at bottom of area generates low CC11 values (0)
- [ ] CC11 values change smoothly as mouse moves vertically
- [ ] Horizontal movement doesn't affect CC11 (only Y position)
- [ ] CC11 messages appear in bottom MIDI log panel
- [ ] Disabling CC11 in settings stops CC11 messages
- [ ] Re-enabling CC11 resumes CC11 messages

### Curve Types
**Test each curve type**:

Linear Curve:
- [ ] Select "Linear" in settings
- [ ] Movement produces proportional CC values
- [ ] Values respond evenly across the range

Exponential Curve:
- [ ] Select "Exponential" in settings
- [ ] Slow movements produce lower values than Linear
- [ ] Fast movements produce higher values than Linear
- [ ] Response feels "accelerated"

Logarithmic Curve:
- [ ] Select "Logarithmic" in settings
- [ ] Slow movements produce higher values than Linear
- [ ] Fast movements produce lower values than Linear
- [ ] Response feels "dampened"

### Integration with Existing Features
- [ ] Keyboard still responds to key presses
- [ ] Keyboard GUI still shows visual feedback
- [ ] Note on/off messages still work
- [ ] MIDI log shows both notes and CC messages
- [ ] No performance degradation

### Performance Tests
- [ ] Rapid mouse movement doesn't cause lag
- [ ] Application remains responsive
- [ ] No excessive MIDI traffic (should throttle to 1 value steps)
- [ ] Visual crosshair updates smoothly

### Edge Cases
- [ ] Mouse leaving the expression area stops CC generation
- [ ] Mouse entering the area resumes CC generation
- [ ] Settings persist while window is hidden
- [ ] Changing settings immediately affects behavior
- [ ] Both CC7 and CC11 can be disabled simultaneously
- [ ] Application works with no MIDI device connected (no crash)

## Expected MIDI Output

### CC7 (Volume) Example
```
MIDI CC: Channel 1, Controller 7, Value 45
MIDI CC: Channel 1, Controller 7, Value 67
MIDI CC: Channel 1, Controller 7, Value 89
```

### CC11 (Expression) Example
```
MIDI CC: Channel 1, Controller 11, Value 100
MIDI CC: Channel 1, Controller 11, Value 85
MIDI CC: Channel 1, Controller 11, Value 72
```

## Known Limitations

- Mouse must be within the expression area to generate CC messages
- CC values only change by integer steps (1-127), no sub-integer resolution
- Maximum velocity is capped at 2000 pixels/second

## Reporting Issues

If you encounter any problems, please report:
1. Description of the issue
2. Steps to reproduce
3. Expected vs actual behavior
4. Build platform (macOS/Windows/Linux)
5. JUCE version
6. Any error messages or console output

## Success Criteria

✅ The feature is working correctly if:
1. Mouse movement generates appropriate CC7 messages
2. Mouse Y position generates appropriate CC11 messages
3. Settings window allows configuration
4. Different curves produce noticeably different responses
5. No crashes or errors occur
6. Existing keyboard functionality is unaffected

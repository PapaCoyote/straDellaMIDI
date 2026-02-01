# Architecture Diagram - Stradella MIDI Emulator

## Component Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Main Application                              │
│                    (straDellaMIDIApplication)                        │
└────────────────────────────────┬────────────────────────────────────┘
                                 │
                                 │ creates
                                 ▼
                    ┌────────────────────────┐
                    │      MainWindow        │
                    │  (DocumentWindow)      │
                    └───────────┬────────────┘
                                │
                                │ contains
                                ▼
        ┌───────────────────────────────────────────────────┐
        │             MainComponent                          │
        │         (AudioAppComponent)                        │
        │                                                    │
        │  Responsibilities:                                 │
        │  • Keyboard event handling (KeyListener)          │
        │  • MIDI output management                         │
        │  • Component coordination                         │
        │  • Audio device management                        │
        └───┬───────────────────────────┬───────────────┬───┘
            │                           │               │
            │ owns                      │ owns          │ owns
            ▼                           ▼               ▼
  ┌─────────────────────┐   ┌──────────────────┐   ┌─────────────────┐
  │ StradellaKeyboard   │   │  KeyboardGUI     │   │ MIDIMessage     │
  │      Mapper         │   │                  │   │    Display      │
  └─────────────────────┘   └──────────────────┘   └─────────────────┘
```

## Component Details

### 1. MainComponent (Controller + Audio Manager)
```
┌────────────────────────────────────────────────────────────┐
│ MainComponent                                               │
├────────────────────────────────────────────────────────────┤
│ Properties:                                                 │
│  • keyboardMapper: StradellaKeyboardMapper                 │
│  • keyboardGUI: unique_ptr<KeyboardGUI>                    │
│  • midiDisplay: unique_ptr<MIDIMessageDisplay>             │
│  • midiOutput: unique_ptr<MidiOutput>                      │
│  • currentlyPressedKeys: Array<int>                        │
├────────────────────────────────────────────────────────────┤
│ Key Methods:                                                │
│  • keyPressed(KeyPress) → bool                             │
│  • keyStateChanged(bool) → bool                            │
│  • handleKeyPress(keyCode) → void                          │
│  • handleKeyRelease(keyCode) → void                        │
│  • sendMidiMessage(MidiMessage) → void                     │
└────────────────────────────────────────────────────────────┘
```

### 2. StradellaKeyboardMapper (Data Model)
```
┌────────────────────────────────────────────────────────────┐
│ StradellaKeyboardMapper                                     │
├────────────────────────────────────────────────────────────┤
│ Properties:                                                 │
│  • keyMappings: HashMap<int, KeyMapping>                   │
├────────────────────────────────────────────────────────────┤
│ Key Methods:                                                │
│  • getMidiNotesForKey(keyCode) → Array<int>                │
│  • getKeyType(keyCode) → KeyType                           │
│  • getKeyDescription(keyCode) → String                     │
│  • loadConfiguration(File) → bool                          │
│  • setupDefaultMappings() → void                           │
│  + static getMidiNoteName(noteNumber) → String             │
├────────────────────────────────────────────────────────────┤
│ Key Structures:                                             │
│  • KeyMapping { keyCode, type, midiNotes[], description }  │
│  • KeyType enum { SingleNote, ThirdNote,                   │
│                   MajorChord, MinorChord }                 │
└────────────────────────────────────────────────────────────┘
```

### 3. KeyboardGUI (View)
```
┌────────────────────────────────────────────────────────────┐
│ KeyboardGUI                                                 │
├────────────────────────────────────────────────────────────┤
│ Properties:                                                 │
│  • keyMapper: StradellaKeyboardMapper& (reference)         │
│  • keys: Array<KeyButton>                                  │
├────────────────────────────────────────────────────────────┤
│ Key Methods:                                                │
│  • setKeyPressed(keyCode, bool) → void                     │
│  • isKeyPressed(keyCode) → bool                            │
│  • paint(Graphics) → void                                  │
│  • resized() → void                                        │
├────────────────────────────────────────────────────────────┤
│ Internal Structures:                                        │
│  • KeyButton { bounds, keyCode, label,                     │
│                isPressed, type }                           │
│  • drawKey(Graphics, KeyButton) → void                     │
│  • getColourForKeyType(KeyType, bool) → Colour             │
└────────────────────────────────────────────────────────────┘
```

### 4. MIDIMessageDisplay (View)
```
┌────────────────────────────────────────────────────────────┐
│ MIDIMessageDisplay                                          │
├────────────────────────────────────────────────────────────┤
│ Properties:                                                 │
│  • messageLog: TextEditor                                  │
│  • toggleButton: TextButton                                │
│  • messageHistory: StringArray                             │
│  • expanded: bool                                          │
├────────────────────────────────────────────────────────────┤
│ Key Methods:                                                │
│  • addMidiMessage(MidiMessage) → void                      │
│  • clearMessages() → void                                  │
│  • setExpanded(bool) → void                                │
│  • isExpanded() → bool                                     │
└────────────────────────────────────────────────────────────┘
```

## Data Flow

### Key Press Event Flow
```
  User Presses Key
       │
       ▼
  ┌────────────────┐
  │ JUCE Framework │
  │  Key Handler   │
  └────────┬───────┘
           │
           ▼
  ┌─────────────────────────┐
  │ MainComponent           │
  │ ::keyPressed()          │
  └──────────┬──────────────┘
             │
             ├─────────────────────────────┐
             │                             │
             ▼                             ▼
  ┌──────────────────────┐    ┌────────────────────────┐
  │ StradellaKeyboard    │    │ KeyboardGUI            │
  │ Mapper               │    │ ::setKeyPressed(true)  │
  │ ::getMidiNotesFor    │    └────────────────────────┘
  │   Key()              │                │
  └──────────┬───────────┘                │
             │                            ▼
             │                    ┌──────────────┐
             │                    │ Repaint GUI  │
             │                    └──────────────┘
             ▼
  ┌────────────────────────┐
  │ For each MIDI note:    │
  └──────────┬─────────────┘
             │
             ├─────────────────────────────┐
             │                             │
             ▼                             ▼
  ┌──────────────────┐        ┌───────────────────────┐
  │ JUCE MidiOutput  │        │ MIDIMessageDisplay    │
  │ ::sendMessage    │        │ ::addMidiMessage()    │
  │   Now()          │        └───────────────────────┘
  └──────────────────┘                    │
             │                            ▼
             │                   ┌────────────────┐
             │                   │ Update Log     │
             │                   └────────────────┘
             ▼
  ┌───────────────────┐
  │ MIDI Device       │
  │ (Hardware/SW)     │
  └───────────────────┘
```

### Key Release Event Flow
```
  User Releases Key
       │
       ▼
  ┌────────────────┐
  │ JUCE Framework │
  │ Key Handler    │
  └────────┬───────┘
           │
           ▼
  ┌─────────────────────────┐
  │ MainComponent           │
  │ ::keyStateChanged()     │
  └──────────┬──────────────┘
             │
             ├─────────────────────────────┐
             │                             │
             ▼                             ▼
  ┌──────────────────────┐    ┌────────────────────────┐
  │ StradellaKeyboard    │    │ KeyboardGUI            │
  │ Mapper               │    │ ::setKeyPressed(false) │
  │ ::getMidiNotesFor    │    └────────────────────────┘
  │   Key()              │                │
  └──────────┬───────────┘                │
             │                            ▼
             │                    ┌──────────────┐
             │                    │ Repaint GUI  │
             │                    └──────────────┘
             ▼
  ┌────────────────────────┐
  │ For each MIDI note:    │
  │ Send Note OFF          │
  └──────────┬─────────────┘
             │
             └─────> (Same as Note ON flow)
```

## Key Relationships

### Ownership
- `MainComponent` **owns** all other components
- `KeyboardGUI` **references** `StradellaKeyboardMapper` (not owned)
- Components are created as `std::unique_ptr` for automatic memory management

### Communication
- **One-way dependencies**: Components don't call back to MainComponent
- **Event-driven**: JUCE framework delivers events to MainComponent
- **Data queries**: GUI components query mapper for display information
- **Command pattern**: MainComponent commands GUI updates

### Lifecycle
1. **Initialization**: MainComponent constructs all components
2. **Runtime**: Event loop processes keyboard input
3. **Shutdown**: Automatic cleanup via unique_ptr destructors

## Threading Model

```
┌──────────────────┐          ┌──────────────────┐
│  Message Thread  │          │  Audio Thread    │
│  (GUI + Input)   │          │                  │
├──────────────────┤          ├──────────────────┤
│ • KeyboardGUI    │          │ • prepareToPlay()│
│ • MIDIDisplay    │          │ • getNextAudio   │
│ • KeyListener    │          │   Block()        │
│ • MIDI Output    │          │ • releaseRes()   │
└──────────────────┘          └──────────────────┘
```

**Note**: MIDI output is sent from message thread (safe for this use case).
For production audio apps, consider thread-safe queuing if MIDI timing is critical.

## Configuration Loading (Future)

```
  Application Start
       │
       ▼
  ┌────────────────────────┐
  │ StradellaKeyboard      │
  │ Mapper Constructor     │
  └──────────┬─────────────┘
             │
             ▼
  ┌────────────────────────┐
  │ Load default config    │
  │ (hardcoded)            │
  └────────────────────────┘

  [Future Enhancement]
       │
       ▼
  ┌────────────────────────┐
  │ Check for config file  │
  └──────────┬─────────────┘
             │
             ▼
  ┌────────────────────────┐
  │ Parse text file        │
  │ Override defaults      │
  └────────────────────────┘
```

## Design Patterns Used

1. **Model-View-Controller (MVC)**
   - Model: `StradellaKeyboardMapper`
   - View: `KeyboardGUI`, `MIDIMessageDisplay`
   - Controller: `MainComponent`

2. **Strategy Pattern**
   - Different key types handled uniformly
   - KeyType enum enables type-specific behavior

3. **Observer Pattern** (via JUCE)
   - KeyListener observes keyboard events
   - Components repaint on state changes

4. **Singleton** (implicit)
   - Single MainComponent per application
   - Single StradellaKeyboardMapper instance

## Extensibility Points

1. **Add new key mappings**: Extend `setupDefaultMappings()`
2. **Custom key types**: Add to `KeyType` enum
3. **Configuration formats**: Implement `loadConfiguration()`
4. **Visual themes**: Modify `getColourForKeyType()`
5. **MIDI features**: Extend `sendMidiMessage()`


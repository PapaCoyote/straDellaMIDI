# Stradella MIDI - Quick Reference Card

## Keyboard Layout

```
┌─────────────────────────────────────────────────────────────────┐
│  Row 1: MINOR CHORDS (Orange)                                   │
│  ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐                     │
│  │ 1 │ │ 2 │ │ 3 │ │ 4 │ │ 5 │ │ 6 │ │ 7 │                     │
│  │Eb │ │Bb │ │ F │ │ C │ │ G │ │ D │ │ A │                     │
│  │min│ │min│ │min│ │min│ │min│ │min│ │min│                     │
│  └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘                     │
│                                                                  │
│  Row 2: MAJOR CHORDS (Green)                                    │
│  ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ │
│  │ Q │ │ W │ │ E │ │ R │ │ T │ │ Y │ │ U │ │ I │ │ O │ │ P │ │
│  │Ab │ │Eb │ │Bb │ │ F │ │ C │ │ G │ │ D │ │ A │ │ E │ │ B │ │
│  │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │Maj│ │
│  └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ │
│                                                                  │
│  Row 3: SINGLE NOTES - Cycle of Fifths (Blue)                   │
│   ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐             │
│   │ A │ │ S │ │ D │ │ F │ │ G │ │ H │ │ J │ │ K │             │
│   │Eb0│ │Bb0│ │F1 │ │C2 │ │G2 │ │D3 │ │A3 │ │E4 │             │
│   │ 15│ │ 22│ │ 29│ │ 36│ │ 43│ │ 50│ │ 57│ │ 64│             │
│   └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘             │
│                                                                  │
│  Row 4: THIRD INTERVALS (Cyan)                                  │
│    ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐                  │
│    │ Z │ │ X │ │ C │ │ V │ │ B │ │ N │ │ M │                  │
│    │G0 │ │D1 │ │A1 │ │E2 │ │B2 │ │F#3│ │C#4│                  │
│    │ 19│ │ 26│ │ 33│ │ 40│ │ 47│ │ 54│ │ 61│                  │
│    └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘                  │
└─────────────────────────────────────────────────────────────────┘
```

## Key Mapping Summary

### Single Notes (A-K Row)
The **F key = C2 (MIDI 36)** is the anchor note.
Each step in the cycle of fifths = 7 semitones.

| Key | Note | MIDI | Interval from F |
|-----|------|------|----------------|
| A   | Eb0  | 15   | -3 fifths      |
| S   | Bb0  | 22   | -2 fifths      |
| D   | F1   | 29   | -1 fifth       |
| **F** | **C2**  | **36**   | **Base**          |
| G   | G2   | 43   | +1 fifth       |
| H   | D3   | 50   | +2 fifths      |
| J   | A3   | 57   | +3 fifths      |
| K   | E4   | 64   | +4 fifths      |

### Third Intervals (Z-M Row)
Each note is a **major third (+4 semitones)** above the corresponding single note.

| Key | Base Note | Third Note | MIDI |
|-----|-----------|------------|------|
| Z   | Eb0 (A)   | G0         | 19   |
| X   | Bb0 (S)   | D1         | 26   |
| C   | F1  (D)   | A1         | 33   |
| V   | C2  (F)   | E2         | 40   |
| B   | G2  (G)   | B2         | 47   |
| N   | D3  (H)   | F#3        | 54   |
| M   | A3  (J)   | C#4        | 61   |

### Major Chords (Q-P Row)
Each chord contains: **Root + Major 3rd + Perfect 5th**

| Key | Chord    | Notes       | MIDI Notes  |
|-----|----------|-------------|-------------|
| Q   | Ab Major | Ab, C, Eb   | 8, 12, 15   |
| W   | Eb Major | Eb, G, Bb   | 15, 19, 22  |
| E   | Bb Major | Bb, D, F    | 22, 26, 29  |
| R   | F Major  | F, A, C     | 29, 33, 36  |
| T   | C Major  | C, E, G     | 36, 40, 43  |
| Y   | G Major  | G, B, D     | 43, 47, 50  |
| U   | D Major  | D, F#, A    | 50, 54, 57  |
| I   | A Major  | A, C#, E    | 57, 61, 64  |
| O   | E Major  | E, G#, B    | 64, 68, 71  |
| P   | B Major  | B, D#, F#   | 71, 75, 78  |

### Minor Chords (1-7 Row)
Each chord contains: **Root + Minor 3rd + Perfect 5th**

| Key | Chord    | Notes       | MIDI Notes  |
|-----|----------|-------------|-------------|
| 1   | Eb Minor | Eb, Gb, Bb  | 15, 18, 22  |
| 2   | Bb Minor | Bb, Db, F   | 22, 25, 29  |
| 3   | F Minor  | F, Ab, C    | 29, 32, 36  |
| 4   | C Minor  | C, Eb, G    | 36, 39, 43  |
| 5   | G Minor  | G, Bb, D    | 43, 46, 50  |
| 6   | D Minor  | D, F, A     | 50, 53, 57  |
| 7   | A Minor  | A, C, E     | 57, 60, 64  |

## Usage Tips

### Getting Started
1. Launch the application
2. Click on the window to give it keyboard focus
3. Press any mapped key to generate MIDI notes
4. Watch the visual feedback and MIDI log

### Playing Technique
- **Single notes**: Use A-K row for melodies
- **Bass accompaniment**: Use number row for minor chords
- **Harmony**: Use Q-P row for major chords
- **Passing tones**: Use Z-M row for chromatic movement

### Visual Feedback
- **Unpressed keys**: Darker colors
- **Pressed keys**: Brighter colors
- **Color coding**: Orange=minor, Green=major, Blue=single, Cyan=thirds

### MIDI Messages
- Click "MIDI Messages" button to show/hide log
- Displays last 100 messages with timestamps
- Shows note names and MIDI numbers

## Cycle of Fifths Reference

The keys follow the circle of fifths pattern:
```
... → Eb → Bb → F → C → G → D → A → E → B → F# → ...
      A    S    D   F   G   H   J   K
```

Each step is a perfect fifth interval (7 semitones or 5 notes).

## MIDI Output
- **Channel**: 1
- **Velocity**: 100 (mf - mezzo-forte)
- **Note On**: Sent when key pressed
- **Note Off**: Sent when key released

## Troubleshooting

### No Sound?
- Check MIDI output device is connected
- Verify MIDI device is not muted
- Ensure application has keyboard focus

### Wrong Notes?
- Verify keyboard layout (QWERTY)
- Check Caps Lock is off (works either way)
- Ensure using correct row

### Keys Not Responding?
- Click on application window for focus
- Check key isn't stuck
- Try restarting application

## Quick Practice Exercise

Try this simple progression:
1. Press **T** (C Major) - hold
2. Press **5** (G Minor) - release T, hold
3. Press **T** (C Major) - release 5, hold
4. Press **R** (F Major) - release T

Congratulations! You just played a I-v-I-IV progression in C major!

# Quick Start: straDellaMIDI VST3 Plugin

## 🚀 Quick Setup

### 1. Build the Plugin
```bash
# Prerequisites: Install JUCE from juce.com

# Open in Projucer
Projucer straDellaMIDI.jucer

# Save project (generates build files)
# Build with Xcode/Visual Studio/Make
```

### 2. Install
```bash
# macOS
cp -r build/straDellaMIDI.vst3 ~/Library/Audio/Plug-Ins/VST3/

# Windows
copy straDellaMIDI.vst3 "C:\Program Files\Common Files\VST3\"

# Linux
cp -r straDellaMIDI.vst3 ~/.vst3/
```

### 3. Use in Logic Pro
1. Create Software Instrument track
2. Add **straDellaMIDI** as MIDI FX
3. Add instrument below it
4. Click plugin window for focus
5. Play using your keyboard!

## 🎹 Keyboard Layout

### Row 1 (Numbers): Minor Chords
```
1=Eb min  2=Bb min  3=F min  4=C min  5=G min
6=D min   7=A min   8=E min  9=B min  0=F# min
```

### Row 2 (Letters): Major Chords
```
Q=Eb maj  W=Bb maj  E=F maj  R=C maj  T=G maj
Y=D maj   U=A maj   I=E maj  O=B maj  P=F# maj
```

### Row 3 (Home Row): Single Notes
```
A=Eb1  S=Bb1  D=F1  F=C1  G=G1
H=D1   J=A1   K=E1  L=B1  ;=F#1
```

### Row 4 (Bottom): Third Intervals
```
Z=G1   X=D1   C=A1   V=E1   B=B1
N=F#1  M=C#1  ,=G#1  .=D#1  /=A#1
```

## 🎚️ Expression Control

- **Mouse Y Position**: Controls note velocity
- **Mouse Movement Speed**: Controls CC7 (Volume)
- **Expression Settings Button**: Configure curves

## 📖 Full Documentation

- [VST3_BUILD_GUIDE.md](VST3_BUILD_GUIDE.md) - Detailed build instructions
- [README.md](README.md) - Complete feature documentation
- [VST3_CONVERSION_SUMMARY.md](VST3_CONVERSION_SUMMARY.md) - Technical details

## ⚠️ Important Notes

- **Click plugin window** to enable keyboard input
- **MIDI Effect**: Route output to an instrument
- **Logic Pro**: Use MIDI FX slot, not audio insert
- **Multi-instance**: Each instance works independently

## 🐛 Troubleshooting

**Plugin not appearing?**
- Check installation path
- Rescan plugins in your DAW
- Verify VST3 format is supported

**Keys not working?**
- Click plugin window to focus
- Check DAW keyboard shortcuts aren't interfering
- Verify `pluginEditorRequiresKeys` is enabled

**No sound?**
- Plugin is MIDI only - add an instrument below it
- Check MIDI routing in your DAW
- Verify instrument is receiving MIDI

## 📧 Support

Issues: https://github.com/PapaCoyote/straDellaMIDI/issues

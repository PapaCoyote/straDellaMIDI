#include "StradellaKeyboardMapper.h"

//==============================================================================
StradellaKeyboardMapper::StradellaKeyboardMapper()
{
    loadDefaultConfiguration();
}

void StradellaKeyboardMapper::loadDefaultConfiguration()
{
    setupDefaultMappings();
}

void StradellaKeyboardMapper::setupDefaultMappings()
{
    keyMappings.clear();
    
    // Row 1: Single notes in cycle of fifths (a,s,d,f,g,h,j,k)
    // f = C2 (MIDI note 36), cycle of fifths: F, C, G, D, A, E, B
    // Going backwards from F: Bb, Eb, Ab, Db, Gb
    
    const int baseNote = 36; // C2
    
    // Mapping for single note row (a,s,d,f,g,h,j,k)
    juce::Array<int> singleNoteKeys = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K' };
    
    // Cycle of fifths positions relative to F (f key maps to F which is -5 semitones from C)
    // F(-5), C(0), G(7), D(2), A(9), E(4), B(11), F#(6)
    juce::Array<int> cycleFifthsOffsets = { -5, 0, 7, 2, 9, 4, 11, 6 }; // semitones from baseNote
    
    for (int i = 0; i < singleNoteKeys.size(); ++i)
    {
        KeyMapping mapping;
        mapping.keyCode = singleNoteKeys[i];
        mapping.type = KeyType::SingleNote;
        mapping.midiNotes.add(baseNote + cycleFifthsOffsets[i]);
        mapping.description = getMidiNoteName(baseNote + cycleFifthsOffsets[i]);
        keyMappings.set(mapping.keyCode, mapping);
    }
    
    // Row 2: Third above (z,x,c,v,b,n,m)
    // These are a major third (4 semitones) above the corresponding single notes
    juce::Array<int> thirdNoteKeys = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M' };
    
    for (int i = 0; i < thirdNoteKeys.size() && i < cycleFifthsOffsets.size(); ++i)
    {
        KeyMapping mapping;
        mapping.keyCode = thirdNoteKeys[i];
        mapping.type = KeyType::ThirdNote;
        mapping.midiNotes.add(baseNote + cycleFifthsOffsets[i] + 4); // +4 for major third
        mapping.description = getMidiNoteName(baseNote + cycleFifthsOffsets[i] + 4);
        keyMappings.set(mapping.keyCode, mapping);
    }
    
    // Row 3: Major triads (q,w,e,r,t,y,u,i,o,p)
    // Major triad: root, major third (+4), perfect fifth (+7)
    juce::Array<int> majorChordKeys = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' };
    juce::Array<int> majorChordRoots = { -5, 0, 7, 2, 9, 4, 11, 6, 1, 8 }; // Extended cycle of fifths
    
    for (int i = 0; i < majorChordKeys.size() && i < majorChordRoots.size(); ++i)
    {
        KeyMapping mapping;
        mapping.keyCode = majorChordKeys[i];
        mapping.type = KeyType::MajorChord;
        int root = baseNote + majorChordRoots[i];
        mapping.midiNotes.add(root);        // Root
        mapping.midiNotes.add(root + 4);    // Major third
        mapping.midiNotes.add(root + 7);    // Perfect fifth
        mapping.description = getMidiNoteName(root) + " Major";
        keyMappings.set(mapping.keyCode, mapping);
    }
    
    // Row 4: Minor triads (1,2,3,4,5,6,7)
    // Minor triad: root, minor third (+3), perfect fifth (+7)
    juce::Array<int> minorChordKeys = { '1', '2', '3', '4', '5', '6', '7' };
    juce::Array<int> minorChordRoots = { -5, 0, 7, 2, 9, 4, 11 }; // Same as single notes
    
    for (int i = 0; i < minorChordKeys.size() && i < minorChordRoots.size(); ++i)
    {
        KeyMapping mapping;
        mapping.keyCode = minorChordKeys[i];
        mapping.type = KeyType::MinorChord;
        int root = baseNote + minorChordRoots[i];
        mapping.midiNotes.add(root);        // Root
        mapping.midiNotes.add(root + 3);    // Minor third
        mapping.midiNotes.add(root + 7);    // Perfect fifth
        mapping.description = getMidiNoteName(root) + " Minor";
        keyMappings.set(mapping.keyCode, mapping);
    }
}

juce::Array<int> StradellaKeyboardMapper::getMidiNotesForKey(int keyCode, bool& isValidKey) const
{
    if (keyMappings.contains(keyCode))
    {
        isValidKey = true;
        return keyMappings[keyCode].midiNotes;
    }
    
    isValidKey = false;
    return {};
}

StradellaKeyboardMapper::KeyType StradellaKeyboardMapper::getKeyType(int keyCode) const
{
    if (keyMappings.contains(keyCode))
        return keyMappings[keyCode].type;
    
    return KeyType::SingleNote; // Default
}

juce::String StradellaKeyboardMapper::getKeyDescription(int keyCode) const
{
    if (keyMappings.contains(keyCode))
        return keyMappings[keyCode].description;
    
    return {};
}

juce::String StradellaKeyboardMapper::getMidiNoteName(int midiNoteNumber)
{
    static const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    
    int octave = (midiNoteNumber / 12) - 1;
    int noteIndex = midiNoteNumber % 12;
    
    return juce::String(noteNames[noteIndex]) + juce::String(octave);
}

bool StradellaKeyboardMapper::loadConfiguration(const juce::File& configFile)
{
    if (!configFile.existsAsFile())
        return false;
    
    // TODO: Implement configuration file parsing
    // For now, just use default configuration
    loadDefaultConfiguration();
    return true;
}

int StradellaKeyboardMapper::getNoteInCycleOfFifths(int startNote, int steps) const
{
    // Each step in the cycle of fifths is 7 semitones up
    return startNote + (steps * 7);
}

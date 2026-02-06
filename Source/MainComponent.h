#pragma once

#include <JuceHeader.h>
#include "StradellaKeyboardMapper.h"
#include "KeyboardGUI.h"
#include "MIDIMessageDisplay.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
                       private juce::KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    // KeyListener interface
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;
    bool keyStateChanged(bool isKeyDown, juce::Component* originatingComponent) override;

private:
    //==============================================================================
    // Stradella accordion components
    StradellaKeyboardMapper keyboardMapper;
    std::unique_ptr<KeyboardGUI> keyboardGUI;
    std::unique_ptr<MIDIMessageDisplay> midiDisplay;
    
    // MIDI output
    std::unique_ptr<juce::MidiOutput> midiOutput;
    
    // Track currently pressed keys
    juce::Array<int> currentlyPressedKeys;
    
    void sendMidiMessage(const juce::MidiMessage& message);
    void handleKeyPress(int keyCode);
    void handleKeyRelease(int keyCode);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

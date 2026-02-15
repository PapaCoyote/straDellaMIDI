/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KeyboardGUI.h"
#include "MIDIMessageDisplay.h"
#include "MouseMidiExpression.h"
#include "MouseMidiSettingsWindow.h"

//==============================================================================
/**
*/
class StraDellaMIDIAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            private juce::KeyListener
{
public:
    StraDellaMIDIAudioProcessorEditor (StraDellaMIDIAudioProcessor&);
    ~StraDellaMIDIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;
    
    //==============================================================================
    // KeyListener interface
    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;
    bool keyStateChanged(bool isKeyDown, juce::Component* originatingComponent) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StraDellaMIDIAudioProcessor& audioProcessor;
    
    //==============================================================================
    // Stradella accordion components
    std::unique_ptr<KeyboardGUI> keyboardGUI;
    std::unique_ptr<MIDIMessageDisplay> midiDisplay;
    
    // Mouse MIDI expression components
    std::unique_ptr<MouseMidiExpression> mouseMidiExpression;
    std::unique_ptr<MouseMidiSettingsWindow> mouseSettingsWindow;
    
    // Settings buttons (bottom bar)
    juce::TextButton noteMapSettingsButton;
    juce::TextButton midiSettingsButton;
    juce::TextButton expressionSettingsButton;
    
    // MIDI channel for output
    static constexpr int defaultMidiChannel = 1;
    
    void sendMidiMessage(const juce::MidiMessage& message);
    void handleKeyPress(int keyCode);
    void handleKeyRelease(int keyCode);
    void retriggerCurrentlyPressedKeys();
    void toggleMouseSettings();
    void showNoteMapSettings();
    void showMidiSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StraDellaMIDIAudioProcessorEditor)
};

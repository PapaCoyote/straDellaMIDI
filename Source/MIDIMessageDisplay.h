#pragma once

#include <JuceHeader.h>
#include "StradellaKeyboardMapper.h"

//==============================================================================
/**
    A component that displays MIDI messages in real-time with collapsible functionality.
*/
class MIDIMessageDisplay : public juce::Component,
                           private juce::Timer
{
public:
    MIDIMessageDisplay();
    ~MIDIMessageDisplay() override;
    
    /** Adds a MIDI message to the display */
    void addMidiMessage(const juce::MidiMessage& message);
    
    /** Clears all messages */
    void clearMessages();
    
    /** Sets whether the display is expanded or collapsed */
    void setExpanded(bool shouldBeExpanded);
    
    /** Returns whether the display is currently expanded */
    bool isExpanded() const { return expanded; }
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;

private:
    juce::TextEditor messageLog;
    juce::TextButton toggleButton;
    bool expanded;
    
    juce::StringArray messageHistory;
    static constexpr int maxMessages = 100;
    
    void timerCallback() override;
    void updateMessageDisplay();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIMessageDisplay)
};

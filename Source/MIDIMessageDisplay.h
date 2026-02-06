#pragma once

#include <JuceHeader.h>
#include "StradellaKeyboardMapper.h"

//==============================================================================
/**
    A component that displays MIDI messages in real-time with collapsible functionality.
    Uses asynchronous updates to avoid blocking MIDI output.
*/
class MIDIMessageDisplay : public juce::Component,
                           private juce::Timer
{
public:
    MIDIMessageDisplay();
    ~MIDIMessageDisplay() override;
    
    /** Adds a MIDI message to the display (non-blocking, queued for async update) */
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
    juce::CriticalSection queueLock;
    juce::Array<juce::MidiMessage> pendingMessages;
    static constexpr int maxMessages = 100;
    bool needsUpdate;
    
    void timerCallback() override;
    void updateMessageDisplay();
    void processPendingMessages();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIMessageDisplay)
};

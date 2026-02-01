#include "MIDIMessageDisplay.h"

//==============================================================================
MIDIMessageDisplay::MIDIMessageDisplay()
    : expanded(true)
{
    // Setup message log
    addAndMakeVisible(messageLog);
    messageLog.setMultiLine(true);
    messageLog.setReadOnly(true);
    messageLog.setScrollbarsShown(true);
    messageLog.setCaretVisible(false);
    messageLog.setPopupMenuEnabled(false);
    messageLog.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    messageLog.setColour(juce::TextEditor::textColourId, juce::Colours::lightgreen);
    messageLog.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 12.0f, juce::Font::plain));
    
    // Setup toggle button
    addAndMakeVisible(toggleButton);
    toggleButton.setButtonText("MIDI Messages");
    toggleButton.onClick = [this]() { setExpanded(!expanded); };
    
    setSize(400, 150);
}

MIDIMessageDisplay::~MIDIMessageDisplay()
{
}

void MIDIMessageDisplay::addMidiMessage(const juce::MidiMessage& message)
{
    juce::String messageText;
    
    if (message.isNoteOn())
    {
        messageText = "Note ON:  " + StradellaKeyboardMapper::getMidiNoteName(message.getNoteNumber()) 
                    + " (MIDI: " + juce::String(message.getNoteNumber()) + ")"
                    + " Vel: " + juce::String(message.getVelocity());
    }
    else if (message.isNoteOff())
    {
        messageText = "Note OFF: " + StradellaKeyboardMapper::getMidiNoteName(message.getNoteNumber())
                    + " (MIDI: " + juce::String(message.getNoteNumber()) + ")";
    }
    else
    {
        messageText = "MIDI: " + message.getDescription();
    }
    
    // Add timestamp
    auto now = juce::Time::getCurrentTime();
    messageText = now.formatted("[%H:%M:%S] ") + messageText;
    
    messageHistory.add(messageText);
    
    // Keep only the last maxMessages
    while (messageHistory.size() > maxMessages)
        messageHistory.remove(0);
    
    updateMessageDisplay();
}

void MIDIMessageDisplay::clearMessages()
{
    messageHistory.clear();
    updateMessageDisplay();
}

void MIDIMessageDisplay::setExpanded(bool shouldBeExpanded)
{
    if (expanded != shouldBeExpanded)
    {
        expanded = shouldBeExpanded;
        messageLog.setVisible(expanded);
        resized();
    }
}

void MIDIMessageDisplay::updateMessageDisplay()
{
    juce::String allMessages;
    for (const auto& msg : messageHistory)
    {
        allMessages += msg + "\n";
    }
    
    messageLog.setText(allMessages, false);
    messageLog.moveCaretToEnd();
}

void MIDIMessageDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MIDIMessageDisplay::resized()
{
    auto area = getLocalBounds();
    
    // Toggle button at the top
    toggleButton.setBounds(area.removeFromTop(30).reduced(2));
    
    // Message log below if expanded
    if (expanded)
    {
        messageLog.setBounds(area.reduced(2));
    }
}

void MIDIMessageDisplay::mouseDown(const juce::MouseEvent& event)
{
    // Allow dragging to resize if needed in the future
}

void MIDIMessageDisplay::timerCallback()
{
    // Reserved for future use (e.g., auto-scrolling, fading old messages)
}

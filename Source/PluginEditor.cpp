/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StraDellaMIDIAudioProcessorEditor::StraDellaMIDIAudioProcessorEditor (StraDellaMIDIAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Create GUI components and add as visible child components
    keyboardGUI = std::make_unique<KeyboardGUI>(audioProcessor.getKeyboardMapper());
    addAndMakeVisible(keyboardGUI.get());
    
    midiDisplay = std::make_unique<MIDIMessageDisplay>();
    addAndMakeVisible(midiDisplay.get());
    
    // Create mouse MIDI expression component (no visual component needed)
    mouseMidiExpression = std::make_unique<MouseMidiExpression>();
    mouseMidiExpression->onMidiMessage = [this](const juce::MidiMessage& msg)
    {
        sendMidiMessage(msg);
        
        // Display CC messages in MIDI log
        juce::MessageManager::callAsync([this, msg]()
        {
            if (midiDisplay != nullptr)
                midiDisplay->addMidiMessage(msg);
        });
    };
    
    // Handle direction changes (bellows direction change)
    mouseMidiExpression->onDirectionChange = [this]()
    {
        retriggerCurrentlyPressedKeys();
    };
    
    // Start global mouse tracking
    mouseMidiExpression->startTracking();
    
    // Create mouse settings window (initially hidden)
    mouseSettingsWindow = std::make_unique<MouseMidiSettingsWindow>(*mouseMidiExpression);
    mouseSettingsWindow->setVisible(false);
    addAndMakeVisible(mouseSettingsWindow.get());
    
    // Create settings buttons (bottom bar)
    noteMapSettingsButton.setButtonText("Note Map Settings");
    noteMapSettingsButton.onClick = [this] { showNoteMapSettings(); };
    addAndMakeVisible(noteMapSettingsButton);
    
    midiSettingsButton.setButtonText("MIDI Settings");
    midiSettingsButton.onClick = [this] { showMidiSettings(); };
    addAndMakeVisible(midiSettingsButton);
    
    expressionSettingsButton.setButtonText("Expression Settings");
    expressionSettingsButton.onClick = [this] { toggleMouseSettings(); };
    addAndMakeVisible(expressionSettingsButton);
    
    // Add this component as a key listener to receive keyboard events
    addKeyListener(this);
    setWantsKeyboardFocus(true);
    
    // Set the size of the editor
    setSize (800, 600);
    
    // Ensure this component gets keyboard focus immediately
    grabKeyboardFocus();
}

StraDellaMIDIAudioProcessorEditor::~StraDellaMIDIAudioProcessorEditor()
{
    // Remove key listener
    removeKeyListener(this);
}

//==============================================================================
void StraDellaMIDIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void StraDellaMIDIAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Settings buttons at the bottom (three buttons side by side)
    auto bottomBar = area.removeFromBottom(40);
    const int buttonWidth = 180;
    const int buttonSpacing = 10;
    const int totalButtonWidth = (buttonWidth * 3) + (buttonSpacing * 2);
    const int leftMargin = (bottomBar.getWidth() - totalButtonWidth) / 2;
    
    auto buttonArea = bottomBar.withTrimmedLeft(leftMargin);
    noteMapSettingsButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(2));
    buttonArea.removeFromLeft(buttonSpacing);
    midiSettingsButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(2));
    buttonArea.removeFromLeft(buttonSpacing);
    expressionSettingsButton.setBounds(buttonArea.removeFromLeft(buttonWidth).reduced(2));
    
    // MIDI display at the bottom (above buttons)
    if (midiDisplay != nullptr)
    {
        auto midiArea = area.removeFromBottom(150);
        midiDisplay->setBounds(midiArea);
    }
    
    // Keyboard GUI takes the full remaining space
    if (keyboardGUI != nullptr)
    {
        keyboardGUI->setBounds(area);
    }
    
    // Position settings window in center (when visible)
    if (mouseSettingsWindow != nullptr && mouseSettingsWindow->isVisible())
    {
        mouseSettingsWindow->centreWithSize(400, 350);
    }
}

void StraDellaMIDIAudioProcessorEditor::visibilityChanged()
{
    // Ensure keyboard focus when component becomes visible
    // This handles cases where grabKeyboardFocus() in constructor fails
    if (isVisible() && isShowing())
    {
        grabKeyboardFocus();
    }
}

bool StraDellaMIDIAudioProcessorEditor::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    int keyCode = key.getKeyCode();
    
    // Convert to uppercase for letter keys
    if (keyCode >= 'a' && keyCode <= 'z')
        keyCode = keyCode - 'a' + 'A';
    
    auto& currentlyPressedKeys = audioProcessor.getCurrentlyPressedKeys();
    
    // Check if this key is already pressed to avoid OS key repeat
    if (!currentlyPressedKeys.contains(keyCode))
    {
        handleKeyPress(keyCode);
        currentlyPressedKeys.add(keyCode);
        return true;
    }
    
    // Key is already pressed - this is an OS key repeat, ignore it but return true
    // to indicate we've handled the event (prevents JUCE from processing it further)
    return true;
}

bool StraDellaMIDIAudioProcessorEditor::keyStateChanged(bool isKeyDown, juce::Component* originatingComponent)
{
    // Handle key releases
    if (!isKeyDown)
    {
        auto& currentlyPressedKeys = audioProcessor.getCurrentlyPressedKeys();
        
        // Check which keys were released
        for (int i = currentlyPressedKeys.size() - 1; i >= 0; --i)
        {
            int keyCode = currentlyPressedKeys[i];
            
            // Check if the key is still down
            bool stillDown = false;
            
            // Check common keys (simplified check)
            if (juce::KeyPress::isKeyCurrentlyDown(keyCode))
                stillDown = true;
            
            if (!stillDown)
            {
                handleKeyRelease(keyCode);
                currentlyPressedKeys.remove(i);
            }
        }
    }
    
    return false;
}

void StraDellaMIDIAudioProcessorEditor::handleKeyPress(int keyCode)
{
    bool isValidKey = false;
    auto midiNotes = audioProcessor.getKeyboardMapper().getMidiNotesForKey(keyCode, isValidKey);
    
    if (isValidKey && !midiNotes.isEmpty())
    {
        // Get velocity from MouseMidiExpression based on Y position
        int velocity = 100;  // Default fallback
        if (mouseMidiExpression != nullptr)
        {
            velocity = mouseMidiExpression->getCurrentNoteVelocity();
        }
        
        // CRITICAL PATH: Send ALL MIDI messages immediately with ZERO delays
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOn(defaultMidiChannel, noteNumber, (juce::uint8)velocity);
            sendMidiMessage(message);
        }
        
        // NON-CRITICAL: Update GUI asynchronously (won't block MIDI)
        juce::MessageManager::callAsync([this, keyCode, midiNotes, velocity]()
        {
            if (keyboardGUI != nullptr)
                keyboardGUI->setKeyPressed(keyCode, true);
            
            // Log messages asynchronously (now non-blocking with queue)
            if (midiDisplay != nullptr)
            {
                for (int noteNumber : midiNotes)
                {
                    auto message = juce::MidiMessage::noteOn(defaultMidiChannel, noteNumber, (juce::uint8)velocity);
                    midiDisplay->addMidiMessage(message);
                }
            }
        });
    }
}

void StraDellaMIDIAudioProcessorEditor::handleKeyRelease(int keyCode)
{
    bool isValidKey = false;
    auto midiNotes = audioProcessor.getKeyboardMapper().getMidiNotesForKey(keyCode, isValidKey);
    
    if (isValidKey && !midiNotes.isEmpty())
    {
        // CRITICAL PATH: Send ALL MIDI messages immediately with ZERO delays
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOff(defaultMidiChannel, noteNumber);
            sendMidiMessage(message);
        }
        
        // NON-CRITICAL: Update GUI asynchronously (won't block MIDI)
        juce::MessageManager::callAsync([this, keyCode, midiNotes]()
        {
            if (keyboardGUI != nullptr)
                keyboardGUI->setKeyPressed(keyCode, false);
            
            // Log messages asynchronously (now non-blocking with queue)
            if (midiDisplay != nullptr)
            {
                for (int noteNumber : midiNotes)
                {
                    auto message = juce::MidiMessage::noteOff(defaultMidiChannel, noteNumber);
                    midiDisplay->addMidiMessage(message);
                }
            }
        });
    }
}

void StraDellaMIDIAudioProcessorEditor::retriggerCurrentlyPressedKeys()
{
    // This simulates the bellows changing direction on an accordion
    // All currently pressed keys briefly stop then resume
    
    auto& currentlyPressedKeys = audioProcessor.getCurrentlyPressedKeys();
    
    if (currentlyPressedKeys.isEmpty())
        return;
    
    // Get current velocity from mouse Y position
    int velocity = 100;  // Default fallback
    if (mouseMidiExpression != nullptr)
    {
        velocity = mouseMidiExpression->getCurrentNoteVelocity();
    }
    
    // For each currently pressed key, send note off then note on
    for (int keyCode : currentlyPressedKeys)
    {
        bool isValidKey = false;
        auto midiNotes = audioProcessor.getKeyboardMapper().getMidiNotesForKey(keyCode, isValidKey);
        
        if (isValidKey && !midiNotes.isEmpty())
        {
            // Send note off for all notes
            for (int noteNumber : midiNotes)
            {
                auto offMessage = juce::MidiMessage::noteOff(defaultMidiChannel, noteNumber);
                sendMidiMessage(offMessage);
            }
            
            // Immediately send note on with current velocity
            for (int noteNumber : midiNotes)
            {
                auto onMessage = juce::MidiMessage::noteOn(defaultMidiChannel, noteNumber, (juce::uint8)velocity);
                sendMidiMessage(onMessage);
            }
            
            // NON-CRITICAL: Update display asynchronously
            juce::MessageManager::callAsync([this, midiNotes, velocity]()
            {
                if (midiDisplay != nullptr)
                {
                    for (int noteNumber : midiNotes)
                    {
                        auto offMessage = juce::MidiMessage::noteOff(defaultMidiChannel, noteNumber);
                        auto onMessage = juce::MidiMessage::noteOn(defaultMidiChannel, noteNumber, (juce::uint8)velocity);
                        midiDisplay->addMidiMessage(offMessage);
                        midiDisplay->addMidiMessage(onMessage);
                    }
                }
            });
        }
    }
}

void StraDellaMIDIAudioProcessorEditor::sendMidiMessage(const juce::MidiMessage& message)
{
    // Send MIDI through the processor to the plugin host
    audioProcessor.addMidiMessageToBuffer(message);
}

void StraDellaMIDIAudioProcessorEditor::toggleMouseSettings()
{
    if (mouseSettingsWindow != nullptr)
    {
        bool currentlyVisible = mouseSettingsWindow->isVisible();
        mouseSettingsWindow->setVisible(!currentlyVisible);
        
        if (!currentlyVisible)
        {
            // Center the window when showing
            mouseSettingsWindow->centreWithSize(400, 350);
            mouseSettingsWindow->toFront(true);
        }
    }
}

void StraDellaMIDIAudioProcessorEditor::showNoteMapSettings()
{
    // Placeholder for future Note Map Settings dialog
    // This will be implemented in a future iteration
    juce::Logger::writeToLog("Note Map Settings button clicked - feature coming soon");
}

void StraDellaMIDIAudioProcessorEditor::showMidiSettings()
{
    // Placeholder for future MIDI Settings dialog
    // This will be implemented in a future iteration
    juce::Logger::writeToLog("MIDI Settings button clicked - feature coming soon");
}

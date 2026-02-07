#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Create GUI components and add as visible child components
    keyboardGUI = std::make_unique<KeyboardGUI>(keyboardMapper);
    addAndMakeVisible(keyboardGUI.get());
    
    midiDisplay = std::make_unique<MIDIMessageDisplay>();
    addAndMakeVisible(midiDisplay.get());
    
    // Create mouse MIDI expression component
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
    addAndMakeVisible(mouseMidiExpression.get());
    
    // Create mouse settings window (initially hidden)
    mouseSettingsWindow = std::make_unique<MouseMidiSettingsWindow>(*mouseMidiExpression);
    mouseSettingsWindow->setVisible(false);
    addAndMakeVisible(mouseSettingsWindow.get());
    
    // Create settings button
    mouseSettingsButton.setButtonText("Mouse Settings");
    mouseSettingsButton.onClick = [this] { toggleMouseSettings(); };
    addAndMakeVisible(mouseSettingsButton);
    
    // Setup MIDI output
    auto midiDevices = juce::MidiOutput::getAvailableDevices();
    if (!midiDevices.isEmpty())
    {
        // Try to open the first available MIDI device
        midiOutput = juce::MidiOutput::openDevice(midiDevices[0].identifier);
        
        if (midiOutput != nullptr)
        {
            juce::String msg = "MIDI Output opened: " + midiDevices[0].name;
            juce::Logger::writeToLog(msg);
        }
    }
    
    // Add this component as a key listener to receive keyboard events
    addKeyListener(this);
    setWantsKeyboardFocus(true);
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    // Ensure this component gets keyboard focus immediately
    grabKeyboardFocus();
}

MainComponent::~MainComponent()
{
    // Remove key listener
    removeKeyListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    auto area = getLocalBounds();
    
    // Mouse settings button at top right
    auto topBar = area.removeFromTop(30);
    mouseSettingsButton.setBounds(topBar.removeFromRight(120).reduced(2));
    
    // MIDI display at the bottom
    if (midiDisplay != nullptr)
    {
        auto midiArea = area.removeFromBottom(150);
        midiDisplay->setBounds(midiArea);
    }
    
    // Split remaining space: keyboard on left, mouse expression on right
    auto mouseArea = area.removeFromRight(200);
    
    // Mouse MIDI expression area
    if (mouseMidiExpression != nullptr)
    {
        mouseMidiExpression->setBounds(mouseArea);
    }
    
    // Keyboard GUI takes the remaining space
    if (keyboardGUI != nullptr)
    {
        keyboardGUI->setBounds(area);
    }
    
    // Position settings window in center (when visible)
    if (mouseSettingsWindow != nullptr && mouseSettingsWindow->isVisible())
    {
        mouseSettingsWindow->centreWithSize(400, 300);
    }
}

void MainComponent::visibilityChanged()
{
    // Ensure keyboard focus when component becomes visible
    // This handles cases where grabKeyboardFocus() in constructor fails
    if (isVisible() && isShowing())
    {
        grabKeyboardFocus();
    }
}

bool MainComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    int keyCode = key.getKeyCode();
    
    // Convert to uppercase for letter keys
    if (keyCode >= 'a' && keyCode <= 'z')
        keyCode = keyCode - 'a' + 'A';
    
    // Check if this key is already pressed to avoid repeated triggering
    if (!currentlyPressedKeys.contains(keyCode))
    {
        handleKeyPress(keyCode);
        currentlyPressedKeys.add(keyCode);
        return true;
    }
    
    return false;
}

bool MainComponent::keyStateChanged(bool isKeyDown, juce::Component* originatingComponent)
{
    // Handle key releases
    if (!isKeyDown)
    {
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

void MainComponent::handleKeyPress(int keyCode)
{
    bool isValidKey = false;
    auto midiNotes = keyboardMapper.getMidiNotesForKey(keyCode, isValidKey);
    
    if (isValidKey && !midiNotes.isEmpty())
    {
        // CRITICAL PATH: Send ALL MIDI messages immediately with ZERO delays
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)100);
            sendMidiMessage(message);
        }
        
        // NON-CRITICAL: Update GUI asynchronously (won't block MIDI)
        juce::MessageManager::callAsync([this, keyCode, midiNotes]()
        {
            if (keyboardGUI != nullptr)
                keyboardGUI->setKeyPressed(keyCode, true);
            
            // Log messages asynchronously (now non-blocking with queue)
            if (midiDisplay != nullptr)
            {
                for (int noteNumber : midiNotes)
                {
                    auto message = juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)100);
                    midiDisplay->addMidiMessage(message);
                }
            }
        });
    }
}

void MainComponent::handleKeyRelease(int keyCode)
{
    bool isValidKey = false;
    auto midiNotes = keyboardMapper.getMidiNotesForKey(keyCode, isValidKey);
    
    if (isValidKey && !midiNotes.isEmpty())
    {
        // CRITICAL PATH: Send ALL MIDI messages immediately with ZERO delays
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOff(1, noteNumber);
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
                    auto message = juce::MidiMessage::noteOff(1, noteNumber);
                    midiDisplay->addMidiMessage(message);
                }
            }
        });
    }
}

void MainComponent::sendMidiMessage(const juce::MidiMessage& message)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(message);
    }
}

void MainComponent::toggleMouseSettings()
{
    if (mouseSettingsWindow != nullptr)
    {
        bool currentlyVisible = mouseSettingsWindow->isVisible();
        mouseSettingsWindow->setVisible(!currentlyVisible);
        
        if (!currentlyVisible)
        {
            // Center the window when showing
            mouseSettingsWindow->centreWithSize(400, 300);
            mouseSettingsWindow->toFront(true);
        }
    }
}

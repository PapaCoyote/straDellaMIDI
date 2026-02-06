#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Create GUI components - but initially hide them
    keyboardGUI = std::make_unique<KeyboardGUI>(keyboardMapper);
    keyboardGUI->setVisible(false);  // Hide until audio is ready
    addChildComponent(keyboardGUI.get());  // Use addChildComponent instead of addAndMakeVisible
    
    midiDisplay = std::make_unique<MIDIMessageDisplay>();
    midiDisplay->setVisible(false);  // Hide until audio is ready
    addChildComponent(midiDisplay.get());  // Use addChildComponent instead of addAndMakeVisible
    
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

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // Remove key listener
    removeKeyListener(this);
    
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    // Mark audio as ready and show the GUI components
    juce::MessageManager::callAsync([this]()
    {
        isAudioReady = true;
        if (keyboardGUI != nullptr)
            keyboardGUI->setVisible(true);
        if (midiDisplay != nullptr)
            midiDisplay->setVisible(true);
        repaint();
    });
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Show loading message if audio is not ready
    if (!isAudioReady)
    {
        g.setColour(juce::Colours::white);
        g.setFont(32.0f);
        g.drawText("Loading...", getLocalBounds(), juce::Justification::centred);
    }
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    auto area = getLocalBounds();
    
    // MIDI display at the bottom
    if (midiDisplay != nullptr)
    {
        auto midiArea = area.removeFromBottom(150);
        midiDisplay->setBounds(midiArea);
    }
    
    // Keyboard GUI takes the remaining space
    if (keyboardGUI != nullptr)
    {
        keyboardGUI->setBounds(area);
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
        // Update GUI
        if (keyboardGUI != nullptr)
            keyboardGUI->setKeyPressed(keyCode, true);
        
        // Send MIDI note on messages
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)100);
            sendMidiMessage(message);
            
            // Display in MIDI log
            if (midiDisplay != nullptr)
                midiDisplay->addMidiMessage(message);
        }
    }
}

void MainComponent::handleKeyRelease(int keyCode)
{
    bool isValidKey = false;
    auto midiNotes = keyboardMapper.getMidiNotesForKey(keyCode, isValidKey);
    
    if (isValidKey && !midiNotes.isEmpty())
    {
        // Update GUI
        if (keyboardGUI != nullptr)
            keyboardGUI->setKeyPressed(keyCode, false);
        
        // Send MIDI note off messages
        for (int noteNumber : midiNotes)
        {
            auto message = juce::MidiMessage::noteOff(1, noteNumber);
            sendMidiMessage(message);
            
            // Display in MIDI log
            if (midiDisplay != nullptr)
                midiDisplay->addMidiMessage(message);
        }
    }
}

void MainComponent::sendMidiMessage(const juce::MidiMessage& message)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(message);
    }
}

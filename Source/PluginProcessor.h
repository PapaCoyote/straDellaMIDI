/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StradellaKeyboardMapper.h"

//==============================================================================
/**
*/
class StraDellaMIDIAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    StraDellaMIDIAudioProcessor();
    ~StraDellaMIDIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public methods for editor to access
    StradellaKeyboardMapper& getKeyboardMapper() { return keyboardMapper; }
    
    // MIDI output handling - called by editor
    void addMidiMessageToBuffer(const juce::MidiMessage& message);
    
    // Track currently pressed keys for editor
    juce::Array<int>& getCurrentlyPressedKeys() { return currentlyPressedKeys; }

private:
    //==============================================================================
    StradellaKeyboardMapper keyboardMapper;
    juce::Array<int> currentlyPressedKeys;
    
    // MIDI buffer for messages from the editor
    juce::MidiBuffer pendingMidiMessages;
    juce::CriticalSection midiBufferLock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StraDellaMIDIAudioProcessor)
};

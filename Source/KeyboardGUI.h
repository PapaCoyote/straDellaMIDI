#pragma once

#include <JuceHeader.h>
#include "StradellaKeyboardMapper.h"

//==============================================================================
/**
    Visual representation of a computer keyboard with color feedback for key presses.
*/
class KeyboardGUI : public juce::Component
{
public:
    KeyboardGUI(StradellaKeyboardMapper& mapper);
    ~KeyboardGUI() override;
    
    /** Sets a key as pressed (will show in different color) */
    void setKeyPressed(int keyCode, bool isPressed);
    
    /** Checks if a key is currently pressed */
    bool isKeyPressed(int keyCode) const;
    
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    struct KeyButton
    {
        juce::Rectangle<float> bounds;
        int keyCode;
        juce::String label;
        bool isPressed;
        StradellaKeyboardMapper::KeyType type;
    };
    
    StradellaKeyboardMapper& keyMapper;
    juce::Array<KeyButton> keys;
    
    void setupKeyLayout();
    void drawKey(juce::Graphics& g, const KeyButton& key);
    juce::Colour getColourForKeyType(StradellaKeyboardMapper::KeyType type, bool isPressed) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardGUI)
};

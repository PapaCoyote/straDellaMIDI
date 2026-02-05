#include "KeyboardGUI.h"

//==============================================================================
KeyboardGUI::KeyboardGUI(StradellaKeyboardMapper& mapper)
    : keyMapper(mapper)
{
    setupKeyLayout();
    setSize(800, 300);
}

KeyboardGUI::~KeyboardGUI()
{
}

void KeyboardGUI::setupKeyLayout()
{
    keys.clear();
    
    const float keyWidth = 50.0f;
    const float keyHeight = 50.0f;
    const float spacing = 5.0f;
    const float rowSpacing = 10.0f;
    
    // Row 1: Numbers (1-0) - Minor chords
    juce::Array<int> row1Keys = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
    
    float startX = 80.0f;
    float currentY = 20.0f;
    
    for (int i = 0; i < row1Keys.size(); ++i)
    {
        KeyButton key;
        key.keyCode = row1Keys[i];
        key.label = keyMapper.getKeyDescription(key.keyCode);
        key.isPressed = false;
        key.type = keyMapper.getKeyType(key.keyCode);
        key.bounds = juce::Rectangle<float>(startX + i * (keyWidth + spacing), currentY, keyWidth, keyHeight);
        keys.add(key);
    }
    
    // Row 2: QWERTY (Q-P) - Major chords
    juce::Array<int> row2Keys = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' };
    
    currentY += keyHeight + rowSpacing;
    startX = 90.0f;
    
    for (int i = 0; i < row2Keys.size(); ++i)
    {
        KeyButton key;
        key.keyCode = row2Keys[i];
        key.label = keyMapper.getKeyDescription(key.keyCode);
        key.isPressed = false;
        key.type = keyMapper.getKeyType(key.keyCode);
        key.bounds = juce::Rectangle<float>(startX + i * (keyWidth + spacing), currentY, keyWidth, keyHeight);
        keys.add(key);
    }
    
    // Row 3: ASDF (A-') - Single notes (cycle of fifths)
    juce::Array<int> row3Keys = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'' };
    
    currentY += keyHeight + rowSpacing;
    startX = 100.0f;
    
    for (int i = 0; i < row3Keys.size(); ++i)
    {
        KeyButton key;
        key.keyCode = row3Keys[i];
        key.label = keyMapper.getKeyDescription(key.keyCode);
        key.isPressed = false;
        key.type = keyMapper.getKeyType(key.keyCode);
        key.bounds = juce::Rectangle<float>(startX + i * (keyWidth + spacing), currentY, keyWidth, keyHeight);
        keys.add(key);
    }
    
    // Row 4: ZXCV (Z-/) - Third notes
    juce::Array<int> row4Keys = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/' };
    
    currentY += keyHeight + rowSpacing;
    startX = 120.0f;
    
    for (int i = 0; i < row4Keys.size(); ++i)
    {
        KeyButton key;
        key.keyCode = row4Keys[i];
        key.label = keyMapper.getKeyDescription(key.keyCode);
        key.isPressed = false;
        key.type = keyMapper.getKeyType(key.keyCode);
        key.bounds = juce::Rectangle<float>(startX + i * (keyWidth + spacing), currentY, keyWidth, keyHeight);
        keys.add(key);
    }
}

void KeyboardGUI::setKeyPressed(int keyCode, bool isPressed)
{
    for (auto& key : keys)
    {
        if (key.keyCode == keyCode)
        {
            key.isPressed = isPressed;
            repaint();
            break;
        }
    }
}

bool KeyboardGUI::isKeyPressed(int keyCode) const
{
    for (const auto& key : keys)
    {
        if (key.keyCode == keyCode)
            return key.isPressed;
    }
    return false;
}

void KeyboardGUI::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colours::darkgrey.darker());
    
    // Draw all keys
    for (const auto& key : keys)
    {
        drawKey(g, key);
    }
    
    // Draw labels for key rows
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText("Minor Chords:", 10, 30, 70, 20, juce::Justification::centredRight);
    g.drawText("Major Chords:", 10, 90, 70, 20, juce::Justification::centredRight);
    g.drawText("Single Notes:", 10, 150, 70, 20, juce::Justification::centredRight);
    g.drawText("Third Notes:", 10, 210, 70, 20, juce::Justification::centredRight);
}

void KeyboardGUI::resized()
{
    // Keys are laid out in setupKeyLayout()
}

void KeyboardGUI::drawKey(juce::Graphics& g, const KeyButton& key)
{
    // Get color based on key type and state
    juce::Colour keyColour = getColourForKeyType(key.type, key.isPressed);
    
    // Draw key background
    g.setColour(keyColour);
    g.fillRoundedRectangle(key.bounds, 5.0f);
    
    // Draw key border
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(key.bounds, 5.0f, 2.0f);
    
    // Draw key label
    g.setColour(key.isPressed ? juce::Colours::white : juce::Colours::lightgrey);
    g.setFont(16.0f);
    g.drawText(key.label, key.bounds, juce::Justification::centred);
}

juce::Colour KeyboardGUI::getColourForKeyType(StradellaKeyboardMapper::KeyType type, bool isPressed) const
{
    juce::Colour baseColour;
    
    switch (type)
    {
        case StradellaKeyboardMapper::KeyType::SingleNote:
            baseColour = juce::Colours::blue;
            break;
        case StradellaKeyboardMapper::KeyType::ThirdNote:
            baseColour = juce::Colours::cyan;
            break;
        case StradellaKeyboardMapper::KeyType::MajorChord:
            baseColour = juce::Colours::green;
            break;
        case StradellaKeyboardMapper::KeyType::MinorChord:
            baseColour = juce::Colours::orange;
            break;
        default:
            baseColour = juce::Colours::grey;
            break;
    }
    
    // Brighten the color when pressed
    if (isPressed)
        return baseColour.brighter(0.5f);
    else
        return baseColour.darker(0.3f);
}

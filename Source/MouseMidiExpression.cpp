#include "MouseMidiExpression.h"

//==============================================================================
MouseMidiExpression::MouseMidiExpression()
{
    // Enable mouse events and set cursor
    setMouseCursor(juce::MouseCursor::CrosshairCursor);
    
    // Initialize positions
    lastMousePosition = juce::Point<float>(0.0f, 0.0f);
    currentMousePosition = juce::Point<float>(0.0f, 0.0f);
    lastMouseTime = juce::Time::currentTimeMillis();
}

MouseMidiExpression::~MouseMidiExpression()
{
}

//==============================================================================
void MouseMidiExpression::paint(juce::Graphics& g)
{
    // Draw a subtle background to indicate the active area
    g.fillAll(juce::Colours::black.withAlpha(0.05f));
    
    // Draw border
    g.setColour(juce::Colours::grey.withAlpha(0.3f));
    g.drawRect(getLocalBounds(), 1);
    
    // Draw crosshair at current mouse position if within bounds
    if (getLocalBounds().toFloat().contains(currentMousePosition))
    {
        g.setColour(juce::Colours::lightblue.withAlpha(0.5f));
        
        // Vertical line
        g.drawLine(currentMousePosition.x, 0.0f, 
                   currentMousePosition.x, (float)getHeight(), 1.0f);
        
        // Horizontal line
        g.drawLine(0.0f, currentMousePosition.y, 
                   (float)getWidth(), currentMousePosition.y, 1.0f);
    }
    
    // Draw info text
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(12.0f);
    
    juce::String infoText = "Mouse MIDI Expression: ";
    if (volumeEnabled && expressionEnabled)
        infoText += "CC7 (Volume) + CC11 (Expression)";
    else if (volumeEnabled)
        infoText += "CC7 (Volume) only";
    else if (expressionEnabled)
        infoText += "CC11 (Expression) only";
    else
        infoText += "Disabled";
    
    g.drawText(infoText, getLocalBounds().reduced(5), 
               juce::Justification::topLeft, true);
}

void MouseMidiExpression::resized()
{
    // Component has been resized
}

//==============================================================================
void MouseMidiExpression::mouseMove(const juce::MouseEvent& event)
{
    // Handle movement
    processMouseMovement(event);
}

void MouseMidiExpression::mouseDrag(const juce::MouseEvent& event)
{
    // Handle dragging
    processMouseMovement(event);
}

void MouseMidiExpression::mouseDown(const juce::MouseEvent& event)
{
    // Reset tracking when mouse is pressed
    lastMousePosition = event.position;
    currentMousePosition = event.position;
    lastMouseTime = juce::Time::currentTimeMillis();
}

void MouseMidiExpression::mouseUp(const juce::MouseEvent& event)
{
    // Could optionally reset expression values here
}

//==============================================================================
void MouseMidiExpression::processMouseMovement(const juce::MouseEvent& event)
{
    currentMousePosition = event.position;
    juce::int64 currentTime = juce::Time::currentTimeMillis();
    juce::int64 timeDelta = currentTime - lastMouseTime;
    
    // Avoid division by zero
    if (timeDelta <= 0)
        timeDelta = 1;
    
    // Calculate velocity from mouse movement (for CC7 - Volume)
    if (volumeEnabled)
    {
        float velocity = calculateVelocity(lastMousePosition, currentMousePosition, timeDelta);
        
        // Normalize velocity to 0.0 - 1.0 range using max velocity constant
        float normalizedVelocity = juce::jlimit(0.0f, 1.0f, velocity / maxVelocityPixelsPerSecond);
        
        // Apply curve
        float curvedValue = applyCurve(normalizedVelocity);
        
        // Convert to MIDI value (0-127)
        int midiValue = (int)(curvedValue * 127.0f);
        
        // Only send if value changed significantly
        if (std::abs(midiValue - lastVolumeValue) >= 1)
        {
            sendVolumeCC(midiValue);
            lastVolumeValue = midiValue;
        }
    }
    
    // Calculate expression from Y position (for CC11 - Expression)
    if (expressionEnabled)
    {
        float height = (float)getHeight();
        if (height > 0)
        {
            // Y position: top = 0 (high expression), bottom = max (low expression)
            // Invert so that top = high value, bottom = low value
            float normalizedY = 1.0f - (currentMousePosition.y / height);
            normalizedY = juce::jlimit(0.0f, 1.0f, normalizedY);
            
            // Apply curve
            float curvedValue = applyCurve(normalizedY);
            
            // Convert to MIDI value (0-127)
            int midiValue = (int)(curvedValue * 127.0f);
            
            // Only send if value changed significantly
            if (std::abs(midiValue - lastExpressionValue) >= 1)
            {
                sendExpressionCC(midiValue);
                lastExpressionValue = midiValue;
            }
        }
    }
    
    // Update tracking variables
    lastMousePosition = currentMousePosition;
    lastMouseTime = currentTime;
    
    // Request repaint to update crosshair
    repaint();
}

float MouseMidiExpression::calculateVelocity(const juce::Point<float>& from, 
                                             const juce::Point<float>& to, 
                                             juce::int64 timeDelta)
{
    // Calculate distance moved
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    // Calculate velocity in pixels per second
    float timeInSeconds = timeDelta / 1000.0f;
    if (timeInSeconds > 0)
        return distance / timeInSeconds;
    
    return 0.0f;
}

float MouseMidiExpression::applyCurve(float normalizedValue) const
{
    normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);
    
    switch (curveType)
    {
        case CurveType::Linear:
            return normalizedValue;
            
        case CurveType::Exponential:
            // Exponential curve: x^2
            return normalizedValue * normalizedValue;
            
        case CurveType::Logarithmic:
            // Logarithmic curve: approximated with sqrt
            return std::sqrt(normalizedValue);
            
        default:
            return normalizedValue;
    }
}

void MouseMidiExpression::sendVolumeCC(int value)
{
    value = juce::jlimit(0, 127, value);
    
    if (onMidiMessage)
    {
        auto message = juce::MidiMessage::controllerEvent(1, 7, value);  // CC7 = Volume
        onMidiMessage(message);
    }
}

void MouseMidiExpression::sendExpressionCC(int value)
{
    value = juce::jlimit(0, 127, value);
    
    if (onMidiMessage)
    {
        auto message = juce::MidiMessage::controllerEvent(1, 11, value);  // CC11 = Expression
        onMidiMessage(message);
    }
}

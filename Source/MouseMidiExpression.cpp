#include "MouseMidiExpression.h"

//==============================================================================
MouseMidiExpression::MouseMidiExpression()
{
    // Initialize positions
    lastMousePosition = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition().toInt();
    currentMousePosition = lastMousePosition;
    lastMouseTime = juce::Time::currentTimeMillis();
    
    // Get desktop bounds for expression calculation
    screenBounds = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea;
}

MouseMidiExpression::~MouseMidiExpression()
{
    stopTracking();
}

void MouseMidiExpression::startTracking()
{
    // Get the desktop component for global mouse tracking
    desktopComponent = &juce::Desktop::getInstance().getComponent(0);
    if (desktopComponent != nullptr)
    {
        desktopComponent->addMouseListener(this, true);
    }
    
    // Start timer to poll mouse position (fallback for global tracking)
    startTimer(16); // ~60 Hz polling
}

void MouseMidiExpression::stopTracking()
{
    stopTimer();
    
    if (desktopComponent != nullptr)
    {
        desktopComponent->removeMouseListener(this);
        desktopComponent = nullptr;
    }
}

void MouseMidiExpression::timerCallback()
{
    // Poll mouse position globally
    auto mousePos = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition().toInt();
    
    // Only process if position changed
    if (mousePos != currentMousePosition)
    {
        processMouseMovement(mousePos);
    }
}

//==============================================================================
void MouseMidiExpression::mouseMove(const juce::MouseEvent& event)
{
    processMouseMovement(event.getScreenPosition().toInt());
}

void MouseMidiExpression::mouseDrag(const juce::MouseEvent& event)
{
    processMouseMovement(event.getScreenPosition().toInt());
}

void MouseMidiExpression::mouseDown(const juce::MouseEvent& event)
{
    // Reset tracking when mouse is pressed
    auto pos = event.getScreenPosition().toInt();
    lastMousePosition = pos;
    currentMousePosition = pos;
    lastMouseTime = juce::Time::currentTimeMillis();
}

void MouseMidiExpression::mouseUp(const juce::MouseEvent& event)
{
    // Could optionally reset expression values here
}

//==============================================================================
void MouseMidiExpression::processMouseMovement(const juce::Point<int>& mousePos)
{
    currentMousePosition = mousePos;
    juce::int64 currentTime = juce::Time::currentTimeMillis();
    juce::int64 timeDelta = currentTime - lastMouseTime;
    
    // Avoid division by zero
    if (timeDelta <= 0)
        timeDelta = 1;
    
    // Calculate velocity from mouse movement (for CC1 - Modulation Wheel)
    if (modulationEnabled)
    {
        float velocity = calculateVelocity(lastMousePosition, currentMousePosition, timeDelta);
        
        // Normalize velocity to 0.0 - 1.0 range using max velocity constant
        float normalizedVelocity = juce::jlimit(0.0f, 1.0f, velocity / maxVelocityPixelsPerSecond);
        
        // Apply curve
        float curvedValue = applyCurve(normalizedVelocity);
        
        // Convert to MIDI value (0-127)
        int midiValue = (int)(curvedValue * 127.0f);
        
        // Only send if value changed significantly
        if (std::abs(midiValue - lastModulationValue) >= 1)
        {
            sendModulationCC(midiValue);
            lastModulationValue = midiValue;
        }
    }
    
    // Calculate expression from Y position (for CC11 - Expression)
    if (expressionEnabled)
    {
        float height = (float)screenBounds.getHeight();
        if (height > 0)
        {
            // Y position relative to screen
            float relativeY = currentMousePosition.y - screenBounds.getY();
            
            // Y position: top = 0 (high expression), bottom = max (low expression)
            // Invert so that top = high value, bottom = low value
            float normalizedY = 1.0f - (relativeY / height);
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
}

float MouseMidiExpression::calculateVelocity(const juce::Point<int>& from, 
                                             const juce::Point<int>& to, 
                                             juce::int64 timeDelta)
{
    // Calculate distance moved
    float dx = (float)(to.x - from.x);
    float dy = (float)(to.y - from.y);
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

void MouseMidiExpression::sendModulationCC(int value)
{
    value = juce::jlimit(0, 127, value);
    
    if (onMidiMessage)
    {
        // CC1 = Modulation Wheel, using channel 1 (MIDI channels are 1-based in the API)
        auto message = juce::MidiMessage::controllerEvent(1, 1, value);
        onMidiMessage(message);
    }
}

void MouseMidiExpression::sendExpressionCC(int value)
{
    value = juce::jlimit(0, 127, value);
    
    if (onMidiMessage)
    {
        // CC11 = Expression, using channel 1 (MIDI channels are 1-based in the API)
        auto message = juce::MidiMessage::controllerEvent(1, 11, value);
        onMidiMessage(message);
    }
}

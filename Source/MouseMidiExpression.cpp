#include "MouseMidiExpression.h"

//==============================================================================
MouseMidiExpression::MouseMidiExpression()
{
    // Initialize positions
    lastMousePosition = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition().toInt();
    currentMousePosition = lastMousePosition;
    lastMouseTime = juce::Time::currentTimeMillis();
    lastXMovementTime = lastMouseTime;
    
    // Get desktop bounds for expression calculation
    screenBounds = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea;
    
    // Initialize note velocity based on starting Y position
    currentNoteVelocity = calculateVelocityFromYPosition(lastMousePosition.y);
}

MouseMidiExpression::~MouseMidiExpression()
{
    stopTracking();
}

void MouseMidiExpression::startTracking()
{
    // Start timer to poll mouse position globally
    startTimer(16); // ~60 Hz polling
}

void MouseMidiExpression::stopTracking()
{
    stopTimer();
}

void MouseMidiExpression::timerCallback()
{
    // Poll mouse position globally
    auto mousePos = juce::Desktop::getInstance().getMainMouseSource().getScreenPosition().toInt();
    
    // Process on position change OR to handle CC decay
    if (mousePos != currentMousePosition)
    {
        processMouseMovement(mousePos);
    }
    else
    {
        // Even if mouse hasn't moved, process to handle CC decay
        juce::int64 currentTime = juce::Time::currentTimeMillis();
        juce::int64 timeSinceLastXMovement = currentTime - lastXMovementTime;
        
        // If we should be decaying, process movement with same position
        if (timeSinceLastXMovement > decayTimeMs && (lastModulationValue > 0 || lastExpressionValue > 0))
        {
            processMouseMovement(mousePos);
        }
    }
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
    
    // Calculate note velocity from Y position (127 at top, 0 at bottom)
    currentNoteVelocity = calculateVelocityFromYPosition(mousePos.y);
    
    // Calculate X movement
    int deltaX = currentMousePosition.x - lastMousePosition.x;
    bool isMovingInX = std::abs(deltaX) > 0;
    
    // Update direction and detect changes if moving in X
    if (isMovingInX)
    {
        bool newDirection = (deltaX > 0);
        
        // Detect direction change only if we were moving in the previous frame
        if (wasMovingInLastFrame && newDirection != isMovingRight)
        {
            // Direction changed! Trigger note retrigger callback
            if (onDirectionChange)
            {
                onDirectionChange();
            }
        }
        
        // Update direction
        isMovingRight = newDirection;
        lastXMovementTime = currentTime;
        wasMovingInLastFrame = true;
    }
    
    // Check if we should decay CC values (no X movement for decay time)
    juce::int64 timeSinceLastXMovement = currentTime - lastXMovementTime;
    bool shouldDecay = timeSinceLastXMovement > decayTimeMs;
    
    // Calculate CC values based on Y position, but only when moving in X
    int ccValue = 0;
    if (isMovingInX)
    {
        // Use Y position for CC values (same as note velocity)
        ccValue = currentNoteVelocity;
        
        // Apply curve
        float normalized = ccValue / 127.0f;
        float curved = applyCurve(normalized);
        ccValue = (int)(curved * 127.0f);
    }
    else if (shouldDecay)
    {
        // Smooth decay to 0
        float decayFactor = 1.0f - juce::jlimit(0.0f, 1.0f, 
            (timeSinceLastXMovement - decayTimeMs) / 200.0f); // 200ms decay time
        
        ccValue = (int)(lastModulationValue * decayFactor);
        wasMovingInLastFrame = false;
    }
    else
    {
        // Keep last value briefly
        ccValue = lastModulationValue;
    }
    
    // Send CC1 (Modulation Wheel) if enabled
    if (modulationEnabled)
    {
        // Only send if value changed significantly
        if (std::abs(ccValue - lastModulationValue) >= 1)
        {
            sendModulationCC(ccValue);
            lastModulationValue = ccValue;
        }
    }
    
    // Send CC11 (Expression) with same value if enabled
    if (expressionEnabled)
    {
        // Only send if value changed significantly
        if (std::abs(ccValue - lastExpressionValue) >= 1)
        {
            sendExpressionCC(ccValue);
            lastExpressionValue = ccValue;
        }
    }
    
    // Update tracking variables
    lastMousePosition = currentMousePosition;
    lastMouseTime = currentTime;
}

int MouseMidiExpression::calculateVelocityFromYPosition(int yPos) const
{
    // Map Y position to velocity: top of screen (y=0) = 127, bottom = 0
    float normalizedY = (float)yPos / (float)screenBounds.getHeight();
    normalizedY = juce::jlimit(0.0f, 1.0f, normalizedY);
    
    // Invert: top = 127, bottom = 0
    int velocity = (int)((1.0f - normalizedY) * 127.0f);
    return juce::jlimit(0, 127, velocity);
}

float MouseMidiExpression::calculateXVelocity(const juce::Point<int>& from, 
                                              const juce::Point<int>& to, 
                                              juce::int64 timeDelta)
{
    // Calculate X distance moved
    float dx = (float)(to.x - from.x);
    float distance = std::abs(dx);
    
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
        
        // Debug log to verify message is being created
        juce::Logger::writeToLog("Sending CC1 (Modulation): value=" + juce::String(value) + 
                                  " channel=" + juce::String(message.getChannel()) +
                                  " controller=" + juce::String(message.getControllerNumber()) +
                                  " controllerValue=" + juce::String(message.getControllerValue()));
        
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
        
        // Debug log to verify message is being created
        juce::Logger::writeToLog("Sending CC11 (Expression): value=" + juce::String(value) + 
                                  " channel=" + juce::String(message.getChannel()) +
                                  " controller=" + juce::String(message.getControllerNumber()) +
                                  " controllerValue=" + juce::String(message.getControllerValue()));
        
        onMidiMessage(message);
    }
}

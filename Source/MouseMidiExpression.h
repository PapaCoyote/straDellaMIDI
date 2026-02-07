#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Handles mouse-based MIDI expression control, emulating accordion bellows.
    - Mouse velocity affects CC1 (Modulation Wheel)
    - Mouse Y position affects CC11 (Expression)
    
    Uses global mouse tracking to monitor movement across the entire desktop.
*/
class MouseMidiExpression : public juce::MouseListener,
                            private juce::Timer
{
public:
    //==============================================================================
    /** Curve types for mapping mouse movement to MIDI values */
    enum class CurveType
    {
        Linear,
        Exponential,
        Logarithmic
    };
    
    //==============================================================================
    MouseMidiExpression();
    ~MouseMidiExpression() override;
    
    /** Sets whether CC1 (Modulation Wheel) is enabled */
    void setModulationEnabled(bool enabled) { modulationEnabled = enabled; }
    
    /** Sets whether CC11 (Expression) is enabled */
    void setExpressionEnabled(bool enabled) { expressionEnabled = enabled; }
    
    /** Sets the curve type for mapping */
    void setCurveType(CurveType type) { curveType = type; }
    
    /** Gets the current modulation enabled state */
    bool isModulationEnabled() const { return modulationEnabled; }
    
    /** Gets the current expression enabled state */
    bool isExpressionEnabled() const { return expressionEnabled; }
    
    /** Gets the current curve type */
    CurveType getCurveType() const { return curveType; }
    
    /** Callback for MIDI message output */
    std::function<void(const juce::MidiMessage&)> onMidiMessage;
    
    /** Starts global mouse tracking */
    void startTracking();
    
    /** Stops global mouse tracking */
    void stopTracking();

private:
    //==============================================================================
    // MouseListener interface
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    
    // Timer callback for polling mouse position
    void timerCallback() override;
    
    //==============================================================================
    bool modulationEnabled = true;      // CC1 enabled by default
    bool expressionEnabled = true;      // CC11 enabled by default
    CurveType curveType = CurveType::Linear;
    
    // Velocity scaling constants
    static constexpr float maxVelocityPixelsPerSecond = 2000.0f;  // Max velocity for normalization
    
    juce::Point<int> lastMousePosition;
    juce::Point<int> currentMousePosition;
    juce::int64 lastMouseTime = 0;
    
    int lastModulationValue = 64;   // Last sent CC1 value (0-127)
    int lastExpressionValue = 64;   // Last sent CC11 value (0-127)
    
    juce::Component* desktopComponent = nullptr;
    juce::Rectangle<int> screenBounds;
    
    //==============================================================================
    /** Processes mouse movement and generates MIDI messages */
    void processMouseMovement(const juce::Point<int>& mousePos);
    
    /** Calculates velocity from mouse movement */
    float calculateVelocity(const juce::Point<int>& from, const juce::Point<int>& to, 
                           juce::int64 timeDelta);
    
    /** Applies curve to a normalized value (0.0 to 1.0) */
    float applyCurve(float normalizedValue) const;
    
    /** Sends CC1 (Modulation Wheel) MIDI message */
    void sendModulationCC(int value);
    
    /** Sends CC11 (Expression) MIDI message */
    void sendExpressionCC(int value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MouseMidiExpression)
};

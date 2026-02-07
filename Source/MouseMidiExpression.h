#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Handles mouse-based MIDI expression control, emulating accordion bellows.
    - Mouse velocity affects CC7 (Volume)
    - Mouse Y position affects CC11 (Expression)
*/
class MouseMidiExpression : public juce::Component,
                            private juce::MouseListener
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
    
    /** Sets whether CC7 (Volume) is enabled */
    void setVolumeEnabled(bool enabled) { volumeEnabled = enabled; }
    
    /** Sets whether CC11 (Expression) is enabled */
    void setExpressionEnabled(bool enabled) { expressionEnabled = enabled; }
    
    /** Sets the curve type for mapping */
    void setCurveType(CurveType type) { curveType = type; }
    
    /** Gets the current volume enabled state */
    bool isVolumeEnabled() const { return volumeEnabled; }
    
    /** Gets the current expression enabled state */
    bool isExpressionEnabled() const { return expressionEnabled; }
    
    /** Gets the current curve type */
    CurveType getCurveType() const { return curveType; }
    
    /** Callback for MIDI message output */
    std::function<void(const juce::MidiMessage&)> onMidiMessage;
    
    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // MouseListener interface
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    
    //==============================================================================
    bool volumeEnabled = true;      // CC7 enabled by default
    bool expressionEnabled = true;   // CC11 enabled by default
    CurveType curveType = CurveType::Linear;
    
    // Velocity scaling constants
    static constexpr float maxVelocityPixelsPerSecond = 2000.0f;  // Max velocity for normalization
    
    juce::Point<float> lastMousePosition;
    juce::Point<float> currentMousePosition;
    juce::int64 lastMouseTime = 0;
    
    int lastVolumeValue = 64;      // Last sent CC7 value (0-127)
    int lastExpressionValue = 64;   // Last sent CC11 value (0-127)
    
    //==============================================================================
    /** Processes mouse movement and generates MIDI messages */
    void processMouseMovement(const juce::MouseEvent& event);
    
    /** Calculates velocity from mouse movement */
    float calculateVelocity(const juce::Point<float>& from, const juce::Point<float>& to, 
                           juce::int64 timeDelta);
    
    /** Applies curve to a normalized value (0.0 to 1.0) */
    float applyCurve(float normalizedValue) const;
    
    /** Sends CC7 (Volume) MIDI message */
    void sendVolumeCC(int value);
    
    /** Sends CC11 (Expression) MIDI message */
    void sendExpressionCC(int value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MouseMidiExpression)
};

#include "MouseMidiSettingsWindow.h"

//==============================================================================
MouseMidiSettingsWindow::MouseMidiSettingsWindow(MouseMidiExpression& midiExpression)
    : mouseMidiExpression(midiExpression)
{
    setupUI();
    setSize(400, 400);  // Increased height to accommodate new slider
}

MouseMidiSettingsWindow::~MouseMidiSettingsWindow()
{
}

//==============================================================================
void MouseMidiSettingsWindow::setupUI()
{
    // Title
    titleLabel.setText("Expression Settings", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // CC1 (Modulation Wheel) checkbox
    modulationLabel.setText("CC1 (Modulation) - Mouse Velocity:", juce::dontSendNotification);
    addAndMakeVisible(modulationLabel);
    
    modulationCheckbox.setToggleState(mouseMidiExpression.isModulationEnabled(), juce::dontSendNotification);
    modulationCheckbox.onClick = [this]
    {
        mouseMidiExpression.setModulationEnabled(modulationCheckbox.getToggleState());
    };
    addAndMakeVisible(modulationCheckbox);
    
    // CC11 (Expression) checkbox
    expressionLabel.setText("CC11 (Expression) - Mouse Velocity:", juce::dontSendNotification);
    addAndMakeVisible(expressionLabel);
    
    expressionCheckbox.setToggleState(mouseMidiExpression.isExpressionEnabled(), juce::dontSendNotification);
    expressionCheckbox.onClick = [this]
    {
        mouseMidiExpression.setExpressionEnabled(expressionCheckbox.getToggleState());
    };
    addAndMakeVisible(expressionCheckbox);
    
    // Curve selector
    curveLabel.setText("Response Curve:", juce::dontSendNotification);
    addAndMakeVisible(curveLabel);
    
    curveSelector.addItem("Linear", 1);
    curveSelector.addItem("Exponential", 2);
    curveSelector.addItem("Logarithmic", 3);
    
    // Set current curve
    switch (mouseMidiExpression.getCurveType())
    {
        case MouseMidiExpression::CurveType::Linear:
            curveSelector.setSelectedId(1, juce::dontSendNotification);
            break;
        case MouseMidiExpression::CurveType::Exponential:
            curveSelector.setSelectedId(2, juce::dontSendNotification);
            break;
        case MouseMidiExpression::CurveType::Logarithmic:
            curveSelector.setSelectedId(3, juce::dontSendNotification);
            break;
    }
    
    curveSelector.onChange = [this]
    {
        int selectedId = curveSelector.getSelectedId();
        switch (selectedId)
        {
            case 1:
                mouseMidiExpression.setCurveType(MouseMidiExpression::CurveType::Linear);
                break;
            case 2:
                mouseMidiExpression.setCurveType(MouseMidiExpression::CurveType::Exponential);
                break;
            case 3:
                mouseMidiExpression.setCurveType(MouseMidiExpression::CurveType::Logarithmic);
                break;
        }
    };
    addAndMakeVisible(curveSelector);
    
    // Base Note Velocity slider
    velocityLabel.setText("Key Press Velocity (0=silent):", juce::dontSendNotification);
    addAndMakeVisible(velocityLabel);
    
    velocitySlider.setRange(0.0, 127.0, 1.0);
    velocitySlider.setValue(mouseMidiExpression.getBaseNoteVelocity(), juce::dontSendNotification);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    velocitySlider.onValueChange = [this]
    {
        mouseMidiExpression.setBaseNoteVelocity((int)velocitySlider.getValue());
    };
    addAndMakeVisible(velocitySlider);
    
    // Close button
    closeButton.setButtonText("Close");
    closeButton.onClick = [this]
    {
        // Hide this window
        setVisible(false);
    };
    addAndMakeVisible(closeButton);
}

void MouseMidiSettingsWindow::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    
    // Draw a border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2);
    
    // Draw some info text
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(11.0f);
    
    juce::String infoText = 
        "Configure how mouse movement affects MIDI expression.\n\n"
        "CC1 (Modulation): Controlled by mouse movement speed\n"
        "CC11 (Expression): Controlled by mouse movement speed\n\n"
        "Key Press Velocity: Accordions don't make sound without\n"
        "bellows movement. Set to 0 for authentic behavior, or\n"
        "increase for more expressiveness.\n\n"
        "Curve affects how values respond to input.\n"
        "Mouse tracking is global across entire desktop.";
    
    auto infoArea = getLocalBounds().reduced(20);
    infoArea.removeFromTop(240);  // Adjusted for taller window
    
    g.drawMultiLineText(infoText, infoArea.getX(), infoArea.getY(), 
                        infoArea.getWidth(), juce::Justification::left);
}

void MouseMidiSettingsWindow::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    // Title
    titleLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(10);
    
    // CC1 Modulation setting
    auto modulationArea = area.removeFromTop(25);
    modulationCheckbox.setBounds(modulationArea.removeFromLeft(25));
    modulationLabel.setBounds(modulationArea);
    area.removeFromTop(10);
    
    // CC11 Expression setting
    auto expressionArea = area.removeFromTop(25);
    expressionCheckbox.setBounds(expressionArea.removeFromLeft(25));
    expressionLabel.setBounds(expressionArea);
    area.removeFromTop(10);
    
    // Curve selector
    auto curveArea = area.removeFromTop(25);
    curveLabel.setBounds(curveArea.removeFromLeft(120));
    curveSelector.setBounds(curveArea.reduced(5, 0));
    area.removeFromTop(10);
    
    // Velocity slider
    auto velocityArea = area.removeFromTop(25);
    velocityLabel.setBounds(velocityArea.removeFromLeft(180));
    velocitySlider.setBounds(velocityArea.reduced(5, 0));
    area.removeFromTop(20);  // Extra space before button
    
    // Close button at bottom - positioned to avoid overlap
    auto buttonArea = getLocalBounds().reduced(20);
    buttonArea = buttonArea.removeFromBottom(40);
    closeButton.setBounds(buttonArea.withSizeKeepingCentre(100, 30));
}

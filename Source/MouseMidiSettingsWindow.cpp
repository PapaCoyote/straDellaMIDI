#include "MouseMidiSettingsWindow.h"

//==============================================================================
MouseMidiSettingsWindow::MouseMidiSettingsWindow(MouseMidiExpression& midiExpression)
    : mouseMidiExpression(midiExpression)
{
    setupUI();
    setSize(400, 300);
}

MouseMidiSettingsWindow::~MouseMidiSettingsWindow()
{
}

//==============================================================================
void MouseMidiSettingsWindow::setupUI()
{
    // Title
    titleLabel.setText("Mouse MIDI Expression Settings", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // CC1 (Modulation Wheel) checkbox
    modulationLabel.setText("CC1 (Modulation Wheel) - Mouse Velocity:", juce::dontSendNotification);
    addAndMakeVisible(modulationLabel);
    
    modulationCheckbox.setToggleState(mouseMidiExpression.isModulationEnabled(), juce::dontSendNotification);
    modulationCheckbox.onClick = [this]
    {
        mouseMidiExpression.setModulationEnabled(modulationCheckbox.getToggleState());
    };
    addAndMakeVisible(modulationCheckbox);
    
    // CC11 (Expression) checkbox
    expressionLabel.setText("CC11 (Expression) - Y Position:", juce::dontSendNotification);
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
        "CC11 (Expression): Controlled by mouse Y position\n\n"
        "Curve affects how values respond to input.\n\n"
        "Mouse tracking is global across entire desktop.";
    
    auto infoArea = getLocalBounds().reduced(20);
    infoArea.removeFromTop(180);
    
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
    
    // Close button at bottom
    auto buttonArea = getLocalBounds().reduced(20);
    buttonArea = buttonArea.removeFromBottom(30);
    closeButton.setBounds(buttonArea.withSizeKeepingCentre(100, 30));
}

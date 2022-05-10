/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"




class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
    }
    //custom vector graphics for rotary dial
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider&) override
    {
        auto radius = jmin (width / 2, height / 2) - 4.0f;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        //hard coded offsets to get the right rotation for the gui
        auto angle = rotaryStartAngle+0.075 + sliderPos * ((rotaryEndAngle - rotaryStartAngle)*0.96);

        
      
        // fill
        g.setColour (Colours::darkgrey);
        g.fillEllipse (rx, ry, rw, rw);

        // outline
        g.setColour (Colours::black);
        g.drawEllipse (rx, ry, rw, rw, 1.0f);

        Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

        // pointer
        g.setColour (Colours::darkorange);
        g.fillPath (p);
    }
    
    //custom graphics for number indicator
    Label* createSliderTextBox (Slider& slider) override
    {
        Label* l = LookAndFeel_V4::createSliderTextBox (slider);
        //remove border around indicator
        l->setColour (Label::outlineColourId, Colours::transparentWhite );
        l->setColour (Label::textColourId, Colours::white );
        return l;
    }
  
};

class JVerbAudioProcessorEditor  : public
AudioProcessorEditor
{
public:
    JVerbAudioProcessorEditor (JVerbAudioProcessor&);
    ~JVerbAudioProcessorEditor();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

 
private:

    OtherLookAndFeel otherLookAndFeel;
    Label  inputLabel;
    Slider inputSlider;
    Label  outputLabel;
    Slider outputSlider;
    Label  colorLabel;
    Slider colorSlider;
    Label  colorGainLabel;
    Slider colorGainSlider;
    Label  diffusionLabel;
    Slider diffusionSlider;
    Label  dampingLabel;
    Slider dampingSlider;
    Label  predelayLabel;
    Slider predelaySlider;
    Label  decayLabel;
    Slider decaySlider;
    Label  sizeLabel;
    Slider sizeSlider;
    Label  drywetLabel;
    Slider drywetSlider;
    Label  lfofreqLabel;
    Slider lfofreqSlider;
    Label  lfodepthLabel;
    Slider lfodepthSlider;
    ComboBox filterSelect;
    ComboBox lfoSelect;
  
    JVerbAudioProcessor& processor;
public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> inputSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> outputSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> colorSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> colorGainSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> diffusionSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> dampingSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> predelaySliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> decaySliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sizeSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> drywetSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lfoFreqSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lfoDepthSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> filterSelectVal;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> lfoSelectVal;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JVerbAudioProcessorEditor)
};

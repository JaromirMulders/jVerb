/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
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
    Label  inputLabel;
    Slider inputSlider;
    Label  outputLabel;
    Slider outputSlider;
    Label  colorLabel;
    Slider colorSlider;
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
  
    JVerbAudioProcessor& processor;
public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> inputSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> outputSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> colorSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> diffusionSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> dampingSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> predelaySliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> decaySliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sizeSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> drywetSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lfoFreqSliderVal;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lfoDepthSliderVal;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JVerbAudioProcessorEditor)
};

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JVerbAudioProcessorEditor::JVerbAudioProcessorEditor (JVerbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 500);

    inputSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, INPUT_ID, inputSlider);
    outputSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, OUTPUT_ID, outputSlider);
    colorSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, COLOR_ID, colorSlider);
    colorGainSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, COLORGAIN_ID, colorGainSlider);
    diffusionSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DIFFUSION_ID, diffusionSlider);
    dampingSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DAMPING_ID, dampingSlider);
    predelaySliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, PREDELAY_ID, predelaySlider);
    decaySliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DECAY_ID, decaySlider);
    sizeSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, SIZE_ID, sizeSlider);
    drywetSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DRYWET_ID, drywetSlider);
    lfoFreqSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, LFOFREQ_ID, lfofreqSlider);
    lfoDepthSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, LFODEPTH_ID, lfodepthSlider);
  
    addAndMakeVisible (inputSlider);
    inputSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    inputSlider.setRange(0.0f, 1.0f);
    inputSlider.setTextValueSuffix("dB");
    inputSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(inputLabel);
    inputLabel.setText (INPUT_NAME, dontSendNotification);
    inputLabel.attachToComponent (&inputSlider, false);

    addAndMakeVisible (outputSlider);
    outputSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    outputSlider.setRange(0.0f, 1.0f);
    outputSlider.setTextValueSuffix("dB");
    outputSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(outputLabel);
    outputLabel.setText (OUTPUT_NAME, dontSendNotification);
    outputLabel.attachToComponent (&outputSlider, false);

    addAndMakeVisible (colorSlider);
    colorSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    colorSlider.setRange(0.0f, 1.0f);
    colorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(colorLabel);
    colorLabel.setText (COLOR_NAME, dontSendNotification);
    colorLabel.attachToComponent (&colorSlider, false);

    addAndMakeVisible (colorGainSlider);
    colorGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    colorGainSlider.setRange(0.0f, 1.0f);
    colorGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(colorGainLabel);
    colorGainLabel.setText (COLORGAIN_NAME, dontSendNotification);
    colorGainLabel.attachToComponent (&colorGainSlider, false);
  
    addAndMakeVisible (diffusionSlider);
    diffusionSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    diffusionSlider.setRange(0.0f, 1.0f);
    diffusionSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(diffusionLabel);
    diffusionLabel.setText (DIFFUSION_NAME, dontSendNotification);
    diffusionLabel.attachToComponent (&diffusionSlider, false);

    addAndMakeVisible (dampingSlider);
    dampingSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    dampingSlider.setRange(0.0f, 1.0f);
    dampingSlider.setTextValueSuffix("dB");
    dampingSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(dampingLabel);
    dampingLabel.setText (DAMPING_NAME, dontSendNotification);
    dampingLabel.attachToComponent (&dampingSlider, false);

    addAndMakeVisible (predelaySlider);
    predelaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    predelaySlider.setRange(0.0f, 250.0f);
    predelaySlider.setTextValueSuffix("ms");
    predelaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(predelayLabel);
    predelayLabel.setText (PREDELAY_NAME, dontSendNotification);
    predelayLabel.attachToComponent (&predelaySlider, false);

    addAndMakeVisible (decaySlider);
    decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    decaySlider.setRange(0.0f, 1.0f);
    decaySlider.setTextValueSuffix("db");
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(decayLabel);
    decayLabel.setText (DECAY_NAME, dontSendNotification);
    decayLabel.attachToComponent (&decaySlider, false);

    addAndMakeVisible (sizeSlider);
    sizeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    sizeSlider.setRange(0.0f, 1.0f);
    sizeSlider.setTextValueSuffix("dB");
    sizeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(sizeLabel);
    sizeLabel.setText (SIZE_NAME, dontSendNotification);
    sizeLabel.attachToComponent (&sizeSlider, false);

    addAndMakeVisible (drywetSlider);
    drywetSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    drywetSlider.setRange(0.0f, 100.0f);
    drywetSlider.setTextValueSuffix("%");
    drywetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(drywetLabel);
    drywetLabel.setText (DRYWET_NAME, dontSendNotification);
    drywetLabel.attachToComponent (&drywetSlider, false);

    addAndMakeVisible (lfofreqSlider);
    lfofreqSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    lfofreqSlider.setRange(0.0f, 1.0f);
    lfofreqSlider.setTextValueSuffix("dB");
    lfofreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(lfofreqLabel);
    lfofreqLabel.setText (LFOFREQ_NAME, dontSendNotification);
    lfofreqLabel.attachToComponent (&lfofreqSlider, false);

    addAndMakeVisible (lfodepthSlider);
    lfodepthSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    lfodepthSlider.setRange(0.0f, 1.0f);
    lfodepthSlider.setTextValueSuffix("dB");
    lfodepthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    addAndMakeVisible(lfodepthLabel);
    lfodepthLabel.setText (LFODEPTH_NAME, dontSendNotification);
    lfodepthLabel.attachToComponent (&lfodepthSlider, false);
  
}

JVerbAudioProcessorEditor::~JVerbAudioProcessorEditor()
{
}

//==============================================================================
void JVerbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void JVerbAudioProcessorEditor::resized()
{
    int xPos = 40;
    int xSpace = 75;
    int sHeight = getHeight() - 60;
    int xWidth = 75;
  
    //x,y,width,heigth
    inputSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    outputSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    colorSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    colorGainSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    diffusionSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    dampingSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    predelaySlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    decaySlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    sizeSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    drywetSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    lfofreqSlider.setBounds(xPos, 30, xWidth, sHeight);
    xPos+=xSpace;
    lfodepthSlider.setBounds(xPos, 30, xWidth, sHeight);

}
  


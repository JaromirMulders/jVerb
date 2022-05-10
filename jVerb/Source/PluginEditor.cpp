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
    setSize(600, 500);
    
setLookAndFeel (&otherLookAndFeel);
    
     
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
    filterSelectVal = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, FILTERSELECT_ID, filterSelect);
    lfoSelectVal = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, LFOSELECT_ID, lfoSelect);
  
    addAndMakeVisible (inputSlider);
    inputSlider.setSliderStyle (Slider::Rotary);
    inputSlider.setRange(0.0f, 1.0f);
    inputSlider.setTextValueSuffix("dB");
    inputSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(inputLabel);
    //inputLabel.setText (INPUT_NAME, dontSendNotification);
    //inputLabel.attachToComponent (&inputSlider, false);

    addAndMakeVisible (outputSlider);
    outputSlider.setSliderStyle (Slider::Rotary);
    outputSlider.setRange(0.0f, 1.0f);
    outputSlider.setTextValueSuffix("dB");
    outputSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(outputLabel);
    //outputLabel.setText (OUTPUT_NAME, dontSendNotification);
    //outputLabel.attachToComponent (&outputSlider, false);

    addAndMakeVisible (colorSlider);
    colorSlider.setSliderStyle (Slider::Rotary);
    colorSlider.setRange(0.0f, 1.0f);
    colorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(colorLabel);
    //colorLabel.setText (COLOR_NAME, dontSendNotification);
    //colorLabel.attachToComponent (&colorSlider, false);

    addAndMakeVisible (colorGainSlider);
    colorGainSlider.setSliderStyle (Slider::Rotary);
    colorGainSlider.setRange(0.0f, 1.0f);
    colorGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(colorGainLabel);
    //colorGainLabel.setText (COLORGAIN_NAME, dontSendNotification);
    //colorGainLabel.attachToComponent (&colorGainSlider, false);
  
    addAndMakeVisible (diffusionSlider);
    diffusionSlider.setSliderStyle (Slider::Rotary);
    diffusionSlider.setRange(0.0f, 1.0f);
    diffusionSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(diffusionLabel);
    //diffusionLabel.setText (DIFFUSION_NAME, dontSendNotification);
    //diffusionLabel.attachToComponent (&diffusionSlider, false);

    addAndMakeVisible (dampingSlider);
    dampingSlider.setSliderStyle (Slider::Rotary);
    dampingSlider.setRange(10.0f, 20000.0f);
    dampingSlider.setTextValueSuffix("Hz");
    dampingSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(dampingLabel);
    //dampingLabel.setText (DAMPING_NAME, dontSendNotification);
    //dampingLabel.attachToComponent (&dampingSlider, false);

    addAndMakeVisible (predelaySlider);
    predelaySlider.setSliderStyle (Slider::Rotary);
    predelaySlider.setRange(0.0f, 250.0f);
    predelaySlider.setTextValueSuffix("ms");
    predelaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(predelayLabel);
    //predelayLabel.setText (PREDELAY_NAME, dontSendNotification);
    //predelayLabel.attachToComponent (&predelaySlider, false);

    addAndMakeVisible (decaySlider);
    decaySlider.setSliderStyle (Slider::Rotary);
    decaySlider.setRange(0.0f, 1.0f);
    decaySlider.setTextValueSuffix("db");
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(decayLabel);
    //decayLabel.setText (DECAY_NAME, dontSendNotification);
    //decayLabel.attachToComponent (&decaySlider, false);

    addAndMakeVisible (sizeSlider);
    sizeSlider.setSliderStyle (Slider::Rotary);
    sizeSlider.setRange(0.0f, 1.0f);
    sizeSlider.setTextValueSuffix("dB");
    sizeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(sizeLabel);
    //sizeLabel.setText (SIZE_NAME, dontSendNotification);
    //sizeLabel.attachToComponent (&sizeSlider, false);
  
    addAndMakeVisible (drywetSlider);
    drywetSlider.setSliderStyle (Slider::Rotary);
    drywetSlider.setRange(0.0f, 100.0f);
    drywetSlider.setTextValueSuffix("%");
    drywetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(drywetLabel);
    //drywetLabel.setText (DRYWET_NAME, dontSendNotification);
    //drywetLabel.attachToComponent (&drywetSlider, false);

    addAndMakeVisible (lfofreqSlider);
    lfofreqSlider.setSliderStyle (Slider::Rotary);
    lfofreqSlider.setRange(0.001f, 20.0f);
    lfofreqSlider.setTextValueSuffix("Hz");
    lfofreqSlider.setSkewFactorFromMidPoint (2.5);
    lfofreqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(lfofreqLabel);
    //lfofreqLabel.setText (LFOFREQ_NAME, dontSendNotification);
    //lfofreqLabel.attachToComponent (&lfofreqSlider, false);

    addAndMakeVisible (lfodepthSlider);
    lfodepthSlider.setSliderStyle (Slider::Rotary);
    lfodepthSlider.setRange(0.0f, 1.0f);
    lfodepthSlider.setSkewFactorFromMidPoint (0.25);
    lfodepthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 25);

    //addAndMakeVisible(lfodepthLabel);
    //lfodepthLabel.setText (LFODEPTH_NAME, dontSendNotification);
    //lfodepthLabel.attachToComponent (&lfodepthSlider, false);
  
    addAndMakeVisible(filterSelect);
    filterSelect.addItem("Ladder", 1);
    filterSelect.addItem("onePole", 2);
    filterSelect.setSelectedId(1);

    addAndMakeVisible(lfoSelect);
    lfoSelect.addItem("Sine", 1);
    lfoSelect.addItem("Triangle", 2);
    lfoSelect.addItem("Square", 3);
    lfoSelect.addItem("Saw", 4);
    lfoSelect.setSelectedId(1);
  
}

JVerbAudioProcessorEditor::~JVerbAudioProcessorEditor()
{
}

//==============================================================================
void JVerbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    Image background = ImageCache::getFromMemory (BinaryData::jverb_main_png, BinaryData::jverb_main_pngSize);
    g.drawImageAt (background, 0, 0);
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void JVerbAudioProcessorEditor::resized()
{
    int sHeight = 105;
    int xWidth = sHeight;
  
    int xScale = (xWidth/2+1);
    int yScale = (sHeight/2-11);
  
    //x,y,width,heigth
    inputSlider.setBounds(86-xScale, 427-yScale, xWidth, sHeight);
    outputSlider.setBounds(196-xScale, 427-yScale, xWidth, sHeight);
    colorSlider.setBounds(196-xScale, 162-yScale, xWidth, sHeight);
    colorGainSlider.setBounds(307-xScale, 162-yScale, xWidth, sHeight);
    diffusionSlider.setBounds(86-xScale, 288-yScale, xWidth, sHeight);
    filterSelect.setBounds (530-xScale, 162+110-yScale, xWidth , 30);
    dampingSlider.setBounds(530-xScale, 162-yScale, xWidth, sHeight);
    predelaySlider.setBounds(418-xScale, 162-yScale, xWidth, sHeight);
    decaySlider.setBounds(307-xScale, 288-yScale, xWidth, sHeight);
    sizeSlider.setBounds(197-xScale, 288-yScale, xWidth, sHeight);
    drywetSlider.setBounds(308-xScale, 427-yScale, xWidth, sHeight);
    lfofreqSlider.setBounds(424-xScale, 374-yScale, xWidth, sHeight);
    lfoSelect.setBounds(424-xScale, 374+110-yScale, xWidth, 30);
    lfodepthSlider.setBounds(534-xScale, 374-yScale, xWidth, sHeight);

  

}
  


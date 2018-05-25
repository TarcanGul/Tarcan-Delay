/*
  ==============================================================================
  The editor side.
  ==============================================================================
*/
#include <stdio.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
TarcanDelayAudioProcessorEditor::TarcanDelayAudioProcessorEditor (TarcanDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200); //Setting the size of the program.
	
	//The setup for volume slider with the label.
	volumeLabel.setText("VOLUME", juce::NotificationType::dontSendNotification);
	volumeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	volumeSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	volumeSlider.setRange(-48, 0);
	volumeSlider.setLookAndFeel(&tarcanFeel);
	volumeSlider.addListener(this); //Adding the slider listener which this class inherited.
	

	//The setup for dry slider with the label.
	dryLabel.setText("Dry Level", juce::NotificationType::dontSendNotification);
	drySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	drySlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	drySlider.setRange(0.0, 1.0, 0.01);
	drySlider.setLookAndFeel(&tarcanFeel);
	drySlider.addListener(this); //Adding the slider listener which this class inherited.
	
	//The setup for dry slider with the label.
	wetLabel.setText("Wet Level", juce::NotificationType::dontSendNotification);
	wetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	wetSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	wetSlider.setRange(0.0, 1.0,0.01);
	wetSlider.setLookAndFeel(&tarcanFeel);	
	wetSlider.addListener(this); //Adding the slider listener which this class inherited.

	
	//The setup for feedback slider with the label.
	feedbackLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
	feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	feedbackSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	feedbackSlider.setRange(0.0, 0.9, 0.05);
	feedbackSlider.setLookAndFeel(&tarcanFeel);
	feedbackSlider.addListener(this); //Adding the slider listener which this class inherited.

	//The setup for time slider with the label.
	timeLabel.setText("Time", juce::NotificationType::dontSendNotification);
	timeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	timeSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	timeSlider.setRange(0.1, 2.0, 0.1);
	timeSlider.setLookAndFeel(&tarcanFeel);
	timeSlider.addListener(this);//Adding the slider listener which this class inherited.

	//Adding every component to the frame.
	addAndMakeVisible(&volumeSlider);
	addAndMakeVisible(&volumeLabel);
	addAndMakeVisible(&drySlider);
	addAndMakeVisible(&dryLabel);
	addAndMakeVisible(&wetLabel);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackLabel);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&timeLabel);
	addAndMakeVisible(&timeSlider);

	//Initiliazing the SliderAttachment scoped pointers. These are for maintaining the communication between the sliders and the parameters.
	volumeAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "vol", volumeSlider);
	timeAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "time", timeSlider);
	feedAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "feedback", feedbackSlider);
	dryAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "dry", drySlider);
	wetAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "wet", wetSlider);

}

//The destructor. Since we only allocated memory for scoped pointers, we don't need any extra action to deallocate it.
TarcanDelayAudioProcessorEditor::~TarcanDelayAudioProcessorEditor()
{
   
}

//Basic paint function. Just writing my name onto the plugin.
//==============================================================================
void TarcanDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.setFont (10.0f);
    g.drawFittedText ("Developed by Tarcan Gul", getLocalBounds(), Justification::topLeft, 1);
    
}

//This is where I layout my components. I used fractions for flexibility.
void TarcanDelayAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	int w = getWidth();
	int h = getHeight();
	volumeLabel.setBounds(w/10 + w/20, h/8-h/12, w/5 , h/8);
	dryLabel.setBounds(2.5*w/5 + w/40, 0, w/5, h/6);
	wetLabel.setBounds(2.5*w/5 + w/40, h/2, w/5, h/6);
	feedbackLabel.setBounds(3.5*w / 5 + w / 40, 0, w / 5, h / 6);
	timeLabel.setBounds(3.5*w / 5 + w / 20, h / 2, w / 5, h / 6);

    volumeSlider.setBounds(w/100, h/4 - h/12, 2*w/5, 3*h/4);
	drySlider.setBounds(2.5*w/5, h/4 - h/12, w/5, h/4);
	wetSlider.setBounds(2.5*w/5, 3*h/4 - h/12, w/5, h/4);
	feedbackSlider.setBounds(3.5*w / 5, h / 4 - h / 12, w / 5, h / 4);
	timeSlider.setBounds(3.5*w / 5, 3 * h / 4 - h / 12, w / 5, h / 4);
	
}

//This is the function from Slider::Listener that listens for slider changes. I change the corresponding values in the processor for corresponding sliders.
void TarcanDelayAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &volumeSlider)
	{
		processor.rawSound =pow(10, volumeSlider.getValue() / 20); //rawSound is a logarithmic variable because we perceive sound logarithmicly. 

	}
	else if (slider == &drySlider)
	{
		processor.dryMix = (float)drySlider.getValue(); //Updating dryMix variable in the processor.
	}
	else if (slider == &wetSlider) 
	{
		processor.wetMix = (float)wetSlider.getValue(); //Updating wetMix variable in the processor.
	}
	else if (slider == &feedbackSlider) 
	{
		processor.feedback = (float)feedbackSlider.getValue(); //Updating feedback variable in the processor.
	}
	else if (slider == &timeSlider) 
	{
		processor.delayTime = (double)timeSlider.getValue(); //Updating delayTime variable in the processor.
		
	}
}


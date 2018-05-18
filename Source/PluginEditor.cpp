/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

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
    setSize (500, 200);
	
	

	volumeLabel.setText("VOLUME", juce::NotificationType::dontSendNotification);
	volumeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	volumeSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	
	volumeSlider.setRange(-48, 0);
	volumeSlider.setValue(-10);
	volumeSlider.setLookAndFeel(&tarcanFeel);
	
	volumeSlider.addListener(this);
	addAndMakeVisible(&volumeSlider);
	addAndMakeVisible(&volumeLabel);

	
	dryLabel.setText("Dry Level", juce::NotificationType::dontSendNotification);
	drySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	drySlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	drySlider.setRange(0.0, 1.0, 0.01);
	drySlider.setValue(1.0);
	drySlider.setLookAndFeel(&tarcanFeel);
	
	drySlider.addListener(this);
	
	
	
	wetLabel.setText("Wet Level", juce::NotificationType::dontSendNotification);
	wetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	wetSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	wetSlider.setRange(0.0, 1.0,0.01);
	wetSlider.setValue(0.5);
	wetSlider.setLookAndFeel(&tarcanFeel);
	
	wetSlider.addListener(this);

	
	
	feedbackLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
	feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	feedbackSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	feedbackSlider.setRange(0.0, 0.9, 0.05);
	feedbackSlider.setValue(0.2);
	feedbackSlider.setLookAndFeel(&tarcanFeel);
	
	feedbackSlider.addListener(this);

	
	timeLabel.setText("Time", juce::NotificationType::dontSendNotification);
	timeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	timeSlider.setTextBoxStyle(Slider::NoTextBox, true, 50, 50);
	timeSlider.setRange(0.1, 2.0, 0.1);
	timeSlider.setValue(0.5);
	timeSlider.setLookAndFeel(&tarcanFeel);
	
	timeSlider.addListener(this);

	



	addAndMakeVisible(&drySlider);
	addAndMakeVisible(&dryLabel);
	
	addAndMakeVisible(&wetLabel);
	addAndMakeVisible(&wetSlider);
	addAndMakeVisible(&feedbackLabel);
	addAndMakeVisible(&feedbackSlider);
	addAndMakeVisible(&timeLabel);
	addAndMakeVisible(&timeSlider);

	volumeAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "vol", volumeSlider);
	timeAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "time", timeSlider);
	feedAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "feedback", feedbackSlider);
	dryAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "dry", drySlider);
	wetAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.getState(), "wet", wetSlider);

}

TarcanDelayAudioProcessorEditor::~TarcanDelayAudioProcessorEditor()
{
   
}

//==============================================================================
void TarcanDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);
    //g.drawFittedText ("Developed by Tarcan Gul", getLocalBounds(), Justification::topLeft, 1);
    
}

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

void TarcanDelayAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &volumeSlider)
	{
		processor.rawSound =pow(10, volumeSlider.getValue() / 20);

	}
	else if (slider == &drySlider)
	{
		processor.dryMix = (float)drySlider.getValue();
	}
	else if (slider == &wetSlider) 
	{
		processor.wetMix = (float)wetSlider.getValue();
	}
	else if (slider == &feedbackSlider) 
	{
		processor.feedback = (float)feedbackSlider.getValue();
	}
	else if (slider == &timeSlider) 
	{
		processor.delayTime = (double)timeSlider.getValue();
		
	}
}


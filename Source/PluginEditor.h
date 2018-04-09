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
class TarcanLookAndFeel : public LookAndFeel_V4
{
public:

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		g.setColour(Colours::maroon);
		const float radius = jmin(width / 2, height / 2) - 4.0f;
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		g.fillEllipse(rx, ry, rw, rw);
		g.setColour(Colours::azure);
		g.drawEllipse(rx, ry, rw, rw, 1.0f);
		Path p;
		const float pointerLength = radius * 0.33f;
		const float pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

		g.setColour(Colours::black);
		g.fillPath(p);

	}
};

class TarcanDelayAudioProcessorEditor  : public AudioProcessorEditor, 
                                         public Slider::Listener
{
public:
    TarcanDelayAudioProcessorEditor (TarcanDelayAudioProcessor&);
    ~TarcanDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    TarcanLookAndFeel tarcanFeel;
	Slider drySlider;
	Label dryLabel;
	Slider wetSlider;
	Label wetLabel;
	Slider volumeSlider;
	Label volumeLabel;
	Slider feedbackSlider;
	Label feedbackLabel;
	Slider timeSlider;
	Label timeLabel;
	void sliderValueChanged(Slider *slider) override;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> sliderAttach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TarcanDelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TarcanDelayAudioProcessorEditor)
};

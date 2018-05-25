/*
  ==============================================================================
  Editor header file.
  ==============================================================================
*/

#pragma once //This makes sure that this header file is included only once in a single compilation.

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
This is my look and feel class where I draw my knob design.
*/
class TarcanLookAndFeel : public LookAndFeel_V4
{
public:

	//The function where I draw my slider design. FROM JUCE TUTORIAL
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		g.setColour(Colours::maroon);
		const float radius = jmin(width / 2, height / 2) - 4.0f;
		const float centreX = x + width * 0.5f; //The x value of the center of the slider.
		const float centreY = y + height * 0.5f; //The y value of the center of the slider.
		const float rx = centreX - radius;// The x value of the ellipse.
		const float ry = centreY - radius; // The y value of the ellipse.
		const float rw = radius * 2.0f; //Width of the circle.
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle); //Storing the angle.
		//Making the ellipse.
		g.fillEllipse(rx, ry, rw, rw);
		g.setColour(Colours::azure);
		g.drawEllipse(rx, ry, rw, rw, 1.0f);

		Path p; //The selected "line" in the slider.
		const float pointerLength = radius * 0.33f; //The length of the line.
		const float pointerThickness = 2.0f;//The thickness of line.
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength); //Drawing the line.
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY)); //Whenever we rotate the circle, the place of the line is updated.

		//Coloring the line as black.
		g.setColour(Colours::black);
		g.fillPath(p);

	}
};

class TarcanDelayAudioProcessorEditor : public AudioProcessorEditor,
	public Slider::Listener
{
public:
    TarcanDelayAudioProcessorEditor (TarcanDelayAudioProcessor&);
    ~TarcanDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    TarcanLookAndFeel tarcanFeel; //Instance for my look and feel class.

	//My sliders and labels.
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

	void sliderValueChanged(Slider *slider) override; //Method from SliderListener. Listens for value changes.

	//Smart pointers for slider attachments which will connect sliders with parameters and change them acoordingly.
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> volumeAttach;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> dryAttach;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> wetAttach;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> feedAttach;
	ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> timeAttach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TarcanDelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TarcanDelayAudioProcessorEditor)
};

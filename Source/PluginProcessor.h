/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class TarcanDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TarcanDelayAudioProcessor();
    ~TarcanDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
	AudioProcessorValueTreeState& getState() { return processorState; };
	/*void parameterChanged(const String& parameterID, float newValue)

	{
		MajorParamChange = true;
	};*/

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    double rawSound;
	float dryMix;
	float wetMix;
	float feedback;
	double delayTime;
	
	//Creating automable parameters for every single value the user can control.
	AudioProcessorValueTreeState processorState;


private:
	AudioSampleBuffer delayBuffer;
	bool MajorParamChange;
	int delayLen;
	int delayReadPos, delayWritePos;
	int delayBufferLen;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TarcanDelayAudioProcessor)
};

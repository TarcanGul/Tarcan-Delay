/*
  ==============================================================================
  Processor header file. Processor is in charge of audio processing.
  ==============================================================================
*/

#pragma once //This makes sure that this header file is included only once in a single compilation.

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
    void prepareToPlay (double sampleRate, int samplesPerBlock) override; //This method handles the operation just before feedback. It is a default method in JUCE.
    void releaseResources() override; //Handles operations right after audio playback.

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override; //Default bus layout configurations.
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override; //Callback function

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
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


	//VARIABLES
    double rawSound; //The level of the volume. 
	float dryMix; //The level of signal passing through without any effect and processing.
	float wetMix; //The level of signal passing through with effect and processing.
	float feedback; //The ratio of the portion of the output signal which feeds the input signal.
	double delayTime; //The amount of time between repeating sound signals. 
	
	//The ValueTree object which stores the state of the processor. This tree is where we are going to add the parameters.
	AudioProcessorValueTreeState processorState;


private:

	AudioSampleBuffer delayBuffer; //This buffer will store the values which can be read and written. Essentially it is a dynamic array.
	
	int delayReadPos, delayWritePos;//The indexes where we might want to read or write. Important for preserving states.
	int delayBufferLen; // The length of the buffer.In other words, how many values do we have in our array?
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TarcanDelayAudioProcessor)
};

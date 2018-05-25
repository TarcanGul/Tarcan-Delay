/*
  ==============================================================================
  The processor side.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
/**
	The constructor.
	Before initializing the values, the constructor is initializing the input and output channels using an initializer list. 
	Since this is an effect plugin, we have a single input and a single output bus.
*/
TarcanDelayAudioProcessor::TarcanDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),processorState(*this, nullptr)//Initializing the AudioProcessorValueTreeState object with no undo manager.
#endif
{
	//These are the ranges I wish to use with my parameters. NormalisableRange object will convert these ranges to the [0,1] range. Some of them have a third number which means the difference between each value. Default is 0.01. 
	NormalisableRange<float> volumeRange(-48, 0);
	NormalisableRange<float> timeRange(0.1, 2.0,0.1);
	NormalisableRange<float> dry_wetRange(0.0, 1.0,0.01);
	NormalisableRange<float> feedRange(0.0, 1.0,0.05);

	//Adding 5 parameters to our AudioProcessorValueTree object with ID's and initial values.
    processorState.createAndAddParameter("vol", "VolumeLevel", "VolumeLevel", volumeRange, -10.0f, nullptr, nullptr);
	processorState.createAndAddParameter("time", "DelayTime", "DelayTime", timeRange, 0.5f, nullptr, nullptr);
	processorState.createAndAddParameter("dry", "DryLevel", "DryLevel", dry_wetRange, 1.0f, nullptr, nullptr);
	processorState.createAndAddParameter("wet", "WetLevel", "WetLevel", dry_wetRange, 0.5f, nullptr, nullptr);
	processorState.createAndAddParameter("feedback", "Feedback", "Feedback", feedRange, 0.2f, nullptr, nullptr);

	delayReadPos, delayWritePos = 0; //Initializing the reading and the writing positions in our delay buffer.
	delayBufferLen = 1; //Initializing delay buffer length.

	
	processorState.state = ValueTree("liveState"); //Initializing the state for our AudioProcessorValueTreeState object since we want to use this to save user's presets.
	
	
	


}

//The destructor. Since we are using scoped pointers, there is no need to have additional operations.
TarcanDelayAudioProcessor::~TarcanDelayAudioProcessor()
{
	
}

//Returns the name.
//==============================================================================
const String TarcanDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//The effect plugin doesn't accept midi signals because it doesn't need it.
bool TarcanDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//The plugin also doesn't generate midi signals.
bool TarcanDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//The plugin have nothing to do with midi signals.
bool TarcanDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//Returns the tail length, which means the sound that keeps on playing even though the music stops. I didn't worry about this too much, thus I just return 0.
double TarcanDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//Returns the number of programs.
int TarcanDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs. (JUCE)
}

//Returns the number of currently active programs. DAW's (Hosts) will use this method.
int TarcanDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

//This is the method for hosts to change the number of running plugins.
void TarcanDelayAudioProcessor::setCurrentProgram (int index)
{
}

//Returns the program name.
const String TarcanDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

//Changes the program name.
void TarcanDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

// This method is for pre-playback initialization.
//==============================================================================
void TarcanDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
	 
	delayBufferLen = (int)(2*sampleRate); //Allocating the delay buffer. We are setting 2 seconds as our maximum delay.
	if (delayBufferLen < 1) //This check is here to avoid calculations with zero. We are setting the length to 1(minimum).
	{
		delayBufferLen = 1;
	}
	delayBuffer.setSize(2, delayBufferLen);// We make sure the buffer is for two output channels (L-R) and have delayBufferLen many samples.
	delayBuffer.clear(); //Clearing the buffer before playback.
	
	
}

void TarcanDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TarcanDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
//The method where we actually process the sound.
void TarcanDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	//Setting the position of the read pointer. The actual length of the delay is the difference between the read and write pointers. Thus we are giving an initial offset determined by delayTime.
	delayReadPos = (int)(delayWritePos - (delayTime * getSampleRate()) + delayBufferLen) % delayBufferLen; 

    auto totalNumInputChannels  = getTotalNumInputChannels(); //Total number of input channels.
    auto totalNumOutputChannels = getTotalNumOutputChannels(); //Total number of output channels.

	int dpr, dpw; //Read and write pointers.
	
	//Iterating through input channels which in this case is only one.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel); //Storing the write pointer of the input channel.
		float *delayData = delayBuffer.getWritePointer(jmin(channel, delayBuffer.getNumChannels() - 1)); //The pointer to the delay buffer.

		//Making a copy of two variables, since these variables should be maintained between processBlock calls.
		dpr = delayReadPos;
		dpw = delayWritePos;

        // ..do something to the data...
		//This is where we process the sound. With a for loop, we are iterating though the input sample.
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			//Essentially every sample we are taking is just an number (digital audio). We are storing this sample value as const, because we want the input signal in the output as well(it is determined by dryMix parameter).
			const float in = channelData[sample];
			float out = 0.0; // The output is initialized as 0.
			
			out = (dryMix * in) + (wetMix*delayData[dpr]); //Delay formula: Output = input + delayed signal. dpr is for keeping track of samples.

			delayData[dpw] = in + (delayData[dpr] * feedback); //The content of the buffer is updated. 
			
			//Using the circular buffer technique. Whenever we exceed the length of the buffer, we go back to the start.
			if (++dpr >= delayBufferLen) 
			{
				dpr = 0;
			}
			if (++dpw >= delayBufferLen) 
			{
				dpw = 0;
			}
			
			channelData[sample] = out; //Updating the channel data which is the actual output.
			channelData[sample] = buffer.getSample(channel, sample) * rawSound; //rawSound is the gain. We are updating the output with gain.
		}
		
    }
	delayReadPos = dpr; // Again, making the copies of pointers since we want to affect every channel seperately and consistently.
	delayWritePos = dpw;

	//Making sure to check and clean in the case of input channels being more that output channels. This can create weird behaviour.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

}

//==============================================================================
bool TarcanDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TarcanDelayAudioProcessor::createEditor()
{
    return new TarcanDelayAudioProcessorEditor (*this);
}

//This method is for saving presets or saving the state of the plugin in a project file.
//==============================================================================
void TarcanDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Storing arameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	ScopedPointer <XmlElement> savepoint(processorState.state.createXml()); //Saving the state of the processor as a xml. xml is easy to store with.
	//Saving the values of parameters so that they are initialized before opening a saved state.
	savepoint->setAttribute("rawSound", rawSound); 
	savepoint->setAttribute("dryMix", dryMix);
	savepoint->setAttribute("wetMix", wetMix);
	savepoint->setAttribute("feedback", feedback);
	savepoint->setAttribute("delayTime", delayTime);

	copyXmlToBinary(*savepoint, destData); //Copying the data to the destination.
	
}

void TarcanDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restoring parameters from "data",
    // "data" is from a getStateInformation() call.
	ScopedPointer<XmlElement> restore(getXmlFromBinary(data, sizeInBytes)); // Getting the data which we set with getStateInformation().
	if (restore != nullptr) //If there is any data.
	{
		if (restore->hasTagName(processorState.state.getType())) //If the data we are looking is about our processor.
		{
			processorState.state = ValueTree::fromXml(*restore); //Updating the state of the valuetree.

			//Restoring the parameters.
			rawSound = restore->getDoubleAttribute("rawSound");
			dryMix = restore->getDoubleAttribute("dryMix");
			wetMix = restore->getDoubleAttribute("wetMix");
			feedback = restore->getDoubleAttribute("feedback");
			delayTime = restore->getDoubleAttribute("delayTime");
		}
	}
	
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TarcanDelayAudioProcessor();
}

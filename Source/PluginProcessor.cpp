/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

/*
AudioProcessorValueTreeState delayTimeState;
AudioProcessorValueTreeState dryLevelState;
AudioProcessorValueTreeState wetLevelState;
AudioProcessorValueTreeState feedbackState;
*/
//==============================================================================
TarcanDelayAudioProcessor::TarcanDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),processorState(*this, nullptr)
#endif
{
	
	NormalisableRange<float> volumeRange(-48, 0);
	NormalisableRange<float> timeRange(0.1, 2.0,0.1);
	NormalisableRange<float> dry_wetRange(0.0, 1.0,0.01);
	NormalisableRange<float> feedRange(0.0, 1.0,0.05);
    processorState.createAndAddParameter("vol", "VolumeLevel", "VolumeLevel", volumeRange, -10.0f, nullptr, nullptr);
	processorState.createAndAddParameter("time", "DelayTime", "DelayTime", timeRange, 0.5f, nullptr, nullptr);
	processorState.createAndAddParameter("dry", "DryLevel", "DryLevel", dry_wetRange, 1.0f, nullptr, nullptr);
	processorState.createAndAddParameter("wet", "WetLevel", "WetLevel", dry_wetRange, 0.5f, nullptr, nullptr);
	processorState.createAndAddParameter("feedback", "Feedback", "Feedback", feedRange, 0.2f, nullptr, nullptr);
	delayReadPos, delayWritePos = 0;
	/*dryMix = 1.0;
	wetMix = 0.5;
	feedback = 0.2;
	delayTime = 0.5;
	*/
	delayBufferLen = 1;
	delayLen = 0.5;

	
	processorState.state = ValueTree("liveState");
	//processorState->addParameterListener("Param_List", this);
	
	


}

TarcanDelayAudioProcessor::~TarcanDelayAudioProcessor()
{
	
}

//==============================================================================
const String TarcanDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TarcanDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TarcanDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TarcanDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TarcanDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TarcanDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TarcanDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TarcanDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String TarcanDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void TarcanDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TarcanDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	
	delayBufferLen = (int)(2*sampleRate);
	if (delayBufferLen < 1) 
	{
		delayBufferLen = 1;
	}
	delayBuffer.setSize(2, delayBufferLen);
	delayBuffer.clear();
	
	
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

void TarcanDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	delayReadPos = (int)(delayWritePos - (delayTime * getSampleRate()) + delayBufferLen) % delayBufferLen;
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
	

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	int dpr, dpw;
	
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
		float *delayData = delayBuffer.getWritePointer(jmin(channel, delayBuffer.getNumChannels() - 1));
		dpr = delayReadPos;
		dpw = delayWritePos;
        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			
			const float in = channelData[sample];
			float out = 0.0;
			
			out = (dryMix * in) + (wetMix*delayData[dpr]);

			delayData[dpw] = in + (delayData[dpr] * feedback);
			
			if (++dpr >= delayBufferLen) 
			{
				dpr = 0;
			}
			if (++dpw >= delayBufferLen) 
			{
				dpw = 0;
			}
			
			channelData[sample] = out;
			channelData[sample] = buffer.getSample(channel, sample) * rawSound;
		}
		
    }
	delayReadPos = dpr;
	delayWritePos = dpw;

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

//==============================================================================
void TarcanDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	ScopedPointer <XmlElement> savepoint(processorState.state.createXml());
	savepoint->setAttribute("rawSound", rawSound);
	savepoint->setAttribute("dryMix", dryMix);
	savepoint->setAttribute("wetMix", wetMix);
	savepoint->setAttribute("feedback", feedback);
	savepoint->setAttribute("delayTime", delayTime);

	copyXmlToBinary(*savepoint, destData);
	
}

void TarcanDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	ScopedPointer<XmlElement> restore(getXmlFromBinary(data, sizeInBytes));
	if (restore != nullptr) 
	{
		if (restore->hasTagName(processorState.state.getType())) 
		{
			processorState.state = ValueTree::fromXml(*restore);
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

/**
 * File name: processor_vst.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/geonkick)
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "GkickVstProcessor.h"
#include "VstIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg
{

GeonkickVstEffect::GeonkickVstEffect()
{
        RK_LOG_INFO("called");
        setControllerClass(GKickVstControllerUID);
}

tresult PLUGIN_API GeonkickVstEffect::initialize(FUnknown* context)
{
        RK_LOG_INFO("called");
        auto res = AudioEffect::initialize(context);
        if (res != kResultTrue)
                return kResultFalse;

        addAudioOutput(STR16("Geonkick Out"), Vst::SpeakerArr::kStereo);
        addEventInput(STR16 ("Geonkick In"), 1);
        return kResultTrue;
}

IPlugView* PLUGIN_API GVstController::createView(const char* name)
{
        if (name && std::string(name) == std::string("editor"))
                return static_cast<IPlugView*>(new GKickVstEditor(this));
        return nullptr;
}

tresult PLUGIN_API GeonkickVstEffect::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                                         int32 numIns,
                                                         Vst::SpeakerArrangement* outputs,
                                                         int32 numOuts)
{
        RK_LOG_INFO("numIns: " << numIns);
        RK_LOG_INFO("numOuts: " << numOuts);
        if (numIns == 0 && numOuts == 1)
                return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        return kResultFalse;
}

tresult PLUGIN_API GeonkickVstEffect::setupProcessing(Vst::ProcessSetup& setup)
{
        RK_LOG_INFO("called");
        return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API GeonkickVstEffect::setActive(TBool state)
{
        RK_LOG_INFO("called");
        return AudioEffect::setActive(state);
}

tresult PLUGIN_API GeonkickVstEffect::process(Vst::ProcessData& data)
{
	auto eventList = data.inputEvents;
	if (eventList) {
		auto nEvents = eventList->getEventCount();
		for (decltype(nEvents) i = 0; i < nEvents; i++) {
			Event event;
			if (eventList->getEvent(i, event) == kResultOk) {
				switch (event.type)
                                        {
                                        case Event::kNoteOnEvent:
                                                RK_LOG_INFO("Note On Event");
                                                break;
                                        case Event::kNoteOffEvent:
                                                RK_LOG_INFO("Note Off Event");
                                                break;
                                        default:
                                                RK_LOG_INFO("Unknwon Event");
                                        }
			}
		}
	}

        // To clarify if it is needed...
        //	if (data.numInputs == 0 || data.numOutputs == 0)
	//	return kResultOk;

        auto numChannels = data.outputs[0].numChannels;
        auto numSamples = data.numSamples;
	auto outBuff = getChannelBuffersPointer(processSetup, data.outputs[0]);

	// Is this needed here?...
        //	data.outputs[0].silenceFlags = 0;

	if (!bBypass) {
                /*
			if (data.symbolicSampleSize == kSample32)
				fVuPPM = processAudio<Sample32> ((Sample32**)in, (Sample32**)out, numChannels,
				                                 data.numSamples, gain);
			else
				fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels,
				                                 data.numSamples, gain);
                */
                // Write into out buffer... outBuff
	}

        return kResultOk;
}

tresult PLUGIN_API GeonkickVstEffect::setState(IBStream* state)
{
        RK_LOG_INFO("called");
        if (!state)
                return kResultFalse;
        return kResultOk;
}

tresult PLUGIN_API GeonkickVstEffect::getState(IBStream* state)
{
        RK_LOG_INFO("called");
        return kResultOk;
}

FUnknown* GeonkickVstEffect::createInstance(void*)
{
        RK_LOG_INFO("called");
        return static_cast<Vst::IAudioProcessor*>(new GeonkickVstEffect());
}

} // namespace Steinberg

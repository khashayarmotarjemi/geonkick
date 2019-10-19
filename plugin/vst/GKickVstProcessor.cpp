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

#include "GKickVstProcessor.h"
#include "VstIds.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "geonkick_api.h"

namespace Steinberg
{

GKickVstProcessor::GKickVstProcessor()
        : geonkickApi{nullptr}
{
        setControllerClass(GKickVstControllerUID);
}

tresult PLUGIN_API GKickVstProcessor::initialize(FUnknown* context)
{
        auto res = Vst::AudioEffect::initialize(context);
        if (res != kResultTrue)
                return kResultFalse;

        addAudioOutput(STR16("AudioOutput"), Vst::SpeakerArr::kStereo);
        addEventInput(STR16("MIDI in"), 1);

        geonkickApi = std::make_unique<GeonkickApi>();
        if (!geonkickApi->init()) {
                GEONKICK_LOG_ERROR("can't init Geonkick API");
                return kResultFalse;
        }
        return kResultTrue;
}

tresult PLUGIN_API GKickVstProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                                         int32 numIns,
                                                         Vst::SpeakerArrangement* outputs,
                                                         int32 numOuts)
{
        GEONKICK_LOG_INFO("called");
        if (numIns == 0 && numOuts == 1)
                return Vst::AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        return kResultFalse;
}

tresult PLUGIN_API GKickVstProcessor::setupProcessing(Vst::ProcessSetup& setup)
{
        GEONKICK_LOG_INFO("called");
        return Vst::AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API GKickVstProcessor::setActive(TBool state)
{
        GEONKICK_LOG_INFO("called");
        return Vst::AudioEffect::setActive(state);
}

tresult PLUGIN_API GKickVstProcessor::process(Vst::ProcessData& data)
{
        if (data.numSamples > 0) {
                //		SpeakerArrangement arr;
                //		getBusArrangement(kOutput, 0, arr);
		auto numChannels = 2;
                auto events = data.inputEvents;
                auto nEvents = events->getEventCount();
                auto eventIndex = 0;
                for (decltype(data.numSamples) i = 0; i < data.numSamples; i++) {
                        if (eventIndex < nEvents) {
                                Vst::Event event;
                                events->getEvent(eventIndex++, event);
                                if (event.sampleOffset == i) {
                                        switch (event.type) {
                                        case Vst::Event::kNoteOnEvent:
                                                geonkickApi->setKeyPressed(true,
                                                                           event.noteOn.pitch,
                                                                           event.noteOn.velocity);
                                                break;

                                        case Vst::Event::kNoteOffEvent:
                                                geonkickApi->setKeyPressed(false,
                                                                           event.noteOff.pitch,
                                                                           event.noteOff.velocity);
                                                break;
                                        default:
                                                break;
                                        }
                                }
                        }
                        auto val = geonkickApi->getAudioFrame();
                        data.outputs[0].channelBuffers32[0][i] = val;
                        data.outputs[0].channelBuffers32[1][i] = val;
                }
	}

        return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::setState(IBStream* state)
{
        GEONKICK_LOG_INFO("called");
        if (!state)
                return kResultFalse;
        return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::getState(IBStream* state)
{
        GEONKICK_LOG_INFO("called");
		return kResultOk;
}

FUnknown* GKickVstProcessor::createInstance(void*)
{
        GEONKICK_LOG_INFO("called");
        return static_cast<Vst::IAudioProcessor*>(new GKickVstProcessor());
}

} // namespace Steinberg

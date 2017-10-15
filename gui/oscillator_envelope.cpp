/**
 * File name: oscillator_envelope.cpp
 * Project: GeonKick
 *
 * Copyright (C) 2015 Iurie Nistor
 * Email: nistor@iurie.org
 *
 * This file is part of GeonKick.
 *
 * waveToMp3 is free software; you can redistribute it and/or modify
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

#include "oscillator_envelope.h"
#include "gkick_oscillator.h"

OscillatorEnvelope::OscillatorEnvelope(std::shared_ptr<GKickOscillator> &osc)
        : oscillator(osc)
{
        if (osc->getType() == GKickOscillator::OSC_1) {
                setCategory(GKickEnvelope::ENV_CATEGORY_OSC_1);
        } else if(osc->getType() == GKickOscillator::OSC_2) {
                setCategory(GKickEnvelope::ENV_CATEGORY_OSC_2);
        } else {
                setCategory(GKickEnvelope::ENV_CATEGORY_NOISE);
        }
}

OscillatorEnvelope::~OscillatorEnvelope()
{
}

void OscillatorEnvelope::pointAddedEvent(double x, double y)
{
        if (type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                oscillator->addEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_APLITUDE, x, y);
        } else {
                oscillator->addEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_FREQUENCY, x, y);
        }
}

void OscillatorEnvelope::pointUpdatedEvent(unsigned int index, double x, double y)
{
        if (type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                oscillator->updateEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_APLITUDE, index, x, y);
        } else {
                oscillator->updateEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_FREQUENCY, index, x, y);
        }
}

void OscillatorEnvelope::pointRemovedEvent(unsigned int index)
{
        if (type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                oscillator->removeEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_APLITUDE, index);
        } else {
                oscillator->removeEnvelopePoint(GKickOscillator::OSC_ENV_TYPE_FREQUENCY, index);
        }
}

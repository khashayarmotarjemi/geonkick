/**
 * File name: general_envelope.cpp
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

GeneralEnvelope::GeneralEnvelope(std::shared_ptr<GKickApi> &api)
        : gkickApi(api)
{
}

GeneralEnvelope::~GeneralEnvelope()
{
}

void GeneralEnvelope::pointEddedEvent(double x, double y)
{
        gkickApi->addEnvelopePoint(x, y);
}

void GeneralEnvelope::pointUpdatedEvent(unsigned int index, double x, double y)
{
        gkickApi->updateEnvelopePoint(index, x, y);
}

void GeneralEnvelope::pointRemovedEvent(unsigned int index)
{
        gkickApi->removeEnvelopePoint(index);
}
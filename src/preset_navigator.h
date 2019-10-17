/**
 * File name: preset_navigator.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
 *
 * This file is part of Geonkick.
 *
 * Geonkick is free software; you can redistribute it and/or modify
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

#ifndef GEONKICK_PRESET_NAVIGATOR_H
#define GEONKICK_PRESET_NAVIGATOR_H

#include "globals.h"
#include "geonkick_widget.h"

class PresetBrowserModel;

/**
 * PresetNavigator
 *
 * A small widget that offers view and navigation throught presets.
 */
class PresetNavigator: public GeonkickWidget {
 public:
        PresetNavigator(GeonkickWidget *parent, PresetBrowserModel* model);
        RK_DECL_ACT(openPresetBrowser, openPresetBrowser(),
                    RK_ARG_TYPE(), RK_ARG_VAL());
 protected:
        void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) final;
 private:
        PresetBrowserModel *browserModel;
};

#endif // GEONKICK_PRESET_NAVIGATOR_H

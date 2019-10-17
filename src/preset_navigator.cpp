/**
 * File name: preset_navigator.cpp
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

#include "preset_navigator.h"
#include "preset_browser_model.h"

extern const unsigned char rk_navigator_bk_png[];

PresetNavigator::PresetNavigator(GeonkickWidget *parent, PresetBrowserModel* model)
        : GeonkickWidget(parent)
        , browserModel{model}
        // , prevButton{nullptr}
        // , nextButton{nullptr}
{
        setFixedSize(230, 30);
        setBackgroundImage(RkImage(size(), rk_navigator_bk_png));
        // prevButton = new GeonnkickButton(this);
        // openFileButton->setSize(0, height());
        // nextButton = new GeonnkickButton(this);
        // openFileButton->setSize(0, height());
        show();
}

void PresetNavigator::mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event)
{
        // if (event->x() > prevButton->x() + prevButton->width()
        //     && event->x() < nextButton->x() + nextButton->width())
        action openPresetBrowser();
}


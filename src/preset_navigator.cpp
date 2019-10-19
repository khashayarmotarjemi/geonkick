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
#include "geonkick_button.h"

extern const unsigned char rk_navigator_label_png[];
extern const unsigned char rk_navigator_prev_png[];
extern const unsigned char rk_navigator_prev_pressed_png[];
extern const unsigned char rk_navigator_next_png[];
extern const unsigned char rk_navigator_next_pressed_png[];

PresetNavigator::PresetNavigator(GeonkickWidget *parent, PresetBrowserModel* model)
        : GeonkickWidget(parent)
        , browserModel{model}
        , prevButton{nullptr}
        , nextButton{nullptr}
        , presetLabel{nullptr}
{
        setFixedSize(230, 30);

        prevButton = new GeonkickButton(this);
        prevButton->setSize(18, height());
        prevButton->setPosition(0, 0);
        prevButton->setPressedImage(RkImage(prevButton->size(), rk_navigator_prev_pressed_png));
        prevButton->setUnpressedImage(RkImage(prevButton->size(), rk_navigator_prev_png));
        RK_ACT_BIND(prevButton, toggled, RK_ACT_ARGS(bool b), browserModel, previousPreset());

        nextButton = new GeonkickButton(this);
        nextButton->setSize(18, height());
        nextButton->setPosition(width() - nextButton->width(), 0);
        nextButton->setPressedImage(RkImage(nextButton->size(), rk_navigator_next_pressed_png));
        nextButton->setUnpressedImage(RkImage(nextButton->size(), rk_navigator_next_png));
        RK_ACT_BIND(nextButton, toggled, RK_ACT_ARGS(bool b), browserModel, nextPreset());

        presetLabel = new NavigatorLabel(this);
        auto preset = model->selectedPreset();
        presetLabel->setText("Hit-Hat Closed");
        presetLabel->setBackgroundColor(background());
        presetLabel->setSize(width() - 2 * prevButton->width(), 26);
        presetLabel->setPosition(prevButton->x() + prevButton->width(), 2);
        presetLabel->show();
        setBackgroundPosition(18, 0);
        setBackgroundImage(RkImage(196, 30, rk_navigator_label_png));
        RK_ACT_BIND(presetLabel, pressed, RK_ACT_ARGS(), this, openPresetBrowser());

        show();
}


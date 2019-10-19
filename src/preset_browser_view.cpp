/**
 * File name: preset_browser_view.cpp
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

#include "preset_browser_view.h"
#include "preset_browser_model.h"

#include <RkListView.h>

PresetBrowserView::PresetBrowserView(GeonkickWidget *parent, PresetBrowserModel *model)
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)
        , presetBrowserModel{model}
        , bundleListView{nullptr}
        , bundleGroupsView{nullptr}
        , presetListView{nullptr}

{
        setModality(Rk::Modality::NonModal);
        setFixedSize(940, 600);
        setTitle("Preset Browser - " + std::string(GEOKICK_APP_NAME));
        show();

        int padding = 15;
        int headerHeight = 50;
        auto listSize = RkSize((width() - 4 * padding) / 3, height() - headerHeight - padding);

        bundleListView = new RkListView(this);
        bundleListView->setBackgroundColor({40, 40, 40});
        bundleListView->setFixedSize(listSize);
        bundleListView->setPosition(padding, headerHeight);

        bundleGroupsView  = new RkListView(this);
        bundleGroupsView->setBackgroundColor({40, 40, 40});
        bundleGroupsView->setFixedSize(listSize);
        bundleGroupsView->setPosition(bundleListView->x() + bundleListView->width() + padding,
                                      headerHeight);

        presetListView = new RkListView(this);
        presetListView->setBackgroundColor({40, 40, 40});
        presetListView->setFixedSize(listSize);
        presetListView->setPosition(bundleGroupsView->x() + bundleGroupsView->width() + padding,
                                    headerHeight);
        setModel(presetBrowserModel);
}

void PresetBrowserView::setModel(PresetBrowserModel *model)
{
        if (model) {
                presetBrowserModel = model;
                bundleListView->setModel(model->getBundlesModel());
                bundleGroupsView->setModel(model->getGroupsModel());
                presetListView->setModel(model->getPresetsModel());
        }
}

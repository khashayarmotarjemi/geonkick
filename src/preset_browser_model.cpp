/**
 * File name: preset_browser_model.cpp
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

#include "globals.h"
#include "geonkick_api.h"

#include <libconfig.h++>

PresetBrowserModel::PresetBrowserModel(GeonkickApi *api, const std::string &path)
        : geonkickApi{api},
          configFilePath{path}
{
        loadData();
}

void PresetBrowserModel::loadData()
{
        libconfig::Config cfg;
        try {
                cfg.readFile(configFilePath);
        } catch(...) {
                GEONKICK_LOG_ERROR("can't read config file" << configFilePath);
                return;
        } catch(const ParseException &pex) {
                GEONKICK_LOG_ERROR("parse error at " << pex.getFile()
                                   << ":" << pex.getLine()
                                   << " - " << pex.getError());
                return;
        }

        // Load the preset bundles list and their data.
        const Setting& root = cfg.getRoot();
        try {
                const Setting &presetBundles = root["PresetBundles"];
                for (const auto &presetBundle: presetBundles) {
                        std::string path;
                        if (presetBundle.lookupValue("name")
                            && presetBundle.lookupValue("path", path))
                                loadPresetBundle(path);
                }
        } catch(const SettingNotFoundException &nfex) {
                GEONKICK_LOG_WARNING("an error occurred on getting preset bundles info");
        }
}

void PresetBrowserModel::loadPresetBundle(const std::stirng &path)
{

}

void PresetBrowserModel::setPresetsPath(const std::path &path)
{
}

const std::string& PresetBrowserModel::getPresetsPath() const
{
}

void PresetBrowserModel::setPresetGroup(int index)
{
}

const PresetGroup& PresetBrowserModel::presetGroup(int index) const
{
}

void PresetBrowserModel::setPresetSubGroup(int index)
{
}

const PresetGroup& PresetBrowserModel::presetSubGroup(int index) const
{
}

void PresetBrowserModel::setPreset(int index)
{
}

const Preset& PresetBrowserModel::getPreset(int index) const
{
}

const std::vector<PresetGroup>& PresetBrowserModel::getGroups() const
{
}

const std::vector<PresetGroup>& PresetBrowserModel::getSubGroups() const
{
}

const std::vector<Preset>& PresetBrowserModel::getPresets() const
{
}

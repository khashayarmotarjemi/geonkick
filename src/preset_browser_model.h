/**
 * File name: preset_browser_model.h
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

#ifndef GEONKICK_PRESET_BROWSER_MODEL_H
#define GEONKICK_PRESET_BROWSER_MODEL_H

#include "globals.h"
#include "geonkick_api.h"

struct PresetGroup {
        std::string name;
        std::string autor;
        std::string autorUrl;
        std::string license;
        std::path path;
};

struct Preset {
        std::string name;
        std::string autor;
        std::string autorUrl;
        std::string license;
        std::path filepath;
};

/**
 * PresetBrowserModel class.
 *
 * Preset browser is based on the slightly modified MVC pattern.
 * But in simple words PresetBrowserModel is the backend class
 * that works with the data.
 * PresetBrowserModel holds a state that can be changed
 * by using setters.
 */
class PresetBrowserModel {
 public:
        PresetBrowserModel(GeonkickApi *api, const std::path &path);
        void setPresetsPath(const std::path &path);
        const std::string& getPresetsPath() const;
        void setPresetGroup(int index);
        const PresetGroup& presetGroup(int index) const;
        void setPresetSubGroup(int index);
        const PresetGroup& presetSubGroup(int index) const;
        void setPreset(int index);
        const Preset& getPreset(int index) const;
        const std::vector<PresetGroup>& getGroups() const;
        const std::vector<PresetGroup>& getSubGroups() const;
        const std::vector<Preset>& getPresets() const;

protected:
        void loadData();
        void saveData();
        void loadPresetBundle();

 private:
        std::string configFilePath;
        std::vector<PresetGroup> presetsGroups;
        std::vector<PresetGroup> presetsSubGroups;
        std::vector<Preset> presetList;
};

#endif // GEONKICK_PRESET_BROWSER_MODEL_H

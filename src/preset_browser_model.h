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

struct Preset {
        std::string name;
        std::string author;
        std::string url;
        std::string license;
        std::path path;
};

struct BundleGroup {
        std::string name;
        std::vector<std::unique_ptr<Preset>> presets;
};

struct PresetBundle {
        std::string name;
        std::string author;
        std::string url;
        std::string license;
        std::path path;
        std::vector<std::qnique_ptr<PresetGroup>> groups;
}

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
        ~PresetBrowserModel();
        const std::string& getPresetsPath() const;
        void setPresetBundle(int index);
        const PresetBundle* presetBundle(int index) const;
        void setPresetGroup(int index);
        const PresetGroup* presetGroup(int index) const;
        void setPreset(int index);
        const Preset* getPreset(int index) const;

 protected:
        void loadData();
        bool loadPresetBundle(struct PresetBundle &bundle,
                              const std::string &path);
        void loadBundleGroups(std::vector<std::uniqeu_ptr<BundleGroup>> &bundleGroups,
                              const rapidjson::Value &groups,
                              const std::string& path);
        void loadGroupPresets(std::vector<std::unique_ptr<Preset>> &groupPresets,
                              const rapidjson::Value &presets,
                              const std::string& path);

 private:
        std::string configFilePath;
        std::vector<std::unique_ptr<PresetBundle>> browserBundles;
        int presetBundleIndex;
        int bundleGroupIndex;
        int presetIndex;
};

#endif // GEONKICK_PRESET_BROWSER_MODEL_H

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

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class RkEventQueue;
class GeonkickApi;
class RkModel;

struct Preset {
        std::string name;
        std::string author;
        std::string authorUrl;
        std::string license;
        std::filesystem::path path;
};

struct PresetGroup {
        std::string name;
        std::vector<std::unique_ptr<Preset>> presets;
};

struct PresetBundle {
        std::string name;
        std::string author;
        std::string authorUrl;
        std::string license;
        std::filesystem::path path;
        std::vector<std::unique_ptr<PresetGroup>> groups;
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
        PresetBrowserModel(GeonkickApi *api,
                           RkEventQueue *queue);
        const std::string& getPresetsPath() const;
        void setPresetBundle(int index);
        const PresetBundle* presetBundle(int index) const;
        void setPresetGroup(int index);
        const PresetGroup* presetGroup(int index) const;
        void setPreset(int index);
        const Preset* getPreset(int index) const;

        RkModel* bundlesModel() const;
        RkModel* groupsModel() const;
        RkModel* presetsModel() const;

 protected:
        void loadData();
        bool loadPresetBundle(const std::unique_ptr<PresetBundle> &bundle,
                              const std::string &path);
        void loadPresetGroups(std::vector<std::unique_ptr<PresetGroup>> &presetGroups,
                              const rapidjson::Value &groups);
        void loadPresets(std::vector<std::unique_ptr<Preset>> &presets,
                         const rapidjson::Value &presetsArray);
        bool loadPresetInfo(const std::unique_ptr<Preset> &preset,
                            const rapidjson::Value &presetValue);

 private:

        class BundlesModel : public RkModel {
                BundlesModel(RkEventQueue *eventQueue) : RkModel(eventQueue);
                RkVariant data(int index, DataType type) const final
                {
                        const bundle = presetBundle(index);
                        if (bundle) {
                                if (type == DataType::Text)
                                        return RkVaraint(bundle.name);
                                else if (RkModel::DataType::Color && index == selectedIndex())
                                        return RkVariant(RkColor(255, 255, 255));
                                else if (type == RkModel::DataType::Background && index == selectedIndex())
                                        return RkVariant(RkColor(100, 100, 200));

                        }
                        return RkVariant();
                }

                size_t rows() const final
                {
                        return presetBundles().size();
                }
        };

        class GroupsModel : public RkModel {
                GroupsModel(RkEventQueue *eventQueue) : RkModel(eventQueue);
        };

        class PresetsModel : public RkModel {
                PresetsModel(RkEventQueue *eventQueue) : RkModel(eventQueue);
        };

        GeonkickApi *geonkickApi;
        RkEventQueue *eventQueue;
        std::vector<std::unique_ptr<PresetBundle>> browserBundles;
        int presetBundleIndex;
        int presetGroupIndex;
        int presetIndex;
};

#endif // GEONKICK_PRESET_BROWSER_MODEL_H

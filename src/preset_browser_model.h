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

#include <RkModel.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class RkEventQueue;
class GeonkickApi;

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

        RkModel* getBundlesModel() const;
        RkModel* getGroupsModel() const;
        RkModel* getPresetsModel() const;
        RK_DECL_ACT(presetSelected,
                    presetSelected(const Preset* preset),
                    RK_ARG_TYPE(const Preset*),
                    RK_ARG_VAL(preset));
        void previousPreset();
        void nextPreset();
        const Preset* selectedPreset() const;

 protected:
        void loadData();
        bool loadPresetBundle(const std::unique_ptr<PresetBundle> &bundle,
                              const std::string &path);
        void loadPresetGroups(std::vector<std::unique_ptr<PresetGroup>> &presetGroups,
                              const rapidjson::Value &groups,
                              const std::string &path);
        void loadPresets(std::vector<std::unique_ptr<Preset>> &presets,
                         const rapidjson::Value &presetsArray,
                         const std::string &path);
        bool loadPresetInfo(const std::unique_ptr<Preset> &preset,
                            const rapidjson::Value &presetValue,
                            const std::string &path);

 private:

        class BundlesModel final : public RkModel {
          public:
                BundlesModel(PresetBrowserModel &parent)
                        : RkModel(parent.eventQueue)
                        , parentModel{parent}
                        , currentIndex{parent.presetBundleIndex} {}
                RkVariant data(int index, DataType type) const
                {
                        const auto bundle = parentModel.presetBundle(index);
                        if (bundle) {
                                if (type == DataType::Text)
                                        return RkVariant(bundle->name);
                                else if (type == RkModel::DataType::Color && index == selectedIndex())
                                        return RkVariant(RkColor(255, 255, 255));
                                else if (type == RkModel::DataType::Background && index == selectedIndex())
                                        return RkVariant(RkColor(100, 100, 200));

                        }
                        return RkVariant();
                }

                size_t rows() const
                {
                        return parentModel.browserBundles.size();
                }

                void selectIndex(int index)
                {
                        if (rows() > 0) {
                                currentIndex = index;
                                if (currentIndex == parentModel.presetBundleIndex)
                                        parentModel.groupsModel->selectIndex(parentModel.presetGroupIndex);
                                else
                                        parentModel.groupsModel->selectIndex(-1);
                                parentModel.groupsModel->modelChanged();
                        }
                }

                int selectedIndex() const
                {
                        if (rows() < 1)
                                return -1;
                        return currentIndex;
                }

          private:
                PresetBrowserModel &parentModel;
                int currentIndex;
        };

        class GroupsModel final : public RkModel {
          public:
                GroupsModel(PresetBrowserModel& parent)
                        : RkModel(parent.eventQueue)
                        , parentModel{parent}
                        , currentIndex{parent.presetGroupIndex} {}
                RkVariant data(int index, DataType type) const
                {
                        const auto group = parentModel.presetGroup(index);
                        if (group) {
                                if (type == DataType::Text)
                                        return RkVariant(group->name);
                                else if (type == RkModel::DataType::Color && index == selectedIndex())
                                        return RkVariant(RkColor(255, 255, 255));
                                else if (type == RkModel::DataType::Background && index == selectedIndex())
                                        return RkVariant(RkColor(100, 100, 200));
                        }
                        return RkVariant();
                }

                size_t rows() const
                {
                        const auto bundle = parentModel.presetBundle(parentModel.bundlesModel->selectedIndex());
                        if (bundle)
                                return bundle->groups.size();
                        return 0;
                }

                void selectIndex(int index)
                {
                        if (rows() > 0) {
                                currentIndex = index;
                                if (currentIndex == parentModel.presetGroupIndex
                                    && parentModel.bundlesModel->selectedIndex() == parentModel.presetBundleIndex)
                                        parentModel.presetsModel->selectIndex(parentModel.presetIndex);
                                else
                                        parentModel.presetsModel->selectIndex(-1);
                                parentModel.presetsModel->modelChanged();
                        }
                }

                int selectedIndex() const
                {
                        if (rows() < 1)
                                return -1;
                        return currentIndex;
                }

          private:
                PresetBrowserModel& parentModel;
                int currentIndex;
        };

        class PresetsModel final : public RkModel {
          public:
                PresetsModel(PresetBrowserModel &parent)
                        : RkModel(parent.eventQueue)
                        , parentModel{parent}
                        , currentIndex{parent.presetIndex} {}
                RkVariant data(int index, DataType type) const
                {
                        const auto preset = parentModel.getPreset(index);
                        if (preset) {
                                if (type == DataType::Text)
                                        return RkVariant(preset->name);
                                else if (type == RkModel::DataType::Color && index == selectedIndex())
                                        return RkVariant(RkColor(255, 255, 255));
                                else if (type == RkModel::DataType::Background && index == selectedIndex())
                                        return RkVariant(RkColor(100, 100, 200));
                        }
                        return RkVariant();
                }

                size_t rows() const
                {
                        const auto group = parentModel.presetGroup(parentModel.groupsModel->selectedIndex());
                        if (group)
                                return group->presets.size();
                        return 0;
                }

                void selectIndex(int index)
                {
                        if (rows() > 0) {
                                currentIndex = index;
                                parentModel.setPreset(index);
                        }
                }

                int selectedIndex() const
                {
                        if (rows() < 1)
                                return -1;
                        return currentIndex;
                }

          private:
                PresetBrowserModel &parentModel;
                int currentIndex;
        };

        GeonkickApi *geonkickApi;
        RkEventQueue *eventQueue;
        std::vector<std::unique_ptr<PresetBundle>> browserBundles;
        int presetBundleIndex;
        int presetGroupIndex;
        int presetIndex;
        std::unique_ptr<BundlesModel> bundlesModel;
        std::unique_ptr<GroupsModel> groupsModel;
        std::unique_ptr<PresetsModel> presetsModel;
};

#endif // GEONKICK_PRESET_BROWSER_MODEL_H

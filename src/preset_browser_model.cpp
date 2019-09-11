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

#include "preset_browser_model.h"
#include "geonkick_api.h"

#include <RkEventQueue.h>

#include <libconfig.h++>

PresetBrowserModel::PresetBrowserModel(GeonkickApi *api, RkEventQueue *queue)
        : geonkickApi{api}
        , eventQueue{queue}
        , presetBundleIndex{-1}
        , presetGroupIndex{-1}
        , presetIndex{-1}
{
        loadData();
}

void PresetBrowserModel::loadData()
{
        libconfig::Config cfg;
        auto configFilePath = geonkickApi->getSettings("Config/PresetBundels");
        try {
                cfg.readFile(configFilePath.c_str());
        } catch(...) {
                GEONKICK_LOG_ERROR("can't read config file" << configFilePath);
                return;
        }

        // Load the preset bundles list and their data.
        const libconfig::Setting& root = cfg.getRoot();
        try {
                const libconfig::Setting &presetBundles = root["PresetBundles"];
                size_t n = 0;
                for (const auto &presetBundle: presetBundles) {
                        std::string path;
                        std::string name;
                        if (presetBundle.lookupValue("name", name)
                            && presetBundle.lookupValue("path", path)) {
                                auto bundle = std::make_unique<PresetBundle>();
                                bundle->name = name.empty() ? "Unknown " + std::to_string(n++) : name;
                                if (loadPresetBundle(bundle, path))
                                        browserBundles.emplace_back(std::move(bundle));
                        }
                }
        } catch(const libconfig::SettingNotFoundException &nfex) {
                GEONKICK_LOG_ERROR("an error occurred on getting preset bundles info");
        }
}

bool PresetBrowserModel::loadPresetBundle(const std::unique_ptr<PresetBundle> &bundle,
                                          const std::string &path)
{
        std::filesystem::path filePath(path);
        if (filePath.extension().empty()
            || (filePath.extension() != ".gkickp"
            && filePath.extension() != ".GKICKP")) {
                RK_LOG_ERROR("can't open preset bundle. Wrong file format.");
                return false;
        }

        std::ifstream file;
        file.open(std::filesystem::absolute(filePath));
        if (!file.is_open()) {
                RK_LOG_ERROR("can't open preset: " << filePath);
                return false;
        }
        std::string fileData((std::istreambuf_iterator<char>(file)),
                             (std::istreambuf_iterator<char>()));

        rapidjson::Document document;
        document.Parse(fileData.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                bundle->name = m.value.GetString();
                        if (m.name == "author"  && m.value.IsString())
                                bundle->author = m.value.GetString();
                        if (m.name == "authorUrl"  && m.value.IsString())
                                bundle->authorUrl = m.value.GetString();
                        if (m.name == "license"  && m.value.IsString())
                                bundle->license = m.value.GetString();
                        if (m.name == "presetGroups" && m.value.IsArray())
                                loadPresetGroups(bundle->groups, m.value);
                }
        }
        return true;
}

void PresetBrowserModel::loadPresetGroups(std::vector<std::unique_ptr<PresetGroup>> &bundleGroups,
                                          const rapidjson::Value &groups)
{
        size_t n = 0;
        for (const auto &e: groups.GetObject()) {
                auto group = std::make_unique<PresetGroup>();
                group->name = "Unknown " + std::to_string(n++);
                if (e.name == "name" && e.value.IsString())
                        group->name = e.value.GetString();
                if (e.name == "presets" && e.value.IsArray())
                        loadPresets(group->presets, e.value);
                bundleGroups.emplace_back(std::move(group));
        }
}

void PresetBrowserModel::loadPresets(std::vector<std::unique_ptr<Preset>> &presets,
                                     const rapidjson::Value &presetsArray)
{
        size_t n = 0;
        for (const auto &e: presetsArray.GetArray()) {
                auto preset = std::make_unique<Preset>();
                preset->name = std::to_string(n++);
                if (loadPresetInfo(preset, e))
                        presets.emplace_back(std::move(preset));
        }
}

bool PresetBrowserModel::loadPresetInfo(const std::unique_ptr<Preset> &preset,
                                        const rapidjson::Value &presetValue)
{
        if (presetValue.IsObject()) {
                for (const auto &m: presetValue.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                preset->name = m.value.GetString();
                        if (m.name == "author"  && m.value.IsString())
                                preset->author = m.value.GetString();
                        if (m.name == "authorUrl"  && m.value.IsString())
                                preset->authorUrl = m.value.GetString();
                        if (m.name == "license"  && m.value.IsString())
                                preset->license = m.value.GetString();
                }
        }

        return true;
}

void PresetBrowserModel::setPresetBundle(int index)
{
        if (!browserBundles.empty() && index > -1 && index < browserBundles.size())
                presetBundleIndex = index;
}

const PresetBundle* PresetBrowserModel::presetBundle(int index) const
{
        if (!browserBundles.empty() && index > -1
            && index < browserBundles.size())
                return browserBundles[index].get();
        return nullptr;
}

void PresetBrowserModel::setPresetGroup(int index)
{
        if (!browserBundles.empty() && presetBundleIndex > -1
            && presetBundleIndex < browserBundles.size()) {
               auto &groups = browserBundles[presetBundleIndex]->groups;
                if (!groups.empty() && index > -1 && index < groups.size())
                        presetGroupIndex = index;
        }
}

const PresetGroup* PresetBrowserModel::presetGroup(int index) const
{
        if (!browserBundles.empty() && presetBundleIndex > -1
            && presetBundleIndex < browserBundles.size()) {
                auto &groups = browserBundles[presetBundleIndex]->groups;
                if (!groups.empty() && index > -1 && index < groups.size())
                        return groups[index].get();
        }
        return nullptr;
}

void PresetBrowserModel::setPreset(int index)
{
        if (!browserBundles.empty() && presetBundleIndex > -1
            && presetBundleIndex < browserBundles.size()) {
                auto &groups = browserBundles[presetBundleIndex]->groups;
                if (!groups.empty() && presetGroupIndex > -1 && presetGroupIndex < groups.size()) {
                        auto &presets = groups[presetGroupIndex]->presets;
                        if (!presets.empty() && index > -1 && index < groups.size())
                                presetIndex = index;
                }
        }
}

const Preset* PresetBrowserModel::getPreset(int index) const
{
        if (!browserBundles.empty() && presetBundleIndex > -1
            && presetBundleIndex < browserBundles.size()) {
                auto &groups = browserBundles[presetBundleIndex]->groups;
                if (!groups.empty() && presetGroupIndex > -1 && presetGroupIndex < groups.size()) {
                        auto &presets = groups[presetGroupIndex]->presets;
                        if (!presets.empty() && index > -1 && index < groups.size())
                                return presets[index].get();
                }
        }
        return nullptr;
}

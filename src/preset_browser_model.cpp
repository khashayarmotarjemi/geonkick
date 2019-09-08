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

PresetBrowserModel::~PresetBrowserModel()
{
        freePresetTree();
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
                size_t n = 0;
                for (const auto &presetBundle: presetBundles) {
                        std::string path;
                        if (presetBundle.lookupValue("name")
                            && presetBundle.lookupValue("path", path)) {
                                PresetBundle bundle;
                                bundle.name = "Unknown " + std::to_string(n++);
                                if (loadPresetBundle(&bundle, path))
                                        browserBundles.push_back(bundle);
                        }
                }
        } catch(const SettingNotFoundException &nfex) {
                GEONKICK_LOG_WARNING("an error occurred on getting preset bundles info");
        }
}

bool PresetBrowserModel::loadPresetBundle(struct PresetBundle &bundle, const std::string &path)
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
        std::string fileData((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

        rapidjson::Document document;
        document.Parse(fileData.c_str());
        if (document.IsObject()) {
                for (const auto &m: document.GetObject()) {
                        if (m.name == "name" && m.value.IsString())
                                bundle.name = m.value.GetString();
                        if (m.name == "author"  && m.value.IsString())
                                bundle.author = m.value.GetString();
                        if (m.name == "license"  && m.value.IsString())
                                bundle.license = m.value.GetString();
                        if (m.name == "groups" && m.value.IsArray())
                                loadBundleGroups(bundle.groups, m.value);
                }
        }
}

void PresetBrowserModel::loadBundleGroups(std::vector<BundleGroup> &bundleGroups, const rapidjson::Value &groups)
{
        size_t n = 0;
        for (const auto &e: groups.GetObject()) {
                if (!e.IsObject())
                        continue;
                BundleGroup group;
                group.name = "Unknown " + std::to_string(n++);
                if (e.name == "name" && e.value.IsString())
                        group.name = m.value.GetString();
                if (e.name == "presets" && e.value.IsArray())
                        loadGroupPresets(group.presets, m.value);
                bundleGroups.push_back(group);
        }
}

void PresetBrowserModel::loadGroupPresets(std::vector<Preset> &groupPresets, const rapidjson::Value &presets)
{
        size_t n = 0;
        for (const auto &e: groups.GetObject()) {
                if (!e.IsString())
                        continue;
                Preset preset;
                if (loadPresetInfo(&preset))
                        groupPresets.bush_back();
        }
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

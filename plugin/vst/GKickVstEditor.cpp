/**
 * File name: GKickVstEditor.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/geonkick)
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "GKickVstEditor.h"
#include "mainwindow.h"

#include <RkMain.h>
#include <RkPlatform.h>

GKickVstEditor::GKickVstEditor(GeonkickVstEffect *effect)
        : Vst::EditorView(effect)
        , guiApp{nullptr}
{
}

GKickVstEditor::~GKickVstEditor()
{
}

tresult PLUGIN_API GKickVstEditor::isPlatformTypeSupported(Steinberg::FIDString type)
{
        RK_UNUSED(type);
        return kResultTrue;
}

tresult PLUGIN_API GKickVstEditor::attached(void* parent, FIDString type)
{
        guiApp = std::make_unique<RkMain>();
        auto info = rk_from_native_win(nullptr, nullptr, reinterpret_cast<HWND>(parent));
        auto mainWindow = new MainWindow(guiApp.get(), geonkickApi, info);
        if (!mainWindow->init()) {
                RK_LOG_ERROR("can't create gui");
                guiApp.reset(nullptr);			
        }

        RK_LOG_INFO("show mainWindow");
        return Vst::EditorView::attached(parent, type);
}

tresult PLUGIN_API GKickVstEditor::removed()
{
        RK_LOG_INFO("called");
        if (guiApp)
                guiApp.reset(nullptr);
        return kResultOk;
}

tresult PLUGIN_API GKickVstEditor::getSize(ViewRect* newSize)
{
	if (newSize == nullptr || guiApp == nullptr || guiApp->topWindow())
		return kResultFalse;

        auto topWindow = guiApp->topWindow();
	newSize->left   = topWindow->x();
	newSize->right  = topWindow->x() + topWindow->width();
	newSize->top    = topWindow->y();
	newSize->bottom = topWindow->y() + topWindow->height();
	return kResultOk;
}


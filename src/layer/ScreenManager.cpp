/*
 * LOVE Lab
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ScreenManager.h"
#include <stdio.h>

#if defined(__LAYER_SDLOPENGL)
#include "ScreenSDLOpenGL.h"
#endif
namespace layer
{

ScreenManager ScreenManager::mScreenManager;

ScreenManager::ScreenManager()
{
	mScreen = NULL;
}

ScreenManager::~ScreenManager()
{
}

Screen* ScreenManager::createScreen(ScreenType type,
					int width,
					int height,
					int colorDepth,
					bool fullScreen,
					bool resizable,
					bool windowDecorations)
{
	switch (type)
	{
#if defined(__LAYER_SDLOPENGL)
	case SDLOpenGL:
		mScreen = new ScreenSDLOpenGL();
		break;
#endif
	default:
		mScreen = NULL;
	}

	if (mScreen != NULL)
	{
		if (!mScreen->init(width,
					height,
					colorDepth,
					fullScreen,
					resizable,
					windowDecorations))
		{
			delete mScreen;
			mScreen = NULL;
		}
	}

	return mScreen;
}

}


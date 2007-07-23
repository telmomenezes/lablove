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

#if defined(__LAYER_SDLOPENGL)

#include "ScreenSDLOpenGL.h"

#include "SDL.h"
#include "SDL_opengl.h" 

namespace layer
{

ScreenSDLOpenGL::ScreenSDLOpenGL()
{
}

ScreenSDLOpenGL::~ScreenSDLOpenGL()
{
}

bool ScreenSDLOpenGL::init(int width,
				int height,
				int colorDepth,
				bool fullScreen,
				bool resizable,
				bool windowDecorations)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		//printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}
 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
 
	if (SDL_SetVideoMode(width, height, colorDepth, SDL_OPENGL | SDL_FULLSCREEN) == NULL)
	{
		return false;
	}

	return true;
}

}

#endif


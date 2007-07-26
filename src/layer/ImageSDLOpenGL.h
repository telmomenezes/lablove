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

#if !defined(__INCLUDE_LAYER_IMAGE_SDL_OPENGL_H)
#define __INCLUDE_LAYER_IMAGE_SDL_OPENGL_H

#include "Image.h"

#include "SDL.h"
#include "SDL_opengl.h"

namespace layer
{

class ImageSDLOpenGL : public Image
{
public:
	ImageSDLOpenGL();
	virtual ~ImageSDLOpenGL();

	virtual bool loadPNG(std::string filePath);

	virtual void draw(float x, float y);

protected:
	GLuint mTexture;

	int mTextureWidth;
	int mTextureHeight;
};

}

#endif


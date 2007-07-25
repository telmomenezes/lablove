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
	if (!Screen::init(width, height, colorDepth, fullScreen, resizable, windowDecorations))
	{
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		//printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}
 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	unsigned int flags = SDL_OPENGL;

	if (fullScreen)
	{
		flags |= SDL_FULLSCREEN; 
	}

	if (resizable)
	{
		flags |= SDL_RESIZABLE;
	}

	if (!windowDecorations)
	{
		flags |= SDL_NOFRAME;
	}

	if (SDL_SetVideoMode(width, height, colorDepth, flags) == NULL)
	{
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glViewport(0, 0, mWidth, mHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0.0f, mWidth, mHeight, 0.0f, -1.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);

	return true;
}

void ScreenSDLOpenGL::setCaption(std::string caption)
{
	SDL_WM_SetCaption(caption.c_str(), NULL);
}

void ScreenSDLOpenGL::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void ScreenSDLOpenGL::endFrame()
{
	SDL_GL_SwapBuffers();
}

void ScreenSDLOpenGL::setColor(float red,
				float green,
				float blue,
				float alpha)
{
	glColor4f(red, green, blue, alpha);
}

void ScreenSDLOpenGL::setBackgroundColor(float red,
				float green,
				float blue)
{
	glClearColor(red, green, blue, 0.0f);
}

void ScreenSDLOpenGL::setPointSize(float size)
{
	glPointSize(size);
}

void ScreenSDLOpenGL::setLineWidth(float width)
{
	glLineWidth(width);
}

void ScreenSDLOpenGL::drawPoint(float x, float y)
{
	glBegin(GL_POINTS);
		glVertex3f(x, y, 0.0f);
	glEnd();
}

void ScreenSDLOpenGL::drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
	glEnd();
}

void ScreenSDLOpenGL::drawTriangle(float x1,
					float y1,
					float x2,
					float y2,
					float x3,
					float y3)
{
	glBegin(GL_LINE_STRIP);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x3, y3, 0.0f);
		glVertex3f(x1, y1, 0.0f);
	glEnd();
}

void ScreenSDLOpenGL::drawFilledTriangle(float x1,
					float y1,
					float x2,
					float y2,
					float x3,
					float y3)
{
	glBegin(GL_TRIANGLES);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x3, y3, 0.0f);
	glEnd();
}

void ScreenSDLOpenGL::drawFilledSquare(float x,
					float y,
					float rad,
					float rot)
{
	float ang = rot;
	float deltaAng = M_PI * 0.5;

	glBegin(GL_QUADS);

	for (unsigned int i = 0; i < 4; i++)
	{
		glVertex3f(x + (cosf(ang) * rad),
			y + (sinf(ang) * rad),
			0.0f);
		ang += deltaAng;
	}

	glEnd();
}

void ScreenSDLOpenGL::drawFilledCircle(float x,
					float y,
					float rad,
					float beginAngle,
					float endAngle)
{
	float ang = beginAngle;
	bool stop = false;

	glBegin(GL_POLYGON);

	glVertex3f(x, y, 0.0f);

	while (!stop)
	{
		if (ang >= endAngle)
		{
			ang = endAngle;
			stop = true;
		}

		glVertex3f(x + (cosf(ang) * rad),
			y + (sinf(ang) * rad),
			0.0f);
		ang += 0.1f;
	}

	glEnd();
}

}

#endif


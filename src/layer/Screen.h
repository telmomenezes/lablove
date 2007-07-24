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

#if !defined(__INCLUDE_LAYER_SCREEN_H)
#define __INCLUDE_LAYER_SCREEN_H

#include <math.h>
#include <string>

namespace layer
{

class Screen
{
public:
	Screen();
	virtual ~Screen();

	virtual bool init(int width,
		int height,
		int colorDepth=0,
		bool fullScreen=false,
		bool resizable=false,
		bool windowDecorations=true)=0;

	int getWidth(){return mWidth;}
	int getHeight(){return mHeight;}

	virtual void setCaption(std::string caption)=0;

	virtual void beginFrame()=0;
	virtual void endFrame()=0;

	virtual void setColor(float red,
				float green,
				float blue,
				float alpha=1.0f)=0;
	virtual void setBackgroundColor(float red,
				float green,
				float blue)=0;

	virtual void setPointSize(float size)=0;
	virtual void setLineWidth(float width)=0;

	virtual void drawPoint(float x, float y)=0;
	virtual void drawLine(float x1, float y1, float x2, float y2)=0;
	virtual void drawTriangle(float x1,
					float y1,
					float x2,
					float y2,
					float x3,
					float y3)=0;
	virtual void drawFilledTriangle(float x1,
					float y1,
					float x2,
					float y2,
					float x3,
					float y3)=0;
	virtual void drawFilledSquare(float x,
					float y,
					float rad,
					float rot=0.0f)=0;
	virtual void drawFilledCircle(float x,
					float y,
					float rad,
					float beginAngle=0.0f,
					float endAngle=M_PI)=0;

protected:
	int mWidth;
	int mHeight;
};

}

#endif


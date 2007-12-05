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

#if !defined(__INCLUDE_GRAPHIC_BIOMORPH_H)
#define __INCLUDE_GRAPHIC_BIOMORPH_H

#include "Graphic.h"
#include "Orbit.h"

#include "pyc.h"

class GraphicBiomorph : public Graphic
{
public:
    GraphicBiomorph(lua_State* luaState=NULL);
    virtual ~GraphicBiomorph();
    
    virtual Graphic* clone();

    virtual void init(SimulationObject* obj, pyc::Pycasso* pycasso);
    virtual void draw(pyc::Layer* layer);

    static const char mClassName[];
    static Orbit<GraphicBiomorph>::MethodType mMethods[];
    static Orbit<GraphicBiomorph>::NumberGlobalType mNumberGlobals[];

protected:
    void calcTree(float x, float y, int length, int dir);
    void normalizePoints();

    float mSize;
    int mRed;
    int mGreen;
    int mBlue;
    int mXIndex;
    int mYIndex;
    int mRotIndex;

    unsigned int mLineCount;
    float* mX1Points;
    float* mY1Points;
    float* mX2Points;
    float* mY2Points;
    unsigned int mCurrentLine;

    float mParameters[19];

    int mDX[8];
    int mDY[8];

    pyc::Window* mWindow;
    pyc::Layer2D* mLayer;
};
#endif


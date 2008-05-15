/*
 * LabLOVE
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

#include "GraphicSquare.h"
#include "SymbolRGB.h"
#include "SymbolFloat.h"
#include "Simulation.h"

#include <stdexcept>

GraphicSquare::GraphicSquare(lua_State* luaState)
{
    mObject = NULL;
    mSize = 0;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
}

GraphicSquare::~GraphicSquare()
{   
}

Graphic* GraphicSquare::clone()
{
    return new GraphicSquare();
}

void GraphicSquare::init(SimObj2D* obj)
{
    mObject = obj;

    mSize = obj->mSize;
    mSize = sqrtf(2.0 * mSize * mSize);

    SymbolRGB* color = (SymbolRGB*)(obj->getSymbolByName("color"));

    if (color != NULL)
    {
        mRed = color->mRed;
        mGreen = color->mGreen;
        mBlue = color->mBlue;
    }
}

void GraphicSquare::draw()
{
    float rot = mObject->mRot;
    float centerX = mObject->mX;
    float centerY = mObject->mY;

    art_setRotation(centerX, centerY, rot);
    art_setColor(mRed, mGreen, mBlue, 255);
    art_fillSquare(centerX, centerY, mSize);
    art_clearRotation();
}

const char GraphicSquare::mClassName[] = "GraphicSquare";

Orbit<GraphicSquare>::MethodType GraphicSquare::mMethods[] = {{0,0}};

Orbit<GraphicSquare>::NumberGlobalType GraphicSquare::mNumberGlobals[] = {{0,0}};


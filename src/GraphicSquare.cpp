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

#include "GraphicSquare.h"
#include "SimulationObject.h"
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

void GraphicSquare::init(SimulationObject* obj, art::Artist* artist)
{
    mObject = obj;

    mXIndex = obj->getNamedFloatDataIndex("x");
    if (mXIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicSquare: object does not define 'x' named data item");
    }
    mYIndex = obj->getNamedFloatDataIndex("y");
    if (mYIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicSquare: object does not define 'y' named data item");
    }
    mRotIndex = obj->getNamedFloatDataIndex("rot");
    if (mRotIndex == -1)
    {
        throw std::runtime_error("Failed to initialize GraphicSquare: object does not define 'rot' named data item");
    }

    SymbolFloat* symSize = (SymbolFloat*)obj->getSymbolByName("size");

    if (symSize == NULL)
    {
        throw std::runtime_error("Failed to initialize GraphicSquare: object does not define 'size' named symbol");
    }
    else
    {
        mSize = symSize->getFloat();
    }

    mSize = sqrtf(2.0 * mSize * mSize);

    SymbolRGB* color = (SymbolRGB*)(obj->getSymbolByName("color"));

    if (color != NULL)
    {
        mRed = color->mRed;
        mGreen = color->mGreen;
        mBlue = color->mBlue;
    }
}

void GraphicSquare::draw(art::Layer* layer)
{
    art::Layer2D* layer2D = (art::Layer2D*)layer;

    float rot = mObject->mFloatData[mRotIndex];
    float centerX = mObject->mFloatData[mXIndex];
    float centerY = mObject->mFloatData[mYIndex];

    layer2D->setRotation(centerX, centerY, rot);
    layer2D->setColor(mRed, mGreen, mBlue);
    layer2D->fillSquare(centerX, centerY, mSize);
    layer2D->clearRotation();
}

const char GraphicSquare::mClassName[] = "GraphicSquare";

Orbit<GraphicSquare>::MethodType GraphicSquare::mMethods[] = {{0,0}};

Orbit<GraphicSquare>::NumberGlobalType GraphicSquare::mNumberGlobals[] = {{0,0}};


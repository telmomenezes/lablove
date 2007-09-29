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

#include "GraphicTriangle.h"
#include "Lab.h"
#include "SimulationObject.h"

GraphicTriangle::GraphicTriangle(lua_State* luaState)
{
    mObject = NULL;
    mSize = 0;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;

    mRootLayer2D = NULL;
}

GraphicTriangle::~GraphicTriangle()
{   
}

Graphic* GraphicTriangle::createSameType()
{
    return new GraphicTriangle();
}

void GraphicTriangle::init(void* obj)
{
    mObject = obj;
    SimulationObject* simObj = (SimulationObject*)mObject;
    mSize = simObj->mSize;
    mRed = simObj->getColor()->mRed;
    mGreen = simObj->getColor()->mGreen;
    mBlue = simObj->getColor()->mBlue;

    mRootLayer2D = Lab::getSingleton().getRootLayer2D();
}

void GraphicTriangle::draw()
{
    SimulationObject* simObj = (SimulationObject*)mObject;

    float rot = simObj->mRotZ;
    float centerX = simObj->mX;
    float centerY = simObj->mY;

    float a1 = rot;
    float a2 = rot + (M_PI * 0.8f);
    float a3 = rot + (M_PI * 1.2f);
    float x1 = centerX + (cosf(a1) * mSize);
    float y1 = centerY + (sinf(a1) * mSize);
    float x2 = centerX + (cosf(a2) * mSize);
    float y2 = centerY + (sinf(a2) * mSize);
    float x3 = centerX + (cosf(a3) * mSize);
    float y3 = centerY + (sinf(a3) * mSize);

    mRootLayer2D->setColor(mRed, mGreen, mBlue);
    mRootLayer2D->setLineWidth(2.0f);
    mRootLayer2D->fillTriangle(x1, y1, x2, y2, x3, y3);
    mRootLayer2D->drawTriangle(x1, y1, x2, y2, x3, y3);
}

const char GraphicTriangle::mClassName[] = "GraphicTriangle";

Orbit<GraphicTriangle>::MethodType GraphicTriangle::mMethods[] = {{0,0}};

Orbit<GraphicTriangle>::NumberGlobalType GraphicTriangle::mNumberGlobals[] = {{0,0}};


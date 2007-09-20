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
}

void GraphicTriangle::draw(float x, float y)
{
    SimulationObject* simObj = (SimulationObject*)mObject;

    float rot = simObj->mRot;
    float centerX = simObj->mX + x;
    float centerY = simObj->mY + y;

    float a1 = rot;
    float a2 = rot + (M_PI * 0.8f);
    float a3 = rot + (M_PI * 1.2f);
    float x1 = centerX + (cosf(a1) * mSize);
    float y1 = centerY + (sinf(a1) * mSize);
    float x2 = centerX + (cosf(a2) * mSize);
    float y2 = centerY + (sinf(a2) * mSize);
    float x3 = centerX + (cosf(a3) * mSize);
    float y3 = centerY + (sinf(a3) * mSize);

    Lab::getSingleton().getRootLayer()->setColor(mRed, mGreen, mBlue);
    Lab::getSingleton().getRootLayer()->setLineWidth(2.0f);
    Lab::getSingleton().getRootLayer()->fillTriangle(x1, y1, x2, y2, x3, y3);
    Lab::getSingleton().getRootLayer()->drawTriangle(x1, y1, x2, y2, x3, y3);
}

const char GraphicTriangle::mClassName[] = "GraphicTriangle";

Orbit<GraphicTriangle>::MethodType GraphicTriangle::mMethods[] = {{0,0}};

Orbit<GraphicTriangle>::NumberGlobalType GraphicTriangle::mNumberGlobals[] = {{0,0}};


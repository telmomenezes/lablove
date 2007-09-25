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
#include "Lab.h"
#include "SimulationObject.h"

GraphicSquare::GraphicSquare(lua_State* luaState)
{
}

GraphicSquare::~GraphicSquare()
{	
}

Graphic* GraphicSquare::createSameType()
{
	return new GraphicSquare();
}

void GraphicSquare::init(void* obj)
{
	mObject = obj;
	SimulationObject* simObj = (SimulationObject*)mObject;
	mSize = simObj->mSize;
	mRed = simObj->getColor()->mRed;
	mGreen = simObj->getColor()->mGreen;
	mBlue = simObj->getColor()->mBlue;
}

void GraphicSquare::draw(float x, float y)
{
	SimulationObject* simObj = (SimulationObject*)mObject;
	float rot = simObj->mRot;
	float centerX = simObj->mX + x;
	float centerY = simObj->mY + y;

	Lab::getSingleton().getRootLayer()->setRotation(centerX, centerY, rot);
	Lab::getSingleton().getRootLayer()->setColor(mRed, mGreen, mBlue);
	Lab::getSingleton().getRootLayer()->fillSquare(centerX, centerY, mSize);
	Lab::getSingleton().getRootLayer()->clearRotation();
}

const char GraphicSquare::mClassName[] = "GraphicSquare";

Orbit<GraphicSquare>::MethodType GraphicSquare::mMethods[] = {{0,0}};

Orbit<GraphicSquare>::NumberGlobalType GraphicSquare::mNumberGlobals[] = {{0,0}};


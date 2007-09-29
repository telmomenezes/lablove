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
    mObject = NULL;
	mSize = 0;
	mRed = 0;
	mGreen = 0;
	mBlue = 0;

    mRootLayer2D = NULL;
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

    mRootLayer2D = Lab::getSingleton().getRootLayer2D();
}

void GraphicSquare::draw()
{
	SimulationObject* simObj = (SimulationObject*)mObject;
	float rot = simObj->mRotZ;
	float centerX = simObj->mX;
	float centerY = simObj->mY;

	mRootLayer2D->setRotation(centerX, centerY, rot);
	mRootLayer2D->setColor(mRed, mGreen, mBlue);
	mRootLayer2D->fillSquare(centerX, centerY, mSize);
	mRootLayer2D->clearRotation();
}

const char GraphicSquare::mClassName[] = "GraphicSquare";

Orbit<GraphicSquare>::MethodType GraphicSquare::mMethods[] = {{0,0}};

Orbit<GraphicSquare>::NumberGlobalType GraphicSquare::mNumberGlobals[] = {{0,0}};


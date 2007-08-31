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

#include "PlantSimple2D.h"
#include "Lab.h"
#include "math.h"
#include "defines.h"
#include "SimSimple2D.h"

PlantSimple2D::PlantSimple2D(lua_State* luaState)
{
	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;
}

PlantSimple2D::~PlantSimple2D()
{	
}

SimulationObject* PlantSimple2D::clone(bool full)
{
	return new PlantSimple2D(this);
}

void PlantSimple2D::draw()
{
	Lab::getSingleton().getRootLayer()->setColor(mColor.mRed, mColor.mGreen, mColor.mBlue);
	Lab::getSingleton().getRootLayer()->fillSquare(mX, mY, mSize, mRot);
}

const char PlantSimple2D::mClassName[] = "PlantSimple2D";

Orbit<PlantSimple2D>::MethodType PlantSimple2D::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setPos", &ObjectSimple2D::setPos},
	{"setRot", &ObjectSimple2D::setRot},
	{"setSize", &ObjectSimple2D::setSize},
	{"setColor", &ObjectSimple2D::setColor},
	{"setAgeRange", &ObjectSimple2D::setAgeRange},
	{"setMetabolism", &ObjectSimple2D::setMetabolism},
        {0,0}
};

Orbit<PlantSimple2D>::NumberGlobalType PlantSimple2D::mNumberGlobals[] = {{0,0}};


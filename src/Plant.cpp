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

#include "Plant.h"
#include "Lab.h"
#include "math.h"
#include "defines.h"
#include "SimSimple.h"

Plant::Plant(lua_State* luaState) : SimulationObject()
{
	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;
}

Plant::~Plant()
{	
}

SimulationObject* Plant::clone(bool full)
{
	return new Plant(this);
}

const char Plant::mClassName[] = "Plant";

Orbit<Plant>::MethodType Plant::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setPos", &SimulationObject::setPos},
	{"setRot", &SimulationObject::setRot},
	{"setSize", &SimulationObject::setSize},
	{"setColor", &SimulationObject::setColor},
	{"setAgeRange", &SimulationObject::setAgeRange},
	{"setMetabolism", &SimulationObject::setMetabolism},
	{"setGraphic", &SimulationObject::setGraphic},
        {0,0}
};

Orbit<Plant>::NumberGlobalType Plant::mNumberGlobals[] = {{0,0}};


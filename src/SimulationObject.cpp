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

#include "SimulationObject.h"
#include "LoveLab.h"
#include <stdlib.h>
#include "functions.h"

unsigned long SimulationObject::CURRENT_ID = 0;

SimulationObject::SimulationObject()
{
	mID = CURRENT_ID++;

	mDeleted = false;
	mSelected = false;

        mSpeciesID = 0;
	mEnergy = 0;
	mInitialEnergy = 0;
	mCreationTime = LoveLab::getInstance().getSimulation()->time();
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
	mID = CURRENT_ID++;

	mDeleted = false;
	mSelected = false;

        mEnergy = obj->mEnergy;
	mInitialEnergy = obj->mInitialEnergy;
        mSpeciesID = obj->mSpeciesID;
	mCreationTime = LoveLab::getInstance().getSimulation()->time();
}

SimulationObject::~SimulationObject()
{
	map<string, Chemistry*>::iterator iterChems;

	for (iterChems = mChemistriesMap.begin();
		iterChems != mChemistriesMap.end();
		iterChems++)
	{
		delete (*iterChems).second;
	}

	mChemistriesMap.clear();
}

int SimulationObject::setInitialEnergy(lua_State* luaState)
{
        float energy = luaL_checknumber(luaState, 1);
        setInitialEnergy(energy);
        return 0;
}


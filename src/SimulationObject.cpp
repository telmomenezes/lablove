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
#include "Lab.h"
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
	mCreationTime = Lab::getSingleton().getSimulation()->time();
	mHuman = false;
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
	mID = CURRENT_ID++;

	mDeleted = false;
	mSelected = false;

        mEnergy = obj->mEnergy;
	mInitialEnergy = obj->mInitialEnergy;
        mSpeciesID = obj->mSpeciesID;
	mCreationTime = Lab::getSingleton().getSimulation()->time();
	mHuman = false;

	std::map<unsigned int, Chemistry*>::iterator iterChems;
	for (iterChems = obj->mChemistries.begin();
		iterChems != obj->mChemistries.end();
		iterChems++)
	{
		mChemistries[(*iterChems).first] = new Chemistry((*iterChems).second);
	}
}

SimulationObject::~SimulationObject()
{
	std::map<unsigned int, Chemistry*>::iterator iterChems;

	for (iterChems = mChemistries.begin();
		iterChems != mChemistries.end();
		iterChems++)
	{
		delete (*iterChems).second;
	}

	mChemistries.clear();
}

void SimulationObject::addChemistry(Chemistry* chem, unsigned int code)
{
	mChemistries[code] = chem;
}

Chemistry* SimulationObject::getChemistry(unsigned int code)
{
	if (mChemistries.count(code) == 0)
	{
		return NULL;
	}
	return mChemistries[code];
}

int SimulationObject::setInitialEnergy(lua_State* luaState)
{
        float energy = luaL_checknumber(luaState, 1);
        setInitialEnergy(energy);
        return 0;
}

int SimulationObject::addChemistry(lua_State* luaState)
{
	Chemistry* chem = (Chemistry*)Orbit<Lab>::pointer(luaState, 1);
	unsigned int code = luaL_checkint(luaState, 2);
	addChemistry(chem, code);
	return 0;
}


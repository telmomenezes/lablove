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

	std::map<unsigned int, SymbolTable*>::iterator iterTables;
	for (iterTables = obj->mSymbolTables.begin();
		iterTables != obj->mSymbolTables.end();
		iterTables++)
	{
		mSymbolTables[(*iterTables).first] = new SymbolTable((*iterTables).second);
	}
}

SimulationObject::~SimulationObject()
{
	std::map<unsigned int, SymbolTable*>::iterator iterTables;

	for (iterTables = mSymbolTables.begin();
		iterTables != mSymbolTables.end();
		iterTables++)
	{
		delete (*iterTables).second;
	}

	mSymbolTables.clear();
}

void SimulationObject::addSymbolTable(SymbolTable* table, unsigned int code)
{
	mSymbolTables[code] = table;
}

SymbolTable* SimulationObject::getSymbolTable(unsigned int code)
{
	if (mSymbolTables.count(code) == 0)
	{
		return NULL;
	}
	return mSymbolTables[code];
}

int SimulationObject::setInitialEnergy(lua_State* luaState)
{
        float energy = luaL_checknumber(luaState, 1);
        setInitialEnergy(energy);
        return 0;
}

int SimulationObject::addSymbolTable(lua_State* luaState)
{
	SymbolTable* table = (SymbolTable*)Orbit<Lab>::pointer(luaState, 1);
	unsigned int code = luaL_checkint(luaState, 2);
	addSymbolTable(table, code);
	return 0;
}


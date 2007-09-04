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
#include "random.h"

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

	mX = -1.0f;
	mY = -1.0f;
	mSize = 1.0f;
	mSizeSquared = 1.0f;
	mRot = 0.0f;

	mNextCellList = NULL;
	mPrevCellList = NULL;

	mCellX = -1;
	mCellY = -1;
	mCellPos = -1;

	mMaxAge = 0;
	mLowAgeLimit = 0;
	mHighAgeLimit = 0;
	mMetabolism = 0.0f;
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

	mSize = obj->mSize;
	mSizeSquared = obj->mSizeSquared;
	mLowAgeLimit = obj->mLowAgeLimit;
	mHighAgeLimit = obj->mHighAgeLimit;
	
	if (mHighAgeLimit > 0)
	{
		mMaxAge = randomUniformInt(mLowAgeLimit, mHighAgeLimit);
	}
	else
	{
		mMaxAge = 0;
	}
	mMetabolism = obj->mMetabolism;

	mX = -1.0f;
	mY = -1.0f;
	mRot = 0.0f;

	mColor = SymbolRGB(obj->mColor);
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

void SimulationObject::setPos(float x, float y)
{
	Simulation* sim = Lab::getSingleton().getSimulation();

	if ((x < 0)
		|| (y < 0)
		|| (x >= sim->getWorldWidth())
		|| (y >= sim->getWorldLength()))
	{
		return;
	}

	unsigned int cellSide = (unsigned int)(sim->getCellSide());
	unsigned int targetCellX = ((unsigned int)x) / cellSide;
	unsigned int targetCellY = ((unsigned int)y) / cellSide;
	SimulationObject** objGrid = sim->getCellGrid();
	unsigned int worldCellWidth = sim->getWorldCellWidth();
	unsigned int targetCellPos = (targetCellY * worldCellWidth) + targetCellX;
	SimulationObject* targetCell = objGrid[targetCellPos];

	if (mX >= 0)
	{
		if ((mCellX != targetCellX) || (mCellY != targetCellY))
		{
			// Remove from origin cell
			if (mNextCellList != NULL)
			{
				mNextCellList->mPrevCellList = mPrevCellList;
			}
			if (mPrevCellList == NULL)
			{
				objGrid[mCellPos] = mNextCellList;
			}
			else
			{
				mPrevCellList->mNextCellList = mNextCellList;
			}

			// Insert in new target cell
			mNextCellList = targetCell;
			mPrevCellList = NULL;
			if (targetCell != NULL)
			{
				targetCell->mPrevCellList = this;
			}
			objGrid[targetCellPos] = this;
			mCellX = targetCellX;
			mCellY = targetCellY;
			mCellPos = targetCellPos;
		}
	}
	else
	{
		// Insert in target cell
		mNextCellList = targetCell;
		mPrevCellList = NULL;
		if (targetCell != NULL)
		{
			targetCell->mPrevCellList = this;
		}
		objGrid[targetCellPos] = this;
		mCellX = targetCellX;
		mCellY = targetCellY;
		mCellPos = targetCellPos;
	}

	mX = x;
	mY = y;
}

void SimulationObject::setSize(float size)
{
	mSize = size;
	mSizeSquared = mSize * mSize;
}

void SimulationObject::setRot(float rot)
{
	mRot = normalizeAngle(rot);
}

void SimulationObject::placeRandom()
{
	Simulation* sim = Lab::getSingleton().getSimulation();

	unsigned int worldWidth = (unsigned int)sim->getWorldWidth();
	unsigned int worldLength = (unsigned int)sim->getWorldLength();

	setPos(rand() % worldWidth, rand() % worldLength);
	setRot(randomUniformProbability() * M_PI * 2);
}

void SimulationObject::onCycle()
{
	mEnergy -= mMetabolism;

	if (mEnergy < 0)
	{
		Lab::getSingleton().getSimulation()->killOrganism(this);
	}

	if (mMaxAge > 0)
	{
		if (Lab::getSingleton().getSimulation()->time() - mCreationTime >= mMaxAge)
		{
			Lab::getSingleton().getSimulation()->killOrganism(this);
		}
	}
}

void SimulationObject::setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit)
{
	mLowAgeLimit = lowAgeLimit;
	mHighAgeLimit = highAgeLimit;
}

void SimulationObject::setColor(SymbolRGB* color)
{
	mColor = SymbolRGB(color);
}

float SimulationObject::getFieldValue(std::string fieldName)
{
	if (fieldName == "energy")
	{
		return mEnergy;
	}
	else
	{
		return SimulationObject::getFieldValue(fieldName);
	}
}

int SimulationObject::setPos(lua_State* luaState)
{
        float x = luaL_checknumber(luaState, 1);
	float y = luaL_checknumber(luaState, 2);
        setPos(x, y);
        return 0;
}

int SimulationObject::setSize(lua_State* luaState)
{
        int size = luaL_checkint(luaState, 1);
        setSize(size);
        return 0;
}

int SimulationObject::setRot(lua_State* luaState)
{
        float rot = luaL_checknumber(luaState, 1);
        setRot(rot);
        return 0;
}

int SimulationObject::setAgeRange(lua_State* luaState)
{
        int lowAgeLimit = luaL_checkint(luaState, 1);
        int highAgeLimit = luaL_checkint(luaState, 2);
        setAgeRange(lowAgeLimit, highAgeLimit);
        return 0;
}

int SimulationObject::setMetabolism(lua_State* luaState)
{
        float metabolism = luaL_checknumber(luaState, 1);
        setMetabolism(metabolism);
        return 0;
}

int SimulationObject::setColor(lua_State* luaState)
{
	SymbolRGB* color = (SymbolRGB*)Orbit<Lab>::pointer(luaState, 1);
        setColor(color);
        return 0;
}


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
#include "functions.h"
#include "random.h"
#include <stdlib.h>

unsigned long SimulationObject::CURRENT_ID = 0;

SimulationObject::SimulationObject(lua_State* luaState)
{
    mID = CURRENT_ID++;

    mDeleted = false;
    mSelected = false;

    mSpeciesID = 0;
    mEnergy = 0;
    mInitialEnergy = 0;
    mCreationTime = 0;
    mHuman = false;

    mX = 0.0f;
    mY = 0.0f;
    mZ = 0.0f;

    mRotX = 0.0f;
    mRotY = 0.0f;
    mRotZ = 0.0f;

    mSize = 1.0f;
    mSizeSquared = 1.0f;

    mMaxAge = 0;
    mLowAgeLimit = 0;
    mHighAgeLimit = 0;
    mMetabolism = 0.0f;

    mGraphic = NULL;

    mType = TYPE_OBJECT;

    mCollisionDetectionIteration = 0;
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
    mID = CURRENT_ID++;

    mDeleted = false;
    mSelected = false;

    mEnergy = obj->mEnergy;
    mInitialEnergy = obj->mInitialEnergy;
    mSpeciesID = obj->mSpeciesID;
    mCreationTime = 0;
    mHuman = false;

    map<unsigned int, SymbolTable*>::iterator iterTables;
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

    mX = 0.0f;
    mY = 0.0f;
    mZ = 0.0f;

    mRotX = 0.0f;
    mRotY = 0.0f;
    mRotZ = 0.0f;

    mColor = SymbolRGB(obj->mColor);

    if (obj->mGraphic != NULL)
    {
        mGraphic= obj->mGraphic->createSameType();
        mGraphic->init(this);
    }
    else
    {
        mGraphic = NULL;
    }

    mType = obj->mType;

    mCollisionDetectionIteration = 0;
}

SimulationObject::~SimulationObject()
{
    map<unsigned int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    if (mGraphic != NULL)
    {
        delete mGraphic;
        mGraphic = NULL;
    }
}

SimulationObject* SimulationObject::clone()
{
    return new SimulationObject(this);
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
    SymbolTable* table = (SymbolTable*)Orbit<SimulationObject>::pointer(luaState, 1);
    unsigned int code = luaL_checkint(luaState, 2);
    addSymbolTable(table, code);
    return 0;
}

void SimulationObject::setSize(float size)
{
    mSize = size;
    mSizeSquared = mSize * mSize;
}

void SimulationObject::draw(pyc::Layer* layer)
{
    if (mGraphic != NULL)
    {
        mGraphic->draw(layer);
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

float SimulationObject::getFieldValue(string fieldName)
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

void SimulationObject::setGraphic(Graphic* graph)
{
    mGraphic = graph;
    mGraphic->init(this);
}

int SimulationObject::setSize(lua_State* luaState)
{
    int size = luaL_checkint(luaState, 1);
    setSize(size);
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
    SymbolRGB* color = (SymbolRGB*)Orbit<SimulationObject>::pointer(luaState, 1);
    setColor(color);
    return 0;
}

int SimulationObject::setGraphic(lua_State* luaState)
{
    Graphic* graph = (Graphic*)Orbit<SimulationObject>::pointer(luaState, 1);
    setGraphic(graph);
    return 0;
}

const char SimulationObject::mClassName[] = "SimulationObject";

Orbit<SimulationObject>::MethodType SimulationObject::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setSize", &SimulationObject::setSize},
	{"setColor", &SimulationObject::setColor},
	{"setAgeRange", &SimulationObject::setAgeRange},
	{"setMetabolism", &SimulationObject::setMetabolism},
	{"setGraphic", &SimulationObject::setGraphic},
        {0,0}
};

Orbit<SimulationObject>::NumberGlobalType SimulationObject::mNumberGlobals[] = {{0,0}};


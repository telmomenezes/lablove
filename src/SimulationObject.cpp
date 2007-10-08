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
#include "Random.h"
#include <stdlib.h>

unsigned long SimulationObject::CURRENT_ID = 0;

SimulationObject::SimulationObject(lua_State* luaState)
{
    mID = CURRENT_ID++;

    mDeleted = false;

    mSpeciesID = 0;
    mEnergy = 0;
    mInitialEnergy = 0;
    mCreationTime = 0;

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

    mType = TYPE_OBJECT;

    mCollisionDetectionIteration = 0;

    mInitialized = false;

    mFloatData = NULL;
    mBoolData = NULL;
    mIntData = NULL;
    mUnsignedLongData = NULL;

    mFloatDataSize = 0;
    mBoolDataSize = 0;
    mIntDataSize = 0;
    mUnsignedLongDataSize = 0;
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
    mID = CURRENT_ID++;

    mDeleted = false;

    mEnergy = obj->mEnergy;
    mInitialEnergy = obj->mInitialEnergy;
    mSpeciesID = obj->mSpeciesID;
    mCreationTime = 0;

    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = obj->mSymbolTables.begin();
        iterTables != obj->mSymbolTables.end();
        iterTables++)
    {
        mSymbolTables[(*iterTables).first] = new SymbolTable((*iterTables).second);
    }

    map<string, SymbolPointer>::iterator iterSymPointers;
    for (iterSymPointers = obj->mNamedSymbols.begin();
        iterSymPointers != obj->mNamedSymbols.end();
        iterSymPointers++)
    {
        mNamedSymbols[(*iterSymPointers).first] = (*iterSymPointers).second;
    }

    mSize = obj->mSize;
    mSizeSquared = obj->mSizeSquared;
    mLowAgeLimit = obj->mLowAgeLimit;
    mHighAgeLimit = obj->mHighAgeLimit;
    
    if (mHighAgeLimit > 0)
    {
        mMaxAge = Random::getUniformInt(mLowAgeLimit, mHighAgeLimit);
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

    for (list<Graphic*>::iterator iterGraph = obj->mGraphics.begin();
            iterGraph != obj->mGraphics.end();
            iterGraph++)
    {
        Graphic* graph = (*iterGraph)->createSameType();
        graph->init(this);
        mGraphics.push_back(graph);
    }

    mType = obj->mType;

    mCollisionDetectionIteration = 0;

    mInitialized = false;

    mFloatDataSize = obj->mFloatDataSize;
    mBoolDataSize = obj->mBoolDataSize;
    mIntDataSize = obj->mIntDataSize;
    mUnsignedLongDataSize = obj->mUnsignedLongDataSize;

    if (obj->mFloatDataSize > 0)
    {
        unsigned int size = mFloatDataSize * sizeof(float);
        mFloatData = (float*)malloc(size);
        memcpy(mFloatData, obj->mFloatData, size);
    }
    else
    {
        mFloatData = NULL;
    }
    if (obj->mBoolDataSize > 0)
    {
        unsigned int size = mBoolDataSize * sizeof(bool);
        mBoolData = (bool*)malloc(size);
        memcpy(mBoolData, obj->mBoolData, size);
    }
    else
    {
        mBoolData = NULL;
    }
    if (obj->mIntDataSize > 0)
    {
        unsigned int size = mIntDataSize * sizeof(int);
        mIntData = (int*)malloc(size);
        memcpy(mIntData, obj->mIntData, size);
    }
    else
    {
        mIntData = NULL;
    }
    if (obj->mUnsignedLongDataSize > 0)
    {
        unsigned int size = mUnsignedLongDataSize * sizeof(unsigned long);
        mUnsignedLongData = (unsigned long*)malloc(size);
        memcpy(mUnsignedLongData, obj->mUnsignedLongData, size);
    }
    else
    {
        mUnsignedLongData = NULL;
    }
}

SimulationObject::~SimulationObject()
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    mNamedSymbols.clear();

    if (mFloatData != NULL)
    {
        free(mFloatData);
        mFloatData = NULL;
    }
    if (mBoolData != NULL)
    {
        free(mBoolData);
        mBoolData = NULL;
    }
    if (mIntData != NULL)
    {
        free(mIntData);
        mIntData = NULL;
    }
    if (mUnsignedLongData != NULL)
    {
        free(mUnsignedLongData);
        mUnsignedLongData = NULL;
    }

    for (list<Graphic*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        delete (*iterGraph);
    }
    mGraphics.clear();
}

SimulationObject* SimulationObject::clone()
{
    return new SimulationObject(this);
}

void SimulationObject::initFloatData(unsigned int size)
{
    if (mFloatData == NULL)
    {
        mFloatDataSize = size;
        mFloatData = (float*)malloc(size * sizeof(float));
    }
}

void SimulationObject::initBoolData(unsigned int size)
{
    if (mBoolData == NULL)
    {
        mBoolDataSize = size;
        mBoolData = (bool*)malloc(size * sizeof(bool));
    }
}

void SimulationObject::initIntData(unsigned int size)
{
    if (mIntData == NULL)
    {
        mIntDataSize = size;
        mIntData = (int*)malloc(size * sizeof(int));
    }
}

void SimulationObject::initUnsignedLongData(unsigned int size)
{
    if (mUnsignedLongData == NULL)
    {
        mUnsignedLongDataSize = size;
        mUnsignedLongData = (unsigned long*)malloc(size * sizeof(unsigned long));
    }
}

void SimulationObject::addSymbolTable(SymbolTable* table)
{
    mSymbolTables[table->getID()] = table;
}

SymbolTable* SimulationObject::getSymbolTable(int id)
{
    if (mSymbolTables.count(id) == 0)
    {
        return NULL;
    }
    return mSymbolTables[id];
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
    addSymbolTable(table);
    return 0;
}

void SimulationObject::setSymbolName(string name, int table, unsigned int pos)
{
    SymbolPointer sp;
    sp.mTable = table;
    sp.mPos = pos;
    mNamedSymbols[name] = sp;
}

Symbol* SimulationObject::getSymbolByName(string name)
{
    if (mNamedSymbols.count(name) == 0)
    {
        return NULL;
    }
    
    SymbolPointer sp = mNamedSymbols[name];
    SymbolTable* symTab = getSymbolTable(sp.mTable);

    if (symTab == NULL)
    {
        return NULL;
    }

    return symTab->getSymbol(sp.mPos);
}

void SimulationObject::setSize(float size)
{
    mSize = size;
    mSizeSquared = mSize * mSize;
}

void SimulationObject::draw(pyc::Layer* layer)
{
    for (list<Graphic*>::iterator iterGraph = mGraphics.begin();
            iterGraph != mGraphics.end();
            iterGraph++)
    {
        (*iterGraph)->draw(layer);
    }
}

void SimulationObject::setAgeRange(unsigned long lowAgeLimit, unsigned long highAgeLimit)
{
    mLowAgeLimit = lowAgeLimit;
    mHighAgeLimit = highAgeLimit;
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

void SimulationObject::addGraphic(Graphic* graph)
{
    mGraphics.push_back(graph);
    graph->init(this);
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

int SimulationObject::addGraphic(lua_State* luaState)
{
    Graphic* graph = (Graphic*)Orbit<SimulationObject>::pointer(luaState, 1);
    addGraphic(graph);
    return 0;
}

int SimulationObject::setSymbolName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    int table = luaL_checkint(luaState, 2);
    int pos = luaL_checkint(luaState, 3);
    setSymbolName(name, table, pos);
    return 0;
}

const char SimulationObject::mClassName[] = "SimulationObject";

Orbit<SimulationObject>::MethodType SimulationObject::mMethods[] = {
	{"setInitialEnergy", &SimulationObject::setInitialEnergy},
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setSize", &SimulationObject::setSize},
	{"setAgeRange", &SimulationObject::setAgeRange},
	{"setMetabolism", &SimulationObject::setMetabolism},
	{"addGraphic", &SimulationObject::addGraphic},
	{"setSymbolName", &SimulationObject::setSymbolName},
    {0,0}
};

Orbit<SimulationObject>::NumberGlobalType SimulationObject::mNumberGlobals[] = {{0,0}};


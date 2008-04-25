/*
 * LabLOVE
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
#include "SymbolFloat.h"
#include "SymbolUL.h"
#include <stdlib.h>

llULINT SimulationObject::CURRENT_ID = 0;

SimulationObject::SimulationObject(lua_State* luaState)
{
    mType = TYPE_OBJECT;
    mID = CURRENT_ID++;

    mDeleted = false;

    mSpeciesID = 0;
    mCreationTime = 0;

    mInitialized = false;

    mFloatData = NULL;
    mBoolData = NULL;
    mIntData = NULL;
    mULData = NULL;

    mFloatDataSize = 0;
    mBoolDataSize = 0;
    mIntDataSize = 0;
    mULDataSize = 0;

    mFitness = 0.0f;

    mDataInitialized = false;

    mBirthRadius = 0.0f;
    mFitnessMeasure = 0;
}

SimulationObject::SimulationObject(SimulationObject* obj)
{
    mID = CURRENT_ID++;

    mDeleted = false;

    mSpeciesID = obj->mSpeciesID;
    mCreationTime = 0;

    map<string, SymbolPointer>::iterator iterSymPointers;
    for (iterSymPointers = obj->mNamedSymbols.begin();
        iterSymPointers != obj->mNamedSymbols.end();
        iterSymPointers++)
    {
        mNamedSymbols[(*iterSymPointers).first] = (*iterSymPointers).second;
    }

    mType = obj->mType;

    mInitialized = false;

    mFloatDataSize = obj->mFloatDataSize;
    mBoolDataSize = obj->mBoolDataSize;
    mIntDataSize = obj->mIntDataSize;
    mULDataSize = obj->mULDataSize;

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
    if (obj->mULDataSize > 0)
    {
        unsigned int size = mULDataSize * sizeof(llULINT);
        mULData = (llULINT*)malloc(size);
        memcpy(mULData, obj->mULData, size);
    }
    else
    {
        mULData = NULL;
    }

    mDataInitialized = obj->mDataInitialized;

    mFitness = 0.0f;
    mFitnessMeasure = obj->mFitnessMeasure;

    mBirthRadius = obj->mBirthRadius;

    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = obj->mSymbolTables.begin();
        iterTables != obj->mSymbolTables.end();
        iterTables++)
    {
        mSymbolTables[(*iterTables).first] = new SymbolTable((*iterTables).second);
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
    if (mULData != NULL)
    {
        free(mULData);
        mULData = NULL;
    }
}

SimulationObject* SimulationObject::clone()
{
    return new SimulationObject(this);
}

void SimulationObject::initFloatData(unsigned int size)
{
    mFloatDataSize = size;
    mFloatData = (float*)malloc(size * sizeof(float));

    for (unsigned int i = 0; i < size; i++)
    {
        mFloatData[i] = 0.0f;
    }
}

void SimulationObject::initBoolData(unsigned int size)
{
    mBoolDataSize = size;
    mBoolData = (bool*)malloc(size * sizeof(bool));

    for (unsigned int i = 0; i < size; i++)
    {
        mBoolData[i] = false;
    }
}

void SimulationObject::initIntData(unsigned int size)
{
    mIntDataSize = size;
    mIntData = (int*)malloc(size * sizeof(int));

    for (unsigned int i = 0; i < size; i++)
    {
        mIntData[i] = 0;
    }
}

void SimulationObject::initULData(unsigned int size)
{
    mULDataSize = size;
    mULData = (llULINT*)malloc(size * sizeof(llULINT));

    for (unsigned int i = 0; i < size; i++)
    {
        mULData[i] = 0;
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

SymbolTable* SimulationObject::getSymbolTableByName(string name)
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        if ((*iterTables).second->getName() == name)
        {
            return (*iterTables).second;
        }
    }

    return NULL;
}

void SimulationObject::setSymbolName(string name, int table, llULINT id)
{
    SymbolPointer sp;
    sp.mTable = table;
    sp.mID = id;
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

    return symTab->getSymbol(sp.mID);
}

string SimulationObject::getSymbolName(int table, llULINT id)
{
    for (map<string, SymbolPointer>::iterator iterName = mNamedSymbols.begin();
            iterName != mNamedSymbols.end();
            iterName++)
    {
        if (((*iterName).second.mTable == table)
            && ((*iterName).second.mID == id))
        {
            return (*iterName).first;
        }
    }

    return "?";
}

string SimulationObject::getTableName(int table)
{
    SymbolTable* t = getSymbolTable(table);

    if (t == NULL)
    {
        return "?";
    }

    if (t->getName() != "")
    {
        return t->getName();
    }

    return "?";
}

bool SimulationObject::getFieldValue(string fieldName, float& value)
{
    if (fieldName == "fitness")
    {
        value = mFitness;
        return true;
    }
    else if (fieldName.substr(0,  14) == "symtable_size_")
    {
        string tableName = fieldName.substr(14, fieldName.size() - 14);

        SymbolTable* table = getSymbolTableByName(tableName);
        if (table)
        {
            value = table->getSize();
        }
        else
        {
            value = 0.0f;
        }
        return true;
    }
    else
    {
        value = 0.0f;
        return false;
    }
}

void SimulationObject::recombine(SimulationObject* parent1, SimulationObject* parent2)
{
    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    for (iterTables = parent1->mSymbolTables.begin();
        iterTables != parent1->mSymbolTables.end();
        iterTables++)
    {
        int tableID = (*iterTables).first;
        SymbolTable* table1 = (*iterTables).second;
        SymbolTable* table2 = parent2->getSymbolTable(tableID);

        mSymbolTables[tableID] = table1->recombine(table2);
    }
}

SimulationObject* SimulationObject::recombine(SimulationObject* otherParent)
{
    SimulationObject* obj = clone();

    obj->recombine(this, otherParent);
    SimulationObject* child = obj->clone();
    delete obj;

    return child;
}

void SimulationObject::setFloatDataFromSymbol(string symbolName, unsigned int dataIndex)
{
    SymbolFloat* symFloat = (SymbolFloat*)getSymbolByName(symbolName);
    if (symFloat != NULL)
    {
        mFloatData[dataIndex] = symFloat->getFloat();
    }
}

void SimulationObject::setULDataFromSymbol(string symbolName, unsigned int dataIndex)
{
    SymbolUL* symUL = (SymbolUL*)getSymbolByName(symbolName);
    if (symUL != NULL)
    {
        mULData[dataIndex] = symUL->getUL();
    }
}

void SimulationObject::setNamedFloatDataIndex(string name, int index)
{
    mNamedFloatDataIndexes[name] = index;
}

void SimulationObject::setNamedBoolDataIndex(string name, int index)
{
    mNamedBoolDataIndexes[name] = index;
}

void SimulationObject::setNamedIntDataIndex(string name, int index)
{
    mNamedIntDataIndexes[name] = index;
}

void SimulationObject::getNamedULDataIndex(string name, int index)
{
    mNamedULDataIndexes[name] = index;
}

int SimulationObject::getNamedFloatDataIndex(string name)
{
    if (mNamedFloatDataIndexes.count(name) <= 0)
    {
        return -1;
    }
    
    return mNamedFloatDataIndexes[name];
}

int SimulationObject::getNamedBoolDataIndex(string name)
{
    if (mNamedBoolDataIndexes.count(name) <= 0)
    {
        return -1;
    }
    
    return mNamedBoolDataIndexes[name];
}

int SimulationObject::getNamedIntDataIndex(string name)
{
    if (mNamedIntDataIndexes.count(name) <= 0)
    {
        return -1;
    }
    
    return mNamedIntDataIndexes[name];
}

int SimulationObject::getNamedULDataIndex(string name)
{
    if (mNamedULDataIndexes.count(name) <= 0)
    {
        return -1;
    }
    
    return mNamedULDataIndexes[name];
}

void SimulationObject::mutate()
{
}

void SimulationObject::printDebug()
{
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        ((*iterTables).second)->printDebug();
    }
}

const char SimulationObject::mClassName[] = "SimulationObject";

Orbit<SimulationObject>::MethodType SimulationObject::mMethods[] = {
	{"addSymbolTable", &SimulationObject::addSymbolTable},
	{"setSymbolName", &SimulationObject::setSymbolName},
	{"setBirthRadius", &SimulationObject::setBirthRadius},
	{"setFitnessMeasure", &SimulationObject::setFitnessMeasure},
    {0,0}
};

Orbit<SimulationObject>::NumberGlobalType SimulationObject::mNumberGlobals[] = {{0,0}};

int SimulationObject::addSymbolTable(lua_State* luaState)
{
    SymbolTable* table = (SymbolTable*)Orbit<SimulationObject>::pointer(luaState, 1);
    addSymbolTable(table);
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

int SimulationObject::setBirthRadius(lua_State* luaState)
{
    float rad = luaL_checknumber(luaState, 1);
    setBirthRadius(rad);
    return 0;
}

int SimulationObject::setFitnessMeasure(lua_State* luaState)
{
    int measure = luaL_checkint(luaState, 1);
    setFitnessMeasure(measure);
    return 0;
}


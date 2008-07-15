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

#include "SimObj.h"
#include "SymbolFloat.h"
#include "SymbolUL.h"
#include <stdlib.h>

llULINT SimObj::CURRENT_ID = 1;

SimObj::SimObj(lua_State* luaState)
{
    mID = CURRENT_ID++;

    mType = TYPE_OBJECT;

    mDeleted = false;

    mSpeciesID = 0;
    mBodyID = -1;
    mCreationTime = 0;

    mInitialized = false;

    mBirthRadius = 0.0f;
    mKeepBodyOnHardDeath = false;
    mKeepBodyOnExpirationDeath = false;

    mBrain = NULL;

    mDeathType = DEATH_HARD;

    mTableSet = new TableSet();
}

SimObj::SimObj(SimObj* obj)
{
    mID = CURRENT_ID++;

    mDeleted = false;

    mSpeciesID = obj->mSpeciesID;
    mBodyID = obj->mBodyID;
    mCreationTime = 0;

    for (map<int, Fitness>::iterator iterFit = obj->mFitMap.begin();
        iterFit != obj->mFitMap.end();
        iterFit++)
    {
        mFitMap[(*iterFit).first] = (*iterFit).second;
    }
    
    mTableSet = new TableSet(obj->mTableSet);

    //printf("\n\n=================\n");
    //mTableSet.printDebug();

    mType = obj->mType;

    mBrain = NULL;

    mInitialized = false;

    mDeathType = DEATH_HARD;

    mBirthRadius = obj->mBirthRadius;
    mKeepBodyOnHardDeath = obj->mKeepBodyOnHardDeath;
    mKeepBodyOnExpirationDeath = obj->mKeepBodyOnExpirationDeath;

    if (mType == TYPE_AGENT)
    {
        mBrain = obj->mBrain->clone();
        mBrain->setOwner(this);
        mBrain->markUsedSymbols(mTableSet);
        mTableSet->cleanAndGrow();
        mBrain->repair();
    }
}

SimObj::~SimObj()
{
    if (mBrain != NULL)
    {
        delete mBrain;
        mBrain = NULL;
    }

    for (list<Message*>::iterator iterMessage = mMessageList.begin();
            iterMessage != mMessageList.end();
            iterMessage++)
    {
        delete (*iterMessage);
    }

    mMessageList.clear();

    if (mTableSet != NULL)
    {
        delete mTableSet;
        mTableSet = NULL;
    }
}

SimObj* SimObj::clone()
{
    return new SimObj(this);
}

void SimObj::setBrain(Brain* brain)
{
    mType = TYPE_AGENT;

    if (mBrain != NULL)
    {
        delete mBrain;
    }
    mBrain = brain;
    mBrain->setOwner(this);
    mBrain->init();
}

void SimObj::compute()
{
    if (mBrain == NULL)
    {
        return;
    }

    mBrain->cycle();
}

void SimObj::addSymbolTable(SymbolTable* table)
{
    mTableSet->addSymbolTable(table);
}

SymbolTable* SimObj::getSymbolTable(int id)
{
    return mTableSet->getSymbolTable(id);
}

SymbolTable* SimObj::getSymbolTableByName(string name)
{
    return mTableSet->getSymbolTableByName(name);
}

void SimObj::setSymbolName(string name, int table, llULINT id)
{
    mTableSet->setSymbolName(name, table, id);
}

Symbol* SimObj::getSymbolByName(string name)
{
    return mTableSet->getSymbolByName(name);
}

string SimObj::getSymbolName(int table, llULINT id)
{
    return mTableSet->getSymbolName(table, id);
}

string SimObj::getTableName(int table)
{
    return mTableSet->getTableName(table);
}

bool SimObj::getFieldValue(string fieldName, float& value)
{
    if (fieldName.substr(0,  14) == "symtable_size_")
    {
        string tableName = fieldName.substr(14, fieldName.size() - 14);

        SymbolTable* table = getSymbolTableByName(tableName);
        if (table)
        {
            value = (float)table->getSize();
        }
        else
        {
            value = 0.0f;
        }
        return true;
    }
    else if (fieldName.substr(0,  14) == "symtable_used_")
    {
        string tableName = fieldName.substr(14, fieldName.size() - 14);

        SymbolTable* table = getSymbolTableByName(tableName);
        if (table)
        {
            value = (float)table->mUsedCount;
        }
        else
        {
            value = 0.0f;
        }
        return true;
    }
    else if (mBrain != NULL)
    {
        return mBrain->getFieldValue(fieldName, value);
    }
    else {
        value = 0.0f;
        return false;
    }
}

void SimObj::addFitness(int type)
{
    mFitMap[type] = Fitness();
}

void SimObj::clearFitnesses()
{
    for (map<int, Fitness>::iterator iterFit = mFitMap.begin();
            iterFit != mFitMap.end();
            iterFit++)
    {
        (*iterFit).second.clear();
    }
}

Fitness* SimObj::getFitnessByIndex(unsigned int index)
{
    map<int, Fitness>::iterator iterFit = mFitMap.begin();

    for (unsigned int i = 0; i < index; i++)
    {
        iterFit++;
    }

    return &((*iterFit).second);
}

void SimObj::mutate(float factor)
{
    if (mBrain != NULL)
    {
        mBrain->mutate(factor);
    }
}

void SimObj::recombine(SimObj* parent1, SimObj* parent2)
{
    Brain* newBrain;

    if (mType == TYPE_AGENT)
    {
        newBrain = parent1->mBrain->recombine(parent2->mBrain);

        newBrain->markUsedSymbols(parent1->mTableSet);
        newBrain->markUsedSymbols(parent2->mTableSet);
        setBrain(newBrain);
    }

    mTableSet->recombine(parent1->mTableSet, parent2->mTableSet);
}

SimObj* SimObj::recombine(SimObj* otherParent)
{
    SimObj* obj = clone();
    obj->recombine(this, otherParent);
    SimObj* child = obj->clone();

    delete obj;

    //child->printDebug();

    return child;
}

void SimObj::onAdd()
{
    mTableSet->resetProtections();
}

void SimObj::setFloatDataFromSymbol(string symbolName, float& var)
{
    SymbolFloat* symFloat = (SymbolFloat*)getSymbolByName(symbolName);
    if (symFloat != NULL)
    {
        var = symFloat->getFloat();
    }
}

void SimObj::setULDataFromSymbol(string symbolName, llULINT& var)
{
    SymbolUL* symUL = (SymbolUL*)getSymbolByName(symbolName);
    if (symUL != NULL)
    {
        var = symUL->getUL();
    }
}

void SimObj::popAdjust(vector<SimObj*>* popVec)
{
    if (mBrain != NULL)
    {
        mBrain->popAdjust(popVec);
    }
}

void SimObj::emptyMessageList()
{
    for (list<Message*>::iterator iterMessage = mMessageList.begin();
                iterMessage != mMessageList.end();
                iterMessage++)
    {
        delete (*iterMessage);
    }
    mMessageList.clear();
}

void SimObj::printDebug()
{
    mTableSet->printDebug();

    if (mBrain != NULL)
    {
        mBrain->printDebug();
    }
}

const char SimObj::mClassName[] = "SimObj";

Orbit<SimObj>::MethodType SimObj::mMethods[] = {
	{"addSymbolTable", &SimObj::addSymbolTable},
	{"setSymbolName", &SimObj::setSymbolName},
	{"setBirthRadius", &SimObj::setBirthRadius},
    {"setBrain", &SimObj::setBrain},
    {"setKeepBodyOnHardDeath", &SimObj::setKeepBodyOnHardDeath},
    {"setKeepBodyOnExpirationDeath", &SimObj::setKeepBodyOnExpirationDeath},
    {0,0}
};

Orbit<SimObj>::NumberGlobalType SimObj::mNumberGlobals[] = {{0,0}};

int SimObj::addSymbolTable(lua_State* luaState)
{
    SymbolTable* table = (SymbolTable*)Orbit<SimObj>::pointer(luaState, 1);
    addSymbolTable(table);
    return 0;
}

int SimObj::setSymbolName(lua_State* luaState)
{
    string name = luaL_checkstring(luaState, 1);
    int table = luaL_checkint(luaState, 2);
    int pos = luaL_checkint(luaState, 3);
    setSymbolName(name, table, pos);
    return 0;
}

int SimObj::setBirthRadius(lua_State* luaState)
{
    float rad = luaL_checknumber(luaState, 1);
    setBirthRadius(rad);
    return 0;
}

int SimObj::setBrain(lua_State* luaState)
{
    Brain* brain = (Brain*)Orbit<SimObj>::pointer(luaState, 1);
    setBrain(brain);
    return 0;
}

int SimObj::setKeepBodyOnHardDeath(lua_State* luaState)
{
    bool val = luaL_checkbool(luaState, 1);
    setKeepBodyOnHardDeath(val);
    return 0;
}

int SimObj::setKeepBodyOnExpirationDeath(lua_State* luaState)
{
    bool val = luaL_checkbool(luaState, 1);
    setKeepBodyOnExpirationDeath(val);
    return 0;
}


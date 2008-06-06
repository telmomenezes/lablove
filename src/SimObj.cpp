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

llULINT SimObj::CURRENT_ID = 0;

SimObj::SimObj(lua_State* luaState)
{
    mID = CURRENT_ID++;

    mType = TYPE_OBJECT;

    mDeleted = false;

    mSpeciesID = 0;
    mCreationTime = 0;

    mInitialized = false;

    mFitness = 0.0f;
    mAgedFitness = 0.0f;
    mGroupFitness = 0.0f;

    mBirthRadius = 0.0f;
    mFitnessMeasure = 0;

    mBrain = NULL;
}

SimObj::SimObj(SimObj* obj)
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

    mBrain = NULL;

    mInitialized = false;

    mFitness = 0.0f;
    mAgedFitness = 0.0f;
    mGroupFitness = 0.0f;
    mFitnessMeasure = obj->mFitnessMeasure;

    mBirthRadius = obj->mBirthRadius;

    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = obj->mSymbolTables.begin();
        iterTables != obj->mSymbolTables.end();
        iterTables++)
    {
        mSymbolTables[(*iterTables).first] = new SymbolTable((*iterTables).second);
    }

    if (mType == TYPE_AGENT)
    {
        mBrain = obj->mBrain->clone();
        mBrain->setOwner(this);

        // Clean and expand symbol tables
        map<int, SymbolTable*>::iterator iterTables;
        for (iterTables = mSymbolTables.begin();
            iterTables != mSymbolTables.end();
            iterTables++)
        {
            SymbolTable* table = (*iterTables).second;

            if (table->isDynamic())
            {
                int tableID = (*iterTables).first;
                table->mUsedCount = 0;

                map<llULINT, Symbol*>::iterator iterSymbol = table->getSymbolMap()->begin();
                while (iterSymbol != table->getSymbolMap()->end())
                {
                    llULINT symbolID = (*iterSymbol).first;

                    Symbol* sym = (*iterSymbol).second;
                    iterSymbol++;

                    sym->mUsed = mBrain->symbolUsed(tableID, symbolID);

                    if (sym->mUsed)
                    {
                        table->mUsedCount++;
                    }

                    if ((!sym->mFixed) && (!sym->mUsed))
                    {
                        table->getSymbolMap()->erase(symbolID);
                        delete sym;
                    }
                }

                table->grow();
            }
        }
    }
}

SimObj::~SimObj()
{
    if (mBrain != NULL)
    {
        delete mBrain;
        mBrain = NULL;
    }

    map<int, SymbolTable*>::iterator iterTables;

    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        delete (*iterTables).second;
    }

    mSymbolTables.clear();

    mNamedSymbols.clear();

    for (list<Message*>::iterator iterMessage = mMessageList.begin();
            iterMessage != mMessageList.end();
            iterMessage++)
    {
        delete (*iterMessage);
    }

    mMessageList.clear();
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
    mSymbolTables[table->getID()] = table;
}

SymbolTable* SimObj::getSymbolTable(int id)
{
    if (mSymbolTables.count(id) == 0)
    {
        return NULL;
    }
    return mSymbolTables[id];
}

SymbolTable* SimObj::getSymbolTableByName(string name)
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

void SimObj::setSymbolName(string name, int table, llULINT id)
{
    SymbolPointer sp;
    sp.mTable = table;
    sp.mID = id;
    mNamedSymbols[name] = sp;
}

Symbol* SimObj::getSymbolByName(string name)
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

string SimObj::getSymbolName(int table, llULINT id)
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

string SimObj::getTableName(int table)
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

bool SimObj::getFieldValue(string fieldName, float& value)
{
    if (fieldName == "fitness")
    {
        value = mFitness;
        return true;
    }
    else if (fieldName == "group_fitness")
    {
        value = mGroupFitness;
        return true;
    }
    else if (fieldName.substr(0,  14) == "symtable_size_")
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

void SimObj::mutate()
{
    if (mBrain != NULL)
    {
        mBrain->mutate();
    }
}

void SimObj::recombine(SimObj* parent1, SimObj* parent2)
{
    Brain* newBrain;

    if (mType == TYPE_AGENT)
    {
        newBrain = parent1->mBrain->recombine(parent2->mBrain);

        newBrain->setSelectedSymbols(parent1);
        newBrain->setSelectedSymbols(parent2);
        setBrain(newBrain);
    }

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

SimObj* SimObj::recombine(SimObj* otherParent)
{
    SimObj* obj = clone();
    obj->recombine(this, otherParent);
    SimObj* child = obj->clone();

    delete obj;

    return child;
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
    map<int, SymbolTable*>::iterator iterTables;
    for (iterTables = mSymbolTables.begin();
        iterTables != mSymbolTables.end();
        iterTables++)
    {
        ((*iterTables).second)->printDebug();
    }

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
	{"setFitnessMeasure", &SimObj::setFitnessMeasure},
    {"setBrain", &SimObj::setBrain},
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

int SimObj::setFitnessMeasure(lua_State* luaState)
{
    int measure = luaL_checkint(luaState, 1);
    setFitnessMeasure(measure);
    return 0;
}

int SimObj::setBrain(lua_State* luaState)
{
    Brain* brain = (Brain*)Orbit<SimObj>::pointer(luaState, 1);
    setBrain(brain);
    return 0;
}


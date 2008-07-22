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

#include "Simulation.h"
#include "PopulationDynamics.h"
#include "Symbol.h"
#include "SymbolTable.h"
//#include "Gridbrain.h"

#include <math.h>
#include <stdexcept>

Simulation* Simulation::CURRENT = NULL;

Simulation::Simulation(lua_State* luaState)
{
    mSimulationTime = 0;
    mSelectedObject = NULL;
    mPopulationDynamics = NULL;

    mStop = false;

    mTimeLimit = 0;

    mDrawGraphics = true;
    mDrawThisCycle = true;

    CURRENT = this;
}

Simulation::~Simulation()
{
    list<SimObj*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        delete *iterObj;
    }
    mObjects.clear();
}

SimObj* Simulation::getObjectByID(gbULINT id)
{
    for (list<SimObj*>::iterator iterObj = mObjects.begin();
            iterObj != mObjects.end();
            iterObj++)
    {
        if ((*iterObj)->getID() == id)
        {
            return (*iterObj);
        }
    }

    return NULL;
}

void Simulation::run()
{
    mPopulationDynamics->init(this);

    while (!mStop)
    {
        cycle();
        if ((mTimeLimit != 0) && (mTimeLimit < mSimulationTime))
        {
            mStop = true;
        }
    }
}

void Simulation::cycle()
{
    //if ((mSimulationTime % 1000) == 0) Gridbrain::debugMutationsCount();

    bool draw = mDrawGraphics && mDrawThisCycle;

    list<SimObj*>::iterator iterObj;

    for (iterObj = mObjectsToKill.begin();
        iterObj != mObjectsToKill.end();
        iterObj++)
    {
        mPopulationDynamics->onOrganismDeath(*iterObj);
    }
    mObjectsToKill.clear();

    if (draw)
    {
        drawBeforeObjects();
    }

    onCycle();

    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimObj* obj = *iterObj;

        obj->process(); 
    }

    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimObj* obj = *iterObj;

        if (obj->mType == SimObj::TYPE_AGENT)
        {
            obj->perceive();
            obj->emptyMessageList();
            obj->compute();
            obj->act();
        }
    
        if (draw)
        {
            obj->draw();
        }
    }

    if (draw)
    {
        drawAfterObjects();
    }

    mPopulationDynamics->onCycle(mSimulationTime, art_getTime());

    mSimulationTime++;
}

void Simulation::addObject(SimObj* object, bool init)
{
    object->setCreationTime(mSimulationTime);
    object->onAdd();
    mObjects.push_back(object);
}

void Simulation::removeObject(SimObj* obj, bool deleteObj)
{
    if (mSelectedObject == obj)
    {
        mSelectedObject = NULL;
    }

    list<SimObj*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        if((*iterObj) == obj)
        {
            mObjects.erase(iterObj);
            if (deleteObj)
            {
                delete obj;
            }
            return;
        }
    }
}

void Simulation::killOrganism(SimObj* org)
{
    if (org->mDeleted)
    {
        return;
    }

    org->mDeleted = true;

    mObjectsToKill.push_back(org);
}

void Simulation::setSelectedObject(SimObj* object)
{
    mSelectedObject = object;
}

void Simulation::setSeedIndex(int index)
{
    gDistManager.setSeedIndex(index);
}

bool Simulation::getFieldValue(SimObj* obj, string fieldName, float& value)
{
    return obj->getFieldValue(fieldName, value);
}

float Simulation::calcSymbolsBinding(SimObj* origObj,
                                    SimObj* targetObj,
                                    int origSymTable,
                                    int targetSymTable,
                                    gbULINT origSymID,
                                    BindingType type)
{
    SymbolTable* targetTable = targetObj->getSymbolTable(targetSymTable);

    if (targetTable == NULL)
    {
        return 0.0f;
    }

    Symbol* targetSym = targetTable->getReferenceSymbol();

    if (targetSym == NULL)
    {
        return 0.0f;
    }

    return calcSymbolsBinding(origObj, origSymTable, origSymID, targetSym, type);
}

float Simulation::calcSymbolsBinding(SimObj* obj,
                                    int symTable,
                                    gbULINT symID,
                                    Symbol* symbol,
                                    BindingType type)
{
    SymbolTable* table = obj->getSymbolTable(symTable);

    if (table == NULL)
    {
        return 0.0f;
    }

    Symbol* sym = table->getSymbol(symID);

    if (sym == NULL)
    {
        return 0.0f;
    }

    float binding = 0.0f;
    
    switch (type)
    {
    case BINDING_EQUALS:
        if (sym->equals(symbol))
        {
            binding = 1.0f;
        }
        break;
    case BINDING_PROXIMITY:
        binding = sym->proximity(symbol);
        break;
    }

    return binding;
}

void Simulation::setTimeLimit(gbULINT limit)
{
    mTimeLimit = limit * 1000;
}

int Simulation::addObject(lua_State *luaState)
{
    SimObj* simObj = (SimObj*)Orbit<SimObj>::pointer(luaState, 1);
    addObject(simObj);
    return 0;
}

int Simulation::run(lua_State* luaState)
{
    run();
    return 0;
}

int Simulation::setPopulationDynamics(lua_State *luaState)
{
    PopulationDynamics* popDyn = (PopulationDynamics*)Orbit<SimObj>::pointer(luaState, 1);
    setPopulationDynamics(popDyn);
    return 0;
}

int Simulation::setSeedIndex(lua_State* luaState)
{
    int index = luaL_optint(luaState, 1, -1);
    setSeedIndex(index);
    return 0;
}

int Simulation::setTimeLimit(lua_State* luaState)
{
    int limit = luaL_checkint(luaState, 1);
    setTimeLimit(limit);
    return 0;
}


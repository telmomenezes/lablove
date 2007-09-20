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

#include "Simulation.h"
#include "Lab.h"
#include "functions.h"
#include "PopulationDynamics.h"

#include <math.h>

Simulation::Simulation()
{
    mSimulationTime = 0;
    mSelectedObject = NULL;
    mPopulationDynamics = NULL;
    mCellGrid = NULL;
}

Simulation::~Simulation()
{
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        delete *iterObj;
    }
    mObjects.clear();

    if (mCellGrid != NULL)
    {
        free(mCellGrid);
        mCellGrid = NULL;
    }
}

void Simulation::init()
{
    mPopulationDynamics->init();

    mLogo = Lab::getSingleton().getWindow()->createPNGLayer("lablove.png");
}

void Simulation::setWorldDimensions(float worldWidth,
                                    float worldLength,
                                    float cellSide)
{
    mWorldWidth = worldWidth;
    mWorldLength = worldLength;
    mCellSide = cellSide;
    mWorldCellWidth = (unsigned int)(ceilf(mWorldWidth / mCellSide));
    mWorldCellLength = (unsigned int)(ceilf(mWorldLength / mCellSide));

    unsigned int gridSize = mWorldCellWidth * mWorldCellLength;
    mCellGrid = (SimulationObject**)malloc(sizeof(SimulationObject*) * gridSize);

    for (unsigned int i = 0; i < gridSize; i++)
    {
        mCellGrid[i] = NULL;
    }

    
}

void Simulation::addObject(SimulationObject* object)
{
    mObjects.push_back(object);
}

void Simulation::removeObject(SimulationObject* object)
{
    SimulationObject* obj = (SimulationObject*)object;

    if (obj->mNextCellList != NULL)
    {
        obj->mNextCellList->mPrevCellList = obj->mPrevCellList;
    }

    if (obj->mPrevCellList == NULL)
    {
        int cellPos = obj->getCellPos();

        if(cellPos >= 0)
        {
            mCellGrid[cellPos] = obj->mNextCellList;
        }
    }
    else
    {
        obj->mPrevCellList->mNextCellList = obj->mNextCellList;
    }

    if (object->isSelected())
    {
        mSelectedObject = NULL;
    }

    bool stop = false;
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); (iterObj != mObjects.end()) && !stop; ++iterObj)
    {
        if((*iterObj) == object)
        {
            mObjects.erase(iterObj);
            delete object;
            return;
        }
    }
}

SimulationObject* Simulation::getObjectByScreenPos(int x, int y)
{
    return NULL;
}

void Simulation::moveView(float x, float y)
{
    mViewX += x;
    mViewY += y;
}

void Simulation::cycle()
{
    list<SimulationObject*>::iterator iterObj;

    for (iterObj = mObjectsToKill.begin();
        iterObj != mObjectsToKill.end();
        iterObj++)
    {
        mPopulationDynamics->onOrganismDeath(*iterObj);
    }
    mObjectsToKill.clear();

    processObjects();

    mPopulationDynamics->onCycle();

    drawBeforeObjects();
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        SimulationObject* obj = *iterObj;
        obj->draw(mViewX, mViewY);
    }

    Lab::getSingleton().getRootLayer()->setColor(255, 255, 255, 200);
    Lab::getSingleton().getRootLayer()->drawLayer(mLogo, 10, 10);

    Lab::getSingleton().getWindow()->update();

    mSimulationTime++;
}

void Simulation::setSelectedObject(SimulationObject* object)
{
    if (mSelectedObject != NULL)
    {
        mSelectedObject->setSelected(false);
    }
    mSelectedObject = object;
    if (mSelectedObject != NULL)
    {
        mSelectedObject->setSelected(true);
    }
}

void Simulation::killOrganism(SimulationObject* org)
{
    if (org->mDeleted)
    {
        return;
    }

    org->mDeleted = true;

    mObjectsToKill.push_back(org);
}

int Simulation::setPopulationDynamics(lua_State *luaState)
{
    PopulationDynamics* popDyn = (PopulationDynamics*)Orbit<Lab>::pointer(luaState, 1);
    setPopulationDynamics(popDyn);
    return 0;
}

int Simulation::setWorldDimensions(lua_State* luaState)
{
    int width = luaL_checkint(luaState, 1);
    int height = luaL_checkint(luaState, 2);
    int cellSide = luaL_checkint(luaState, 3);
    setWorldDimensions(width, height, cellSide);
    return 0;
}


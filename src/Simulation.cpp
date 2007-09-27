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
}

Simulation::~Simulation()
{
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
    {
        delete *iterObj;
    }
    mObjects.clear();
}

void Simulation::init()
{
    mPopulationDynamics->init();

    mLogo = Lab::getSingleton().getWindow()->createPNGLayer("lablove.png");
}

void Simulation::addObject(SimulationObject* object)
{
    mObjects.push_back(object);
}

void Simulation::removeObject(SimulationObject* obj)
{
    bool stop = false;
    list<SimulationObject*>::iterator iterObj;
    for (iterObj = mObjects.begin(); (iterObj != mObjects.end()) && !stop; ++iterObj)
    {
        if((*iterObj) == obj)
        {
            mObjects.erase(iterObj);
            delete obj;
            return;
        }
    }
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
        obj->draw();
    }

    Lab::getSingleton().getRootLayer2D()->setColor(255, 255, 255, 200);
    Lab::getSingleton().getRootLayer2D()->drawLayer(mLogo, 10, 10);

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


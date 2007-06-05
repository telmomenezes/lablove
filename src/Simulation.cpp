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
#include "LoveLab.h"
#include <math.h>
#include "functions.h"
#include "PopulationDynamics.h"

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
	LoveLab::getInstance().addKeyboardMouseHandler(this);
}

void Simulation::addObject(SimulationObject* object)
{
	mObjects.push_back(object);
}

void Simulation::removeObject(SimulationObject* object)
{
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

	mPopulationDynamics->onCycle();

#ifdef __LOVE_GRAPHICS
	drawBeforeObjects();
#endif

	for (iterObj = mObjects.begin(); iterObj != mObjects.end(); ++iterObj)
	{
		SimulationObject* obj = *iterObj;
		obj->onCycle();
#ifdef __LOVE_GRAPHICS
		obj->draw();
#endif
	}

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
        PopulationDynamics* popDyn = (PopulationDynamics*)Orbit<LoveLab>::pointer(luaState, 1);
        setPopulationDynamics(popDyn);
        return 0;
}


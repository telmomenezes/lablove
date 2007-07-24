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

#if !defined(__INCLUDE_SIMULATION_H)
#define __INCLUDE_SIMULATION_H

#include "SimulationObject.h"
#include "PopulationDynamics.h"
#include "InputHandler.h"

#include <list>

#include "Orbit.h"

class Simulation : public InputHandler
{
public:
	Simulation();
	virtual ~Simulation();

	virtual void init();

	virtual void drawBeforeObjects(){}

	virtual void addObject(SimulationObject* object);
	virtual void removeObject(SimulationObject* object);
	void cycle();
	virtual SimulationObject* getObjectByScreenPos(int x, int y)=0;
	void setSelectedObject(SimulationObject* object);
	SimulationObject* getSelectedObject(){return mSelectedObject;}
	void setPopulationDynamics(PopulationDynamics* popDyn){mPopulationDynamics = popDyn;}
	PopulationDynamics* getPopulationDynamics(){return mPopulationDynamics;}
	virtual void killOrganism(SimulationObject* org);
	unsigned long time(){return mSimulationTime;}

	int setPopulationDynamics(lua_State* luaState);

protected:
	std::list<SimulationObject*> mObjects;
	std::list<SimulationObject*> mObjectsToKill;

	unsigned long mSimulationTime;
	SimulationObject* mSelectedObject;
	PopulationDynamics* mPopulationDynamics;
};
#endif


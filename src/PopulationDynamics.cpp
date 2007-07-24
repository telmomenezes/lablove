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

#include "PopulationDynamics.h"
#include "Lab.h"

PopulationDynamics::PopulationDynamics()
{
	mHuman = NULL;
	mStatisticsTimeInterval = 1000;
}

PopulationDynamics::~PopulationDynamics()
{	
}

void PopulationDynamics::init()
{
	if (mHuman)
	{
		Lab::getSingleton().getSimulation()->addObject(mHuman);
	}
}

void PopulationDynamics::onCycle()
{
	// Dump statistics
	if ((Lab::getSingleton().getSimulation()->time() % mStatisticsTimeInterval) == 0)
	{
		for (std::list<Statistics*>::iterator iterStats = mStatistics.begin();
			iterStats != mStatistics.end();
			iterStats++)
		{
			(*iterStats)->dump();
		}
	}
}

void PopulationDynamics::onOrganismDeath(SimulationObject* org)
{
	// Update statistics
	for (std::list<Statistics*>::iterator iterStats = mStatistics.begin();
		iterStats != mStatistics.end();
		iterStats++)
	{
		(*iterStats)->process(org);
	}
}

void PopulationDynamics::setHuman(SimulationObject* human)
{
	mHuman = human;
	mHuman->mHuman = true;
}

void PopulationDynamics::addStatistics(Statistics* stats)
{
	stats->init();
	mStatistics.push_back(stats);
}

int PopulationDynamics::setHuman(lua_State* luaState)
{
	SimulationObject* obj = (SimulationObject*)Orbit<Lab>::pointer(luaState, 1);
        setHuman(obj);
        return 0;
}

int PopulationDynamics::addStatistics(lua_State* luaState)
{
        Statistics* stats = (Statistics*)Orbit<Lab>::pointer(luaState, 1);
        addStatistics(stats);
        return 0;
}

int PopulationDynamics::setStatisticsTimeInterval(lua_State* luaState)
{
	unsigned long interval = luaL_checkint(luaState, 1);
        setStatisticsTimeInterval(interval);
        return 0;
}


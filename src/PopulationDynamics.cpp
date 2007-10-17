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

PopulationDynamics::PopulationDynamics()
{
    mStatisticsTimeInterval = 1000;
}

PopulationDynamics::~PopulationDynamics()
{   
}

void PopulationDynamics::init(PopulationManager* popManager)
{
    mPopManager = popManager;
}

void PopulationDynamics::onCycle(unsigned long time, double realTime)
{
    // Dump statistics
    if ((time % mStatisticsTimeInterval) == 0)
    {
        for (list<Statistics*>::iterator iterStats = mStatistics.begin();
            iterStats != mStatistics.end();
            iterStats++)
        {
            (*iterStats)->dump(time, realTime);
        }
    }
}

void PopulationDynamics::onOrganismDeath(SimulationObject* org)
{
    // Update statistics
    for (list<Statistics*>::iterator iterStats = mStatistics.begin();
        iterStats != mStatistics.end();
        iterStats++)
    {
        (*iterStats)->process(org, mPopManager);
    }
}

void PopulationDynamics::addStatistics(Statistics* stats)
{
    stats->init();
    mStatistics.push_back(stats);
}

int PopulationDynamics::addStatistics(lua_State* luaState)
{
    Statistics* stats = (Statistics*)Orbit<SimulationObject>::pointer(luaState, 1);
    addStatistics(stats);
    return 0;
}

int PopulationDynamics::setStatisticsTimeInterval(lua_State* luaState)
{
    unsigned long interval = luaL_checkint(luaState, 1);
    setStatisticsTimeInterval(interval);
    return 0;
}


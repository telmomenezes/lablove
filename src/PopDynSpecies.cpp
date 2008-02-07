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

#include "PopDynSpecies.h"
#include "SimulationObject.h"

unsigned int PopDynSpecies::CURRENT_SPECIES_ID = 1;
mt_distribution* PopDynSpecies::mDistOrganism = gDistManager.getNewDistribution();

PopDynSpecies::PopDynSpecies(lua_State* luaState)
{
    mLogTimeInterval = 100 * 1000;
}

PopDynSpecies::~PopDynSpecies()
{
}

unsigned int PopDynSpecies::addSpecies(SimulationObject* org, unsigned int population)
{
    unsigned int speciesID = CURRENT_SPECIES_ID++;
    org->setSpeciesID(speciesID);

    SpeciesData species;
    mSpecies[speciesID] = species;
    mSpecies[speciesID].mBaseOrganism = org;
    mSpecies[speciesID].mPopulation = population;

    return speciesID;
}

void PopDynSpecies::addSampleLog(unsigned int speciesID, Log* log)
{
    log->init();
    mSpecies[speciesID].mSampleLogs.push_back(log);
}

void PopDynSpecies::addDeathLog(unsigned int speciesID, Log* log)
{
    log->init();
    mSpecies[speciesID].mDeathLogs.push_back(log);
}

void PopDynSpecies::onCycle(unsigned long time, double realTime)
{
    if ((time % mLogTimeInterval) == 0)
    {
        for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            // Dump death statistics
            for (list<Log*>::iterator iterLogs = (*iterSpecies).second.mDeathLogs.begin();
                iterLogs != (*iterSpecies).second.mDeathLogs.end();
                iterLogs++)
            {
                (*iterLogs)->dump(time, realTime);
            }

            // Process and dump sample statistics
            for (list<Log*>::iterator iterLogs = (*iterSpecies).second.mSampleLogs.begin();
                iterLogs != (*iterSpecies).second.mSampleLogs.end();
                iterLogs++)
            {
                for (vector<SimulationObject*>::iterator iterOrg = (*iterSpecies).second.mOrganismVector.begin();
                        iterOrg != (*iterSpecies).second.mOrganismVector.end();
                        iterOrg++)
                {
                    (*iterLogs)->process(*iterOrg, mPopManager);
                }
                (*iterLogs)->dump(time, realTime);
            }
        }
    }
}

void PopDynSpecies::onOrganismDeath(SimulationObject* org)
{
    int speciesID = org->getSpeciesID();

    if (speciesID == 0)
    {
        return;
    }
    SpeciesData* species = &(mSpecies[speciesID]);
            
    // Update death statistics
    for (list<Log*>::iterator iterLogs = species->mDeathLogs.begin();
            iterLogs != species->mDeathLogs.end();
            iterLogs++)
    {
        (*iterLogs)->process(org, mPopManager);
    }
}

int PopDynSpecies::addSampleLog(lua_State* luaState)
{
    unsigned int speciesID = luaL_checkint(luaState, 1);
    Log* log = (Log*)Orbit<SimulationObject>::pointer(luaState, 2);
    addSampleLog(speciesID, log);
    return 0;
}

int PopDynSpecies::addDeathLog(lua_State* luaState)
{
    unsigned int speciesID = luaL_checkint(luaState, 1);
    Log* log = (Log*)Orbit<SimulationObject>::pointer(luaState, 2);
    addDeathLog(speciesID, log);
    return 0;
}

int PopDynSpecies::setLogTimeInterval(lua_State* luaState)
{
    unsigned long interval = luaL_checkint(luaState, 1);
    setLogTimeInterval(interval);
    return 0;
}


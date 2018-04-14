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

#include "PopDynSEGA.h"
#include "SimObj.h"

unsigned int PopDynSEGA::CURRENT_SPECIES_ID = 1;

PopDynSEGA::PopDynSEGA(lua_State* luaState)
{
    mLogTimeInterval = 100 * 1000;
    mEvolutionStopTime = 0;
    mEvolutionOn = true;
}

PopDynSEGA::~PopDynSEGA()
{
    for (map<unsigned int, Species*>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        delete &((*iterSpecies).second);
    }
    mSpecies.clear();
}

void PopDynSEGA::init(Simulation* sim)
{
    PopulationDynamics::init(sim);

    unsigned int startBodyID = 1;

    for (map<unsigned int, Species*>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        (*iterSpecies).second->setSimulation(mSimulation);
        (*iterSpecies).second->init(startBodyID);
        startBodyID += (*iterSpecies).second->getPopulation();
    }
}

unsigned int PopDynSEGA::addSpecies(Species* species)
{
    unsigned int speciesID = CURRENT_SPECIES_ID++;
    species->setID(speciesID);

    mSpecies[speciesID] = species;

    return speciesID;
}

void PopDynSEGA::onOrganismDeath(SimObj* org)
{

    unsigned int speciesID = org->getSpeciesID();
    if (speciesID == 0)
    {
        return;
    }

    mSpecies[speciesID]->onOrganismDeath(org);
}

void PopDynSEGA::onCycle(gbULINT time, double realTime)
{
    if (mEvolutionOn
        && (mEvolutionStopTime > 0)
        && (time > mEvolutionStopTime))
    {
        mEvolutionOn = false;
        for (map<unsigned int, Species*>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            (*iterSpecies).second->setEvolution(false);
        }
    }

    if ((time % mLogTimeInterval) == 0)
    {
        for (map<unsigned int, Species*>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            (*iterSpecies).second->dumpStatistics(time, realTime, mSimulation);
        }
    }

    for (map<unsigned int, Species*>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {

        (*iterSpecies).second->onCycle(time);
    }
}

const char PopDynSEGA::mClassName[] = "PopDynSEGA";

Orbit<PopDynSEGA>::MethodType PopDynSEGA::mMethods[] = {
    {"setLogTimeInterval", &PopDynSEGA::setLogTimeInterval},
    {"addSpecies", &PopDynSEGA::addSpecies},
    {"setEvolutionStopTime", &PopDynSEGA::setEvolutionStopTime},
    {0,0}
};

Orbit<PopDynSEGA>::NumberGlobalType PopDynSEGA::mNumberGlobals[] = {{0,0}};

int PopDynSEGA::setLogTimeInterval(lua_State* luaState)
{
    gbULINT interval = luaL_checkinteger(luaState, 1);
    setLogTimeInterval(interval);
    return 0;
}

int PopDynSEGA::setEvolutionStopTime(lua_State* luaState)
{
    unsigned int time = luaL_checkinteger(luaState, 1);
    setEvolutionStopTime(time);
    return 0;
}

int PopDynSEGA::addSpecies(lua_State* luaState)
{
    Species* species = (Species*)Orbit<PopDynSEGA>::pointer(luaState, 1);
    unsigned int id = addSpecies(species);
    lua_pushinteger(luaState, id);
    return 1;
}


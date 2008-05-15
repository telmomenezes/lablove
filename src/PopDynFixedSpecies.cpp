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

#include "PopDynFixedSpecies.h"
#include "SimObj.h"

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* luaState)
{
    mTournamentSize = 2;
}

PopDynFixedSpecies::~PopDynFixedSpecies()
{
}

void PopDynFixedSpecies::init(Simulation* sim)
{
    PopulationDynamics::init(sim);

    for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        SpeciesData* species = &((*iterSpecies).second);
        for (unsigned int i = 0; i < species->mPopulation; i++)
        {
            SimObj* org = species->mBaseOrganism->clone();
            mSimulation->addObject(org);
            mSimulation->placeRandom(org);
            species->mOrganismVector.push_back(org);
        }
    }
}

unsigned int PopDynFixedSpecies::addSpecies(SimObj* org, unsigned int population)
{
    unsigned int speciesID = PopDynSpecies::addSpecies(org, population);

    return speciesID;
}

void PopDynFixedSpecies::onOrganismDeath(SimObj* org)
{
    PopDynSpecies::onOrganismDeath(org);

    int speciesID = org->getSpeciesID();

    if (speciesID == 0)
    {
        return;
    }
    SpeciesData* species = &(mSpecies[speciesID]);

    vector<SimObj*>::iterator iterOrg;

    int orgPos = -1;
    int curPos = 0;
    for (iterOrg = species->mOrganismVector.begin();
        (iterOrg != species->mOrganismVector.end()) && orgPos < 0;
        iterOrg++)
    {
        if ((*iterOrg) == org)
        {
            orgPos = curPos;
        }
        curPos++;
    }

    SimObj* newOrganism = NULL;

    // Tournament selection
    SimObj* bestOrganism = NULL;
    float bestFitness = 0.0f;
    
    for (unsigned int tournamentStep = 0; tournamentStep < mTournamentSize; tournamentStep++)
    {
        unsigned int organismNumber = mDistOrganism->iuniform(0, species->mPopulation);

        iterOrg = species->mOrganismVector.begin();
        for (unsigned int i = 0; i < organismNumber; i++)
        {
            iterOrg++;
        }

        if ((tournamentStep == 0) || ((*iterOrg)->mFitness > bestFitness))
        {
            bestOrganism = (*iterOrg);
            bestFitness = bestOrganism->mFitness;
        }
    }

    // Clone best and add to simulation
    newOrganism = bestOrganism->clone();
            
    // Mutate
    newOrganism->mutate();

    mSimulation->addObject(newOrganism);
    mSimulation->placeRandom(newOrganism);

    // Remove
    species->mOrganismVector[orgPos] = newOrganism;
    mSimulation->removeObject(org);
}

const char PopDynFixedSpecies::mClassName[] = "PopDynFixedSpecies";

Orbit<PopDynFixedSpecies>::MethodType PopDynFixedSpecies::mMethods[] = {
    {"setLogTimeInterval", &PopDynSpecies::setLogTimeInterval},
    {"addSpecies", &PopDynFixedSpecies::addSpecies},
    {"addSampleLog", &PopDynSpecies::addSampleLog},
    {"addDeathLog", &PopDynSpecies::addDeathLog},
    {"setTournamentSize", &PopDynFixedSpecies::setTournamentSize},
    {0,0}
};

Orbit<PopDynFixedSpecies>::NumberGlobalType PopDynFixedSpecies::mNumberGlobals[] = {{0,0}};

int PopDynFixedSpecies::addSpecies(lua_State* luaState)
{
    SimObj* obj = (SimObj*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
    int population = luaL_checkint(luaState, 2);
    unsigned int index = addSpecies(obj, population);
    lua_pushinteger(luaState, index);
    return 1;
}

int PopDynFixedSpecies::setTournamentSize(lua_State* luaState)
{
    unsigned int size = luaL_checkint(luaState, 1);
    setTournamentSize(size);
    return 0;
}


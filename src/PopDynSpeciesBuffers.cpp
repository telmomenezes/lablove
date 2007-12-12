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

#include "PopDynSpeciesBuffers.h"
#include "SimulationObject.h"

PopDynSpeciesBuffers::PopDynSpeciesBuffers(lua_State* luaState)
{
    mTournamentSize = 2;
}

PopDynSpeciesBuffers::~PopDynSpeciesBuffers()
{
}

void PopDynSpeciesBuffers::init(PopulationManager* popManager)
{
    PopulationDynamics::init(popManager);

    for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        for (unsigned int i = 0; i < (*iterSpecies).second.mPopulation; i++)
        {
            SimulationObject* org = (*iterSpecies).second.mBaseOrganism->clone(true);
            mPopManager->addObject(org);
            mPopManager->placeRandom(org);
            (*iterSpecies).second.mOrganismVector.push_back(org);
        }
    }
}

void PopDynSpeciesBuffers::onOrganismDeath(SimulationObject* org)
{
    PopDynSpecies::onOrganismDeath(org);

    SpeciesData* species = &(mSpecies[org->getSpeciesID()]);

    vector<SimulationObject*>::iterator iterOrg;

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

    SimulationObject* newOrganism = NULL;

    // Tournament selection
    SimulationObject* bestOrganism = NULL;
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

    mPopManager->addObject(newOrganism);
    mPopManager->placeRandom(newOrganism);

    // Remove
    species->mOrganismVector[orgPos] = newOrganism;
    mPopManager->removeObject(org);
}

const char PopDynSpeciesBuffers::mClassName[] = "PopDynSpeciesBuffers";

Orbit<PopDynSpeciesBuffers>::MethodType PopDynSpeciesBuffers::mMethods[] = {
    {"setLogTimeInterval", &PopDynSpecies::setLogTimeInterval},
    {"addSpecies", &PopDynSpecies::addSpecies},
    {"addSampleLog", &PopDynSpecies::addSampleLog},
    {"addDeathLog", &PopDynSpecies::addDeathLog},
    {"setTournamentSize", &PopDynSpeciesBuffers::setTournamentSize},
    {0,0}
};

Orbit<PopDynSpeciesBuffers>::NumberGlobalType PopDynSpeciesBuffers::mNumberGlobals[] = {{0,0}};

int PopDynSpeciesBuffers::setTournamentSize(lua_State* luaState)
{
    unsigned int size = luaL_checkint(luaState, 1);
    setTournamentSize(size);
    return 0;
}


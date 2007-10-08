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

#include "PopDynFixedSpecies.h"
#include "SimulationObject.h"
#include "Random.h"

unsigned int PopDynFixedSpecies::CURRENT_SPECIES_ID = 0;

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* luaState)
{
    mTournmentSize = 2;
}

PopDynFixedSpecies::~PopDynFixedSpecies()
{
}

unsigned int PopDynFixedSpecies::addSpecies(SimulationObject* org, long population)
{
    org->setSpeciesID(++CURRENT_SPECIES_ID);

    SpeciesData species;
    species.mBaseOrganism = org;
    species.mPopulation = population;

    mSpecies.push_back(species);

    return mSpecies.size() - 1;
}

void PopDynFixedSpecies::addSpeciesStatistics(unsigned int speciesIndex, Statistics* stats)
{
    stats->init();
    mSpecies[speciesIndex].mStatistics.push_back(stats);
}

void PopDynFixedSpecies::init(PopulationManager* popManager)
{
    PopulationDynamics::init(popManager);

    for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        (*iterSpecies).mBaseOrganism->initRandom();

        for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
        {
            SimulationObject* org = (*iterSpecies).mBaseOrganism->clone();
            org->initRandom();
            org->setEnergy(org->getInitialEnergy());
            mPopManager->addObject(org);
            mPopManager->placeRandom(org);
            (*iterSpecies).mOrganismVector.push_back(org);
        }
    }
}

void PopDynFixedSpecies::onCycle(unsigned long time, double realTime)
{
    PopulationDynamics::onCycle(time, realTime);

    if ((time % mStatisticsTimeInterval) == 0)
    {
        for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            // Dump statistics
            for (list<Statistics*>::iterator iterStats = (*iterSpecies).mStatistics.begin();
                iterStats != (*iterSpecies).mStatistics.end();
                iterStats++)
            {
                (*iterStats)->dump(time, realTime);
            }
        }
    }
}

void PopDynFixedSpecies::onOrganismDeath(SimulationObject* org)
{
    PopulationDynamics::onOrganismDeath(org);

    unsigned int speciesPos = 0;
    for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        if (org->getSpeciesID() == (*iterSpecies).mBaseOrganism->getSpeciesID())
        {
            // Update statistics
            for (list<Statistics*>::iterator iterStats = (*iterSpecies).mStatistics.begin();
                iterStats != (*iterSpecies).mStatistics.end();
                iterStats++)
            {
                (*iterStats)->process(org);
            }

            vector<SimulationObject*>::iterator iterOrg;

            int orgPos = -1;
            int curPos = 0;
            for (iterOrg = (*iterSpecies).mOrganismVector.begin();
                (iterOrg != (*iterSpecies).mOrganismVector.end()) && orgPos < 0;
                iterOrg++)
            {
                if ((*iterOrg) == org)
                {
                    orgPos = curPos;
                }
                curPos++;
            }

            SimulationObject* newOrganism = NULL;

            // Tournment selection of 2
            SimulationObject* bestOrganism = NULL;
            float bestFitness = 0.0f;

            for (unsigned int tournmentStep = 0; tournmentStep < mTournmentSize; tournmentStep++)
            {
                unsigned int organismNumber = Random::getUniformInt(0, (*iterSpecies).mPopulation - 1);

                iterOrg = (*iterSpecies).mOrganismVector.begin();
                for (unsigned int i = 0; i < organismNumber; i++)
                {
                    iterOrg++;
                }

                if ((tournmentStep == 0) || ((*iterOrg)->getEnergy() > bestFitness))
                {
                    bestOrganism = (*iterOrg);
                    bestFitness = bestOrganism->getEnergy();
                }
            }

            // Clone best and add to simulation
            newOrganism = bestOrganism->clone();
            newOrganism->setEnergy(newOrganism->getInitialEnergy());
            mPopManager->addObject(newOrganism);
            mPopManager->placeRandom(newOrganism);

            // Mutate
            newOrganism->mutate();

            // Remove
            (*iterSpecies).mOrganismVector[orgPos] = newOrganism;
            mPopManager->removeObject(org);

            return;
        }
    }
}

const char PopDynFixedSpecies::mClassName[] = "PopDynFixedSpecies";

Orbit<PopDynFixedSpecies>::MethodType PopDynFixedSpecies::mMethods[] = {
    {"addStatistics", &PopulationDynamics::addStatistics},
    {"addStatisticsTimeInterval", &PopulationDynamics::addStatistics},
    {"addSpecies", &PopDynFixedSpecies::addSpecies},
    {"addSpeciesStatistics", &PopDynFixedSpecies::addSpeciesStatistics},
    {"setTournmentSize", &PopDynFixedSpecies::setTournmentSize},
    {0,0}
};

Orbit<PopDynFixedSpecies>::NumberGlobalType PopDynFixedSpecies::mNumberGlobals[] = {{0,0}};

int PopDynFixedSpecies::addSpecies(lua_State* luaState)
{
    SimulationObject* obj = (SimulationObject*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
    int population = luaL_checkint(luaState, 2);
    unsigned int index = addSpecies(obj, population);
    lua_pushinteger(luaState, index);
    return 1;
}

int PopDynFixedSpecies::addSpeciesStatistics(lua_State* luaState)
{
    unsigned int speciesIndex = luaL_checkint(luaState, 1);
    Statistics* stats = (Statistics*)Orbit<PopDynFixedSpecies>::pointer(luaState, 2);
    addSpeciesStatistics(speciesIndex, stats);
    return 0;
}

int PopDynFixedSpecies::setTournmentSize(lua_State* luaState)
{
    unsigned int size = luaL_checkint(luaState, 1);
    setTournmentSize(size);
    return 0;
}


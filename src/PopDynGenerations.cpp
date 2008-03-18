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

#include "PopDynGenerations.h"
#include "SimulationObject.h"
#include "types.h"

unsigned int PopDynGenerations::CURRENT_SPECIES_ID = 0;
mt_distribution* PopDynGenerations::mDistOrganism = gDistManager.getNewDistribution();

PopDynGenerations::PopDynGenerations(lua_State* luaState)
{
    mGenerationTime = 0;
    mGeneration = 0;
}

PopDynGenerations::~PopDynGenerations()
{
}

unsigned int PopDynGenerations::addSpecies(SimulationObject* org, long population, bool isStatic)
{
    org->setSpeciesID(++CURRENT_SPECIES_ID);

    SpeciesData species;
    species.mBaseOrganism = org;
    species.mPopulation = population;
    species.mStatic = isStatic;

    mSpecies.push_back(species);

    return mSpecies.size() - 1;
}

void PopDynGenerations::addSpeciesLog(unsigned int speciesIndex, Log* log)
{
    log->init();
    mSpecies[speciesIndex].mLogs.push_back(log);
}

void PopDynGenerations::init(PopulationManager* popManager)
{
    PopulationDynamics::init(popManager);

    for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
        {
            SimulationObject* org = (*iterSpecies).mBaseOrganism->clone();
            mPopManager->addObject(org);
            mPopManager->placeRandom(org);
            (*iterSpecies).mOrganismList.push_back(org);
        }
    }
}

void PopDynGenerations::onCycle(llULINT time, double realTime)
{
    if (((time % mGenerationTime) == 0)
        && (time != 0))
    {
        for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            

            if (!(*iterSpecies).mStatic)
            {
                // Create next generation
                SimulationObject* newOrgList = NULL;
                SimulationObject* newOrganism = NULL;
                SimulationObject* prevNewOrganism = NULL;

                for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
                {
                    // Tournament selection of 2
                    SimulationObject* bestOrganism = NULL;
                    float bestFitness = 0.0f;

                    for (unsigned int tournmentStep = 0; tournmentStep < 2; tournmentStep++)
                    {
                        unsigned int organismNumber = mDistOrganism->iuniform(0, (*iterSpecies).mPopulation);

                        list<SimulationObject*>::iterator iterOrg = (*iterSpecies).mOrganismList.begin();
                        for (unsigned int i = 0; i < organismNumber; i++)
                        {
                            iterOrg++;
                        }
                        SimulationObject* org = *iterOrg;

                        if ((tournmentStep == 0) || (org->mFitness > bestFitness))
                        {
                            bestOrganism = org;
                            bestFitness = org->mFitness;
                        }
                    }

                    // Clone best and add to simulation
                    newOrganism = bestOrganism->clone();

                    // Mutate
                    newOrganism->mutate();

                    mPopManager->addObject(newOrganism);
                    mPopManager->placeRandom(newOrganism);

                    

                    (*iterSpecies).mOrganismList.push_back(newOrganism);
                }

                // Delete old population
                for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
                {
                    SimulationObject* org = (*iterSpecies).mOrganismList.front();

                    // Update statistics
                    for (list<Log*>::iterator iterLogs = (*iterSpecies).mLogs.begin();
                        iterLogs != (*iterSpecies).mLogs.end();
                        iterLogs++)
                    {
                        (*iterLogs)->process(org, mPopManager);
                    }

                    (*iterSpecies).mOrganismList.pop_front();
                    mPopManager->removeObject(org);
                }
            }

            // Dump logs
            for (list<Log*>::iterator iterLogs = (*iterSpecies).mLogs.begin();
                iterLogs != (*iterSpecies).mLogs.end();
                iterLogs++)
            {
                (*iterLogs)->dump(time, realTime);
            }
        }

        mGeneration++;
    }
}

void PopDynGenerations::onOrganismDeath(SimulationObject* org)
{
    vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
    while (iterSpecies != mSpecies.end())
    {
        if (org->getSpeciesID() == (*iterSpecies).mBaseOrganism->getSpeciesID())
        {
            if (!(*iterSpecies).mStatic)
            {
                return;
            }

            mPopManager->removeObject(org);

            SimulationObject* org = (*iterSpecies).mBaseOrganism->clone();
            mPopManager->addObject(org);
            mPopManager->placeRandom(org);

            return;
        }

        iterSpecies++;
    }
}

const char PopDynGenerations::mClassName[] = "PopDynGenerations";

Orbit<PopDynGenerations>::MethodType PopDynGenerations::mMethods[] = {
    {"addSpecies", &PopDynGenerations::addSpecies},
    {"addSpeciesLog", &PopDynGenerations::addSpeciesLog},
    {"setGenerationTime", &PopDynGenerations::setGenerationTime},
    {0,0}
};

Orbit<PopDynGenerations>::NumberGlobalType PopDynGenerations::mNumberGlobals[] = {{0,0}};

int PopDynGenerations::addSpecies(lua_State* luaState)
{
    SimulationObject* obj = (SimulationObject*)Orbit<PopDynGenerations>::pointer(luaState, 1);
    int population = luaL_checkint(luaState, 2);
    bool isStatic = luaL_checkbool(luaState, 3);
    unsigned int index = addSpecies(obj, population, isStatic);

    lua_pushinteger(luaState, index);
    return 1;
}

int PopDynGenerations::addSpeciesLog(lua_State* luaState)
{
    unsigned int speciesIndex = luaL_checkint(luaState, 1);
    Log* log = (Log*)Orbit<PopDynGenerations>::pointer(luaState, 2);
    addSpeciesLog(speciesIndex, log);
    return 0;
}

int PopDynGenerations::setGenerationTime(lua_State* luaState)
{
    int time = luaL_checkint(luaState, 1);
    setGenerationTime(time);
    return 0;
}


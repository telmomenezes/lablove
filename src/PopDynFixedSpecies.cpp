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

unsigned int PopDynFixedSpecies::CURRENT_SPECIES_ID = 0;
mt_distribution* PopDynFixedSpecies::mDistOrganism = gDistManager.getNewDistribution();

PopDynFixedSpecies::PopDynFixedSpecies(lua_State* luaState)
{
    mTournamentSize = 2;
    mLogTimeInterval = 100 * 1000;
}

PopDynFixedSpecies::~PopDynFixedSpecies()
{
}

unsigned int PopDynFixedSpecies::addSpecies(SimulationObject* org, long population, bool diversify)
{
    org->setSpeciesID(++CURRENT_SPECIES_ID);

    SpeciesData species;
    species.mBaseOrganism = org;
    species.mPopulation = population;
    species.mDiversify = diversify;

    mSpecies.push_back(species);

    return mSpecies.size() - 1;
}

void PopDynFixedSpecies::addSampleLog(unsigned int speciesIndex, Log* log)
{
    log->init();
    mSpecies[speciesIndex].mSampleLogs.push_back(log);
}

void PopDynFixedSpecies::addDeathLog(unsigned int speciesIndex, Log* log)
{
    log->init();
    mSpecies[speciesIndex].mDeathLogs.push_back(log);
}

void PopDynFixedSpecies::init(PopulationManager* popManager)
{
    PopulationDynamics::init(popManager);

    for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        for (unsigned int i = 0; i < (*iterSpecies).mPopulation; i++)
        {
            SimulationObject* org = (*iterSpecies).mBaseOrganism->clone((*iterSpecies).mDiversify);
            mPopManager->addObject(org);
            mPopManager->placeRandom(org);
            (*iterSpecies).mOrganismVector.push_back(org);
        }
    }
}

void PopDynFixedSpecies::onCycle(unsigned long time, double realTime)
{
    if ((time % mLogTimeInterval) == 0)
    {
        for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            // Dump death statistics
            for (list<Log*>::iterator iterLogs = (*iterSpecies).mDeathLogs.begin();
                iterLogs != (*iterSpecies).mDeathLogs.end();
                iterLogs++)
            {
                (*iterLogs)->dump(time, realTime);
            }

            // Process and dump sample statistics
            for (list<Log*>::iterator iterLogs = (*iterSpecies).mSampleLogs.begin();
                iterLogs != (*iterSpecies).mSampleLogs.end();
                iterLogs++)
            {
                for (vector<SimulationObject*>::iterator iterOrg = (*iterSpecies).mOrganismVector.begin();
                        iterOrg != (*iterSpecies).mOrganismVector.end();
                        iterOrg++)
                {
                    (*iterLogs)->process(*iterOrg, mPopManager);
                }
                (*iterLogs)->dump(time, realTime);
            }
        }
    }
}

void PopDynFixedSpecies::onOrganismDeath(SimulationObject* org)
{
    unsigned int speciesPos = 0;
    for (vector<SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        if (org->getSpeciesID() == (*iterSpecies).mBaseOrganism->getSpeciesID())
        {
            // Update death statistics
            for (list<Log*>::iterator iterLogs = (*iterSpecies).mDeathLogs.begin();
                iterLogs != (*iterSpecies).mDeathLogs.end();
                iterLogs++)
            {
                (*iterLogs)->process(org, mPopManager);
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

            // Tournament selection
            SimulationObject* bestOrganism = NULL;
            float bestFitness = 0.0f;

            for (unsigned int tournamentStep = 0; tournamentStep < mTournamentSize; tournamentStep++)
            {
                unsigned int organismNumber = mDistOrganism->iuniform(0, (*iterSpecies).mPopulation);

                iterOrg = (*iterSpecies).mOrganismVector.begin();
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
            (*iterSpecies).mOrganismVector[orgPos] = newOrganism;
            mPopManager->removeObject(org);

            return;
        }
    }
}

const char PopDynFixedSpecies::mClassName[] = "PopDynFixedSpecies";

Orbit<PopDynFixedSpecies>::MethodType PopDynFixedSpecies::mMethods[] = {
    {"setLogTimeInterval", &PopDynFixedSpecies::setLogTimeInterval},
    {"addSpecies", &PopDynFixedSpecies::addSpecies},
    {"addSampleLog", &PopDynFixedSpecies::addSampleLog},
    {"addDeathLog", &PopDynFixedSpecies::addDeathLog},
    {"setTournamentSize", &PopDynFixedSpecies::setTournamentSize},
    {0,0}
};

Orbit<PopDynFixedSpecies>::NumberGlobalType PopDynFixedSpecies::mNumberGlobals[] = {{0,0}};

int PopDynFixedSpecies::addSpecies(lua_State* luaState)
{
    SimulationObject* obj = (SimulationObject*)Orbit<PopDynFixedSpecies>::pointer(luaState, 1);
    int population = luaL_checkint(luaState, 2);
    bool diversify = true;
    if (lua_gettop(luaState) > 2)
    {        
        diversify = luaL_checkbool(luaState, 3);
    }
    unsigned int index = addSpecies(obj, population, diversify);
    lua_pushinteger(luaState, index);
    return 1;
}

int PopDynFixedSpecies::addSampleLog(lua_State* luaState)
{
    unsigned int speciesIndex = luaL_checkint(luaState, 1);
    Log* log = (Log*)Orbit<PopDynFixedSpecies>::pointer(luaState, 2);
    addSampleLog(speciesIndex, log);
    return 0;
}

int PopDynFixedSpecies::addDeathLog(lua_State* luaState)
{
    unsigned int speciesIndex = luaL_checkint(luaState, 1);
    Log* log = (Log*)Orbit<PopDynFixedSpecies>::pointer(luaState, 2);
    addDeathLog(speciesIndex, log);
    return 0;
}

int PopDynFixedSpecies::setTournamentSize(lua_State* luaState)
{
    unsigned int size = luaL_checkint(luaState, 1);
    setTournamentSize(size);
    return 0;
}

int PopDynFixedSpecies::setLogTimeInterval(lua_State* luaState)
{
    unsigned long interval = luaL_checkint(luaState, 1);
    setLogTimeInterval(interval);
    return 0;
}


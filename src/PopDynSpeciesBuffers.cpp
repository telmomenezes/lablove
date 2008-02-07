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

mt_distribution* PopDynSpeciesBuffers::mDistRecombine = gDistManager.getNewDistribution();

PopDynSpeciesBuffers::PopDynSpeciesBuffers(lua_State* luaState)
{
    mCompCount = 1;
    mFitnessAging = 0.0f;
    mRecombineProb = 0.0f;
}

PopDynSpeciesBuffers::~PopDynSpeciesBuffers()
{
    for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        SpeciesData* species = &((*iterSpecies).second);
        for (unsigned int i = 0; i < species->mBufferSize; i++)
        {
            delete species->mOrganismVector[i];
        }
    }
}

void PopDynSpeciesBuffers::init(PopulationManager* popManager)
{
    PopulationDynamics::init(popManager);

    for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        unsigned int speciesID = (*iterSpecies).first;
        SpeciesData* species = &((*iterSpecies).second);
        unsigned int subSpecSize = species->mBufferSize / species->mSubSpecies;
        species->mBufferSize = subSpecSize * species->mSubSpecies;
        for (unsigned int i = 0; i < species->mBufferSize; i++)
        {
            SimulationObject* org = species->mBaseOrganism->clone(species->mDiversify);
            org->setSubSpeciesID(i / subSpecSize);
            species->mOrganismVector.push_back(org);
        }
        for (unsigned int i = 0; i < species->mPopulation; i++)
        {
            xoverMutateSend(speciesID, true);
        }
    }
}

unsigned int PopDynSpeciesBuffers::addSpecies(SimulationObject* org,
                                                unsigned int population,
                                                unsigned int bufferSize,
                                                unsigned int subSpecies,
                                                bool diversify)
{
    unsigned int speciesID = CURRENT_SPECIES_ID++;
    org->setSpeciesID(speciesID);

    SpeciesData species;
    mSpecies[speciesID] = species;
    mSpecies[speciesID].mBaseOrganism = org;
    mSpecies[speciesID].mPopulation = population;
    mSpecies[speciesID].mBufferSize = bufferSize;
    mSpecies[speciesID].mSubSpecies = subSpecies;
    mSpecies[speciesID].mDiversify = diversify;

    return speciesID;
}

void PopDynSpeciesBuffers::onCycle(unsigned long time, double realTime)
{
    PopDynSpecies::onCycle(time, realTime);

    if ((time % 1000) == 0)
    {
        for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
            iterSpecies != mSpecies.end();
            iterSpecies++)
        {
            SpeciesData* species = &((*iterSpecies).second);

            if (species->mSubSpecies > 1)
            {
                unsigned int subSpecSize = species->mBufferSize / species->mSubSpecies;

                unsigned int subSpecies1 = mDistOrganism->iuniform(0, species->mSubSpecies);
                unsigned int subSpecies2 = mDistOrganism->iuniform(0, species->mSubSpecies - 1);

                if (subSpecies2 >= subSpecies1)
                {
                    subSpecies2++;
                }

                unsigned int rangeStart1 = subSpecies1 * subSpecSize;
                unsigned int rangeEnd1 = rangeStart1 + subSpecSize;
                unsigned int rangeStart2 = subSpecies2 * subSpecSize;
                unsigned int rangeEnd2 = rangeStart2 + subSpecSize;

                float avgFit1 = 0.0f;
                float avgFit2 = 0.0f;

                for (unsigned int i = rangeStart1; i < rangeEnd1; i++)
                {
                    SimulationObject* org = species->mOrganismVector[i];
                    avgFit1 += org->mFitness;
                }
                for (unsigned int i = rangeStart2; i < rangeEnd2; i++)
                {
                    SimulationObject* org = species->mOrganismVector[i];
                    avgFit2 += org->mFitness;
                }

                avgFit1 /= (float)subSpecSize;
                avgFit2 /= (float)subSpecSize;

                float fitRatio = avgFit1 / avgFit2;
                float mSubIsolation = 1.0f;
                float transfProb = 1.0f - (fitRatio * mSubIsolation);
                if ((avgFit1 == 0) || (avgFit2 == 0))
                {
                    transfProb = 0.0f;
                }


                if (transfProb > 0.0f)
                {
                    if (mDistOrganism->uniform(0, 1.0f) < transfProb)
                    {
                        printf("Sub species transference: %d -> %d (%f)\n", subSpecies1, subSpecies2, transfProb);
                        unsigned int orgNumber1 = mDistOrganism->iuniform(rangeStart1, rangeEnd1);
                        unsigned int orgNumber2 = mDistOrganism->iuniform(rangeStart2, rangeEnd2);
                        SimulationObject* org2 = species->mOrganismVector[orgNumber2];
                        SimulationObject* newOrg = org2->clone();
                        newOrg->mFitness = org2->mFitness;

                        delete species->mOrganismVector[orgNumber1];
                        species->mOrganismVector[orgNumber1] = newOrg;
                    }
                }
            }
        }
    }
}

void PopDynSpeciesBuffers::xoverMutateSend(unsigned int speciesID, bool init)
{
    SpeciesData* species = &(mSpecies[speciesID]);
    unsigned int organismNumber = mDistOrganism->iuniform(0, species->mBufferSize);
    SimulationObject* org = species->mOrganismVector[organismNumber];
   
    SimulationObject* newOrganism;

    float prob = mDistRecombine->uniform(0.0f, 1.0f);
    if (prob < mRecombineProb)
    {
        // Recombine
        organismNumber = mDistOrganism->iuniform(0, species->mBufferSize);
        SimulationObject* org2 = species->mOrganismVector[organismNumber];
        newOrganism = org->recombine(org2);
    }
    else
    {
        // Simple clone
        newOrganism = org->clone();
    }
            
    // Mutate
    newOrganism->mutate();

    mPopManager->addObject(newOrganism, init);
    mPopManager->placeRandom(newOrganism);
}

void PopDynSpeciesBuffers::onOrganismDeath(SimulationObject* org)
{
    PopDynSpecies::onOrganismDeath(org);

    unsigned int speciesID = org->getSpeciesID();
    unsigned int subSpecies = org->getSubSpeciesID();
    if (speciesID == 0)
    {
        return;
    }
    SpeciesData* species = &(mSpecies[speciesID]);

    unsigned int subSpecSize = species->mBufferSize / species->mSubSpecies;
    unsigned int rangeStart = subSpecies * subSpecSize;
    unsigned int rangeEnd = rangeStart + subSpecSize;

    bool deleteObj = true;
    bool keepComparing = true;

    // Buffer replacements
    for (unsigned int i = 0; (i < mCompCount) && keepComparing; i++)
    {
        unsigned int organismNumber = mDistOrganism->iuniform(rangeStart, rangeEnd);
        SimulationObject* org2 = species->mOrganismVector[organismNumber];

        if (org->mFitness >= org2->mFitness)
        {
            delete species->mOrganismVector[organismNumber];
            species->mOrganismVector[organismNumber] = org;
            deleteObj = false;
            keepComparing = false;
        }
        else
        {
            org2->mFitness *= (1.0f - mFitnessAging);
        }
    }

    // Remove
    mPopManager->removeObject(org, deleteObj);

    // Replace
    xoverMutateSend(speciesID);
}

const char PopDynSpeciesBuffers::mClassName[] = "PopDynSpeciesBuffers";

Orbit<PopDynSpeciesBuffers>::MethodType PopDynSpeciesBuffers::mMethods[] = {
    {"setLogTimeInterval", &PopDynSpecies::setLogTimeInterval},
    {"addSpecies", &PopDynSpeciesBuffers::addSpecies},
    {"addSampleLog", &PopDynSpecies::addSampleLog},
    {"addDeathLog", &PopDynSpecies::addDeathLog},
    {"setCompCount", &PopDynSpeciesBuffers::setCompCount},
    {"setFitnessAging", &PopDynSpeciesBuffers::setFitnessAging},
    {"setRecombineProb", &PopDynSpeciesBuffers::setRecombineProb},
    {0,0}
};

Orbit<PopDynSpeciesBuffers>::NumberGlobalType PopDynSpeciesBuffers::mNumberGlobals[] = {{0,0}};

int PopDynSpeciesBuffers::setCompCount(lua_State* luaState)
{
    unsigned int count = luaL_checkint(luaState, 1);
    setCompCount(count);
    return 0;
}

int PopDynSpeciesBuffers::setFitnessAging(lua_State* luaState)
{
    float aging = luaL_checknumber(luaState, 1);
    setFitnessAging(aging);
    return 0;
}

int PopDynSpeciesBuffers::setRecombineProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setRecombineProb(prob);
    return 0;
}

int PopDynSpeciesBuffers::addSpecies(lua_State* luaState)
{
    SimulationObject* obj = (SimulationObject*)Orbit<PopDynSpeciesBuffers>::pointer(luaState, 1);
    unsigned int population = luaL_checkint(luaState, 2);
    unsigned int bufferSize = luaL_checkint(luaState, 3);
    unsigned int subSpecies = luaL_optint(luaState, 4, 1);
    bool diversify = true;
    if (lua_gettop(luaState) > 4)
    {
        diversify = luaL_checkbool(luaState, 5);
    }
    unsigned int id = addSpecies(obj, population, bufferSize, subSpecies, diversify);
    lua_pushinteger(luaState, id);
    return 1;
}


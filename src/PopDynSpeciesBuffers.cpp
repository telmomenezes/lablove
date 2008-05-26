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

#include "PopDynSpeciesBuffers.h"
#include "SimObj.h"

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

void PopDynSpeciesBuffers::init(Simulation* sim)
{
    PopulationDynamics::init(sim);

    for (map<unsigned int, SpeciesData>::iterator iterSpecies = mSpecies.begin();
        iterSpecies != mSpecies.end();
        iterSpecies++)
    {
        unsigned int speciesID = (*iterSpecies).first;
        SpeciesData* species = &((*iterSpecies).second);
        for (unsigned int i = 0; i < species->mBufferSize; i++)
        {
            SimObj* org = species->mBaseOrganism->clone();
            species->mOrganismVector.push_back(org);
        }
        for (unsigned int i = 0; i < species->mPopulation; i++)
        {
            xoverMutateSend(speciesID, true);
        }
    }
}

unsigned int PopDynSpeciesBuffers::addSpecies(SimObj* org,
                                                unsigned int population,
                                                unsigned int bufferSize,
                                                bool queen)
{
    unsigned int speciesID = CURRENT_SPECIES_ID++;
    org->setSpeciesID(speciesID);

    SpeciesData species;
    mSpecies[speciesID] = species;
    mSpecies[speciesID].mBaseOrganism = org;
    mSpecies[speciesID].mPopulation = population;
    mSpecies[speciesID].mBufferSize = bufferSize;
    mSpecies[speciesID].mQueen = queen;
    mSpecies[speciesID].mCurrentQueen = 0;
    mSpecies[speciesID].mQueenState = population;
    mSpecies[speciesID].mSuperSister = NULL;

    return speciesID;
}

void PopDynSpeciesBuffers::xoverMutateSend(unsigned int speciesID, bool init, SimObj* nearObj)
{
    SpeciesData* species = &(mSpecies[speciesID]);
    
    unsigned int parent1;
    unsigned int parent2;

    if (species->mQueen)
    {
        if (species->mQueenState >= species->mPopulation)
        {
            species->mQueenState = 0;
            if (species->mCurrentQueen >= species->mBufferSize)
            {
                species->mCurrentQueen = 0;
            }

            parent1 = species->mCurrentQueen;
            parent2 = parent1 + 1;
            if (parent2 >= species->mBufferSize)
            {
                parent2 = 0;
            }
            if (species->mSuperSister != NULL)
            {
                delete species->mSuperSister;
                species->mSuperSister = NULL;
            }
        }
        species->mQueenState++;
    }
    else
    {
        parent1 = mDistOrganism->iuniform(0, species->mBufferSize);
        if (species->mBufferSize > 1)
        {
            parent1 = mDistOrganism->iuniform(0, species->mBufferSize - 1);
            if (parent2 >= parent1)
            {
                parent2++;
            }
        }
        else
        {
            parent2 = parent1;
        }
    }

    SimObj* newOrganism;

    if ((!species->mQueen)
        || (species->mSuperSister == NULL))
    {
        SimObj* org = species->mOrganismVector[parent1];

        float prob = mDistRecombine->uniform(0.0f, 1.0f);
        if ((prob < mRecombineProb) && mEvolutionOn)
        {
            // Recombine
            SimObj* org2 = species->mOrganismVector[parent2];

            /*unsigned int r = mDistRecombine->iuniform(0, 100);
            if (speciesID == 1)
            {
            Agent* agent1 = (Agent*)org;
            Agent* agent2 = (Agent*)org2;
            char fileName1[255];
            char fileName2[255];
            sprintf(fileName1, "%d_P1.svg", r);
            sprintf(fileName2, "%d_P2.svg", r);
            FILE* file1 = fopen(fileName1, "w");
            FILE* file2 = fopen(fileName2, "w");
            fprintf(file1, agent1->getBrain()->write(agent1, mSimulation).c_str());
            fprintf(file2, agent2->getBrain()->write(agent2, mSimulation).c_str());
            fflush(file1);
            fflush(file2);
            fclose(file1);
            fclose(file2);
            }*/

            newOrganism = org->recombine(org2);

            /*if (speciesID == 1)
            {
            char fileName3[255];
            sprintf(fileName3, "%d_CH.svg", r);
            FILE* file3 = fopen(fileName3, "w");
            Agent* agent3 = (Agent*)newOrganism;
            fprintf(file3, agent3->getBrain()->write(agent3, mSimulation).c_str());
            fflush(file3);
            fclose(file3);
            }*/
        }
        else
        {
            // Simple clone
            newOrganism = org->clone();
        }
            
        // Mutate
        if (mEvolutionOn)
        {
            newOrganism->mutate();
        }

        if (species->mQueen)
        {
            species->mSuperSister = newOrganism->clone();
        }
    }
    else
    {
        newOrganism = species->mSuperSister->clone();
    }

    mSimulation->addObject(newOrganism, init);
    
    if (nearObj == NULL)
    {
        mSimulation->placeRandom(newOrganism);
    }
    else
    {
        mSimulation->placeNear(newOrganism, nearObj);
    }
}

void PopDynSpeciesBuffers::onOrganismDeath(SimObj* org)
{
    PopDynSpecies::onOrganismDeath(org);

    unsigned int speciesID = org->getSpeciesID();
    if (speciesID == 0)
    {
        return;
    }
    SpeciesData* species = &(mSpecies[speciesID]);

    bool deleteObj = true;
    bool keepComparing = true;

    // Buffer replacements
    for (unsigned int i = 0; (i < mCompCount) && keepComparing; i++)
    {
        unsigned int organismNumber = mDistOrganism->iuniform(0, species->mBufferSize);
        SimObj* org2 = species->mOrganismVector[organismNumber];

        if (org->mFitness >= org2->mFitness)
        {
            delete species->mOrganismVector[organismNumber];

            species->mOrganismVector[organismNumber] = org;
            deleteObj = false;
            keepComparing = false;

            org->popAdjust(&species->mOrganismVector);
        }
        else
        {
            org2->mFitness *= (1.0f - mFitnessAging);
        }
    }

    // Remove
    mSimulation->removeObject(org, deleteObj);

    // Find organism to place near
    unsigned int refOrgNumber = mDistOrganism->iuniform(0, species->mPopulation - 1);

    list<SimObj*>* objects = mSimulation->getObjectList();

    list<SimObj*>::iterator iterObj = objects->begin();
    SimObj* refObj;
    unsigned int pos = 0;
    while (pos <= refOrgNumber)
    {
        refObj = (*iterObj);
        if (refObj->getSpeciesID() == speciesID)
        {
            pos++;
        }
        iterObj++;
    }

    // Replace
    xoverMutateSend(speciesID, false, refObj);
}

const char PopDynSpeciesBuffers::mClassName[] = "PopDynSpeciesBuffers";

Orbit<PopDynSpeciesBuffers>::MethodType PopDynSpeciesBuffers::mMethods[] = {
    {"setLogTimeInterval", &PopDynSpecies::setLogTimeInterval},
    {"addSpecies", &PopDynSpeciesBuffers::addSpecies},
    {"addSampleLog", &PopDynSpecies::addSampleLog},
    {"addDeathLog", &PopDynSpecies::addDeathLog},
    {"setEvolutionStopTime", &PopDynSpecies::setEvolutionStopTime},
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
    SimObj* obj = (SimObj*)Orbit<PopDynSpeciesBuffers>::pointer(luaState, 1);
    unsigned int population = luaL_checkint(luaState, 2);
    unsigned int bufferSize = luaL_checkint(luaState, 3);
    bool queen = false;
    if (lua_gettop(luaState) > 3)
    {
        queen = luaL_checkbool(luaState, 4);
    }

    unsigned int id = addSpecies(obj, population, bufferSize, queen);
    lua_pushinteger(luaState, id);
    return 1;
}


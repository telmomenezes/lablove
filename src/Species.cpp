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

#include "Species.h"
#include "Orbit.h"

mt_distribution* Species::mDistOrganism = gDistManager.getNewDistribution();
mt_distribution* Species::mDistRecombine = gDistManager.getNewDistribution();

Species::Species(lua_State* luaState)
{
    if (luaState != NULL)
    {
        mBaseOrganism = (SimObj*)Orbit<Species>::pointer(luaState, 1);
        mPopulation = luaL_checkint(luaState, 2);
    }
    else
    {
        mBaseOrganism = NULL;
        mPopulation = 0;
    }

    mBufferSize = 0;
    mKinFactor = 0.0f;
    mKinMutation = true;
    mCurrentQueen = 0;
    mQueenState = 0;
    mSuperSister = NULL;
    mFitnessAging = 0.0f;
    mRecombineProb = 0.0f;

    mEvolutionOn = true;
}

Species::~Species()
{
    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        delete mBuffer[i];
    }
    mBuffer.clear();
}

void Species::addGoal(int fitMeasure, unsigned int bufSize)
{
    mGoals.push_back(Goal(fitMeasure, bufSize));
    mBufferSize += bufSize;
}

void Species::init()
{
    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        SimObj* obj = mBaseOrganism->clone();
        mBuffer.push_back(obj);
    }

    mQueenState = (int)(((float)mPopulation) * mKinFactor) + 1;

    for (unsigned int i = 0; i < mPopulation; i++)
    {
        xoverMutateSend(i, true);
    }
}

void Species::setID(unsigned int id)
{
    if (mBaseOrganism != NULL)
    {
        mBaseOrganism->setSpeciesID(id);
    }
    mID = id;
}

void Species::addSampleLog(Log* log)
{
    log->init();
    mSampleLogs.push_back(log);
}

void Species::addDeathLog(Log* log)
{
    log->init();
    mDeathLogs.push_back(log);
}

void Species::dumpStatistics(llULINT time, double realTime, Simulation* sim)
{
    // Dump death statistics
    for (list<Log*>::iterator iterLogs = mDeathLogs.begin();
        iterLogs != mDeathLogs.end();
        iterLogs++)
    {
        (*iterLogs)->dump(time, realTime);
    }

    // Process and dump sample statistics
    for (list<Log*>::iterator iterLogs = mSampleLogs.begin();
        iterLogs != mSampleLogs.end();
        iterLogs++)
    {
        for (vector<SimObj*>::iterator iterOrg = mBuffer.begin();
                iterOrg != mBuffer.end();
                iterOrg++)
        {
            (*iterLogs)->process(*iterOrg, sim);
        }
        (*iterLogs)->dump(time, realTime);
    }
}

void Species::xoverMutateSend(int bodyID, bool init, SimObj* nearObj, SimObj* deadObj)
{
    unsigned int parent1;
    unsigned int parent2;

    if (mKinFactor > 0.0f)
    {
        //printf(">> current queen: %d state: %d\n", species->mCurrentQueen, species->mQueenState);
        if (mQueenState >= (int)(((float)mPopulation) * mKinFactor))
        {
            parent1 = mCurrentQueen;
            parent2 = mDistOrganism->iuniform(0, mBufferSize - 1);
            if (parent2 >= parent1)
            {
                parent2++;
            }
            //printf(">> parent1: %d parent2: %d\n", parent1, parent2);
            mCurrentQueen = parent2;

            mQueenState = 0;

            if (mSuperSister != NULL)
            {
                delete mSuperSister;
            }

            SimObj* org1 = mBuffer[parent1];
            SimObj* org2 = mBuffer[parent2];
            mSuperSister = org1->recombine(org2);

            if (mKinMutation)
            {
                mSuperSister->mutate(0.5f);
            }
            else
            {
                mSuperSister->mutate(1.0f);
            }
        }
        mQueenState++;
    }
    else
    {
        parent1 = mDistOrganism->iuniform(0, mBufferSize);
        if (mBufferSize > 1)
        {
            parent2 = mDistOrganism->iuniform(0, mBufferSize - 1);
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

    if (mKinFactor == 0.0f)
    {
        SimObj* org = mBuffer[parent1];

        float prob = mDistRecombine->uniform(0.0f, 1.0f);
        if ((prob < mRecombineProb) && mEvolutionOn)
        {
            // Recombine
            SimObj* org2 = mBuffer[parent2];

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
    }
    else
    {
        newOrganism = mSuperSister->clone();

        if (mKinMutation)
        {
            newOrganism->mutate(0.5f);
        }
    }

    // Set body ID
    newOrganism->setBodyID(bodyID);

    mSimulation->addObject(newOrganism, init);
   
    // Place object
    bool placeNear = false;
    bool replace = false;

    if ((newOrganism->getKeepBodyOnHardDeath())
        && (deadObj != NULL)
        && (deadObj->getDeathType() == SimObj::DEATH_HARD))
    {
        replace = true;
    }
    else if ((newOrganism->getKeepBodyOnExpirationDeath())
        && (deadObj != NULL)
        && (deadObj->getDeathType() == SimObj::DEATH_EXPIRED))
    {
        replace = true;
    }
    else if (nearObj != NULL)
    {
        placeNear = true;
    }


    if (placeNear)
    {
        mSimulation->placeNear(newOrganism, nearObj);
    }
    else if (replace)
    {
        mSimulation->replace(newOrganism, deadObj);
    }
    else
    {
        mSimulation->placeRandom(newOrganism);
    }
}

void Species::onOrganismDeath(SimObj* org)
{
    // Remove
    mSimulation->removeObject(org, false);

    int bodyID = org->getBodyID();

    // Update death statistics
    for (list<Log*>::iterator iterLogs = mDeathLogs.begin();
            iterLogs != mDeathLogs.end();
            iterLogs++)
    {
        (*iterLogs)->process(org, mSimulation);
    }

    // Challenges
    unsigned int startPos = 0;

    for (list<Goal>::iterator iterGoal = mGoals.begin();
        iterGoal != mGoals.end();
        iterGoal++)
    {   
        unsigned int objPos = mDistOrganism->iuniform(0, (*iterGoal).mSize);
        objPos += startPos;
        SimObj* org2 = mBuffer[objPos];

        float fitness = org->getFitness((*iterGoal).mFitnessMeasure);

        if (fitness >= org2->mFitness)
        {
            delete mBuffer[objPos];

            SimObj* newObj = org->clone();
            newObj->mFitness = fitness;

            mBuffer[objPos] = newObj;

            org->popAdjust(&mBuffer);
        }
        else
        {
            org2->mFitness *= (1.0f - mFitnessAging);
        }

        startPos += (*iterGoal).mSize;
    }

    // Find organism to place near
    unsigned int refOrgNumber = mDistOrganism->iuniform(0, mPopulation - 1);

    list<SimObj*>* objects = mSimulation->getObjectList();

    list<SimObj*>::iterator iterObj = objects->begin();
    SimObj* refObj;
    unsigned int pos = 0;
    while (pos <= refOrgNumber)
    {
        refObj = (*iterObj);
        if (refObj->getSpeciesID() == mID)
        {
            pos++;
        }
        iterObj++;
    }

    // Replace
    xoverMutateSend(bodyID, false, refObj, org);

    delete org;
}

const char Species::mClassName[] = "Species";

Orbit<Species>::MethodType Species::mMethods[] = {
    {"addGoal", &Species::addGoal},
    {"addSampleLog", &Species::addSampleLog},
    {"addDeathLog", &Species::addDeathLog},
    {"setKinFactor", &Species::setKinFactor},
    {"setKinMutation", &Species::setKinMutation},
    {"setFitnessAging", &Species::setFitnessAging},
    {"setRecombineProb", &Species::setRecombineProb},
    {0,0}
};

Orbit<Species>::NumberGlobalType Species::mNumberGlobals[] = {{0,0}};

int Species::addGoal(lua_State* luaState)
{
    int fitMeasure = luaL_checkint(luaState, 1);
    unsigned int bufSize = luaL_checkint(luaState, 2);
    addGoal(fitMeasure, bufSize);
    return 0;
}

int Species::addSampleLog(lua_State* luaState)
{
    Log* log = (Log*)Orbit<SimObj>::pointer(luaState, 1);
    addSampleLog(log);
    return 0;
}

int Species::addDeathLog(lua_State* luaState)
{
    Log* log = (Log*)Orbit<SimObj>::pointer(luaState, 1);
    addDeathLog(log);
    return 0;
}

int Species::setKinFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setKinFactor(factor);
    return 0;
}

int Species::setKinMutation(lua_State* luaState)
{
    bool value = luaL_checkbool(luaState, 1);
    setKinMutation(value);
    return 0;
}

int Species::setFitnessAging(lua_State* luaState)
{
    float aging = luaL_checknumber(luaState, 1);
    setFitnessAging(aging);
    return 0;
}

int Species::setRecombineProb(lua_State* luaState)
{
    float prob = luaL_checknumber(luaState, 1);
    setRecombineProb(prob);
    return 0;
}


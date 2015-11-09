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
#include "Random.h"

#include <sys/stat.h>


Species::Species(lua_State* luaState)
{
    if (luaState != NULL)
    {
        mBaseOrganism = (SimObj*)Orbit<Species>::pointer(luaState, 1);
        mPopulation = luaL_checkint(luaState, 2);
        mBufferSize = luaL_checkint(luaState, 3);
    }
    else
    {
        mBaseOrganism = NULL;
        mPopulation = 0;
        mBufferSize = 0;
    }

    mKinFactor = 0.0f;
    mKinMutation = true;
    mGroupFactor = 0.0f;
    mCurrentQueen = 0;
    mQueenState = 0;
    mSuperSister = NULL;
    mFitnessAging = 0.0f;
    mRecombineProb = 0.0f;

    mEvolutionOn = true;

    mLogInterval = 0;
    mFile = stdout;

    mBufferDumpDir = "";
}

Species::~Species()
{
    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        delete mBuffer[i];
    }
    mBuffer.clear();

    fclose(mFile);
}

void Species::addGoal(int fitMeasure, float factor)
{
    mGoals.push_back(Goal(fitMeasure, factor));
}

void Species::init(unsigned int startBodyID)
{
    //mBaseOrganism->printDebug();

    for (list<Goal>::iterator iterGoal = mGoals.begin();
        iterGoal != mGoals.end();
        iterGoal++)
    {
        mBaseOrganism->addFitness((*iterGoal).mFitnessMeasure);
    }

    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        SimObj* obj = mBaseOrganism->clone();
        mBuffer.push_back(obj);
        mBrainBuffer.push_back(obj->getBrain());
    }

    mQueenState = (int)(((float)mPopulation) * mKinFactor) + 1;

    for (unsigned int i = 0; i < mPopulation; i++)
    {
        xoverMutateSend(i + startBodyID, true);
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

void Species::dumpStatistics(gbULINT time, double realTime, Simulation* sim)
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

    if (mBufferDumpDir != "")
    {
        bufferDump(time, sim);
    }
}

void Species::bufferDump(gbULINT time, Simulation* sim)
{
    umask(0);
    mkdir(mBufferDumpDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    char fileName[255];
    sprintf(fileName, "%s/data.txt", mBufferDumpDir.c_str());
    FILE* globalFile = fopen(fileName, "w");
    if (globalFile != NULL)
    {
        fprintf(globalFile, "object_number");
        for (unsigned int f = 0; f < mGoals.size(); f++)
        {
            fprintf(globalFile, ", fitness_%d, group_fitness_%d, final_fitness_%d", f, f, f);
        }
        fprintf(globalFile, "\n");

        for (unsigned int i = 0; i < mBufferSize; i++)
        {
            SimObj* obj = mBuffer[i];
            sprintf(fileName, "%s/brain%d.svg", mBufferDumpDir.c_str(), i);
            FILE* file = fopen(fileName, "w");
            if (file != NULL)
            {
                fprintf(file, obj->getBrain()->write().c_str());
                fflush(file);
                fclose(file);
            }

            fprintf(globalFile, "%d", i);
            for (unsigned int f = 0; f < mGoals.size(); f++)
            {
                Fitness* fit = obj->getFitnessByIndex(f);
                fprintf(globalFile, ", %f, %f, %f", fit->mFitness, fit->mGroupFitness, fit->mFinalFitness);
            }
            fprintf(globalFile, "\n");
        }

        fflush(globalFile);
        fclose(globalFile);
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
            parent2 = gRandom.iuniform(0, mBufferSize - 1);
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
        parent1 = gRandom.iuniform(0, mBufferSize);
        if (mBufferSize > 1)
        {
            parent2 = gRandom.iuniform(0, mBufferSize - 1);
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

        float prob = gRandom.uniform(0.0f, 1.0f);
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

    // Clear fitnesses
    newOrganism->clearFitnesses();

    
    // Init group fitness
    list<SimObj*>* objList = mSimulation->getObjectList();
 
    for (list<SimObj*>::iterator iterObj = objList->begin();
            iterObj != objList->end();
            iterObj++)
    {
        SimObj* obj = (*iterObj);

        if (obj->getSpeciesID() == mID)
        {
            for (list<Goal>::iterator iterGoal = mGoals.begin();
                iterGoal != mGoals.end();
                iterGoal++)
            {
                Fitness* fit1 = newOrganism->getFitness((*iterGoal).mFitnessMeasure);
                obj->updateFitnesses();
                Fitness* fit2 = obj->getFitness((*iterGoal).mFitnessMeasure);
                fit1->mGroupFitness = fit1->mGroupFitness - fit2->mFitness;
            }
        }
    }

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

    // Update fitnesses
    org->updateFitnesses();

    // Group fitness calculations
    list<SimObj*>* objList = mSimulation->getObjectList();
 
    for (list<SimObj*>::iterator iterObj = objList->begin();
            iterObj != objList->end();
            iterObj++)
    {
        SimObj* obj = (*iterObj);

        if (obj->getSpeciesID() == mID)
        {
            if (obj->getID() != org->getID())
            {
                for (list<Goal>::iterator iterGoal = mGoals.begin();
                        iterGoal != mGoals.end();
                        iterGoal++)
                {
                    Fitness* fit1 = org->getFitness((*iterGoal).mFitnessMeasure);
                    obj->updateFitnesses();
                    Fitness* fit2 = obj->getFitness((*iterGoal).mFitnessMeasure);
                    fit1->mGroupFitness = fit1->mGroupFitness + fit2->mFitness;
                    fit2->mGroupFitness = fit2->mGroupFitness + fit1->mFitness;
                }
            }
        }
    }
    for (list<Goal>::iterator iterGoal = mGoals.begin();
            iterGoal != mGoals.end();
            iterGoal++)
    {
            Fitness* fit1 = org->getFitness((*iterGoal).mFitnessMeasure);
            fit1->mGroupFitness = fit1->mGroupFitness / ((float)mPopulation);
            fit1->mFinalFitness = (fit1->mFitness * (1.0f - mGroupFactor))
                                    + (fit1->mGroupFitness * mGroupFactor);
    }


    int bodyID = org->getBodyID();

    // Update death statistics
    for (list<Log*>::iterator iterLogs = mDeathLogs.begin();
            iterLogs != mDeathLogs.end();
            iterLogs++)
    {
        (*iterLogs)->process(org, mSimulation);
    }

    // Challenges
    unsigned int objPos = gRandom.iuniform(0, mBufferSize);
    SimObj* org2 = mBuffer[objPos];
    float totalFitness1 = 0.0f;
    float totalFitness2 = 0.0f;

    for (list<Goal>::iterator iterGoal = mGoals.begin();
        iterGoal != mGoals.end();
        iterGoal++)
    {   
        Fitness* fit1 = org->getFitness((*iterGoal).mFitnessMeasure);
        Fitness* fit2 = org2->getFitness((*iterGoal).mFitnessMeasure);
        float fitness1 = fit1->mFinalFitness;
        float fitness2 = fit2->mFinalFitness;

        if (fitness1 > (*iterGoal).mBestFitness)
        {
            (*iterGoal).mBestFitness = fitness1;
        }

        fitness1 /= (*iterGoal).mBestFitness;
        fitness2 /= (*iterGoal).mBestFitness;

        totalFitness1 += fitness1;
        totalFitness2 += fitness2;
    }

    if (totalFitness1 >= totalFitness2)
    {
        SimObj* newObj = org->clone();
        Gridbrain* mBrain = newObj->getBrain();

        if (mBrain != NULL)
        {
            mBrain->generateGenes(&mBrainBuffer);
            mBrainBuffer[objPos] = newObj->getBrain();
        }

        delete mBuffer[objPos];
        mBuffer[objPos] = newObj;
    }
    else
    {
        for (list<Goal>::iterator iterGoal = mGoals.begin();
                iterGoal != mGoals.end();
                iterGoal++)
        {   
            Fitness* fit2 = org2->getFitness((*iterGoal).mFitnessMeasure);
            fit2->mFinalFitness = fit2->mFinalFitness * (1.0f - mFitnessAging);
        }
    }

    // Find organism to place near
    SimObj* refObj = NULL;

    if (mPopulation > 1)
    {
        unsigned int refOrgNumber = gRandom.iuniform(0, mPopulation - 1);

        list<SimObj*>* objects = mSimulation->getObjectList();

        list<SimObj*>::iterator iterObj = objects->begin();
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
    }

    // Replace
    xoverMutateSend(bodyID, false, refObj, org);

    delete org;
}

void Species::onCycle(gbULINT time)
{
    if (mLogInterval == 0)
    {
        return;
    }

    if ((time % mLogInterval) == 0)
    {
        float comps = 0.0f;
        float totalDistance = 0.0f;

        for (unsigned int i = 0; i < mBufferSize; i++)
        {
            for (unsigned int j = i + 1; j < mBufferSize; j++)
            {
                SimObj* obj1 = mBuffer[i];
                SimObj* obj2 = mBuffer[j];
                totalDistance += obj1->getBrain()->getDistance(obj2->getBrain());
                comps += 1.0f;
            }
        }

        float avgDistance = totalDistance / comps;

        fprintf(mFile, "%d, ", time / 1000);
        fprintf(mFile, "%f\n", avgDistance);
        fflush(mFile);
    }
}

void Species::setLog(string filePath, unsigned int interval)
{
    mFile = fopen(filePath.c_str(), "w");
    mLogInterval = interval * 1000;

    fprintf(mFile, "time, avg_brain_distance\n");
    fflush(mFile);
}

const char Species::mClassName[] = "Species";

Orbit<Species>::MethodType Species::mMethods[] = {
    {"addGoal", &Species::addGoal},
    {"addSampleLog", &Species::addSampleLog},
    {"addDeathLog", &Species::addDeathLog},
    {"setKinFactor", &Species::setKinFactor},
    {"setKinMutation", &Species::setKinMutation},
    {"setGroupFactor", &Species::setGroupFactor},
    {"setFitnessAging", &Species::setFitnessAging},
    {"setRecombineProb", &Species::setRecombineProb},
    {"setLog", &Species::setLog},
    {"setBufferDump", &Species::setBufferDump},
    {0,0}
};

Orbit<Species>::NumberGlobalType Species::mNumberGlobals[] = {{0,0}};

int Species::addGoal(lua_State* luaState)
{
    int fitMeasure = luaL_checkint(luaState, 1);
    float factor = luaL_optnumber(luaState, 2, 1.0f);
    addGoal(fitMeasure, factor);
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

int Species::setGroupFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setGroupFactor(factor);
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

int Species::setLog(lua_State* luaState)
{
    string path = luaL_checkstring(luaState, 1);
    unsigned int interval = luaL_checkint(luaState, 2);
    setLog(path, interval);
    return 0;
}

int Species::setBufferDump(lua_State* luaState)
{
    string path = luaL_checkstring(luaState, 1);
    setBufferDump(path);
    return 0;
}


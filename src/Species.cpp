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
        mBufferSize = luaL_checkint(luaState, 3);
        
    }
    else
    {
        mBaseOrganism = NULL;
        mPopulation = 0;
        mBufferSize = 0;
    }

    mFitFactor = 1.0f;
    mKinFactor = 0.0f;
    mTeamFactor = 0.0f;
    mBodyFactor = 0.0f;
    mBodySamples = 0;
    mBodySmoothing = 0.0f;
    mCurrentQueen = 0;
    mQueenState = mPopulation;
    mSuperSister = NULL;
    mFitnessAging = 0.0f;
    mRecombineProb = 0.0f;

    mEvolutionOn = true;

    mBodyGroupLists = NULL;
}

Species::~Species()
{
    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        delete mOrganismVector[i];
    }
    mOrganismVector.clear();

    deleteBodyGroupLists(); 
}

void Species::init()
{
    for (unsigned int i = 0; i < mBufferSize; i++)
    {
        SimObj* org = mBaseOrganism->clone();
        mOrganismVector.push_back(org);
    }
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
        for (vector<SimObj*>::iterator iterOrg = mOrganismVector.begin();
                iterOrg != mOrganismVector.end();
                iterOrg++)
        {
            (*iterLogs)->process(*iterOrg, sim);
        }
        (*iterLogs)->dump(time, realTime);
    }
}

void Species::setBodyParams(float factor, unsigned int samples)
{
    mBodyFactor = factor;
    mBodySamples = samples;
    mBodySmoothing = 2.0f / (((float)mBodySamples) + 1.0f);

    if (samples > 0)
    {
        deleteBodyGroupLists(); 

        mBodyGroupLists = (list<SimObj*>**)malloc(samples * sizeof(list<SimObj*>*));
        for (unsigned int i = 0; i < samples; i++)
        {
            mBodyGroupLists[i] = new list<SimObj*>();
        }
    }
}

void Species::deleteBodyGroupLists()
{
    if (mBodyGroupLists != NULL)
    {
        for (unsigned int i = 0; i < mBodySamples; i++)
        {
            for (list<SimObj*>::iterator iterObj = mBodyGroupLists[i]->begin();
                iterObj != mBodyGroupLists[i]->end();
                iterObj++)
            {
                delete(*iterObj);
            }
            delete mBodyGroupLists[i];
        }   
    }
    free(mBodyGroupLists);
    mBodyGroupLists = NULL;
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

            SimObj* org1 = mOrganismVector[parent1];
            SimObj* org2 = mOrganismVector[parent2];
            mSuperSister = org1->recombine(org2);
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
        SimObj* org = mOrganismVector[parent1];

        float prob = mDistRecombine->uniform(0.0f, 1.0f);
        if ((prob < mRecombineProb) && mEvolutionOn)
        {
            // Recombine
            SimObj* org2 = mOrganismVector[parent2];

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

        // Mutate
        if (mEvolutionOn)
        {
            newOrganism->mutate();
        }
    }

    // Team fitness calculations
    list<SimObj*>* objList = mSimulation->getObjectList();

    for (list<SimObj*>::iterator iterObj = objList->begin();
            iterObj != objList->end();
            iterObj++)
    {
        SimObj* obj = (*iterObj);

        if (obj->getSpeciesID() == mID)
        {
            newOrganism->mTeamFitness -= obj->mFitness;
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

    // Team fitness calculations
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
                obj->mTeamFitness += org->mFitness;
                org->mTeamFitness += obj->mFitness;
            }
        }
    }
    org->mTeamFitness /= ((float)mPopulation);

    // Get candidate
    SimObj* candidate = getCandidate(org);

    bool deleteCandidate = false;

    if (candidate != NULL)
    {
        candidate->mFitness = (mFitFactor * candidate->mFitness)
            + (mTeamFactor * candidate->mTeamFitness)
            + (mBodyFactor * candidate->mBodyFitness);

        // Update death statistics
        for (list<Log*>::iterator iterLogs = mDeathLogs.begin();
                iterLogs != mDeathLogs.end();
                iterLogs++)
        {
            (*iterLogs)->process(candidate, mSimulation);
        }

        // Buffer replacements
        unsigned int organismNumber = mDistOrganism->iuniform(0, mBufferSize);
        SimObj* org2 = mOrganismVector[organismNumber];

        if (candidate->mFitness >= org2->mFitness)
        {
            delete mOrganismVector[organismNumber];

            mOrganismVector[organismNumber] = candidate;

            candidate->popAdjust(&mOrganismVector);
        }
        else
        {
            org2->mFitness *= (1.0f - mFitnessAging);
            deleteCandidate = true;
        }
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

    if (deleteCandidate)
    {
        delete candidate;
    }
}

SimObj* Species::getCandidate(SimObj* obj)
{
    if (mBodySamples == 0)
    {
        return obj;
    }

    list<SimObj*>* bodyList = mBodyGroupLists[obj->getBodyID()];

    bodyList->push_back(obj);

    if (bodyList->size() < (mBodySamples + 1))
    {
        return NULL;
    }

    SimObj* retObj = bodyList->front();
    bodyList->pop_front();

    float n = 0.0f;
    float d1 = 0.0f;
    float d2 = 0.0f;
    for (list<SimObj*>::iterator iterObj = bodyList->begin();
            iterObj != bodyList->end();
            iterObj++)
    {
        float fit = (*iterObj)->mFitness;
        float factor = powf((1 - mBodySmoothing), n);

        d1 += factor * fit;
        d2 += factor; 

        n += 1.0f;
    }

    retObj->mBodyFitness = d1 / d2;
    
    return retObj;
}

const char Species::mClassName[] = "Species";

Orbit<Species>::MethodType Species::mMethods[] = {
    {"addSampleLog", &Species::addSampleLog},
    {"addDeathLog", &Species::addDeathLog},
    {"setFitFactor", &Species::setFitFactor},
    {"setKinFactor", &Species::setKinFactor},
    {"setTeamFactor", &Species::setTeamFactor},
    {"setBodyParams", &Species::setBodyParams},
    {"setFitnessAging", &Species::setFitnessAging},
    {"setRecombineProb", &Species::setRecombineProb},
    {0,0}
};

Orbit<Species>::NumberGlobalType Species::mNumberGlobals[] = {{0,0}};

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

int Species::setFitFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setFitFactor(factor);
    return 0;
}

int Species::setKinFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setKinFactor(factor);
    return 0;
}

int Species::setTeamFactor(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    setTeamFactor(factor);
    return 0;
}

int Species::setBodyParams(lua_State* luaState)
{
    float factor = luaL_checknumber(luaState, 1);
    unsigned int samples = luaL_checkint(luaState, 2);
    setBodyParams(factor, samples);
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


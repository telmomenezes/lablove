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

#if !defined(__INCLUDE_SPECIES_H)
#define __INCLUDE_SPECIES_H

#include "SimObj.h"
#include "Log.h"
#include "Orbit.h"
#include "Goal.h"

#include <vector>

using std::vector;

class Species
{
public:
    Species(lua_State* luaState=NULL);
    virtual ~Species();

    void init();

    void onOrganismDeath(SimObj* org);

    void setID(unsigned int id);
    void setSimulation(Simulation* sim){mSimulation = sim;}
    
    void addGoal(int fitMeasure, unsigned int bufSize);
    void setFitnessAging(float aging){mFitnessAging = aging;}
    void setRecombineProb(float prob){mRecombineProb = prob;}

    void setKinFactor(float factor){mKinFactor = factor;}

    void addSampleLog(Log* log);
    void addDeathLog(Log* log);

    void dumpStatistics(llULINT time, double realTime, Simulation* sim);

    void setEvolution(bool evo){mEvolutionOn = evo;}

    static const char mClassName[];
    static Orbit<Species>::MethodType mMethods[];
    static Orbit<Species>::NumberGlobalType mNumberGlobals[];

    int addGoal(lua_State* luaState);
    int setFitnessAging(lua_State* luaState);
    int setRecombineProb(lua_State* luaState);
    int addSampleLog(lua_State* luaState);
    int addDeathLog(lua_State* luaState);
    int setKinFactor(lua_State* luaState);

protected:
    void xoverMutateSend(int bodyID, bool init=false, SimObj* nearObj=NULL, SimObj* deadObj=NULL);

    static mt_distribution* mDistOrganism;
    static mt_distribution* mDistRecombine;

    vector<SimObj*> mBuffer;
    list<Goal> mGoals; 
    SimObj* mBaseOrganism;
    list<Log*> mDeathLogs;
    list<Log*> mSampleLogs;
    unsigned int mPopulation;
    unsigned int mBufferSize;
    list<SimObj*>** mBodyGroupLists;
    float mKinFactor;
    unsigned int mCurrentQueen;
    unsigned int mQueenState;
    SimObj* mSuperSister;
    unsigned int mID;
    Simulation* mSimulation;
    float mFitnessAging;
    float mRecombineProb;

    bool mEvolutionOn;
};
#endif


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

#if !defined(__INCLUDE_POPDYN_FIXED_SPECIES_H)
#define __INCLUDE_POPDYN_FIXED_SPECIES_H

#include "PopulationDynamics.h"
#include "Statistics.h"
#include "RandDistManager.h"

#include <list>
#include <vector>

using std::list;
using std::vector;

class PopDynFixedSpecies : public PopulationDynamics
{

typedef struct
{
    SimulationObject* mBaseOrganism;
    vector<SimulationObject*> mOrganismVector;
    list<Statistics*> mDeathStatistics;
    list<Statistics*> mSampleStatistics;
    long mPopulation;
    bool mDiversify;
} SpeciesData;

public:
    static unsigned int CURRENT_SPECIES_ID;

    PopDynFixedSpecies(lua_State* luaState=NULL);
    virtual ~PopDynFixedSpecies();
    virtual void init(PopulationManager* popManager);
    virtual void onCycle(unsigned long time, double realTime);
    virtual void onOrganismDeath(SimulationObject* org);

    unsigned int addSpecies(SimulationObject* org, long population, bool diversify=true);

    void addSampleStatistics(unsigned int speciesIndex, Statistics* stats);
    void addDeathStatistics(unsigned int speciesIndex, Statistics* stats);
    void setStatisticsTimeInterval(unsigned long interval){mStatisticsTimeInterval = interval * 1000;}

    void setTournamentSize(unsigned int size){mTournamentSize = size;}

    static const char mClassName[];
    static Orbit<PopDynFixedSpecies>::MethodType mMethods[];
    static Orbit<PopDynFixedSpecies>::NumberGlobalType mNumberGlobals[];

    int addSpecies(lua_State* luaState);
    int addSampleStatistics(lua_State* luaState);
    int addDeathStatistics(lua_State* luaState);
    int setTournamentSize(lua_State* luaState);
    int setStatisticsTimeInterval(lua_State* luaState);

protected:
    static mt_distribution* mDistOrganism;

    vector<SpeciesData> mSpecies;
    unsigned int mTournamentSize;

    unsigned long mStatisticsTimeInterval;
};
#endif


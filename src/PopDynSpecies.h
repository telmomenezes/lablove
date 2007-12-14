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

#if !defined(__INCLUDE_POPDYN_SPECIES_H)
#define __INCLUDE_POPDYN_SPECIES_H

#include "PopulationDynamics.h"
#include "Log.h"
#include "RandDistManager.h"

#include <map>
#include <vector>
#include <list>

using std::map;
using std::vector;
using std::list;

class PopDynSpecies : public PopulationDynamics
{
protected:
typedef struct
{
    SimulationObject* mBaseOrganism;
    vector<SimulationObject*> mOrganismVector;
    list<Log*> mDeathLogs;
    list<Log*> mSampleLogs;
    unsigned int mPopulation;
    unsigned int mBufferSize;
    bool mDiversify;
} SpeciesData;

public:
    static unsigned int CURRENT_SPECIES_ID;

    PopDynSpecies(lua_State* luaState=NULL);
    virtual ~PopDynSpecies();
    virtual void onCycle(unsigned long time, double realTime);
    virtual void onOrganismDeath(SimulationObject* org);

    unsigned int addSpecies(SimulationObject* org, unsigned int population);

    void addSampleLog(unsigned int speciesIndex, Log* log);
    void addDeathLog(unsigned int speciesIndex, Log* log);
    void setLogTimeInterval(unsigned long interval){mLogTimeInterval = interval * 1000;}

    int addSampleLog(lua_State* luaState);
    int addDeathLog(lua_State* luaState);
    int setLogTimeInterval(lua_State* luaState);

protected:
    static mt_distribution* mDistOrganism;
    map<unsigned int, SpeciesData> mSpecies;
    unsigned long mLogTimeInterval;
};
#endif


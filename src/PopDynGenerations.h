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

#if !defined(__INCLUDE_POPDYN_GENERATIONS_H)
#define __INCLUDE_POPDYN_GENERATIONS_H

#include "PopulationDynamics.h"
#include "Log.h"
#include "RandDistManager.h"
#include "types.h"

#include <list>
#include <vector>

using std::list;
using std::vector;

class PopDynGenerations : public PopulationDynamics
{

typedef struct
{
    SimObj* mBaseOrganism;
    list<SimObj*> mOrganismList;
    list<Log*> mLogs;
    long mPopulation;
    bool mStatic;
} SpeciesData;

public:
    static unsigned int CURRENT_SPECIES_ID;

    PopDynGenerations(lua_State* luaState=NULL);
    virtual ~PopDynGenerations();
    virtual void init(Simulation* sim);
    virtual void onCycle(llULINT time, double realTime);
    virtual void onOrganismDeath(SimObj* org);

    unsigned int addSpecies(SimObj* org, long population, bool isStatic);
    void addSpeciesLog(unsigned int speciesIndex, Log* log);
    void setGenerationTime(unsigned int time){mGenerationTime = time;}

    static const char mClassName[];
    static Orbit<PopDynGenerations>::MethodType mMethods[];
    static Orbit<PopDynGenerations>::NumberGlobalType mNumberGlobals[];

    int addSpecies(lua_State* luaState);
    int addSpeciesLog(lua_State* luaState);
    int setGenerationTime(lua_State* luaState);

protected:
    vector<SpeciesData> mSpecies;

    static mt_distribution* mDistOrganism;

    unsigned int mGenerationTime;
    llULINT mGeneration;
};
#endif


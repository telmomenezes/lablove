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

#if !defined(__INCLUDE_POPDYN_GENERATIONS_H)
#define __INCLUDE_POPDYN_GENERATIONS_H

#include "PopulationDynamics.h"
#include "Statistics.h"

#include <list>
#include <vector>

using std::list;
using std::vector;

class PopDynGenerations : public PopulationDynamics
{

typedef struct
{
    SimulationObject* mBaseOrganism;
    list<SimulationObject*> mOrganismList;
    list<Statistics*> mStatistics;
    long mPopulation;
    bool mStatic;
} SpeciesData;

public:
    static unsigned int CURRENT_SPECIES_ID;

    PopDynGenerations(lua_State* luaState=NULL);
    virtual ~PopDynGenerations();
    virtual void init();
    virtual void onCycle();
    virtual void onOrganismDeath(SimulationObject* org);

    unsigned int addSpecies(SimulationObject* org, long population, bool isStatic);
    void addSpeciesStatistics(unsigned int speciesIndex, Statistics* stats);
    void setGenerationTime(unsigned int time){mGenerationTime = time;}

    static const char mClassName[];
    static Orbit<PopDynGenerations>::MethodType mMethods[];
    static Orbit<PopDynGenerations>::NumberGlobalType mNumberGlobals[];

    int addSpecies(lua_State* luaState);
    int addSpeciesStatistics(lua_State* luaState);
    int setGenerationTime(lua_State* luaState);

protected:

    vector<SpeciesData> mSpecies;

    unsigned int mGenerationTime;
    unsigned long mGeneration;
};
#endif

